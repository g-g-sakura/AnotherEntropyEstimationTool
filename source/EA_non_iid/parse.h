////////////////////////////////////////////////////////////////////////////////
// parse.h
//
//
// Copyright (c) 2021-2023 G. G. SAKURAI <g.garland823@gmail.com>
//
////////////////////////////////////////////////////////////////////////////////
#if defined(_MSC_VER)
#pragma once
#endif

#ifndef __EA_NON_IID_PARSE_H__
#define __EA_NON_IID_PARSE_H__

#include "reporter.h"

namespace ns_consts = entropy_estimator_lib::constants;
namespace ns_dt = entropy_estimator_lib::data_types;

ns_consts::EnmReturnStatus parse(ns_dt::t_data_for_estimator& io_refData,
	ns_dt::t_data_for_estimator& io_refDataBitString,
	IDInfoForReport& i_refInfoReport,
	int ac, wchar_t* av[], wchar_t* envp[]);

#endif