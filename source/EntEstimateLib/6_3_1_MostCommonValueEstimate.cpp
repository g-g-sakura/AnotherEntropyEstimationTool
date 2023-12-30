////////////////////////////////////////////////////////////////////////////////
// 6_3_1_MostCommonValueEstimate.cpp
//
//
//
// Copyright (c) 2021-2023 G. G. SAKURAI <g.garland823@gmail.com>
//
////////////////////////////////////////////////////////////////////////////////
#include "pch.h"
#include "6_3_1_MostCommonValueEstimate.h"
#include "./support/checkArgs.h"
#include "./math/SpecialFunctions.h"
#include "./support/WindowedHistogram.h"

namespace entropy_estimator_lib
{
	namespace estimators
	{
		namespace mcv
		{
			namespace ns_consts = entropy_estimator_lib::constants;
			namespace ns_dt = entropy_estimator_lib::data_types;
			namespace ns_es = entropy_estimator_lib::estimators::support;
			namespace ns_spt = entropy_estimator_lib::support;

			// -------------------------------------------------------------------------- //
			/// <summary>
			///  Output LaTeX
			/// </summary>
			/// <remarks>
			/// </remarks>
			/// <params="io_refData">
			/// </params>
			/// <params="i_ref_whg">
			/// </params>
			/// <returns>
			///  <c>entropy_estimator_lib::constants::EnmReturnStatus::ErrorNullPointer</c>:  when the following condition is met:
			///    <ul>
			///     <li><c>io_refData.p_ssLaTeXFragment</c> == nullptr</li>
			///    </ul>
			///  <c>entropy_estimator_lib::constants::EnmReturnStatus::Success</c>:  otherwise.
			/// </returns>
			/// <precondition>
			/// </precondition>
			/// <postcondition>
			/// </postcondition>
			// -------------------------------------------------------------------------- //
			ns_consts::EnmReturnStatus outputLaTeX(ns_dt::t_data_for_estimator& io_refData,
				ns_es::WindowedHistogram& i_ref_whg)
			{
				ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorUnexpected;
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				if (nullptr == io_refData.p_ssLaTeXFragment)
				{
					return	sts = ns_consts::EnmReturnStatus::ErrorNullPointer;
				}
				int	xmax = (*io_refData.p_bzSampleSpaceA)(io_refData.p_bzSampleSpaceA->extent(blitz::firstDim) - 1);
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				(*io_refData.p_ssLaTeXFragment) << L"\\subsection{The Most Common Value Estimate (NIST SP 800-90B Section 6.3.1)}";
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				switch (io_refData.bits_per_sample)
				{
				case 1:
					(*io_refData.p_ssLaTeXFragment) << L"\\label{sec:Binary631}" << std::endl;
					break;
				default:
					(*io_refData.p_ssLaTeXFragment) << L"\\label{sec:NonBinary631}" << std::endl;
					break;
				}
				(*io_refData.p_ssLaTeXFragment) << std::endl;
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				(*io_refData.p_ssLaTeXFragment) << L"\\begin{figure}[htbp]" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"\\centering" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << std::endl;
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				(*io_refData.p_ssLaTeXFragment) << L"\\begin{tikzpicture}" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"\\begin{axis}[" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"	ybar," << std::endl;
				if (2 == io_refData.k)
				{
					(*io_refData.p_ssLaTeXFragment) << L"	bar width=5pt," << std::endl;
				}
				else
				{
					(*io_refData.p_ssLaTeXFragment) << L"	bar width=1.25pt," << std::endl;
				}
				(*io_refData.p_ssLaTeXFragment) << L"	xmin=-0.125," << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"xmax=" << static_cast<double>(xmax) + 0.125 << ",";
				(*io_refData.p_ssLaTeXFragment) << L"	ymin=0," << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"	width=20cm," << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"	xlabel=$x_i$," << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"	ylabel=\\#$\\{x_i \\,\\textrm{in} \\,S\\} / L$" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"]" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"\\addplot coordinates {" << std::endl;
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				ns_es::idx_map& index_map_hg = i_ref_whg.m_hg.get<ns_es::idx>();
				for (ns_es::idx_map::iterator it = index_map_hg.begin(); it != index_map_hg.end(); ++it)
				{
					(*io_refData.p_ssLaTeXFragment) << L"(";
					(*io_refData.p_ssLaTeXFragment) << std::setw(8) << (it->idx);
					(*io_refData.p_ssLaTeXFragment) << L", " << std::setw(8) << static_cast<double>(it->cnt) / static_cast<double>(io_refData.L);
					(*io_refData.p_ssLaTeXFragment) << L")" << std::endl;
				}
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				(*io_refData.p_ssLaTeXFragment) << L"};" << std::endl;
				// -------------------------------------------------------------------------- //
				// 
				// -------------------------------------------------------------------------- //
				(*io_refData.p_ssLaTeXFragment) << L"\\addplot+[Nigelle,no marks,sharp plot,update limits=false] " << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"coordinates {(0," << io_refData.t_6_3_1.p_hat << L") (" << xmax << L"," << io_refData.t_6_3_1.p_hat << L")}" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"node[above] at (axis cs:" << 0.5 * xmax << L"," << io_refData.t_6_3_1.p_hat << L") {\\shortstack{$\\hat{p}$ = " << std::endl;
				(*io_refData.p_ssLaTeXFragment) << io_refData.t_6_3_1.p_hat << L"\\\\($\\rightarrow$ min-entropy = " << io_refData.t_6_3_1.t_common.min_entropy;
				(*io_refData.p_ssLaTeXFragment) << L" [bit / " << io_refData.bits_per_sample << L"-bit])}};" << std::endl;
				// -------------------------------------------------------------------------- //
				// 
				// -------------------------------------------------------------------------- //
				(*io_refData.p_ssLaTeXFragment) << L"\\end{axis}" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"\\end{tikzpicture}" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"\\caption{Distribution of $x_i$}" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"\\end{figure}" << std::endl;
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				return sts = ns_consts::EnmReturnStatus::Success;
			}

