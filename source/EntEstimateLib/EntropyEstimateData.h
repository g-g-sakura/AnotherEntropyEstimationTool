////////////////////////////////////////////////////////////////////////////////
// EntropyEstimateData.h
//
//
// Copyright (c) 2021-2022 G. G. SAKURAI <g.garland823@gmail.com>
//
////////////////////////////////////////////////////////////////////////////////
#if defined(_MSC_VER)
#pragma once
#endif

#ifndef ENTROPY_ESTIMATOR_LIB_ENTROPY_ESTIMATE_DATA_H
#define ENTROPY_ESTIMATOR_LIB_ENTROPY_ESTIMATE_DATA_H

#include <blitz/array.h>
#include <ctime>

namespace entropy_estimator_lib
{
	namespace data_types
	{
		typedef unsigned char	octet;
		
		template<typename T>	struct NullableT
		{
			bool	bIsValueNull;
			T		value;
		};

		template<typename T>	std::ostream& operator<<(std::ostream& os, NullableT<T>	nt)
		{
			if (nt.bIsValueNull)
			{
				os << "--";
			}
			else
			{
				os << static_cast<int>(nt.value);
			}
			return	os;
		}

		typedef struct _TDataForPerformanceInfo
		{
			tm		startTime;
			int64_t		start_delta_microseconds;
			tm		endTime;
			tm		duration;
		} t_data_for_performance_info;

		typedef struct _TDataCommon
		{
			double	min_entropy;
			// -------------------------------------------------------------------------- //
			// 
			// -------------------------------------------------------------------------- //
			double	min_entropy_lower_bound;
			double	min_entropy_upper_bound;
			// -------------------------------------------------------------------------- //
			// 
			// -------------------------------------------------------------------------- //
			double	number_of_significant_digits;
		} t_data_common;

		typedef struct _TDataForMCV
		{
			// -------------------------------------------------------------------------- //
			// 
			// -------------------------------------------------------------------------- //
			t_data_common					t_common;
			// -------------------------------------------------------------------------- //
			// 
			// -------------------------------------------------------------------------- //
			t_data_for_performance_info		t_performance;
			// -------------------------------------------------------------------------- //
			// 
			// -------------------------------------------------------------------------- //
			unsigned int	mode;
			double	p_hat;
			double	p_u;
		} t_data_for_mcv;

		typedef struct _TDataForCollisionEstimate
		{
			// -------------------------------------------------------------------------- //
			// 
			// -------------------------------------------------------------------------- //
			t_data_common					t_common;
			// -------------------------------------------------------------------------- //
			// 
			// -------------------------------------------------------------------------- //
			t_data_for_performance_info		t_performance;
			// -------------------------------------------------------------------------- //
			// 
			// -------------------------------------------------------------------------- //
			double	p;
			long double	x_bar;
			long double	sigma_hat;
			long double	x_bar_prime;

			bool	bIsRootFound;
		} t_data_for_collision_estimate;

		typedef struct _TDataForMarkovEstimate
		{
			// -------------------------------------------------------------------------- //
			// 
			// -------------------------------------------------------------------------- //
			t_data_common					t_common;
			// -------------------------------------------------------------------------- //
			// 
			// -------------------------------------------------------------------------- //
			t_data_for_performance_info		t_performance;
			// -------------------------------------------------------------------------- //
			// 
			// -------------------------------------------------------------------------- //
			double	p_max;
			int		mostLikelySequenceCategoryIndex;

			double	negativeLogPoverLength[14];
			double	p_initial[2];
			double	p_transition[2][2];
		} t_data_for_markov_estimate;

		typedef struct _TDataForCompressionEstimate
		{
			// -------------------------------------------------------------------------- //
			// 
			// -------------------------------------------------------------------------- //
			t_data_common					t_common;
			// -------------------------------------------------------------------------- //
			// 
			// -------------------------------------------------------------------------- //
			t_data_for_performance_info		t_performance;
			// -------------------------------------------------------------------------- //
			// 
			// -------------------------------------------------------------------------- //
			double	p;
			double	x_bar;
			double	sigma_hat;
			double	x_bar_prime;
			int	b;
			int	d;

			bool	bIsRootFound;
		} t_data_for_compression_estimate;

		typedef struct _TDataForTTupleEstimate
		{
			// -------------------------------------------------------------------------- //
			// 
			// -------------------------------------------------------------------------- //
			t_data_common					t_common;
			// -------------------------------------------------------------------------- //
			// 
			// -------------------------------------------------------------------------- //
			t_data_for_performance_info		t_performance;
			// -------------------------------------------------------------------------- //
			// 
			// -------------------------------------------------------------------------- //
			double	p_hat_max;
			double	p;

			int		cutoff;
			int		t;
		} t_data_for_t_tuple_estimate;

		typedef struct _TDataForLRSEstimate
		{
			// -------------------------------------------------------------------------- //
			// 
			// -------------------------------------------------------------------------- //
			t_data_common					t_common;
			// -------------------------------------------------------------------------- //
			// 
			// -------------------------------------------------------------------------- //
			t_data_for_performance_info		t_performance;
			// -------------------------------------------------------------------------- //
			// 
			// -------------------------------------------------------------------------- //
			double	p_u;

			int		cutoff;
			int		u;
			int		nu;
			double	p_hat;
			int		argmax_p_hat;
			//
			//
			// 
			int		initial_u;	// for optimization purpose
		} t_data_for_lrs_estimate;

