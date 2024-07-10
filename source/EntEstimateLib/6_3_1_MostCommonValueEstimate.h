////////////////////////////////////////////////////////////////////////////////
// 6_3_1_MostCommonValueEstimate.h
//
//
// Copyright (c) 2021-2023 G. G. SAKURAI <g.garland823@gmail.com>
//
////////////////////////////////////////////////////////////////////////////////
#if defined(_MSC_VER)
#pragma once
#endif

#ifndef ENTROPY_ESTIMATOR_LIB_6_3_1_MOST_COMMON_VALUE_ESTIMATE_H
#define ENTROPY_ESTIMATOR_LIB_6_3_1_MOST_COMMON_VALUE_ESTIMATE_H

#include "EntropyEstimateData.h"
#include "constants.h"

namespace entropy_estimator_lib
{
	namespace estimators
	{
		namespace mcv
		{
			namespace ns_consts = entropy_estimator_lib::constants;
			namespace ns_dt = entropy_estimator_lib::data_types;
			
			ns_consts::EnmReturnStatus estimate(ns_dt::t_data_for_estimator& refData);
			
		}
	}
}

#endif