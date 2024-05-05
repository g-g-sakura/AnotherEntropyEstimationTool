////////////////////////////////////////////////////////////////////////////////
// 6_3_6_LRSEstimate.cpp
//
//
//
// Copyright (c) 2021-2023 G. G. SAKURAI <g.garland823@gmail.com>
//
////////////////////////////////////////////////////////////////////////////////
#include "pch.h"
#include "6_3_6_LRSEstimate.h"
#include "./support/checkArgs.h"
#include "./support/conversion.h"
#include "./math/SpecialFunctions.h"
#include <boost/math/special_functions.hpp>
#include "./support/TupleHistogram.h"

namespace entropy_estimator_lib
{
	namespace estimators
	{
		namespace lrs
		{
			namespace ns_consts = entropy_estimator_lib::constants;
			namespace ns_dt = entropy_estimator_lib::data_types;
			namespace ns_spt = entropy_estimator_lib::support;
			namespace ns_es = entropy_estimator_lib::estimators::support;

			// -------------------------------------------------------------------------- //
			/// <summary>
			///  Output LaTeX header for
			/// </summary>
			/// <remarks>
			/// </remarks>
			/// <params="io_refData">
			/// </params>
			/// <params="extentSecondDim">
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
			ns_consts::EnmReturnStatus outputLaTeXHeaderTuple(ns_dt::t_data_for_estimator& io_refData, int extentSecondDim)
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
				(*io_refData.p_ssLaTeXFragment) << L"\\clearpage" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"\\subsection{The LRS Estimate (NIST SP 800-90B Section 6.3.6)}";
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				switch (io_refData.bits_per_sample)
				{
				case 1:
					(*io_refData.p_ssLaTeXFragment) << L"\\label{sec:Binary636}" << L"\n";
					break;
				default:
					(*io_refData.p_ssLaTeXFragment) << L"\\label{sec:NonBinary636}" << L"\n";
					break;
				}
				(*io_refData.p_ssLaTeXFragment) << L"\n";
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				(*io_refData.p_ssLaTeXFragment) << L"\\begin{figure}[htbp]" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"\\centering" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"\\begin{tikzpicture}" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"\\begin{axis}[" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"	view/h=160," << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"	colormap/bluered, colorbar horizontal," << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"	width=20cm," << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"	ymin=2," << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"	xlabel=$W$," << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"	ylabel=$C_i$," << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"	zlabel=multiplicity," << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"]" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"\\addplot3[surf, ";
				(*io_refData.p_ssLaTeXFragment) << L"mesh/ordering=y varies, shader=faceted interp] coordinates {" << L"\n";
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				return sts = ns_consts::EnmReturnStatus::Success;
			}

			// -------------------------------------------------------------------------- //
			/// <summary>
			///  Output LaTeX header for Pmax
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
			ns_consts::EnmReturnStatus outputLaTeXHeaderPmax(ns_dt::t_data_for_estimator& io_refData)
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
				(*io_refData.p_ssLaTeXFragment) << L"\\begin{figure}[htbp]" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"\\centering" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"\\begin{tikzpicture}" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"\\begin{axis}[" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"	width=20cm," << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"	xlabel=$W$," << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"	ylabel=$\\left( P_W \\right) ^{i/W}$," << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"    ticklabel style={" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"        % change \"directory\" to the number format" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"        /pgf/number format/.cd," << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"            fixed," << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"        % change \"directory\" back to tikz" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"        /tikz/.cd," << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"    }," << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"	yticklabel style = { /pgf/number format/precision=6 }" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"]" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"\\addplot  coordinates {" << L"\n";
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				return sts = ns_consts::EnmReturnStatus::Success;
			}

