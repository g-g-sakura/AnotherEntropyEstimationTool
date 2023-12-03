////////////////////////////////////////////////////////////////////////////////
// toolinfo.h
//
//
// Copyright (c) 2021-2023 G. G. SAKURAI <g.garland823@gmail.com>
//
////////////////////////////////////////////////////////////////////////////////
#if defined(_MSC_VER)
#pragma once
#endif

#ifndef __EA_NON_IID_TOOL_INFO_H__
#define __EA_NON_IID_TOOL_INFO_H__

#include "EntEstimateLib/constants.h"
#include "reportItems.h"

namespace ns_consts = entropy_estimator_lib::constants;

ns_consts::EnmReturnStatus getToolInfo(const InfoAnalysisTool& i_refAnalysisTool);

#endif