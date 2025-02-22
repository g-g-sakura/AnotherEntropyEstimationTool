////////////////////////////////////////////////////////////////////////////////
// reporter.h
//
//
// Copyright (c) 2021-2024 G. G. SAKURAI <g.garland823@gmail.com>
//
////////////////////////////////////////////////////////////////////////////////
#if defined(_MSC_VER)
#pragma once
#endif

#ifndef EA_NON_IID_REPORTER_H
#define EA_NON_IID_REPORTER_H

#include "EntEstimateLib/constants.h"
#include "EntEstimateLib/EntropyEstimateData.h"
#include "reportItems.h"

namespace ns_consts = entropy_estimator_lib::constants;
namespace ns_dt = entropy_estimator_lib::data_types;
namespace bs_fs = boost::filesystem;

void showHeadSamples(const ns_dt::t_data_for_estimator& i_refData);
void showTailSamples(const ns_dt::t_data_for_estimator& i_refData);

ns_consts::EnmReturnStatus reportXMLNonBinary(const IDInfoForReport & i_refInfoReport,
	const ns_dt::t_data_for_estimator& i_refDataOriginal, const ns_dt::t_data_for_estimator& i_refDataBinary);

ns_consts::EnmReturnStatus reportXMLBinary(const IDInfoForReport& i_refInfoReport,
	const ns_dt::t_data_for_estimator& i_refDataBinary);

ns_consts::EnmReturnStatus reportLaTeXNonBinary(IDInfoForReport& i_refInfoReport,
	ns_dt::t_data_for_estimator& io_refDataOriginal, ns_dt::t_data_for_estimator& io_refDataBinary);

ns_consts::EnmReturnStatus reportLaTeXBinary(IDInfoForReport& i_refInfoReport,
	ns_dt::t_data_for_estimator& io_refDataBinary);

ns_consts::EnmReturnStatus synthesizeReportPath(bs_fs::path& o_report_complete_path,
	const bs_fs::path& i_path_entropy_input);

ns_consts::EnmReturnStatus synthesizeReportPathTex(bs_fs::path& o_report_complete_path,
	const bs_fs::path& i_path_entropy_input);

ns_consts::EnmReturnStatus loadLaTeXPreamble(std::stringstream&);

#endif