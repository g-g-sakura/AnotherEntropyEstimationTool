////////////////////////////////////////////////////////////////////////////////
// reporter.cpp
//
//
//
// Copyright (c) 2021-2025 G. G. SAKURAI <g.garland823@gmail.com>
//
////////////////////////////////////////////////////////////////////////////////
#include "reporter.h"
#include <boost/program_options.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>
#include "boost/date_time/posix_time/posix_time.hpp"
#include "EntEstimateLib/support/enumerateAlphabet.h"
#include <boost/filesystem/fstream.hpp>
#include <vector>
#include <sstream>
#include <Windows.h>
#include <boost/version.hpp>
#include "calcMessageDigest.h"
#include <EntEstimateLib/support/LaTeX.h>

namespace bs_fs = boost::filesystem;

namespace ns_consts = entropy_estimator_lib::constants;
namespace ns_dt = entropy_estimator_lib::data_types;
namespace ns_spt = entropy_estimator_lib::support;

// -------------------------------------------------------------------------- //
/// <summary>
/// </summary>
/// <remarks>
/// </remarks>
/// <param name="i_refData">
/// </param>
/// <returns>
/// </returns>
/// <precondition>
/// </precondition>
/// <postcondition>
/// </postcondition>
// -------------------------------------------------------------------------- //
void showHeadSamples(const ns_dt::t_data_for_estimator& i_refData)
{
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    const int total_length = i_refData.p_bzInputS->length(blitz::firstDim);
    int num_bytes = 64;
    if (total_length < 64)
    {
        num_bytes = total_length;
    }
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    std::cout << "\n";
    std::cout << "The following are the first " << num_bytes << "-byte samples, in hexadecimal, of the specified file for confirmation:" << "\n";
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    std::cout.setf(std::ios::hex, std::ios::basefield);
    const char chFillSaved = std::cout.fill('0');
    for (int j = 0; j < num_bytes; ++j)
    {
        std::cout << std::setw(2) << static_cast<int>((*i_refData.p_bzInputS)(j)) << ", ";
        if ((j != 0) && (15 == j % 16))
        {
            std::cout << "\n";
        }
    }
    std::cout.fill(chFillSaved);
    std::cout.setf(std::ios::dec, std::ios::basefield);
}
// -------------------------------------------------------------------------- //
/// <summary>
/// </summary>
/// <remarks>
/// </remarks>
/// <param name="i_refData">
/// </param>
/// <returns>
/// </returns>
/// <precondition>
/// </precondition>
/// <postcondition>
/// </postcondition>
// -------------------------------------------------------------------------- //
void showTailSamples(const ns_dt::t_data_for_estimator& i_refData)
{
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    const int total_length = i_refData.p_bzInputS->length(blitz::firstDim);
    int offset = total_length;
    int num_bytes = 64;
    if (64 <= total_length)
    {
        offset = total_length - 64;
    }
    else
    {
        offset = 0;
        num_bytes = total_length;
    }
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    std::cout << "\n";
    std::cout << "The following are the last " << num_bytes << "-byte samples, in hexadecimal, of the specified file for confirmation:" << "\n";
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    std::cout.setf(std::ios::hex, std::ios::basefield);
    const char chFillSaved = std::cout.fill('0');
    for (int j = 0; j < num_bytes; ++j)
    {
        std::cout << std::setw(2) << static_cast<int>((*i_refData.p_bzInputS)(offset + j)) << ", ";
        if ((j != 0) && (15 == j % 16))
        {
            std::cout << "\n";
        }
    }
    std::cout.fill(chFillSaved);
    std::cout.setf(std::ios::dec, std::ios::basefield);
}


struct MinEntropy
{
    double  value;

    ns_consts::EnmNonIIDTrack   estimator_info;
    ns_consts::EnmSampleInterpretation  sample_interpret;
    unsigned int     bits_per_sample;
    bool    bIsSolutionFound;

    MinEntropy() : value(0.0), estimator_info(ns_consts::EnmNonIIDTrack::EstimatorMostCommonValue), sample_interpret(ns_consts::EnmSampleInterpretation::ELiteralNonBinary), bits_per_sample(8), bIsSolutionFound(true){}

    MinEntropy(double val, ns_consts::EnmNonIIDTrack i_est, ns_consts::EnmSampleInterpretation i_si, unsigned int i_bits, bool i_bIsSolutionFound)
        : value(val), estimator_info(i_est), sample_interpret(i_si), bits_per_sample(i_bits), bIsSolutionFound(i_bIsSolutionFound){}

    std::wstring getUnit() const
    {
        std::wstring str_ret(L"/ ");
        str_ret += std::to_wstring(this->bits_per_sample);
        str_ret += L"-bit";

        return  str_ret;
    }
};

// -------------------------------------------------------------------------- //
/// <summary>
/// </summary>
/// <remarks>
/// </remarks>
/// <param name="i_est">
/// </param>
/// <returns>
/// </returns>
/// <precondition>
/// </precondition>
/// <postcondition>
/// </postcondition>
// -------------------------------------------------------------------------- //
std::wstring getEstimatorInfo(ns_consts::EnmNonIIDTrack i_est)
{
    std::wstring str_ret;
    switch (i_est)
    {
    case ns_consts::EnmNonIIDTrack::EstimatorMostCommonValue:
        str_ret = std::wstring(L"The Most Common Value Estimate (NIST SP 800-90B Section 6.3.1)");
        break;
    case ns_consts::EnmNonIIDTrack::EstimatorCollision:
        str_ret = std::wstring(L"The Collision Estimate (NIST SP 800-90B Section 6.3.2)");
        break;
    case ns_consts::EnmNonIIDTrack::EstimatorMarkov:
        str_ret = std::wstring(L"The Markov Estimate (NIST SP 800-90B Section 6.3.3)");
        break;
    case ns_consts::EnmNonIIDTrack::EstimatorCompression:
        str_ret = std::wstring(L"The Compression Estimate (NIST SP 800-90B Section 6.3.4)");
        break;
    case ns_consts::EnmNonIIDTrack::EstimatorTTuple:
        str_ret = std::wstring(L"The t-Tuple Estimate (NIST SP 800-90B Section 6.3.5)");
        break;
    case ns_consts::EnmNonIIDTrack::EstimatorLRS:
        str_ret = std::wstring(L"The Longest Repeated Substring (LRS) Estimate (NIST SP 800-90B Section 6.3.6)");
        break;
    case ns_consts::EnmNonIIDTrack::EstimatorMMCWPrediction:
        str_ret = std::wstring(L"The Multi Most Common in Window Prediction Estimate (NIST SP 800-90B Section 6.3.7)");
        break;
    case ns_consts::EnmNonIIDTrack::EstimatorLagPrediction:
        str_ret = std::wstring(L"The Lag Prediction Estimate (NIST SP 800-90B Section 6.3.8)");
        break;
    case ns_consts::EnmNonIIDTrack::EstimatorMultiMMCPrediction:
        str_ret = std::wstring(L"The MultiMMC Prediction Estimate (NIST SP 800-90B Section 6.3.9)");
        break;
    case ns_consts::EnmNonIIDTrack::EstimatorLZ78YPrediction:
        str_ret = std::wstring(L"The LZ78Y Estimate (NIST SP 800-90B Section 6.3.10)");
        break;
    default:
        break;
    }
    return  str_ret;
}

// -------------------------------------------------------------------------- //
/// <summary>
/// </summary>
/// <remarks>
/// </remarks>
/// <param name="i_si">
/// </param>
/// <returns>
/// </returns>
/// <precondition>
/// </precondition>
/// <postcondition>
/// </postcondition>
// -------------------------------------------------------------------------- //
std::wstring getSampleInterpretationMode(ns_consts::EnmSampleInterpretation i_si)
{
    std::wstring str_ret;
    switch (i_si)
    {
    case ns_consts::EnmSampleInterpretation::ELiteralNonBinary:
    case ns_consts::EnmSampleInterpretation::ELiteralButBinary:
        str_ret = std::wstring(L"literal");
        break;
    case ns_consts::EnmSampleInterpretation::EInterpretationBinary:
        str_ret = std::wstring(L"binary");
        break;
    default:
        break;
    }
    return  str_ret;
}

// -------------------------------------------------------------------------- //
/// <summary>
/// </summary>
/// <remarks>
/// </remarks>
/// <param name="i_refInfoReport">
/// </param>
/// <param name="i_refDataOriginal">
/// </param>
/// <param name="i_refDataBinary">
/// </param>
/// <returns>
/// </returns>
/// <precondition>
/// </precondition>
/// <postcondition>
/// </postcondition>
// -------------------------------------------------------------------------- //
ns_consts::EnmReturnStatus reportXMLNonBinary(const IDInfoForReport& i_refInfoReport,
    const ns_dt::t_data_for_estimator& i_refDataOriginal, const ns_dt::t_data_for_estimator& i_refDataBinary)
{
    ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorUnexpected;

    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    constexpr  ns_consts::EnmNonIIDTrack   enm_estimate_ids_non_binary[] = {
            ns_consts::EnmNonIIDTrack::EstimatorMostCommonValue,
            ns_consts::EnmNonIIDTrack::EstimatorCollision,
            ns_consts::EnmNonIIDTrack::EstimatorMarkov,
            ns_consts::EnmNonIIDTrack::EstimatorCompression,
            ns_consts::EnmNonIIDTrack::EstimatorTTuple,
            ns_consts::EnmNonIIDTrack::EstimatorLRS,
            ns_consts::EnmNonIIDTrack::EstimatorMMCWPrediction,
            ns_consts::EnmNonIIDTrack::EstimatorLagPrediction,
            ns_consts::EnmNonIIDTrack::EstimatorMultiMMCPrediction,
            ns_consts::EnmNonIIDTrack::EstimatorLZ78YPrediction
    };
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    ns_consts::EnmSampleInterpretation  enm_interp[2] =
    { ns_consts::EnmSampleInterpretation::ELiteralNonBinary,
        ns_consts::EnmSampleInterpretation::EInterpretationBinary };
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    if (nullptr == i_refInfoReport.info_source.p_path_to_entropy_input)
    {
        return  sts = ns_consts::EnmReturnStatus::ErrorNullPointer;
    }
    bs_fs::path the_report_path;
    sts = synthesizeReportPath(the_report_path, *i_refInfoReport.info_source.p_path_to_entropy_input);
    if (ns_consts::EnmReturnStatus::Success != sts)
    {
        return  sts;
    }
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    double  min_entropy_bitstring = 1.0;
    auto  min_entropy_literal = static_cast<double>(i_refDataOriginal.bits_per_sample);
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    boost::property_tree::wptree the_tree;
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    std::vector<MinEntropy> vec_me;
    vec_me.reserve(17);
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    for (int i = 0; i < 10; ++i)
    {
        for (int j = 0; j < 2; ++j)
        {
            switch (i)
            {
            case 1:
            case 2:
            case 3:
                if (0 == j)
                {
                    // skip literal
                    continue;
                }
                break;
            default:
                break;
            }
            // -------------------------------------------------------------------------- //
            // 
            // -------------------------------------------------------------------------- //
            const ns_dt::t_data_for_estimator* pData = nullptr;
            switch (j)
            {
            case 0:
                pData = &i_refDataOriginal;
                break;
            case 1:
                pData = &i_refDataBinary;
                break;
            default:
                break;
            }
            // -------------------------------------------------------------------------- //
            // 
            // -------------------------------------------------------------------------- //
            const double* p_min_entropy = nullptr;
            switch (i)
            {
            case 0:
                p_min_entropy = &(pData->t_6_3_1.t_common.min_entropy);
                break;
            case 1:
                p_min_entropy = &(pData->t_6_3_2.t_common.min_entropy);
                break;
            case 2:
                p_min_entropy = &(pData->t_6_3_3.t_common.min_entropy);
                break;
            case 3:
                p_min_entropy = &(pData->t_6_3_4.t_common.min_entropy);
                break;
            case 4:
                p_min_entropy = &(pData->t_6_3_5.t_common.min_entropy);
                break;
            case 5:
                p_min_entropy = &(pData->t_6_3_6.t_common.min_entropy);
                break;
            case 6:
                p_min_entropy = &(pData->t_6_3_7.t_common.min_entropy);
                break;
            case 7:
                p_min_entropy = &(pData->t_6_3_8.t_common.min_entropy);
                break;
            case 8:
                p_min_entropy = &(pData->t_6_3_9.t_common.min_entropy);
                break;
            case 9:
                p_min_entropy = &(pData->t_6_3_10.t_common.min_entropy);
                break;
            default:
                return  sts;
            }
            // -------------------------------------------------------------------------- //
            // 
            // -------------------------------------------------------------------------- //
            if (nullptr != p_min_entropy)
            {
                switch (j)
                {
                case 0:
                    if (*p_min_entropy < min_entropy_literal)
                    {
                        min_entropy_literal = *p_min_entropy;
                    }
                    break;
                case 1:
                    if (*p_min_entropy < min_entropy_bitstring)
                    {
                        min_entropy_bitstring = *p_min_entropy;
                    }
                    break;
                default:
                    break;
                }
            }
            // -------------------------------------------------------------------------- //
            // 
            // -------------------------------------------------------------------------- //
            bool    bIsSolutionFound = true;
            switch (i)
            {
            case 1:
                bIsSolutionFound = pData->t_6_3_2.bIsRootFound;
                break;
            case 4:
                bIsSolutionFound = pData->t_6_3_4.bIsRootFound;
                break;
            default:
                break;
            }

            vec_me.emplace_back(MinEntropy(*p_min_entropy, enm_estimate_ids_non_binary[i], enm_interp[j], pData->bits_per_sample, bIsSolutionFound));
        }
    }
    // -------------------------------------------------------------------------- //
    // <identification><source>
    // -------------------------------------------------------------------------- //
    {
        the_tree.add(L"entropy_report.identification.source.path", i_refInfoReport.info_source.p_path_to_entropy_input->wstring());

        struct tm newtime;

        errno_t err = localtime_s(&newtime, &(i_refInfoReport.info_source.tm_last_write_time));
        if (0 != err)
        {
            std::cerr << "localtime_s failed with code "<< err << std::endl;
            return  sts = ns_consts::EnmReturnStatus::ErrorInvalidData;
        }

        the_tree.add(L"entropy_report.identification.source.last_write_time", std::put_time(&newtime, L"%Y-%b-%d %H:%M:%S"));
        the_tree.add(L"entropy_report.identification.source.bits_per_sample", std::to_wstring(i_refDataOriginal.bits_per_sample));
        the_tree.add(L"entropy_report.identification.source.description", std::wstring(L" "));
    }
    // -------------------------------------------------------------------------- //
    // <identification><analysis_tool>
    // -------------------------------------------------------------------------- //
    if (nullptr != i_refInfoReport.info_analysis_tool.p_analyzer_name)
    {
        the_tree.add(L"entropy_report.identification.analysis_tool.name", *i_refInfoReport.info_analysis_tool.p_analyzer_name);
    }
    if (nullptr != i_refInfoReport.info_analysis_tool.p_analyzer_versioning)
    {
        the_tree.add(L"entropy_report.identification.analysis_tool.version", *i_refInfoReport.info_analysis_tool.p_analyzer_versioning);
    }
    // -------------------------------------------------------------------------- //
    // <identification><environment>
    // -------------------------------------------------------------------------- //
    if (nullptr != i_refInfoReport.info_env.p_hostname)
    {
        the_tree.add(L"entropy_report.identification.environment.hostname", *i_refInfoReport.info_env.p_hostname);
    }
    if (nullptr != i_refInfoReport.info_env.p_cpuinfo)
    {
        the_tree.add(L"entropy_report.identification.environment.cpuinfo", *i_refInfoReport.info_env.p_cpuinfo);
    }
    if (nullptr != i_refInfoReport.info_env.p_physicalmemory)
    {
        the_tree.add(L"entropy_report.identification.environment.physicalmemory", *i_refInfoReport.info_env.p_physicalmemory);
    }
    if (nullptr != i_refInfoReport.info_env.p_osname)
    {
        the_tree.add(L"entropy_report.identification.environment.os_name", *i_refInfoReport.info_env.p_osname);
    }
    if (nullptr != i_refInfoReport.info_env.p_osversion)
    {
        the_tree.add(L"entropy_report.identification.environment.os_version", *i_refInfoReport.info_env.p_osversion);
    }
    if (nullptr != i_refInfoReport.info_env.p_system_type)
    {
        the_tree.add(L"entropy_report.identification.environment.system_type", *i_refInfoReport.info_env.p_system_type);
    }
    if (nullptr != i_refInfoReport.info_env.p_username)
    {
        the_tree.add(L"entropy_report.identification.environment.username", *i_refInfoReport.info_env.p_username);
    }
    // -------------------------------------------------------------------------- //
    // <summary>
    // -------------------------------------------------------------------------- //
    {
        double  min_entropy_global = min_entropy_literal;
        if (static_cast<double>(i_refDataOriginal.bits_per_sample) * min_entropy_bitstring < min_entropy_literal)
        {
            min_entropy_global = static_cast<double>(i_refDataOriginal.bits_per_sample) * min_entropy_bitstring;
        }
        boost::property_tree::wptree& child = the_tree.add(L"entropy_report.summary.min_entropy", std::to_wstring(min_entropy_global));
        if (!vec_me.empty())
        {
            child.put(L"<xmlattr>.unit", vec_me[0].getUnit());
        }
    }
    // -------------------------------------------------------------------------- //
    // <results>
    // -------------------------------------------------------------------------- //
    BOOST_FOREACH(const MinEntropy & me_entry, vec_me) {
        boost::property_tree::wptree& child = the_tree.add(L"entropy_report.results.min_entropy_estimate", std::to_wstring(me_entry.value));
        child.put(L"<xmlattr>.estimator", getEstimatorInfo(me_entry.estimator_info));
        child.put(L"<xmlattr>.sample_interpretation", getSampleInterpretationMode(me_entry.sample_interpret));
        child.put(L"<xmlattr>.unit", me_entry.getUnit());
        switch (me_entry.estimator_info)
        {
        case ns_consts::EnmNonIIDTrack::EstimatorCollision:
        case ns_consts::EnmNonIIDTrack::EstimatorCompression:
            if (false == me_entry.bIsSolutionFound)
            {
                child.put(L"<xmlattr>.remark", "a solution was not found in step 7.");
            }
            break;
        default:
            break;
        }
    }
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    constexpr  int indent = 4;
    boost::property_tree::xml_parser::write_xml(the_report_path.string(), the_tree, std::locale(),
        boost::property_tree::xml_parser::xml_writer_make_settings<std::wstring>(L' ', indent));
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    return  sts = ns_consts::EnmReturnStatus::Success;
}


