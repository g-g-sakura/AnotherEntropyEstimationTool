////////////////////////////////////////////////////////////////////////////////
//
//
//
// Copyright (c) 2021-2022 G. G. SAKURAI <g.garland823@gmail.com>
//
////////////////////////////////////////////////////////////////////////////////
#if defined(_MSC_VER)
#pragma once
#endif

#ifndef __ENTROPY_ESTIMATOR_LIB_6_3_3_MARKOV_ESTIMATE_H__
#define __ENTROPY_ESTIMATOR_LIB_6_3_3_MARKOV_ESTIMATE_H__

#include "EntropyEstimateData.h"
#include "constants.h"

namespace entropy_estimator_lib
{
	namespace estimators
	{
		namespace markov
		{
			namespace ns_consts = entropy_estimator_lib::constants;
			namespace ns_dt = entropy_estimator_lib::data_types;

			ns_consts::EnmReturnStatus estimate(ns_dt::t_data_for_estimator& refData);

		}
	}
}
#endif