////////////////////////////////////////////////////////////////////////////////
// reporter.cpp
//
//
//
// Copyright (c) 2021-2022 G. G. SAKURAI <g.garland823@gmail.com>
//
////////////////////////////////////////////////////////////////////////////////
#include <string>
#include "reporter.h"
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>
#include "boost/date_time/posix_time/posix_time.hpp"
#include "EntEstimateLib/support/enumerateAlphabet.h"
#include "EntEstimateLib/support/setUp.h"
#include <boost/filesystem/fstream.hpp>
#include <vector>
#include <sstream>
#include <ctime>
#include <locale> 
#include <codecvt> 

namespace po = boost::program_options;
namespace bs_fs = boost::filesystem;

namespace ns_consts = entropy_estimator_lib::constants;
namespace ns_dt = entropy_estimator_lib::data_types;

// -------------------------------------------------------------------------- //
/// <summary>
/// </summary>
/// <remarks>
/// </remarks>
/// <params="i_refData">
/// </params>
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
    int total_length = i_refData.p_bzInputS->length(blitz::firstDim);
    int num_bytes = 64;
    if (total_length < 64)
    {
        num_bytes = total_length;
    }
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    std::cout << std::endl;
    std::cout << "The following are the first " << num_bytes << "-byte samples, in hexadecimal, of the specified file for confirmation:" << std::endl;
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    std::cout.setf(std::ios::hex, std::ios::basefield);
    char chFillSaved = std::cout.fill('0');
    for (int j = 0; j < num_bytes; ++j)
    {
        std::cout << std::setw(2) << (int)(*i_refData.p_bzInputS)(j) << ", ";
        if ((j != 0) && (15 == j % 16))
        {
            std::cout << std::endl;
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
/// <params="i_refData">
/// </params>
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
    int total_length = i_refData.p_bzInputS->length(blitz::firstDim);
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
    std::cout << std::endl;
    std::cout << "The following are the last " << num_bytes << "-byte samples, in hexadecimal, of the specified file for confirmation:" << std::endl;
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    std::cout.setf(std::ios::hex, std::ios::basefield);
    char chFillSaved = std::cout.fill('0');
    for (int j = 0; j < num_bytes; ++j)
    {
        std::cout << std::setw(2) << (int)(*i_refData.p_bzInputS)(offset + j) << ", ";
        if ((j != 0) && (15 == j % 16))
        {
            std::cout << std::endl;
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
    int     bits_per_sample;
    bool    bIsSolutionFound;

    MinEntropy() : value(0.0), estimator_info(ns_consts::EnmNonIIDTrack::EstimatorMostCommonValue), sample_interpret(ns_consts::EnmSampleInterpretation::ELiteralNonBinary), bits_per_sample(8), bIsSolutionFound(true){};

    MinEntropy(double val, ns_consts::EnmNonIIDTrack i_est, ns_consts::EnmSampleInterpretation i_si, int i_bits, bool i_bIsSolutionFound)
        : value(val), estimator_info(i_est), sample_interpret(i_si), bits_per_sample(i_bits), bIsSolutionFound(i_bIsSolutionFound){};

    std::wstring getUnit(void) const
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
/// <params="i_est">
/// </params>
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
/// <params="i_refData">
/// </params>
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
/// <params="i_refInfoReport">
/// </params>
/// <params="io_refDataOriginal">
/// </params>
/// <params="io_refDataBinary">
/// </params>
/// <returns>
/// </returns>
/// <precondition>
/// </precondition>
/// <postcondition>
/// </postcondition>
// -------------------------------------------------------------------------- //
ns_consts::EnmReturnStatus reportXMLNonBinary(IDInfoForReport& i_refInfoReport,
    ns_dt::t_data_for_estimator& io_refDataOriginal, ns_dt::t_data_for_estimator& io_refDataBinary)
{
    ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorUnexpected;

    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    ns_consts::EnmNonIIDTrack   enm_estimate_ids_non_binary[] = {
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
    double  min_entropy_literal = (double)io_refDataOriginal.bits_per_sample;
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
                break;
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

            vec_me.push_back(MinEntropy(*p_min_entropy, enm_estimate_ids_non_binary[i], enm_interp[j], pData->bits_per_sample, bIsSolutionFound));
        }
    }
    // -------------------------------------------------------------------------- //
    // <identification><source>
    // -------------------------------------------------------------------------- //
    {
        wchar_t buffer[128];
        memset(buffer, 0, sizeof(buffer));
        the_tree.add(L"entropy_report.identification.source.path", i_refInfoReport.info_source.p_path_to_entropy_input->wstring());

        struct tm newtime;

        errno_t err = localtime_s(&newtime, &(i_refInfoReport.info_source.tm_last_write_time));
        if (err)
        {
            return  sts = ns_consts::EnmReturnStatus::ErrorInvalidData;
        }

        the_tree.add(L"entropy_report.identification.source.last_write_time", std::put_time(&newtime, L"%Y-%b-%d %H:%M:%S"));
        the_tree.add(L"entropy_report.identification.source.bits_per_sample", std::to_wstring(io_refDataOriginal.bits_per_sample));
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
    if (nullptr != i_refInfoReport.info_env.p_username)
    {
        the_tree.add(L"entropy_report.identification.environment.username", *i_refInfoReport.info_env.p_username);
    }
    // -------------------------------------------------------------------------- //
    // <summary>
    // -------------------------------------------------------------------------- //
    {
        double  min_entropy_global = min_entropy_literal;
        if ((double)io_refDataOriginal.bits_per_sample * min_entropy_bitstring < min_entropy_literal)
        {
            min_entropy_global = (double)io_refDataOriginal.bits_per_sample * min_entropy_bitstring;
        }
        boost::property_tree::wptree& child = the_tree.add(L"entropy_report.summary.min_entropy", std::to_wstring(min_entropy_global));
        if (1 <= vec_me.size())
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
            if (false == me_entry.bIsSolutionFound)
            {
                child.put(L"<xmlattr>.remark", "a solution was not found in step 7.");
            }
            break;
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
    const int indent = 4;
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
/// <params="i_refInfoReport">
/// </params>
/// <params="io_refDataBinary">
/// </params>
/// <returns>
/// </returns>
/// <precondition>
/// </precondition>
/// <postcondition>
/// </postcondition>
// -------------------------------------------------------------------------- //
ns_consts::EnmReturnStatus reportXMLBinary(IDInfoForReport& i_refInfoReport,
    ns_dt::t_data_for_estimator& io_refDataBinary)
{
    ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorUnexpected;

    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    ns_consts::EnmNonIIDTrack   enm_estimate_ids_binary[] = {
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
    sts = synthesizeReportPath(the_report_path, *i_refInfoReport.info_source.p_path_to_entropy_input);
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
            double* p_min_entropy = nullptr;
            switch (i)
            {
            case 0:
                p_min_entropy = &(io_refDataBinary.t_6_3_1.t_common.min_entropy);
                break;
            case 1:
                p_min_entropy = &(io_refDataBinary.t_6_3_2.t_common.min_entropy);
                break;
            case 2:
                p_min_entropy = &(io_refDataBinary.t_6_3_3.t_common.min_entropy);
                break;
            case 3:
                p_min_entropy = &(io_refDataBinary.t_6_3_4.t_common.min_entropy);
                break;
            case 4:
                p_min_entropy = &(io_refDataBinary.t_6_3_5.t_common.min_entropy);
                break;
            case 5:
                p_min_entropy = &(io_refDataBinary.t_6_3_6.t_common.min_entropy);
                break;
            case 6:
                p_min_entropy = &(io_refDataBinary.t_6_3_7.t_common.min_entropy);
                break;
            case 7:
                p_min_entropy = &(io_refDataBinary.t_6_3_8.t_common.min_entropy);
                break;
            case 8:
                p_min_entropy = &(io_refDataBinary.t_6_3_9.t_common.min_entropy);
                break;
            case 9:
                p_min_entropy = &(io_refDataBinary.t_6_3_10.t_common.min_entropy);
                break;
            default:
                return  sts;
                break;
            }
            // -------------------------------------------------------------------------- //
            // 
            // -------------------------------------------------------------------------- //
            bool    bIsSolutionFound = true;
            switch (i)
            {
            case 1:
                bIsSolutionFound = io_refDataBinary.t_6_3_2.bIsRootFound;
                break;
            case 4:
                bIsSolutionFound = io_refDataBinary.t_6_3_4.bIsRootFound;
                break;
            default:
                break;
            }

            vec_me.push_back(MinEntropy(*p_min_entropy, enm_estimate_ids_binary[i], ns_consts::EnmSampleInterpretation::ELiteralButBinary, io_refDataBinary.bits_per_sample, bIsSolutionFound));
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
        wchar_t buffer[128];
        memset(buffer, 0, sizeof(buffer));
        the_tree.add(L"entropy_report.identification.source.path", i_refInfoReport.info_source.p_path_to_entropy_input->wstring());

        struct tm newtime;

        errno_t err = localtime_s(&newtime, &(i_refInfoReport.info_source.tm_last_write_time));
        if (err)
        {
            return  sts = ns_consts::EnmReturnStatus::ErrorInvalidData;
        }

        the_tree.add(L"entropy_report.identification.source.last_write_time", std::put_time(&newtime, L"%Y-%b-%d %H:%M:%S"));
        the_tree.add(L"entropy_report.identification.source.bits_per_sample", std::to_wstring(io_refDataBinary.bits_per_sample));
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
    if (nullptr != i_refInfoReport.info_env.p_username)
    {
        the_tree.add(L"entropy_report.identification.environment.username", *i_refInfoReport.info_env.p_username);
    }
    // -------------------------------------------------------------------------- //
    // <summary>
    // -------------------------------------------------------------------------- //
    {
        boost::property_tree::wptree& child = the_tree.add(L"entropy_report.summary.min_entropy", std::to_wstring(min_entropy_bitstring));
        if (1 <= vec_me.size())
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
            if (false == me_entry.bIsSolutionFound)
            {
                child.put(L"<xmlattr>.remark", "a solution was not found in step 7.");
            }
            break;
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
    const int indent = 4;
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
/// <params="o_report_complete_path">
/// </params>
/// <params="i_path_entropy_input">
/// </params>
/// <returns>
/// </returns>
/// <precondition>
/// </precondition>
/// <postcondition>
/// </postcondition>
// -------------------------------------------------------------------------- //
ns_consts::EnmReturnStatus synthesizeReportPath(bs_fs::path& o_report_complete_path,
    bs_fs::path& i_path_entropy_input)
{
    ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorUnexpected;

    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    std::string strBaseFileName("entropy_report_");
    std::string strExtension(".xml");

    boost::posix_time::ptime pt =
        boost::posix_time::ptime(
            boost::posix_time::microsec_clock::local_time());

    std::string strTimeInfo(boost::posix_time::to_iso_string(pt));
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
/// <params="o_report_complete_path">
/// </params>
/// <params="i_path_entropy_input">
/// </params>
/// <returns>
/// </returns>
/// <precondition>
/// </precondition>
/// <postcondition>
/// </postcondition>
// -------------------------------------------------------------------------- //
ns_consts::EnmReturnStatus synthesizeReportPathTex(bs_fs::path& o_report_complete_path,
    bs_fs::path& i_path_entropy_input)
{
    ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorUnexpected;

    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    std::string strBaseFileName("entropy_report_");
    std::string strExtension(".tex");

    boost::posix_time::ptime pt =
        boost::posix_time::ptime(
            boost::posix_time::microsec_clock::local_time());

    std::string strTimeInfo(boost::posix_time::to_iso_string(pt));
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
/// <params="o_ssLaTeX">
/// </params>
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
    o_ssLaTeX << L"\\documentclass[a3paper,xelatex,english]{bxjsarticle}" << std::endl;
    o_ssLaTeX << L"\\usepackage{pgfplots,pgfplotstable}" << std::endl;
    o_ssLaTeX << L"\\pgfplotsset{ compat = newest }" << std::endl;
    o_ssLaTeX << L"\\usepackage{tikz}" << std::endl;
    o_ssLaTeX << L"\\usetikzlibrary{arrows.meta,bending,calc,shapes,positioning}" << std::endl;
    o_ssLaTeX << L"\\usepackage{ascmac}" << std::endl;
    o_ssLaTeX << L"\\usepackage{fancybox}" << std::endl;
    o_ssLaTeX << L"\\usepackage{amsmath,amssymb}" << std::endl;
    o_ssLaTeX << L"\\usepackage{algorithm}" << std::endl;
    o_ssLaTeX << L"\\usepackage[edges]{forest}" << std::endl;
    o_ssLaTeX << L"\\usepackage{array}" << std::endl;
    o_ssLaTeX << L"\\usepackage{algpseudocode}" << std::endl;
    o_ssLaTeX << L"\\usepackage{paralist}" << std::endl;
    o_ssLaTeX << L"\\usepackage{cases}" << std::endl;
    o_ssLaTeX << L"\\usepackage{fvextra}" << std::endl;
    o_ssLaTeX << L"\\usepackage{colortbl}" << std::endl;
    o_ssLaTeX << L"\\usepackage{xcolor}" << std::endl;
    o_ssLaTeX << L"\\usepackage{fancyhdr}" << std::endl;
    o_ssLaTeX << L"\\usepackage[explicit]{titlesec}" << std::endl;
    o_ssLaTeX << L"\\usepackage{xspace}" << std::endl;
    o_ssLaTeX << L"\\usepackage[many]{tcolorbox}" << std::endl;
    o_ssLaTeX << L"\\usepackage[unicode,pdftitle={Report of Entropy estimates based on NIST SP 800-90B non-IID track},setpagesize=false]{hyperref}" << std::endl;
    o_ssLaTeX << L"\\usepackage[open,openlevel=4]{bookmark}" << std::endl;
    o_ssLaTeX << L"\\newcommand\\mib[1]{\\boldsymbol{#1}}" << std::endl;
    o_ssLaTeX << L"\\usepgfplotslibrary{patchplots}" << std::endl;
    o_ssLaTeX << L"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << std::endl;
    o_ssLaTeX << L"%%%%%%" << std::endl;
    o_ssLaTeX << L"%%%%%% output up to 4-th level" << std::endl;
    o_ssLaTeX << L"%%%%%%" << std::endl;
    o_ssLaTeX << L"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << std::endl;
    o_ssLaTeX << L"\\setcounter{secnumdepth}{4}" << std::endl;
    o_ssLaTeX << L"\\setcounter{tocdepth}{4}" << std::endl;    
    o_ssLaTeX << L"\\setlength{\\topmargin}{-1cm}" << std::endl;
    o_ssLaTeX << L"\\setlength{\\textheight}{37cm}" << std::endl;
    o_ssLaTeX << L"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << std::endl;
    o_ssLaTeX << L"%%%%%%" << std::endl;
    o_ssLaTeX << L"%%%%%%" << std::endl;
    o_ssLaTeX << L"%%%%%%" << std::endl;
    o_ssLaTeX << L"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << std::endl;
    o_ssLaTeX << L"%%%\\renewcommand{ \\figurename }{Figure }" << std::endl;
    o_ssLaTeX << L"%%%\\renewcommand{ \\tablename }{Table }" << std::endl;
    o_ssLaTeX << L"%%%\\renewcommand{ \\refname }{References}" << std::endl;
    o_ssLaTeX << L"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << std::endl;
    o_ssLaTeX << L"%%%%%%" << std::endl;
    o_ssLaTeX << L"%%%%%%" << std::endl;
    o_ssLaTeX << L"%%%%%%" << std::endl;
    o_ssLaTeX << L"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << std::endl;
    o_ssLaTeX << L"\\definecolor{rowcolorlightblue}{RGB}{191,233,251}" << std::endl;
    o_ssLaTeX << L"\\definecolor{bordercolordarkblue}{RGB}{0,163,243}" << std::endl;
    o_ssLaTeX << L"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << std::endl;
    o_ssLaTeX << L"%%%%%%" << std::endl;
    o_ssLaTeX << L"%%%%%%" << std::endl;
    o_ssLaTeX << L"%%%%%%" << std::endl;
    o_ssLaTeX << L"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << std::endl;
    o_ssLaTeX << L"\\setlength{ \\topmargin }{-1.5cm}" << std::endl;
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    o_ssLaTeX << L"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << std::endl;
    o_ssLaTeX << L"%%%%%%" << std::endl;
    o_ssLaTeX << L"%%%%%%" << std::endl;
    o_ssLaTeX << L"%%%%%%" << std::endl;
    o_ssLaTeX << L"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << std::endl;
    o_ssLaTeX << L"\\title{Report of Entropy estimates based on NIST SP 800-90B non-IID track}" << std::endl;
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    boost::posix_time::ptime pt =
        boost::posix_time::ptime(
            boost::posix_time::microsec_clock::local_time());

    std::string strTimeInfo(boost::posix_time::to_simple_string(pt));

    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> cv;
    std::wstring wstrTimeInfo = cv.from_bytes(strTimeInfo);
    o_ssLaTeX << L"\\date{" << wstrTimeInfo << L"}" <<std::endl;
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
/// <params="o_ssLaTeX">
/// </params>
/// <params="isBinary">
/// </params>
/// <params="min_entropy">
/// </params>
/// <params="bits_per_sample">
/// </params>
/// <returns>
/// </returns>
/// <precondition>
/// </precondition>
/// <postcondition>
/// </postcondition>
// -------------------------------------------------------------------------- //
ns_consts::EnmReturnStatus loadPGFPlotSummary(std::wstringstream& o_ssLaTeX, bool isBinary, double min_entropy, int bits_per_sample)
{
    ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorUnexpected;

    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    o_ssLaTeX << L"\\begin{figure}[htbp]" << std::endl;
    o_ssLaTeX << L"\\begin{tikzpicture} " << std::endl;
    o_ssLaTeX << L"\\begin{axis}" << std::endl;
    o_ssLaTeX << L"\t[symbolic x coords={6.3.1,6.3.2,6.3.3,6.3.4,6.3.5,6.3.6,6.3.7,6.3.8,6.3.9,6.3.10}," << std::endl;
    o_ssLaTeX << L"\twidth=20cm," << std::endl;
    o_ssLaTeX << L"\tymin=0," << std::endl;
    o_ssLaTeX << L"\tymax=" << bits_per_sample << "," << std::endl;
    o_ssLaTeX << L"	xlabel=Sub-sub-section of NIST SP 800-90B," << std::endl;
    o_ssLaTeX << L"	ylabel={Estimated min-entropy $[$bit / " << bits_per_sample << L"-bit$]$}," << std::endl;
    o_ssLaTeX << L"\txtick=data]" << std::endl;
    o_ssLaTeX << L"\\addplot+[forget plot,only marks] " << std::endl;
    o_ssLaTeX << L"  plot[error bars/.cd, y dir=both, y explicit]" << std::endl;
    o_ssLaTeX << L"  table[x=section,y=y,y error plus expr=\\thisrow{y-max},y error minus expr=\\thisrow{y-min}] {\\summarytable";
    if (false == isBinary)
    {
        o_ssLaTeX << L"Non";
    }
    o_ssLaTeX << L"Binary};" << std::endl;
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    o_ssLaTeX << L"\\addplot+[teal,no marks,sharp plot,update limits=false] " << std::endl;
    o_ssLaTeX << L"coordinates {(6.3.1," << min_entropy << L") (6.3.10," << min_entropy << L")}" << std::endl;
    o_ssLaTeX << L"node[below] at (axis cs:6.3.5," << min_entropy << L") {Estimated min-entropy = " << std::endl;
    o_ssLaTeX << min_entropy << L"};" << std::endl;
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    o_ssLaTeX << L"\\end{axis} " << std::endl;
    o_ssLaTeX << L"\\end{tikzpicture}" << std::endl;
    o_ssLaTeX << L"\\caption{Estimated Min-Entropy using $\\S$6.3 of NIST SP 800-90B}" << std::endl;
    o_ssLaTeX << L"\\end{figure}" << std::endl;
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
/// <params="o_ssLaTeX">
/// </params>
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
    o_ssLaTeX << L"\\begin{thebibliography}{99}" << std::endl;
    o_ssLaTeX << L"% 1" << std::endl;
    o_ssLaTeX << L"\\bibitem{SP80090B}" << std::endl;
    o_ssLaTeX << L"Meltem S\\\"{o}nmez Turan," << std::endl;
    o_ssLaTeX << L"Elaine Barker," << std::endl;
    o_ssLaTeX << L"John Kelsey," << std::endl;
    o_ssLaTeX << L"Kerry A. McKay," << std::endl;
    o_ssLaTeX << L"Mary L. Baish," << std::endl;
    o_ssLaTeX << L"Mike Boyle" << std::endl;
    o_ssLaTeX << L"\\textit{Recommendation for the Entropy Sources Used for Random Bit Generation}," << std::endl;
    o_ssLaTeX << L"NIST Special Publication 800-90B, Jan. 2018" << std::endl;
    o_ssLaTeX << L"% 2" << std::endl;
    o_ssLaTeX << L"\\bibitem{CorrectionsSP80090B}" << std::endl;
    o_ssLaTeX << L"G. Sakurai, \\textit{Proposed list of corrections for NIST SP 800-90B 6.3 Estimators}, Dec. 2022" << std::endl;
    o_ssLaTeX << L"\\url{https://github.com/g-g-sakura/AnotherEntropyEstimationTool/blob/main/documentation/ProposedListOfCorrections_SP800-90B.pdf}" << std::endl;
    o_ssLaTeX << L"\\end{thebibliography}" << std::endl;
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
/// <params="i_refInfoReport">
/// </params>
/// <params="io_refDataOriginal">
/// </params>
/// <params="io_refDataBinary">
/// </params>
/// <returns>
/// </returns>
/// <precondition>
/// </precondition>
/// <postcondition>
/// </postcondition>
// -------------------------------------------------------------------------- //
ns_consts::EnmReturnStatus reportLaTeXSupportingInfo(std::wstringstream &o_refLaTeXSupportingInfo, 
    IDInfoForReport& i_refInfoReport,
    ns_dt::t_data_for_estimator& io_refDataOriginal)
{
    ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorUnexpected;

    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    o_refLaTeXSupportingInfo << L"\\section{Identification information}" << std::endl;
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    o_refLaTeXSupportingInfo << L"\\subsection{Identification of acquisition data from entropy source}" << std::endl;
    o_refLaTeXSupportingInfo << L"\\renewcommand{\\arraystretch}{1.8}" << std::endl;
    o_refLaTeXSupportingInfo << L"\\begin{table}[h]" << std::endl;
    o_refLaTeXSupportingInfo << L"\\caption{Identification information of acquisition data from entropy source}" << std::endl;
    o_refLaTeXSupportingInfo << L"\\begin{center}" << std::endl;
    o_refLaTeXSupportingInfo << L"\\begin{tabular}{|>{\\columncolor{rowcolorlightblue}}p{2cm}|p{20.5cm}|}" << std::endl;
    o_refLaTeXSupportingInfo << L"\\hline " << std::endl;
    o_refLaTeXSupportingInfo << L"Path to the acquisition data & \\verb|" << (*i_refInfoReport.info_source.p_path_to_entropy_input) << L"| \\\\" << std::endl;
    o_refLaTeXSupportingInfo << L"\\hline" << std::endl;

    struct tm newtime;

    errno_t err = localtime_s(&newtime, &(i_refInfoReport.info_source.tm_last_write_time));
    if (err)
    {
        return  sts = ns_consts::EnmReturnStatus::ErrorInvalidData;
    }

    o_refLaTeXSupportingInfo << L"Last write time & " << std::put_time(&newtime, L"%Y-%b-%d %H:%M:%S") << L" \\\\" << std::endl;
    o_refLaTeXSupportingInfo << L"\\hline" << std::endl;
    o_refLaTeXSupportingInfo << L"\\end{tabular}" << std::endl;
    o_refLaTeXSupportingInfo << L"\\end{center}" << std::endl;
    o_refLaTeXSupportingInfo << L"\\end{table}" << std::endl;
    o_refLaTeXSupportingInfo << L"\\renewcommand{\\arraystretch}{1.4}" << std::endl;
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    o_refLaTeXSupportingInfo << L"\\begin{itemize}" << std::endl;
    o_refLaTeXSupportingInfo << L"\t	\\item Brief explanation of the acquisition data (or entropy source) : \\\\" << std::endl;
    o_refLaTeXSupportingInfo << L"\t	    \\begin{Form}" << std::endl;
    o_refLaTeXSupportingInfo << L"\t	    \\noindent" << std::endl;
    o_refLaTeXSupportingInfo << L"\t	    \\TextField[name=multilinetextbox, multiline=true, bordercolor=bordercolordarkblue,width=\\linewidth,height=1in]{}" << std::endl;
    o_refLaTeXSupportingInfo << L"\t	    \\end{Form}" << std::endl;
    o_refLaTeXSupportingInfo << L"\t\\end{itemize} " << std::endl;
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    o_refLaTeXSupportingInfo << L"\\subsection{Identification of analysis environment}" << std::endl;

    o_refLaTeXSupportingInfo << L"\\renewcommand{\\arraystretch}{1.8}" << std::endl;
    o_refLaTeXSupportingInfo << L"\\begin{table}[h]" << std::endl;
    o_refLaTeXSupportingInfo << L"\\caption{Identification information of analysis environment}" << std::endl;
    o_refLaTeXSupportingInfo << L"\\begin{center}" << std::endl;
    o_refLaTeXSupportingInfo << L"\\begin{tabular}{|>{\\columncolor{rowcolorlightblue}}l|>{\\columncolor{rowcolorlightblue}}l|p{12cm}|}" << std::endl;
    o_refLaTeXSupportingInfo << L"\\hline " << std::endl;
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    o_refLaTeXSupportingInfo << L"Analysis tool & Name & " << (*i_refInfoReport.info_analysis_tool.p_analyzer_name) << L" \\\\" << std::endl;
    o_refLaTeXSupportingInfo << L"\\cline{2-3}" << std::endl;
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    o_refLaTeXSupportingInfo << L"\\, & Versioning information & " << (*i_refInfoReport.info_analysis_tool.p_analyzer_versioning) << L" \\\\" << std::endl;
    o_refLaTeXSupportingInfo << L"\\hline" << std::endl;
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    o_refLaTeXSupportingInfo << L"Analysis environment & Hostname & " << (*i_refInfoReport.info_env.p_hostname) << L" \\\\" << std::endl;
    o_refLaTeXSupportingInfo << L"\\cline{2-3}" << std::endl;
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    o_refLaTeXSupportingInfo << L"\\, & CPU information & " << (*i_refInfoReport.info_env.p_cpuinfo) << L" \\\\" << std::endl;
    o_refLaTeXSupportingInfo << L"\\cline{2-3}" << std::endl;
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    o_refLaTeXSupportingInfo << L"\\, &  Physical memory size & " << (*i_refInfoReport.info_env.p_physicalmemory) << L" \\\\" << std::endl;
    o_refLaTeXSupportingInfo << L"\\cline{2-3}" << std::endl;
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    o_refLaTeXSupportingInfo << L"\\, &  OS information & "<< (*i_refInfoReport.info_env.p_osinfo) << L" \\\\" << std::endl;
    o_refLaTeXSupportingInfo << L"\\cline{2-3}" << std::endl;
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    o_refLaTeXSupportingInfo << L"\\, &  Username & "<< (*i_refInfoReport.info_env.p_username) << L" \\\\" << std::endl;
    o_refLaTeXSupportingInfo << L"\\hline" << std::endl;
    o_refLaTeXSupportingInfo << L"\\end{tabular}" << std::endl;
    o_refLaTeXSupportingInfo << L"\\end{center}" << std::endl;
    o_refLaTeXSupportingInfo << L"\\end{table}" << std::endl;
    o_refLaTeXSupportingInfo << L"\\renewcommand{\\arraystretch}{1.4}" << std::endl;
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    o_refLaTeXSupportingInfo << L"\\subsection{Identification of analysis conditions}" << std::endl;
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    o_refLaTeXSupportingInfo << L"\\renewcommand{\\arraystretch}{1.8}" << std::endl;
    o_refLaTeXSupportingInfo << L"\\begin{table}[h]" << std::endl;
    o_refLaTeXSupportingInfo << L"\\caption{Identification information of analysis conditions}" << std::endl;
    o_refLaTeXSupportingInfo << L"\\begin{center}" << std::endl;
    o_refLaTeXSupportingInfo << L"\\begin{tabular}{|>{\\columncolor{rowcolorlightblue}}l|p{8cm}|}" << std::endl;
    o_refLaTeXSupportingInfo << L"\\hline " << std::endl;
    o_refLaTeXSupportingInfo << L"Number of samples & " << io_refDataOriginal.L << L" \\\\" << std::endl;
    o_refLaTeXSupportingInfo << L"\\hline" << std::endl;
    o_refLaTeXSupportingInfo << L"Bits per sample & " << io_refDataOriginal.bits_per_sample << L" \\\\" << std::endl;
    o_refLaTeXSupportingInfo << L"\\hline" << std::endl;
    if (1 < io_refDataOriginal.bits_per_sample)
    {
        o_refLaTeXSupportingInfo << L"Byte to bit conversion & " << std::endl;
        if (io_refDataOriginal.bIsMSbFirstByteBitConversion)
        {
            o_refLaTeXSupportingInfo << L"Most Significant bit (MSb) first" << std::endl;
        }
        else
        {
            o_refLaTeXSupportingInfo << L"Least Significant bit (LSb) first" << std::endl;
        }
        o_refLaTeXSupportingInfo << L" \\\\" << std::endl;
        o_refLaTeXSupportingInfo << L"\\hline" << std::endl;
    }
    o_refLaTeXSupportingInfo << L"\\end{tabular}" << std::endl;
    o_refLaTeXSupportingInfo << L"\\end{center}" << std::endl;
    o_refLaTeXSupportingInfo << L"\\end{table}" << std::endl;
    o_refLaTeXSupportingInfo << L"\\renewcommand{\\arraystretch}{1.4}" << std::endl;
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    o_refLaTeXSupportingInfo << L"\\subsection{Identification of analysis method}" << std::endl;
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    o_refLaTeXSupportingInfo << L"NIST SP 800-90B \\cite{SP80090B} 6.3 with corrections \\cite{CorrectionsSP80090B} is applied" << std::endl;
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
/// <params="i_refInfoReport">
/// </params>
/// <params="io_refDataOriginal">
/// </params>
/// <params="io_refDataBinary">
/// </params>
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
    double  min_entropy_literal = (double)io_refDataOriginal.bits_per_sample;
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
        ssLaTeXSummary << L"\\pgfplotstableread{" << std::endl;
        ssLaTeXSummary << L"section\ty\ty-min\ty-max" << std::endl;
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
                break;
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
                    ssLaTeXSummary << std::setw(8) << std::fabs(*p_min_entropy_upper_bound - *p_min_entropy) << std::endl;
                }
                else
                {
                    ssLaTeXSummary << std::setw(8) << 0.0 << std::endl;
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
        ssLaTeXSummary << L"Binary}" << std::endl;
    }
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    ssLaTeXSummary << L"\\begin{table}[h]" << std::endl;
    ssLaTeXSummary << L"\\caption{Numerical results}" << std::endl;
    ssLaTeXSummary << L"\\begin{center}" << std::endl;
    ssLaTeXSummary << L"\\begin{tabular}{|l|c|c|}" << std::endl;
    ssLaTeXSummary << L"\\hline " << std::endl;
    ssLaTeXSummary << L"Estimator										& $H_{\\textrm{original}}$$^{\\textrm{\\,a}}$			& $H_{\\textrm{bitstring}}$$^{\\textrm{\\,b}}$				\\\\ " << std::endl;
    ssLaTeXSummary << L"\\cline{2-3}" << std::endl;
    ssLaTeXSummary << L"\\,												& [bit / "<< io_refDataOriginal.bits_per_sample << L" - bit] & [bit / 1 - bit]		\\\\" << std::endl;
    ssLaTeXSummary << L"\\hline " << std::endl;
    ssLaTeXSummary << L"The Most Common Value Estimate					& " << io_refDataOriginal.t_6_3_1.t_common.min_entropy << L"& " << io_refDataBinary.t_6_3_1.t_common.min_entropy << L"\\\\" << std::endl;
    ssLaTeXSummary << L"\\hline " << std::endl;
    ssLaTeXSummary << L"The Collision Estimate							& ---				& " << io_refDataBinary.t_6_3_2.t_common.min_entropy << L"\\\\" << std::endl;
    ssLaTeXSummary << L"\\hline " << std::endl;
    ssLaTeXSummary << L"The Markov Estimate								& ---				& " << io_refDataBinary.t_6_3_3.t_common.min_entropy << L"\\\\" << std::endl;
    ssLaTeXSummary << L"\\hline " << std::endl;
    ssLaTeXSummary << L"The Compression Estimate						& ---				& " << io_refDataBinary.t_6_3_4.t_common.min_entropy << L"\\\\" << std::endl;
    ssLaTeXSummary << L"\\hline " << std::endl;
    ssLaTeXSummary << L"The t-Tuple Estimate							& " << io_refDataOriginal.t_6_3_5.t_common.min_entropy << L"& " << io_refDataBinary.t_6_3_5.t_common.min_entropy << L"\\\\" << std::endl;
    ssLaTeXSummary << L"\\hline " << std::endl;
    ssLaTeXSummary << L"The Longest Repeated Substring (LRS) Estimate	& " << io_refDataOriginal.t_6_3_6.t_common.min_entropy << L"& " << io_refDataBinary.t_6_3_6.t_common.min_entropy << L"\\\\" << std::endl;
    ssLaTeXSummary << L"\\hline " << std::endl;
    ssLaTeXSummary << L"Multi Most Common in Window Prediction Estimate	& " << io_refDataOriginal.t_6_3_7.t_common.min_entropy << L"& " << io_refDataBinary.t_6_3_7.t_common.min_entropy << L"\\\\" << std::endl;
    ssLaTeXSummary << L"\\hline " << std::endl;
    ssLaTeXSummary << L"The Lag Prediction Estimate						& " << io_refDataOriginal.t_6_3_8.t_common.min_entropy << L"& " << io_refDataBinary.t_6_3_8.t_common.min_entropy << L"\\\\" << std::endl;
    ssLaTeXSummary << L"\\hline " << std::endl;
    ssLaTeXSummary << L"The MultiMMC Prediction Estimate				& " << io_refDataOriginal.t_6_3_9.t_common.min_entropy << L"& " << io_refDataBinary.t_6_3_9.t_common.min_entropy << L"\\\\" << std::endl;
    ssLaTeXSummary << L"\\hline " << std::endl;
    ssLaTeXSummary << L"The LZ78Y Prediction Estimate					& " << io_refDataOriginal.t_6_3_10.t_common.min_entropy << L"& " << io_refDataBinary.t_6_3_10.t_common.min_entropy << L"\\\\" << std::endl;
    ssLaTeXSummary << L"\\hline \\hline " << std::endl;
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    {
        double  min_entropy_global = min_entropy_literal;
        if ((double)io_refDataOriginal.bits_per_sample * min_entropy_bitstring < min_entropy_literal)
        {
            min_entropy_global = (double)io_refDataOriginal.bits_per_sample * min_entropy_bitstring;
        }
        ssLaTeXSummary << L"The intial entropy source estimate [bit / " << io_refDataOriginal.bits_per_sample << L" - bit]	& \\multicolumn{2}{|c|}{" << min_entropy_global << L"}	\\\\" << std::endl;
    }
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    ssLaTeXSummary << L"$H_{I} = \\min (H_{\\textrm{original}}, " << io_refDataOriginal.bits_per_sample << L"\\times H_{\\textrm{bitstring}})$ &\\multicolumn{2}{ | c | } {\\, }	\\\\" << std::endl;
    ssLaTeXSummary << L"\\hline \\hline " << std::endl;
    ssLaTeXSummary << L"\\multicolumn{3}{|l|}{$^{\\,a}$\\quad Entropy estimate of the sequential dataset [source: NIST SP 800-90B \\cite{SP80090B} 3.1.3]} \\\\" << std::endl;
    ssLaTeXSummary << L"\\multicolumn{3}{|l|}{$^{\\,b}$\\quad An additional entropy estimation (per bit) for the non-binary sequential dataset [see NIST SP 800-90B 3.1.3]} \\\\" << std::endl;
    ssLaTeXSummary << L"\\hline " << std::endl;
    ssLaTeXSummary << L"\\end{tabular}" << std::endl;
    ssLaTeXSummary << L"\\end{center}" << std::endl;
    ssLaTeXSummary << L"\\end{table}" << std::endl;
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    ssLaTeXSummary << L"\\clearpage" << std::endl;
    ssLaTeXSummary << L"\\subsection{Visual comparison of min-entropy estimates from original samples}" << std::endl;
    loadPGFPlotSummary(ssLaTeXSummary, false, min_entropy_literal, io_refDataOriginal.bits_per_sample);
    ssLaTeXSummary << L"\\clearpage" << std::endl;
    ssLaTeXSummary << L"\\subsection{Visual comparison of min-entropy estimates by interpreting each sample as bitstring}" << std::endl;
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
    ssLaTeX << L"\\begin{document}" << std::endl;
    ssLaTeX << L"\\maketitle" << std::endl;
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    ssLaTeX << ssLaTeXSupportingInfo.rdbuf();
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    ssLaTeX << L"\\section{Executive summary}" << std::endl;
    ssLaTeX << L"\\subsection{Numerical results of min-entropy estimates based on non-IID track}" << std::endl;
    ssLaTeX << ssLaTeXSummary.rdbuf();
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    ssLaTeX << L"\\clearpage" << std::endl;
    ssLaTeX << L"\\section{Detailed results of analysis from original samples}" << std::endl;
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    ssLaTeX << io_refDataOriginal.p_ssLaTeXFragment->rdbuf();
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    ssLaTeX << L"\\section{Detailed results of analysis by interpreting each sample as bitstrings}" << std::endl;
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
    ssLaTeX << L"\\end{document}" << std::endl;
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
    // 
    // -------------------------------------------------------------------------- //
    return  sts = ns_consts::EnmReturnStatus::Success;
}

// -------------------------------------------------------------------------- //
/// <summary>
/// </summary>
/// <remarks>
/// </remarks>
/// <params="i_refInfoReport">
/// </params>
/// <params="io_refDataBinary">
/// </params>
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
    ssLaTeXSummary << L"\\pgfplotstableread{" << std::endl;
    ssLaTeXSummary << L"section\ty\ty-min\ty-max" << std::endl;
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
            break;
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
                ssLaTeXSummary << std::setw(8) << *p_min_entropy_upper_bound << std::endl;
            }
            else
            {
                ssLaTeXSummary << std::setw(8) << 0.0 << std::endl;
            }
        }
    }
    ssLaTeXSummary << L"}{\\summarytableBinary}" << std::endl;
    ssLaTeXSummary << L"\\begin{table}[h]" << std::endl;
    ssLaTeXSummary << L"\\caption{Numerical results}" << std::endl;
    ssLaTeXSummary << L"\\begin{center}" << std::endl;
    ssLaTeXSummary << L"\\begin{tabular}{|l|c|}" << std::endl;
    ssLaTeXSummary << L"\\hline " << std::endl;
    ssLaTeXSummary << L"Estimator										& $H_{\\textrm{bitstring}}$$^{\\textrm{\\,a}}$				\\\\ " << std::endl;
    ssLaTeXSummary << L"\\cline{1-2}" << std::endl;
    ssLaTeXSummary << L"\\,												& (/ 1 - bit)		\\\\" << std::endl;
    ssLaTeXSummary << L"\\hline " << std::endl;
    ssLaTeXSummary << L"The Most Common Value Estimate					& " << io_refDataBinary.t_6_3_1.t_common.min_entropy << L"\\\\" << std::endl;
    ssLaTeXSummary << L"\\hline " << std::endl;
    ssLaTeXSummary << L"The Collision Estimate							& " << io_refDataBinary.t_6_3_2.t_common.min_entropy << L"\\\\" << std::endl;
    ssLaTeXSummary << L"\\hline " << std::endl;
    ssLaTeXSummary << L"The Markov Estimate								& " << io_refDataBinary.t_6_3_3.t_common.min_entropy << L"\\\\" << std::endl;
    ssLaTeXSummary << L"\\hline " << std::endl;
    ssLaTeXSummary << L"The Compression Estimate						& " << io_refDataBinary.t_6_3_4.t_common.min_entropy << L"\\\\" << std::endl;
    ssLaTeXSummary << L"\\hline " << std::endl;
    ssLaTeXSummary << L"The t-Tuple Estimate							& " << io_refDataBinary.t_6_3_5.t_common.min_entropy << L"\\\\" << std::endl;
    ssLaTeXSummary << L"\\hline " << std::endl;
    ssLaTeXSummary << L"The Longest Repeated Substring (LRS) Estimate	& " << io_refDataBinary.t_6_3_6.t_common.min_entropy << L"\\\\" << std::endl;
    ssLaTeXSummary << L"\\hline " << std::endl;
    ssLaTeXSummary << L"Multi Most Common in Window Prediction Estimate	& " << io_refDataBinary.t_6_3_7.t_common.min_entropy << L"\\\\" << std::endl;
    ssLaTeXSummary << L"\\hline " << std::endl;
    ssLaTeXSummary << L"The Lag Prediction Estimate						& " << io_refDataBinary.t_6_3_8.t_common.min_entropy << L"\\\\" << std::endl;
    ssLaTeXSummary << L"\\hline " << std::endl;
    ssLaTeXSummary << L"The MultiMMC Prediction Estimate				& " << io_refDataBinary.t_6_3_9.t_common.min_entropy << L"\\\\" << std::endl;
    ssLaTeXSummary << L"\\hline " << std::endl;
    ssLaTeXSummary << L"The LZ78Y Prediction Estimate					& " << io_refDataBinary.t_6_3_10.t_common.min_entropy << L"\\\\" << std::endl;
    ssLaTeXSummary << L"\\hline \\hline " << std::endl;
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    ssLaTeXSummary << L"The intial entropy source estimate (/ " << io_refDataBinary.bits_per_sample << L"-bit)	& " << min_entropy_bitstring << L"	\\\\" << std::endl;
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    ssLaTeXSummary << L"$H_{I} = H_{\\textrm{bitstring}}$ & \\, 	\\\\" << std::endl;
    ssLaTeXSummary << L"\\hline \\hline " << std::endl;
    ssLaTeXSummary << L"\\multicolumn{2}{|l|}{$^{\\,a}$\\quad Entropy estimate of the sequential dataset [source: NIST SP 800-90B \\cite{SP80090B} 3.1.3]} \\\\" << std::endl;
    ssLaTeXSummary << L"\\hline " << std::endl;
    ssLaTeXSummary << L"\\end{tabular}" << std::endl;
    ssLaTeXSummary << L"\\end{center}" << std::endl;
    ssLaTeXSummary << L"\\end{table}" << std::endl;
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    ssLaTeXSummary << L"\\clearpage" << std::endl;
    ssLaTeXSummary << L"\\subsection{Visual comparison of min-entropy estimates from binary samples}" << std::endl;
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
    ssLaTeX << L"\\begin{document}" << std::endl;
    ssLaTeX << L"\\maketitle" << std::endl;
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    ssLaTeX << ssLaTeXSupportingInfo.rdbuf();
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    ssLaTeX << L"\\section{Executive summary}" << std::endl;
    ssLaTeX << L"\\subsection{Numerical results of min-entropy estimates based on non-IID track}" << std::endl;
    ssLaTeX << ssLaTeXSummary.rdbuf();
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    ssLaTeX << L"\\clearpage" << std::endl;
    ssLaTeX << L"\\section{Detailed results of analysis from original samples}" << std::endl;
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
    ssLaTeX << L"\\end{document}" << std::endl;
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
    // 
    // -------------------------------------------------------------------------- //
    return  sts = ns_consts::EnmReturnStatus::Success;
}
