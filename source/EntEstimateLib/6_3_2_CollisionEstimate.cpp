////////////////////////////////////////////////////////////////////////////////
// 6_3_2_CollisionEstimate.cpp
//
//
//
// Copyright (c) 2021-2023 G. G. SAKURAI <g.garland823@gmail.com>
//
////////////////////////////////////////////////////////////////////////////////
#include "pch.h"
#include "6_3_2_CollisionEstimate.h"
#include "./support/checkArgs.h"
#include "./support/significantDigits.h"
#include <cmath>
#include "./math/SpecialFunctions.h"
#include <boost/math/special_functions.hpp>
#include <map>

namespace entropy_estimator_lib
{
	namespace estimators
	{
		namespace collision
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
			/// <params="i_ref_bz_t_nu">
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
				blitz::Array<int, 1>& i_ref_bz_t_nu)
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
				(*io_refData.p_ssLaTeXFragment) << L"\\subsection{The Collision Estimate (NIST SP 800-90B Section 6.3.2)}";
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				(*io_refData.p_ssLaTeXFragment) << L"\\label{sec:Binary632}" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"\n";
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				(*io_refData.p_ssLaTeXFragment) << L"\\begin{figure}[htbp]" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"\\centering" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"\n";
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				(*io_refData.p_ssLaTeXFragment) << L"\\begin{tikzpicture}" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"\\begin{axis}[" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"	ybar," << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"	xmin=0," << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"	xtick={0, 1, 2, 3}," << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"	ymin=0," << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"	width=20cm," << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"	xlabel=$t_{\\nu}$," << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"	ylabel=occurrences of $t_{\\nu}$" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"]" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"\\addplot+[ybar] coordinates {" << L"\n";
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				std::map<int, int>	mp_t_nu_counts;
				for (int i = 0; i < i_ref_bz_t_nu.length(blitz::firstDim); ++i)
				{
					auto	mit = mp_t_nu_counts.find(i_ref_bz_t_nu(i));
					if (mit == mp_t_nu_counts.end())
					{
						mp_t_nu_counts.insert(std::make_pair(i_ref_bz_t_nu(i), 1));
					}
					else
					{
						const int count = mit->second;
						mit->second = (count + 1);
					}
				}
				int max_t_nu = 1;
				for (const auto& e : mp_t_nu_counts)
				{
					if (max_t_nu < (e.second))
					{
						max_t_nu = (e.second);
					}
					(*io_refData.p_ssLaTeXFragment) << L"(";
					(*io_refData.p_ssLaTeXFragment) << std::setw(8) << (e.first);
					(*io_refData.p_ssLaTeXFragment) << L", " << std::setw(8) << (e.second);
					(*io_refData.p_ssLaTeXFragment) << L")" << L"\n";
				}
				(*io_refData.p_ssLaTeXFragment) << L"};" << L"\n";
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				(*io_refData.p_ssLaTeXFragment) << L"\\addplot+[Nigelle,no marks,sharp plot,update limits=false] " << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"coordinates {(" << io_refData.t_6_3_2.x_bar_prime << L"," << max_t_nu << L") (" << io_refData.t_6_3_2.x_bar_prime << L",1)}" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"node[above right] at (axis cs:" << io_refData.t_6_3_2.x_bar_prime << L"," << max_t_nu;
				(*io_refData.p_ssLaTeXFragment) << L") {$\\bar{X}'$ = " << io_refData.t_6_3_2.x_bar_prime << L"};" << L"\n";
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				(*io_refData.p_ssLaTeXFragment) << L"\\end{axis}" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"\\end{tikzpicture}" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"\\caption{Distribution of intermediate value $t_{\\nu}$}" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"\\end{figure}" << L"\n";
				// -------------------------------------------------------------------------- //
				//
				// plot solution to the equation in step 7
				//
				// -------------------------------------------------------------------------- //
				(*io_refData.p_ssLaTeXFragment) << L"\\begin{figure}[htbp]" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"\\centering" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"\n";
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				(*io_refData.p_ssLaTeXFragment) << L"\\begin{tikzpicture}[scale=12]" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"\\draw[very thin,color=gray,dotted] (0,2) grid[step=0.25] (1,3);" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"\\draw[->] (0, 2) -- (1.1,2) node[right] {$p$};" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"\\draw[->] (0, 1.95) -- (0,3.05) node[above] {\\shortstack{RHS of equation in step 7 \\\\$\\equiv g(p)$}};" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"\\draw[domain=0.5:1, smooth, variable=\\x, color=blue] plot (\\x,{2*(\\x*(1-\\x)+1)}) node[above right, xshift = 2mm, yshift = 2mm] {$g(p) = 2 \\left[ p (1 - p) + 1 \\right] $};" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"\\draw[gray,loosely dotted] (  0.5,2.5) -- ( 0.0,2.5);" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"\\draw[gray,loosely dotted] (  0.5,2.5) -- ( 0.5,2);" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"\\draw (-0.1,  3) node {3} ;" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"\\draw (-0.1,  2) node {2} ;" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"\\draw (-0.1,  2.5) node {$\\frac{5}{2}$} ;" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"\\draw ( 0  ,  1.9) node {0} ;" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"\\draw ( 0.5,  1.9) node {$\\frac{1}{2}$} ;" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"\\draw ( 1.0,  1.9) node {1} ;" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"%" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"%" << L"\n";
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				if (io_refData.t_6_3_2.bIsRootFound)
				{
					(*io_refData.p_ssLaTeXFragment) << L"\\draw[Nigelle,dashed] ( 0, " << io_refData.t_6_3_2.x_bar_prime << L") ";
					(*io_refData.p_ssLaTeXFragment) << L"--(" << io_refData.t_6_3_2.p << L", " << io_refData.t_6_3_2.x_bar_prime << L"); " << L"\n";
					(*io_refData.p_ssLaTeXFragment) << L"\\draw[Nigelle,dashed] ( " << io_refData.t_6_3_2.p << L", 2) ";
					(*io_refData.p_ssLaTeXFragment) << L"--(" << io_refData.t_6_3_2.p << L", " << io_refData.t_6_3_2.x_bar_prime << L"); " << L"\n";
					(*io_refData.p_ssLaTeXFragment) << L"\\draw (" << io_refData.t_6_3_2.p << L", 2) node[below]{ "; 
					(*io_refData.p_ssLaTeXFragment) << L"\\textcolor{Nigelle}{ ";
					(*io_refData.p_ssLaTeXFragment) << L"\\shortstack{ ";	// start of shortstack
					(*io_refData.p_ssLaTeXFragment) << io_refData.t_6_3_2.p;
					(*io_refData.p_ssLaTeXFragment) << L" \\\\ " << L"\n";	// newline for shortstack
					(*io_refData.p_ssLaTeXFragment) << L"($\\rightarrow$ min-entropy = " << io_refData.t_6_3_2.t_common.min_entropy;
					(*io_refData.p_ssLaTeXFragment) << L" [bit / " << io_refData.bits_per_sample << L"-bit]) " << L"\n";
					(*io_refData.p_ssLaTeXFragment) << L"} "; // end of shortstack
					(*io_refData.p_ssLaTeXFragment) << L"} }; " << L"\n";
				}
				else
				{
					(*io_refData.p_ssLaTeXFragment) << L"\\draw[Nigelle,dashed] ( 0, " << io_refData.t_6_3_2.x_bar_prime << L") ";
					(*io_refData.p_ssLaTeXFragment) << L"--( 1, " << io_refData.t_6_3_2.x_bar_prime << L"); " << L"\n";
				}
				(*io_refData.p_ssLaTeXFragment) << L"\\draw (0.125, " << io_refData.t_6_3_2.x_bar_prime << L") ";
				if (io_refData.t_6_3_2.x_bar_prime < 2.5)
				{
					(*io_refData.p_ssLaTeXFragment) << L"node[below]{ ";
				}
				else
				{
					(*io_refData.p_ssLaTeXFragment) << L"node[above]{ ";
				}
				(*io_refData.p_ssLaTeXFragment) << L"\\textcolor{Nigelle}{ ";	// start of textcolor
				(*io_refData.p_ssLaTeXFragment) << L"$\\bar{X}' = " << io_refData.t_6_3_2.x_bar_prime << L"$";
				(*io_refData.p_ssLaTeXFragment) << L"}  " << L"\n";	// end of textcolor
				(*io_refData.p_ssLaTeXFragment) << L"}; " << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"%" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"%" << L"\n";
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				(*io_refData.p_ssLaTeXFragment) << L"\\end{tikzpicture}" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"\\caption{Solution to the equation in step 7}" << L"\n";
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
				(*io_refData.p_ssLaTeXFragment) << L"\\caption{Supplemental information for traceability (NIST SP 800-90B Section 6.3.2)}" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"\\begin{center}" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"\\begin{tabular}{|l|c|}" << L"\n";
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				(*io_refData.p_ssLaTeXFragment) << L"\\hline " << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"\\rowcolor{anotherlightblue} %%" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"Symbol				& Value ";
				(*io_refData.p_ssLaTeXFragment) << L"\\\\ \\hline " << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"$p$				& " << std::setw(8) << io_refData.t_6_3_2.p;
				(*io_refData.p_ssLaTeXFragment) << L"\\\\ \\hline " << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"$\\bar{X}$ 		& " << std::setw(8) << io_refData.t_6_3_2.x_bar;
				(*io_refData.p_ssLaTeXFragment) << L"\\\\ \\hline" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"$\\bar{X}'$		& " << std::setw(8) << io_refData.t_6_3_2.x_bar_prime;
				(*io_refData.p_ssLaTeXFragment) << L"\\\\ \\hline" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"$\\hat{\\sigma}$		& " << std::setw(8) << io_refData.t_6_3_2.sigma_hat;
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
			///     <li><c>i_refData.L</c> &lt; 2</li>
			///     <li><c>i_refData.k</c> &lt; 2</li>
			///     <li><c>i_refData.k</c> &ne; 2</li>
			///     <li><c>i_refData.k</c> &gt; INT_MAX</li>
			///     <li><c>i_refData.k</c> is not equal to <c>i_refData.p_bzSampleSpaceA-&gt;length(blitz::firstDim)</c>.</li>
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
				if (i_refData.k != 2)
				{
					return sts;
				}
				if (i_refData.L < 2)
				{
					return sts;
				}

