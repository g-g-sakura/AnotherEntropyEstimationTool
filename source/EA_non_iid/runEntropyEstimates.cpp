////////////////////////////////////////////////////////////////////////////////
// runEntropyEstimates.cpp
//
//
//
// Copyright (c) 2021-2022 G. G. SAKURAI <g.garland823@gmail.com>
//
////////////////////////////////////////////////////////////////////////////////
#include "runEntropyEstimates.h"
#include "EntEstimateLib/support/showTestSummary.h"
#include "parse.h"
#include <EntEstimateLib/6_3_1_MostCommonValueEstimate.h>
#include <EntEstimateLib/6_3_2_CollisionEstimate.h>
#include <EntEstimateLib/6_3_3_MarkovEstimate.h>
#include <EntEstimateLib/6_3_4_CompressionEstimate.h>
#include <EntEstimateLib/6_3_5_t_TupleEstimate.h>
#include <EntEstimateLib/6_3_6_LRSEstimate.h>
#include <EntEstimateLib/6_3_7_MMCWPredictionEstimate.h>
#include <EntEstimateLib/6_3_8_LagPredictionEstimate.h>
#include <EntEstimateLib/6_3_9_MultiMMCPredictionEstimate.h>
#include <EntEstimateLib/6_3_10_LZ78YPredictionEstimate.h>
#include "boost/date_time/posix_time/posix_time.hpp"

typedef ns_consts::EnmReturnStatus(*PF_EE)(ns_dt::t_data_for_estimator&);

ns_consts::EnmReturnStatus runEntropyEstimatesBinary(ns_dt::t_data_for_estimator& io_refData)
{
    PF_EE	funcs_binary[11] = {
                        entropy_estimator_lib::estimators::mcv::estimate,
                        entropy_estimator_lib::estimators::collision::estimate,
                        entropy_estimator_lib::estimators::markov::estimate,
                        entropy_estimator_lib::estimators::compression::estimate,
                        entropy_estimator_lib::estimators::t_tuple::estimate,
                        entropy_estimator_lib::estimators::lrs::estimate,
                        entropy_estimator_lib::estimators::mmcw_prediction::estimate,
                        entropy_estimator_lib::estimators::lag_prediction::estimate,
                        entropy_estimator_lib::estimators::multimmc_prediction::estimate,
                        entropy_estimator_lib::estimators::lz78y_prediction::estimate,
                        0
    };

    ns_consts::EnmNonIIDTrack   enm_estimate_ids_binary[] = {
            ns_consts::EnmNonIIDTrack::EstimatorMostCommonValue,
            ns_consts::EnmNonIIDTrack::EstimatorCollision,
            ns_consts::EnmNonIIDTrack::EstimatorMarkov,
            ns_consts::EnmNonIIDTrack::EstimatorCompression,
            ns_consts::EnmNonIIDTrack::EstimatorTTuple,
            ns_consts::EnmNonIIDTrack::EstimatorLRS,
            ns_consts::EnmNonIIDTrack::EstimatorMMCWPrediction,
            ns_consts::EnmNonIIDTrack::EstimatorLagPrediction,
            ns_consts::EnmNonIIDTrack::EstimatorMultiMMCPrediction,
            ns_consts::EnmNonIIDTrack::EstimatorLZ78YPrediction
    };

    ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorUnexpected;
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    ns_consts::EnmSampleInterpretation  enm_interp = ns_consts::EnmSampleInterpretation::ELiteralButBinary;
    double  min_entropy_bitstring = 1.0;
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    std::stringstream ss = std::stringstream();
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    for (int j = 0; j < 10; ++j)
    {
        // -------------------------------------------------------------------------- //
        // show case specific header
        // -------------------------------------------------------------------------- //
        ns_consts::EnmReturnStatus stsShowInfo = entropy_estimator_lib::support::showTestSpecificHeader(ss, enm_estimate_ids_binary[j],
            enm_interp, io_refData);
        if (ns_consts::EnmReturnStatus::Success != stsShowInfo)
        {
            return  sts = stsShowInfo;
        }
        // -------------------------------------------------------------------------- //
        // run entropy estimate
        // -------------------------------------------------------------------------- //
        sts = (*funcs_binary[j])(io_refData);
        if (ns_consts::EnmReturnStatus::Success != sts)
        {
            std::cout << "The entropy estimate was terminated..." << std::endl;
            return  sts;
        }
        // -------------------------------------------------------------------------- //
        // show case specific footer
        // -------------------------------------------------------------------------- //
        stsShowInfo = entropy_estimator_lib::support::showTestSpecificFooter(ss, enm_estimate_ids_binary[j],
            enm_interp, io_refData);
        if (ns_consts::EnmReturnStatus::Success != stsShowInfo)
        {
            return  sts = stsShowInfo;
        }
        // -------------------------------------------------------------------------- //
        // show test result
        // -------------------------------------------------------------------------- //
        stsShowInfo = entropy_estimator_lib::support::showTestSpecificResult(ss, enm_estimate_ids_binary[j],
            enm_interp, io_refData);
        if (ns_consts::EnmReturnStatus::Success != stsShowInfo)
        {
            return  sts = stsShowInfo;
        }
        // -------------------------------------------------------------------------- //
        // for the purpose of optimization
        // initial value for searching u is set from the result of t-Tuple Estimate
        // -------------------------------------------------------------------------- //
        switch (j)
        {
        case 4:
            io_refData.t_6_3_6.initial_u = io_refData.t_6_3_5.t;
            break;
        default:
            break;
        }
        // -------------------------------------------------------------------------- //
        // update min-entropy from the current entropy estimate performed
        // -------------------------------------------------------------------------- //
        double* p_min_entropy = nullptr;
        switch (j)
        {
        case 0:
            p_min_entropy = &(io_refData.t_6_3_1.t_common.min_entropy);
            break;
        case 1:
            p_min_entropy = &(io_refData.t_6_3_2.t_common.min_entropy);
            break;
        case 2:
            p_min_entropy = &(io_refData.t_6_3_3.t_common.min_entropy);
            break;
        case 3:
            p_min_entropy = &(io_refData.t_6_3_4.t_common.min_entropy);
            break;
        case 4:
            p_min_entropy = &(io_refData.t_6_3_5.t_common.min_entropy);
            break;
        case 5:
            p_min_entropy = &(io_refData.t_6_3_6.t_common.min_entropy);
            break;
        case 6:
            p_min_entropy = &(io_refData.t_6_3_7.t_common.min_entropy);
            break;
        case 7:
            p_min_entropy = &(io_refData.t_6_3_8.t_common.min_entropy);
            break;
        case 8:
            p_min_entropy = &(io_refData.t_6_3_9.t_common.min_entropy);
            break;
        case 9:
            p_min_entropy = &(io_refData.t_6_3_10.t_common.min_entropy);
            break;
        default:
            break;
        }
        if (nullptr != p_min_entropy)
        {
            if (*p_min_entropy < min_entropy_bitstring)
            {
                min_entropy_bitstring = *p_min_entropy;
            }
        }
    }
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    std::cout << std::endl;
    std::cout << "H_original\t\t\t:\t" << min_entropy_bitstring << std::endl;
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    return  sts;
}

