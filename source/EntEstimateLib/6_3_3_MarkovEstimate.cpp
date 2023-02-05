////////////////////////////////////////////////////////////////////////////////
// 6_3_3_MarkovEstimate.cpp
//
//
//
// Copyright (c) 2021-2023 G. G. SAKURAI <g.garland823@gmail.com>
//
////////////////////////////////////////////////////////////////////////////////
#include "pch.h"
#include "6_3_3_MarkovEstimate.h"
#include "./math/SpecialFunctions.h"
#include "./support/checkArgs.h"
#include <boost/math/special_functions.hpp>

namespace entropy_estimator_lib
{
	namespace estimators
	{
		namespace markov
		{
			namespace ns_consts = entropy_estimator_lib::constants;
			namespace ns_dt = entropy_estimator_lib::data_types;
			namespace ns_spt = entropy_estimator_lib::support;

			typedef ns_consts::EnmReturnStatus(*PF_MARKOV_EE)(double& o_refMinEntropyEstimate,
				const blitz::Array<double, 2>& i_refPTransition,
				const blitz::Array<double, 1>& i_refInitialP, int i_lengthOfSeq);

			// -------------------------------------------------------------------------- //
			/// <summary>
			///  Output Figure in LaTeX
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
			ns_consts::EnmReturnStatus outputLaTeXFigure(ns_dt::t_data_for_estimator& io_refData)
			{
				ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorUnexpected;

				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				(*io_refData.p_ssLaTeXFragment) << L"\\begin{figure}[htbp]" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"\\begin{tikzpicture} " << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"\\begin{axis}[" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"\txlabel=$i$," << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"\tylabel=$P_{i,j}$," << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"	width=10cm," << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"	xmin=-0.125,xmax=1.125," << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"	xtick={0, 1}," << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"	legend style={at={(1,0.75)},anchor=north west}," << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"%	clickable coords={(xy): \\thisrow{label}},%" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"	/pgf/number format/.cd,fixed,precision=6," << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"	scatter/classes={%" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"		a={mark=square*,blue}," << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"		b={mark=square*,red}," << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"		c={mark=square*,green}," << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"		d={mark=square*,cyan}}]" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"	\\addplot[scatter,only marks,%" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"		scatter src=explicit symbolic]%" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"	table[meta=label] {" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"x\ty\tlabel" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << std::setw(2) << 0 << L"\t" << std::setw(8) << io_refData.t_6_3_3.p_transition[0][0] << L"\ta" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << std::setw(2) << 0 << L"\t" << std::setw(8) << io_refData.t_6_3_3.p_transition[1][0] << L"\tb" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << std::setw(2) << 1 << L"\t" << std::setw(8) << io_refData.t_6_3_3.p_transition[0][1] << L"\tc" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << std::setw(2) << 1 << L"\t" << std::setw(8) << io_refData.t_6_3_3.p_transition[1][1] << L"\td" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"	};" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"\\legend{$P_{0,0}$, $P_{0,1}$, $P_{1,0}$, $P_{1,1}$}" << std::endl;
				// -------------------------------------------------------------------------- //
				// 
				// -------------------------------------------------------------------------- //
				(*io_refData.p_ssLaTeXFragment) << L"\\end{axis} " << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"\\end{tikzpicture}" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"\\caption{Transition probability $P_{i,j}$ of $\\S$6.3.3 of NIST SP 800-90B}" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"\\end{figure}" << std::endl;
				// -------------------------------------------------------------------------- //
				// 
				// -------------------------------------------------------------------------- //
				return  sts = ns_consts::EnmReturnStatus::Success;
			}


			// -------------------------------------------------------------------------- //
			/// <summary>
			///  Output Figures in LaTeX
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
			ns_consts::EnmReturnStatus outputLaTeXFigures(ns_dt::t_data_for_estimator& io_refData)
			{
				ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorUnexpected;

				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				(*io_refData.p_ssLaTeXFragment) << L"\\clearpage" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"\\subsection{The Markov Estimate (NIST SP 800-90B Section 6.3.3)}" << std::endl;
				// -------------------------------------------------------------------------- //
				// output transition probability P_{i,j}
				// -------------------------------------------------------------------------- //
				outputLaTeXFigure(io_refData);
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				(*io_refData.p_ssLaTeXFragment) << L"\\begin{figure}[htbp]" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"\\begin{tikzpicture} " << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"\\begin{axis}[" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"\txlabel=Sequence index," << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"\tylabel=$-\\log_{2}\\left ( \\textrm{Probability}\\right ) / 128$," << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"	width=18cm," << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"	xmin=0.5,xmax=14.5," << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"	legend style={at={(1,1)},anchor=north west}," << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"%	clickable coords={(xy): \\thisrow{label}},%" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"	/pgf/number format/.cd,fixed,precision=6," << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"	scatter/classes={%" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"		a={mark=square*,blue}," << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"		b={mark=square*,red}," << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"		c={mark=square*,green}," << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"		d={mark=square*,cyan}," << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"		e={mark=square*,magenta}," << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"		f={mark=square*,yellow}," << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"		g={mark=triangle*,blue}," << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"		h={mark=triangle*,red}," << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"		i={mark=triangle*,green}," << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"		j={mark=triangle*,cyan}," << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"		k={mark=triangle*,magenta}," << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"		l={mark=triangle*,yellow}," << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"		m={mark=o,blue}," << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"		n={mark=o,red}}]" << std::endl;
				for (int j = 0; j < 14; ++j)
				{
					(*io_refData.p_ssLaTeXFragment) << L"	\\addplot[scatter,only marks,%" << std::endl;
					(*io_refData.p_ssLaTeXFragment) << L"		scatter src=explicit symbolic]%" << std::endl;
					(*io_refData.p_ssLaTeXFragment) << L"	table[meta=label] {" << std::endl;
					(*io_refData.p_ssLaTeXFragment) << L"x\ty\tlabel" << std::endl;
					(*io_refData.p_ssLaTeXFragment) << std::setw(2) << j + 1 << L"\t" << std::setw(8) << io_refData.t_6_3_3.negativeLogPoverLength[j];
					switch (j % 14)
					{
					case 0:
						(*io_refData.p_ssLaTeXFragment) << L"\ta" << std::endl;
						break;
					case 1:
						(*io_refData.p_ssLaTeXFragment) << L"\tb" << std::endl;
						break;
					case 2:
						(*io_refData.p_ssLaTeXFragment) << L"\tc" << std::endl;
						break;
					case 3:
						(*io_refData.p_ssLaTeXFragment) << L"\td" << std::endl;
						break;
					case 4:
						(*io_refData.p_ssLaTeXFragment) << L"\te" << std::endl;
						break;
					case 5:
						(*io_refData.p_ssLaTeXFragment) << L"\tf" << std::endl;
						break;
					case 6:
						(*io_refData.p_ssLaTeXFragment) << L"\tg" << std::endl;
						break;
					case 7:
						(*io_refData.p_ssLaTeXFragment) << L"\th" << std::endl;
						break;
					case 8:
						(*io_refData.p_ssLaTeXFragment) << L"\ti" << std::endl;
						break;
					case 9:
						(*io_refData.p_ssLaTeXFragment) << L"\tj" << std::endl;
						break;
					case 10:
						(*io_refData.p_ssLaTeXFragment) << L"\tk" << std::endl;
						break;
					case 11:
						(*io_refData.p_ssLaTeXFragment) << L"\tl" << std::endl;
						break;
					case 12:
						(*io_refData.p_ssLaTeXFragment) << L"\tm" << std::endl;
						break;
					case 13:
						(*io_refData.p_ssLaTeXFragment) << L"\tn" << std::endl;
						break;
					default:
						(*io_refData.p_ssLaTeXFragment) << L"\ta" << std::endl;
						break;
					}
					(*io_refData.p_ssLaTeXFragment) << L"	};" << std::endl;
				}
				(*io_refData.p_ssLaTeXFragment) << L"\\legend{$[$sequence index 1$]$ $0000 \\cdots 0000$, $[$sequence index 2$]$ $0101 \\cdots 0101001010 \\cdots 1010$, $[$sequence index 3$]$ $0101 \\cdots 0101101010 \\cdots 1010$, $[$sequence index 4$]$ $0111 \\cdots 1110$, $[$sequence index 5$]$ $0000 \\cdots 0001$, $[$sequence index 6$]$ $0101 \\cdots 0101$, $[$sequence index 7$]$ $0111 \\cdots 1111$, $[$sequence index 8$]$ $1000 \\cdots 0000$, $[$sequence index 9$]$ $1010 \\cdots 1010$, $[$sequence index 10$]$ $1111 \\cdots 1110$, $[$sequence index 11$]$ $1000 \\cdots 0001$, $[$sequence index 12$]$ $1010 \\cdots 1010100101 \\cdots 0101$, $[$sequence index 13$]$ $1010 \\cdots 1010110101 \\cdots 0101$, $[$sequence index 14$]$ $1111 \\cdots 1111$}" << std::endl;
				// -------------------------------------------------------------------------- //
				// 
				// -------------------------------------------------------------------------- //
				(*io_refData.p_ssLaTeXFragment) << L"\\end{axis} " << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"\\end{tikzpicture}" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"\\caption{Estimated Min-Entropy using $\\S$6.3.3 of NIST SP 800-90B}" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"\\end{figure}" << std::endl;
				// -------------------------------------------------------------------------- //
				// 
				// -------------------------------------------------------------------------- //
				return  sts = ns_consts::EnmReturnStatus::Success;
			}