// -------------------------------------------------------------------------- //
/// <summary>
/// </summary>
/// <remarks>
/// </remarks>
/// <param name="i_refInfoReport">
/// </param>
/// <param name="i_refDataBinary">
/// </param>
/// <returns>
/// </returns>
/// <precondition>
/// </precondition>
/// <postcondition>
/// </postcondition>
// -------------------------------------------------------------------------- //
ns_consts::EnmReturnStatus reportXMLBinary(const IDInfoForReport& i_refInfoReport,
    const ns_dt::t_data_for_estimator& i_refDataBinary)
{
    ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorUnexpected;

    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    constexpr ns_consts::EnmNonIIDTrack   enm_estimate_ids_binary[] = {
            ns_consts::EnmNonIIDTrack::EstimatorMostCommonValue,
            ns_consts::EnmNonIIDTrack::EstimatorCollision,
            ns_consts::EnmNonIIDTrack::EstimatorMarkov,
            ns_consts::EnmNonIIDTrack::EstimatorCompression,
            ns_consts::EnmNonIIDTrack::EstimatorTTuple,
            ns_consts::EnmNonIIDTrack::EstimatorLRS,
            ns_consts::EnmNonIIDTrack::EstimatorMMCWPrediction,
            ns_consts::EnmNonIIDTrack::EstimatorLagPrediction,
            ns_consts::EnmNonIIDTrack::EstimatorMultiMMCPrediction,
            ns_consts::EnmNonIIDTrack::EstimatorLZ78YPrediction
    };
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    if (nullptr == i_refInfoReport.info_source.p_path_to_entropy_input)
    {
        return  sts = ns_consts::EnmReturnStatus::ErrorNullPointer;
    }
    bs_fs::path the_report_path;
    const ns_consts::EnmReturnStatus	stsSynthesize = synthesizeReportPath(the_report_path, *i_refInfoReport.info_source.p_path_to_entropy_input);
    if (ns_consts::EnmReturnStatus::Success != stsSynthesize)
    {
        return  sts = stsSynthesize;
    }
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    double  min_entropy_bitstring = 1.0;
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    boost::property_tree::wptree the_tree;
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    std::vector<MinEntropy> vec_me;
    vec_me.reserve(17);
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    for (int i = 0; i < 10; ++i)
    {
            // -------------------------------------------------------------------------- //
            // 
            // -------------------------------------------------------------------------- //
            const double* p_min_entropy = nullptr;
            switch (i)
            {
            case 0:
                p_min_entropy = &(i_refDataBinary.t_6_3_1.t_common.min_entropy);
                break;
            case 1:
                p_min_entropy = &(i_refDataBinary.t_6_3_2.t_common.min_entropy);
                break;
            case 2:
                p_min_entropy = &(i_refDataBinary.t_6_3_3.t_common.min_entropy);
                break;
            case 3:
                p_min_entropy = &(i_refDataBinary.t_6_3_4.t_common.min_entropy);
                break;
            case 4:
                p_min_entropy = &(i_refDataBinary.t_6_3_5.t_common.min_entropy);
                break;
            case 5:
                p_min_entropy = &(i_refDataBinary.t_6_3_6.t_common.min_entropy);
                break;
            case 6:
                p_min_entropy = &(i_refDataBinary.t_6_3_7.t_common.min_entropy);
                break;
            case 7:
                p_min_entropy = &(i_refDataBinary.t_6_3_8.t_common.min_entropy);
                break;
            case 8:
                p_min_entropy = &(i_refDataBinary.t_6_3_9.t_common.min_entropy);
                break;
            case 9:
                p_min_entropy = &(i_refDataBinary.t_6_3_10.t_common.min_entropy);
                break;
            default:
                return  sts;
            }
            // -------------------------------------------------------------------------- //
            // 
            // -------------------------------------------------------------------------- //
            bool    bIsSolutionFound = true;
            switch (i)
            {
            case 1:
                bIsSolutionFound = i_refDataBinary.t_6_3_2.bIsRootFound;
                break;
            case 4:
                bIsSolutionFound = i_refDataBinary.t_6_3_4.bIsRootFound;
                break;
            default:
                break;
            }

            vec_me.emplace_back(MinEntropy(*p_min_entropy, enm_estimate_ids_binary[i], ns_consts::EnmSampleInterpretation::ELiteralButBinary, i_refDataBinary.bits_per_sample, bIsSolutionFound));
            // -------------------------------------------------------------------------- //
            // 
            // -------------------------------------------------------------------------- //
            if (nullptr != p_min_entropy)
            {
                if (*p_min_entropy < min_entropy_bitstring)
                {
                    min_entropy_bitstring = *p_min_entropy;
                }
            }
    }
    // -------------------------------------------------------------------------- //
    // <identification><source>
    // -------------------------------------------------------------------------- //
    {
        the_tree.add(L"entropy_report.identification.source.path", i_refInfoReport.info_source.p_path_to_entropy_input->wstring());

        struct tm newtime;

        errno_t err = localtime_s(&newtime, &(i_refInfoReport.info_source.tm_last_write_time));
        if (0 != err)
        {
            std::cerr << "localtime_s failed with code " << err << std::endl;
            return  sts = ns_consts::EnmReturnStatus::ErrorInvalidData;
        }

        the_tree.add(L"entropy_report.identification.source.last_write_time", std::put_time(&newtime, L"%Y-%b-%d %H:%M:%S"));
        the_tree.add(L"entropy_report.identification.source.bits_per_sample", std::to_wstring(i_refDataBinary.bits_per_sample));
        the_tree.add(L"entropy_report.identification.source.description", std::wstring(L" "));
    }
    // -------------------------------------------------------------------------- //
    // <identification><analysis_tool>
    // -------------------------------------------------------------------------- //
    if (nullptr != i_refInfoReport.info_analysis_tool.p_analyzer_name)
    {
        the_tree.add(L"entropy_report.identification.analysis_tool.name", *i_refInfoReport.info_analysis_tool.p_analyzer_name);
    }
    if (nullptr != i_refInfoReport.info_analysis_tool.p_analyzer_versioning)
    {
        the_tree.add(L"entropy_report.identification.analysis_tool.version", *i_refInfoReport.info_analysis_tool.p_analyzer_versioning);
    }
    // -------------------------------------------------------------------------- //
    // <identification><environment>
    // -------------------------------------------------------------------------- //
    if (nullptr != i_refInfoReport.info_env.p_hostname)
    {
        the_tree.add(L"entropy_report.identification.environment.hostname", *i_refInfoReport.info_env.p_hostname);
    }
    if (nullptr != i_refInfoReport.info_env.p_cpuinfo)
    {
        the_tree.add(L"entropy_report.identification.environment.cpuinfo", *i_refInfoReport.info_env.p_cpuinfo);
    }
    if (nullptr != i_refInfoReport.info_env.p_physicalmemory)
    {
        the_tree.add(L"entropy_report.identification.environment.physicalmemory", *i_refInfoReport.info_env.p_physicalmemory);
    }
    if (nullptr != i_refInfoReport.info_env.p_osname)
    {
        the_tree.add(L"entropy_report.identification.environment.os_name", *i_refInfoReport.info_env.p_osname);
    }
    if (nullptr != i_refInfoReport.info_env.p_osversion)
    {
        the_tree.add(L"entropy_report.identification.environment.os_version", *i_refInfoReport.info_env.p_osversion);
    }
    if (nullptr != i_refInfoReport.info_env.p_system_type)
    {
        the_tree.add(L"entropy_report.identification.environment.system_type", *i_refInfoReport.info_env.p_system_type);
    }
    if (nullptr != i_refInfoReport.info_env.p_username)
    {
        the_tree.add(L"entropy_report.identification.environment.username", *i_refInfoReport.info_env.p_username);
    }
    // -------------------------------------------------------------------------- //
    // <summary>
    // -------------------------------------------------------------------------- //
    {
        boost::property_tree::wptree& child = the_tree.add(L"entropy_report.summary.min_entropy", std::to_wstring(min_entropy_bitstring));
        if (!vec_me.empty())
        {
            child.put(L"<xmlattr>.unit", vec_me[0].getUnit());
        }
    }
    // -------------------------------------------------------------------------- //
    // <results>
    // -------------------------------------------------------------------------- //
    BOOST_FOREACH(const MinEntropy & me_entry, vec_me) {
        boost::property_tree::wptree& child = the_tree.add(L"entropy_report.results.min_entropy_estimate", std::to_wstring(me_entry.value));
        child.put(L"<xmlattr>.estimator", getEstimatorInfo(me_entry.estimator_info));
        child.put(L"<xmlattr>.sample_interpretation", getSampleInterpretationMode(me_entry.sample_interpret));
        child.put(L"<xmlattr>.unit", me_entry.getUnit());
        switch (me_entry.estimator_info)
        {
        case ns_consts::EnmNonIIDTrack::EstimatorCollision:
        case ns_consts::EnmNonIIDTrack::EstimatorCompression:
            if (false == me_entry.bIsSolutionFound)
            {
                child.put(L"<xmlattr>.remark", "a solution was not found in step 7.");
            }
            break;
        default:
            break;
        }
    }
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    constexpr int indent = 4;
    boost::property_tree::xml_parser::write_xml(the_report_path.string(), the_tree, std::locale(),
        boost::property_tree::xml_parser::xml_writer_make_settings<std::wstring>(L' ', indent));
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    return  sts = ns_consts::EnmReturnStatus::Success;
}

