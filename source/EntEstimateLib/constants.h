////////////////////////////////////////////////////////////////////////////////
// constants.h
//
//
// Copyright (c) 2021-2022 G. G. SAKURAI <g.garland823@gmail.com>
//
////////////////////////////////////////////////////////////////////////////////
#if defined(_MSC_VER)
#pragma once
#endif

#ifndef ENTROPY_ESTIMATOR_LIB_CONSTANTS_H
#define ENTROPY_ESTIMATOR_LIB_CONSTANTS_H

namespace entropy_estimator_lib
{
	namespace  constants
	{
		enum class EnmReturnStatus
		{
			Success = 0,
			ErrorNotFound = -8,
			ErrorFirstIndexNotFound = -9,
			ErrorFirstIndexFoundButSecondIndexNotFound = -10,
			ErrorNoTask = -16,
			ErrorInvalidData = -120,
			ErrorDomain = -121,
			ErrorMemoryAllocation = -126,
			ErrorNullPointer = -127,
			ErrorFileIO = -250,
			ErrorPreconditions = -251,
			ErrorInsufficientData = -252,
			ErrorChildProcess = -480,
			ErrorUnexpected = -8192,
		};

		enum class EnmNonIIDTrack
		{
			EstimatorMostCommonValue = 1,
			EstimatorCollision,
			EstimatorMarkov,
			EstimatorCompression,
			EstimatorTTuple,
			EstimatorLRS,
			EstimatorMMCWPrediction,
			EstimatorLagPrediction,
			EstimatorMultiMMCPrediction,
			EstimatorLZ78YPrediction,
		};

		enum class EnmSampleInterpretation
		{
			ELiteralNonBinary		= 8,
			EInterpretationBinary	= 1,
			ELiteralButBinary		= 9,
		};
	}
}

#endif