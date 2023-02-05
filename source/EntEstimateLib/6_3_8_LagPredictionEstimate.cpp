////////////////////////////////////////////////////////////////////////////////
// 6_3_8_LagPredictionEstimate.cpp
//
//
//
// Copyright (c) 2021-2023 G. G. SAKURAI <g.garland823@gmail.com>
//
////////////////////////////////////////////////////////////////////////////////
#include "pch.h"
#include "6_3_8_LagPredictionEstimate.h"
#include "./support/checkArgs.h"
#include "./support/significantDigits.h"
#include "./math/SpecialFunctions.h"
#include "./math/Equations.h"
#include <boost/math/tools/roots.hpp>
#include <map>

namespace entropy_estimator_lib
{
	namespace estimators
	{
		namespace lag_prediction
		{
			namespace ns_consts = entropy_estimator_lib::constants;
			namespace ns_dt = entropy_estimator_lib::data_types;
			namespace ns_math = entropy_estimator_lib::estimators::math;
			namespace ns_es = entropy_estimator_lib::estimators::support;
			namespace ns_spt = entropy_estimator_lib::support;

			// -------------------------------------------------------------------------- //
			/// <summary>
			///  Output LaTeX header for
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
			ns_consts::EnmReturnStatus outputLaTeXHeader(ns_dt::t_data_for_estimator& io_refData)
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
				(*io_refData.p_ssLaTeXFragment) << L"\\subsection{Lag Prediction Estimate (NIST SP 800-90B Section 6.3.8)}" << std::endl;
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				(*io_refData.p_ssLaTeXFragment) << L"\\begin{figure}[htbp]" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"\\begin{tikzpicture}" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"\\begin{axis}[" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"	ybar," << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"	xmin=0," << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"	ymin=0," << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"	width=20cm," << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"	xlabel=length of run of ones in $correct$," << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"	ylabel=occurences" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"]" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"\\addplot+[ybar] coordinates {" << std::endl;
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				return sts = ns_consts::EnmReturnStatus::Success;
			}