			// -------------------------------------------------------------------------- //
			/// <summary>
			///  Output LaTeX Footer for P_max
			/// </summary>
			/// <remarks>
			/// </remarks>
			/// <params="io_refData">
			/// </params>
			/// <params="i_refCaption">
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
			ns_consts::EnmReturnStatus outputLaTeXFooter(ns_dt::t_data_for_estimator& io_refData, const std::wstring &i_refCaption)
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
				(*io_refData.p_ssLaTeXFragment) << L"};" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"\\end{axis}" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"\\end{tikzpicture}" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"\\caption{" << i_refCaption << "}" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"\\end{figure}" << L"\n";
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				return sts = ns_consts::EnmReturnStatus::Success;
			}

			// -------------------------------------------------------------------------- //
			/// <summary>
			///  Output LaTeX Footer for \hat{P}
			/// </summary>
			/// <remarks>
			/// </remarks>
			/// <params="io_refData">
			/// </params>
			/// <params="i_refCaption">
			/// </params>
			/// <params="argMax">
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
			ns_consts::EnmReturnStatus outputLaTeXFooterWithPhat(ns_dt::t_data_for_estimator& io_refData, 
				const std::wstring& i_refCaption, int argMax)
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
				int xticks = 1 + io_refData.t_6_3_6.nu - io_refData.t_6_3_6.u;
				int xshift = 0;	// in mm
				if (xticks <= 3)
				{
					if (argMax == io_refData.t_6_3_6.u)
					{
						xshift = 10;
					}
					else if (io_refData.t_6_3_6.nu == argMax)
					{
						xshift = -10;
					}
				}
				else if (xticks <= 5)
				{
					if (argMax <= (io_refData.t_6_3_6.u + 1))
					{
						xshift = 10;
					}
					else if ((io_refData.t_6_3_6.nu - 1) <= argMax)
					{
						xshift = -10;
					}
				}
				else
				{
					if (argMax <= (io_refData.t_6_3_6.u + 2))
					{
						xshift = 10;
					}
					else if ((io_refData.t_6_3_6.nu - 2) <= argMax)
					{
						xshift = -10;
					}
				}
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				(*io_refData.p_ssLaTeXFragment) << L"};" << L"\n";
				// -------------------------------------------------------------------------- //
				// 
				// -------------------------------------------------------------------------- //
				(*io_refData.p_ssLaTeXFragment) << L"\\addplot+[Nigelle,no marks,sharp plot,update limits=false] " << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"coordinates {(" << io_refData.t_6_3_6.u << L"," << io_refData.t_6_3_6.p_hat << L") (" << io_refData.t_6_3_6.nu << L"," << io_refData.t_6_3_6.p_hat << L")}" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"node[above";
				if (0 != xshift)
				{
					(*io_refData.p_ssLaTeXFragment) << L", xshift=" << xshift << L"mm";
				}
				(*io_refData.p_ssLaTeXFragment) << L"] at (axis cs:" << argMax << L"," << io_refData.t_6_3_6.p_hat << L") {\\shortstack{$\\hat{p}$ = ";
				(*io_refData.p_ssLaTeXFragment) << io_refData.t_6_3_6.p_hat;
				(*io_refData.p_ssLaTeXFragment) << L" \\\\($\\rightarrow$ min-entropy = " <<io_refData.t_6_3_6.t_common.min_entropy;
				(*io_refData.p_ssLaTeXFragment) << L" [bit / " << io_refData.bits_per_sample << L"-bit])}};" << L"\n";
				// -------------------------------------------------------------------------- //
				// 
				// -------------------------------------------------------------------------- //
				(*io_refData.p_ssLaTeXFragment) << L"\\end{axis}" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"\\end{tikzpicture}" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"\\caption{" << i_refCaption << "}" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"\\end{figure}" << L"\n";
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
				(*io_refData.p_ssLaTeXFragment) << L"\\clearpage" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"\\subsubsection{Supplemental information for traceability}" << L"\n";
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				(*io_refData.p_ssLaTeXFragment) << L"\\renewcommand{\\arraystretch}{1.8}" << L"\n";
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				(*io_refData.p_ssLaTeXFragment) << L"\\begin{table}[h]" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"\\caption{Supplemental information for traceability (NIST SP 800-90B Section 6.3.6)}" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"\\begin{center}" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"\\begin{tabular}{|l|c|}" << L"\n";
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				(*io_refData.p_ssLaTeXFragment) << L"\\hline " << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"\\rowcolor{anotherlightblue} %%" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"Symbol				& Value ";
				(*io_refData.p_ssLaTeXFragment) << L"\\\\ \\hline " << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"$u$				& " << std::setw(8) << io_refData.t_6_3_6.u;
				(*io_refData.p_ssLaTeXFragment) << L"\\\\ \\hline " << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"$v$				& " << std::setw(8) << io_refData.t_6_3_6.nu;
				(*io_refData.p_ssLaTeXFragment) << L"\\\\ \\hline " << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"$\\hat{p}$ 			& " << std::setw(8) << io_refData.t_6_3_6.p_hat;
				(*io_refData.p_ssLaTeXFragment) << L"\\\\ \\hline" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"$p_u$				& " << std::setw(8) << io_refData.t_6_3_6.p_u;
				(*io_refData.p_ssLaTeXFragment) << L"\\\\ \\hline" << L"\n";
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				(*io_refData.p_ssLaTeXFragment) << L"\\end{tabular}" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"\\end{center}" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"\\end{table}" << L"\n";
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				(*io_refData.p_ssLaTeXFragment) << L"\\renewcommand{\\arraystretch}{1.4}" << L"\n";
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				return sts = ns_consts::EnmReturnStatus::Success;
			}

			// -------------------------------------------------------------------------- //
			/// <summary>
			///  Implements steps 1 to 3 of 6.3.6 of NIST SP 800-90B.
			/// </summary>
			/// <remarks>
			/// </remarks>
			/// <params="o_ref_bz_P_W">
			///  Specifies the reference to store $P_{W}$ in step 3.
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
			ns_consts::EnmReturnStatus steps123(blitz::Array<double, 1>& o_ref_bz_P_W,
				ns_dt::t_data_for_estimator& io_refData)
			{
				ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorUnexpected;

				int number_of_occurrences_for_most_common_t_tuple = 0;
				int t = 0;
				int u = 0;
				bool	bIsUFound = false;
				// -------------------------------------------------------------------------- //
				// for LaTeX output
				// -------------------------------------------------------------------------- //
				std::wstringstream	ssFragmentForLaTeX;
				std::wstringstream	ssFragmentForLaTeXPmax;
				int		global_Ci_max = 0;
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				if (1 < io_refData.t_6_3_6.initial_u)
				{
					// -------------------------------------------------------------------------- //
					// for optimization
					// where pre-calculated u value is available through 6.3.5 of NIST SP 800-90B.
					// -------------------------------------------------------------------------- //
					t = (io_refData.t_6_3_6.initial_u - 1);
				}
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
						boost::dynamic_bitset<>		bitSetExp(t * io_refData.bits_per_sample, 0);
						sts = ns_spt::convertSeqSamplesToBitSet(bitSetExp, bz_tuple, io_refData.bits_per_sample);
						if (ns_consts::EnmReturnStatus::Success != sts)
						{
							return sts;
						}
						// -------------------------------------------------------------------------- //
						//
						// -------------------------------------------------------------------------- //
						int	countForThisTupleExp = 1;
						ns_consts::EnmReturnStatus	stsUpdate = ns_es::incrementCount(countForThisTupleExp, bitSetExp, hg);
						if (ns_consts::EnmReturnStatus::ErrorNotFound == stsUpdate)
						{
							hg.insert(ns_es::t_bsx_bin(bitSetExp, 1));
						}
						else if (ns_consts::EnmReturnStatus::Success == stsUpdate)
						{
							bitSetExp.clear();
						}
						else if (ns_consts::EnmReturnStatus::Success != stsUpdate)
						{
							bitSetExp.clear();
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
					number_of_occurrences_for_most_common_t_tuple = max_number_of_occurrences_for_current_t;
					// -------------------------------------------------------------------------- //
					//
					// -------------------------------------------------------------------------- //
					if (false == bIsUFound)
					{
						if (number_of_occurrences_for_most_common_t_tuple < io_refData.t_6_3_6.cutoff)
						{
							u = t;
							bIsUFound = true;
						}
					}
					// -------------------------------------------------------------------------- //
					//
					// -------------------------------------------------------------------------- //
					if (true == bIsUFound)
					{
						if (2 <= number_of_occurrences_for_most_common_t_tuple)
						{
							// -------------------------------------------------------------------------- //
							// For W = u to \nu, compute the estimated W-tuple collision probability
							//   $P_{W} = \sum_{i} \binom_{C_{i}}{2} / \binom{L - W + 1}{2}$
							// where C_{i} is the number of occurrences of the i-th unique W-tuple.
							// -------------------------------------------------------------------------- //
							ns_es::tpl_bscnt_map& cnt_map_hg = hg.get<ns_es::bsx_cnt>();

							double	numerator = 0.0;
							// -------------------------------------------------------------------------- //
							//
							// -------------------------------------------------------------------------- //
							uint64_t	ui64_numerator = 0;
							// -------------------------------------------------------------------------- //
							//
							// -------------------------------------------------------------------------- //
							for (ns_es::tpl_bscnt_map::iterator it = cnt_map_hg.begin(); it != cnt_map_hg.end(); ++it)
							{
								if (2 <= it->ex_cnt)
								{
									numerator += boost::math::binomial_coefficient<double>(it->ex_cnt, 2);
									// -------------------------------------------------------------------------- //
									//
									// -------------------------------------------------------------------------- //
									ui64_numerator += ((static_cast<uint64_t>(it->ex_cnt)) * ((static_cast<uint64_t>(it->ex_cnt)) - 1)) >> 1;
									// -------------------------------------------------------------------------- //
									//
									// -------------------------------------------------------------------------- //
								}
							}
							// -------------------------------------------------------------------------- //
							//
							// -------------------------------------------------------------------------- //
							if (true == io_refData.isGeneratingReportInLaTeXformatRequested)
							{
								//ns_es::tpl_bscnt_map::iterator	it_ci_min = cnt_map_hg.begin();
								//int	ci_min = it_ci_min->ex_cnt;
								ns_es::tpl_bscnt_map::reverse_iterator	rit_ci_max = cnt_map_hg.rbegin();
								int	ci_max = rit_ci_max->ex_cnt;

								for (int y = 2; y <= ci_max; ++y)
								{
									std::pair<ns_es::tpl_bscnt_map::iterator, ns_es::tpl_bscnt_map::iterator> rg_x = cnt_map_hg.equal_range(y);

									int	multiplicity_for_ci = 0;
									if (rg_x.first != rg_x.second)
									{
										// multiplicity != 0
										for (ns_es::tpl_bscnt_map::iterator it = rg_x.first; it != rg_x.second; ++it)
										{
											++multiplicity_for_ci;
										}
									}
									else
									{
										// multiplicity = 0;
										//sts = ns_consts::EnmReturnStatus::ErrorNotFound;
									}
									// -------------------------------------------------------------------------- //
									//
									// -------------------------------------------------------------------------- //
									ssFragmentForLaTeX << L"(";
									ssFragmentForLaTeX << std::setw(4) << t;
									ssFragmentForLaTeX << L"," << std::setw(4) << y;
									ssFragmentForLaTeX << L"," << std::setw(8) << multiplicity_for_ci;
									ssFragmentForLaTeX << L")  ";
								}

								for (int y = ci_max + 1; y <= global_Ci_max; ++y)
								{
									ssFragmentForLaTeX << L"(";
									ssFragmentForLaTeX << std::setw(4) << t;
									ssFragmentForLaTeX << L"," << std::setw(4) << y;
									ssFragmentForLaTeX << L"," << std::setw(8) << 0;
									ssFragmentForLaTeX << L")  ";
								}
								ssFragmentForLaTeX << L"\n" << L"\n";
								// -------------------------------------------------------------------------- //
								//
								// -------------------------------------------------------------------------- //
								if (global_Ci_max < ci_max)
								{
									global_Ci_max = ci_max;
								}
							}
							// -------------------------------------------------------------------------- //
							//
							// -------------------------------------------------------------------------- //
							if (1 < io_refData.verbose_level)
							{
								std::cout << "# \tNumerator of P_W ( W = " << std::setw(8) << t << " ) = " << std::setw(18) << ui64_numerator << L"\n";
							}
							// -------------------------------------------------------------------------- //
							//
							// -------------------------------------------------------------------------- //
							if (io_refData.isGeneratingReportInLaTeXformatRequested)
							{
								int	LminusWplusOne = io_refData.p_bzInputS->length(blitz::firstDim) - t + 1;
								double	P_W = (double)numerator / boost::math::binomial_coefficient<double>(LminusWplusOne, 2);
								ssFragmentForLaTeXPmax << L"(";
								ssFragmentForLaTeXPmax << std::setw(4) << t;
								ssFragmentForLaTeXPmax << L", " << std::setw(8) << std::pow(P_W, 1.0 / (static_cast<double>(t)));
								ssFragmentForLaTeXPmax << L")" << L"\n";
							}
							// -------------------------------------------------------------------------- //
							//
							// -------------------------------------------------------------------------- //
							int	LminusWplusOne = io_refData.p_bzInputS->length(blitz::firstDim) - t + 1;
							o_ref_bz_P_W.resizeAndPreserve(1 + t - u);
							o_ref_bz_P_W(t - u) = numerator / boost::math::binomial_coefficient<double>(LminusWplusOne, 2);
						}
					}
					// -------------------------------------------------------------------------- //
					//
					// -------------------------------------------------------------------------- //
					hg.clear();
					// -------------------------------------------------------------------------- //
					//
					// -------------------------------------------------------------------------- //
				} while (2 <= number_of_occurrences_for_most_common_t_tuple);
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				io_refData.t_6_3_6.u = u;
				io_refData.t_6_3_6.nu = (t - 1);
				// -------------------------------------------------------------------------- //
				// Compute the estimated average collision probability per string symbol as 
				// $P_{max, W} = P_{W}^{1/W}$.
				// Let $\hat{p} = \max(P_{max,u}, \ldots, P_{max,\nu})$
				// -------------------------------------------------------------------------- //
				blitz::Array<double, 1>	bz_P_max(o_ref_bz_P_W.length(blitz::firstDim));
				bz_P_max = 0.0;
				for (int j = 0; j < o_ref_bz_P_W.length(blitz::firstDim); ++j)
				{
					// -------------------------------------------------------------------------- //
					// Here, W = u + j, because offset u must be taken into account.
					// -------------------------------------------------------------------------- //
					bz_P_max(j) = pow(o_ref_bz_P_W(j), 1.0 / static_cast<double>(j + u));
				}
				io_refData.t_6_3_6.p_hat = blitz::max(bz_P_max);
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				blitz::TinyVector<int, 1>	bzMaxIndex = blitz::maxIndex(bz_P_max);
				io_refData.t_6_3_6.argmax_p_hat = bzMaxIndex(0) + u;
				// -------------------------------------------------------------------------- //
				// output LaTeX
				// -------------------------------------------------------------------------- //
				if (io_refData.isGeneratingReportInLaTeXformatRequested)
				{
					std::wstring	wstrCaptionTuple = L"Estimated $W$-tuple collision probability in Step 3 of $\\S6.3.6$ of NIST SP 800-90B";
					outputLaTeXHeaderTuple(io_refData, global_Ci_max - 1);
					if (nullptr != io_refData.p_ssLaTeXFragment)
					{
						(*io_refData.p_ssLaTeXFragment) << ssFragmentForLaTeX.rdbuf();
					}
					outputLaTeXFooter(io_refData, wstrCaptionTuple);
					outputLaTeXHeaderPmax(io_refData);
					if (nullptr != io_refData.p_ssLaTeXFragment)
					{
						(*io_refData.p_ssLaTeXFragment) << ssFragmentForLaTeXPmax.rdbuf();
					}
				}
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				return sts = ns_consts::EnmReturnStatus::Success;
			}

			// -------------------------------------------------------------------------- //
			/// <summary>
			///  Implements steps 4 to 5 of 6.3.6 of NIST SP 800-90B.
			/// </summary>
			/// <remarks>
			/// </remarks>
			/// <params="io_refData">
			/// </params>
			/// <returns>
			///  <c>entropy_estimator_lib::constants::EnmReturnStatus::Success</c>: if this function is executed successfully.
			/// </returns>
			/// <precondition>
			/// </precondition>
			/// <postcondition>
			/// </postcondition>
			// -------------------------------------------------------------------------- //
			ns_consts::EnmReturnStatus steps45(ns_dt::t_data_for_estimator& io_refData)
			{
				ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorUnexpected;

				const double& p_hat = io_refData.t_6_3_6.p_hat;
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				const double	z_alpha = calc_Z_alpha(0.995);
				double	p_u = p_hat + z_alpha * sqrt((p_hat * (1.0 - p_hat)) / static_cast<double>(io_refData.L - 1));

				if (1.0 < p_u)
				{
					p_u = 1.0;
				}
				io_refData.t_6_3_6.p_u = p_u;
				io_refData.t_6_3_6.t_common.min_entropy = -log2(p_u);
				// -------------------------------------------------------------------------- //
				// output LaTeX
				// -------------------------------------------------------------------------- //
				if (io_refData.isGeneratingReportInLaTeXformatRequested)
				{
					// -------------------------------------------------------------------------- //
					//
					// -------------------------------------------------------------------------- //
					std::wstring	wstrCaptionPmax = L"Estimated average collision probability per string symbol in Step 3 of $\\S6.3.6$ of NIST SP 800-90B";
					outputLaTeXFooterWithPhat(io_refData, wstrCaptionPmax, io_refData.t_6_3_6.argmax_p_hat);
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
			///     <li>When <c>i_refData.isModeDemonstration</c> is false, <c>i_refData.t_6_3_6.cutoff</c> is not identical to the value in NIST SP 800-90B.</li>
			///     <li>When <c>i_refData.isModeDemonstration</c> is true, <c>i_refData.t_6_3_6.cutoff</c> is not identical to the value in examples in NIST SP 800-90B 6.3.6.</li>
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
				ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorInvalidData;

				const ns_consts::EnmReturnStatus	stsCommon = ns_spt::perform_common_args_for_estimate(i_refData);
				if (ns_consts::EnmReturnStatus::Success != stsCommon)
				{
					return sts = stsCommon;
				}

				if (i_refData.L < 2)
				{
					return sts;
				}

				int		def_cutoff = 35;
				// -------------------------------------------------------------------------- //
				// for demonstrating examples in NIST SP 800-90B 6.3.x
				// -------------------------------------------------------------------------- //
				if (true == i_refData.isModeDemonstration)
				{
					def_cutoff = 3;
				}
				if (i_refData.t_6_3_6.cutoff != def_cutoff)
				{
					return sts;
				}

				return sts = ns_consts::EnmReturnStatus::Success;
			}

			// -------------------------------------------------------------------------- //
			/// <summary>
			///  Implements Longest Repeated Substring (LRS) Estimate (see 6.3.6 of NIST SP 800-90B).
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
			///     <li>When <c>io_refData.isModeDemonstration</c> is false, <c>io_refData.t_6_3_6.cutoff</c> is not identical to the value in NIST SP 800-90B.</li>
			///     <li>When <c>io_refData.isModeDemonstration</c> is true, <c>io_refData.t_6_3_6.cutoff</c> is not identical to the value in examples in NIST SP 800-90B 6.3.6.</li>
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

				blitz::Array<double, 1>	bz_P_W(128);
				bz_P_W = 0.0;
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
				// -------------------------------------------------------------------------- //
				// Step 3.
				//  For i = 1 to t, let P[i] = Q[i] / (L-i+1), and compute an estimate on the maximum individual sample value probability as P_{max}[i]=P[i]^{1/i}.
				//  Let p_{max}= max (P_{max}[1], ... , P_{max}[t]).
				// -------------------------------------------------------------------------- //
				sts = steps123(bz_P_W, io_refData);
				if (ns_consts::EnmReturnStatus::Success != sts)
				{
					return sts;
				}
				// -------------------------------------------------------------------------- //
				// Step 4.
				//  Calculate an upper bound on the probability of the most common value p_{u} as
				// -------------------------------------------------------------------------- //
				// -------------------------------------------------------------------------- //
				// Step 5.
				//  The entropy estimate is calculated as -log2(p_{u}).
				// -------------------------------------------------------------------------- //
				sts = steps45(io_refData);
				if (ns_consts::EnmReturnStatus::Success != sts)
				{
					return sts;
				}

				return sts = ns_consts::EnmReturnStatus::Success;
			}
		}
	}
}