			// -------------------------------------------------------------------------- //
			/// <summary>
			///  Calculate -log(P) for categorized sample sequence case 1
			/// </summary>
			/// <remarks>
			///  called from <c>calcNegativeLogarithmProbability</c> function.
			/// </remarks>
			/// <params="o_refMinEntropyEstimate">
			///   [Output] Resultant -log2(P) / L.
			/// </params>
			/// <params="i_refPTransition">
			///   [Input] Transition matrix.
			///   Note here that this matrix is defined so as to operate on the colum vector from left.
			///   See Step 1 of 6.3.3 of NIST SP 800-90B for more details.
			/// </params>
			/// <params="i_refInitialP">
			///   [Input] initial probabilities for each output value.
			///   See Step 1 of 6.3.3 of NIST SP 800-90B for more details.
			/// </params>
			/// <params="i_lengthOfSeq">
			///   [Input] Length of input S. See 6.3.3 of NIST SP 800-90B for more details.
			/// </params>
			/// <returns>
			/// </returns>
			/// <precondition>
			/// </precondition>
			/// <postcondition>
			/// </postcondition>
			// -------------------------------------------------------------------------- //
			ns_consts::EnmReturnStatus calcNegativeLogarithmProbabilityC01(double& o_refMinEntropyEstimate,
				const blitz::Array<double, 2>& i_refPTransition, 
				const blitz::Array<double, 1>& i_refInitialP, int i_lengthOfSeq)
			{
				ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorDomain;

				if (i_refInitialP(0) < DBL_EPSILON)
				{
					return	sts;
				}
				if (i_refPTransition(0, 0) < DBL_EPSILON)
				{
					return	sts;
				}
				o_refMinEntropyEstimate = -(((double)(i_lengthOfSeq - 1) / (double)i_lengthOfSeq) * log2(i_refPTransition(0, 0))
					+ log2(i_refInitialP(0)) / (double)i_lengthOfSeq);
				return	sts = ns_consts::EnmReturnStatus::Success;
			}

			// -------------------------------------------------------------------------- //
			/// <summary>
			///  Calculate -log(P) for categorized sample sequence case 2
			/// </summary>
			/// <remarks>
			///  called from <c>calcNegativeLogarithmProbability</c> function.
			/// </remarks>
			/// <params="o_refMinEntropyEstimate">
			///   [Output] Resultant -log2(P) / L.
			/// </params>
			/// <params="i_refPTransition">
			///   [Input] Transition matrix.
			///   Note here that this matrix is defined so as to operate on the colum vector from left.
			///   See Step 1 of 6.3.3 of NIST SP 800-90B for more details.
			/// </params>
			/// <params="i_refInitialP">
			///   [Input] initial probabilities for each output value.
			///   See Step 1 of 6.3.3 of NIST SP 800-90B for more details.
			/// </params>
			/// <params="i_lengthOfSeq">
			///   [Input] Length of input S. See 6.3.3 of NIST SP 800-90B for more details.
			/// </params>
			/// <returns>
			/// </returns>
			/// <precondition>
			/// </precondition>
			/// <postcondition>
			/// </postcondition>
			// -------------------------------------------------------------------------- //
			ns_consts::EnmReturnStatus calcNegativeLogarithmProbabilityC02(double& o_refMinEntropyEstimate,
				const blitz::Array<double, 2>& i_refPTransition,
				const blitz::Array<double, 1>& i_refInitialP, int i_lengthOfSeq)
			{
				ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorDomain;

				if (i_refInitialP(0) < DBL_EPSILON)
				{
					return	sts;
				}
				if (i_refPTransition(0, 0) < DBL_EPSILON)
				{
					return	sts;
				}
				if (i_refPTransition(1, 0) < DBL_EPSILON)
				{
					return	sts;
				}
				if (i_refPTransition(0, 1) < DBL_EPSILON)
				{
					return	sts;
				}
				o_refMinEntropyEstimate = - (0.5 * ((double)(i_lengthOfSeq - 2) / (double)i_lengthOfSeq) * log2(i_refPTransition(0, 1) * i_refPTransition(1, 0))
					+ log2(i_refInitialP(0) * i_refPTransition(0, 0)) / (double)i_lengthOfSeq);
				return	sts = ns_consts::EnmReturnStatus::Success;
			}

