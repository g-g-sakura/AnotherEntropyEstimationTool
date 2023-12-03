////////////////////////////////////////////////////////////////////////////////
// showTestSummary.h
//
//
// Copyright (c) 2021-2023 G. G. SAKURAI <g.garland823@gmail.com>
//
////////////////////////////////////////////////////////////////////////////////
#if defined(_MSC_VER)
#pragma once
#endif

#ifndef __ENTROPY_ESTIMATOR_LIB_SHOW_TEST_SUMMARY_H__
#define __ENTROPY_ESTIMATOR_LIB_SHOW_TEST_SUMMARY_H__

#include "../EntropyEstimateData.h"
#include "../constants.h"
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

namespace entropy_estimator_lib
{
	namespace support
	{
		namespace ns_consts = entropy_estimator_lib::constants;
		namespace ns_dt = entropy_estimator_lib::data_types;

		ns_consts::EnmReturnStatus showTestSpecificHeader(std::stringstream & o_refStream, 
			const ns_consts::EnmNonIIDTrack & i_enmEstimator, 
			const ns_consts::EnmSampleInterpretation& i_enmInterpretation,
			ns_dt::t_data_for_estimator& io_refData);

		ns_consts::EnmReturnStatus showTestSpecificFooter(std::stringstream& o_refStream, 
			const ns_consts::EnmNonIIDTrack& i_enmEstimator,
			ns_consts::EnmSampleInterpretation& i_enmInterpretation,
			ns_dt::t_data_for_estimator& io_refData);

		ns_consts::EnmReturnStatus showTestSpecificResult(std::stringstream& o_refStream,
			ns_consts::EnmNonIIDTrack& i_enmEstimator,
			ns_consts::EnmSampleInterpretation& i_enmInterpretation,
			ns_dt::t_data_for_estimator& io_refData);
	}
}
#endif