			// -------------------------------------------------------------------------- //
			/// <summary>
			///  Output supplemental information in LaTeX format
			/// </summary>
			/// <remarks>
			/// </remarks>
			/// <params="io_refData">
			/// </params>
			/// <returns>
			///  <c>entropy_estimator_lib::constants::EnmReturnStatus::ErrorNullPointer</c>:  when the following condition is met:
			///    <ul>
			///     <li><c>io_refData.p_ssLaTeXFragment</c> == nullptr</li>
			///    </ul>
			///  <c>entropy_estimator_lib::constants::EnmReturnStatus::Success</c>:  otherwise.
			/// </returns>
			/// <precondition>
			/// </precondition>
			/// <postcondition>
			/// </postcondition>
			// -------------------------------------------------------------------------- //
			ns_consts::EnmReturnStatus outputSupplementalInfoLaTeX(ns_dt::t_data_for_estimator& io_refData)
			{
				ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorUnexpected;
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				if (nullptr == io_refData.p_ssLaTeXFragment)
				{
					return	sts = ns_consts::EnmReturnStatus::ErrorNullPointer;
				}
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				(*io_refData.p_ssLaTeXFragment) << L"\\subsubsection{Supplemental information for traceability}" << std::endl;
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				(*io_refData.p_ssLaTeXFragment) << L"\\renewcommand{\\arraystretch}{1.8}" << std::endl;
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				(*io_refData.p_ssLaTeXFragment) << L"\\begin{table}[h]" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"\\caption{Supplemental information for traceability (NIST SP 800-90B Section 6.3.1)}" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"\\begin{center}" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"\\begin{tabular}{|l|c|}" << std::endl;
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				(*io_refData.p_ssLaTeXFragment) << L"\\hline " << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"\\rowcolor{anotherlightblue} %%" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"Symbol				& Value ";
				(*io_refData.p_ssLaTeXFragment) << L"\\\\ \\hline " << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"mode				& " << std::setw(8) << io_refData.t_6_3_1.mode;
				(*io_refData.p_ssLaTeXFragment) << L"\\\\ \\hline " << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"$\\hat{p}$ 			& " << std::setw(8) << io_refData.t_6_3_1.p_hat;
				(*io_refData.p_ssLaTeXFragment) << L"\\\\ \\hline" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"$p_u$				& " << std::setw(8) << io_refData.t_6_3_1.p_u;
				(*io_refData.p_ssLaTeXFragment) << L"\\\\ \\hline" << std::endl;
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				(*io_refData.p_ssLaTeXFragment) << L"\\end{tabular}" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"\\end{center}" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"\\end{table}" << std::endl;
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				(*io_refData.p_ssLaTeXFragment) << L"\\renewcommand{\\arraystretch}{1.4}" << std::endl;
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
			/// <params="i_refData">
			/// </params>
			/// <returns>
			/// </returns>
			/// <precondition>
			/// </precondition>
			/// <postcondition>
			/// </postcondition>
			// -------------------------------------------------------------------------- //
			ns_consts::EnmReturnStatus check_args_for_estimate(const ns_dt::t_data_for_estimator& i_refData)
			{
				ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorUnexpected;

				const ns_consts::EnmReturnStatus	stsCommon = ns_spt::perform_common_args_for_estimate(i_refData);
				if (ns_consts::EnmReturnStatus::Success != stsCommon)
				{
					return sts = stsCommon;
				}

				if (i_refData.L < 2)
				{
					sts = ns_consts::EnmReturnStatus::ErrorInvalidData;
					return sts;
				}

				return sts = ns_consts::EnmReturnStatus::Success;
			}

			// -------------------------------------------------------------------------- //
			/// <summary>
			///  Implements The Most Common Value Estimate (see 6.3.1 of NIST SP 800-90B).
			/// </summary>
			/// <remarks>
			/// </remarks>
			/// <params="io_refData">
			/// </params>
			/// <returns>
			/// </returns>
			/// <precondition>
			/// </precondition>
			/// <postcondition>
			/// </postcondition>
			// -------------------------------------------------------------------------- //
			ns_consts::EnmReturnStatus estimate(ns_dt::t_data_for_estimator& io_refData)
			{
				ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorUnexpected;

				sts = check_args_for_estimate(io_refData);
				if (ns_consts::EnmReturnStatus::Success != sts)
				{
					return sts;
				}

				// Calc $Z_{\alpha}$ value (=2.5758293035489008)
				const double z_alpha = calc_Z_alpha(0.995);
				// -------------------------------------------------------------------------- //
				// Step 1.
				//  Find the proportion of the most common value \hat{p} in the dataset
				// -------------------------------------------------------------------------- //
				ns_es::WindowedHistogram	whg(io_refData.p_bzInputS->length(blitz::firstDim), 0, io_refData.p_bzInputS->length(blitz::firstDim) - 1, io_refData);
				ns_es::t_bin	binFrequent = { 0, 0, 0 };
				sts = whg.getFrequent(binFrequent);
				if (ns_consts::EnmReturnStatus::Success != sts)
				{
					return sts;
				}
				const unsigned int max_occurrences = binFrequent.cnt;
				// -------------------------------------------------------------------------- //
				// Step 2.
				//  Calculate an upper bound on the probability of the most common value p_{u}
				// -------------------------------------------------------------------------- //
				io_refData.t_6_3_1.t_common.number_of_significant_digits = log10(static_cast<double>(max_occurrences));
				const double p_hat = static_cast<double>(max_occurrences) / static_cast<double>(io_refData.L);
				double p_u = p_hat + z_alpha * sqrt(p_hat * (1.0 - p_hat) / static_cast<double>(io_refData.L - 1));
				if (1.0 < p_u)
				{
					p_u = 1.0;
					io_refData.t_6_3_1.t_common.number_of_significant_digits = 1.0;
				}
				// -------------------------------------------------------------------------- //
				// Step 3.
				//  The estimated min-entropy is -log2(p_{u}).
				// -------------------------------------------------------------------------- //
				io_refData.t_6_3_1.mode = max_occurrences;
				io_refData.t_6_3_1.p_hat = p_hat;
				io_refData.t_6_3_1.p_u = p_u;
				io_refData.t_6_3_1.t_common.min_entropy = -log2(p_u);
				// -------------------------------------------------------------------------- //
				// Estimate significant digits
				//  
				// -------------------------------------------------------------------------- //
				const double p_hat_plus_eps = static_cast<double>(max_occurrences + 1) / static_cast<double>(io_refData.L);
				double p_u_plus_eps = p_hat_plus_eps + z_alpha * sqrt(p_hat_plus_eps * (1.0 - p_hat_plus_eps) / static_cast<double>(io_refData.L - 1));
				if (1.0 < p_u_plus_eps)
				{
					p_u_plus_eps = 1.0;
				}
				double p_hat_minus_eps = static_cast<double>(max_occurrences - 1) / static_cast<double>(io_refData.L);
				if (max_occurrences <= 1)
				{
					p_hat_minus_eps = 1.0 / static_cast<double>(io_refData.L);
				}
				double p_u_minus_eps = p_hat_minus_eps + z_alpha * sqrt(p_hat_minus_eps * (1.0 - p_hat_minus_eps) / static_cast<double>(io_refData.L - 1));
				if (p_u_minus_eps < (1.0 / io_refData.k))
				{
					p_u_minus_eps = 1.0 / io_refData.k;
				}
				// -------------------------------------------------------------------------- //
				// 
				// -------------------------------------------------------------------------- //
				io_refData.t_6_3_1.t_common.min_entropy_lower_bound = -log2(p_u_plus_eps);
				io_refData.t_6_3_1.t_common.min_entropy_lower_bound = -log2(p_u_minus_eps);

				const double	delta_entropy = fabs(-log2(p_u_plus_eps) + log2(p_u));
				io_refData.t_6_3_1.t_common.number_of_significant_digits = -log10(delta_entropy / io_refData.t_6_3_1.t_common.min_entropy);
				// -------------------------------------------------------------------------- //
				// output LaTeX
				// -------------------------------------------------------------------------- //
				if (io_refData.isGeneratingReportInLaTeXformatRequested)
				{
					// -------------------------------------------------------------------------- //
					// 
					// -------------------------------------------------------------------------- //
					outputLaTeX(io_refData, whg);
					// -------------------------------------------------------------------------- //
					// supplemental information for traceability
					// -------------------------------------------------------------------------- //
					outputSupplementalInfoLaTeX(io_refData);
				}
				// -------------------------------------------------------------------------- //
				// 
				// -------------------------------------------------------------------------- //
				return sts = ns_consts::EnmReturnStatus::Success;
			}

		}
	}
}