////////////////////////////////////////////////////////////////////////////////
// 6_3_5_t_TupleEstimate.cpp
//
//
//
// Copyright (c) 2021-2023 G. G. SAKURAI <g.garland823@gmail.com>
//
////////////////////////////////////////////////////////////////////////////////
#include "pch.h"
#include "6_3_5_t_TupleEstimate.h"
#include "./support/checkArgs.h"
#include "./support/conversion.h"
#include "./math/SpecialFunctions.h"
#include <boost/math/special_functions.hpp>
#include <boost/math/tools/roots.hpp>
#include "./support/TupleHistogram.h"

namespace entropy_estimator_lib
{
	namespace estimators
	{
		namespace t_tuple
		{
			namespace ns_consts = entropy_estimator_lib::constants;
			namespace ns_dt = entropy_estimator_lib::data_types;
			namespace ns_spt = entropy_estimator_lib::support;
			namespace ns_es = entropy_estimator_lib::estimators::support;
			namespace mp = boost::multiprecision;

			// -------------------------------------------------------------------------- //
			/// <summary>
			///  Output LaTeX header for
			/// </summary>
			/// <remarks>
			/// </remarks>
			/// <params="io_refData">
			/// </params>
			/// <params="i_ref_bz_Q">
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
				blitz::Array<int, 1>& i_ref_bz_Q)
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
				(*io_refData.p_ssLaTeXFragment) << L"\\clearpage" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"\\subsection{The t-tuple Estimate (NIST SP 800-90B Section 6.3.5)}" << std::endl;
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				(*io_refData.p_ssLaTeXFragment) << L"\\begin{figure}[htbp]" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"\\begin{tikzpicture}" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"\\begin{semilogyaxis}[" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"	width=20cm," << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"	xlabel=$i$," << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"	ylabel=$Q \\lbrack i \\rbrack $" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"]" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"\\addplot coordinates {" << std::endl;
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				for (int i = 0; i < i_ref_bz_Q.length(blitz::firstDim); ++i)
				{
					(*io_refData.p_ssLaTeXFragment) << L"(";
					(*io_refData.p_ssLaTeXFragment) << std::setw(4) << (i + 1);
					(*io_refData.p_ssLaTeXFragment) << L", " << i_ref_bz_Q(i);
					(*io_refData.p_ssLaTeXFragment) << L")" << std::endl;
				}
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				(*io_refData.p_ssLaTeXFragment) << L"};" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"\\end{semilogyaxis}" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"\\end{tikzpicture}" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"\\caption{Intermediate value $Q[i]$ \\, in $\\S$6.3.5 of NIST SP 800-90B}" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"\\end{figure}" << std::endl;
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				(*io_refData.p_ssLaTeXFragment) << L"\\begin{figure}[htbp]" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"\\begin{tikzpicture}" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"\\begin{axis}[" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"	width=20cm," << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"	xlabel=$i$," << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"	ylabel=$\\left( P \\lbrack i \\rbrack \\right)^{1/i}$," << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"	/pgf/number format/.cd,fixed,precision=6" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"]" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"\\addplot coordinates {" << std::endl;
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				for (int i = 0; i < i_ref_bz_Q.length(blitz::firstDim); ++i)
				{
					double	P_i = (double)i_ref_bz_Q(i) / (double)(io_refData.L - i);
					double	P_i_power = pow(P_i, 1.0 / (double)(i + 1));
					(*io_refData.p_ssLaTeXFragment) << L"(";
					(*io_refData.p_ssLaTeXFragment) << std::setw(4) << (i + 1) << L", ";
					(*io_refData.p_ssLaTeXFragment) << std::setw(8) << (P_i_power);
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
				(*io_refData.p_ssLaTeXFragment) << L"coordinates {(" << 1 << L"," << io_refData.t_6_3_5.p_hat_max << L") (" << io_refData.t_6_3_5.t << L"," << io_refData.t_6_3_5.p_hat_max << L")}" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"node[above left] at (axis cs:" << io_refData.t_6_3_5.t << L"," << io_refData.t_6_3_5.p_hat_max;
				(*io_refData.p_ssLaTeXFragment) << L") {\\shortstack{$\\hat{p}_{\\textrm{max}}$ = " << io_refData.t_6_3_5.p_hat_max;
				(*io_refData.p_ssLaTeXFragment) << L"\\\\($\\rightarrow$ min-entropy = " << io_refData.t_6_3_5.t_common.min_entropy;
				(*io_refData.p_ssLaTeXFragment) << L" [bit / " << io_refData.bits_per_sample << L"-bit])}};" << std::endl;
				// -------------------------------------------------------------------------- //
				// 
				// -------------------------------------------------------------------------- //
				(*io_refData.p_ssLaTeXFragment) << L"\\end{axis}" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"\\end{tikzpicture}" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"\\caption{$P[i]^{1/i}$ \\, in $\\S$6.3.5 of NIST SP 800-90B}" << std::endl;
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
				(*io_refData.p_ssLaTeXFragment) << L"\\clearpage" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"\\subsubsection{Supplemental information for traceability}" << std::endl;
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				(*io_refData.p_ssLaTeXFragment) << L"\\renewcommand{\\arraystretch}{1.8}" << std::endl;
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				(*io_refData.p_ssLaTeXFragment) << L"\\begin{table}[h]" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"\\caption{Supplemental information for traceability (NIST SP 800-90B Section 6.3.5)}" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"\\begin{center}" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"\\begin{tabular}{|l|c|}" << std::endl;
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				(*io_refData.p_ssLaTeXFragment) << L"\\hline " << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"\\rowcolor{rowcolorlightblue} %%" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"Symbol				& Value ";
				(*io_refData.p_ssLaTeXFragment) << L"\\\\ \\hline " << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"$t$				& " << std::setw(8) << io_refData.t_6_3_5.t;
				(*io_refData.p_ssLaTeXFragment) << L"\\\\ \\hline " << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"$\\hat{p}_{\\textrm{max}}$ 			& " << std::setw(8) << io_refData.t_6_3_5.p_hat_max;
				(*io_refData.p_ssLaTeXFragment) << L"\\\\ \\hline" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"$p_u$				& " << std::setw(8) << io_refData.t_6_3_5.p;
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
			///  Implements steps 1 to 2 of 6.3.5 of NIST SP 800-90B.
			/// </summary>
			/// <remarks>
			/// </remarks>
			/// <params="o_ref_bz_Q">
			/// </params>
			/// <params="io_refData">
			/// </params>
			/// <returns>
			///  <c>entropy_estimator_lib::constants::EnmReturnStatus::ErrorInvalidData</c>: when one of the following conditions is met:
			///    <ul>
			///     <li><c>io_refData.bits_per_sample</c> &lt; 1</li>
			///     <li><c>io_refData.bits_per_sample</c> &gt; 8</li>
			///    </ul>
			///  <c>entropy_estimator_lib::constants::EnmReturnStatus::Success</c>:  otherwise.
			/// </returns>
			/// <precondition>
			/// </precondition>
			/// <postcondition>
			/// </postcondition>
			// -------------------------------------------------------------------------- //
			ns_consts::EnmReturnStatus steps12(blitz::Array<int, 1>& o_ref_bz_Q,
				ns_dt::t_data_for_estimator& io_refData)
			{
				ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorUnexpected;

				int number_of_occurrences = 0;
				int t = 0;
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				do
				{
					++t;
					// -------------------------------------------------------------------------- //
					//
					// -------------------------------------------------------------------------- //
					ns_es::BitSetHistogram		hg;
					int		max_number_of_occurrences_for_current_t = 0;
					// -------------------------------------------------------------------------- //
					//
					// -------------------------------------------------------------------------- //
					for (int i = 0; i < io_refData.p_bzInputS->length(blitz::firstDim) - (t - 1); ++i)
					{
						blitz::Array<ns_dt::octet, 1> bz_tuple = (*io_refData.p_bzInputS)(blitz::Range(i, i + t - 1));
						// -------------------------------------------------------------------------- //
						//
						// -------------------------------------------------------------------------- //
						boost::dynamic_bitset<>		tupleExp(t * io_refData.bits_per_sample, 0);
						sts = ns_spt::convertSeqSamplesToBitSet(tupleExp, bz_tuple, io_refData.bits_per_sample);
						if (ns_consts::EnmReturnStatus::Success != sts)
						{
							return sts;
						}
						// -------------------------------------------------------------------------- //
						//
						// -------------------------------------------------------------------------- //
						int	countForThisTupleExp = 1;
						ns_consts::EnmReturnStatus	stsUpdate = ns_es::incrementCount(countForThisTupleExp, tupleExp, hg);
						if (ns_consts::EnmReturnStatus::ErrorNotFound == stsUpdate)
						{
							hg.insert(ns_es::t_bsx_bin(tupleExp, 1));
						}
						else if (ns_consts::EnmReturnStatus::Success == stsUpdate)
						{
							tupleExp.clear();
						}
						else if (ns_consts::EnmReturnStatus::Success != stsUpdate)
						{
							tupleExp.clear();
							return sts = stsUpdate;
						}
						// -------------------------------------------------------------------------- //
						//
						// -------------------------------------------------------------------------- //
						if (max_number_of_occurrences_for_current_t < countForThisTupleExp)
						{
							max_number_of_occurrences_for_current_t = countForThisTupleExp;
						}
					}
					// -------------------------------------------------------------------------- //
					//
					// -------------------------------------------------------------------------- //
					number_of_occurrences = max_number_of_occurrences_for_current_t;
					// -------------------------------------------------------------------------- //
					//
					// -------------------------------------------------------------------------- //
					if (io_refData.t_6_3_5.cutoff <= number_of_occurrences)
					{
						o_ref_bz_Q.resizeAndPreserve(t);
						o_ref_bz_Q(t - 1) = number_of_occurrences;
					}
					// -------------------------------------------------------------------------- //
					//
					// -------------------------------------------------------------------------- //
					hg.clear();
					// -------------------------------------------------------------------------- //
					//
					// -------------------------------------------------------------------------- //
				} while (io_refData.t_6_3_5.cutoff <= number_of_occurrences);
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				io_refData.t_6_3_5.t = (t - 1);
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				return sts = ns_consts::EnmReturnStatus::Success;
			}

			// -------------------------------------------------------------------------- //
			/// <summary>
			///  Implements steps 3 to 5 of 6.3.5 of NIST SP 800-90B.
			/// </summary>
			/// <remarks>
			/// </remarks>
			/// <params="io_refData">
			/// </params>
			/// <params="i_ref_bz_Q">
			/// </params>
			/// <returns>
			///  <c>entropy_estimator_lib::constants::EnmReturnStatus::Success</c>: if this function is executed successfully.
			/// </returns>
			/// <precondition>
			/// </precondition>
			/// <postcondition>
			/// </postcondition>
			// -------------------------------------------------------------------------- //
			ns_consts::EnmReturnStatus steps345(ns_dt::t_data_for_estimator& io_refData,
				blitz::Array<int, 1>& i_ref_bz_Q)
			{
				ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorUnexpected;
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				blitz::Array<double, 1>	bz_P(i_ref_bz_Q.length(blitz::firstDim));
				bz_P = 0.0;
				for (int i = 0; i < i_ref_bz_Q.length(blitz::firstDim); ++i)
				{
					bz_P(i) = i_ref_bz_Q(i) / (double)(io_refData.L - i);
				}
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				blitz::Array<double, 1>	bz_P_max(bz_P.length(blitz::firstDim));
				bz_P_max = 0.0;
				for (int i = 0; i < bz_P.length(blitz::firstDim); ++i)
				{
					bz_P_max(i) = pow(bz_P(i), (double)(1.0 / (double)(i + 1)));
				}
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				const double	z_alpha = calc_Z_alpha(0.995);

				const double	p_hat_max = blitz::max(bz_P_max);

				double	p_u = p_hat_max + z_alpha * sqrt((p_hat_max * (1.0 - p_hat_max)) / (io_refData.L - 1));

				if (1.0 < p_u)
				{
					p_u = 1.0;
				}
				io_refData.t_6_3_5.p_hat_max = p_hat_max;
				io_refData.t_6_3_5.p = p_u;
				io_refData.t_6_3_5.t_common.min_entropy = -log2(p_u);
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
			///     <li>When <c>i_refData.isModeDemonstration</c> is false, <c>i_refData.t_6_3_5.cutoff</c> is not identical to the value in NIST SP 800-90B.</li>
			///     <li>When <c>i_refData.isModeDemonstration</c> is true, <c>i_refData.t_6_3_5.cutoff</c> is not identical to the value in examples in NIST SP 800-90B 6.3.5.</li>
			///    </ul>
			///  <c>entropy_estimator_lib::constants::EnmReturnStatus::Success</c>:  otherwise.
			/// </returns>
			/// <precondition>
			/// </precondition>
			/// <postcondition>
			/// </postcondition>
			// -------------------------------------------------------------------------- //
			ns_consts::EnmReturnStatus check_args_for_estimate(const ns_dt::t_data_for_estimator& i_refData)
			{
				ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorUnexpected;

				ns_consts::EnmReturnStatus	stsCommon = ns_spt::perform_common_args_for_estimate(i_refData);
				if (ns_consts::EnmReturnStatus::Success != stsCommon)
				{
					return sts = stsCommon;
				}
				if (i_refData.L < 2)
				{
					sts = ns_consts::EnmReturnStatus::ErrorInvalidData;
					return sts;
				}
				int	def_cutoff = 35;
				// -------------------------------------------------------------------------- //
				// for demonstrating examples in NIST SP 800-90B 6.3.x
				// -------------------------------------------------------------------------- //
				if (true == i_refData.isModeDemonstration)
				{
					def_cutoff = 3;
				}
				if (i_refData.t_6_3_5.cutoff != def_cutoff)
				{
					sts = ns_consts::EnmReturnStatus::ErrorInvalidData;
					return sts;
				}

				return sts = ns_consts::EnmReturnStatus::Success;
			}

			// -------------------------------------------------------------------------- //
			/// <summary>
			///  Implements t-Tuple Estimate (see 6.3.5 of NIST SP 800-90B).
			/// </summary>
			/// <remarks>
			/// </remarks>
			/// <params="io_refData">
			/// </params>
			/// <returns>
			///  <c>entropy_estimator_lib::constants::EnmReturnStatus::ErrorNullPointer</c>: when one of the following conditions is met:
			///    <ul>
			///     <li><c>io_refData.p_bzInputS</c> is <c>nullptr</c>.</li>
			///     <li><c>io_refData.p_bzSampleSpaceA</c> is <c>nullptr</c>.</li>
			///    </ul>
			///  <c>entropy_estimator_lib::constants::EnmReturnStatus::ErrorInvalidData</c>: when one of the following conditions is met: 
			///    <ul>
			///     <li><c>io_refData.L</c> &gt; INT_MAX.</li>
			///     <li><c>io_refData.L</c> is not equal to <c>io_refData.p_bzInputS-&gt;length(blitz::firstDim)</c>.</li>
			///     <li><c>io_refData.k</c> &lt; 2</li>
			///     <li><c>io_refData.k</c> &gt; 256</li>
			///     <li><c>io_refData.k</c> &gt; INT_MAX</li>
			///     <li><c>io_refData.k</c> is not equal to <c>io_refData.p_bzSampleSpaceA-&gt;length(blitz::firstDim)</c>.</li>
			///     <li>When <c>io_refData.isModeDemonstration</c> is false, <c>io_refData.t_6_3_5.cutoff</c> is not identical to the value in NIST SP 800-90B.</li>
			///     <li>When <c>io_refData.isModeDemonstration</c> is true, <c>io_refData.t_6_3_5.cutoff</c> is not identical to the value in examples in NIST SP 800-90B 6.3.5.</li>
			///     <li><c>io_refData.bits_per_sample</c> &lt; 1</li>
			///     <li><c>io_refData.bits_per_sample</c> &gt; 8</li>
			///    </ul>
			///  <c>entropy_estimator_lib::constants::EnmReturnStatus::Success</c>:  otherwise.
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

				io_refData.t_6_3_5.t = 1;
				blitz::Array<int, 1>	bz_occurrences(16);
				bz_occurrences = 0;
				// -------------------------------------------------------------------------- //
				// Step 1.
				//  Find the largest t such that the number of occurrences of the most common t-tuple in S is at least 35.
				// -------------------------------------------------------------------------- //
				// -------------------------------------------------------------------------- //
				// Step 2.
				//  Let Q[i] store the number of occurrences of the most common i-tuple in S for i = 1, ..., t. 
				//  For example, in S=(2, 2, 0, 1, 0, 2, 0, 1, 2, 1, 2, 0, 1, 2, 1, 0, 0, 1, 0, 0, 0), 
				//  Q[1] = max(#0Åfs,#1Åfs,#2Åfs) = #0Åfs = 9, and Q[2] = 4 is obtained by the number of the tuple 01 in S.
				// -------------------------------------------------------------------------- //
				sts = steps12(bz_occurrences, io_refData);
				if (ns_consts::EnmReturnStatus::Success != sts)
				{
					return sts;
				}
				// -------------------------------------------------------------------------- //
				// Step 3.
				//  For i = 1 to t, let P[i] = Q[i] / (L-i+1), and compute an estimate on the maximum individual sample value probability as P_{max}[i]=P[i]^{1/i}.
				//  Let p_{max}= max (P_{max}[1], ... , P_{max}[t]).
				// -------------------------------------------------------------------------- //
				// -------------------------------------------------------------------------- //
				// Step 4.
				//  Calculate an upper bound on the probability of the most common value p_{u} as
				// -------------------------------------------------------------------------- //
				// -------------------------------------------------------------------------- //
				// Step 5.
				//  The entropy estimate is calculated as -log2(p_{u}).
				// -------------------------------------------------------------------------- //
				sts = steps345(io_refData, bz_occurrences);
				if (ns_consts::EnmReturnStatus::Success != sts)
				{
					return sts;
				}
				// -------------------------------------------------------------------------- //
				// output LaTeX
				// -------------------------------------------------------------------------- //
				if (io_refData.isGeneratingReportInLaTeXformatRequested)
				{
					// -------------------------------------------------------------------------- //
					// 
					// -------------------------------------------------------------------------- //
					outputLaTeX(io_refData, bz_occurrences);
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