// -------------------------------------------------------------------------- //
/// <summary>
/// </summary>
/// <remarks>
/// </remarks>
/// <param name="o_report_complete_path">
/// </param>
/// <param name="i_path_entropy_input">
/// </param>
/// <returns>
/// </returns>
/// <precondition>
/// </precondition>
/// <postcondition>
/// </postcondition>
// -------------------------------------------------------------------------- //
ns_consts::EnmReturnStatus synthesizeReportPath(bs_fs::path& o_report_complete_path,
    const bs_fs::path& i_path_entropy_input)
{
    ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorUnexpected;

    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    const std::string strBaseFileName("entropy_report_");
    const std::string strExtension(".xml");

    const auto pt =
        boost::posix_time::ptime(
            boost::posix_time::microsec_clock::local_time());

    const std::string strTimeInfo(boost::posix_time::to_iso_string(pt));
    std::string strFileName(strBaseFileName);
    strFileName += i_path_entropy_input.stem().string();
    strFileName += "_";
    strFileName += strTimeInfo.substr(0, 15);
    strFileName += strExtension;
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    o_report_complete_path = i_path_entropy_input.parent_path();
    o_report_complete_path /= strFileName;
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    return  sts = ns_consts::EnmReturnStatus::Success;
}

// -------------------------------------------------------------------------- //
/// <summary>
/// </summary>
/// <remarks>
/// </remarks>
/// <param name="o_report_complete_path">
/// </param>
/// <param name="i_path_entropy_input">
/// </param>
/// <returns>
/// </returns>
/// <precondition>
/// </precondition>
/// <postcondition>
/// </postcondition>
// -------------------------------------------------------------------------- //
ns_consts::EnmReturnStatus synthesizeReportPathTex(bs_fs::path& o_report_complete_path,
    const bs_fs::path& i_path_entropy_input)
{
    ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorUnexpected;

    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    const std::string strBaseFileName("entropy_report_");
    const std::string strExtension(".tex");

    const auto pt =
        boost::posix_time::ptime(
            boost::posix_time::microsec_clock::local_time());

    const std::string strTimeInfo(boost::posix_time::to_iso_string(pt));
    std::string strFileName(strBaseFileName);
    strFileName += i_path_entropy_input.stem().string();
    strFileName += "_";
    strFileName += strTimeInfo.substr(0, 15);
    strFileName += strExtension;
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    o_report_complete_path = i_path_entropy_input.parent_path();
    o_report_complete_path /= strFileName;
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    return  sts = ns_consts::EnmReturnStatus::Success;
}

// -------------------------------------------------------------------------- //
/// <summary>
/// </summary>
/// <remarks>
/// </remarks>
/// <param name="o_refComment">
/// </param>
/// <param name="i_refCommentSummary">
/// </param>
/// <returns>
/// </returns>
/// <precondition>
/// </precondition>
/// <postcondition>
/// </postcondition>
// -------------------------------------------------------------------------- //
ns_consts::EnmReturnStatus getLaTeXComment(std::wstring& o_refComment, const std::wstring& i_refCommentSummary)
{
    ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorUnexpected;

    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    std::wstringstream	ssComments = std::wstringstream();
    ssComments << L"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << L"\n";
    ssComments << L"%%%%%%" << L"\n";
    ssComments << L"%%%%%%";
    if (false == i_refCommentSummary.empty())
    {
        ssComments << L" " << i_refCommentSummary;
    }
    ssComments << L"\n";
    ssComments << L"%%%%%%" << L"\n";
    ssComments << L"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << L"\n";

    o_refComment = ssComments.str();
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    return  sts = ns_consts::EnmReturnStatus::Success;
}

// -------------------------------------------------------------------------- //
/// <summary>
/// </summary>
/// <remarks>
/// </remarks>
/// <param name="o_ssLaTeX">
/// </param>
/// <returns>
/// </returns>
/// <precondition>
/// </precondition>
/// <postcondition>
/// </postcondition>
// -------------------------------------------------------------------------- //
ns_consts::EnmReturnStatus loadLaTeXPreamble(std::wstringstream& o_ssLaTeX)
{
    ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorUnexpected;

    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    std::wstring	strCommentDefault = std::wstring();
    std::wstring	strNull = std::wstring();

    getLaTeXComment(strCommentDefault, strNull);
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    o_ssLaTeX << L"\\documentclass[a3paper,xelatex,english]{bxjsarticle}" << L"\n";
    o_ssLaTeX << L"\\usepackage{pgfplots,pgfplotstable}" << L"\n";
    o_ssLaTeX << L"\\pgfplotsset{ compat = newest }" << L"\n";
    o_ssLaTeX << L"\\usepackage{tikz}" << L"\n";
    o_ssLaTeX << L"\\usetikzlibrary{arrows.meta,bending,calc,shapes,positioning}" << L"\n";
    o_ssLaTeX << L"\\usepackage{ascmac}" << L"\n";
    o_ssLaTeX << L"\\usepackage{fancybox}" << L"\n";
    o_ssLaTeX << L"\\usepackage{amsmath,amssymb}" << L"\n";
    o_ssLaTeX << L"\\usepackage{algorithm}" << L"\n";
    o_ssLaTeX << L"\\usepackage[edges]{forest}" << L"\n";
    o_ssLaTeX << L"\\usepackage{array}" << L"\n";
    o_ssLaTeX << L"\\usepackage{algpseudocode}" << L"\n";
    o_ssLaTeX << L"\\usepackage{paralist}" << L"\n";
    o_ssLaTeX << L"\\usepackage{cases}" << L"\n";
    o_ssLaTeX << L"\\usepackage{fvextra}" << L"\n";
    o_ssLaTeX << L"\\usepackage{colortbl}" << L"\n";
    o_ssLaTeX << L"\\usepackage{xcolor}" << L"\n";
    o_ssLaTeX << L"\\usepackage{fancyhdr}" << L"\n";
    o_ssLaTeX << L"\\usepackage[explicit]{titlesec}" << L"\n";
    o_ssLaTeX << L"\\usepackage{xspace}" << L"\n";
    o_ssLaTeX << L"\\usepackage[many]{tcolorbox}" << L"\n";
    o_ssLaTeX << L"\\usepackage{lastpage}" << L"\n";
    o_ssLaTeX << L"\\usepackage{verbatim}" << L"\n";
    o_ssLaTeX << L"\\usepackage{multirow}" << L"\n";
    o_ssLaTeX << L"\\usepackage{censor}" << L"\n";
    o_ssLaTeX << L"\\usepackage{adjustbox}" << L"\n";
    o_ssLaTeX << L"\\usepackage[unicode,pdftitle={Report of Entropy estimates based on NIST SP 800-90B non-IID track},pdfkeywords={SP800-90B; entropy; random number generation;},setpagesize=false]{hyperref}" << L"\n";
    o_ssLaTeX << L"\\usepackage[open,openlevel=4]{bookmark}" << L"\n";
    o_ssLaTeX << L"\\newcommand\\mib[1]{\\boldsymbol{#1}}" << L"\n";
    o_ssLaTeX << L"\\usepgfplotslibrary{patchplots}" << L"\n";
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    std::wstring	strCommentCustomPageNumber = std::wstring();
    std::wstring	strSummaryCustomPageNumber = std::wstring(L"customize page numbering");

    getLaTeXComment(strCommentCustomPageNumber, strSummaryCustomPageNumber);

	o_ssLaTeX << strCommentCustomPageNumber;
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    o_ssLaTeX << L"\\fancypagestyle{mypagestylewithtotalpagenumbers}{" << L"\n"; //
    o_ssLaTeX << L"\\lhead{}" << L"\n"; // 
    o_ssLaTeX << L"\\rhead{}" << L"\n"; //
    o_ssLaTeX << L"\\cfoot{\\thepage/\\pageref{LastPage}}" << L"\n"; // 
    o_ssLaTeX << L"\\renewcommand{\\headrulewidth}{0.0pt}" << L"\n"; // 
    o_ssLaTeX << L"}" << L"\n";
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    std::wstring	strCommentToC = std::wstring();
    std::wstring	strSummaryToC = std::wstring(L"output up to 4-th level");

    getLaTeXComment(strCommentToC, strSummaryToC);

    o_ssLaTeX << strCommentToC;
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    o_ssLaTeX << L"\\setcounter{secnumdepth}{4}" << L"\n";
    o_ssLaTeX << L"\\setcounter{tocdepth}{4}" << L"\n";    
    o_ssLaTeX << L"\\setlength{\\topmargin}{-1cm}" << L"\n";
    o_ssLaTeX << L"\\setlength{\\textheight}{37cm}" << L"\n";
    o_ssLaTeX << strCommentDefault;
    o_ssLaTeX << L"%%%\\renewcommand{ \\figurename }{Figure }" << L"\n";
    o_ssLaTeX << L"%%%\\renewcommand{ \\tablename }{Table }" << L"\n";
    o_ssLaTeX << L"%%%\\renewcommand{ \\refname }{References}" << L"\n";
    o_ssLaTeX << strCommentDefault;
    o_ssLaTeX << L"\\definecolor{rowcolorlightblue}{RGB}{191,233,251}" << L"\n";
    o_ssLaTeX << L"\\definecolor{bordercolordarkblue}{RGB}{0,163,243}" << L"\n";
    o_ssLaTeX << L"\\definecolor{BleuDur}{RGB}{27,61,176}" << L"\n";
    o_ssLaTeX << L"\\definecolor{Nigelle}{RGB}{0,133,201}" << L"\n";
    o_ssLaTeX << L"\\definecolor{BleuFaience}{RGB}{105,171,219}" << L"\n";
    o_ssLaTeX << L"\\definecolor{anotherlightblue}{RGB}{61,143,244}" << L"\n";
    o_ssLaTeX << L"\\definecolor{NPGred}{RGB} {230, 75, 53}" << L"\n";
    o_ssLaTeX << L"\\definecolor{NPGskyblue}{RGB} {77, 187, 213}" << L"\n";
    o_ssLaTeX << L"\\definecolor{NPGgreen}{RGB} {0, 160, 135}" << L"\n";
    o_ssLaTeX << L"\\definecolor{NPGnavyblue}{RGB} {60, 84, 136}" << L"\n";
    o_ssLaTeX << L"\\definecolor{NPGsalmon}{RGB} {243, 155, 127}" << L"\n";
    o_ssLaTeX << strCommentDefault;
    o_ssLaTeX << L"\\def\\chpcolor{BleuDur}" << L"\n";
    o_ssLaTeX << L"\\def\\chpcolortxt{BleuDur}" << L"\n";
    o_ssLaTeX << L"\\def\\sectionfont{\\sffamily\\LARGE}" << L"\n";
    o_ssLaTeX << strCommentDefault;
    o_ssLaTeX << L"\\makeatletter" << L"\n";
    o_ssLaTeX << L"%Section:" << L"\n";
    o_ssLaTeX << L"\\def\\@sectionstrut{\\vrule\\@width\\z@\\@height12.5\\p@}" << L"\n";
    o_ssLaTeX << L"\\def\\@makesectionhead#1{%" << L"\n";
    o_ssLaTeX << L"  {\\par\\vspace{20pt}%" << L"\n";
    o_ssLaTeX << L"   \\parindent 0pt\\raggedleft\\sectionfont" << L"\n";
    o_ssLaTeX << L"   \\colorbox{\\chpcolor}{%" << L"\n";
    o_ssLaTeX << L"     \\parbox[t]{90pt}{\\color{white}\\@sectionstrut\\@depth4.5\\p@\\hfill" << L"\n";
    o_ssLaTeX << L"       \\ifnum\\c@secnumdepth>\\z@\\thesection\\fi}%" << L"\n";
    o_ssLaTeX << L"   }%" << L"\n";
    o_ssLaTeX << L"   \\begin{minipage}[t]{\\dimexpr\\textwidth-90pt-2\\fboxsep\\relax}" << L"\n";
    o_ssLaTeX << L"   \\color{\\chpcolortxt}\\@sectionstrut\\hspace{5pt}#1" << L"\n";
    o_ssLaTeX << L"   \\end{minipage}\\par" << L"\n";
    o_ssLaTeX << L"   \\vspace{10pt}%" << L"\n";
    o_ssLaTeX << L"  }" << L"\n";
    o_ssLaTeX << L"}" << L"\n";
    o_ssLaTeX << L"\\def\\section{\\@afterindentfalse\\secdef\\@section\\@ssection}" << L"\n";
    o_ssLaTeX << L"\\def\\@section[#1]#2{%" << L"\n";
    o_ssLaTeX << L"  \\ifnum\\c@secnumdepth>\\m@ne" << L"\n";
    o_ssLaTeX << L"    \\refstepcounter{section}%" << L"\n";
    o_ssLaTeX << L"    \\addcontentsline{toc}{section}{\\protect\\numberline{\\thesection}#1}%" << L"\n";
    o_ssLaTeX << L"  \\else" << L"\n";
    o_ssLaTeX << L"    \\phantomsection" << L"\n";
    o_ssLaTeX << L"    \\addcontentsline{toc}{section}{#1}%" << L"\n";
    o_ssLaTeX << L"  \\fi" << L"\n";
    o_ssLaTeX << L"  \\sectionmark{#1}%" << L"\n";
    o_ssLaTeX << L"  \\if@twocolumn" << L"\n";
    o_ssLaTeX << L"    \\@topnewpage[\\@makesectionhead{#2}]%" << L"\n";
    o_ssLaTeX << L"  \\else" << L"\n";
    o_ssLaTeX << L"    \\@makesectionhead{#2}\\@afterheading" << L"\n";
    o_ssLaTeX << L"  \\fi" << L"\n";
    o_ssLaTeX << L"}" << L"\n";
    o_ssLaTeX << L"\\def\\@ssection#1{%" << L"\n";
    o_ssLaTeX << L"  \\if@twocolumn" << L"\n";
    o_ssLaTeX << L"    \\@topnewpage[\\@makesectionhead{#1}]%" << L"\n";
    o_ssLaTeX << L"  \\else" << L"\n";
    o_ssLaTeX << L"    \\@makesectionhead{#1}\\@afterheading" << L"\n";
    o_ssLaTeX << L"  \\fi" << L"\n";
    o_ssLaTeX << L"}" << L"\n";
    o_ssLaTeX << L"\\makeatother" << L"\n";
    o_ssLaTeX << strCommentDefault;
    o_ssLaTeX << L"\\setlength{ \\topmargin }{-1.5cm}" << L"\n";
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    o_ssLaTeX << strCommentDefault;
    o_ssLaTeX << L"\\pagestyle{mypagestylewithtotalpagenumbers}" << L"\n";
    o_ssLaTeX << L"%%%%%%" << L"\n";
    o_ssLaTeX << L"%%%%%%" << L"\n";
    o_ssLaTeX << L"%%%%%%" << L"\n";
    o_ssLaTeX << L"\\title{Report of Entropy estimates based on NIST SP 800-90B non-IID track}" << L"\n";
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    const auto pt =
        boost::posix_time::ptime(
            boost::posix_time::microsec_clock::local_time());

    const std::string strTimeInfo(boost::posix_time::to_simple_string(pt));

    const int size_needed = MultiByteToWideChar(CP_UTF8, 0, strTimeInfo.data(), static_cast<int>(strTimeInfo.size()), nullptr, 0);
    std::wstring    wstrTimeInfo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, strTimeInfo.data(), static_cast<int>(strTimeInfo.size()), wstrTimeInfo.data(), size_needed);

    o_ssLaTeX << L"\\date{" << wstrTimeInfo << L"}" <<L"\n";
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    return  sts = ns_consts::EnmReturnStatus::Success;
}

