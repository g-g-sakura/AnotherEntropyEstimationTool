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

#ifndef __EA_NON_IID_REPORT_ITEMS_H__
#define __EA_NON_IID_REPORT_ITEMS_H__

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
	std::wstring* p_osinfo;
	std::wstring* p_username;
};

struct IDInfoForReport
{
	InfoEntropyInput	info_source;
	InfoAnalysisTool	info_analysis_tool;
	InfoEnv				info_env;
};

#endif