			// -------------------------------------------------------------------------- //
			/// <summary>
			///  Calculate -log(P) for categorized sample sequence case 3
			/// </summary>
			/// <remarks>
			///  called from <c>calcNegativeLogarithmProbability</c> function.
			/// </remarks>
			/// <params="o_refMinEntropyEstimate">
			///   [Output] Resultant -log2(P) / L.
			/// </params>
			/// <params="i_refPTransition">
			///   [Input] Transition matrix.
			///   Note here that this matrix is defined so as to operate on the colum vector from left.
			///   See Step 1 of 6.3.3 of NIST SP 800-90B for more details.
			/// </params>
			/// <params="i_refInitialP">
			///   [Input] initial probabilities for each output value.
			///   See Step 1 of 6.3.3 of NIST SP 800-90B for more details.
			/// </params>
			/// <params="i_lengthOfSeq">
			///   [Input] Length of input S. See 6.3.3 of NIST SP 800-90B for more details.
			/// </params>
			/// <returns>
			/// </returns>
			/// <precondition>
			/// </precondition>
			/// <postcondition>
			/// </postcondition>
			// -------------------------------------------------------------------------- //
			ns_consts::EnmReturnStatus calcNegativeLogarithmProbabilityC03(double& o_refMinEntropyEstimate,
				const blitz::Array<double, 2>& i_refPTransition,
				const blitz::Array<double, 1>& i_refInitialP, int i_lengthOfSeq)
			{
				ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorDomain;

				if (i_refInitialP(0) < DBL_EPSILON)
				{
					return	sts;
				}
				if (i_refPTransition(1, 1) < DBL_EPSILON)
				{
					return	sts;
				}
				if (i_refPTransition(1, 0) < DBL_EPSILON)
				{
					return	sts;
				}
				if (i_refPTransition(0, 1) < DBL_EPSILON)
				{
					return	sts;
				}
				o_refMinEntropyEstimate = - (0.5 * ((double)(i_lengthOfSeq - 2) / (double)i_lengthOfSeq) * log2(i_refPTransition(0, 1) * i_refPTransition(1, 0))
					+ log2(i_refInitialP(0) * i_refPTransition(1, 1)) / (double)i_lengthOfSeq);
				return	sts = ns_consts::EnmReturnStatus::Success;
			}

			// -------------------------------------------------------------------------- //
			/// <summary>
			///  Calculate -log(P) for categorized sample sequence case 4
			/// </summary>
			/// <remarks>
			///  called from <c>calcNegativeLogarithmProbability</c> function.
			/// </remarks>
			/// <params="o_refMinEntropyEstimate">
			///   [Output] Resultant -log2(P) / L.
			/// </params>
			/// <params="i_refPTransition">
			///   [Input] Transition matrix.
			///   Note here that this matrix is defined so as to operate on the colum vector from left.
			///   See Step 1 of 6.3.3 of NIST SP 800-90B for more details.
			/// </params>
			/// <params="i_refInitialP">
			///   [Input] initial probabilities for each output value.
			///   See Step 1 of 6.3.3 of NIST SP 800-90B for more details.
			/// </params>
			/// <params="i_lengthOfSeq">
			///   [Input] Length of input S. See 6.3.3 of NIST SP 800-90B for more details.
			/// </params>
			/// <returns>
			/// </returns>
			/// <precondition>
			/// </precondition>
			/// <postcondition>
			/// </postcondition>
			// -------------------------------------------------------------------------- //
			ns_consts::EnmReturnStatus calcNegativeLogarithmProbabilityC04(double& o_refMinEntropyEstimate,
				const blitz::Array<double, 2>& i_refPTransition,
				const blitz::Array<double, 1>& i_refInitialP, int i_lengthOfSeq)
			{
				ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorDomain;

				if (i_refInitialP(0) < DBL_EPSILON)
				{
					return	sts;
				}
				if (i_refPTransition(1, 1) < DBL_EPSILON)
				{
					return	sts;
				}
				if (i_refPTransition(1, 0) < DBL_EPSILON)
				{
					return	sts;
				}
				if (i_refPTransition(0, 1) < DBL_EPSILON)
				{
					return	sts;
				}
				o_refMinEntropyEstimate = -(((double)(i_lengthOfSeq - 3) / (double)i_lengthOfSeq) * log2(i_refPTransition(1, 1))
					+ log2(i_refInitialP(0) * i_refPTransition(1, 0) * i_refPTransition(0, 1)) / (double)i_lengthOfSeq);
				return	sts = ns_consts::EnmReturnStatus::Success;
			}

			// -------------------------------------------------------------------------- //
			/// <summary>
			///  Calculate -log(P) for categorized sample sequence case 5
			/// </summary>
			/// <remarks>
			///  called from <c>calcNegativeLogarithmProbability</c> function.
			/// </remarks>
			/// <params="o_refMinEntropyEstimate">
			///   [Output] Resultant -log2(P) / L.
			/// </params>
			/// <params="i_refPTransition">
			///   [Input] Transition matrix.
			///   Note here that this matrix is defined so as to operate on the colum vector from left.
			///   See Step 1 of 6.3.3 of NIST SP 800-90B for more details.
			/// </params>
			/// <params="i_refInitialP">
			///   [Input] initial probabilities for each output value.
			///   See Step 1 of 6.3.3 of NIST SP 800-90B for more details.
			/// </params>
			/// <params="i_lengthOfSeq">
			///   [Input] Length of input S. See 6.3.3 of NIST SP 800-90B for more details.
			/// </params>
			/// <returns>
			/// </returns>
			/// <precondition>
			/// </precondition>
			/// <postcondition>
			/// </postcondition>
			// -------------------------------------------------------------------------- //
			ns_consts::EnmReturnStatus calcNegativeLogarithmProbabilityC05(double& o_refMinEntropyEstimate,
				const blitz::Array<double, 2>& i_refPTransition,
				const blitz::Array<double, 1>& i_refInitialP, int i_lengthOfSeq)
			{
				ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorDomain;

				if (i_refInitialP(0) < DBL_EPSILON)
				{
					return	sts;
				}
				if (i_refPTransition(0, 0) < DBL_EPSILON)
				{
					return	sts;
				}
				if (i_refPTransition(1, 0) < DBL_EPSILON)
				{
					return	sts;
				}
				o_refMinEntropyEstimate = -(((double)(i_lengthOfSeq - 2) / (double)i_lengthOfSeq) * log2(i_refPTransition(0, 0))
					+ log2(i_refInitialP(0) * i_refPTransition(1, 0)) / (double)i_lengthOfSeq);
				return	sts = ns_consts::EnmReturnStatus::Success;
			}