// -------------------------------------------------------------------------- //
/// <summary>
/// </summary>
/// <remarks>
/// </remarks>
/// <param name="o_ssLaTeX">
/// </param>
/// <param name="isBinary">
/// </param>
/// <param name="min_entropy">
/// </param>
/// <param name="bits_per_sample">
/// </param>
/// <returns>
/// </returns>
/// <precondition>
/// </precondition>
/// <postcondition>
/// </postcondition>
// -------------------------------------------------------------------------- //
ns_consts::EnmReturnStatus loadPGFPlotSummary(std::wstringstream& o_ssLaTeX, bool isBinary, double min_entropy, unsigned int bits_per_sample)
{
    ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorUnexpected;

    if (bits_per_sample <= 0)
    {
        return sts;
    }
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    o_ssLaTeX << L"\\begin{figure}[htbp]" << L"\n";
    o_ssLaTeX << L"\\centering" << L"\n";
    o_ssLaTeX << L"\n";
    o_ssLaTeX << L"\\begin{tikzpicture} " << L"\n";
    o_ssLaTeX << L"\\begin{axis}" << L"\n";
    o_ssLaTeX << L"\t[symbolic x coords={6.3.1,6.3.2,6.3.3,6.3.4,6.3.5,6.3.6,6.3.7,6.3.8,6.3.9,6.3.10}," << L"\n";
    o_ssLaTeX << L"\twidth=18cm," << L"\n";
    o_ssLaTeX << L"\tymin=0," << L"\n";
    o_ssLaTeX << L"\tymax=" << bits_per_sample << "," << L"\n";
    o_ssLaTeX << L"	xlabel=Sub-sub-section of NIST SP 800-90B," << L"\n";
    o_ssLaTeX << L"	ylabel={Estimated min-entropy $[$bit / " << bits_per_sample << L"-bit$]$}," << L"\n";
    o_ssLaTeX << L"\txtick=data]" << L"\n";
    o_ssLaTeX << L"\\addplot+[forget plot,only marks] " << L"\n";
    o_ssLaTeX << L"  plot[error bars/.cd, y dir=both, y explicit]" << L"\n";
    o_ssLaTeX << L"  table[x=section,y=y,y error plus expr=\\thisrow{y-max},y error minus expr=\\thisrow{y-min}] {\\summarytable";
    if (false == isBinary)
    {
        o_ssLaTeX << L"Non";
    }
    o_ssLaTeX << L"Binary};" << L"\n";
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    o_ssLaTeX << L"\\addplot+[Nigelle,no marks,sharp plot,update limits=false] " << L"\n";
    o_ssLaTeX << L"coordinates {(6.3.1," << min_entropy << L") (6.3.10," << min_entropy << L")}" << L"\n";
    o_ssLaTeX << L"node[";
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    const double relativeRange = min_entropy / (static_cast<double>(bits_per_sample));
    if (relativeRange < 0.25)
    {
        o_ssLaTeX << L"above";
        if (relativeRange < 0.05)
        {
            o_ssLaTeX << L", yshift=5mm";
        }
    }
    else
    {
        o_ssLaTeX << L"below";
    }
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    o_ssLaTeX << L"] at (axis cs:6.3.5," << min_entropy << L") {Estimated min-entropy = " << L"\n";
    o_ssLaTeX << min_entropy << L"};" << L"\n";
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    o_ssLaTeX << L"\\end{axis} " << L"\n";
    o_ssLaTeX << L"\\end{tikzpicture}" << L"\n";
    o_ssLaTeX << L"\n";
    o_ssLaTeX << L"\\caption{Estimated Min-Entropy using $\\S$6.3 of NIST SP 800-90B}" << L"\n";
    o_ssLaTeX << L"\\end{figure}" << L"\n";
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    return  sts = ns_consts::EnmReturnStatus::Success;
}

// -------------------------------------------------------------------------- //
/// <summary>
/// </summary>
/// <remarks>
/// </remarks>
/// <param name="o_ssLaTeX">
/// </param>
/// <returns>
/// </returns>
/// <precondition>
/// </precondition>
/// <postcondition>
/// </postcondition>
// -------------------------------------------------------------------------- //
ns_consts::EnmReturnStatus loadLaTeXBibliography(std::wstringstream& o_ssLaTeX)
{
    ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorUnexpected;

    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    o_ssLaTeX << L"\\begin{thebibliography}{99}" << L"\n";
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    o_ssLaTeX << L"% 1" << L"\n";
    o_ssLaTeX << L"\\bibitem{SP80090B}" << L"\n";
    o_ssLaTeX << L"Meltem S\\\"{o}nmez Turan," << L"\n";
    o_ssLaTeX << L"Elaine Barker," << L"\n";
    o_ssLaTeX << L"John Kelsey," << L"\n";
    o_ssLaTeX << L"Kerry A. McKay," << L"\n";
    o_ssLaTeX << L"Mary L. Baish," << L"\n";
    o_ssLaTeX << L"Mike Boyle," << L"\n";
    o_ssLaTeX << L"\\textit{Recommendation for the Entropy Sources Used for Random Bit Generation}," << L"\n";
    o_ssLaTeX << L"NIST Special Publication 800-90B, Jan. 2018 " << L"\n";
    o_ssLaTeX << L"\\url{https://nvlpubs.nist.gov/nistpubs/SpecialPublications/NIST.SP.800-90B.pdf}" << L"\n";
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    o_ssLaTeX << L"% 2" << L"\n";
    o_ssLaTeX << L"\\bibitem{CorrectionsSP80090B}" << L"\n";
    o_ssLaTeX << L"G. Sakurai, \\textit{Proposed list of corrections for NIST SP 800-90B 6.3 Estimators}, Dec. 2022 " << L"\n";
    o_ssLaTeX << L"\\url{https://github.com/g-g-sakura/AnotherEntropyEstimationTool/blob/main/documentation/ProposedListOfCorrections_SP800-90B.pdf}" << L"\n";
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    o_ssLaTeX << L"% 3" << L"\n";
    o_ssLaTeX << L"\\bibitem{MIT}" << L"\n";
    o_ssLaTeX << L"Thomas H. Cormen, Charles E. Leiserson, Ronald L. Rivest and Clifford Stein, \\textit{Introduction to Algorithms (fourth edition)}, The MIT Press." << L"\n";
    o_ssLaTeX << L"\\url{https://mitpress.mit.edu/9780262046305/introduction-to-algorithms/}" << L"\n";
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    o_ssLaTeX << L"% 4" << L"\n";
    o_ssLaTeX << L"\\bibitem{ImplementationNotes}" << L"\n";
    o_ssLaTeX << L"G. Sakurai, \\textit{ ImplementationNotes for entropy estimation based on NIST SP800-90B non-IID track}, Sep. 2025 " << L"\n";
    o_ssLaTeX << L"\\url{https://github.com/g-g-sakura/AnotherEntropyEstimationTool/blob/main/documentation/SP800-90B_EntropyEstimate_ImplementationNotes.pdf}" << L"\n";
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    o_ssLaTeX << L"\\end{thebibliography}" << L"\n";
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    return  sts = ns_consts::EnmReturnStatus::Success;
}

// -------------------------------------------------------------------------- //
/// <summary>
/// </summary>
/// <remarks>
/// </remarks>
/// <param name="o_refLaTeXSupportingInfo">
/// </param>
/// <returns>
/// </returns>
/// <precondition>
/// </precondition>
/// <postcondition>
/// </postcondition>
// -------------------------------------------------------------------------- //
ns_consts::EnmReturnStatus reportLaTeXSupportingCompilerInfo(std::wstringstream& o_refLaTeXSupportingInfo)
{
    ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorUnexpected;

    // -------------------------------------------------------------------------- //
    // compiler information
    // -------------------------------------------------------------------------- //
    o_refLaTeXSupportingInfo << L"\\, & built by & ";
    {
#if defined(_MSC_VER) && !defined(__INTEL_LLVM_COMPILER) && !defined(__INTEL_COMPILER)
#if _MSC_VER >= 1944
        o_refLaTeXSupportingInfo << L" Visual Studio 2022 version 17.14";
        o_refLaTeXSupportingInfo << L" (\\verb|_MSC_FULL_VER|: " << _MSC_FULL_VER << L" )";
#elif _MSC_VER >= 1943
        o_refLaTeXSupportingInfo << L" Visual Studio 2022 version 17.13";
        o_refLaTeXSupportingInfo << L" (\\verb|_MSC_FULL_VER|: " << _MSC_FULL_VER << L" )";
#elif _MSC_VER >= 1942
        o_refLaTeXSupportingInfo << L" Visual Studio 2022 version 17.12";
        o_refLaTeXSupportingInfo << L" (\\verb|_MSC_FULL_VER|: " << _MSC_FULL_VER << L" )";
#elif _MSC_VER >= 1941
        o_refLaTeXSupportingInfo << L" Visual Studio 2022 version 17.11";
        o_refLaTeXSupportingInfo << L" (\\verb|_MSC_FULL_VER|: " << _MSC_FULL_VER << L" )";
#elif _MSC_VER >= 1940
        o_refLaTeXSupportingInfo << L" Visual Studio 2022 version 17.10";
        o_refLaTeXSupportingInfo << L" (\\verb|_MSC_FULL_VER|: " << _MSC_FULL_VER << L" )";
#elif _MSC_VER >= 1939
        o_refLaTeXSupportingInfo << L" Visual Studio 2022 version 17.9";
        o_refLaTeXSupportingInfo << L" (\\verb|_MSC_FULL_VER|: " << _MSC_FULL_VER << L" )";
#elif _MSC_VER >= 1938
        o_refLaTeXSupportingInfo << L" Visual Studio 2022 version 17.8";
        o_refLaTeXSupportingInfo << L" (\\verb|_MSC_FULL_VER|: " << _MSC_FULL_VER << L" )";
#elif _MSC_VER >= 1937
        o_refLaTeXSupportingInfo << L" Visual Studio 2022 version 17.7";
        o_refLaTeXSupportingInfo << L" (\\verb|_MSC_FULL_VER|: " << _MSC_FULL_VER << L" )";
#elif _MSC_VER >= 1936
        o_refLaTeXSupportingInfo << L" Visual Studio 2022 version 17.6";
        o_refLaTeXSupportingInfo << L" (\\verb|_MSC_FULL_VER|: " << _MSC_FULL_VER << L" )";
#elif _MSC_VER >= 1935
        o_refLaTeXSupportingInfo << L" Visual Studio 2022 version 17.5";
        o_refLaTeXSupportingInfo << L" (\\verb|_MSC_FULL_VER|: " << _MSC_FULL_VER << L" )";
#elif _MSC_VER >= 1934
        o_refLaTeXSupportingInfo << L" Visual Studio 2022 version 17.4";
        o_refLaTeXSupportingInfo << L" (\\verb|_MSC_FULL_VER|: " << _MSC_FULL_VER << L" )";
#elif _MSC_VER >= 1933
        o_refLaTeXSupportingInfo << L" Visual Studio 2022 version 17.3";
        o_refLaTeXSupportingInfo << L" (\\verb|_MSC_FULL_VER|: " << _MSC_FULL_VER << L" )";
#elif _MSC_VER >= 1932
        o_refLaTeXSupportingInfo << L" Visual Studio 2022 version 17.2";
        o_refLaTeXSupportingInfo << L" (\\verb|_MSC_FULL_VER|: " << _MSC_FULL_VER << L" )";
#elif _MSC_VER >= 1931
        o_refLaTeXSupportingInfo << L" Visual Studio 2022 version 17.1";
        o_refLaTeXSupportingInfo << L" (\\verb|_MSC_FULL_VER|: " << _MSC_FULL_VER << L" )";
#elif _MSC_VER >= 1930
        o_refLaTeXSupportingInfo << L" Visual Studio 2022 RTW (17.0)";
        o_refLaTeXSupportingInfo << L" (\\verb|_MSC_FULL_VER|: " << _MSC_FULL_VER << L" )";
#elif _MSC_VER >= 1929
        o_refLaTeXSupportingInfo << L" Visual Studio 2019 version ";
#if _MSC_FULL_VER >= 192930100 
        o_refLaTeXSupportingInfo << L"16.11";
#else
        o_refLaTeXSupportingInfo << L"16.10";
#endif
        o_refLaTeXSupportingInfo << L" (\\verb|_MSC_FULL_VER|: " << _MSC_FULL_VER << L" )";
#elif _MSC_VER >= 1928
        o_refLaTeXSupportingInfo << L" Visual Studio 2019 version ";
#if _MSC_FULL_VER >= 192829500 
        o_refLaTeXSupportingInfo << L"16.9";
#else
        o_refLaTeXSupportingInfo << L"16.8";
#endif
        o_refLaTeXSupportingInfo << L" (\\verb|_MSC_FULL_VER|: " << _MSC_FULL_VER << L" )";
#elif _MSC_VER >= 1927
        o_refLaTeXSupportingInfo << L" Visual Studio 2019 version 16.7";
        o_refLaTeXSupportingInfo << L" (\\verb|_MSC_FULL_VER|: " << _MSC_FULL_VER << L" )";
#endif
#elif defined(_MSC_VER) && defined(__INTEL_LLVM_COMPILER)
        o_refLaTeXSupportingInfo << L" Intel C++ Compiler";
        o_refLaTeXSupportingInfo << L" (";
        o_refLaTeXSupportingInfo << L" \\verb|__INTEL_LLVM_COMPILER|: " << __INTEL_LLVM_COMPILER;
        o_refLaTeXSupportingInfo << L" )";
#elif defined(_MSC_VER) && defined(__INTEL_COMPILER)
        o_refLaTeXSupportingInfo << L" Intel C++ Compiler Classic";
        o_refLaTeXSupportingInfo << L" (";
        o_refLaTeXSupportingInfo << L" \\verb|__INTEL_COMPILER|: " << __INTEL_COMPILER;
        o_refLaTeXSupportingInfo << L" )";
#endif
    }
    o_refLaTeXSupportingInfo << L" \\\\" << L"\n";
    o_refLaTeXSupportingInfo << L"\\cline{2-3}" << L"\n";
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    return  sts = ns_consts::EnmReturnStatus::Success;
}

