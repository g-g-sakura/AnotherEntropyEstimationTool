////////////////////////////////////////////////////////////////////////////////
// 6_3_9_MultiMMCPredictionEstimate.h
//
//
// Copyright (c) 2021-2022 G. G. SAKURAI <g.garland823@gmail.com>
//
////////////////////////////////////////////////////////////////////////////////
#if defined(_MSC_VER)
#pragma once
#endif

#ifndef ENTROPY_ESTIMATOR_LIB_6_3_9_MULTI_MMC_PREDICTION_ESTIMATE_H
#define ENTROPY_ESTIMATOR_LIB_6_3_9_MULTI_MMC_PREDICTION_ESTIMATE_H

#include "EntropyEstimateData.h"
#include "constants.h"

namespace entropy_estimator_lib
{
	namespace estimators
	{
		namespace multimmc_prediction
		{
			namespace ns_consts = entropy_estimator_lib::constants;
			namespace ns_dt = entropy_estimator_lib::data_types;

			ns_consts::EnmReturnStatus estimate(ns_dt::t_data_for_estimator& refData);

		}
	}
}
#endif