			// -------------------------------------------------------------------------- //
			/// <summary>
			///  Calculate -log(P) for categorized sample sequence case 6
			/// </summary>
			/// <remarks>
			///  called from <c>calcNegativeLogarithmProbability</c> function.
			/// </remarks>
			/// <params="o_refMinEntropyEstimate">
			///   [Output] Resultant -log2(P) / L.
			/// </params>
			/// <params="i_refPTransition">
			///   [Input] Transition matrix.
			///   Note here that this matrix is defined so as to operate on the colum vector from left.
			///   See Step 1 of 6.3.3 of NIST SP 800-90B for more details.
			/// </params>
			/// <params="i_refInitialP">
			///   [Input] initial probabilities for each output value.
			///   See Step 1 of 6.3.3 of NIST SP 800-90B for more details.
			/// </params>
			/// <params="i_lengthOfSeq">
			///   [Input] Length of input S. See 6.3.3 of NIST SP 800-90B for more details.
			/// </params>
			/// <returns>
			/// </returns>
			/// <precondition>
			/// </precondition>
			/// <postcondition>
			/// </postcondition>
			// -------------------------------------------------------------------------- //
			ns_consts::EnmReturnStatus calcNegativeLogarithmProbabilityC06(double& o_refMinEntropyEstimate,
				const blitz::Array<double, 2>& i_refPTransition,
				const blitz::Array<double, 1>& i_refInitialP, int i_lengthOfSeq)
			{
				ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorDomain;

				if (i_refInitialP(0) < DBL_EPSILON)
				{
					return	sts;
				}
				if (i_refPTransition(1, 0) < DBL_EPSILON)
				{
					return	sts;
				}
				if (i_refPTransition(0, 1) < DBL_EPSILON)
				{
					return	sts;
				}
				o_refMinEntropyEstimate = - (0.5 * ((double)(i_lengthOfSeq - 2) / (double)i_lengthOfSeq) * log2(i_refPTransition(0, 1) * i_refPTransition(1, 0))
					+ log2(i_refInitialP(0) * i_refPTransition(1, 0)) / (double)i_lengthOfSeq);
				return	sts = ns_consts::EnmReturnStatus::Success;
			}

			// -------------------------------------------------------------------------- //
			/// <summary>
			///  Calculate -log(P) for categorized sample sequence case 7
			/// </summary>
			/// <remarks>
			///  called from <c>calcNegativeLogarithmProbability</c> function.
			/// </remarks>
			/// <params="o_refMinEntropyEstimate">
			///   [Output] Resultant -log2(P) / L.
			/// </params>
			/// <params="i_refPTransition">
			///   [Input] Transition matrix.
			///   Note here that this matrix is defined so as to operate on the colum vector from left.
			///   See Step 1 of 6.3.3 of NIST SP 800-90B for more details.
			/// </params>
			/// <params="i_refInitialP">
			///   [Input] initial probabilities for each output value.
			///   See Step 1 of 6.3.3 of NIST SP 800-90B for more details.
			/// </params>
			/// <params="i_lengthOfSeq">
			///   [Input] Length of input S. See 6.3.3 of NIST SP 800-90B for more details.
			/// </params>
			/// <returns>
			/// </returns>
			/// <precondition>
			/// </precondition>
			/// <postcondition>
			/// </postcondition>
			// -------------------------------------------------------------------------- //
			ns_consts::EnmReturnStatus calcNegativeLogarithmProbabilityC07(double& o_refMinEntropyEstimate,
				const blitz::Array<double, 2>& i_refPTransition,
				const blitz::Array<double, 1>& i_refInitialP, int i_lengthOfSeq)
			{
				ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorDomain;

				if (i_refInitialP(0) < DBL_EPSILON)
				{
					return	sts;
				}
				if (i_refPTransition(1, 1) < DBL_EPSILON)
				{
					return	sts;
				}
				if (i_refPTransition(1, 0) < DBL_EPSILON)
				{
					return	sts;
				}
				o_refMinEntropyEstimate = - (((double)(i_lengthOfSeq - 2) / (double)i_lengthOfSeq) * log2(i_refPTransition(1, 1))
					+ log2(i_refInitialP(0) * i_refPTransition(1, 0)) / (double)i_lengthOfSeq);
				return	sts = ns_consts::EnmReturnStatus::Success;
			}

			// -------------------------------------------------------------------------- //
			/// <summary>
			///  Calculate -log(P) for categorized sample sequence case 8
			/// </summary>
			/// <remarks>
			///  called from <c>calcNegativeLogarithmProbability</c> function.
			/// </remarks>
			/// <params="o_refMinEntropyEstimate">
			///   [Output] Resultant -log2(P) / L.
			/// </params>
			/// <params="i_refPTransition">
			///   [Input] Transition matrix.
			///   Note here that this matrix is defined so as to operate on the colum vector from left.
			///   See Step 1 of 6.3.3 of NIST SP 800-90B for more details.
			/// </params>
			/// <params="i_refInitialP">
			///   [Input] initial probabilities for each output value.
			///   See Step 1 of 6.3.3 of NIST SP 800-90B for more details.
			/// </params>
			/// <params="i_lengthOfSeq">
			///   [Input] Length of input S. See 6.3.3 of NIST SP 800-90B for more details.
			/// </params>
			/// <returns>
			/// </returns>
			/// <precondition>
			/// </precondition>
			/// <postcondition>
			/// </postcondition>
			// -------------------------------------------------------------------------- //
			ns_consts::EnmReturnStatus calcNegativeLogarithmProbabilityC08(double& o_refMinEntropyEstimate,
				const blitz::Array<double, 2>& i_refPTransition,
				const blitz::Array<double, 1>& i_refInitialP, int i_lengthOfSeq)
			{
				ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorDomain;

				if (i_refInitialP(1) < DBL_EPSILON)
				{
					return	sts;
				}
				if (i_refPTransition(0, 0) < DBL_EPSILON)
				{
					return	sts;
				}
				if (i_refPTransition(0, 1) < DBL_EPSILON)
				{
					return	sts;
				}
				o_refMinEntropyEstimate = - (((double)(i_lengthOfSeq - 2) / (double)i_lengthOfSeq) * log2(i_refPTransition(0, 0))
					+ log2(i_refInitialP(1) * i_refPTransition(0, 1)) / (double)i_lengthOfSeq);
				return	sts = ns_consts::EnmReturnStatus::Success;
			}