// -------------------------------------------------------------------------- //
/// <summary>
/// </summary>
/// <remarks>
/// </remarks>
/// <param name="o_refLaTeXSupportingInfo">
/// </param>
/// <param name="i_refInfoReport">
/// </param>
/// <param name="io_refDataOriginal">
/// </param>
/// <returns>
/// </returns>
/// <precondition>
/// </precondition>
/// <postcondition>
/// </postcondition>
// -------------------------------------------------------------------------- //
ns_consts::EnmReturnStatus reportLaTeXSupportingInfo(std::wstringstream &o_refLaTeXSupportingInfo, 
    const IDInfoForReport& i_refInfoReport,
    const ns_dt::t_data_for_estimator& io_refDataOriginal)
{
    ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorUnexpected;

    // -------------------------------------------------------------------------- //
    // calculate hash value of the acquisition data in advance
    // -------------------------------------------------------------------------- //
    std::string strHashOfAcquisitionData = std::string();

    constexpr entropy_estimator_app::constants::EnmHashAlgorithm  enmDefaultHashId = entropy_estimator_app::constants::EnmHashAlgorithm::ESHA_256;
    const ns_consts::EnmReturnStatus  stsCalcHash = entropy_estimator_app::calcMessageDigest(strHashOfAcquisitionData,
        io_refDataOriginal.p_bzInputS->data(), io_refDataOriginal.p_bzInputS->length(blitz::firstDim),
        enmDefaultHashId);

    if (ns_consts::EnmReturnStatus::Success != stsCalcHash)
    {
        return  sts = stsCalcHash;
    }

    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    std::wstring	strSection = std::wstring();
    std::wstring	strSectionTitle = std::wstring(L"Identification information");
    std::wstring	strLabel = std::wstring(L"sec:IdentificationInfo");
    ns_spt::getLaTeXSection(strSection, strSectionTitle, strLabel);
    o_refLaTeXSupportingInfo << strSection << L"\n";
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    std::wstring	strSubsectionAcquisitionData = std::wstring();
    std::wstring	strSubsectionTitleAcquisitionData = std::wstring(L"Identification of acquisition data from entropy source");
    strLabel = std::wstring(L"sec:IdentificationInfo-acquisitiondata");
    ns_spt::getLaTeXSubsection(strSubsectionAcquisitionData, strSubsectionTitleAcquisitionData, strLabel);
    o_refLaTeXSupportingInfo << strSubsectionAcquisitionData << L"\n";
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    o_refLaTeXSupportingInfo << L"\\renewcommand{\\arraystretch}{1.8}" << L"\n";
    o_refLaTeXSupportingInfo << L"\\begin{table}[h]" << L"\n";
    o_refLaTeXSupportingInfo << L"\\caption{Identification information of acquisition data from entropy source}" << L"\n";
    o_refLaTeXSupportingInfo << L"\\begin{center}" << L"\n";
    o_refLaTeXSupportingInfo << L"\\begin{tabular}{|>{\\columncolor{anotherlightblue}}p{2cm}|p{20.5cm}|}" << L"\n";
    o_refLaTeXSupportingInfo << L"\\hline " << L"\n";
    o_refLaTeXSupportingInfo << L"Path to the acquisition data & \\verb|" << (*i_refInfoReport.info_source.p_path_to_entropy_input) << L"| \\\\" << L"\n";
    o_refLaTeXSupportingInfo << L"\\hline" << L"\n";

    switch (enmDefaultHashId)
    {
    case entropy_estimator_app::constants::EnmHashAlgorithm::ESHA_256:
        o_refLaTeXSupportingInfo << L"SHA-256";
        break;
    case entropy_estimator_app::constants::EnmHashAlgorithm::ESHA_384:
        o_refLaTeXSupportingInfo << L"SHA-384";
        break;
    case entropy_estimator_app::constants::EnmHashAlgorithm::ESHA_512:
        o_refLaTeXSupportingInfo << L"SHA-512";
        break;
    default:
        return  sts = ns_consts::EnmReturnStatus::ErrorUnexpected;
    }
    o_refLaTeXSupportingInfo << L" hash value of the acquisition data [hex] & " << L"\n";
    o_refLaTeXSupportingInfo << L"\\begin{verbatim}" << L"\n";

    const int size_needed = MultiByteToWideChar(CP_UTF8, 0, strHashOfAcquisitionData.data(), static_cast<int>(strHashOfAcquisitionData.size()), nullptr, 0);
    std::wstring    wstrHashOfAcquisitionData(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, strHashOfAcquisitionData.data(), static_cast<int>(strHashOfAcquisitionData.size()), wstrHashOfAcquisitionData.data(), size_needed);

    o_refLaTeXSupportingInfo << wstrHashOfAcquisitionData << L"\n";
    o_refLaTeXSupportingInfo << L"\\end{verbatim} " << L"\n";
    o_refLaTeXSupportingInfo << L"\\\\" << L"\n";
    o_refLaTeXSupportingInfo << L"\\hline" << L"\n";

    struct tm newtime;

    errno_t err = localtime_s(&newtime, &(i_refInfoReport.info_source.tm_last_write_time));
    if (0 != err)
    {
        std::cerr << "localtime_s failed with code " << err << std::endl;
        return  sts = ns_consts::EnmReturnStatus::ErrorInvalidData;
    }

    o_refLaTeXSupportingInfo << L"Last write time & " << std::put_time(&newtime, L"%Y-%b-%d %H:%M:%S") << L" \\\\" << L"\n";
    o_refLaTeXSupportingInfo << L"\\hline" << L"\n";
    o_refLaTeXSupportingInfo << L"\\end{tabular}" << L"\n";
    o_refLaTeXSupportingInfo << L"\\end{center}" << L"\n";
    o_refLaTeXSupportingInfo << L"\\end{table}" << L"\n";
    o_refLaTeXSupportingInfo << L"\\renewcommand{\\arraystretch}{1.8}" << L"\n";
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    o_refLaTeXSupportingInfo << L"\\vspace*{-3mm}" << L"\n";
    o_refLaTeXSupportingInfo << L"\\begin{itemize}" << L"\n";
    o_refLaTeXSupportingInfo << L"		\\item Name of the submitter of the acquisition data : " << L"\n";
    o_refLaTeXSupportingInfo << L"		    \\begin{Form}" << L"\n";
    o_refLaTeXSupportingInfo << L"		    \\noindent" << L"\n";
    o_refLaTeXSupportingInfo << L"		    \\adjustbox{valign=M}{\\TextField[name=NameOfSubmitter, multiline=false, bordercolor=bordercolordarkblue,width=14cm,height=1cm,charsize=0pt]{}}" << L"\n";
    o_refLaTeXSupportingInfo << L"		    \\end{Form}" << L"\n";
    o_refLaTeXSupportingInfo << L"\t	\\item Brief explanation of the acquisition data (or entropy source) : \\\\" << L"\n";
    o_refLaTeXSupportingInfo << L"\t	    \\begin{Form}" << L"\n";
    o_refLaTeXSupportingInfo << L"\t	    \\noindent" << L"\n";
    o_refLaTeXSupportingInfo << L"\t	    \\TextField[name=ExplanationOfAcquisitionData, multiline=true, bordercolor=bordercolordarkblue,width=\\linewidth,height=1in]{}" << L"\n";
    o_refLaTeXSupportingInfo << L"\t	    \\end{Form}" << L"\n";
    o_refLaTeXSupportingInfo << L"\t\\end{itemize} " << L"\n";
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    std::wstring	strSubsectionAnalysisEnv = std::wstring();
    std::wstring	strSubsectionTitleAnalysisEnv = std::wstring(L"Identification of analysis environment");
    strLabel = std::wstring(L"sec:IdentificationInfo-analysisenv");
    ns_spt::getLaTeXSubsection(strSubsectionAnalysisEnv, strSubsectionTitleAnalysisEnv, strLabel);
    o_refLaTeXSupportingInfo << strSubsectionAnalysisEnv << L"\n";
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    o_refLaTeXSupportingInfo << L"\\renewcommand{\\arraystretch}{1.8}" << L"\n";
    o_refLaTeXSupportingInfo << L"\\begin{table}[h]" << L"\n";
    o_refLaTeXSupportingInfo << L"\\caption{Identification information of analysis environment}" << L"\n";
    o_refLaTeXSupportingInfo << L"\\begin{center}" << L"\n";
    o_refLaTeXSupportingInfo << L"\\begin{tabular}{|>{\\columncolor{anotherlightblue}}l|>{\\columncolor{anotherlightblue}}l|p{12cm}|}" << L"\n";
    o_refLaTeXSupportingInfo << L"\\hline " << L"\n";
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    o_refLaTeXSupportingInfo << L"Analysis tool & Name & " << (*i_refInfoReport.info_analysis_tool.p_analyzer_name) << L" \\\\" << L"\n";
    o_refLaTeXSupportingInfo << L"\\cline{2-3}" << L"\n";
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    o_refLaTeXSupportingInfo << L"\\, & Versioning information & " << (*i_refInfoReport.info_analysis_tool.p_analyzer_versioning) << L" \\\\" << L"\n";
    o_refLaTeXSupportingInfo << L"\\cline{2-3}" << L"\n";
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    o_refLaTeXSupportingInfo << L"\\, & built as & ";
    {
#if defined(_WIN64)
        o_refLaTeXSupportingInfo << L" 64-bit application";
#elif defined(_WIN32)
        o_refLaTeXSupportingInfo << L" 32-bit application";
#else
#endif
    }
    o_refLaTeXSupportingInfo << L" \\\\" << L"\n";
    o_refLaTeXSupportingInfo << L"\\cline{2-3}" << L"\n";
    // -------------------------------------------------------------------------- //
    // compiler information
    // -------------------------------------------------------------------------- //
    const ns_consts::EnmReturnStatus  stsCompiler = reportLaTeXSupportingCompilerInfo(o_refLaTeXSupportingInfo);
    if (ns_consts::EnmReturnStatus::Success != stsCompiler)
    {
        return  sts = stsCompiler;
    }
    // -------------------------------------------------------------------------- //
    // information of linked libraries
    // -------------------------------------------------------------------------- //
    o_refLaTeXSupportingInfo << L"\\, & linked libraries & ";
    {
        o_refLaTeXSupportingInfo << L" Boost C++ ";
        o_refLaTeXSupportingInfo << BOOST_VERSION / 100000 << L"." << ((BOOST_VERSION / 100) % 1000) << L"." << BOOST_VERSION % 100;
        o_refLaTeXSupportingInfo << L" \\\\" << L"\n";
    }
    o_refLaTeXSupportingInfo << L"\\hline" << L"\n";
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    o_refLaTeXSupportingInfo << L"Analysis environment & Hostname & ";
    o_refLaTeXSupportingInfo << L"\\censor{";   // censor/redact
    o_refLaTeXSupportingInfo << (*i_refInfoReport.info_env.p_hostname);
    o_refLaTeXSupportingInfo << L"}";           // censor/redact
    o_refLaTeXSupportingInfo << L" \\\\" << L"\n";
    o_refLaTeXSupportingInfo << L"\\cline{2-3}" << L"\n";
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    o_refLaTeXSupportingInfo << L"\\, & CPU information & " << (*i_refInfoReport.info_env.p_cpuinfo) << L" \\\\" << L"\n";
    o_refLaTeXSupportingInfo << L"\\cline{2-3}" << L"\n";
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    o_refLaTeXSupportingInfo << L"\\, &  Physical memory size & " << (*i_refInfoReport.info_env.p_physicalmemory) << L" \\\\" << L"\n";
    o_refLaTeXSupportingInfo << L"\\cline{2-3}" << L"\n";
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    o_refLaTeXSupportingInfo << L"\\, &  OS name &  \\verb|"<< (*i_refInfoReport.info_env.p_osname) << L"| \\\\" << L"\n";
    o_refLaTeXSupportingInfo << L"\\cline{2-3}" << L"\n";
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    o_refLaTeXSupportingInfo << L"\\, &  OS version &  \\verb|" << (*i_refInfoReport.info_env.p_osversion) << L"| \\\\" << L"\n";
    o_refLaTeXSupportingInfo << L"\\cline{2-3}" << L"\n";
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    o_refLaTeXSupportingInfo << L"\\, &  System type & " << (*i_refInfoReport.info_env.p_system_type) << L" \\\\" << L"\n";
    o_refLaTeXSupportingInfo << L"\\cline{2-3}" << L"\n";
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    o_refLaTeXSupportingInfo << L"\\, &  Username & ";
    o_refLaTeXSupportingInfo << L"\\censor{";   // censor/redact
    o_refLaTeXSupportingInfo << (*i_refInfoReport.info_env.p_username);
    o_refLaTeXSupportingInfo << L"}";           // censor/redact
    o_refLaTeXSupportingInfo << L" \\\\" << L"\n";
    o_refLaTeXSupportingInfo << L"\\hline" << L"\n";
    o_refLaTeXSupportingInfo << L"\\end{tabular}" << L"\n";
    o_refLaTeXSupportingInfo << L"\\end{center}" << L"\n";
    o_refLaTeXSupportingInfo << L"\\end{table}" << L"\n";
    o_refLaTeXSupportingInfo << L"\\renewcommand{\\arraystretch}{1.4}" << L"\n";
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    std::wstring	strSubsectionAnalysisCond = std::wstring();
    std::wstring	strSubsectionTitleAnalysisCond = std::wstring(L"Identification of analysis conditions");
    strLabel = std::wstring(L"sec:IdentificationInfo-analysisconditions");
    ns_spt::getLaTeXSubsection(strSubsectionAnalysisCond, strSubsectionTitleAnalysisCond, strLabel);
    o_refLaTeXSupportingInfo << strSubsectionAnalysisCond << L"\n";
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    o_refLaTeXSupportingInfo << L"\\renewcommand{\\arraystretch}{1.8}" << L"\n";
    o_refLaTeXSupportingInfo << L"\\begin{table}[h]" << L"\n";
    o_refLaTeXSupportingInfo << L"\\caption{Identification information of analysis conditions}" << L"\n";
    o_refLaTeXSupportingInfo << L"\\begin{center}" << L"\n";
    o_refLaTeXSupportingInfo << L"\\begin{tabular}{|>{\\columncolor{anotherlightblue}}l|p{8cm}|}" << L"\n";
    o_refLaTeXSupportingInfo << L"\\hline " << L"\n";
    o_refLaTeXSupportingInfo << L"Number of samples & " << io_refDataOriginal.L << L" \\\\" << L"\n";
    o_refLaTeXSupportingInfo << L"\\hline" << L"\n";
    o_refLaTeXSupportingInfo << L"Bits per sample & " << io_refDataOriginal.bits_per_sample << L" \\\\" << L"\n";
    o_refLaTeXSupportingInfo << L"\\hline" << L"\n";
    // -------------------------------------------------------------------------- //
    // MSb or LSb
    // -------------------------------------------------------------------------- //
    if (1 < io_refDataOriginal.bits_per_sample)
    {
        o_refLaTeXSupportingInfo << L"Byte to bit conversion & " << L"\n";
        if (io_refDataOriginal.bIsMSbFirstByteBitConversion)
        {
            o_refLaTeXSupportingInfo << L"Most Significant bit (MSb) first" << L"\n";
        }
        else
        {
            o_refLaTeXSupportingInfo << L"Least Significant bit (LSb) first" << L"\n";
        }
        o_refLaTeXSupportingInfo << L" \\\\" << L"\n";
        o_refLaTeXSupportingInfo << L"\\hline" << L"\n";
    }
    // -------------------------------------------------------------------------- //
    // using LCP applied or not
    //  add footnotemark instead of footnote, as this is inside tabular environment
    // -------------------------------------------------------------------------- //
    o_refLaTeXSupportingInfo << L"Use Longest Common Prefix\\footnotemark for 6.3.5 and 6.3.6 & ";
    if (io_refDataOriginal.isUsingLcpRequested)
    {
        o_refLaTeXSupportingInfo << "True";
    }
    else
    {
        o_refLaTeXSupportingInfo << "False";
    }
    o_refLaTeXSupportingInfo << L" \\\\" << L"\n";
    o_refLaTeXSupportingInfo << L"\\hline" << L"\n";

    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    o_refLaTeXSupportingInfo << L"\\end{tabular}" << L"\n";
    o_refLaTeXSupportingInfo << L"\\end{center}" << L"\n";
    o_refLaTeXSupportingInfo << L"\\end{table}" << L"\n";
    // -------------------------------------------------------------------------- //
    //  add footnotetext related to footnotemark
    // -------------------------------------------------------------------------- //
    o_refLaTeXSupportingInfo << L"\\footnotetext[1]{See \\cite{MIT} and \\cite{ImplementationNotes}}" << L"\n";
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    o_refLaTeXSupportingInfo << L"\\renewcommand{\\arraystretch}{1.4}" << L"\n";
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    std::wstring	strSubsectionAnalysisMethod = std::wstring();
    std::wstring	strSubsectionTitleAnalysisMethod = std::wstring(L"Identification of analysis method");
    strLabel = std::wstring(L"sec:IdentificationInfo-analysismethod");
    ns_spt::getLaTeXSubsection(strSubsectionAnalysisMethod, strSubsectionTitleAnalysisMethod, strLabel);
    o_refLaTeXSupportingInfo << strSubsectionAnalysisMethod << L"\n";
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    o_refLaTeXSupportingInfo << L"NIST SP 800-90B \\cite{SP80090B} 6.3 with corrections \\cite{CorrectionsSP80090B} is applied" << L"\n";
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    return  sts = ns_consts::EnmReturnStatus::Success;
}

