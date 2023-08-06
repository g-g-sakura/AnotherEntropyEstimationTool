////////////////////////////////////////////////////////////////////////////////
// calcMessageDigest.h
//
//
// Copyright (c) 2021-2023 G. G. SAKURAI <g.garland823@gmail.com>
//
////////////////////////////////////////////////////////////////////////////////
#if defined(_MSC_VER)
#pragma once
#endif

#ifndef __EA_NON_IID_CALC_MESSAGE_DIGEST_H__
#define __EA_NON_IID_CALC_MESSAGE_DIGEST_H__

#include <string>
#include "EntEstimateLib/constants.h"

namespace ns_consts = entropy_estimator_lib::constants;

namespace entropy_estimator_app
{
	namespace constants
	{
		enum class EnmHashAlgorithm
		{
			ESHA_256 = 0x03040201,
			ESHA_384 = 0x03040202,
			ESHA_512 = 0x03040203,
			//ESHA3_256 = 0x03040208,
			//ESHA3_384 = 0x03040209,
			//ESHA3_512 = 0x0304020a,
		};
	}

	ns_consts::EnmReturnStatus calcMessageDigest(std::string& o_refMessageDigestInHex,
		unsigned char* i_pInputMessage,
		unsigned int i_cuiInputMessageLengthInOctets,
		constants::EnmHashAlgorithm i_enmHashAlgorithm);
}

#endif