			// -------------------------------------------------------------------------- //
			/// <summary>
			///  Calculate -log(P) for categorized sample sequence case 9
			/// </summary>
			/// <remarks>
			///  called from <c>calcNegativeLogarithmProbability</c> function.
			/// </remarks>
			/// <params="o_refMinEntropyEstimate">
			///   [Output] Resultant -log2(P) / L.
			/// </params>
			/// <params="i_refPTransition">
			///   [Input] Transition matrix.
			///   Note here that this matrix is defined so as to operate on the colum vector from left.
			///   See Step 1 of 6.3.3 of NIST SP 800-90B for more details.
			/// </params>
			/// <params="i_refInitialP">
			///   [Input] initial probabilities for each output value.
			///   See Step 1 of 6.3.3 of NIST SP 800-90B for more details.
			/// </params>
			/// <params="i_lengthOfSeq">
			///   [Input] Length of input S. See 6.3.3 of NIST SP 800-90B for more details.
			/// </params>
			/// <returns>
			/// </returns>
			/// <precondition>
			/// </precondition>
			/// <postcondition>
			/// </postcondition>
			// -------------------------------------------------------------------------- //
			ns_consts::EnmReturnStatus calcNegativeLogarithmProbabilityC09(double& o_refMinEntropyEstimate,
				const blitz::Array<double, 2>& i_refPTransition,
				const blitz::Array<double, 1>& i_refInitialP, int i_lengthOfSeq)
			{
				ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorDomain;

				if (i_refInitialP(1) < DBL_EPSILON)
				{
					return	sts;
				}
				if (i_refPTransition(1, 0) < DBL_EPSILON)
				{
					return	sts;
				}
				if (i_refPTransition(0, 1) < DBL_EPSILON)
				{
					return	sts;
				}
				o_refMinEntropyEstimate = - (0.5 * ((double)(i_lengthOfSeq - 2) / (double)i_lengthOfSeq) * log2(i_refPTransition(0, 1) * i_refPTransition(1, 0))
					+ log2(i_refInitialP(1) * i_refPTransition(0, 1)) / (double)i_lengthOfSeq);
				return	sts = ns_consts::EnmReturnStatus::Success;
			}

			// -------------------------------------------------------------------------- //
			/// <summary>
			///  Calculate -log(P) for categorized sample sequence case 10
			/// </summary>
			/// <remarks>
			///  called from <c>calcNegativeLogarithmProbability</c> function.
			/// </remarks>
			/// <params="o_refMinEntropyEstimate">
			///   [Output] Resultant -log2(P) / L.
			/// </params>
			/// <params="i_refPTransition">
			///   [Input] Transition matrix.
			///   Note here that this matrix is defined so as to operate on the colum vector from left.
			///   See Step 1 of 6.3.3 of NIST SP 800-90B for more details.
			/// </params>
			/// <params="i_refInitialP">
			///   [Input] initial probabilities for each output value.
			///   See Step 1 of 6.3.3 of NIST SP 800-90B for more details.
			/// </params>
			/// <params="i_lengthOfSeq">
			///   [Input] Length of input S. See 6.3.3 of NIST SP 800-90B for more details.
			/// </params>
			/// <returns>
			/// </returns>
			/// <precondition>
			/// </precondition>
			/// <postcondition>
			/// </postcondition>
			// -------------------------------------------------------------------------- //
			ns_consts::EnmReturnStatus calcNegativeLogarithmProbabilityC10(double& o_refMinEntropyEstimate,
				const blitz::Array<double, 2>& i_refPTransition,
				const blitz::Array<double, 1>& i_refInitialP, int i_lengthOfSeq)
			{
				ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorDomain;

				if (i_refInitialP(1) < DBL_EPSILON)
				{
					return	sts;
				}
				if (i_refPTransition(0, 1) < DBL_EPSILON)
				{
					return	sts;
				}
				if (i_refPTransition(1, 1) < DBL_EPSILON)
				{
					return	sts;
				}
				o_refMinEntropyEstimate = - (((double)(i_lengthOfSeq - 2) / (double)i_lengthOfSeq) * log2(i_refPTransition(1, 1))
					+ log2(i_refInitialP(1) * i_refPTransition(0, 1)) / (double)i_lengthOfSeq);
				return	sts = ns_consts::EnmReturnStatus::Success;
			}

			// -------------------------------------------------------------------------- //
			/// <summary>
			///  Calculate -log(P) for categorized sample sequence case 11
			/// </summary>
			/// <remarks>
			///  called from <c>calcNegativeLogarithmProbability</c> function.
			/// </remarks>
			/// <params="o_refMinEntropyEstimate">
			///   [Output] Resultant -log2(P) / L.
			/// </params>
			/// <params="i_refPTransition">
			///   [Input] Transition matrix.
			///   Note here that this matrix is defined so as to operate on the colum vector from left.
			///   See Step 1 of 6.3.3 of NIST SP 800-90B for more details.
			/// </params>
			/// <params="i_refInitialP">
			///   [Input] initial probabilities for each output value.
			///   See Step 1 of 6.3.3 of NIST SP 800-90B for more details.
			/// </params>
			/// <params="i_lengthOfSeq">
			///   [Input] Length of input S. See 6.3.3 of NIST SP 800-90B for more details.
			/// </params>
			/// <returns>
			/// </returns>
			/// <precondition>
			/// </precondition>
			/// <postcondition>
			/// </postcondition>
			// -------------------------------------------------------------------------- //
			ns_consts::EnmReturnStatus calcNegativeLogarithmProbabilityC11(double& o_refMinEntropyEstimate,
				const blitz::Array<double, 2>& i_refPTransition,
				const blitz::Array<double, 1>& i_refInitialP, int i_lengthOfSeq)
			{
				ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorDomain;

				if (i_refInitialP(1) < DBL_EPSILON)
				{
					return	sts;
				}
				if (i_refPTransition(0, 0) < DBL_EPSILON)
				{
					return	sts;
				}
				if (i_refPTransition(1, 0) < DBL_EPSILON)
				{
					return	sts;
				}
				if (i_refPTransition(0, 1) < DBL_EPSILON)
				{
					return	sts;
				}
				o_refMinEntropyEstimate = - (((double)(i_lengthOfSeq - 3) / (double)i_lengthOfSeq) * log2(i_refPTransition(0, 0))
					+ log2(i_refInitialP(1) * i_refPTransition(0, 1) * i_refPTransition(1, 0)) / (double)i_lengthOfSeq);
				return	sts = ns_consts::EnmReturnStatus::Success;
			}

