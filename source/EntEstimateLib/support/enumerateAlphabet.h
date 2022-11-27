////////////////////////////////////////////////////////////////////////////////
// enumerateAlphabet.h
//
//
// Copyright (c) 2021-2022 G. G. SAKURAI <g.garland823@gmail.com>
//
////////////////////////////////////////////////////////////////////////////////
#if defined(_MSC_VER)
#pragma once
#endif

#ifndef __ENTROPY_ESTIMATOR_LIB_ENUMERATE_ALPHABET_H__
#define __ENTROPY_ESTIMATOR_LIB_ENUMERATE_ALPHABET_H__

#include "../EntropyEstimateData.h"
#include "../constants.h"

namespace entropy_estimator_lib
{
	namespace support
	{
		namespace ns_consts = entropy_estimator_lib::constants;
		namespace ns_dt = entropy_estimator_lib::data_types;

		ns_consts::EnmReturnStatus enumerateAlphabet(ns_dt::t_data_for_estimator& io_refData);
	}
}
#endif