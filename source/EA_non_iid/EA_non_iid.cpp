////////////////////////////////////////////////////////////////////////////////
// EA_non_iid.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//
//
//
// Copyright (c) 2021-2023 G. G. SAKURAI <g.garland823@gmail.com>
//
////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>
#include <boost/program_options.hpp>
#include "parse.h"
#include "runEntropyEstimates.h"
#include "hostinfo.h"
#include "toolinfo.h"

namespace bs_fs = boost::filesystem;

namespace ns_consts = entropy_estimator_lib::constants;
namespace ns_dt = entropy_estimator_lib::data_types;

typedef ns_consts::EnmReturnStatus(*PF_EE)(ns_dt::t_data_for_estimator&);

int wmain(int ac, wchar_t* av[], wchar_t* envp[])
{
    ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorUnexpected;
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    ns_dt::t_data_for_estimator     data;
    ns_dt::t_data_for_estimator     data_for_bitstring;
    memset(&data, 0, sizeof(data));
    memset(&data_for_bitstring, 0, sizeof(data_for_bitstring));
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    IDInfoForReport the_info_report;
    memset(&the_info_report, 0, sizeof(the_info_report));
    // -------------------------------------------------------------------------- //
    // obtain tool information, for traceability
    // -------------------------------------------------------------------------- //
    std::wstring strToolName;
    std::wstring strToolVersioning;
    the_info_report.info_analysis_tool.p_analyzer_name = &strToolName;
    the_info_report.info_analysis_tool.p_analyzer_versioning = &strToolVersioning;
    sts = getToolInfo(the_info_report.info_analysis_tool);
    if (ns_consts::EnmReturnStatus::Success == sts)
    {
        std::wcout << L"# \t\t" << strToolName << L"\n";
        std::wcout << L"# \t\tVersion info:\t" << strToolVersioning << L"\n";
    }
    // -------------------------------------------------------------------------- //
    // show host name and username, for traceability
    // -------------------------------------------------------------------------- //
    std::wstring strHostName;
    std::wstring strCPUInfo;
    std::wstring strPhysicalMemory;
    std::wstring strOSInfo;
    std::wstring strUserName;
    the_info_report.info_env.p_hostname = &strHostName;
    the_info_report.info_env.p_cpuinfo = &strCPUInfo;
    the_info_report.info_env.p_physicalmemory = &strPhysicalMemory;
    the_info_report.info_env.p_osinfo = &strOSInfo;
    the_info_report.info_env.p_username = &strUserName;
    sts = getEnvInfo(the_info_report.info_env);
    if (ns_consts::EnmReturnStatus::Success == sts)
    {
        std::wcout << std::wstring(L"# Host name:\t") << strHostName << L"\n";
        std::wcout << std::wstring(L"# CPU id:\t") << strCPUInfo << L"\n";
        std::wcout << std::wstring(L"# Amount of Physical Memory:\t") << strPhysicalMemory << L"\n";
        std::wcout << std::wstring(L"# Operating System:\t") << strOSInfo << L"\n";
        std::wcout << std::wstring(L"# User name:\t") << strUserName << L"\n";
    }
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    bs_fs::path     the_report_path;
    bs_fs::path     the_path_entropy_input;
    the_info_report.info_source.p_path_to_entropy_input = &the_path_entropy_input;
    // -------------------------------------------------------------------------- //
    // parse command line parameters
    // -------------------------------------------------------------------------- //
    sts = parse(data, data_for_bitstring, 
        the_info_report, ac, av, envp);
    if (ns_consts::EnmReturnStatus::Success != sts)
    {
        return  static_cast<int>(sts);
    }
    // -------------------------------------------------------------------------- //
    // show some samples from the head of file, for confirmation
    // -------------------------------------------------------------------------- //
    showHeadSamples(data);
    // -------------------------------------------------------------------------- //
    // show some samples from the tail of file, for confirmation
    // -------------------------------------------------------------------------- //
    showTailSamples(data);
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    std::wstringstream ssLaTeXFragmentOriginal = std::wstringstream();
    data.p_ssLaTeXFragment = &ssLaTeXFragmentOriginal;
    std::wstringstream ssLaTeXFragmentBinary = std::wstringstream();
    data_for_bitstring.p_ssLaTeXFragment = &ssLaTeXFragmentBinary;
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    if (data.p_bzSampleSpaceA->length(blitz::firstDim) == 2)
    {
        // -------------------------------------------------------------------------- //
        // 
        // -------------------------------------------------------------------------- //
        sts = runEntropyEstimatesBinary(data);
        if (ns_consts::EnmReturnStatus::Success != sts)
        {
            return  static_cast<int>(sts);
        }
        // -------------------------------------------------------------------------- //
        // 
        // -------------------------------------------------------------------------- //
        if (data.isGeneratingReportInLaTeXformatRequested)
        {
            // -------------------------------------------------------------------------- //
            // 
            // -------------------------------------------------------------------------- //
            sts = reportXMLBinary(the_info_report, data);
            if (ns_consts::EnmReturnStatus::Success != sts)
            {
                return  static_cast<int>(sts);
            }
            // -------------------------------------------------------------------------- //
            // 
            // -------------------------------------------------------------------------- //
            sts = reportLaTeXBinary(the_info_report, data);
            if (ns_consts::EnmReturnStatus::Success != sts)
            {
                return  static_cast<int>(sts);
            }
        }
    }
    else
    {
        // -------------------------------------------------------------------------- //
        // 
        // -------------------------------------------------------------------------- //
        sts = runEntropyEstimatesNonBinary(data, data_for_bitstring);
        if (ns_consts::EnmReturnStatus::Success != sts)
        {
            return  static_cast<int>(sts);
        }
        // -------------------------------------------------------------------------- //
        // 
        // -------------------------------------------------------------------------- //
        if (data.isGeneratingReportInLaTeXformatRequested)
        {
            // -------------------------------------------------------------------------- //
            // 
            // -------------------------------------------------------------------------- //
            sts = reportXMLNonBinary(the_info_report, data, data_for_bitstring);
            if (ns_consts::EnmReturnStatus::Success != sts)
            {
                return  static_cast<int>(sts);
            }
            // -------------------------------------------------------------------------- //
            // 
            // -------------------------------------------------------------------------- //
            sts = reportLaTeXNonBinary(the_info_report, data, data_for_bitstring);
            if (ns_consts::EnmReturnStatus::Success != sts)
            {
                return  static_cast<int>(sts);
            }
        }
    }
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
}