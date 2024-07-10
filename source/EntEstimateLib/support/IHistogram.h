////////////////////////////////////////////////////////////////////////////////
// IHistogram.h
//
//
// Copyright (c) 2021-2022 G. G. SAKURAI <g.garland823@gmail.com>
//
////////////////////////////////////////////////////////////////////////////////
#if defined(_MSC_VER)
#pragma once
#endif

#ifndef ENTROPY_ESTIMATOR_LIB_IHISTOGRAM_H
#define ENTROPY_ESTIMATOR_LIB_IHISTOGRAM_H

#include "../EntropyEstimateData.h"
#include "../constants.h"

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
		/// </params>
		/// <params="i_refDTuple">
		/// </params>
		/// <returns>
		/// </returns>
		/// <precondition>
		/// </precondition>
		/// <postcondition>
		/// </postcondition>
		// -------------------------------------------------------------------------- //
		template <class T>
		class IHistogram
		{
		public:
			virtual ~IHistogram() = default;

			virtual ns_consts::EnmReturnStatus getFrequent(T& o_refFrequent, int start_pos, int end_pos, const ns_dt::t_data_for_estimator& i_refData) const = 0;
		};

	}
}
#endif