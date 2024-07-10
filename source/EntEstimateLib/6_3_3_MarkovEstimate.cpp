////////////////////////////////////////////////////////////////////////////////
// 6_3_3_MarkovEstimate.cpp
//
//
//
// Copyright (c) 2021-2024 G. G. SAKURAI <g.garland823@gmail.com>
//
////////////////////////////////////////////////////////////////////////////////
#include "pch.h"
#include "6_3_3_MarkovEstimate.h"
#include "./support/checkArgs.h"
#include <boost/math/special_functions.hpp>
#include "./support/LaTeX.h"

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
				(*io_refData.p_ssLaTeXFragment) << L"\\begin{figure}[htbp]" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"\\begin{tikzpicture} " << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"\\begin{axis}[" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"\txlabel=$i$," << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"\tylabel=$P_{i,j}$," << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"	width=10cm," << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"	xmin=-0.125,xmax=1.125," << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"	xtick={0, 1}," << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"	legend style={at={(1,0.75)},anchor=north west}," << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"	/pgf/number format/.cd,fixed,precision=6," << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"	scatter/classes={%" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"		a={mark=square*,NPGskyblue}," << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"		b={mark=square*,NPGnavyblue}," << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"		c={mark=square*,NPGsalmon}," << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"		d={mark=square*,NPGred}}]" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"	\\addplot[scatter,only marks,%" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"		scatter src=explicit symbolic]%" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"	table[meta=label] {" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"x\ty\tlabel" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << std::setw(2) << 0 << L"\t" << std::setw(8) << io_refData.t_6_3_3.p_transition[0][0] << L"\ta" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << std::setw(2) << 0 << L"\t" << std::setw(8) << io_refData.t_6_3_3.p_transition[1][0] << L"\tb" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << std::setw(2) << 1 << L"\t" << std::setw(8) << io_refData.t_6_3_3.p_transition[0][1] << L"\tc" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << std::setw(2) << 1 << L"\t" << std::setw(8) << io_refData.t_6_3_3.p_transition[1][1] << L"\td" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"	};" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"\\legend{$P_{0,0}$, $P_{0,1}$, $P_{1,0}$, $P_{1,1}$}" << L"\n";
				// -------------------------------------------------------------------------- //
				// 
				// -------------------------------------------------------------------------- //
				(*io_refData.p_ssLaTeXFragment) << L"\\end{axis} " << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"\\end{tikzpicture}" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"\\caption{Transition probability $P_{i,j}$ of $\\S$6.3.3 of NIST SP 800-90B}" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"\\end{figure}" << L"\n";
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
				(*io_refData.p_ssLaTeXFragment) << L"\\clearpage" << L"\n";
				// -------------------------------------------------------------------------- //
				// prepend subsection 
				// -------------------------------------------------------------------------- //
				std::wstring	ssSubsection633 = std::wstring();
				std::wstring	ssSubsectionTitle633 = std::wstring(L"The Markov Estimate (NIST SP 800-90B Section 6.3.3)");

				ns_spt::getLaTeXSubsection(ssSubsection633, ssSubsectionTitle633);

				(*io_refData.p_ssLaTeXFragment) << ssSubsection633;
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				(*io_refData.p_ssLaTeXFragment) << L"\\label{sec:Binary633}" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"\n";
				// -------------------------------------------------------------------------- //
				// output transition probability P_{i,j}
				// -------------------------------------------------------------------------- //
				outputLaTeXFigure(io_refData);
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				(*io_refData.p_ssLaTeXFragment) << L"\\begin{figure}[htbp]" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"\\begin{tikzpicture} " << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"\\begin{axis}[" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"\txlabel=Sequence index," << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"\tylabel=$-\\log_{2}\\left ( \\textrm{Probability}\\right ) / 128$," << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"	width=18cm," << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"	xmin=0.5,xmax=14.5," << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"	legend style={at={(1,1)},anchor=north west}," << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"	/pgf/number format/.cd,fixed,precision=6," << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"	scatter/classes={%" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"		a={mark=triangle*,NPGskyblue}," << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"		b={mark=triangle*,NPGnavyblue}," << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"		c={mark=triangle,NPGnavyblue}," << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"		d={mark=triangle*,NPGred}," << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"		e={mark=triangle,NPGskyblue}," << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"		f={mark=square*,NPGnavyblue}," << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"		g={mark=triangle,NPGred}," << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"		h={mark=square*,NPGskyblue}," << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"		i={mark=square,NPGnavyblue}," << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"		j={mark=square*,NPGred}," << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"		k={mark=square,NPGskyblue}," << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"		l={mark=*,NPGnavyblue}," << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"		m={mark=o,NPGnavyblue}," << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"		n={mark=square,NPGred}}]" << L"\n";
				for (int j = 0; j < 14; ++j)
				{
					(*io_refData.p_ssLaTeXFragment) << L"	\\addplot[scatter,only marks,%" << L"\n";
					(*io_refData.p_ssLaTeXFragment) << L"		scatter src=explicit symbolic]%" << L"\n";
					(*io_refData.p_ssLaTeXFragment) << L"	table[meta=label] {" << L"\n";
					(*io_refData.p_ssLaTeXFragment) << L"x\ty\tlabel" << L"\n";
					(*io_refData.p_ssLaTeXFragment) << std::setw(2) << j + 1 << L"\t" << std::setw(8) << io_refData.t_6_3_3.negativeLogPoverLength[j];
					switch (j % 14)
					{
					case 0:
						(*io_refData.p_ssLaTeXFragment) << L"\ta" << L"\n";
						break;
					case 1:
						(*io_refData.p_ssLaTeXFragment) << L"\tb" << L"\n";
						break;
					case 2:
						(*io_refData.p_ssLaTeXFragment) << L"\tc" << L"\n";
						break;
					case 3:
						(*io_refData.p_ssLaTeXFragment) << L"\td" << L"\n";
						break;
					case 4:
						(*io_refData.p_ssLaTeXFragment) << L"\te" << L"\n";
						break;
					case 5:
						(*io_refData.p_ssLaTeXFragment) << L"\tf" << L"\n";
						break;
					case 6:
						(*io_refData.p_ssLaTeXFragment) << L"\tg" << L"\n";
						break;
					case 7:
						(*io_refData.p_ssLaTeXFragment) << L"\th" << L"\n";
						break;
					case 8:
						(*io_refData.p_ssLaTeXFragment) << L"\ti" << L"\n";
						break;
					case 9:
						(*io_refData.p_ssLaTeXFragment) << L"\tj" << L"\n";
						break;
					case 10:
						(*io_refData.p_ssLaTeXFragment) << L"\tk" << L"\n";
						break;
					case 11:
						(*io_refData.p_ssLaTeXFragment) << L"\tl" << L"\n";
						break;
					case 12:
						(*io_refData.p_ssLaTeXFragment) << L"\tm" << L"\n";
						break;
					case 13:
						(*io_refData.p_ssLaTeXFragment) << L"\tn" << L"\n";
						break;
					default:
						(*io_refData.p_ssLaTeXFragment) << L"\ta" << L"\n";
						break;
					}
					(*io_refData.p_ssLaTeXFragment) << L"	};" << L"\n";
				}
				(*io_refData.p_ssLaTeXFragment) << L"\\legend{$[$sequence index 1$]$ $0000 \\cdots 0000$, $[$sequence index 2$]$ $0101 \\cdots 0101001010 \\cdots 1010$, $[$sequence index 3$]$ $0101 \\cdots 0101101010 \\cdots 1010$, $[$sequence index 4$]$ $0111 \\cdots 1110$, $[$sequence index 5$]$ $0000 \\cdots 0001$, $[$sequence index 6$]$ $0101 \\cdots 0101$, $[$sequence index 7$]$ $0111 \\cdots 1111$, $[$sequence index 8$]$ $1000 \\cdots 0000$, $[$sequence index 9$]$ $1010 \\cdots 1010$, $[$sequence index 10$]$ $1111 \\cdots 1110$, $[$sequence index 11$]$ $1000 \\cdots 0001$, $[$sequence index 12$]$ $1010 \\cdots 1010100101 \\cdots 0101$, $[$sequence index 13$]$ $1010 \\cdots 1010110101 \\cdots 0101$, $[$sequence index 14$]$ $1111 \\cdots 1111$}" << L"\n";
				// -------------------------------------------------------------------------- //
				// 
				// -------------------------------------------------------------------------- //
				(*io_refData.p_ssLaTeXFragment) << L"\\end{axis} " << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"\\end{tikzpicture}" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"\\caption{Estimated Min-Entropy using $\\S$6.3.3 of NIST SP 800-90B}" << L"\n";
				(*io_refData.p_ssLaTeXFragment) << L"\\end{figure}" << L"\n";
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
			///   Note here that this matrix is defined to operate on the column vector from left.
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
				o_refMinEntropyEstimate = -((static_cast<double>(i_lengthOfSeq - 1) / static_cast<double>(i_lengthOfSeq)) * log2(i_refPTransition(0, 0))
					+ log2(i_refInitialP(0)) / static_cast<double>(i_lengthOfSeq));
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
			///   Note here that this matrix is defined to operate on the column vector from left.
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
				o_refMinEntropyEstimate = - (0.5 * (static_cast<double>(i_lengthOfSeq - 2) / static_cast<double>(i_lengthOfSeq)) * log2(i_refPTransition(0, 1) * i_refPTransition(1, 0))
					+ log2(i_refInitialP(0) * i_refPTransition(0, 0)) / static_cast<double>(i_lengthOfSeq));
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
			///   Note here that this matrix is defined to operate on the column vector from left.
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
				o_refMinEntropyEstimate = - (0.5 * (static_cast<double>(i_lengthOfSeq - 2) / static_cast<double>(i_lengthOfSeq)) * log2(i_refPTransition(0, 1) * i_refPTransition(1, 0))
					+ log2(i_refInitialP(0) * i_refPTransition(1, 1)) / static_cast<double>(i_lengthOfSeq));
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
			///   Note here that this matrix is defined to operate on the column vector from left.
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
				o_refMinEntropyEstimate = -((static_cast<double>(i_lengthOfSeq - 3) / static_cast<double>(i_lengthOfSeq)) * log2(i_refPTransition(1, 1))
					+ log2(i_refInitialP(0) * i_refPTransition(1, 0) * i_refPTransition(0, 1)) / static_cast<double>(i_lengthOfSeq));
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
			///   Note here that this matrix is defined to operate on the column vector from left.
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
				o_refMinEntropyEstimate = -((static_cast<double>(i_lengthOfSeq - 2) / static_cast<double>(i_lengthOfSeq)) * log2(i_refPTransition(0, 0))
					+ log2(i_refInitialP(0) * i_refPTransition(1, 0)) / static_cast<double>(i_lengthOfSeq));
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
			///   Note here that this matrix is defined to operate on the column vector from left.
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
				o_refMinEntropyEstimate = - (0.5 * (static_cast<double>(i_lengthOfSeq - 2) / static_cast<double>(i_lengthOfSeq)) * log2(i_refPTransition(0, 1) * i_refPTransition(1, 0))
					+ log2(i_refInitialP(0) * i_refPTransition(1, 0)) / static_cast<double>(i_lengthOfSeq));
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
			///   Note here that this matrix is defined to operate on the column vector from left.
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
				o_refMinEntropyEstimate = - ((static_cast<double>(i_lengthOfSeq - 2) / static_cast<double>(i_lengthOfSeq)) * log2(i_refPTransition(1, 1))
					+ log2(i_refInitialP(0) * i_refPTransition(1, 0)) / static_cast<double>(i_lengthOfSeq));
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
			///   Note here that this matrix is defined to operate on the column vector from left.
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
				o_refMinEntropyEstimate = - ((static_cast<double>(i_lengthOfSeq - 2) / static_cast<double>(i_lengthOfSeq)) * log2(i_refPTransition(0, 0))
					+ log2(i_refInitialP(1) * i_refPTransition(0, 1)) / static_cast<double>(i_lengthOfSeq));
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
			///   Note here that this matrix is defined to operate on the column vector from left.
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
				o_refMinEntropyEstimate = - (0.5 * (static_cast<double>(i_lengthOfSeq - 2) / static_cast<double>(i_lengthOfSeq)) * log2(i_refPTransition(0, 1) * i_refPTransition(1, 0))
					+ log2(i_refInitialP(1) * i_refPTransition(0, 1)) / static_cast<double>(i_lengthOfSeq));
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
			///   Note here that this matrix is defined to operate on the column vector from left.
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
				o_refMinEntropyEstimate = - ((static_cast<double>(i_lengthOfSeq - 2) / static_cast<double>(i_lengthOfSeq)) * log2(i_refPTransition(1, 1))
					+ log2(i_refInitialP(1) * i_refPTransition(0, 1)) / static_cast<double>(i_lengthOfSeq));
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
			///   Note here that this matrix is defined to operate on the column vector from left.
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
				o_refMinEntropyEstimate = - ((static_cast<double>(i_lengthOfSeq - 3) / static_cast<double>(i_lengthOfSeq)) * log2(i_refPTransition(0, 0))
					+ log2(i_refInitialP(1) * i_refPTransition(0, 1) * i_refPTransition(1, 0)) / static_cast<double>(i_lengthOfSeq));
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
			///   Note here that this matrix is defined to operate on the column vector from left.
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
				o_refMinEntropyEstimate = - (0.5 * (static_cast<double>(i_lengthOfSeq - 2) / static_cast<double>(i_lengthOfSeq)) * log2(i_refPTransition(0, 1) * i_refPTransition(1, 0))
					+ log2(i_refInitialP(1) * i_refPTransition(0, 0)) / static_cast<double>(i_lengthOfSeq));
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
			///   Note here that this matrix is defined to operate on the column vector from left.
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
				o_refMinEntropyEstimate = - (0.5 * (static_cast<double>(i_lengthOfSeq - 2) / static_cast<double>(i_lengthOfSeq)) * log2(i_refPTransition(0, 1) * i_refPTransition(1, 0))
					+ log2(i_refInitialP(1) * i_refPTransition(1, 1)) / static_cast<double>(i_lengthOfSeq));
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
			///   Note here that this matrix is defined to operate on the column vector from left.
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
				o_refMinEntropyEstimate = - ((static_cast<double>(i_lengthOfSeq - 1) / static_cast<double>(i_lengthOfSeq)) * log2(i_refPTransition(1, 1))
					+ log2(i_refInitialP(1)) / static_cast<double>(i_lengthOfSeq));
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
			///   Note here that this matrix is defined to operate on the column vector from left.
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
				ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorInvalidData;

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
					return sts;
				}
				if (i_refPTransition.length(blitz::secondDim) != 2)
				{
					return sts;
				}
				if (i_refInitialP.length(blitz::firstDim) != 2)
				{
					return sts;
				}

				//blitz::Array<double, 1>	bz_log_p_over_L(14);
				//bz_log_p_over_L = 0.0;

				double	minEntropyForCases[14] = { 0.0 };
				io_refData.t_common.min_entropy = 1.0;
				for (int i = 0; i < 14; ++i)
				{
					const ns_consts::EnmReturnStatus	stsForCase
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
				if (i_refData.L < 3)
				{
					return sts;
				}
				if (0 == (*i_refData.p_bzSampleSpaceA)(0))
				{
					if (1 != (*i_refData.p_bzSampleSpaceA)(1))
					{
						return sts;
					}
				}
				else if (1 == (*i_refData.p_bzSampleSpaceA)(0))
				{
					if (0 != (*i_refData.p_bzSampleSpaceA)(1))
					{
						return sts;
					}
				}
				else
				{
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
					bz_initial_p(0) = bz_count(0) / static_cast<double>(io_refData.p_bzInputS->length(blitz::firstDim));
					bz_initial_p(1) = 1.0 - bz_initial_p(0);
				}
				else
				{
					bz_initial_p(1) = bz_count(0) / static_cast<double>(io_refData.p_bzInputS->length(blitz::firstDim));
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
				bz_p_transition(0, 0) = static_cast<double>(tuple_count[0][0]) / static_cast<double>(tuple_count[0][0] + tuple_count[1][0]);
				bz_p_transition(0, 1) = static_cast<double>(tuple_count[0][1]) / static_cast<double>(tuple_count[0][1] + tuple_count[1][1]);
				bz_p_transition(1, 0) = static_cast<double>(tuple_count[1][0]) / static_cast<double>(tuple_count[0][0] + tuple_count[1][0]);
				bz_p_transition(1, 1) = static_cast<double>(tuple_count[1][1]) / static_cast<double>(tuple_count[0][1] + tuple_count[1][1]);

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
				constexpr  int lengthOfSequence = 128;
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
						bz_p_transition_virtual(0, 0) = (static_cast<double>(tuple_count[0][0]) - 1.0) / static_cast<double>(tuple_count[0][0] + tuple_count[1][0]);
						bz_p_transition_virtual(1, 0) = (static_cast<double>(tuple_count[1][0]) + 1.0) / static_cast<double>(tuple_count[0][0] + tuple_count[1][0]);
					}
					else
					{
						bz_p_transition_virtual(0, 0) = static_cast<double>(tuple_count[0][0]) / static_cast<double>(tuple_count[0][0] + tuple_count[1][0]);
						bz_p_transition_virtual(1, 0) = static_cast<double>(tuple_count[1][0]) / static_cast<double>(tuple_count[0][0] + tuple_count[1][0]);
					}
				}
				else
				{
					if (1 <= tuple_count[1][0])
					{
						bz_p_transition_virtual(0, 0) = (static_cast<double>(tuple_count[0][0]) + 1.0) / static_cast<double>(tuple_count[0][0] + tuple_count[1][0]);
						bz_p_transition_virtual(1, 0) = (static_cast<double>(tuple_count[1][0]) - 1.0) / static_cast<double>(tuple_count[0][0] + tuple_count[1][0]);
					}
					else
					{
						bz_p_transition_virtual(0, 0) = static_cast<double>(tuple_count[0][0]) / static_cast<double>(tuple_count[0][0] + tuple_count[1][0]);
						bz_p_transition_virtual(1, 0) = static_cast<double>(tuple_count[1][0]) / static_cast<double>(tuple_count[0][0] + tuple_count[1][0]);
					}
				}

				if (tuple_count[1][1] < tuple_count[0][1])
				{
					if (1 <= tuple_count[1][1])
					{
						bz_p_transition_virtual(0, 1) = (static_cast<double>(tuple_count[0][1]) + 1.0) / static_cast<double>(tuple_count[0][1] + tuple_count[1][1]);
						bz_p_transition_virtual(1, 1) = (static_cast<double>(tuple_count[1][1]) - 1.0) / static_cast<double>(tuple_count[0][1] + tuple_count[1][1]);
					}
					else
					{
						bz_p_transition_virtual(0, 1) = static_cast<double>(tuple_count[0][1]) / static_cast<double>(tuple_count[0][1] + tuple_count[1][1]);
						bz_p_transition_virtual(1, 1) = static_cast<double>(tuple_count[1][1]) / static_cast<double>(tuple_count[0][1] + tuple_count[1][1]);
					}
				}
				else
				{
					if (1 <= tuple_count[0][1])
					{
						bz_p_transition_virtual(0, 1) = (static_cast<double>(tuple_count[0][1]) - 1.0) / static_cast<double>(tuple_count[0][1] + tuple_count[1][1]);
						bz_p_transition_virtual(1, 1) = (static_cast<double>(tuple_count[1][1]) + 1.0) / static_cast<double>(tuple_count[0][1] + tuple_count[1][1]);
					}
					else
					{
						bz_p_transition_virtual(0, 1) = static_cast<double>(tuple_count[0][1]) / static_cast<double>(tuple_count[0][1] + tuple_count[1][1]);
						bz_p_transition_virtual(1, 1) = static_cast<double>(tuple_count[1][1]) / static_cast<double>(tuple_count[0][1] + tuple_count[1][1]);
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

					const ns_consts::EnmReturnStatus	stsNumSignificantDigits = funcs[io_refData.t_6_3_3.mostLikelySequenceCategoryIndex](min_entropy_lower_bound, bz_p_transition_virtual, bz_initial_p, lengthOfSequence);
					if (ns_consts::EnmReturnStatus::Success == stsNumSignificantDigits)
					{
						double	number_of_significant_digits = 1.0;

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