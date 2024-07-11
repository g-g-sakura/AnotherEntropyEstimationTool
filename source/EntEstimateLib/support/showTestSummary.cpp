////////////////////////////////////////////////////////////////////////////////
// showTestSummary.cpp
//
//
//
// Copyright (c) 2021-2023 G. G. SAKURAI <g.garland823@gmail.com>
//
////////////////////////////////////////////////////////////////////////////////
#include "../pch.h"
#include "showTestSummary.h"
#include "boost/date_time/posix_time/posix_time.hpp"

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
		/// <param name="o_refStream">
		/// </param>
		/// <param name="i_enmEstimator">
		/// </param>
		/// <param name="i_enmInterpretation">
		/// </param>
		/// <param name="io_refData">
		/// </param>
		/// <returns>
		/// </returns>
		/// <precondition>
		/// </precondition>
		/// <postcondition>
		/// </postcondition>
		// -------------------------------------------------------------------------- //
		ns_consts::EnmReturnStatus showTestSpecificHeader(std::stringstream& o_refStream,
			const ns_consts::EnmNonIIDTrack& i_enmEstimator,
			const ns_consts::EnmSampleInterpretation& i_enmInterpretation,
			ns_dt::t_data_for_estimator& io_refData)
		{
			ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorUnexpected;
			// -------------------------------------------------------------------------- //
			// 
			// -------------------------------------------------------------------------- //
			o_refStream.str("");
			o_refStream << "\n" << "\n";
			o_refStream << "# -------------------------------------------------------------------------- " << "\n";
			o_refStream << "# " << "\n";
			o_refStream << "# Running ";
			switch (i_enmEstimator)
			{
			case ns_consts::EnmNonIIDTrack::EstimatorMostCommonValue:
				o_refStream << "The Most Common Value Estimate (NIST SP 800-90B Section 6.3.1) ";
				break;
			case ns_consts::EnmNonIIDTrack::EstimatorCollision:
				o_refStream << "The Collision Estimate (NIST SP 800-90B Section 6.3.2) ";
				break;
			case ns_consts::EnmNonIIDTrack::EstimatorMarkov:
				o_refStream << "The Markov Estimate (NIST SP 800-90B Section 6.3.3) ";
				break;
			case ns_consts::EnmNonIIDTrack::EstimatorCompression:
				o_refStream << "The Compression Estimate (NIST SP 800-90B Section 6.3.4) ";
				break;
			case ns_consts::EnmNonIIDTrack::EstimatorTTuple:
				o_refStream << "The t-Tuple Estimate (NIST SP 800-90B Section 6.3.5) ";
				break;
			case ns_consts::EnmNonIIDTrack::EstimatorLRS:
				o_refStream << "The Longest Repeated Substring (LRS) Estimate (NIST SP 800-90B Section 6.3.6) ";
				break;
			case ns_consts::EnmNonIIDTrack::EstimatorMMCWPrediction:
				o_refStream << "The Multi Most Common in Window Prediction Estimate (NIST SP 800-90B Section 6.3.7) ";
				break;
			case ns_consts::EnmNonIIDTrack::EstimatorLagPrediction:
				o_refStream << "The Lag Prediction Estimate (NIST SP 800-90B Section 6.3.8) ";
				break;
			case ns_consts::EnmNonIIDTrack::EstimatorMultiMMCPrediction:
				o_refStream << "The MultiMMC Prediction Estimate (NIST SP 800-90B Section 6.3.9) ";
				break;
			case ns_consts::EnmNonIIDTrack::EstimatorLZ78YPrediction:
				o_refStream << "The LZ78Y Estimate (NIST SP 800-90B Section 6.3.10) ";
				break;
			default:
				break;
			}
			o_refStream << "\n";
			o_refStream << "# " << "\n";
			// -------------------------------------------------------------------------- //
			// 
			// -------------------------------------------------------------------------- //
			o_refStream << "#\tThis estimate is performed on ";
			switch (i_enmInterpretation)
			{
			case ns_consts::EnmSampleInterpretation::ELiteralNonBinary:
			case ns_consts::EnmSampleInterpretation::ELiteralButBinary:
				o_refStream << "original samples.";
				break;
			case ns_consts::EnmSampleInterpretation::EInterpretationBinary:
				o_refStream << "interpreting each sample as bitstrings.";
				break;
			default:
				break;
			}
			o_refStream << "\n";
			// -------------------------------------------------------------------------- //
			// 
			// -------------------------------------------------------------------------- //
			const auto pt_s =
				boost::posix_time::ptime(
					boost::posix_time::microsec_clock::local_time());

			o_refStream << "#\tThis estimate was initiated at \t" << pt_s << "\n";
			// -------------------------------------------------------------------------- //
			// 
			// -------------------------------------------------------------------------- //
			std::cout << o_refStream.str();
			std::cout.flush();
			// -------------------------------------------------------------------------- //
			// 
			// -------------------------------------------------------------------------- //
			ns_dt::t_data_for_performance_info* p_perf = nullptr;
			switch (i_enmEstimator)
			{
			case ns_consts::EnmNonIIDTrack::EstimatorMostCommonValue:
				p_perf = &io_refData.t_6_3_1.t_performance;
				break;
			case ns_consts::EnmNonIIDTrack::EstimatorCollision:
				p_perf = &io_refData.t_6_3_2.t_performance;
				break;
			case ns_consts::EnmNonIIDTrack::EstimatorMarkov:
				p_perf = &io_refData.t_6_3_3.t_performance;
				break;
			case ns_consts::EnmNonIIDTrack::EstimatorCompression:
				p_perf = &io_refData.t_6_3_4.t_performance;
				break;
			case ns_consts::EnmNonIIDTrack::EstimatorTTuple:
				p_perf = &io_refData.t_6_3_5.t_performance;
				break;
			case ns_consts::EnmNonIIDTrack::EstimatorLRS:
				p_perf = &io_refData.t_6_3_6.t_performance;
				break;
			case ns_consts::EnmNonIIDTrack::EstimatorMMCWPrediction:
				p_perf = &io_refData.t_6_3_7.t_performance;
				break;
			case ns_consts::EnmNonIIDTrack::EstimatorLagPrediction:
				p_perf = &io_refData.t_6_3_8.t_performance;
				break;
			case ns_consts::EnmNonIIDTrack::EstimatorMultiMMCPrediction:
				p_perf = &io_refData.t_6_3_9.t_performance;
				break;
			case ns_consts::EnmNonIIDTrack::EstimatorLZ78YPrediction:
				p_perf = &io_refData.t_6_3_10.t_performance;
				break;
			default:
				break;
			}
			if (nullptr != p_perf)
			{
				p_perf->startTime = boost::posix_time::to_tm(pt_s);

				const boost::posix_time::ptime	pt_round = boost::posix_time::ptime_from_tm(p_perf->startTime);
				boost::posix_time::time_duration	tdelta = pt_s - pt_round;
				p_perf->start_delta_microseconds = tdelta.total_microseconds();
			}
			// -------------------------------------------------------------------------- //
			// 
			// -------------------------------------------------------------------------- //
			return sts = ns_consts::EnmReturnStatus::Success;
		}

		// -------------------------------------------------------------------------- //
		/// <summary>
		/// </summary>
		/// <remarks>
		/// </remarks>
		/// <param name="o_refStream">
		/// </param>
		/// <param name="i_enmEstimator">
		/// </param>
		/// <param name="i_enmInterpretation">
		/// </param>
		/// <param name="io_refData">
		/// </param>
		/// <returns>
		/// </returns>
		/// <precondition>
		/// </precondition>
		/// <postcondition>
		/// </postcondition>
		// -------------------------------------------------------------------------- //
		ns_consts::EnmReturnStatus showTestSpecificFooter(std::stringstream& o_refStream,
			const ns_consts::EnmNonIIDTrack& i_enmEstimator,
			ns_consts::EnmSampleInterpretation& i_enmInterpretation,
			ns_dt::t_data_for_estimator& io_refData)
		{
			ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorUnexpected;
			// -------------------------------------------------------------------------- //
			// 
			// -------------------------------------------------------------------------- //
			const auto pt_e =
				boost::posix_time::ptime(
					boost::posix_time::microsec_clock::local_time());

			// -------------------------------------------------------------------------- //
			// 
			// -------------------------------------------------------------------------- //
			o_refStream.str("");
			o_refStream << "# " << "\n";
			o_refStream << "#\tThis estimate was finished at \t" << pt_e << "\n";
			// -------------------------------------------------------------------------- //
			// 
			// -------------------------------------------------------------------------- //
			ns_dt::t_data_for_performance_info* p_perf = nullptr;
			switch (i_enmEstimator)
			{
			case ns_consts::EnmNonIIDTrack::EstimatorMostCommonValue:
				p_perf = &io_refData.t_6_3_1.t_performance;
				break;
			case ns_consts::EnmNonIIDTrack::EstimatorCollision:
				p_perf = &io_refData.t_6_3_2.t_performance;
				break;
			case ns_consts::EnmNonIIDTrack::EstimatorMarkov:
				p_perf = &io_refData.t_6_3_3.t_performance;
				break;
			case ns_consts::EnmNonIIDTrack::EstimatorCompression:
				p_perf = &io_refData.t_6_3_4.t_performance;
				break;
			case ns_consts::EnmNonIIDTrack::EstimatorTTuple:
				p_perf = &io_refData.t_6_3_5.t_performance;
				break;
			case ns_consts::EnmNonIIDTrack::EstimatorLRS:
				p_perf = &io_refData.t_6_3_6.t_performance;
				break;
			case ns_consts::EnmNonIIDTrack::EstimatorMMCWPrediction:
				p_perf = &io_refData.t_6_3_7.t_performance;
				break;
			case ns_consts::EnmNonIIDTrack::EstimatorLagPrediction:
				p_perf = &io_refData.t_6_3_8.t_performance;
				break;
			case ns_consts::EnmNonIIDTrack::EstimatorMultiMMCPrediction:
				p_perf = &io_refData.t_6_3_9.t_performance;
				break;
			case ns_consts::EnmNonIIDTrack::EstimatorLZ78YPrediction:
				p_perf = &io_refData.t_6_3_10.t_performance;
				break;
			default:
				break;
			}
			if (nullptr != p_perf)
			{
				p_perf->endTime = boost::posix_time::to_tm(pt_e);

				const boost::posix_time::ptime    pt_s = boost::posix_time::ptime_from_tm(p_perf->startTime);
				boost::posix_time::time_duration    tdelta = boost::posix_time::microseconds(p_perf->start_delta_microseconds);
				const boost::posix_time::time_duration    td = pt_e - pt_s - tdelta;
				p_perf->duration = boost::posix_time::to_tm(td);

				o_refStream << "#\tThe time duration for this Entropy Estimate :\t" << boost::posix_time::to_simple_string(td) << "\n";
			}
			// -------------------------------------------------------------------------- //
			// 
			// -------------------------------------------------------------------------- //
			std::cout << o_refStream.str();
			std::cout.flush();
			// -------------------------------------------------------------------------- //
			// 
			// -------------------------------------------------------------------------- //
			return sts = ns_consts::EnmReturnStatus::Success;
		}

		// -------------------------------------------------------------------------- //
		/// <summary>
		/// </summary>
		/// <remarks>
		/// </remarks>
		/// <param name="o_refStream">
		/// </param>
		/// <param name="i_enmEstimator">
		/// </param>
		/// <param name="i_enmInterpretation">
		/// </param>
		/// <param name="io_refData">
		/// </param>
		/// <returns>
		/// </returns>
		/// <precondition>
		/// </precondition>
		/// <postcondition>
		/// </postcondition>
		// -------------------------------------------------------------------------- //
		ns_consts::EnmReturnStatus showTestSpecificResult(std::stringstream& o_refStream,
			ns_consts::EnmNonIIDTrack& i_enmEstimator,
			ns_consts::EnmSampleInterpretation& i_enmInterpretation,
			ns_dt::t_data_for_estimator& io_refData)
		{
			ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorUnexpected;
			// -------------------------------------------------------------------------- //
			// 
			// -------------------------------------------------------------------------- //
			std::streamsize	size_real_prec_tracing = 10;
			std::streamsize	size_real_prec_final = 7;
			// -------------------------------------------------------------------------- //
			// 
			// -------------------------------------------------------------------------- //
			std::stringstream	ssSampleInterp;
			std::stringstream	ssTrail;
			std::stringstream	ssUnitInfo;
			std::stringstream	ssSeparator = std::stringstream("#\n# --------------------------------------------------------------------------\n");
			std::stringstream	ssSeparatorSimple = std::stringstream("# --------------------------------------------------------------------------\n");
			switch (i_enmInterpretation)
			{
			case ns_consts::EnmSampleInterpretation::ELiteralNonBinary:
			case ns_consts::EnmSampleInterpretation::ELiteralButBinary:
				ssSampleInterp = std::stringstream("Literal ");
				ssTrail = std::stringstream();
				ssUnitInfo << " / " << io_refData.bits_per_sample << "-bit";
				break;
			case ns_consts::EnmSampleInterpretation::EInterpretationBinary:
				ssSampleInterp = std::stringstream("Bitstring ");
				ssTrail = std::stringstream(" (bit string)");
				ssUnitInfo = std::stringstream(" / 1-bit");
				break;
			default:
				break;
			}
			// -------------------------------------------------------------------------- //
			// 
			// -------------------------------------------------------------------------- //
			o_refStream.str("");
			o_refStream << "# Finalizing ..." << "\n";
			// -------------------------------------------------------------------------- //
			// 
			// -------------------------------------------------------------------------- //
			switch (i_enmEstimator)
			{
			case ns_consts::EnmNonIIDTrack::EstimatorMostCommonValue:
				// -------------------------------------------------------------------------- //
				// 
				// -------------------------------------------------------------------------- //
				if (1 < io_refData.verbose_level)
				{
					o_refStream << ssSeparator.str();
					o_refStream << "#   " << "Mode count \t= " << io_refData.t_6_3_1.mode << "\n";
					o_refStream << "#   " << "p-hat \t= " << std::setprecision(size_real_prec_tracing) << io_refData.t_6_3_1.p_hat << "\n";
					o_refStream << "#   " << "p_u \t= " << std::setprecision(size_real_prec_tracing) << io_refData.t_6_3_1.p_u << "\n";
					o_refStream << "#   " << "min entropy (lower bound)" << ssTrail.str() << " = ";
					o_refStream << std::setprecision(size_real_prec_final) << io_refData.t_6_3_1.t_common.min_entropy_lower_bound << ssUnitInfo.str() << "\n";
					o_refStream << "#   " << "min entropy (upper bound)" << ssTrail.str() << " = ";
					o_refStream << std::setprecision(size_real_prec_final) << io_refData.t_6_3_1.t_common.min_entropy_upper_bound << ssUnitInfo.str() << "\n";
					o_refStream << "#   " << "number of significant digits of min entropy \t= " << std::setprecision(2) << io_refData.t_6_3_1.t_common.number_of_significant_digits << "\n";
					// -------------------------------------------------------------------------- //
					// 
					// -------------------------------------------------------------------------- //
					o_refStream << ssSeparatorSimple.str();
					o_refStream << "# " << ssSampleInterp.str() << "MCV Estimate:\t" << "\n";
					o_refStream << "#   " << "min entropy" << ssTrail.str() << " = ";
					o_refStream << std::setprecision(size_real_prec_final) << io_refData.t_6_3_1.t_common.min_entropy << ssUnitInfo.str() << "\n";
				}
				else
				{
					o_refStream << "# " << ssSampleInterp.str() << "MCV Estimate:\t";
					o_refStream << "mode = " << io_refData.t_6_3_1.mode << ", ";
					o_refStream << "p-hat = " << std::setprecision(size_real_prec_tracing) << io_refData.t_6_3_1.p_hat << ", ";
					o_refStream << "p_u = " << std::setprecision(size_real_prec_tracing) << io_refData.t_6_3_1.p_u << "\n";
					o_refStream << ssSeparator.str();
					// -------------------------------------------------------------------------- //
					// 
					// -------------------------------------------------------------------------- //
					o_refStream << "\tMost Common Value Estimate" << ssTrail.str() << " = ";
					o_refStream << std::setprecision(size_real_prec_final) << io_refData.t_6_3_1.t_common.min_entropy << ssUnitInfo.str() << "\n";
				}
				break;
			case ns_consts::EnmNonIIDTrack::EstimatorCollision:
				// -------------------------------------------------------------------------- //
				// 
				// -------------------------------------------------------------------------- //
				if (1 < io_refData.verbose_level)
				{
					o_refStream << ssSeparator.str();
					o_refStream << "#   " << "X-bar \t= " << std::setprecision(size_real_prec_tracing) << io_refData.t_6_3_2.x_bar << "\n";
					o_refStream << "#   " << "sigma-hat \t= " << std::setprecision(size_real_prec_tracing) << io_refData.t_6_3_2.sigma_hat << "\n";
					o_refStream << "#   " << "X-bar' \t= " << std::setprecision(size_real_prec_tracing) << io_refData.t_6_3_2.x_bar_prime << "\n";
					if (false == io_refData.t_6_3_2.bIsRootFound)
					{
						o_refStream << "#   " << "Note that the root is not found." << "\n";
					}
					o_refStream << "#   " << "p \t\t= " << std::setprecision(size_real_prec_tracing) << io_refData.t_6_3_2.p << "\n";
					o_refStream << "#   " << "min entropy (lower bound)" << ssTrail.str() << " = ";
					o_refStream << std::setprecision(size_real_prec_final) << io_refData.t_6_3_2.t_common.min_entropy_lower_bound << ssUnitInfo.str() << "\n";
					o_refStream << "#   " << "min entropy (upper bound)" << ssTrail.str() << " = ";
					o_refStream << std::setprecision(size_real_prec_final) << io_refData.t_6_3_2.t_common.min_entropy_upper_bound << ssUnitInfo.str() << "\n";
					o_refStream << "#   " << "number of significant digits of min entropy\t= " << std::setprecision(2) << io_refData.t_6_3_2.t_common.number_of_significant_digits << "\n";
					// -------------------------------------------------------------------------- //
					// 
					// -------------------------------------------------------------------------- //
					o_refStream << ssSeparatorSimple.str();
					o_refStream << "# " << ssSampleInterp.str() << "Collision Estimate:\t" << "\n";
					o_refStream << "#   " << "min entropy" << ssTrail.str() << " = ";
					o_refStream << std::setprecision(size_real_prec_final) << io_refData.t_6_3_2.t_common.min_entropy << ssUnitInfo.str() << "\n";
				}
				else
				{
					o_refStream << "# " << ssSampleInterp.str() << "Collision Estimate:\t";
					o_refStream << "p = " << std::setprecision(size_real_prec_tracing) << io_refData.t_6_3_2.p << "\n";
					o_refStream << "# X-bar = " << std::setprecision(size_real_prec_tracing) << io_refData.t_6_3_2.x_bar << "\n";
					o_refStream << "# X-bar-prime = " << std::setprecision(size_real_prec_tracing) << io_refData.t_6_3_2.x_bar_prime << "\n";
					o_refStream << "# sigma-hat = " << std::setprecision(size_real_prec_tracing) << io_refData.t_6_3_2.sigma_hat << "\n";
					o_refStream << ssSeparator.str();
					// -------------------------------------------------------------------------- //
					// 
					// -------------------------------------------------------------------------- //
					o_refStream << "\tCollision Estimate" << ssTrail.str() << " = ";
					o_refStream << std::setprecision(size_real_prec_final) << io_refData.t_6_3_2.t_common.min_entropy << ssUnitInfo.str() << "\n";
				}
				break;
			case ns_consts::EnmNonIIDTrack::EstimatorMarkov:
				// -------------------------------------------------------------------------- //
				// 
				// -------------------------------------------------------------------------- //
				if (1 < io_refData.verbose_level)
				{
					o_refStream << ssSeparator.str();
					o_refStream << "#   " << "P_0 \t= " << std::setprecision(size_real_prec_tracing) << io_refData.t_6_3_3.p_initial[0] << "\n";
					o_refStream << "#   " << "P_1 \t= " << std::setprecision(size_real_prec_tracing) << io_refData.t_6_3_3.p_initial[1] << "\n";
					o_refStream << "#   " << "P_0,0 \t= " << std::setprecision(size_real_prec_tracing) << io_refData.t_6_3_3.p_transition[0][0] << "\n";
					o_refStream << "#   " << "P_0,1 \t= " << std::setprecision(size_real_prec_tracing) << io_refData.t_6_3_3.p_transition[1][0] << "\n";
					o_refStream << "#   " << "P_1,0 \t= " << std::setprecision(size_real_prec_tracing) << io_refData.t_6_3_3.p_transition[0][1] << "\n";
					o_refStream << "#   " << "P_1,1 \t= " << std::setprecision(size_real_prec_tracing) << io_refData.t_6_3_3.p_transition[1][1] << "\n";
					o_refStream << "#   " << "Most likely sequence index (extended)\t= " << io_refData.t_6_3_3.mostLikelySequenceCategoryIndex + 1 << "\n";
					o_refStream << "#   " << "p-hat_max = " << std::setprecision(size_real_prec_tracing) << io_refData.t_6_3_3.p_max << "\n";
					// -------------------------------------------------------------------------- //
					// 
					// -------------------------------------------------------------------------- //
					o_refStream << ssSeparatorSimple.str();
					o_refStream << "# " << ssSampleInterp.str() << "Markov Estimate:\t" << "\n";
					o_refStream << "#   " << "min entropy" << ssTrail.str() << " = ";
					o_refStream << std::setprecision(size_real_prec_final) << io_refData.t_6_3_3.t_common.min_entropy << ssUnitInfo.str() << "\n";
				}
				else
				{
					o_refStream << "# " << ssSampleInterp.str() << "Markov Estimate:\t";
					o_refStream << "P_0 = " << std::setprecision(size_real_prec_tracing) << io_refData.t_6_3_3.p_initial[0] << ", ";
					o_refStream << "P_1 = " << std::setprecision(size_real_prec_tracing) << io_refData.t_6_3_3.p_initial[1] << ", ";
					o_refStream << "P_0,0 = " << std::setprecision(size_real_prec_tracing) << io_refData.t_6_3_3.p_transition[0][0] << ", ";
					o_refStream << "P_0,1 = " << std::setprecision(size_real_prec_tracing) << io_refData.t_6_3_3.p_transition[1][0] << ", ";
					o_refStream << "P_1,0 = " << std::setprecision(size_real_prec_tracing) << io_refData.t_6_3_3.p_transition[0][1] << ", ";
					o_refStream << "P_1,1 = " << std::setprecision(size_real_prec_tracing) << io_refData.t_6_3_3.p_transition[1][1] << ", ";
					o_refStream << "p-hat_max = " << std::setprecision(size_real_prec_tracing) << io_refData.t_6_3_3.p_max << "\n";
					o_refStream << ssSeparator.str();
					// -------------------------------------------------------------------------- //
					// 
					// -------------------------------------------------------------------------- //
					o_refStream << "\tMarkov Estimate" << ssTrail.str() << " = ";
					o_refStream << std::setprecision(size_real_prec_final) << io_refData.t_6_3_3.t_common.min_entropy << ssUnitInfo.str() << "\n";
				}
				break;
			case ns_consts::EnmNonIIDTrack::EstimatorCompression:
				// -------------------------------------------------------------------------- //
				// 
				// -------------------------------------------------------------------------- //
				if (1 < io_refData.verbose_level)
				{
					o_refStream << ssSeparator.str();
					o_refStream << "#   " << "X-bar \t= " << std::setprecision(size_real_prec_tracing) << io_refData.t_6_3_4.x_bar << "\n";
					o_refStream << "#   " << "sigma-hat \t= " << std::setprecision(size_real_prec_tracing) << io_refData.t_6_3_4.sigma_hat << "\n";
					o_refStream << "#   " << "X-bar' \t= " << std::setprecision(size_real_prec_tracing) << io_refData.t_6_3_4.x_bar_prime << "\n";
					if (false == io_refData.t_6_3_4.bIsRootFound)
					{
						o_refStream << "#   " << "Note that the root is not found." << "\n";
					}
					o_refStream << "#   " << "p \t\t= " << std::setprecision(size_real_prec_tracing) << io_refData.t_6_3_4.p << "\n";
					// -------------------------------------------------------------------------- //
					// 
					// -------------------------------------------------------------------------- //
					o_refStream << ssSeparatorSimple.str();
					o_refStream << "# " << ssSampleInterp.str() << "Compression Estimate:\t" << "\n";
					o_refStream << "#   " << "min entropy" << ssTrail.str() << " = ";
					o_refStream << std::setprecision(size_real_prec_final) << io_refData.t_6_3_4.t_common.min_entropy << ssUnitInfo.str() << "\n";
				}
				else
				{
					o_refStream << "# " << ssSampleInterp.str() << "Compression Estimate:\t";
					o_refStream << "X-bar = " << std::setprecision(size_real_prec_tracing) << io_refData.t_6_3_4.x_bar << ", ";
					o_refStream << "sigma-hat = " << std::setprecision(size_real_prec_tracing) << io_refData.t_6_3_4.sigma_hat << ", ";
					o_refStream << "p = " << std::setprecision(size_real_prec_tracing) << io_refData.t_6_3_4.p << "\n";
					o_refStream << ssSeparator.str();
					// -------------------------------------------------------------------------- //
					// 
					// -------------------------------------------------------------------------- //
					o_refStream << "\tCompression Estimate" << ssTrail.str() << " = ";
					o_refStream << std::setprecision(size_real_prec_final) << io_refData.t_6_3_4.t_common.min_entropy << ssUnitInfo.str() << "\n";
				}
				break;
			case ns_consts::EnmNonIIDTrack::EstimatorTTuple:
				// -------------------------------------------------------------------------- //
				// 
				// -------------------------------------------------------------------------- //
				if (1 < io_refData.verbose_level)
				{
					o_refStream << ssSeparator.str();
					o_refStream << "#   " << "t \t\t= " << io_refData.t_6_3_5.t << "\n";
					o_refStream << "#   " << "p-hat_max \t= " << std::setprecision(size_real_prec_tracing) << io_refData.t_6_3_5.p_hat_max << "\n";
					o_refStream << "#   " << "p_u \t= " << std::setprecision(size_real_prec_tracing) << io_refData.t_6_3_5.p << "\n";
					// -------------------------------------------------------------------------- //
					// 
					// -------------------------------------------------------------------------- //
					o_refStream << ssSeparatorSimple.str();
					o_refStream << "# " << ssSampleInterp.str() << "t-Tuple Estimate:\t" << "\n";
					o_refStream << "#   " << "min entropy" << ssTrail.str() << " = ";
					o_refStream << std::setprecision(size_real_prec_final) << io_refData.t_6_3_5.t_common.min_entropy << ssUnitInfo.str() << "\n";
				}
				else
				{
					o_refStream << "# " << ssSampleInterp.str() << "t-Tuple Estimate:\t";
					o_refStream << "t = " << io_refData.t_6_3_5.t << ", ";
					o_refStream << "p-hat_max = " << std::setprecision(size_real_prec_tracing) << io_refData.t_6_3_5.p_hat_max << ", ";
					o_refStream << "p_u = " << std::setprecision(size_real_prec_tracing) << io_refData.t_6_3_5.p << "\n";
					o_refStream << ssSeparator.str();
					// -------------------------------------------------------------------------- //
					// 
					// -------------------------------------------------------------------------- //
					o_refStream << "\tt-Tuple Estimate" << ssTrail.str() << " = ";
					o_refStream << std::setprecision(size_real_prec_final) << io_refData.t_6_3_5.t_common.min_entropy << ssUnitInfo.str() << "\n";
				}
				break;
			case ns_consts::EnmNonIIDTrack::EstimatorLRS:
				// -------------------------------------------------------------------------- //
				// 
				// -------------------------------------------------------------------------- //
				if (1 < io_refData.verbose_level)
				{
					o_refStream << ssSeparator.str();
					o_refStream << "#   " << "u \t\t= " << io_refData.t_6_3_6.u << "\n";
					o_refStream << "#   " << "nu \t\t= " << io_refData.t_6_3_6.nu << "\n";
					o_refStream << "#   " << "p-hat \t= " << std::setprecision(size_real_prec_tracing) << io_refData.t_6_3_6.p_hat << "\n";
					o_refStream << "#   " << "p_u \t= " << std::setprecision(size_real_prec_tracing) << io_refData.t_6_3_6.p_u << "\n";
					// -------------------------------------------------------------------------- //
					// 
					// -------------------------------------------------------------------------- //
					o_refStream << ssSeparatorSimple.str();
					o_refStream << "# " << ssSampleInterp.str() << "LRS Estimate:\t" << "\n";
					o_refStream << "#   " << "min entropy" << ssTrail.str() << " = ";
					o_refStream << std::setprecision(size_real_prec_final) << io_refData.t_6_3_6.t_common.min_entropy << ssUnitInfo.str() << "\n";
				}
				else
				{
					o_refStream << "# " << ssSampleInterp.str() << "LRS Estimate:\t";
					o_refStream << "u = " << io_refData.t_6_3_6.u << ", ";
					o_refStream << "nu = " << io_refData.t_6_3_6.nu << ", ";
					o_refStream << "p-hat = " << std::setprecision(size_real_prec_tracing) << io_refData.t_6_3_6.p_hat << ", ";
					o_refStream << "p_u = " << std::setprecision(size_real_prec_tracing) << io_refData.t_6_3_6.p_u << "\n";
					o_refStream << ssSeparator.str();
					// -------------------------------------------------------------------------- //
					// 
					// -------------------------------------------------------------------------- //
					o_refStream << "\tLRS Estimate" << ssTrail.str() << " = ";
					o_refStream << std::setprecision(size_real_prec_final) << io_refData.t_6_3_6.t_common.min_entropy << ssUnitInfo.str() << "\n";
				}
				break;
			case ns_consts::EnmNonIIDTrack::EstimatorMMCWPrediction:
				// -------------------------------------------------------------------------- //
				// 
				// -------------------------------------------------------------------------- //
				if (1 < io_refData.verbose_level)
				{
					o_refStream << ssSeparator.str();
					o_refStream << "#   " << "C \t\t= " << io_refData.t_6_3_7.C << "\n";
					o_refStream << "#   " << "r \t\t= " << io_refData.t_6_3_7.r << "\n";
					o_refStream << "#   " << "N \t\t= " << io_refData.t_6_3_7.N << "\n";
					o_refStream << "#   " << "P_global \t= " << std::setprecision(size_real_prec_tracing) << io_refData.t_6_3_7.p_global << "\n";
					o_refStream << "#   " << "P_global' \t= " << std::setprecision(size_real_prec_tracing) << io_refData.t_6_3_7.p_prime_global << "\n";
					o_refStream << "#   " << "P_local \t= " << std::setprecision(size_real_prec_tracing) << io_refData.t_6_3_7.p_local << "\n";
					o_refStream << "#   " << "min entropy (lower bound)" << ssTrail.str() << " = ";
					o_refStream << std::setprecision(size_real_prec_final) << io_refData.t_6_3_7.t_common.min_entropy_lower_bound << ssUnitInfo.str() << "\n";
					o_refStream << "#   " << "min entropy (upper bound)" << ssTrail.str() << " = ";
					o_refStream << std::setprecision(size_real_prec_final) << io_refData.t_6_3_7.t_common.min_entropy_upper_bound << ssUnitInfo.str() << "\n";
					o_refStream << "#   " << "number of significant digits of min entropy \t= " << std::setprecision(2) << io_refData.t_6_3_7.t_common.number_of_significant_digits << "\n";
					// -------------------------------------------------------------------------- //
					// 
					// -------------------------------------------------------------------------- //
					o_refStream << ssSeparatorSimple.str();
					o_refStream << "# " << ssSampleInterp.str() << "MultiMCW Prediction Estimate:\t" << "\n";
					o_refStream << "#   " << "min entropy" << ssTrail.str() << " = ";
					o_refStream << std::setprecision(size_real_prec_final) << io_refData.t_6_3_7.t_common.min_entropy << ssUnitInfo.str() << "\n";
				}
				else
				{
					o_refStream << "# " << ssSampleInterp.str() << "MultiMCW Prediction Estimate:\t";
					o_refStream << "N = " << io_refData.t_6_3_7.N << ", ";
					o_refStream << "Pglobal' = " << std::setprecision(size_real_prec_tracing) << io_refData.t_6_3_7.p_prime_global << " ";
					o_refStream << "(C = " << io_refData.t_6_3_7.C << "), ";
					o_refStream << "Plocal = " << std::setprecision(size_real_prec_tracing) << io_refData.t_6_3_7.p_local << " ";
					o_refStream << "(r = " << io_refData.t_6_3_7.r << ")" << "\n";
					o_refStream << ssSeparator.str();
					// -------------------------------------------------------------------------- //
					// 
					// -------------------------------------------------------------------------- //
					o_refStream << "\tMulti Most Common in Window (MultiMCW) Prediction Estimate" << ssTrail.str() << " = ";
					o_refStream << std::setprecision(size_real_prec_final) << io_refData.t_6_3_7.t_common.min_entropy << ssUnitInfo.str() << "\n";
				}
				break;
			case ns_consts::EnmNonIIDTrack::EstimatorLagPrediction:
				// -------------------------------------------------------------------------- //
				// 
				// -------------------------------------------------------------------------- //
				if (1 < io_refData.verbose_level)
				{
					o_refStream << ssSeparator.str();
					o_refStream << "#   " << "C \t\t= " << io_refData.t_6_3_8.C << "\n";
					o_refStream << "#   " << "r \t\t= " << io_refData.t_6_3_8.r << "\n";
					o_refStream << "#   " << "N \t\t= " << io_refData.t_6_3_8.N << "\n";
					o_refStream << "#   " << "P_global \t= " << std::setprecision(size_real_prec_tracing) << io_refData.t_6_3_8.p_global << "\n";
					o_refStream << "#   " << "P_global' \t= " << std::setprecision(size_real_prec_tracing) << io_refData.t_6_3_8.p_prime_global << "\n";
					o_refStream << "#   " << "P_local \t= " << std::setprecision(size_real_prec_tracing) << io_refData.t_6_3_8.p_local << "\n";
					o_refStream << "#   " << "min entropy (lower bound)" << ssTrail.str() << " = ";
					o_refStream << std::setprecision(size_real_prec_final) << io_refData.t_6_3_8.t_common.min_entropy_lower_bound << ssUnitInfo.str() << "\n";
					o_refStream << "#   " << "min entropy (upper bound)" << ssTrail.str() << " = ";
					o_refStream << std::setprecision(size_real_prec_final) << io_refData.t_6_3_8.t_common.min_entropy_upper_bound << ssUnitInfo.str() << "\n";
					o_refStream << "#   " << "number of significant digits of min entropy \t= " << std::setprecision(2) << io_refData.t_6_3_8.t_common.number_of_significant_digits << "\n";
					// -------------------------------------------------------------------------- //
					// 
					// -------------------------------------------------------------------------- //
					o_refStream << ssSeparatorSimple.str();
					o_refStream << "# " << ssSampleInterp.str() << "Lag Prediction Estimate:\t" << "\n";
					o_refStream << "#   " << "min entropy" << ssTrail.str() << " = ";
					o_refStream << std::setprecision(size_real_prec_final) << io_refData.t_6_3_8.t_common.min_entropy << ssUnitInfo.str() << "\n";
				}
				else
				{
					o_refStream << "# " << ssSampleInterp.str() << "Lag Prediction Estimate:\t";
					o_refStream << "N = " << io_refData.t_6_3_8.N << ", ";
					o_refStream << "Pglobal' = " << std::setprecision(size_real_prec_tracing) << io_refData.t_6_3_8.p_prime_global << " ";
					o_refStream << "(C = " << io_refData.t_6_3_8.C << "), ";
					o_refStream << "Plocal = " << std::setprecision(size_real_prec_tracing) << io_refData.t_6_3_8.p_local << " ";
					o_refStream << "(r = " << io_refData.t_6_3_8.r << ")" << "\n";
					o_refStream << ssSeparator.str();
					// -------------------------------------------------------------------------- //
					// 
					// -------------------------------------------------------------------------- //
					o_refStream << "\tLag Prediction Estimate" << ssTrail.str() << " = ";
					o_refStream << std::setprecision(size_real_prec_final) << io_refData.t_6_3_8.t_common.min_entropy << ssUnitInfo.str() << "\n";
				}
				break;
			case ns_consts::EnmNonIIDTrack::EstimatorMultiMMCPrediction:
				// -------------------------------------------------------------------------- //
				// 
				// -------------------------------------------------------------------------- //
				if (1 < io_refData.verbose_level)
				{
					o_refStream << ssSeparator.str();
					o_refStream << "#   " << "C \t\t= " << io_refData.t_6_3_9.C << "\n";
					o_refStream << "#   " << "r \t\t= " << io_refData.t_6_3_9.r << "\n";
					o_refStream << "#   " << "N \t\t= " << io_refData.t_6_3_9.N << "\n";
					o_refStream << "#   " << "P_global \t= " << std::setprecision(size_real_prec_tracing) << io_refData.t_6_3_9.p_global << "\n";
					o_refStream << "#   " << "P_global' \t= " << std::setprecision(size_real_prec_tracing) << io_refData.t_6_3_9.p_prime_global << "\n";
					o_refStream << "#   " << "P_local \t= " << std::setprecision(size_real_prec_tracing) << io_refData.t_6_3_9.p_local << "\n";
					o_refStream << "#   " << "min entropy (lower bound)" << ssTrail.str() << " = ";
					o_refStream << std::setprecision(size_real_prec_final) << io_refData.t_6_3_9.t_common.min_entropy_lower_bound << ssUnitInfo.str() << "\n";
					o_refStream << "#   " << "min entropy (upper bound)" << ssTrail.str() << " = ";
					o_refStream << std::setprecision(size_real_prec_final) << io_refData.t_6_3_9.t_common.min_entropy_upper_bound << ssUnitInfo.str() << "\n";
					o_refStream << "#   " << "number of significant digits of min entropy \t= " << std::setprecision(2) << io_refData.t_6_3_9.t_common.number_of_significant_digits << "\n";
					// -------------------------------------------------------------------------- //
					// 
					// -------------------------------------------------------------------------- //
					o_refStream << ssSeparatorSimple.str();
					o_refStream << "# " << ssSampleInterp.str() << "MultiMMC Prediction Estimate:\t" << "\n";
					o_refStream << "#   " << "min entropy" << ssTrail.str() << " = ";
					o_refStream << std::setprecision(size_real_prec_final) << io_refData.t_6_3_9.t_common.min_entropy << ssUnitInfo.str() << "\n";
				}
				else
				{
					o_refStream << "# " << ssSampleInterp.str() << "MultiMMC Prediction Estimate:\t";
					o_refStream << "N = " << io_refData.t_6_3_9.N << ", ";
					o_refStream << "Pglobal' = " << std::setprecision(size_real_prec_tracing) << io_refData.t_6_3_9.p_prime_global << " ";
					o_refStream << "(C = " << io_refData.t_6_3_9.C << "), ";
					o_refStream << "Plocal = " << std::setprecision(size_real_prec_tracing) << io_refData.t_6_3_9.p_local << " ";
					o_refStream << "(r = " << io_refData.t_6_3_9.r << ")" << "\n";
					o_refStream << ssSeparator.str();
					// -------------------------------------------------------------------------- //
					// 
					// -------------------------------------------------------------------------- //
					o_refStream << "\tMulti Markov Model with Counting (MultiMMC) Prediction Estimate" << ssTrail.str() << " = ";
					o_refStream << std::setprecision(size_real_prec_final) << io_refData.t_6_3_9.t_common.min_entropy << ssUnitInfo.str() << "\n";
				}
				break;
			case ns_consts::EnmNonIIDTrack::EstimatorLZ78YPrediction:
				// -------------------------------------------------------------------------- //
				// 
				// -------------------------------------------------------------------------- //
				if (1 < io_refData.verbose_level)
				{
					o_refStream << ssSeparator.str();
					o_refStream << "#   " << "C \t\t= " << io_refData.t_6_3_10.C << "\n";
					o_refStream << "#   " << "r \t\t= " << io_refData.t_6_3_10.r << "\n";
					o_refStream << "#   " << "N \t\t= " << io_refData.t_6_3_10.N << "\n";
					o_refStream << "#   " << "P_global \t= " << std::setprecision(size_real_prec_tracing) << io_refData.t_6_3_10.p_global << "\n";
					o_refStream << "#   " << "P_global' \t= " << std::setprecision(size_real_prec_tracing) << io_refData.t_6_3_10.p_prime_global << "\n";
					o_refStream << "#   " << "P_local \t= " << std::setprecision(size_real_prec_tracing) << io_refData.t_6_3_10.p_local << "\n";
					o_refStream << "#   " << "min entropy (lower bound)" << ssTrail.str() << " = ";
					o_refStream << std::setprecision(size_real_prec_final) << io_refData.t_6_3_10.t_common.min_entropy_lower_bound << ssUnitInfo.str() << "\n";
					o_refStream << "#   " << "min entropy (upper bound)" << ssTrail.str() << " = ";
					o_refStream << std::setprecision(size_real_prec_final) << io_refData.t_6_3_10.t_common.min_entropy_upper_bound << ssUnitInfo.str() << "\n";
					o_refStream << "#   " << "number of significant digits of min entropy \t= " << std::setprecision(2) << io_refData.t_6_3_10.t_common.number_of_significant_digits << "\n";
					// -------------------------------------------------------------------------- //
					// 
					// -------------------------------------------------------------------------- //
					o_refStream << ssSeparatorSimple.str();
					o_refStream << "# " << ssSampleInterp.str() << "LZ78Y Prediction Estimate:\t" << "\n";
					o_refStream << "#   " << "min entropy" << ssTrail.str() << " = ";
					o_refStream << std::setprecision(size_real_prec_final) << io_refData.t_6_3_10.t_common.min_entropy << ssUnitInfo.str() << "\n";
				}
				else
				{
					o_refStream << "# " << ssSampleInterp.str() << "LZ78Y Prediction Estimate:\t";
					o_refStream << "N = " << io_refData.t_6_3_10.N << ", ";
					o_refStream << "Pglobal' = " << std::setprecision(size_real_prec_tracing) << io_refData.t_6_3_10.p_prime_global << " ";
					o_refStream << "(C = " << io_refData.t_6_3_10.C << "), ";
					o_refStream << "Plocal = " << std::setprecision(size_real_prec_tracing) << io_refData.t_6_3_10.p_local << " ";
					o_refStream << "(r = " << io_refData.t_6_3_10.r << ")" << "\n";
					o_refStream << ssSeparator.str();
					// -------------------------------------------------------------------------- //
					// 
					// -------------------------------------------------------------------------- //
					o_refStream << "\tLZ78Y Prediction Estimate" << ssTrail.str() << " = ";
					o_refStream << std::setprecision(size_real_prec_final) << io_refData.t_6_3_10.t_common.min_entropy << ssUnitInfo.str() << "\n";
				}
				break;
			default:
				break;
			}
			// -------------------------------------------------------------------------- //
			// 
			// -------------------------------------------------------------------------- //
			std::cout << o_refStream.str() << "\n";
			std::cout.flush();
			// -------------------------------------------------------------------------- //
			// 
			// -------------------------------------------------------------------------- //
			return sts = ns_consts::EnmReturnStatus::Success;
		}
	}
}