		typedef struct _TDataForMultiMCWPredictionEstimate
		{
			// -------------------------------------------------------------------------- //
			// 
			// -------------------------------------------------------------------------- //
			t_data_common					t_common;
			// -------------------------------------------------------------------------- //
			// 
			// -------------------------------------------------------------------------- //
			t_data_for_performance_info		t_performance;
			// -------------------------------------------------------------------------- //
			// 
			// -------------------------------------------------------------------------- //
			double	p_local;
			double	p_prime_global;
			double	p_global;

			int		r;
			int		occurrences_at_longest_run;

			int		N;
			int		C;

			int		w[4];
		} t_data_for_multi_mcw_prediction_estimate;

		typedef struct _TDataForLagPredictionEstimate
		{
			// -------------------------------------------------------------------------- //
			// 
			// -------------------------------------------------------------------------- //
			t_data_common					t_common;
			// -------------------------------------------------------------------------- //
			// 
			// -------------------------------------------------------------------------- //
			t_data_for_performance_info		t_performance;
			// -------------------------------------------------------------------------- //
			// 
			// -------------------------------------------------------------------------- //
			double	p_local;
			double	p_prime_global;
			double	p_global;

			int		r;
			int		occurrences_at_longest_run;

			int		D;
			int		N;
			int		C;
		} t_data_for_lag_prediction_estimate;

		typedef struct _TDataForMultiMMCPredictionEstimate
		{
			// -------------------------------------------------------------------------- //
			// 
			// -------------------------------------------------------------------------- //
			t_data_common					t_common;
			// -------------------------------------------------------------------------- //
			// 
			// -------------------------------------------------------------------------- //
			t_data_for_performance_info		t_performance;
			// -------------------------------------------------------------------------- //
			// 
			// -------------------------------------------------------------------------- //
			double	p_local;
			double	p_prime_global;
			double	p_global;
			double	min_entropy;
			double	number_of_significant_digits;

			int		r;
			int		occurrences_at_longest_run;

			int		D;
			int		N;
			int		C;
			int		maxEntries;
		} t_data_for_multi_mmc_prediction_estimate;

		typedef struct _TDataForLZ78YPredictionEstimate
		{
			// -------------------------------------------------------------------------- //
			// 
			// -------------------------------------------------------------------------- //
			t_data_common					t_common;
			// -------------------------------------------------------------------------- //
			// 
			// -------------------------------------------------------------------------- //
			t_data_for_performance_info		t_performance;
			// -------------------------------------------------------------------------- //
			// 
			// -------------------------------------------------------------------------- //
			double	p_local;
			double	p_prime_global;
			double	p_global;
			double	min_entropy;
			double	number_of_significant_digits;
			// -------------------------------------------------------------------------- //
			// 
			// -------------------------------------------------------------------------- //
			int		r;
			int		occurrences_at_longest_run;

			int		B;
			int		N;
			int		C;
			int		maxDictionarySize;
		} t_data_for_lz78_y_prediction_estimate;

		typedef struct _TDataForEstimator
		{
			unsigned int			k;	// size of alphabet
			uintmax_t				L;	// size of input
			unsigned int			bits_per_sample;	//
			blitz::Array<octet, 1>	*p_bzSampleSpaceA;
			blitz::Array<octet, 1>	*p_bzInputS;
			bool					bIsMSbFirstByteBitConversion;
			// -------------------------------------------------------------------------- //
			// 
			// -------------------------------------------------------------------------- //
			t_data_for_mcv								t_6_3_1;
			t_data_for_collision_estimate				t_6_3_2;
			t_data_for_markov_estimate					t_6_3_3;
			t_data_for_compression_estimate				t_6_3_4;
			t_data_for_t_tuple_estimate					t_6_3_5;
			t_data_for_lrs_estimate						t_6_3_6;
			t_data_for_multi_mcw_prediction_estimate	t_6_3_7;
			t_data_for_lag_prediction_estimate			t_6_3_8;
			t_data_for_multi_mmc_prediction_estimate	t_6_3_9;
			t_data_for_lz78_y_prediction_estimate		t_6_3_10;
			// -------------------------------------------------------------------------- //
			// 
			// -------------------------------------------------------------------------- //
			//
			bool		isGeneratingReportInLaTeXformatRequested;
			//
			int			verbose_level;	//
			//
			bool		isTestingAllBitsRequested;
			//
			bool		isModeDemonstration;
			// -------------------------------------------------------------------------- //
			// 
			// -------------------------------------------------------------------------- //
			bool		isUsingLcpRequested;
			// -------------------------------------------------------------------------- //
			// 
			// -------------------------------------------------------------------------- //
			std::wstringstream* p_ssLaTeXFragment;
			// -------------------------------------------------------------------------- //
			// 
			// -------------------------------------------------------------------------- //
		} t_data_for_estimator;

	}
}

#endif