			// -------------------------------------------------------------------------- //
			/// <summary>
			///  Calculate -log(P) for categorized sample sequence case 12
			/// </summary>
			/// <remarks>
			///  called from <c>calcNegativeLogarithmProbability</c> function.
			/// </remarks>
			/// <params="o_refMinEntropyEstimate">
			///   [Output] Resultant -log2(P) / L.
			/// </params>
			/// <params="i_refPTransition">
			///   [Input] Transition matrix.
			///   Note here that this matrix is defined so as to operate on the colum vector from left.
			///   See Step 1 of 6.3.3 of NIST SP 800-90B for more details.
			/// </params>
			/// <params="i_refInitialP">
			///   [Input] initial probabilities for each output value.
			///   See Step 1 of 6.3.3 of NIST SP 800-90B for more details.
			/// </params>
			/// <params="i_lengthOfSeq">
			///   [Input] Length of input S. See 6.3.3 of NIST SP 800-90B for more details.
			/// </params>
			/// <returns>
			/// </returns>
			/// <precondition>
			/// </precondition>
			/// <postcondition>
			/// </postcondition>
			// -------------------------------------------------------------------------- //
			ns_consts::EnmReturnStatus calcNegativeLogarithmProbabilityC12(double& o_refMinEntropyEstimate,
				const blitz::Array<double, 2>& i_refPTransition,
				const blitz::Array<double, 1>& i_refInitialP, int i_lengthOfSeq)
			{
				ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorDomain;

				if (i_refInitialP(1) < DBL_EPSILON)
				{
					return	sts;
				}
				if (i_refPTransition(0, 0) < DBL_EPSILON)
				{
					return	sts;
				}
				if (i_refPTransition(1, 0) < DBL_EPSILON)
				{
					return	sts;
				}
				if (i_refPTransition(0, 1) < DBL_EPSILON)
				{
					return	sts;
				}
				o_refMinEntropyEstimate = - (0.5 * ((double)(i_lengthOfSeq - 2) / (double)i_lengthOfSeq) * log2(i_refPTransition(0, 1) * i_refPTransition(1, 0))
					+ log2(i_refInitialP(1) * i_refPTransition(0, 0)) / (double)i_lengthOfSeq);
				return	sts = ns_consts::EnmReturnStatus::Success;
			}

			// -------------------------------------------------------------------------- //
			/// <summary>
			///  Calculate -log(P) for categorized sample sequence case 13
			/// </summary>
			/// <remarks>
			///  called from <c>calcNegativeLogarithmProbability</c> function.
			/// </remarks>
			/// <params="o_refMinEntropyEstimate">
			///   [Output] Resultant -log2(P) / L.
			/// </params>
			/// <params="i_refPTransition">
			///   [Input] Transition matrix.
			///   Note here that this matrix is defined so as to operate on the colum vector from left.
			///   See Step 1 of 6.3.3 of NIST SP 800-90B for more details.
			/// </params>
			/// <params="i_refInitialP">
			///   [Input] initial probabilities for each output value.
			///   See Step 1 of 6.3.3 of NIST SP 800-90B for more details.
			/// </params>
			/// <params="i_lengthOfSeq">
			///   [Input] Length of input S. See 6.3.3 of NIST SP 800-90B for more details.
			/// </params>
			/// <returns>
			/// </returns>
			/// <precondition>
			/// </precondition>
			/// <postcondition>
			/// </postcondition>
			// -------------------------------------------------------------------------- //
			ns_consts::EnmReturnStatus calcNegativeLogarithmProbabilityC13(double& o_refMinEntropyEstimate,
				const blitz::Array<double, 2>& i_refPTransition,
				const blitz::Array<double, 1>& i_refInitialP, int i_lengthOfSeq)
			{
				ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorDomain;

				if (i_refInitialP(1) < DBL_EPSILON)
				{
					return	sts;
				}
				if (i_refPTransition(1, 1) < DBL_EPSILON)
				{
					return	sts;
				}
				if (i_refPTransition(1, 0) < DBL_EPSILON)
				{
					return	sts;
				}
				if (i_refPTransition(0, 1) < DBL_EPSILON)
				{
					return	sts;
				}
				o_refMinEntropyEstimate = - (0.5 * ((double)(i_lengthOfSeq - 2) / (double)i_lengthOfSeq) * log2(i_refPTransition(0, 1) * i_refPTransition(1, 0))
					+ log2(i_refInitialP(1) * i_refPTransition(1, 1)) / (double)i_lengthOfSeq);
				return	sts = ns_consts::EnmReturnStatus::Success;
			}

			// -------------------------------------------------------------------------- //
			/// <summary>
			///  Calculate -log(P) for categorized sample sequence case 14
			/// </summary>
			/// <remarks>
			///  called from <c>calcNegativeLogarithmProbability</c> function.
			/// </remarks>
			/// <params="o_refMinEntropyEstimate">
			///   [Output] Resultant -log2(P) / L.
			/// </params>
			/// <params="i_refPTransition">
			///   [Input] Transition matrix.
			///   Note here that this matrix is defined so as to operate on the colum vector from left.
			///   See Step 1 of 6.3.3 of NIST SP 800-90B for more details.
			/// </params>
			/// <params="i_refInitialP">
			///   [Input] initial probabilities for each output value.
			///   See Step 1 of 6.3.3 of NIST SP 800-90B for more details.
			/// </params>
			/// <params="i_lengthOfSeq">
			///   [Input] Length of input S. See 6.3.3 of NIST SP 800-90B for more details.
			/// </params>
			/// <returns>
			/// </returns>
			/// <precondition>
			/// </precondition>
			/// <postcondition>
			/// </postcondition>
			// -------------------------------------------------------------------------- //
			ns_consts::EnmReturnStatus calcNegativeLogarithmProbabilityC14(double& o_refMinEntropyEstimate,
				const blitz::Array<double, 2>& i_refPTransition,
				const blitz::Array<double, 1>& i_refInitialP, int i_lengthOfSeq)
			{
				ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorDomain;

				if (i_refInitialP(1) < DBL_EPSILON)
				{
					return	sts;
				}
				if (i_refPTransition(1, 1) < DBL_EPSILON)
				{
					return	sts;
				}
				o_refMinEntropyEstimate = - (((double)(i_lengthOfSeq - 1) / (double)i_lengthOfSeq) * log2(i_refPTransition(1, 1))
					+ log2(i_refInitialP(1)) / (double)i_lengthOfSeq);
				return	sts = ns_consts::EnmReturnStatus::Success;
			}

