////////////////////////////////////////////////////////////////////////////////
// enumerateAlphabet.cpp
//
//
//
// Copyright (c) 2021-2023 G. G. SAKURAI <g.garland823@gmail.com>
//
////////////////////////////////////////////////////////////////////////////////
#include "../pch.h"
#include "enumerateAlphabet.h"
#include <map>

namespace entropy_estimator_lib
{
	namespace support
	{
		namespace ns_consts = entropy_estimator_lib::constants;
		namespace ns_dt = entropy_estimator_lib::data_types;

		// -------------------------------------------------------------------------- //
		/// <summary>
		///  Analyze the input samples, and enumerate the set of alphabet
		/// </summary>
		/// <remarks>
		///  Enumerated alphabet will be stored in <c>*(io_refData.p_bzSampleSpaceA)</c>
		/// </remarks>
		/// <params="io_refData">
		///  Specifies the reference to data structure for entropy estimation.
		/// </params>
		/// <returns>
		/// </returns>
		/// <precondition>
		/// </precondition>
		/// <postcondition>
		/// </postcondition>
		// -------------------------------------------------------------------------- //
		ns_consts::EnmReturnStatus enumerateAlphabet(ns_dt::t_data_for_estimator& io_refData)
		{
			ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorNullPointer;
			// -------------------------------------------------------------------------- //
			//
			// -------------------------------------------------------------------------- //
			if (io_refData.p_bzInputS == nullptr)
			{
				return	sts;
			}
			if (io_refData.p_bzSampleSpaceA == nullptr)
			{
				return	sts;
			}
			// -------------------------------------------------------------------------- //
			//
			// -------------------------------------------------------------------------- //
			std::map<ns_dt::octet, int>	mp_sampleA;
			for (int i = 0; i < io_refData.p_bzInputS->length(blitz::firstDim); ++i)
			{
				std::map<ns_dt::octet, int>::const_iterator	cit = mp_sampleA.find((*io_refData.p_bzInputS)(i));
				if (cit == mp_sampleA.cend())
				{
					mp_sampleA.insert(std::make_pair((*io_refData.p_bzInputS)(i), 1));
				}
			}
			// -------------------------------------------------------------------------- //
			//
			// -------------------------------------------------------------------------- //
			io_refData.p_bzSampleSpaceA->resizeAndPreserve(mp_sampleA.size());
			(*io_refData.p_bzSampleSpaceA) = 0;
			int	index = 0;
			for (const auto& e : mp_sampleA)
			{
				(*io_refData.p_bzSampleSpaceA)(index) = e.first;
				++index;
			}
			// -------------------------------------------------------------------------- //
			//
			// -------------------------------------------------------------------------- //
			io_refData.k = static_cast<unsigned int>(index);
			// -------------------------------------------------------------------------- //
			//
			// -------------------------------------------------------------------------- //
			return sts = ns_consts::EnmReturnStatus::Success;
		}
	}
}