			// -------------------------------------------------------------------------- //
			/// <summary>
			///  Output LaTeX Footer for
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
			ns_consts::EnmReturnStatus outputLaTeXFooter(ns_dt::t_data_for_estimator& io_refData)
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
				(*io_refData.p_ssLaTeXFragment) << L"};" << std::endl;
				// -------------------------------------------------------------------------- //
				// 
				// -------------------------------------------------------------------------- //
				(*io_refData.p_ssLaTeXFragment) << L"\\addplot+[Nigelle,no marks,sharp plot,update limits=false] " << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"coordinates {(" << io_refData.t_6_3_8.r - 1 << L", " << io_refData.t_6_3_8.occurences_at_longest_run << L") ";
				(*io_refData.p_ssLaTeXFragment) << L"(" << io_refData.t_6_3_8.r - 1 << L", "<< io_refData.t_6_3_8.occurences_at_longest_run << L")}" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"node[above left] at (axis cs:" << io_refData.t_6_3_8.r - 1 << L", ";
				(*io_refData.p_ssLaTeXFragment) << io_refData.t_6_3_8.occurences_at_longest_run << L") ";
				(*io_refData.p_ssLaTeXFragment) << L"{\\shortstack{$r - 1$ = ";
				(*io_refData.p_ssLaTeXFragment) << io_refData.t_6_3_8.r - 1 << L" " << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"\\\\($\\rightarrow$ min-entropy = " << io_refData.t_6_3_8.t_common.min_entropy;
				(*io_refData.p_ssLaTeXFragment) << L" [bit / " << io_refData.bits_per_sample << L"-bit])}};" << std::endl;
				// -------------------------------------------------------------------------- //
				// 
				// -------------------------------------------------------------------------- //
				(*io_refData.p_ssLaTeXFragment) << L"\\end{axis}" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"\\end{tikzpicture}" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"\\caption{Distribution of $correct$}" << std::endl;
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
				(*io_refData.p_ssLaTeXFragment) << L"\\caption{Supplemental information for traceability (NIST SP 800-90B Section 6.3.8)}" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"\\begin{center}" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"\\begin{tabular}{|l|c|}" << std::endl;
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				(*io_refData.p_ssLaTeXFragment) << L"\\hline " << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"\\rowcolor{rowcolorlightblue} %%" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"Symbol				& Value ";
				(*io_refData.p_ssLaTeXFragment) << L"\\\\ \\hline " << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"$N$				& " << io_refData.t_6_3_8.N;
				(*io_refData.p_ssLaTeXFragment) << L"\\\\ \\hline " << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"$C$				& " << io_refData.t_6_3_8.C;
				(*io_refData.p_ssLaTeXFragment) << L"\\\\ \\hline " << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"$P_{\\textrm{global}}$				& " << std::setw(8) << io_refData.t_6_3_8.p_global;
				(*io_refData.p_ssLaTeXFragment) << L"\\\\ \\hline " << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"$P'_{\\textrm{global}}$			& " << std::setw(8) << io_refData.t_6_3_8.p_prime_global;
				(*io_refData.p_ssLaTeXFragment) << L"\\\\ \\hline " << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"$r$				& " << io_refData.t_6_3_8.r;
				(*io_refData.p_ssLaTeXFragment) << L"\\\\ \\hline " << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"$P_{\\textrm{local}}$ 			& " << std::setw(8) << io_refData.t_6_3_8.p_local;
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
			///  Implements step 1 of 6.3.8 of NIST SP 800-90B.
			/// </summary>
			/// <remarks>
			/// </remarks>
			/// <params="o_ref_bz_lag">
			/// </params>
			/// <params="o_ref_bz_correct">
			/// </params>
			/// <params="i_refData">
			/// </params>
			/// <returns>
			/// </returns>
			/// <precondition>
			/// </precondition>
			/// <postcondition>
			/// </postcondition>
			// -------------------------------------------------------------------------- //
			ns_consts::EnmReturnStatus step1(blitz::Array<ns_dt::NullableT<ns_dt::octet>, 1>& o_ref_bz_lag,
				blitz::Array<ns_dt::octet, 1>& o_ref_bz_correct,
				const ns_dt::t_data_for_estimator& i_refData)
			{
				ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorUnexpected;

				int		def_D = 128;
				// -------------------------------------------------------------------------- //
				// for demonstrating examples in NIST SP 800-90B 6.3.x
				// -------------------------------------------------------------------------- //
				if (true == i_refData.isModeDemonstration)
				{
					def_D = 3;
				}
				if (i_refData.t_6_3_8.D != def_D)
				{
					return	sts = ns_consts::EnmReturnStatus::ErrorInvalidData;
				}
				ns_dt::NullableT<ns_dt::octet>	nv = { true, 0 };
				o_ref_bz_lag.resize(i_refData.t_6_3_8.D);
				o_ref_bz_lag = nv;

				int		N = i_refData.p_bzInputS->length(blitz::firstDim) - 1;
				o_ref_bz_correct.resize(N);
				o_ref_bz_correct = 0;

				return sts = ns_consts::EnmReturnStatus::Success;
			}


