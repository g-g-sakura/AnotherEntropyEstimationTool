////////////////////////////////////////////////////////////////////////////////
// setUp.h
//
//
// Copyright (c) 2021-2023 G. G. SAKURAI <g.garland823@gmail.com>
//
////////////////////////////////////////////////////////////////////////////////
#if defined(_MSC_VER)
#pragma once
#endif

#ifndef __ENTROPY_ESTIMATOR_LIB_SET_UP_H__
#define __ENTROPY_ESTIMATOR_LIB_SET_UP_H__

#include "../EntropyEstimateData.h"
#include "../constants.h"
#include <boost/filesystem.hpp>

namespace entropy_estimator_lib
{
	namespace support
	{
		namespace ns_consts = entropy_estimator_lib::constants;
		namespace ns_dt = entropy_estimator_lib::data_types;
		namespace bs_fs = boost::filesystem;

		ns_consts::EnmReturnStatus setUp(ns_dt::t_data_for_estimator& io_refData);

		ns_consts::EnmReturnStatus loadSamples(ns_dt::t_data_for_estimator& io_refData,
			const bs_fs::path& i_refFullPath);

		ns_consts::EnmReturnStatus loadSamplesByInterpretation(ns_dt::t_data_for_estimator& io_refData,
			const bs_fs::path& i_refFullPath,
			uintmax_t i_number_of_bits_to_be_loaded,
			unsigned int i_bits_per_sample);
	}
}
#endif