// -------------------------------------------------------------------------- //
/// <summary>
/// </summary>
/// <remarks>
/// </remarks>
/// <param name="i_report_path_LaTeX">
/// </param>
/// <returns>
/// </returns>
/// <precondition>
/// </precondition>
/// <postcondition>
/// </postcondition>
// -------------------------------------------------------------------------- //
void showLaTeXCompilationHint(const bs_fs::path& i_report_path_LaTeX)
{
    // -------------------------------------------------------------------------- //
    // Output supplementary information to compile LateX source file
    // -------------------------------------------------------------------------- //
    setlocale(LC_ALL, "");
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    std::wcout << L"\n";
    std::wcout << L"# [INFO] In order to compile the generated XeLaTeX source file of entropy estimation report, " << L"\n";
    std::wcout << L"# [INFO] you need to type the following command and press \"Enter\" key." << L"\n";
    std::wcout << L"# [INFO] ";
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_BLUE);
    std::wcout << L" xelatex " << i_report_path_LaTeX.wstring() << L"\n";
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    std::wcout << L"\n";
    std::wcout << L"# [INFO] In a case when you get an error message like ";
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN);
    std::wcout << L"\"TeX capacity exceeded, sorry [main memory size=...]\"," << L"\n";
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    std::wcout << L"# [INFO] first you need to update the \"texmf.cnf\" file so as to modify the memory size like as follows:" << L"\n";
    std::wcout << L"# [INFO] ";
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_BLUE);
    std::wcout << L" main_memory = 12400000" << L"\n";
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    std::wcout << L"# [INFO] Next you need to run command prompt with Administrator privilege and type as follows:" << L"\n";
    std::wcout << L"# [INFO] ";
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_BLUE);
    std::wcout << L" fmtutil-sys --all" << L"\n";
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    std::wcout << L"# [INFO] and press \"Enter\" key.";
}