			// -------------------------------------------------------------------------- //
			/// <summary>
			///  Implements step 3 of 6.3.8 of NIST SP 800-90B.
			/// </summary>
			/// <remarks>
			/// </remarks>
			/// <params="io_ref_bz_lag">
			/// </params>
			/// <params="io_ref_bz_correct">
			/// </params>
			/// <params="io_ref_bz_scoreboard">
			/// </params>
			/// <params="io_ref_winner">
			/// </params>
			/// <params="i_refData">
			/// </params>
			/// <returns>
			/// </returns>
			/// <precondition>
			/// </precondition>
			/// <postcondition>
			/// </postcondition>
			// -------------------------------------------------------------------------- //
			ns_consts::EnmReturnStatus step3(blitz::Array<ns_dt::NullableT<ns_dt::octet>, 1>& io_ref_bz_lag,
				blitz::Array<ns_dt::octet, 1>& io_ref_bz_correct,
				blitz::Array<int, 1>& io_ref_bz_scoreboard,
				int& io_ref_winner,
				ns_dt::t_data_for_estimator& i_refData)
			{
				ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorUnexpected;

				// -------------------------------------------------------------------------- //
				// 3. For i = 2 to L:
				// -------------------------------------------------------------------------- //
				for (int i = 1; i < i_refData.p_bzInputS->length(blitz::firstDim); ++i)
				{
					// -------------------------------------------------------------------------- //
					// a. For d = 1 to D
					// -------------------------------------------------------------------------- //
					for (int d = 0; d < i_refData.t_6_3_8.D; ++d)
					{
						if (d < i)
						{
							// -------------------------------------------------------------------------- //
							// i. If (d < i), lag_{d} = s_{i - d},
							// -------------------------------------------------------------------------- //
							ns_dt::NullableT<ns_dt::octet>	val = { false, (*(i_refData.p_bzInputS))(i - d - 1) };
							io_ref_bz_lag(d) = val;
						}
						else
						{
							// -------------------------------------------------------------------------- //
							// ii. Else lag_{d} = Null;
							// -------------------------------------------------------------------------- //
							io_ref_bz_lag(d) = { true, 0 };
						}
					}
					// -------------------------------------------------------------------------- //
					// b. Let prediction = frequent_winner.
					// -------------------------------------------------------------------------- //
					ns_dt::NullableT<ns_dt::octet>	prediction = io_ref_bz_lag(io_ref_winner - 1);
					// -------------------------------------------------------------------------- //
					// c. If (prediction = s_{i}), let correct_{i - 1} = 1.
					// -------------------------------------------------------------------------- //
					if (false == prediction.bIsValueNull)
					{
						if (prediction.value == (*(i_refData.p_bzInputS))(i))
						{
							io_ref_bz_correct(i - 1) = 1;
						}
					}
					// -------------------------------------------------------------------------- //
					// for demonstrating examples in NIST SP 800-90B 6.3.x
					// -------------------------------------------------------------------------- //
					if (true == i_refData.isModeDemonstration)
					{
						std::cout << "// ------------------------- //" << std::endl;
						std::cout << "i:\t\t\t" << i + 1 << std::endl;
						std::cout << "lag:\t\t\t" << io_ref_bz_lag << std::endl;
						std::cout << "winner:\t\t\t" << io_ref_winner << std::endl;
						std::cout << "prediction:\t\t" << (int)prediction.value << std::endl;
						std::cout << "correct_{i - 1}:\t" << (int)io_ref_bz_correct(i - 1) << std::endl;
					}
					// -------------------------------------------------------------------------- //
					// d. Update the scoreboard. 
					//  For d = 1 to D,
					// -------------------------------------------------------------------------- //
					for (int d = 0; d < i_refData.t_6_3_8.D; ++d)
					{
						// -------------------------------------------------------------------------- //
						// i. If (lag_{d} = s_{i})
						// -------------------------------------------------------------------------- //
						if (false == io_ref_bz_lag(d).bIsValueNull)
						{
							if (io_ref_bz_lag(d).value == (*(i_refData.p_bzInputS))(i))
							{
								// -------------------------------------------------------------------------- //
								// 1. Let scoreboard_{d} = scoreboard_{d} + 1
								// -------------------------------------------------------------------------- //
								io_ref_bz_scoreboard(d) = io_ref_bz_scoreboard(d) + 1;
								// -------------------------------------------------------------------------- //
								// 2. If scorebaord_{d} >= scoreboard_{winner}, let winner = d
								// -------------------------------------------------------------------------- //
								if (io_ref_bz_scoreboard(io_ref_winner - 1) <= io_ref_bz_scoreboard(d))
								{
									io_ref_winner = d + 1;
								}
							}
						}
					}
					// -------------------------------------------------------------------------- //
					// for demonstrating examples in NIST SP 800-90B 6.3.x
					// -------------------------------------------------------------------------- //
					if (true == i_refData.isModeDemonstration)
					{
						std::cout << "scoreboard (step 3d):\t" << io_ref_bz_scoreboard << std::endl;
					}
				}

				return sts = ns_consts::EnmReturnStatus::Success;
			}

