////////////////////////////////////////////////////////////////////////////////
// conversion.cpp
//
//
//
// Copyright (c) 2021-2022 G. G. SAKURAI <g.garland823@gmail.com>
//
////////////////////////////////////////////////////////////////////////////////
#include "../pch.h"
#include "conversion.h"

namespace entropy_estimator_lib
{
	namespace support
	{
		namespace ns_consts = entropy_estimator_lib::constants;
		namespace ns_dt = entropy_estimator_lib::data_types;

		// -------------------------------------------------------------------------- //
		/// <summary>
		/// </summary>
		/// <remarks>
		/// </remarks>
		/// <params="o_refInt">
		///  Specifies the reference to the converted boost::multiplrecision::cpp_int type integer.
		/// </params>
		/// <params="i_refSeqSamples">
		///  Specifies the reference to the sequence of samples to be be converted to boost::multiplrecision::cpp_int type integer, using base (=256).
		/// </params>
		/// <params="i_bit_width">
		///  Specifies the number of bits per sample.
		/// </params>
		/// <returns>
		/// </returns>
		/// <precondition>
		/// </precondition>
		/// <postcondition>
		/// </postcondition>
		// -------------------------------------------------------------------------- //
		ns_consts::EnmReturnStatus convertSeqSamplesToCppInt(bmp::cpp_int& o_refInt, blitz::Array<ns_dt::octet, 1>& i_refSeqSamples, const int i_bit_width)
		{
			ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorInvalidData;

			if ((i_bit_width <= 0) || (8 < i_bit_width))
			{
				return sts;
			}
			int shift_width = 8;
			
			if ((1 <= i_bit_width) && (i_bit_width <= 7))
			{
				shift_width = i_bit_width;
			}
			o_refInt = 0;
			for (int i = 0; i < i_refSeqSamples.length(blitz::firstDim); ++i)
			{
				o_refInt <<= shift_width;
				o_refInt += i_refSeqSamples(i);
			}
			return sts = ns_consts::EnmReturnStatus::Success;
		}

		// -------------------------------------------------------------------------- //
		/// <summary>
		/// </summary>
		/// <remarks>
		/// </remarks>
		/// <params="o_refBitSet">
		///  Specifies the reference to the bitset to store the converted bitset from <c>i_refSeqSamples</c>.
		/// </params>
		/// <params="i_refSeqSamples">
		///  Specifies the reference to the sequence of samples to be be converted to boost::dynamic_bitset<> type.
		/// </params>
		/// <params="i_bit_width">
		///  Specifies the number of bits per sample.
		/// </params>
		/// <returns>
		///  <c>entropy_estimator_lib::constants::EnmReturnStatus::ErrorInvalidData</c>: when one of the following conditions is met:
		///    <ul>
		///     <li><c>io_refData.bits_per_sample</c> &lt; 1</li>
		///     <li><c>io_refData.bits_per_sample</c> &gt; 8</li>
		///    </ul>
		///  <c>entropy_estimator_lib::constants::EnmReturnStatus::Success</c>:  otherwise.
		/// </returns>
		/// <precondition>
		/// </precondition>
		/// <postcondition>
		/// </postcondition>
		// -------------------------------------------------------------------------- //
		ns_consts::EnmReturnStatus convertSeqSamplesToBitSet(boost::dynamic_bitset<>& o_refBitSet, blitz::Array<ns_dt::octet, 1>& i_refSeqSamples, const int i_bit_width)
		{
			ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorInvalidData;

			if ((i_bit_width < 1) || (8 < i_bit_width))
			{
				return sts;
			}

			size_t bs_length = (size_t)i_refSeqSamples.length(blitz::firstDim) * i_bit_width;
			o_refBitSet.resize(bs_length);
			o_refBitSet.reset();
			for (int i = 0; i < i_refSeqSamples.length(blitz::firstDim); ++i)
			{
				for (int j = 0; j < i_bit_width; ++j)
				{
					int shift_width = ((int)i_bit_width - 1) - (int)j;
					size_t	index = (size_t)i * i_bit_width + j;
					o_refBitSet[ index ] = (i_refSeqSamples(i) >> shift_width) & 0x01;
				}
			}
			return sts = ns_consts::EnmReturnStatus::Success;
		}
	}
}