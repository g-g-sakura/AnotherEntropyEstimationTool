////////////////////////////////////////////////////////////////////////////////
// checkArgs.cpp
//
//
//
// Copyright (c) 2021-2023 G. G. SAKURAI <g.garland823@gmail.com>
//
////////////////////////////////////////////////////////////////////////////////
#include "../pch.h"
#include "checkArgs.h"

namespace entropy_estimator_lib
{
	namespace support
	{
		// -------------------------------------------------------------------------- //
		/// <summary>
		/// </summary>
		/// <remarks>
		/// </remarks>
		/// <param name="i_refData">
		///  Specifies the reference to data structure for entropy estimation.
		/// </param>
		/// <returns>
		///  <c>entropy_estimator_lib::constants::EnmReturnStatus::ErrorNullPointer</c>: when one of the following conditions is met:
		///    <ul>
		///     <li><c>i_refData.p_bzInputS</c> is <c>nullptr</c>.</li>
		///     <li><c>i_refData.p_bzSampleSpaceA</c> is <c>nullptr</c>.</li>
		///    </ul>
		///  <c>entropy_estimator_lib::constants::EnmReturnStatus::ErrorInvalidData</c>: when one of the following conditions is met: 
		///    <ul>
		///     <li><c>i_refData.L</c> &gt; INT_MAX.</li>
		///     <li><c>i_refData.L</c> is not equal to <c>i_refData.p_bzInputS-&gt;length(blitz::firstDim)</c>.</li>
		///     <li><c>i_refData.k</c> &lt; 2</li>
		///     <li><c>i_refData.k</c> &gt; 256</li>
		///     <li><c>i_refData.k</c> &gt; INT_MAX</li>
		///     <li><c>i_refData.k</c> is not equal to <c>i_refData.p_bzSampleSpaceA-&gt;length(blitz::firstDim)</c>.</li>
		///     <li><c>i_refData.bits_per_sample</c> &lt; 1</li>
		///     <li><c>i_refData.bits_per_sample</c> &gt; 8</li>
		///    </ul>
		///  <c>entropy_estimator_lib::constants::EnmReturnStatus::Success</c>:  otherwise.
		/// </returns>
		/// <precondition>
		/// </precondition>
		/// <postcondition>
		/// </postcondition>
		// -------------------------------------------------------------------------- //
		ns_consts::EnmReturnStatus perform_common_args_for_estimate(const ns_dt::t_data_for_estimator& i_refData)
		{
			ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorInvalidData;
			if (nullptr == i_refData.p_bzInputS)
			{
				sts = ns_consts::EnmReturnStatus::ErrorNullPointer;
				return sts;
			}
			if (nullptr == i_refData.p_bzSampleSpaceA)
			{
				sts = ns_consts::EnmReturnStatus::ErrorNullPointer;
				return sts;
			}
			if (i_refData.p_bzInputS->length(blitz::firstDim) <= 0)
			{
				return sts;
			}
			if (INT_MAX < i_refData.L)
			{
				return sts;
			}
			if (i_refData.p_bzInputS->length(blitz::firstDim) != static_cast<int>(i_refData.L))
			{
				return sts;
			}
			if (i_refData.k < 2)
			{
				return sts;
			}
			if (INT_MAX < i_refData.k)
			{
				return sts;
			}
			if (256 < i_refData.k)
			{
				return sts;
			}
			if (i_refData.p_bzSampleSpaceA->length(blitz::firstDim) != static_cast<int>(i_refData.k))
			{
				return sts;
			}
			if (i_refData.bits_per_sample < 1)
			{
				return sts;
			}
			if (8 < i_refData.bits_per_sample)
			{
				return sts;
			}

			return sts = ns_consts::EnmReturnStatus::Success;
		}
	}
}