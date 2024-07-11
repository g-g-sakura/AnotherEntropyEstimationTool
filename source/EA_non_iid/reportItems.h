////////////////////////////////////////////////////////////////////////////////
// reportItems.h
//
//
// Copyright (c) 2021-2022 G. G. SAKURAI <g.garland823@gmail.com>
//
////////////////////////////////////////////////////////////////////////////////
#if defined(_MSC_VER)
#pragma once
#endif

#ifndef EA_NON_IID_REPORT_ITEMS_H
#define EA_NON_IID_REPORT_ITEMS_H

#include <boost/filesystem.hpp>

namespace bs_fs = boost::filesystem;

struct InfoEntropyInput
{
	bs_fs::path* p_path_to_entropy_input;
	std::time_t		tm_last_write_time;
};

struct InfoAnalysisTool
{
	std::wstring* p_analyzer_name;
	std::wstring* p_analyzer_versioning;
};

struct InfoEnv
{
	std::wstring* p_hostname;
	std::wstring* p_cpuinfo;
	std::wstring* p_physicalmemory;
	std::wstring* p_osname;
	std::wstring* p_osversion;
	std::wstring* p_system_type;
	std::wstring* p_username;
};

struct IDInfoForReport
{
	InfoEntropyInput	info_source;
	InfoAnalysisTool	info_analysis_tool;
	InfoEnv				info_env;
};

#endif