				return sts = ns_consts::EnmReturnStatus::Success;
			}

			// -------------------------------------------------------------------------- //
			/// <summary>
			///  Implements The Collision Estimate (see 6.3.2 of NIST SP 800-90B).
			/// </summary>
			/// <remarks>
			/// </remarks>
			/// <params="io_refData">
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
			///     <li><c>i_refData.L</c> &lt; 2</li>
			///     <li><c>i_refData.k</c> &lt; 2</li>
			///     <li><c>i_refData.k</c> &ne; 2</li>
			///     <li><c>i_refData.k</c> &gt; INT_MAX</li>
			///     <li><c>i_refData.k</c> is not equal to <c>i_refData.p_bzSampleSpaceA-&gt;length(blitz::firstDim)</c>.</li>
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

				// Calc $Z_{\alpha}$ value (=2.5758293035489008)
				const long double z_alpha = calc_Z_alpha(0.995);
				uintmax_t nu_max = io_refData.L / 2;
				if (1 == io_refData.L % 2)
				{
					nu_max = (io_refData.L + 1) / 2;
				}

				blitz::Array<int, 1>	bz_t_nu(nu_max);
				bz_t_nu = 0;
				// -------------------------------------------------------------------------- //
				// Step 1.
				//  Set nu = 0, index = 1.
				// -------------------------------------------------------------------------- //
				int nu = 0;
				int index = 0;
				bool	bIsEndReached = false;
				while ((false == bIsEndReached)
					&& (index < (io_refData.p_bzInputS->length(blitz::firstDim) - 1)))
				{
					// -------------------------------------------------------------------------- //
					// Step 2.
					//  Beginning with s_index, step through the input until any observed value is repeated;
					//  i.e., find the smallest j such that s_i = s_j, for some i with index <= i < j.
					// -------------------------------------------------------------------------- //
					bool	bIsCollisionFound = false;
					for (int j = index + 1; (j <= (index + 2))
						&& (false == bIsCollisionFound)
						&& (j <= (io_refData.p_bzInputS->length(blitz::firstDim) - 1)); ++j)
					{
						for (int i = index; i < j; ++i)
						{
							if (j == io_refData.p_bzInputS->length(blitz::firstDim) - 1)
							{
								if (i == (j - 1))
								{
									bIsEndReached = true;
								}
							}
							if ((*io_refData.p_bzInputS)(i) == (*io_refData.p_bzInputS)(j))
							{
								// -------------------------------------------------------------------------- //
								// Step 3.
								//  Set nu = nu + 1, t_nu = j - index + 1 and index = j + 1.
								// -------------------------------------------------------------------------- //
								bz_t_nu(nu) = j - index + 1;
								index = j + 1;
								++nu;
								bIsCollisionFound = true;
								break;
							}
						}
					}
					// -------------------------------------------------------------------------- //
					// Step 4.
					//  Repeat steps 2-3 until the end of the dataset is reached.
					// -------------------------------------------------------------------------- //
				}

				bz_t_nu.resizeAndPreserve(nu);
				// -------------------------------------------------------------------------- //
				// Step 5.
				//  Calculate the sample mean \bar{X}, and the sample standard deviation \hat{\sigma}, of t_i as
				//  
				// -------------------------------------------------------------------------- //
				const long double	x_bar = blitz::mean(bz_t_nu);
				blitz::Array<long double, 1>		bz_t_nu_square(bz_t_nu.extent());
				bz_t_nu_square = bz_t_nu;	// type promotion
				bz_t_nu_square -= x_bar;	// subtract mean
				const long double	sigma_hat = sqrt(blitz::sum(blitz::pow2(bz_t_nu_square)) / static_cast<double>(nu - 1));
				// -------------------------------------------------------------------------- //
				// Step 6.
				//  Compute the lower-bound of the confidence interval for the mean,
				//  based on a normal distribution with a confidence level of 99 %,
				// -------------------------------------------------------------------------- //
				const long double	x_bar_dash = x_bar - z_alpha * sigma_hat / sqrt(static_cast<long double>(nu));
				io_refData.t_6_3_2.x_bar = x_bar;
				io_refData.t_6_3_2.x_bar_prime = x_bar_dash;
				io_refData.t_6_3_2.sigma_hat = sigma_hat;
				// -------------------------------------------------------------------------- //
				// Step 7.
				//  Using a binary search, solve for the parameter p, such that
				//
				//  where
				//    q = 1 - p,
				//    p >= q,
				//    F(1/z) = \Gamma(3, z)z^{-3}\exp(z)
				// -------------------------------------------------------------------------- //
				if ((x_bar_dash < 2.0) || (2.5 < x_bar_dash))
				{
					// -------------------------------------------------------------------------- //
					// The r.h.s. of the equation to be solved, is monotonically decreasing function, 
					// and its maximum is 2.5 at p = 0.5, its minimum is 2.0 at p = 1.0.
					// Therefore if the l.h.s of the equation is greater than 2.5, there will be no solution.
					// Here, we set min-entropy = 1.0 when <c>x_bar_dash</c> is greater than 2.5
					// -------------------------------------------------------------------------- //
					io_refData.t_6_3_2.p = 0.5;
					io_refData.t_6_3_2.t_common.min_entropy = 1.0;
					io_refData.t_6_3_2.bIsRootFound = false;
					io_refData.t_6_3_2.t_common.number_of_significant_digits = 1.0;
					io_refData.t_6_3_2.t_common.min_entropy_lower_bound = 1.0;
					io_refData.t_6_3_2.t_common.min_entropy_upper_bound = 1.0;
				}
				else
				{
					// -------------------------------------------------------------------------- //
					// Step 8.
					//  If the binary search yields a solution, then the min-entropy estimation is the negative logarithm of the parameter, p:
					//    min-entropy = -log2(p).
					//	If the search does not yield a solution, then the min - entropy estimation is :
					//    min-entropy = log2(2) = 1.
					// -------------------------------------------------------------------------- //
					// -------------------------------------------------------------------------- //
					// Here the solution p is expressed by the following equation:
					//  p = \frac{1}{2} + \sqrt{\frac{5}{4} - \frac{\bar{X}'}{2}}.
					// -------------------------------------------------------------------------- //
					io_refData.t_6_3_2.p = 0.5 + sqrt(1.25 - 0.5 * x_bar_dash);
					io_refData.t_6_3_2.t_common.min_entropy = -log2(io_refData.t_6_3_2.p);
					io_refData.t_6_3_2.bIsRootFound = true;
					// -------------------------------------------------------------------------- //
					// 
					// -------------------------------------------------------------------------- //
					double	min_entropy_lower_bound = 0.0;
					double	min_entropy_upper_bound = 0.0;
					double	number_of_significant_digits = 1.0;
					// -------------------------------------------------------------------------- //
					// evaluate number of significant digits based on p_prime_global
					// -------------------------------------------------------------------------- //
					const ns_consts::EnmReturnStatus	stsNumSignificantDigits = ns_es::evaluateSignificantDigitsSP800_90B_6_3_2(
						min_entropy_lower_bound, min_entropy_upper_bound,
						number_of_significant_digits, x_bar_dash, nu);
					if (ns_consts::EnmReturnStatus::Success == stsNumSignificantDigits)
					{
						io_refData.t_6_3_2.t_common.number_of_significant_digits = number_of_significant_digits;
						io_refData.t_6_3_2.t_common.min_entropy_lower_bound = min_entropy_lower_bound;
						io_refData.t_6_3_2.t_common.min_entropy_upper_bound = min_entropy_upper_bound;
					}
				}
				// -------------------------------------------------------------------------- //
				// map for LaTeX output
				// -------------------------------------------------------------------------- //
				if (io_refData.isGeneratingReportInLaTeXformatRequested)
				{
					// -------------------------------------------------------------------------- //
					//
					// -------------------------------------------------------------------------- //
					outputLaTeX(io_refData, bz_t_nu);
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