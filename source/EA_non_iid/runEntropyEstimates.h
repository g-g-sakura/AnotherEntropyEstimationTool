////////////////////////////////////////////////////////////////////////////////
// parse.h
//
//
// Copyright (c) 2021-2022 G. G. SAKURAI <g.garland823@gmail.com>
//
////////////////////////////////////////////////////////////////////////////////
#if defined(_MSC_VER)
#pragma once
#endif


#ifndef __EA_NON_IID_RUN_ENTROPY_ESTIMATES_H__
#define __EA_NON_IID_RUN_ENTROPY_ESTIMATES_H__

#include "EntEstimateLib/constants.h"
#include "EntEstimateLib/EntropyEstimateData.h"

namespace ns_consts = entropy_estimator_lib::constants;
namespace ns_dt = entropy_estimator_lib::data_types;

ns_consts::EnmReturnStatus runEntropyEstimatesBinary(ns_dt::t_data_for_estimator& io_refData);

ns_consts::EnmReturnStatus runEntropyEstimatesNonBinary(ns_dt::t_data_for_estimator& io_refDataOriginal,
	ns_dt::t_data_for_estimator& io_refDataBinary);

#endif