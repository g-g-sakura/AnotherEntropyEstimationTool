////////////////////////////////////////////////////////////////////////////////
// 6_3_5_t_TupleEstimateAlternative.h
//
//
// Copyright (c) 2025 G. G. SAKURAI <g.garland823@gmail.com>
//
////////////////////////////////////////////////////////////////////////////////
#if defined(_MSC_VER)
#pragma once
#endif

#ifndef ENTROPY_ESTIMATOR_LIB_6_3_5_T_TUPLE_ESTIMATE_ALTERNATIVE_H
#define ENTROPY_ESTIMATOR_LIB_6_3_5_T_TUPLE_ESTIMATE_ALTERNATIVE_H

#include "EntropyEstimateData.h"
#include "constants.h"

namespace entropy_estimator_lib
{
	namespace estimators
	{
		namespace t_tuple_alternative
		{
			namespace ns_consts = entropy_estimator_lib::constants;
			namespace ns_dt = entropy_estimator_lib::data_types;

			ns_consts::EnmReturnStatus estimate(ns_dt::t_data_for_estimator& refData);

		}
	}
}
#endif