			// -------------------------------------------------------------------------- //
			/// <summary>
			/// </summary>
			/// <remarks>
			/// </remarks>
			/// <params="o_refMinEntropyEstimate">
			///   [Output] Resultant -log2(P) / L.
			/// </params>
			/// <params="i_refPTransition">
			///   [Input] Transition matrix.
			///   Note here that this matrix is defined so as to operate on the colum vector from left.
			///   See Step 1 of 6.3.3 of NIST SP 800-90B for more details.
			/// </params>
			/// <params="i_refInitialP">
			///   [Input] initial probabilities for each output value.
			///   See Step 1 of 6.3.3 of NIST SP 800-90B for more details.
			/// </params>
			/// <params="i_lengthOfSeq">
			///   [Input] Length of input S. See 6.3.3 of NIST SP 800-90B for more details.
			/// </params>
			/// <returns>
			/// </returns>
			/// <precondition>
			/// </precondition>
			/// <postcondition>
			/// </postcondition>
			// -------------------------------------------------------------------------- //
			ns_consts::EnmReturnStatus calcNegativeLogarithmProbability(ns_dt::t_data_for_markov_estimate & io_refData,
				const blitz::Array<double, 2>& i_refPTransition, 
				const blitz::Array<double, 1>& i_refInitialP, int i_lengthOfSequence)
			{
				ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorUnexpected;

				PF_MARKOV_EE	funcs[14] = {
					calcNegativeLogarithmProbabilityC01,
					calcNegativeLogarithmProbabilityC02,
					calcNegativeLogarithmProbabilityC03,
					calcNegativeLogarithmProbabilityC04,
					calcNegativeLogarithmProbabilityC05,
					calcNegativeLogarithmProbabilityC06,
					calcNegativeLogarithmProbabilityC07,
					calcNegativeLogarithmProbabilityC08,
					calcNegativeLogarithmProbabilityC09,
					calcNegativeLogarithmProbabilityC10,
					calcNegativeLogarithmProbabilityC11,
					calcNegativeLogarithmProbabilityC12,
					calcNegativeLogarithmProbabilityC13,
					calcNegativeLogarithmProbabilityC14,
				};
				if (i_refPTransition.length(blitz::firstDim) != 2)
				{
					return sts = ns_consts::EnmReturnStatus::ErrorInvalidData;
				}
				if (i_refPTransition.length(blitz::secondDim) != 2)
				{
					return sts = ns_consts::EnmReturnStatus::ErrorInvalidData;
				}
				if (i_refInitialP.length(blitz::firstDim) != 2)
				{
					return sts = ns_consts::EnmReturnStatus::ErrorInvalidData;
				}

				//blitz::Array<double, 1>	bz_log_p_over_L(14);
				//bz_log_p_over_L = 0.0;

				double	minEntropyForCases[14] = { 0.0 };
				io_refData.t_common.min_entropy = 1.0;
				for (int i = 0; i < 14; ++i)
				{
					ns_consts::EnmReturnStatus	stsForCase
						= funcs[i](minEntropyForCases[i],
							i_refPTransition, i_refInitialP, i_lengthOfSequence);
					if (ns_consts::EnmReturnStatus::Success == stsForCase)
					{
						if ((DBL_EPSILON < minEntropyForCases[i]) && (minEntropyForCases[i] < io_refData.t_common.min_entropy))
						{
							io_refData.t_common.min_entropy = minEntropyForCases[i];
							io_refData.p_max = pow(2.0, -minEntropyForCases[i]);
							io_refData.mostLikelySequenceCategoryIndex = i;
						}
					}
					io_refData.negativeLogPoverLength[i] = minEntropyForCases[i];
				}

				if (1.0 < io_refData.t_common.min_entropy)
				{
					io_refData.t_common.min_entropy = 1.0;
				}
				return	sts = ns_consts::EnmReturnStatus::Success;
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
				if (i_refData.k != 2)
				{
					sts = ns_consts::EnmReturnStatus::ErrorInvalidData;
					return sts;
				}
				if (i_refData.L < 3)
				{
					sts = ns_consts::EnmReturnStatus::ErrorInvalidData;
					return sts;
				}
				if (0 == (*i_refData.p_bzSampleSpaceA)(0))
				{
					if (1 != (*i_refData.p_bzSampleSpaceA)(1))
					{
						sts = ns_consts::EnmReturnStatus::ErrorInvalidData;
						return sts;
					}
				}
				else if (1 == (*i_refData.p_bzSampleSpaceA)(0))
				{
					if (0 != (*i_refData.p_bzSampleSpaceA)(1))
					{
						sts = ns_consts::EnmReturnStatus::ErrorInvalidData;
						return sts;
					}
				}
				else
				{
					sts = ns_consts::EnmReturnStatus::ErrorInvalidData;
					return sts;
				}

				return sts = ns_consts::EnmReturnStatus::Success;
			}

			// -------------------------------------------------------------------------- //
			/// <summary>
			///  Implements The Markov Estimate (see 6.3.3 of NIST SP 800-90B).
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

				// -------------------------------------------------------------------------- //
				// Step 1.
				//  Estimate the initial probabilities for each output value, P_0 = #{0 in S} / L and P_1 = 1-P_0.
				// -------------------------------------------------------------------------- //
				blitz::Array<int, 1>	bz_count(1);
				bz_count = blitz::count((*io_refData.p_bzInputS) == (*io_refData.p_bzSampleSpaceA)(0));
				blitz::Array<double, 1>	bz_initial_p(2);
				if (0 == (*io_refData.p_bzSampleSpaceA)(0))
				{
					bz_initial_p(0) = bz_count(0) / (double)(io_refData.p_bzInputS->length(blitz::firstDim));
					bz_initial_p(1) = 1.0 - bz_initial_p(0);
				}
				else
				{
					bz_initial_p(1) = bz_count(0) / (double)(io_refData.p_bzInputS->length(blitz::firstDim));
					bz_initial_p(0) = 1.0 - bz_initial_p(1);
				}

				unsigned long long	tuple_count[2][2] = { {0, 0}, {0, 0} };
				for (int i = 0; i < (io_refData.p_bzInputS->length(blitz::firstDim) - 1); ++i)
				{
					switch ((*io_refData.p_bzInputS)(i))
					{
					case 0:
						switch ((*io_refData.p_bzInputS)(i + 1))
						{
						case 0:
							// 00
							tuple_count[0][0] += 1;
							break;
						case 1:
							// 01 (i.e. 0 -> 1)
							tuple_count[1][0] += 1;
							break;
						default:
							break;
						}
						break;
					case 1:
						switch ((*io_refData.p_bzInputS)(i + 1))
						{
						case 0:
							// 10 (i.e. 1 -> 0)
							tuple_count[0][1] += 1;
							break;
						case 1:
							// 11
							tuple_count[1][1] += 1;
							break;
						default:
							break;
						}
						break;
					default:
						break;
					}
				}
				// -------------------------------------------------------------------------- //
				// Step 2.
				//  Let T be the 2 \times 2 transition matrix of the form
				// 
				//  where the probabilities are calculated as
				// -------------------------------------------------------------------------- //
				blitz::Array<double, 2>		bz_p_transition(2, 2);
				bz_p_transition = 0.0;
				bz_p_transition(0, 0) = (double)tuple_count[0][0] / ((double)tuple_count[0][0] + (double)tuple_count[1][0]);
				bz_p_transition(0, 1) = (double)tuple_count[0][1] / ((double)tuple_count[0][1] + (double)tuple_count[1][1]);
				bz_p_transition(1, 0) = (double)tuple_count[1][0] / ((double)tuple_count[0][0] + (double)tuple_count[1][0]);
				bz_p_transition(1, 1) = (double)tuple_count[1][1] / ((double)tuple_count[0][1] + (double)tuple_count[1][1]);