			// -------------------------------------------------------------------------- //
			/// <summary>
			///  Implements step 5 of 6.3.8 of NIST SP 800-90B.
			/// </summary>
			/// <remarks>
			/// </remarks>
			/// <params="o_refC">
			/// </params>
			/// <params="o_ref_bz_correct">
			/// </params>
			/// <returns>
			/// </returns>
			/// <precondition>
			/// </precondition>
			/// <postcondition>
			/// </postcondition>
			// -------------------------------------------------------------------------- //
			ns_consts::EnmReturnStatus step4(int& o_refC, blitz::Array<ns_dt::octet, 1>& io_ref_bz_correct)
			{
				ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorUnexpected;

				o_refC = blitz::count(io_ref_bz_correct == (ns_dt::octet)1);

				return sts = ns_consts::EnmReturnStatus::Success;
			}

			// -------------------------------------------------------------------------- //
			/// <summary>
			///  Implements step 5 of 6.3.8 of NIST SP 800-90B.
			/// </summary>
			/// <remarks>
			/// </remarks>
			/// <params="o_ref_p_prime_global">
			/// </params>
			/// <params="i_refC">
			/// </params>
			/// <params="i_refN">
			/// </params>
			/// <returns>
			/// </returns>
			/// <precondition>
			/// </precondition>
			/// <postcondition>
			/// </postcondition>
			// -------------------------------------------------------------------------- //
			ns_consts::EnmReturnStatus step5(double& o_ref_p_prime_global,
				double& o_ref_p_global,
				const int& i_refC, const int& i_refN)
			{
				ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorUnexpected;

				o_ref_p_global = (double)i_refC / (double)i_refN;

				if (0 == i_refC)
				{
					o_ref_p_prime_global = 1.0 - pow(0.01, 1.0 / (double)i_refN);
				}
				else
				{
					double z_alpha = calc_Z_alpha(0.995);
					o_ref_p_prime_global = o_ref_p_global + z_alpha * sqrt(o_ref_p_global * (1.0 - o_ref_p_global) / (double)(i_refN - 1));
					if (1.0 < o_ref_p_prime_global)
					{
						o_ref_p_prime_global = 1.0;
					}
				}
				return sts = ns_consts::EnmReturnStatus::Success;
			}

