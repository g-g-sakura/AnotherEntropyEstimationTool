////////////////////////////////////////////////////////////////////////////////
// hostinfo.h
//
//
// Copyright (c) 2021-2023 G. G. SAKURAI <g.garland823@gmail.com>
//
////////////////////////////////////////////////////////////////////////////////
#if defined(_MSC_VER)
#pragma once
#endif

#ifndef EA_NON_IID_HOST_INFO_H
#define EA_NON_IID_HOST_INFO_H

#include "EntEstimateLib/constants.h"
#include "reportItems.h"

namespace ns_consts = entropy_estimator_lib::constants;

ns_consts::EnmReturnStatus getEnvInfo(const InfoEnv&);

#endif