ns_consts::EnmReturnStatus runEntropyEstimatesNonBinary(ns_dt::t_data_for_estimator& io_refDataOriginal,
	ns_dt::t_data_for_estimator& io_refDataBinary)
{
    PF_EE	funcs_non_binary[11] = {
                        entropy_estimator_lib::estimators::mcv::estimate,
                        entropy_estimator_lib::estimators::collision::estimate,
                        entropy_estimator_lib::estimators::markov::estimate,
                        entropy_estimator_lib::estimators::compression::estimate,
                        entropy_estimator_lib::estimators::t_tuple::estimate,
                        entropy_estimator_lib::estimators::lrs::estimate,
                        entropy_estimator_lib::estimators::mmcw_prediction::estimate,
                        entropy_estimator_lib::estimators::lag_prediction::estimate,
                        entropy_estimator_lib::estimators::multimmc_prediction::estimate,
                        entropy_estimator_lib::estimators::lz78y_prediction::estimate,
                        0
    };

    ns_consts::EnmNonIIDTrack   enm_estimate_ids_non_binary[] = {
            ns_consts::EnmNonIIDTrack::EstimatorMostCommonValue,
            ns_consts::EnmNonIIDTrack::EstimatorCollision,
            ns_consts::EnmNonIIDTrack::EstimatorMarkov,
            ns_consts::EnmNonIIDTrack::EstimatorCompression,
            ns_consts::EnmNonIIDTrack::EstimatorTTuple,
            ns_consts::EnmNonIIDTrack::EstimatorLRS,
            ns_consts::EnmNonIIDTrack::EstimatorMMCWPrediction,
            ns_consts::EnmNonIIDTrack::EstimatorLagPrediction,
            ns_consts::EnmNonIIDTrack::EstimatorMultiMMCPrediction,
            ns_consts::EnmNonIIDTrack::EstimatorLZ78YPrediction
    };

    ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorUnexpected;
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    std::stringstream ss = std::stringstream();
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    ns_consts::EnmSampleInterpretation  enm_interp[2] =
    { ns_consts::EnmSampleInterpretation::ELiteralNonBinary,
        ns_consts::EnmSampleInterpretation::EInterpretationBinary };
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    double  min_entropy_literal = (double)io_refDataOriginal.bits_per_sample;
    double  min_entropy_bitstring = 1.0;
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    for (int j = 0; j < 10; ++j)
    {
        for (int n = 0; n < 2; ++n)
        {
            switch (j)
            {
            case 1:
            case 2:
            case 3:
                if (0 == n)
                {
                    // skip literal
                    continue;
                }
                break;
            default:
                break;
            }
            // -------------------------------------------------------------------------- //
            // 
            // -------------------------------------------------------------------------- //
            ns_consts::EnmSampleInterpretation* pInterp = &(enm_interp[n]);
            ns_dt::t_data_for_estimator* pData = nullptr;
            switch (n)
            {
            case 0:
                pData = &io_refDataOriginal;
                break;
            case 1:
                pData = &io_refDataBinary;
                break;
            default:
                break;
            }
            // -------------------------------------------------------------------------- //
            // 
            // -------------------------------------------------------------------------- //
            if (nullptr == pData)
            {
                // should not reach here
                return sts = ns_consts::EnmReturnStatus::ErrorNullPointer;
            }
            // -------------------------------------------------------------------------- //
            // show case specific header
            // -------------------------------------------------------------------------- //
            ns_consts::EnmReturnStatus stsShowInfo = entropy_estimator_lib::support::showTestSpecificHeader(ss, enm_estimate_ids_non_binary[j],
                *pInterp, *pData);
            if (ns_consts::EnmReturnStatus::Success != stsShowInfo)
            {
                return  sts = stsShowInfo;
            }
            // -------------------------------------------------------------------------- //
            // run entropy estimate
            // -------------------------------------------------------------------------- //
            sts = (*funcs_non_binary[j])(*pData);
            if (ns_consts::EnmReturnStatus::Success != sts)
            {
                std::cout << "The entropy estimate was terminated..." << std::endl;
                return  sts;
            }
            // -------------------------------------------------------------------------- //
            // show case specific footer
            // -------------------------------------------------------------------------- //
            stsShowInfo = entropy_estimator_lib::support::showTestSpecificFooter(ss, enm_estimate_ids_non_binary[j],
                *pInterp, *pData);
            if (ns_consts::EnmReturnStatus::Success != stsShowInfo)
            {
                return  sts = stsShowInfo;
            }
            // -------------------------------------------------------------------------- //
            // show test result
            // -------------------------------------------------------------------------- //
            stsShowInfo = entropy_estimator_lib::support::showTestSpecificResult(ss, enm_estimate_ids_non_binary[j],
                *pInterp, *pData);
            if (ns_consts::EnmReturnStatus::Success != stsShowInfo)
            {
                return  sts = stsShowInfo;
            }
            // -------------------------------------------------------------------------- //
            // for the purpose of optimization
            // initial value for searching u is set from the result of t-Tuple Estimate
            // -------------------------------------------------------------------------- //
            switch (j)
            {
            case 4:
                switch (n)
                {
                case 0:
                    io_refDataOriginal.t_6_3_6.initial_u = io_refDataOriginal.t_6_3_5.t;
                    break;
                case 1:
                    io_refDataBinary.t_6_3_6.initial_u = io_refDataBinary.t_6_3_5.t;
                    break;
                default:
                    break;
                }
                break;
            default:
                break;
            }
            // -------------------------------------------------------------------------- //
            // update min-entropy from the current entropy estimate performed
            // -------------------------------------------------------------------------- //
            double* p_min_entropy = nullptr;
            switch (j)
            {
            case 0:
                p_min_entropy = &(pData->t_6_3_1.t_common.min_entropy);
                break;
            case 1:
                p_min_entropy = &(pData->t_6_3_2.t_common.min_entropy);
                break;
            case 2:
                p_min_entropy = &(pData->t_6_3_3.t_common.min_entropy);
                break;
            case 3:
                p_min_entropy = &(pData->t_6_3_4.t_common.min_entropy);
                break;
            case 4:
                p_min_entropy = &(pData->t_6_3_5.t_common.min_entropy);
                break;
            case 5:
                p_min_entropy = &(pData->t_6_3_6.t_common.min_entropy);
                break;
            case 6:
                p_min_entropy = &(pData->t_6_3_7.t_common.min_entropy);
                break;
            case 7:
                p_min_entropy = &(pData->t_6_3_8.t_common.min_entropy);
                break;
            case 8:
                p_min_entropy = &(pData->t_6_3_9.t_common.min_entropy);
                break;
            case 9:
                p_min_entropy = &(pData->t_6_3_10.t_common.min_entropy);
                break;
            default:
                break;
            }
            if (nullptr != p_min_entropy)
            {
                switch (n)
                {
                case 0:
                    if (*p_min_entropy < min_entropy_literal)
                    {
                        min_entropy_literal = *p_min_entropy;
                    }
                    break;
                case 1:
                    if (*p_min_entropy < min_entropy_bitstring)
                    {
                        min_entropy_bitstring = *p_min_entropy;
                    }
                    break;
                default:
                    break;
                }
            }
        }
    }
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    std::cout << std::endl;
    std::cout << "H_original\t\t\t:\t" << min_entropy_literal << std::endl;
    std::cout << "H_bitstring\t\t\t:\t" << min_entropy_bitstring << std::endl;
    std::cout << std::endl;
    double  min_entropy_global = min_entropy_literal;
    if ((double)io_refDataOriginal.bits_per_sample * min_entropy_bitstring < min_entropy_literal)
    {
        min_entropy_global = (double)io_refDataOriginal.bits_per_sample * min_entropy_bitstring;
    }
    std::cout << "min(H_original, 8 X H_bitstring):\t" << min_entropy_global << std::endl;
    // -------------------------------------------------------------------------- //
    // 
    // -------------------------------------------------------------------------- //
    return  sts;
}