			// -------------------------------------------------------------------------- //
			/// <summary>
			///  Implements step 6 of 6.3.8 of NIST SP 800-90B.
			/// </summary>
			/// <remarks>
			/// </remarks>
			/// <params="o_ref_r">
			/// </params>
			/// <params="o_ref_p_local">
			/// </params>
			/// <params="i_ref_bz_correct">
			/// </params>
			/// <params="i_refN">
			/// </params>
			/// <params="o_ref_occurences_at_logest_run">
			/// </params>
			/// <params="o_refSSFragmentForLaTeX">
			/// </params>
			/// <params="i_bIsGeneratingReportInLaTeXformatRequested">
			/// </params>
			/// <returns>
			/// </returns>
			/// <precondition>
			/// </precondition>
			/// <postcondition>
			/// </postcondition>
			// -------------------------------------------------------------------------- //
			ns_consts::EnmReturnStatus step6(int& o_ref_r,
				double& o_ref_p_local,
				const blitz::Array<ns_dt::octet, 1>& i_ref_bz_correct,
				int& i_refN,
				int& o_ref_occurences_at_logest_run,
				std::wstringstream& o_refSSFragmentForLaTeX,
				bool i_bIsGeneratingReportInLaTeXformatRequested)
			{
				ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorUnexpected;
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				int	r_current = 0;
				int	r_prev = 0;
				// -------------------------------------------------------------------------- //
				// map for LaTeX output
				// -------------------------------------------------------------------------- //
				std::map<int, int>	mp_correct;
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				for (int m = 0; m < i_ref_bz_correct.length(blitz::firstDim); ++m)
				{
					if (i_ref_bz_correct(m) == 0)
					{
						if (r_prev < r_current)
						{
							r_prev = r_current;
							r_current = 0;
							// -------------------------------------------------------------------------- //
							// 
							// -------------------------------------------------------------------------- //
							std::map<int, int>::iterator	mit = mp_correct.find(r_prev);
							if (mit == mp_correct.cend())
							{
								mp_correct.insert(std::make_pair(r_prev, 1));
							}
							else
							{
								int count = mit->second;
								mit->second = (count + 1);
							}
						}
					}
					else
					{
						if (0 == m)
						{
							r_current = 1;
						}
						else if (i_ref_bz_correct(m - 1) == 0)
						{
							r_current = 1;
						}
						else if (i_ref_bz_correct(m - 1) == 1)
						{
							++r_current;
						}
					}
				}
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				if (r_prev < r_current)
				{
					o_ref_r = r_current;
				}
				else
				{
					o_ref_r = r_prev;
				}
				// -------------------------------------------------------------------------- //
				// 
				// -------------------------------------------------------------------------- //
				std::map<int, int>::iterator	mit = mp_correct.find(r_current);
				if (mit == mp_correct.cend())
				{
					mp_correct.insert(std::make_pair(r_current, 1));
				}
				else
				{
					int count = mit->second;
					mit->second = (count + 1);
				}
				// -------------------------------------------------------------------------- //
				// output LaTeX
				// -------------------------------------------------------------------------- //
				if (i_bIsGeneratingReportInLaTeXformatRequested)
				{
					for (std::map<int, int>::const_iterator cit = mp_correct.cbegin(); cit != mp_correct.cend(); cit++)
					{
						o_refSSFragmentForLaTeX << L"(";
						o_refSSFragmentForLaTeX << std::setw(8) << cit->first << L"," << std::setw(8) << cit->second;
						o_refSSFragmentForLaTeX << L")" << std::endl;

						if (cit->first == o_ref_r)
						{
							o_ref_occurences_at_logest_run = cit->second;
						}
					}
				}
				// -------------------------------------------------------------------------- //
				++o_ref_r;
				// -------------------------------------------------------------------------- //
				ns_math::RhsMinusLhs	fnc(o_ref_r, i_refN, 0.99);
				double	lowerbound = pow(0.5, 20.0);
				double	upperbound = 1.0 - pow(0.5, 20.0);
				boost::math::tools::eps_tolerance<double> tol(std::numeric_limits<double>::digits - 4);
				boost::uintmax_t it = 32;
				std::pair<double, double>	rg = boost::math::tools::toms748_solve(fnc, lowerbound, upperbound, tol, it);
				// -------------------------------------------------------------------------- //
				// 
				// -------------------------------------------------------------------------- //
				o_ref_p_local = rg.first;
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

				ns_consts::EnmReturnStatus	stsCommon = ns_spt::perform_common_args_for_estimate(i_refData);
				if (ns_consts::EnmReturnStatus::Success != stsCommon)
				{
					return sts = stsCommon;
				}
				int		def_D = 128;
				// -------------------------------------------------------------------------- //
				// for demonstrating examples in NIST SP 800-90B 6.3.x
				// -------------------------------------------------------------------------- //
				if (true == i_refData.isModeDemonstration)
				{
					def_D = 3;
				}
				if (i_refData.t_6_3_8.D != def_D)
				{
					sts = ns_consts::EnmReturnStatus::ErrorInvalidData;
					return sts;
				}

				return sts = ns_consts::EnmReturnStatus::Success;
			}