// -------------------------------------------------------------------------- //
/// <summary>
/// </summary>
/// <remarks>
/// </remarks>
/// <param name="i_refInfoReport">
/// </param>
/// <param name="io_refDataOriginal">
/// </param>
/// <param name="io_refDataBinary">
/// </param>
/// <returns>
/// </returns>
/// <precondition>
/// </precondition>
/// <postcondition>
/// </postcondition>
// -------------------------------------------------------------------------- //
ns_consts::EnmReturnStatus reportLaTeXNonBinary(IDInfoForReport& i_refInfoReport,
    ns_dt::t_data_for_estimator& io_refDataOriginal, ns_dt::t_data_for_estimator& io_refDataBinary)
{
    ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorUnexpected;

    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    if (nullptr == i_refInfoReport.info_source.p_path_to_entropy_input)
    {
        return  sts = ns_consts::EnmReturnStatus::ErrorNullPointer;
    }
    bs_fs::path the_report_path_LaTeX;
    sts = synthesizeReportPathTex(the_report_path_LaTeX, *i_refInfoReport.info_source.p_path_to_entropy_input);
    if (ns_consts::EnmReturnStatus::Success != sts)
    {
        return  sts;
    }
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    double  min_entropy_bitstring = 1.0;
    auto  min_entropy_literal = static_cast<double>(io_refDataOriginal.bits_per_sample);
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    std::wstringstream ssLaTeXSummary = std::wstringstream();
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    for (int j = 0; j < 2; ++j)
    {
        // -------------------------------------------------------------------------- //
        // 
        // -------------------------------------------------------------------------- //
        ssLaTeXSummary << L"\\pgfplotstableread{" << L"\n";
        ssLaTeXSummary << L"section\ty\ty-min\ty-max" << L"\n";
        // -------------------------------------------------------------------------- //
        // 
        // -------------------------------------------------------------------------- //
        for (int i = 0; i < 10; ++i)
        {
            switch (i)
            {
            case 1:
            case 2:
            case 3:
                if (0 == j)
                {
                    // skip literal
                    continue;
                }
                break;
            default:
                break;
            }
            // -------------------------------------------------------------------------- //
            // 
            // -------------------------------------------------------------------------- //
            ns_dt::t_data_for_estimator* pData = nullptr;
            switch (j)
            {
            case 0:
                pData = &io_refDataOriginal;
                break;
            case 1:
                pData = &io_refDataBinary;
                break;
            default:
                break;
            }
            // -------------------------------------------------------------------------- //
            // 
            // -------------------------------------------------------------------------- //
            double* p_min_entropy = nullptr;
            double* p_min_entropy_lower_bound = nullptr;
            double* p_min_entropy_upper_bound = nullptr;
            switch (i)
            {
            case 0:
                p_min_entropy = &(pData->t_6_3_1.t_common.min_entropy);
                p_min_entropy_lower_bound = &(pData->t_6_3_1.t_common.min_entropy_lower_bound);
                p_min_entropy_upper_bound = &(pData->t_6_3_1.t_common.min_entropy_upper_bound);
                break;
            case 1:
                p_min_entropy = &(pData->t_6_3_2.t_common.min_entropy);
                p_min_entropy_lower_bound = &(pData->t_6_3_2.t_common.min_entropy_lower_bound);
                p_min_entropy_upper_bound = &(pData->t_6_3_2.t_common.min_entropy_upper_bound);
                break;
            case 2:
                p_min_entropy = &(pData->t_6_3_3.t_common.min_entropy);
                p_min_entropy_lower_bound = &(pData->t_6_3_3.t_common.min_entropy_lower_bound);
                p_min_entropy_upper_bound = &(pData->t_6_3_3.t_common.min_entropy_upper_bound);
                break;
            case 3:
                p_min_entropy = &(pData->t_6_3_4.t_common.min_entropy);
                p_min_entropy_lower_bound = &(pData->t_6_3_4.t_common.min_entropy_lower_bound);
                p_min_entropy_upper_bound = &(pData->t_6_3_4.t_common.min_entropy_upper_bound);
                break;
            case 4:
                p_min_entropy = &(pData->t_6_3_5.t_common.min_entropy);
                p_min_entropy_lower_bound = &(pData->t_6_3_5.t_common.min_entropy_lower_bound);
                p_min_entropy_upper_bound = &(pData->t_6_3_5.t_common.min_entropy_upper_bound);
                break;
            case 5:
                p_min_entropy = &(pData->t_6_3_6.t_common.min_entropy);
                p_min_entropy_lower_bound = &(pData->t_6_3_6.t_common.min_entropy_lower_bound);
                p_min_entropy_upper_bound = &(pData->t_6_3_6.t_common.min_entropy_upper_bound);
                break;
            case 6:
                p_min_entropy = &(pData->t_6_3_7.t_common.min_entropy);
                p_min_entropy_lower_bound = &(pData->t_6_3_7.t_common.min_entropy_lower_bound);
                p_min_entropy_upper_bound = &(pData->t_6_3_7.t_common.min_entropy_upper_bound);
                break;
            case 7:
                p_min_entropy = &(pData->t_6_3_8.t_common.min_entropy);
                p_min_entropy_lower_bound = &(pData->t_6_3_8.t_common.min_entropy_lower_bound);
                p_min_entropy_upper_bound = &(pData->t_6_3_8.t_common.min_entropy_upper_bound);
                break;
            case 8:
                p_min_entropy = &(pData->t_6_3_9.t_common.min_entropy);
                p_min_entropy_lower_bound = &(pData->t_6_3_9.t_common.min_entropy_lower_bound);
                p_min_entropy_upper_bound = &(pData->t_6_3_9.t_common.min_entropy_upper_bound);
                break;
            case 9:
                p_min_entropy = &(pData->t_6_3_10.t_common.min_entropy);
                p_min_entropy_lower_bound = &(pData->t_6_3_10.t_common.min_entropy_lower_bound);
                p_min_entropy_upper_bound = &(pData->t_6_3_10.t_common.min_entropy_upper_bound);
                break;
            default:
                return  sts;
            }
            // -------------------------------------------------------------------------- //
            // 
            // -------------------------------------------------------------------------- //
            if (nullptr != p_min_entropy)
            {
                switch (j)
                {
                case 0:
                    if (*p_min_entropy < min_entropy_literal)
                    {
                        min_entropy_literal = *p_min_entropy;
                    }
                    break;
                case 1:
                    if (*p_min_entropy < min_entropy_bitstring)
                    {
                        min_entropy_bitstring = *p_min_entropy;
                    }
                    break;
                default:
                    break;
                }
            }
            // -------------------------------------------------------------------------- //
            // summary
            // -------------------------------------------------------------------------- //
            {
                ssLaTeXSummary << L"6.3." << i + 1;
                ssLaTeXSummary << L"\t" << std::setw(8) << *p_min_entropy << L"\t";
                if (0.0 != *p_min_entropy_lower_bound)
                {
                    ssLaTeXSummary << std::setw(8) << std::fabs(*p_min_entropy_lower_bound - *p_min_entropy);
                }
                else
                {
                    ssLaTeXSummary << std::setw(8) << 0.0;
                }
                ssLaTeXSummary << L"\t";
                if (0.0 != *p_min_entropy_upper_bound)
                {
                    ssLaTeXSummary << std::setw(8) << std::fabs(*p_min_entropy_upper_bound - *p_min_entropy) << L"\n";
                }
                else
                {
                    ssLaTeXSummary << std::setw(8) << 0.0 << L"\n";
                }
            }
        }
        ssLaTeXSummary << L"}{\\summarytable" ;
        switch (j)
        {
        case 0:
            ssLaTeXSummary << L"Non";
            break;
        case 1:
            break;
        default:
            break;
        }
        ssLaTeXSummary << L"Binary}" << L"\n";
    }
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    ssLaTeXSummary << L"\\begin{table}[h]" << L"\n";
    ssLaTeXSummary << L"\\caption{Numerical results}" << L"\n";
    ssLaTeXSummary << L"\\begin{center}" << L"\n";
    ssLaTeXSummary << L"\\begin{tabular}{|l|c|c|c|c|}" << L"\n";
    ssLaTeXSummary << L"\\hline " << L"\n";
    ssLaTeXSummary << L"\\rowcolor{anotherlightblue} %%" << L"\n";
    ssLaTeXSummary << L"Estimator										& $H_{\\textrm{original}}$$^{\\textrm{\\,a}}$			& Notes to $H_{\\textrm{original}}$  & $H_{\\textrm{bitstring}}$$^{\\textrm{\\,b}}$	& Notes to $H_{\\textrm{bitstring}}$			\\\\ " << L"\n";
    ssLaTeXSummary << L"\\cline{2-5}" << L"\n";
    ssLaTeXSummary << L"\\rowcolor{anotherlightblue} %%" << L"\n";
    ssLaTeXSummary << L"\\,												& [bit / "<< io_refDataOriginal.bits_per_sample << L" - bit] & \\, & [bit / 1 - bit] &	\\,	\\\\" << L"\n";
    ssLaTeXSummary << L"\\hline " << L"\n";
    ssLaTeXSummary << L"The Most Common Value Estimate					& " << io_refDataOriginal.t_6_3_1.t_common.min_entropy << L"& see \\ref{sec:NonBinary631} & " << io_refDataBinary.t_6_3_1.t_common.min_entropy << L"& see \\ref{sec:Binary631} " << L"\\\\" << L"\n";
    ssLaTeXSummary << L"\\hline " << L"\n";
    ssLaTeXSummary << L"The Collision Estimate							& ---		  & --- & " << io_refDataBinary.t_6_3_2.t_common.min_entropy << L"& see \\ref{sec:Binary632} " << L"\\\\" << L"\n";
    ssLaTeXSummary << L"\\hline " << L"\n";
    ssLaTeXSummary << L"The Markov Estimate								& ---		  & --- & " << io_refDataBinary.t_6_3_3.t_common.min_entropy << L"& see \\ref{sec:Binary633} " << L"\\\\" << L"\n";
    ssLaTeXSummary << L"\\hline " << L"\n";
    ssLaTeXSummary << L"The Compression Estimate						& ---		  & --- & " << io_refDataBinary.t_6_3_4.t_common.min_entropy << L"& see \\ref{sec:Binary634} " << L"\\\\" << L"\n";
    ssLaTeXSummary << L"\\hline " << L"\n";
    ssLaTeXSummary << L"The t-Tuple Estimate							& " << io_refDataOriginal.t_6_3_5.t_common.min_entropy << L"& see \\ref{sec:NonBinary635} & " << io_refDataBinary.t_6_3_5.t_common.min_entropy << L"& see \\ref{sec:Binary635} " << L"\\\\" << L"\n";
    ssLaTeXSummary << L"\\hline " << L"\n";
    ssLaTeXSummary << L"The Longest Repeated Substring (LRS) Estimate	& " << io_refDataOriginal.t_6_3_6.t_common.min_entropy << L"& see \\ref{sec:NonBinary636} & " << io_refDataBinary.t_6_3_6.t_common.min_entropy << L"& see \\ref{sec:Binary636} " << L"\\\\" << L"\n";
    ssLaTeXSummary << L"\\hline " << L"\n";
    ssLaTeXSummary << L"Multi Most Common in Window Prediction Estimate	& " << io_refDataOriginal.t_6_3_7.t_common.min_entropy << L"& see \\ref{sec:NonBinary637} & " << io_refDataBinary.t_6_3_7.t_common.min_entropy << L"& see \\ref{sec:Binary637} " << L"\\\\" << L"\n";
    ssLaTeXSummary << L"\\hline " << L"\n";
    ssLaTeXSummary << L"The Lag Prediction Estimate						& " << io_refDataOriginal.t_6_3_8.t_common.min_entropy << L"& see \\ref{sec:NonBinary638} & " << io_refDataBinary.t_6_3_8.t_common.min_entropy << L"& see \\ref{sec:Binary638} " << L"\\\\" << L"\n";
    ssLaTeXSummary << L"\\hline " << L"\n";
    ssLaTeXSummary << L"The MultiMMC Prediction Estimate				& " << io_refDataOriginal.t_6_3_9.t_common.min_entropy << L"& see \\ref{sec:NonBinary639} & " << io_refDataBinary.t_6_3_9.t_common.min_entropy << L"& see \\ref{sec:Binary639} " << L"\\\\" << L"\n";
    ssLaTeXSummary << L"\\hline " << L"\n";
    ssLaTeXSummary << L"The LZ78Y Prediction Estimate					& " << io_refDataOriginal.t_6_3_10.t_common.min_entropy << L"& see \\ref{sec:NonBinary6310} &" << io_refDataBinary.t_6_3_10.t_common.min_entropy << L"& see \\ref{sec:Binary6310} " << L"\\\\" << L"\n";
    ssLaTeXSummary << L"\\hline \\hline " << L"\n";
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    {
        double  min_entropy_global = min_entropy_literal;
        if (static_cast<double>(io_refDataOriginal.bits_per_sample) * min_entropy_bitstring < min_entropy_literal)
        {
            min_entropy_global = static_cast<double>(io_refDataOriginal.bits_per_sample) * min_entropy_bitstring;
        }
        ssLaTeXSummary << L"The intial entropy source estimate [bit / " << io_refDataOriginal.bits_per_sample << L" - bit]	& \\multicolumn{4}{|c|}{" << min_entropy_global << L"}	\\\\" << L"\n";
    }
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    ssLaTeXSummary << L"$H_{I} = \\min (H_{\\textrm{original}}, " << io_refDataOriginal.bits_per_sample << L"\\times H_{\\textrm{bitstring}})$ &\\multicolumn{4}{ | c | } {\\, }	\\\\" << L"\n";
    ssLaTeXSummary << L"\\hline \\hline " << L"\n";
    ssLaTeXSummary << L"\\multicolumn{5}{|l|}{$^{\\,a}$\\quad Entropy estimate of the sequential dataset [source: NIST SP 800-90B \\cite{SP80090B} 3.1.3]} \\\\" << L"\n";
    ssLaTeXSummary << L"\\multicolumn{5}{|l|}{$^{\\,b}$\\quad An additional entropy estimation (per bit) for the non-binary sequential dataset [see NIST SP 800-90B \\cite{SP80090B} 3.1.3]} \\\\" << L"\n";
    ssLaTeXSummary << L"\\hline " << L"\n";
    ssLaTeXSummary << L"\\end{tabular}" << L"\n";
    ssLaTeXSummary << L"\\end{center}" << L"\n";
    ssLaTeXSummary << L"\\end{table}" << L"\n";
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    ssLaTeXSummary << L"\\clearpage" << L"\n";
    // -------------------------------------------------------------------------- //
    // prepend subsection 
    // -------------------------------------------------------------------------- //
    std::wstring	strSubsectionOriginal = std::wstring();
    std::wstring	strSubsectionTitleOriginal = std::wstring(L"Visual comparison of min-entropy estimates from original samples");
    std::wstring	strLabel = std::wstring(L"sec:VisualComparison-original");
    ns_spt::getLaTeXSubsection(strSubsectionOriginal, strSubsectionTitleOriginal, strLabel);
    ssLaTeXSummary << strSubsectionOriginal << L"\n";
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    loadPGFPlotSummary(ssLaTeXSummary, false, min_entropy_literal, io_refDataOriginal.bits_per_sample);
    // -------------------------------------------------------------------------- //
    // prepend subsection 
    // -------------------------------------------------------------------------- //
    std::wstring	strSubsectionBitstring = std::wstring();
    std::wstring	strSubsectionTitleBitstring = std::wstring(L"Visual comparison of min-entropy estimates by interpreting each sample as bitstring");
    strLabel = std::wstring(L"sec:VisualComparison-bitstring");
    ns_spt::getLaTeXSubsection(strSubsectionBitstring, strSubsectionTitleBitstring, strLabel);
    ssLaTeXSummary << strSubsectionBitstring << L"\n";
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    loadPGFPlotSummary(ssLaTeXSummary, true, min_entropy_bitstring, io_refDataBinary.bits_per_sample);
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    std::wstringstream ssLaTeXSupportingInfo = std::wstringstream();
    reportLaTeXSupportingInfo(ssLaTeXSupportingInfo, i_refInfoReport, io_refDataOriginal);
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    std::wstringstream ssLaTeX = std::wstringstream();
    loadLaTeXPreamble(ssLaTeX);
    ssLaTeX << L"\\begin{document}" << L"\n";
    ssLaTeX << L"\\StopCensoring" << L"\n";
    ssLaTeX << L"\\maketitle" << L"\n";
	// -------------------------------------------------------------------------- //
	// 
	// -------------------------------------------------------------------------- //
	std::wstring	strCommentDefault = std::wstring();
	std::wstring	strNull = std::wstring();

	getLaTeXComment(strCommentDefault, strNull);
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    ssLaTeX << strCommentDefault;
    ssLaTeX << L"\\thispagestyle{mypagestylewithtotalpagenumbers}" << L"\n";
    ssLaTeX << strCommentDefault;
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    ssLaTeX << ssLaTeXSupportingInfo.rdbuf();
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    ssLaTeX << L"\\clearpage" << L"\n";
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    std::wstring	strSectionExecutiveSummary = std::wstring();
    std::wstring	strSectionExecutiveSummaryTitle = std::wstring(L"Executive summary");
    strLabel = std::wstring(L"seq:ExecSummary");
    ns_spt::getLaTeXSection(strSectionExecutiveSummary, strSectionExecutiveSummaryTitle, strLabel);
    ssLaTeX << strSectionExecutiveSummary;
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    std::wstring	strSubsectionNumericalResutls = std::wstring();
    std::wstring	strSubsectionTitleNumericalResutls = std::wstring(L"Numerical results of min-entropy estimates based on non-IID track");
    strLabel = std::wstring(L"seq:ExecSummary-numericalresults");
    ns_spt::getLaTeXSubsection(strSubsectionNumericalResutls, strSubsectionTitleNumericalResutls, strLabel);
    ssLaTeX << strSubsectionNumericalResutls;
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    ssLaTeX << ssLaTeXSummary.rdbuf();
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    ssLaTeX << L"\\clearpage" << L"\n";
    std::wstring	strSectionDetailedResultsOriginal = std::wstring();
    std::wstring	strSectionTitleDetailedResultsOriginal = std::wstring(L"Detailed results of analysis from original samples");
    strLabel = std::wstring(L"seq:DetailResultsOriginal");
    ns_spt::getLaTeXSection(strSectionDetailedResultsOriginal, strSectionTitleDetailedResultsOriginal, strLabel);
    ssLaTeX << strSectionDetailedResultsOriginal;
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    ssLaTeX << io_refDataOriginal.p_ssLaTeXFragment->rdbuf();
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    ssLaTeX << L"\\clearpage" << L"\n";
    std::wstring	strSectionDetailedResultsBitstring = std::wstring();
    std::wstring	strSectionTitleDetailedResultsBitstring = std::wstring(L"Detailed results of analysis by interpreting each sample as bitstrings");
    strLabel = std::wstring(L"seq:DetailResultsBitstring");
    ns_spt::getLaTeXSection(strSectionDetailedResultsBitstring, strSectionTitleDetailedResultsBitstring, strLabel);
    ssLaTeX << strSectionDetailedResultsBitstring;
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    ssLaTeX << io_refDataBinary.p_ssLaTeXFragment->rdbuf();
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    std::wstringstream ssLaTeXBiblio = std::wstringstream();
    loadLaTeXBibliography(ssLaTeXBiblio);
    ssLaTeX << ssLaTeXBiblio.rdbuf();
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    ssLaTeX << L"\\end{document}" << L"\n";
    // -------------------------------------------------------------------------- //
    //
    // -------------------------------------------------------------------------- //
    boost::filesystem::wofstream	file(the_report_path_LaTeX);
    if (!file)
    {
        return sts = ns_consts::EnmReturnStatus::ErrorFileIO;
    }
    file << ssLaTeX.rdbuf();
    file.close();
    // -------------------------------------------------------------------------- //
    // Output supplementary information to compile XeLateX source file
    // -------------------------------------------------------------------------- //
    showLaTeXCompilationHint(the_report_path_LaTeX);
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    return  sts = ns_consts::EnmReturnStatus::Success;
}

