////////////////////////////////////////////////////////////////////////////////
// conversion.h
//
//
// Copyright (c) 2021-2023 G. G. SAKURAI <g.garland823@gmail.com>
//
////////////////////////////////////////////////////////////////////////////////
#if defined(_MSC_VER)
#pragma once
#endif

#ifndef __ENTROPY_ESTIMATOR_LIB_CONVERSION_H__
#define __ENTROPY_ESTIMATOR_LIB_CONVERSION_H__

#include "../EntropyEstimateData.h"
#include "../constants.h"
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/dynamic_bitset.hpp>

namespace entropy_estimator_lib
{
	namespace support
	{
		namespace ns_consts = entropy_estimator_lib::constants;
		namespace ns_dt = entropy_estimator_lib::data_types;
		namespace bmp = boost::multiprecision;

		// -------------------------------------------------------------------------- //
		/// <summary>
		/// </summary>
		/// <remarks>
		/// </remarks>
		/// <params="o_refInt">
		/// </params>
		/// <params="i_refSeqSamples">
		/// </params>
		/// <params="i_bit_width">
		/// </params>
		/// <returns>
		/// </returns>
		/// <precondition>
		/// </precondition>
		/// <postcondition>
		/// </postcondition>
		// -------------------------------------------------------------------------- //
		ns_consts::EnmReturnStatus convertSeqSamplesToCppInt(bmp::cpp_int& o_refInt, blitz::Array<ns_dt::octet, 1>& i_refSeqSamples, const unsigned int i_bit_width = 8);

		// -------------------------------------------------------------------------- //
		/// <summary>
		/// </summary>
		/// <remarks>
		/// </remarks>
		/// <params="o_refBitSet">
		/// </params>
		/// <params="i_refDTuple">
		/// </params>
		/// <params="i_bit_width">
		/// </params>
		/// <returns>
		/// </returns>
		/// <precondition>
		/// </precondition>
		/// <postcondition>
		/// </postcondition>
		// -------------------------------------------------------------------------- //
		ns_consts::EnmReturnStatus convertSeqSamplesToBitSet(boost::dynamic_bitset<>& o_refBitSet, blitz::Array<ns_dt::octet, 1>& i_refSeqSamples, const unsigned int i_bit_width = 8);

	}
}
#endif