			// -------------------------------------------------------------------------- //
			/// <summary>
			///  Implements The Lag Prediction Estimate (see 6.3.8 of NIST SP 800-90B).
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

				int		N = io_refData.p_bzInputS->length(blitz::firstDim) - 1;
				blitz::Array<ns_dt::octet, 1>	bz_correct(N);
				bz_correct = 0;

				ns_dt::NullableT<ns_dt::octet>	nv = { true, 0 };
				blitz::Array<ns_dt::NullableT<ns_dt::octet>, 1>	bz_lag(io_refData.t_6_3_8.D);
				bz_lag = nv;

				// -------------------------------------------------------------------------- //
				// Step 1.
				//  Let D = 128, and N = L - 1. Let lag be a list of D values, each initialized to Null.
				//  Let correct be a list of N Boolean values, each initialized to 0.
				// -------------------------------------------------------------------------- //
				sts = step1(bz_lag, bz_correct, io_refData);
				if (ns_consts::EnmReturnStatus::Success != sts)
				{
					return sts;
				}
				// -------------------------------------------------------------------------- //
				// Step 2.
				//  Let scoreboard be a list of D counters, each initialized to 0. 
				//  Let winner = 1.
				// -------------------------------------------------------------------------- //
				blitz::Array<int, 1>	bz_scoreboard(io_refData.t_6_3_8.D);
				bz_scoreboard = 0;