// -------------------------------------------------------------------------- //
/// <summary>
/// </summary>
/// <remarks>
/// </remarks>
/// <param name="i_refInfoReport">
/// </param>
/// <param name="io_refDataBinary">
/// </param>
/// <returns>
/// </returns>
/// <precondition>
/// </precondition>
/// <postcondition>
/// </postcondition>
// -------------------------------------------------------------------------- //
ns_consts::EnmReturnStatus reportLaTeXBinary(IDInfoForReport& i_refInfoReport,
    ns_dt::t_data_for_estimator& io_refDataBinary)
{
    ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorUnexpected;

    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    if (nullptr == i_refInfoReport.info_source.p_path_to_entropy_input)
    {
        return  sts = ns_consts::EnmReturnStatus::ErrorNullPointer;
    }
    bs_fs::path the_report_path_LaTeX;
    sts = synthesizeReportPathTex(the_report_path_LaTeX, *i_refInfoReport.info_source.p_path_to_entropy_input);
    if (ns_consts::EnmReturnStatus::Success != sts)
    {
        return  sts;
    }
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    double  min_entropy_bitstring = 1.0;
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    std::wstringstream ssLaTeXSummary = std::wstringstream();
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    ssLaTeXSummary << L"\\pgfplotstableread{" << L"\n";
    ssLaTeXSummary << L"section\ty\ty-min\ty-max" << L"\n";
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    for (int i = 0; i < 10; ++i)
    {
        // -------------------------------------------------------------------------- //
        // 
        // -------------------------------------------------------------------------- //
        double* p_min_entropy = nullptr;
        double* p_min_entropy_lower_bound = nullptr;
        double* p_min_entropy_upper_bound = nullptr;
        switch (i)
        {
        case 0:
            p_min_entropy = &(io_refDataBinary.t_6_3_1.t_common.min_entropy);
            p_min_entropy_lower_bound = &(io_refDataBinary.t_6_3_1.t_common.min_entropy_lower_bound);
            p_min_entropy_upper_bound = &(io_refDataBinary.t_6_3_1.t_common.min_entropy_upper_bound);
            break;
        case 1:
            p_min_entropy = &(io_refDataBinary.t_6_3_2.t_common.min_entropy);
            p_min_entropy_lower_bound = &(io_refDataBinary.t_6_3_2.t_common.min_entropy_lower_bound);
            p_min_entropy_upper_bound = &(io_refDataBinary.t_6_3_2.t_common.min_entropy_upper_bound);
            break;
        case 2:
            p_min_entropy = &(io_refDataBinary.t_6_3_3.t_common.min_entropy);
            p_min_entropy_lower_bound = &(io_refDataBinary.t_6_3_3.t_common.min_entropy_lower_bound);
            p_min_entropy_upper_bound = &(io_refDataBinary.t_6_3_3.t_common.min_entropy_upper_bound);
            break;
        case 3:
            p_min_entropy = &(io_refDataBinary.t_6_3_4.t_common.min_entropy);
            p_min_entropy_lower_bound = &(io_refDataBinary.t_6_3_4.t_common.min_entropy_lower_bound);
            p_min_entropy_upper_bound = &(io_refDataBinary.t_6_3_4.t_common.min_entropy_upper_bound);
            break;
        case 4:
            p_min_entropy = &(io_refDataBinary.t_6_3_5.t_common.min_entropy);
            p_min_entropy_lower_bound = &(io_refDataBinary.t_6_3_5.t_common.min_entropy_lower_bound);
            p_min_entropy_upper_bound = &(io_refDataBinary.t_6_3_5.t_common.min_entropy_upper_bound);
            break;
        case 5:
            p_min_entropy = &(io_refDataBinary.t_6_3_6.t_common.min_entropy);
            p_min_entropy_lower_bound = &(io_refDataBinary.t_6_3_6.t_common.min_entropy_lower_bound);
            p_min_entropy_upper_bound = &(io_refDataBinary.t_6_3_6.t_common.min_entropy_upper_bound);
            break;
        case 6:
            p_min_entropy = &(io_refDataBinary.t_6_3_7.t_common.min_entropy);
            p_min_entropy_lower_bound = &(io_refDataBinary.t_6_3_7.t_common.min_entropy_lower_bound);
            p_min_entropy_upper_bound = &(io_refDataBinary.t_6_3_7.t_common.min_entropy_upper_bound);
            break;
        case 7:
            p_min_entropy = &(io_refDataBinary.t_6_3_8.t_common.min_entropy);
            p_min_entropy_lower_bound = &(io_refDataBinary.t_6_3_8.t_common.min_entropy_lower_bound);
            p_min_entropy_upper_bound = &(io_refDataBinary.t_6_3_8.t_common.min_entropy_upper_bound);
            break;
        case 8:
            p_min_entropy = &(io_refDataBinary.t_6_3_9.t_common.min_entropy);
            p_min_entropy_lower_bound = &(io_refDataBinary.t_6_3_9.t_common.min_entropy_lower_bound);
            p_min_entropy_upper_bound = &(io_refDataBinary.t_6_3_9.t_common.min_entropy_upper_bound);
            break;
        case 9:
            p_min_entropy = &(io_refDataBinary.t_6_3_10.t_common.min_entropy);
            p_min_entropy_lower_bound = &(io_refDataBinary.t_6_3_10.t_common.min_entropy_lower_bound);
            p_min_entropy_upper_bound = &(io_refDataBinary.t_6_3_10.t_common.min_entropy_upper_bound);
            break;
        default:
            return  sts;
        }
        // -------------------------------------------------------------------------- //
        // 
        // -------------------------------------------------------------------------- //
        if (nullptr != p_min_entropy)
        {
            if (*p_min_entropy < min_entropy_bitstring)
            {
                min_entropy_bitstring = *p_min_entropy;
            }
        }
        // -------------------------------------------------------------------------- //
        // summary
        // -------------------------------------------------------------------------- //
        {
            ssLaTeXSummary << L"6.3." << i + 1;
            ssLaTeXSummary << L"\t" << std::setw(8) << *p_min_entropy << L"\t";
            if (0.0 != *p_min_entropy_lower_bound)
            {
                ssLaTeXSummary << std::setw(8) << - (*p_min_entropy_lower_bound - *p_min_entropy);
            }
            else
            {
                ssLaTeXSummary << std::setw(8) << 0.0;
            }
            ssLaTeXSummary << L"\t";
            if (0.0 != *p_min_entropy_upper_bound)
            {
                ssLaTeXSummary << std::setw(8) << *p_min_entropy_upper_bound << L"\n";
            }
            else
            {
                ssLaTeXSummary << std::setw(8) << 0.0 << L"\n";
            }
        }
    }
    ssLaTeXSummary << L"}{\\summarytableBinary}" << L"\n";
    ssLaTeXSummary << L"\\begin{table}[h]" << L"\n";
    ssLaTeXSummary << L"\\caption{Numerical results}" << L"\n";
    ssLaTeXSummary << L"\\begin{center}" << L"\n";
    ssLaTeXSummary << L"\\begin{tabular}{|l|c|c|}" << L"\n";
    ssLaTeXSummary << L"\\hline " << L"\n";
    ssLaTeXSummary << L"\\rowcolor{anotherlightblue} %%" << L"\n";
    ssLaTeXSummary << L"Estimator										& $H_{\\textrm{bitstring}}$$^{\\textrm{\\,a}}$ & Notes to $H_{\\textrm{bitstring}}$	\\\\ " << L"\n";
    ssLaTeXSummary << L"\\cline{2-3}" << L"\n";
    ssLaTeXSummary << L"\\rowcolor{anotherlightblue} %%" << L"\n";
    ssLaTeXSummary << L"\\,												& [bit / 1 - bit] & \\,		\\\\" << L"\n";
    ssLaTeXSummary << L"\\hline " << L"\n";
    ssLaTeXSummary << L"The Most Common Value Estimate					& " << io_refDataBinary.t_6_3_1.t_common.min_entropy << L"& see \\ref{sec:Binary631} " << L"\\\\" << L"\n";
    ssLaTeXSummary << L"\\hline " << L"\n";
    ssLaTeXSummary << L"The Collision Estimate							& " << io_refDataBinary.t_6_3_2.t_common.min_entropy << L"& see \\ref{sec:Binary632} " << L"\\\\" << L"\n";
    ssLaTeXSummary << L"\\hline " << L"\n";
    ssLaTeXSummary << L"The Markov Estimate								& " << io_refDataBinary.t_6_3_3.t_common.min_entropy << L"& see \\ref{sec:Binary633} " << L"\\\\" << L"\n";
    ssLaTeXSummary << L"\\hline " << L"\n";
    ssLaTeXSummary << L"The Compression Estimate						& " << io_refDataBinary.t_6_3_4.t_common.min_entropy << L"& see \\ref{sec:Binary634} " << L"\\\\" << L"\n";
    ssLaTeXSummary << L"\\hline " << L"\n";
    ssLaTeXSummary << L"The t-Tuple Estimate							& " << io_refDataBinary.t_6_3_5.t_common.min_entropy << L"& see \\ref{sec:Binary635} " << L"\\\\" << L"\n";
    ssLaTeXSummary << L"\\hline " << L"\n";
    ssLaTeXSummary << L"The Longest Repeated Substring (LRS) Estimate	& " << io_refDataBinary.t_6_3_6.t_common.min_entropy << L"& see \\ref{sec:Binary636} " << L"\\\\" << L"\n";
    ssLaTeXSummary << L"\\hline " << L"\n";
    ssLaTeXSummary << L"Multi Most Common in Window Prediction Estimate	& " << io_refDataBinary.t_6_3_7.t_common.min_entropy << L"& see \\ref{sec:Binary637} " << L"\\\\" << L"\n";
    ssLaTeXSummary << L"\\hline " << L"\n";
    ssLaTeXSummary << L"The Lag Prediction Estimate						& " << io_refDataBinary.t_6_3_8.t_common.min_entropy << L"& see \\ref{sec:Binary638} " << L"\\\\" << L"\n";
    ssLaTeXSummary << L"\\hline " << L"\n";
    ssLaTeXSummary << L"The MultiMMC Prediction Estimate				& " << io_refDataBinary.t_6_3_9.t_common.min_entropy << L"& see \\ref{sec:Binary639} " << L"\\\\" << L"\n";
    ssLaTeXSummary << L"\\hline " << L"\n";
    ssLaTeXSummary << L"The LZ78Y Prediction Estimate					& " << io_refDataBinary.t_6_3_10.t_common.min_entropy << L"& see \\ref{sec:Binary6310} " << L"\\\\" << L"\n";
    ssLaTeXSummary << L"\\hline \\hline " << L"\n";
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    ssLaTeXSummary << L"The intial entropy source estimate [bit / " << io_refDataBinary.bits_per_sample << L" -bit]	& \\multicolumn{2}{|c|}{" << min_entropy_bitstring << L"}	\\\\" << L"\n";
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    ssLaTeXSummary << L"$H_{I} = H_{\\textrm{bitstring}}$ & \\multicolumn{2}{|c|}{ \\, } 	\\\\" << L"\n";
    ssLaTeXSummary << L"\\hline \\hline " << L"\n";
    ssLaTeXSummary << L"\\multicolumn{3}{|l|}{$^{\\,a}$\\quad Entropy estimate of the sequential dataset [source: NIST SP 800-90B \\cite{SP80090B} 3.1.3]} \\\\" << L"\n";
    ssLaTeXSummary << L"\\hline " << L"\n";
    ssLaTeXSummary << L"\\end{tabular}" << L"\n";
    ssLaTeXSummary << L"\\end{center}" << L"\n";
    ssLaTeXSummary << L"\\end{table}" << L"\n";
    // -------------------------------------------------------------------------- //
    // prepend subsection 
    // -------------------------------------------------------------------------- //
    std::wstring	strSubsectionBinary = std::wstring();
    std::wstring	strSubsectionTitleBinary = std::wstring(L"Visual comparison of min-entropy estimates from binary samples");
    std::wstring	strLabel = std::wstring(L"sec:VisualComparison-binary");
    ns_spt::getLaTeXSubsection(strSubsectionBinary, strSubsectionTitleBinary, strLabel);
    ssLaTeXSummary << strSubsectionBinary << L"\n";
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    loadPGFPlotSummary(ssLaTeXSummary, true, min_entropy_bitstring, 1.0);
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    std::wstringstream ssLaTeXSupportingInfo = std::wstringstream();
    reportLaTeXSupportingInfo(ssLaTeXSupportingInfo, i_refInfoReport, io_refDataBinary);
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    std::wstringstream ssLaTeX = std::wstringstream();
    loadLaTeXPreamble(ssLaTeX);
    ssLaTeX << L"\\begin{document}" << L"\n";
    ssLaTeX << L"\\StopCensoring" << L"\n";
    ssLaTeX << L"\\maketitle" << L"\n";
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    std::wstring	strCommentDefault = std::wstring();
    std::wstring	strNull = std::wstring();

    getLaTeXComment(strCommentDefault, strNull);
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    ssLaTeX << strCommentDefault;
    ssLaTeX << L"\\thispagestyle{mypagestylewithtotalpagenumbers}" << L"\n";
    ssLaTeX << strCommentDefault;
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    ssLaTeX << ssLaTeXSupportingInfo.rdbuf();
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    ssLaTeX << L"\\clearpage" << L"\n";
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    std::wstring	strSectionExecutiveSummary = std::wstring();
    std::wstring	strSectionExecutiveSummaryTitle = std::wstring(L"Executive summary");
    strLabel = std::wstring(L"seq:ExecSummary");
    ns_spt::getLaTeXSection(strSectionExecutiveSummary, strSectionExecutiveSummaryTitle, strLabel);
    ssLaTeX << strSectionExecutiveSummary;
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    std::wstring	strSubsectionNumericalResutls = std::wstring();
    std::wstring	strSubsectionTitle = std::wstring(L"Numerical results of min-entropy estimates based on non-IID track");
    strLabel = std::wstring(L"seq:ExecSummary-numericalresults");
    ns_spt::getLaTeXSubsection(strSubsectionNumericalResutls, strSubsectionTitle, strLabel);
    ssLaTeX << strSubsectionNumericalResutls;
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    ssLaTeX << ssLaTeXSummary.rdbuf();
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    ssLaTeX << L"\\clearpage" << L"\n";
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    std::wstring	strSectionDetailedResultsOriginal = std::wstring();
    std::wstring	strSectionTitleDetailedResultsOriginal = std::wstring(L"Detailed results of analysis from original samples");
    strLabel = std::wstring(L"seq:DetailResultsOriginal");
    ns_spt::getLaTeXSection(strSectionDetailedResultsOriginal, strSectionTitleDetailedResultsOriginal, strLabel);
    ssLaTeX << strSectionDetailedResultsOriginal;
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    ssLaTeX << io_refDataBinary.p_ssLaTeXFragment->rdbuf();
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    std::wstringstream ssLaTeXBiblio = std::wstringstream();
    loadLaTeXBibliography(ssLaTeXBiblio);
    ssLaTeX << ssLaTeXBiblio.rdbuf();
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    ssLaTeX << L"\\end{document}" << L"\n";
    // -------------------------------------------------------------------------- //
    //
    // -------------------------------------------------------------------------- //
    boost::filesystem::wofstream	file(the_report_path_LaTeX);
    if (!file)
    {
        return sts = ns_consts::EnmReturnStatus::ErrorFileIO;
    }
    file << ssLaTeX.rdbuf();
    file.close();
    // -------------------------------------------------------------------------- //
    // Output supplementary information to compile XeLateX source file
    // -------------------------------------------------------------------------- //
    showLaTeXCompilationHint(the_report_path_LaTeX);
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    return  sts = ns_consts::EnmReturnStatus::Success;
}