				for (int j = 0; j < 2; ++j)
				{
					io_refData.t_6_3_3.p_initial[j] = bz_initial_p(j);
					for (int m = 0; m < 2; ++m)
					{
						io_refData.t_6_3_3.p_transition[j][m] = bz_p_transition(j, m);
					}
				}
				// -------------------------------------------------------------------------- //
				// Step 3.
				//  Find the probability of the most likely sequence of outputs of length 128, as calculated below.
				//  Note here that there are possible 14 cases instead of 6 cases in 6.3.3 of NIST SP 800-90B.
				// -------------------------------------------------------------------------- //
				// -------------------------------------------------------------------------- //
				// Step 4.
				//  Let \hat{p}_{max} be the maximum of the probabilities in the table given above. 
				// The min-entropy estimate is the negative logarithm of the probability of the most likely sequence of outputs, \hat{p}_{max}:
				//   min - entropy = min(-log2(\hat{p}_max) / 128, 1)
				// -------------------------------------------------------------------------- //
				const int lengthOfSequence = 128;
				sts = calcNegativeLogarithmProbability(io_refData.t_6_3_3,
					bz_p_transition, bz_initial_p, lengthOfSequence);
				// -------------------------------------------------------------------------- //
				// 
				// -------------------------------------------------------------------------- //
				blitz::Array<double, 2>		bz_p_transition_virtual(2, 2);
				bz_p_transition_virtual = 0.0;

				if (tuple_count[0][0] < tuple_count[1][0])
				{
					if (1 <= tuple_count[0][0])
					{
						bz_p_transition_virtual(0, 0) = ((double)tuple_count[0][0] - 1.0) / ((double)tuple_count[0][0] + (double)tuple_count[1][0]);
						bz_p_transition_virtual(1, 0) = ((double)tuple_count[1][0] + 1.0) / ((double)tuple_count[0][0] + (double)tuple_count[1][0]);
					}
					else
					{
						bz_p_transition_virtual(0, 0) = (double)tuple_count[0][0] / ((double)tuple_count[0][0] + (double)tuple_count[1][0]);
						bz_p_transition_virtual(1, 0) = (double)tuple_count[1][0] / ((double)tuple_count[0][0] + (double)tuple_count[1][0]);
					}
				}
				else
				{
					if (1 <= tuple_count[1][0])
					{
						bz_p_transition_virtual(0, 0) = ((double)tuple_count[0][0] + 1.0) / ((double)tuple_count[0][0] + (double)tuple_count[1][0]);
						bz_p_transition_virtual(1, 0) = ((double)tuple_count[1][0] - 1.0) / ((double)tuple_count[0][0] + (double)tuple_count[1][0]);
					}
					else
					{
						bz_p_transition_virtual(0, 0) = (double)tuple_count[0][0] / ((double)tuple_count[0][0] + (double)tuple_count[1][0]);
						bz_p_transition_virtual(1, 0) = (double)tuple_count[1][0] / ((double)tuple_count[0][0] + (double)tuple_count[1][0]);
					}
				}

				if (tuple_count[1][1] < tuple_count[0][1])
				{
					if (1 <= tuple_count[1][1])
					{
						bz_p_transition_virtual(0, 1) = ((double)tuple_count[0][1] + 1.0) / ((double)tuple_count[0][1] + (double)tuple_count[1][1]);
						bz_p_transition_virtual(1, 1) = ((double)tuple_count[1][1] - 1.0) / ((double)tuple_count[0][1] + (double)tuple_count[1][1]);
					}
					else
					{
						bz_p_transition_virtual(0, 1) = (double)tuple_count[0][1] / ((double)tuple_count[0][1] + (double)tuple_count[1][1]);
						bz_p_transition_virtual(1, 1) = (double)tuple_count[1][1] / ((double)tuple_count[0][1] + (double)tuple_count[1][1]);
					}
				}
				else
				{
					if (1 <= tuple_count[0][1])
					{
						bz_p_transition_virtual(0, 1) = ((double)tuple_count[0][1] - 1.0) / ((double)tuple_count[0][1] + (double)tuple_count[1][1]);
						bz_p_transition_virtual(1, 1) = ((double)tuple_count[1][1] + 1.0) / ((double)tuple_count[0][1] + (double)tuple_count[1][1]);
					}
					else
					{
						bz_p_transition_virtual(0, 1) = (double)tuple_count[0][1] / ((double)tuple_count[0][1] + (double)tuple_count[1][1]);
						bz_p_transition_virtual(1, 1) = (double)tuple_count[1][1] / ((double)tuple_count[0][1] + (double)tuple_count[1][1]);
					}
				}
				// -------------------------------------------------------------------------- //
				// 
				// -------------------------------------------------------------------------- //
				// -------------------------------------------------------------------------- //
				// evaluate number of significant digits based on p_prime_global
				// -------------------------------------------------------------------------- //
				PF_MARKOV_EE	funcs[14] = {
					calcNegativeLogarithmProbabilityC01,
					calcNegativeLogarithmProbabilityC02,
					calcNegativeLogarithmProbabilityC03,
					calcNegativeLogarithmProbabilityC04,
					calcNegativeLogarithmProbabilityC05,
					calcNegativeLogarithmProbabilityC06,
					calcNegativeLogarithmProbabilityC07,
					calcNegativeLogarithmProbabilityC08,
					calcNegativeLogarithmProbabilityC09,
					calcNegativeLogarithmProbabilityC10,
					calcNegativeLogarithmProbabilityC11,
					calcNegativeLogarithmProbabilityC12,
					calcNegativeLogarithmProbabilityC13,
					calcNegativeLogarithmProbabilityC14,
				};
				// -------------------------------------------------------------------------- //
				// 
				// -------------------------------------------------------------------------- //
				if ((0 <= io_refData.t_6_3_3.mostLikelySequenceCategoryIndex) && (io_refData.t_6_3_3.mostLikelySequenceCategoryIndex < 14))
				{
					double	min_entropy_lower_bound = 0.0;
					double	number_of_significant_digits = 1.0;

					ns_consts::EnmReturnStatus	stsNumSignificantDigits = funcs[io_refData.t_6_3_3.mostLikelySequenceCategoryIndex](min_entropy_lower_bound, bz_p_transition_virtual, bz_initial_p, lengthOfSequence);
					if (ns_consts::EnmReturnStatus::Success == stsNumSignificantDigits)
					{

						number_of_significant_digits = -log10(fabs((min_entropy_lower_bound - io_refData.t_6_3_3.t_common.min_entropy) / io_refData.t_6_3_3.t_common.min_entropy));

						io_refData.t_6_3_3.t_common.number_of_significant_digits = number_of_significant_digits;
						io_refData.t_6_3_3.t_common.min_entropy_lower_bound = min_entropy_lower_bound;
						io_refData.t_6_3_3.t_common.min_entropy_upper_bound = io_refData.t_6_3_3.t_common.min_entropy;
					}
				}
				// -------------------------------------------------------------------------- //
				// output LaTeX
				// -------------------------------------------------------------------------- //
				if (io_refData.isGeneratingReportInLaTeXformatRequested)
				{
					outputLaTeXFigures(io_refData);
				}
				// -------------------------------------------------------------------------- //
				// 
				// -------------------------------------------------------------------------- //
				return sts;
			}

		}
	}
}