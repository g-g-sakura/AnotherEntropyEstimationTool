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

#ifndef ENTROPY_ESTIMATOR_LIB_CONVERSION_H
#define ENTROPY_ESTIMATOR_LIB_CONVERSION_H

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
		/// <param name="o_refInt">
		/// </param>
		/// <param name="i_refSeqSamples">
		/// </param>
		/// <param name="i_bit_width">
		/// </param>
		/// <returns>
		/// </returns>
		/// <precondition>
		/// </precondition>
		/// <postcondition>
		/// </postcondition>
		// -------------------------------------------------------------------------- //
		ns_consts::EnmReturnStatus convertSeqSamplesToCppInt(bmp::cpp_int& o_refInt, blitz::Array<ns_dt::octet, 1>& i_refSeqSamples, unsigned int i_bit_width = 8);

		// -------------------------------------------------------------------------- //
		/// <summary>
		/// </summary>
		/// <remarks>
		/// </remarks>
		/// <param name="o_refBitSet">
		/// </param>
		/// <param name="i_refDTuple">
		/// </param>
		/// <param name="i_bit_width">
		/// </param>
		/// <returns>
		/// </returns>
		/// <precondition>
		/// </precondition>
		/// <postcondition>
		/// </postcondition>
		// -------------------------------------------------------------------------- //
		ns_consts::EnmReturnStatus convertSeqSamplesToBitSet(boost::dynamic_bitset<>& o_refBitSet, blitz::Array<ns_dt::octet, 1>& i_refSeqSamples, unsigned int i_bit_width = 8);

	}
}
#endif