				int		winner = 1;
				// -------------------------------------------------------------------------- //
				// Step 3.
				//  For i = 2 to L:
				// -------------------------------------------------------------------------- //
				sts = step3(bz_lag, bz_correct, bz_scoreboard, winner, io_refData);
				if (ns_consts::EnmReturnStatus::Success != sts)
				{
					return sts;
				}
				// -------------------------------------------------------------------------- //
				// Step 4.
				//  Let C be the number of ones in correct.
				// -------------------------------------------------------------------------- //
				io_refData.t_6_3_8.C = 0;
				sts = step4(io_refData.t_6_3_8.C, bz_correct);
				if (ns_consts::EnmReturnStatus::Success != sts)
				{
					return sts;
				}
				// -------------------------------------------------------------------------- //
				// Step 5.
				//  Calculate the predictor' s global performance as P_{global} = C / N.
				//  The upper bound of the 99% confidence interval on P_{global}, denoted P'_{gloabal} is calculated as:
				// -------------------------------------------------------------------------- //
				io_refData.t_6_3_8.p_global = 0;
				io_refData.t_6_3_8.p_prime_global = 0.0;
				sts = step5(io_refData.t_6_3_8.p_prime_global, io_refData.t_6_3_8.p_global, io_refData.t_6_3_8.C, N);
				if (ns_consts::EnmReturnStatus::Success != sts)
				{
					return sts;
				}
				// -------------------------------------------------------------------------- //
				// 
				// -------------------------------------------------------------------------- //
				std::wstringstream	ssFragmentForLaTeX;
				// -------------------------------------------------------------------------- //
				// Step 6.
				// -------------------------------------------------------------------------- //
				io_refData.t_6_3_8.p_local = 0.0;
				io_refData.t_6_3_8.r = 0;
				io_refData.t_6_3_8.occurences_at_longest_run = 0;
				sts = step6(io_refData.t_6_3_8.r, io_refData.t_6_3_8.p_local, bz_correct, N, 
					io_refData.t_6_3_8.occurences_at_longest_run, 
					ssFragmentForLaTeX, io_refData.isGeneratingReportInLaTeXformatRequested);
				if (ns_consts::EnmReturnStatus::Success != sts)
				{
					return sts;
				}
				// -------------------------------------------------------------------------- //
				// Step 7.
				// -------------------------------------------------------------------------- //
				double	one_over_k = 1.0 / (double)io_refData.k;
				// -------------------------------------------------------------------------- //
				// 
				// -------------------------------------------------------------------------- //
				double	min_entropy_lower_bound = 0.0;
				double	min_entropy_upper_bound = 0.0;
				double	number_of_significant_digits = 1.0;
				// -------------------------------------------------------------------------- //
				// 
				// -------------------------------------------------------------------------- //
				if (io_refData.t_6_3_8.p_prime_global < io_refData.t_6_3_8.p_local)
				{
					if (io_refData.t_6_3_8.p_local < one_over_k)
					{
						io_refData.t_6_3_8.t_common.min_entropy = -log2(one_over_k);
					}
					else
					{
						// -------------------------------------------------------------------------- //
						// 
						// -------------------------------------------------------------------------- //
						io_refData.t_6_3_8.t_common.min_entropy = -log2(io_refData.t_6_3_8.p_local);
						// -------------------------------------------------------------------------- //
						// evaluate number of significant digits based on p_local
						// -------------------------------------------------------------------------- //
						ns_consts::EnmReturnStatus	stsNumSignificantDigits = ns_es::evaluateSignificantDigitsOfMinEntropyByPlocalSP800_90B_6_3_7_6_3_10(
							min_entropy_lower_bound, min_entropy_upper_bound,
							number_of_significant_digits, io_refData.t_6_3_8.r, N, io_refData.k);
						if (ns_consts::EnmReturnStatus::Success == stsNumSignificantDigits)
						{
							io_refData.t_6_3_8.t_common.number_of_significant_digits = number_of_significant_digits;
							io_refData.t_6_3_8.t_common.min_entropy_lower_bound = min_entropy_lower_bound;
							io_refData.t_6_3_8.t_common.min_entropy_upper_bound = min_entropy_upper_bound;
						}
					}
				}
				else
				{
					if (io_refData.t_6_3_8.p_prime_global < one_over_k)
					{
						io_refData.t_6_3_8.t_common.min_entropy = -log2(one_over_k);
					}
					else
					{
						// -------------------------------------------------------------------------- //
						// 
						// -------------------------------------------------------------------------- //
						io_refData.t_6_3_8.t_common.min_entropy = -log2(io_refData.t_6_3_8.p_prime_global);
						// -------------------------------------------------------------------------- //
						// evaluate number of significant digits based on p_prime_global
						// -------------------------------------------------------------------------- //
						ns_consts::EnmReturnStatus	stsNumSignificantDigits = ns_es::evaluateSignificantDigitsOfMinEntropyByPglobalSP800_90B_6_3_7_6_3_10(
							min_entropy_lower_bound, min_entropy_upper_bound,
							number_of_significant_digits, io_refData.t_6_3_8.C, N);
						if (ns_consts::EnmReturnStatus::Success == stsNumSignificantDigits)
						{
							io_refData.t_6_3_8.t_common.number_of_significant_digits = number_of_significant_digits;
							io_refData.t_6_3_8.t_common.min_entropy_lower_bound = min_entropy_lower_bound;
							io_refData.t_6_3_8.t_common.min_entropy_upper_bound = min_entropy_upper_bound;
						}
					}
				}
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				io_refData.t_6_3_8.N = N;
				// -------------------------------------------------------------------------- //
				// output LaTeX
				// -------------------------------------------------------------------------- //
				if (io_refData.isGeneratingReportInLaTeXformatRequested)
				{
					outputLaTeXHeader(io_refData);
					if (nullptr != io_refData.p_ssLaTeXFragment)
					{
						(*io_refData.p_ssLaTeXFragment) << ssFragmentForLaTeX.rdbuf();
					}
					outputLaTeXFooter(io_refData);
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