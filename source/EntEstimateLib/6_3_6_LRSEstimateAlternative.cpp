////////////////////////////////////////////////////////////////////////////////
// 6_3_6_LRSEstimateAlternative.cpp
//
//
//
// Copyright (c) 2025 G. G. SAKURAI <g.garland823@gmail.com>
//
////////////////////////////////////////////////////////////////////////////////
#include "6_3_6_LRSEstimateAlternative.h"
#include "./support/checkArgs.h"
#include "./support/conversion.h"
#include "./math/SpecialFunctions.h"
#include <boost/math/special_functions.hpp>
#include "./support/TupleHistogram.h"
#include "./support/LongestCommonPrefix.h"
#include "./support/LaTeX.h"
#include <boost/range/iterator_range_core.hpp>

namespace entropy_estimator_lib
{
	namespace estimators
	{
		namespace lrs_alternative
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
			/// <param name="io_refData">
			/// </param>
			/// <param name="extentSecondDim">
			/// </param>
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
				// -------------------------------------------------------------------------- //
				// prepend subsection 
				// -------------------------------------------------------------------------- //
				std::wstring	strSubsection636 = std::wstring();
				std::wstring	strSubsectionTitle636 = std::wstring(L"The LRS Estimate (NIST SP 800-90B Section 6.3.6)");
				std::wstring	strLabel = std::wstring();
				switch (io_refData.bits_per_sample)
				{
				case 1:
					strLabel = std::wstring(L"sec:Binary636");
					break;
				default:
					strLabel = std::wstring(L"sec:NonBinary636");
					break;
				}
				ns_spt::getLaTeXSubsection(strSubsection636, strSubsectionTitle636, strLabel);
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				(*io_refData.p_ssLaTeXFragment) << strSubsection636;
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
			/// <param name="io_refData">
			/// </param>
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
				(*io_refData.p_ssLaTeXFragment) << L"	ylabel=$\\left( P_W \\right) ^{1/W}$," << L"\n";
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
			/// <param name="io_refData">
			/// </param>
			/// <param name="i_refCaption">
			/// </param>
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
			/// <param name="io_refData">
			/// </param>
			/// <param name="i_refCaption">
			/// </param>
			/// <param name="argMax">
			/// </param>
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
			/// <param name="io_refData">
			/// </param>
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
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				std::wstring	strSubsubsectionTraceability = std::wstring();
				std::wstring	strLabel = std::wstring();
				switch (io_refData.bits_per_sample)
				{
				case 1:
					strLabel = std::wstring(L"sec:Binary636-traceability");
					break;
				default:
					strLabel = std::wstring(L"sec:NonBinary636-traceability");
					break;
				}
				ns_spt::getLaTeXSubsubsectionTraceability(strSubsubsectionTraceability, strLabel);
				(*io_refData.p_ssLaTeXFragment) << strSubsubsectionTraceability;
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
			/// </summary>
			/// <remarks>
			/// </remarks>
			/// <param name="io_refLRSdata">
			/// </param>
			/// <returns>
			///  <c>entropy_estimator_lib::constants::EnmReturnStatus::ErrorNullPointer</c>:  when the following condition is met:
			///    <ul>
			///     <li><c>io_refLRSdata.p_LCP</c> == nullptr</li>
			///    </ul>
			///  <c>entropy_estimator_lib::constants::EnmReturnStatus::Success</c>:  otherwise.
			/// </returns>
			/// <precondition>
			/// </precondition>
			/// <postcondition>
			/// </postcondition>
			// -------------------------------------------------------------------------- //
			ns_consts::EnmReturnStatus  SetUpArrayQTilde(ns_es::t_data_for_LRS& io_refLRSdata)
			{
				ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorNullPointer;

				if (nullptr == io_refLRSdata.p_LCP)
				{
					return sts;
				}
				// -------------------------------------------------------------------------- //
				// Step 2
				// 
				// -------------------------------------------------------------------------- //
				io_refLRSdata.eta_max = (io_refLRSdata.p_LCP->max());
				// -------------------------------------------------------------------------- //
				// Step 3
				// -------------------------------------------------------------------------- //
				if (io_refLRSdata.eta_max < 1)
				{
					return sts = ns_consts::EnmReturnStatus::ErrorInvalidData;
				}
				io_refLRSdata.nu = io_refLRSdata.eta_max;
				// -------------------------------------------------------------------------- //
				// Steps 3-6
				// 
				// -------------------------------------------------------------------------- //
				io_refLRSdata.p_qtilde_master = new std::valarray<uint32_t>((uint32_t)0, io_refLRSdata.nu);
				io_refLRSdata.p_qtilde_work = new std::valarray<uint32_t>((uint32_t)0, io_refLRSdata.nu);
				// -------------------------------------------------------------------------- //
				// Steps 3-
				// 
				// -------------------------------------------------------------------------- //
				io_refLRSdata.p_sigma_dbl = new std::valarray<double>((double)0, io_refLRSdata.nu);
				// -------------------------------------------------------------------------- //
				// set up histogram for LCP array
				// -------------------------------------------------------------------------- //
				io_refLRSdata.p_hg = new ns_es::LCPHistogram();

				return sts = ns_consts::EnmReturnStatus::Success;
			}

			// -------------------------------------------------------------------------- //
			/// <summary>
			/// </summary>
			/// <remarks>
			/// </remarks>
			/// <param name="io_refLRSdata">
			/// </param>
			/// <returns>
			/// </returns>
			/// <precondition>
			/// </precondition>
			/// <postcondition>
			/// </postcondition>
			// -------------------------------------------------------------------------- //
			void TearDownArrayQTilde(ns_es::t_data_for_LRS& io_refLRSdata)
			{
				// -------------------------------------------------------------------------- //
				// dispose internally allocated array
				// -------------------------------------------------------------------------- //
				if (nullptr != io_refLRSdata.p_hg)
				{
					delete io_refLRSdata.p_hg;
					io_refLRSdata.p_hg = nullptr;
				}
				// -------------------------------------------------------------------------- //
				// dispose internally allocated array
				// -------------------------------------------------------------------------- //
				if (nullptr != io_refLRSdata.p_sigma_dbl)
				{
					delete io_refLRSdata.p_sigma_dbl;
					io_refLRSdata.p_sigma_dbl = nullptr;
				}
				// -------------------------------------------------------------------------- //
				// dispose internally allocated array
				// -------------------------------------------------------------------------- //
				delete io_refLRSdata.p_qtilde_work;
				io_refLRSdata.p_qtilde_work = nullptr;
				// -------------------------------------------------------------------------- //
				// dispose internally allocated array
				// -------------------------------------------------------------------------- //
				delete io_refLRSdata.p_qtilde_master;
				io_refLRSdata.p_qtilde_master = nullptr;
			}

			// -------------------------------------------------------------------------- //
			/// <summary>
			/// </summary>
			/// <remarks>
			/// </remarks>
			/// <param name="io_refLRSdata">
			/// </param>
			/// <returns>
			/// </returns>
			/// <precondition>
			/// </precondition>
			/// <postcondition>
			/// </postcondition>
			// -------------------------------------------------------------------------- //
			ns_consts::EnmReturnStatus   AccumulateLambda(ns_es::t_data_for_LRS& io_refLRSdata)
			{
				ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorNullPointer;

				if (nullptr == io_refLRSdata.p_qtilde_work)
				{
					return sts;
				}

				int eta_prev_minus_one = 0;
				if (0 < io_refLRSdata.eta_prev - 1)
				{
					eta_prev_minus_one = io_refLRSdata.eta_prev - 1;
				}
				(*io_refLRSdata.p_qtilde_work)[eta_prev_minus_one] = io_refLRSdata.lambda;

				int lower_bound = io_refLRSdata.eta_current;
				if (lower_bound < 1)
				{
					lower_bound = 1;
				}

				for (int eta = (io_refLRSdata.eta_prev - 1); eta >= lower_bound; eta--)
				{
					int eta_minus_one = eta - 1;
					(*io_refLRSdata.p_qtilde_work)[eta_minus_one] += (*io_refLRSdata.p_qtilde_work)[eta_minus_one + 1];
				}

				return sts = ns_consts::EnmReturnStatus::Success;
			}

			// -------------------------------------------------------------------------- //
			/// <summary>
			/// </summary>
			/// <remarks>
			/// </remarks>
			/// <param name="io_refLRSdata">
			/// </param>
			/// <returns>
			/// </returns>
			/// <precondition>
			/// </precondition>
			/// <postcondition>
			/// </postcondition>
			// -------------------------------------------------------------------------- //
			ns_consts::EnmReturnStatus	UpdateLambda(ns_es::t_data_for_LRS& io_refLRSdata)
			{
				ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorNullPointer;

				if (nullptr == io_refLRSdata.p_qtilde_work)
				{
					return sts;
				}

				if (io_refLRSdata.eta_current == 0)
				{
					io_refLRSdata.lambda = 1;
				}
				else
				{
					int eta_minus_one = io_refLRSdata.eta_current - 1;
					io_refLRSdata.lambda = (*io_refLRSdata.p_qtilde_work)[eta_minus_one] + 1;
				}

				return sts = ns_consts::EnmReturnStatus::Success;
			}

			// -------------------------------------------------------------------------- //
			/// <summary>
			/// </summary>
			/// <remarks>
			/// </remarks>
			/// <param name="io_refLRSdata">
			/// </param>
			/// <returns>
			/// </returns>
			/// <precondition>
			/// </precondition>
			/// <postcondition>
			/// </postcondition>
			// -------------------------------------------------------------------------- //
			ns_consts::EnmReturnStatus	UpdateQTildeMaster(ns_es::t_data_for_LRS& io_refLRSdata)
			{
				ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorNullPointer;

				if (nullptr == io_refLRSdata.p_qtilde_master)
				{
					return sts;
				}

				if (nullptr == io_refLRSdata.p_qtilde_work)
				{
					return sts;
				}

				for (int eta = io_refLRSdata.eta_prev; eta >= (io_refLRSdata.eta_current + 1); eta--)
				{
					int eta_minus_one = eta - 1;
					if ((*io_refLRSdata.p_qtilde_master)[eta_minus_one] < (*io_refLRSdata.p_qtilde_work)[eta_minus_one])
					{
						(*io_refLRSdata.p_qtilde_master)[eta_minus_one] = (*io_refLRSdata.p_qtilde_work)[eta_minus_one];
					}
				}

				return sts = ns_consts::EnmReturnStatus::Success;
			}

			// -------------------------------------------------------------------------- //
			/// <summary>
			/// </summary>
			/// <remarks>
			/// </remarks>
			/// <param name="io_refLRSdata">
			/// </param>
			/// <returns>
			/// </returns>
			/// <precondition>
			/// </precondition>
			/// <postcondition>
			/// </postcondition>
			// -------------------------------------------------------------------------- //
			ns_consts::EnmReturnStatus	ResetQTildeWork(ns_es::t_data_for_LRS& io_refLRSdata)
			{
				ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorNullPointer;

				if (nullptr == io_refLRSdata.p_qtilde_work)
				{
					return sts;
				}

				int lower_bound = io_refLRSdata.eta_current;
				if (lower_bound < 1)
				{
					lower_bound = 1;
				}

				for (int eta = io_refLRSdata.eta_prev; eta >= lower_bound; eta--)
				{
					int eta_minus_one = eta - 1;
					(*io_refLRSdata.p_qtilde_work)[eta_minus_one] = 0;
				}

				return sts = ns_consts::EnmReturnStatus::Success;
			}

			// -------------------------------------------------------------------------- //
			/// <summary>
			/// </summary>
			/// <remarks>
			/// </remarks>
			/// <param name="o_ref_bz_Q">
			/// </param>
			/// <param name="io_refLRSdata">
			/// </param>
			/// <returns>
			/// </returns>
			/// <precondition>
			/// </precondition>
			/// <postcondition>
			/// </postcondition>
			// -------------------------------------------------------------------------- //
			ns_consts::EnmReturnStatus UpdateNumerator(ns_es::t_data_for_LRS& io_refLRSdata)
			{
				ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorNullPointer;

				if (nullptr == io_refLRSdata.p_qtilde_work)
				{
					return sts;
				}

				if (nullptr == io_refLRSdata.p_sigma_dbl)
				{
					return sts;
				}

				if (nullptr == io_refLRSdata.p_hg)
				{
					return sts;
				}

				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				ns_es::lcp_idx_map& w_map_hg = io_refLRSdata.p_hg->get<ns_es::lcp_idx>();
				// -------------------------------------------------------------------------- //
				// Step 2
				//   for $\eta \gets \eta_{\textrm{prev}} \Downto \textrm{\textbf{max}}(\eta_{\textrm{current}}, 1) $
				// -------------------------------------------------------------------------- //
				for (int eta = io_refLRSdata.eta_prev; eta >= (io_refLRSdata.eta_current + 1); eta--)
				{
					int eta_minus_one = eta - 1;
					// -------------------------------------------------------------------------- //
					// number of occurrences
					// -------------------------------------------------------------------------- //
					const int c_i = 1 + (*io_refLRSdata.p_qtilde_work)[eta_minus_one];
					// -------------------------------------------------------------------------- //
					// Step 3
					//   if {$\theta_{\textrm{work}}[\eta] + 1 >= 2$}
					// -------------------------------------------------------------------------- //
					if (c_i < 2)
					{
						continue;
					}
					// -------------------------------------------------------------------------- //
					// Step 4
					// $\sigma[\eta] \gets \sigma[\eta] + \binom{\tilde{q}_{\textrm{work}}[\eta] + 1}{2}$
					// -------------------------------------------------------------------------- //
					(*io_refLRSdata.p_sigma_dbl)[eta_minus_one] += boost::math::binomial_coefficient<double>(c_i, 2);
					// -------------------------------------------------------------------------- //
					//
					// -------------------------------------------------------------------------- //
					bool	found = false;
					// -------------------------------------------------------------------------- //
					//
					// -------------------------------------------------------------------------- //
					const std::pair<ns_es::lcp_idx_map::iterator, ns_es::lcp_idx_map::iterator> rg_x = w_map_hg.equal_range(eta);
					if (rg_x.first != rg_x.second)
					{
						for (ns_es::lcp_idx_map::iterator it = rg_x.first; it != rg_x.second; ++it)
						{
							// -------------------------------------------------------------------------- //
							// Find a record where the number of occurrences is equal to
							// $\tilde{q}_{\textrm{work}}[\eta] + 1$.
							// Note here that c_i = $\theta_{\textrm{work}}[\eta] + 1$
							// -------------------------------------------------------------------------- //
							if (it->ex_num_occ == c_i)
							{
								const uint32_t new_multiplicity = 1 + (it->ex_multi);
								// -------------------------------------------------------------------------- //
								// eta corresponds to W
								// c_i corresponds to number of occurrences
								// -------------------------------------------------------------------------- //
								w_map_hg.replace(it, ns_es::t_lcp_bin(eta, c_i, new_multiplicity));
								sts = ns_consts::EnmReturnStatus::Success;

								found = true;
								break;
							}
						}
					}
					if (false == found)
					{
						// -------------------------------------------------------------------------- //
						// eta corresponds to W
						// c_i corresponds to number of occurrences
						// -------------------------------------------------------------------------- //
						w_map_hg.insert(ns_es::t_lcp_bin(eta, c_i, 1));
					}
				}

				return sts = ns_consts::EnmReturnStatus::Success;
			}

			// -------------------------------------------------------------------------- //
			/// <summary>
			/// </summary>
			/// <remarks>
			/// </remarks>
			/// <param name="io_refData">
			/// </param>
			/// <param name="io_refLRSdata">
			/// </param>
			/// <param name="io_refSSFragmentForLaTeXPmax">
			/// </param>
			/// <returns>
			/// </returns>
			/// <precondition>
			/// </precondition>
			/// <postcondition>
			/// </postcondition>
			// -------------------------------------------------------------------------- //
			ns_consts::EnmReturnStatus CalcPHat(ns_dt::t_data_for_estimator& io_refData, ns_es::t_data_for_LRS& io_refLRSdata,
				std::wstringstream& io_refSSFragmentForLaTeXPmax)
			{
				ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorNullPointer;

				if (nullptr == io_refLRSdata.p_sigma_dbl)
				{
					return sts;
				}

				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				blitz::Array<double, 1>	bz_p(io_refData.t_6_3_6.nu - io_refData.t_6_3_6.u + 1);
				bz_p = 0.0;

				const int L = io_refData.p_bzInputS->length(blitz::firstDim);
				for (int w = io_refData.t_6_3_6.u; w <= io_refData.t_6_3_6.nu; ++w)
				{
					int lhs_idx = w - io_refData.t_6_3_6.u;
					int rhs_idx = w - 1;
					int	LminusWplusOne = L + 1 - w;
					bz_p(lhs_idx)= ((*io_refLRSdata.p_sigma_dbl)[rhs_idx]) / boost::math::binomial_coefficient<double>(LminusWplusOne, 2);
				}

				// -------------------------------------------------------------------------- //
				// Compute the estimated average collision probability per string symbol as 
				// $P_{max, W} = P_{W}^{1/W}$.
				// Let $\hat{p} = \max(P_{max,u}, \ldots, P_{max,\nu})$
				// -------------------------------------------------------------------------- //
				blitz::Array<double, 1>	bz_P_max(bz_p.length(blitz::firstDim));
				bz_P_max = 0.0;
				for (int j = 0; j < bz_p.length(blitz::firstDim); ++j)
				{
					// -------------------------------------------------------------------------- //
					// Here, W = u + j, because offset u must be taken into account.
					// -------------------------------------------------------------------------- //
					bz_P_max(j) = pow(bz_p(j), 1.0 / static_cast<double>(j + io_refData.t_6_3_6.u));
				}
				io_refData.t_6_3_6.p_hat = blitz::max(bz_P_max);
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				blitz::TinyVector<int, 1>	bzMaxIndex = blitz::maxIndex(bz_P_max);
				io_refData.t_6_3_6.argmax_p_hat = bzMaxIndex(0) + io_refData.t_6_3_6.u;
				// -------------------------------------------------------------------------- //
				// LaTeX report generation steps
				// -------------------------------------------------------------------------- //
				if (true == io_refData.isGeneratingReportInLaTeXformatRequested)
				{
					// -------------------------------------------------------------------------- //
					//
					// -------------------------------------------------------------------------- //
					for (int w = io_refData.t_6_3_6.u; w <= io_refData.t_6_3_6.nu; ++w)
					{
						// -------------------------------------------------------------------------- //
						// LaTeX output for $P_{max}$
						// -------------------------------------------------------------------------- //
						double	P_max_W = bz_P_max(w - io_refData.t_6_3_6.u);
						io_refSSFragmentForLaTeXPmax << L"(";
						io_refSSFragmentForLaTeXPmax << std::setw(4) << w;
						io_refSSFragmentForLaTeXPmax << L", " << std::setw(8) << P_max_W;
						io_refSSFragmentForLaTeXPmax << L")" << L"\n";
					}
				}

				return sts = ns_consts::EnmReturnStatus::Success;
			}

			// -------------------------------------------------------------------------- //
			/// <summary>
			/// </summary>
			/// <remarks>
			/// </remarks>
			/// <param name="io_refData">
			/// </param>
			/// <param name="io_refLRSdata">
			/// </param>
			/// <param name="i_refSSFragmentForLaTeXPmax">
			/// </param>
			/// <returns>
			/// </returns>
			/// <precondition>
			/// </precondition>
			/// <postcondition>
			/// </postcondition>
			// -------------------------------------------------------------------------- //
			ns_consts::EnmReturnStatus outputLaTeXDistribution(ns_dt::t_data_for_estimator& io_refData, ns_es::t_data_for_LRS& io_refLRSdata,
				std::wstringstream& i_refSSFragmentForLaTeXPmax)
			{
				ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorNullPointer;

				if (nullptr == io_refLRSdata.p_hg)
				{
					return sts;
				}

				if (false == io_refData.isGeneratingReportInLaTeXformatRequested)
				{
					// -------------------------------------------------------------------------- //
					// if LaTeX report generation is not requested, skip the following steps.
					// -------------------------------------------------------------------------- //
					return sts = ns_consts::EnmReturnStatus::Success;
				}
				// -------------------------------------------------------------------------- //
				// for LaTeX output
				// -------------------------------------------------------------------------- //
				std::wstringstream	ssFragmentForLaTeX;
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				ns_es::lcp_idx_map& w_map_hg = io_refLRSdata.p_hg->get<ns_es::lcp_idx>();
				// -------------------------------------------------------------------------- //
				// remove items from <c>io_refLRSdata.p_hg</c> irrelevant for distribution
				// -------------------------------------------------------------------------- //
				for (int w = 1; w < io_refData.t_6_3_6.u; ++w)
				{
					w_map_hg.erase(w);
				}
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				ns_es::lcp_cnt_map& num_occ_map_hg = io_refLRSdata.p_hg->get<ns_es::lcp_cnt>();
				ns_es::lcp_cnt_map::reverse_iterator	rit_ci_max = num_occ_map_hg.rbegin();
				// -------------------------------------------------------------------------- //
				// note here that preceding step of erase is important to calculate <c>global_Ci_max</c>
				// -------------------------------------------------------------------------- //
				const int	global_Ci_max = rit_ci_max->ex_num_occ;
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				blitz::Array<uint32_t, 1>		bz_dist(global_Ci_max);
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				for (int w = io_refData.t_6_3_6.u; w <= io_refData.t_6_3_6.nu; ++w)
				{
					bz_dist = 0;

					const std::pair<ns_es::lcp_idx_map::iterator, ns_es::lcp_idx_map::iterator> rg_x = w_map_hg.equal_range(w);
					if (rg_x.first != rg_x.second)
					{
						for (ns_es::lcp_idx_map::iterator it = rg_x.first; it != rg_x.second; ++it)
						{
							bz_dist(it->ex_num_occ - 1) = it->ex_multi;
						}
					}

					for (int y = 1; y < global_Ci_max; ++y)
					{
						// -------------------------------------------------------------------------- //
						//
						// -------------------------------------------------------------------------- //
						ssFragmentForLaTeX << L"(";
						// -------------------------------------------------------------------------- //
						// w
						// -------------------------------------------------------------------------- //
						ssFragmentForLaTeX << std::setw(4) << w;
						// -------------------------------------------------------------------------- //
						// number of occurrences
						// -------------------------------------------------------------------------- //
						ssFragmentForLaTeX << L"," << std::setw(4) << (y + 1);
						// -------------------------------------------------------------------------- //
						// multiplicity
						// -------------------------------------------------------------------------- //
						ssFragmentForLaTeX << L"," << std::setw(8) << bz_dist(y);
						ssFragmentForLaTeX << L")  ";
					}
					ssFragmentForLaTeX << L"\n" << L"\n";
				}
				// -------------------------------------------------------------------------- //
				// output LaTeX
				// -------------------------------------------------------------------------- //
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
					(*io_refData.p_ssLaTeXFragment) << i_refSSFragmentForLaTeXPmax.rdbuf();
				}

				return sts = ns_consts::EnmReturnStatus::Success;
			}

			// -------------------------------------------------------------------------- //
			/// <summary>
			///  Implements steps 1 to 3 of 6.3.6 of NIST SP 800-90B.
			/// </summary>
			/// <remarks>
			/// </remarks>
			/// <param name="io_refData">
			/// </param>
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
			ns_consts::EnmReturnStatus steps123(ns_dt::t_data_for_estimator& io_refData)
			{
				ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorUnexpected;

				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				std::valarray<ns_dt::octet>		valT(io_refData.L);
				std::valarray<uint32_t>			vaSA((uint32_t)0, io_refData.L);
				std::valarray<uint32_t>			vaLCP((uint32_t)0, io_refData.L);

				for (int j = 0; j < io_refData.p_bzInputS->length(blitz::firstDim); ++j)
				{
					valT[j] = (*io_refData.p_bzInputS)(j);
				}
				// -------------------------------------------------------------------------- //
				// Step 1
				//   Compute the Longest Common Prefix array LCP[j](j \in {1,2,...,L}) from S
				// -------------------------------------------------------------------------- //
				ns_consts::EnmReturnStatus	stsSA = ns_es::ComputeSuffixArray(vaSA, valT, io_refData.L);
				if (ns_consts::EnmReturnStatus::Success != stsSA)
				{
					return sts = stsSA;
				}

				ns_consts::EnmReturnStatus	stsLCP = ns_es::ComputeLCP(vaLCP, valT, vaSA, io_refData.L);
				if (ns_consts::EnmReturnStatus::Success != stsLCP)
				{
					return sts = stsLCP;
				}

				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				ns_es::t_data_for_LRS	lrs_data;
				lrs_data.p_qtilde_master = nullptr;
				lrs_data.p_qtilde_work = nullptr;
				lrs_data.p_sigma_dbl = nullptr;
				lrs_data.p_LCP = &vaLCP;

				// -------------------------------------------------------------------------- //
				// Steps 2 - 12
				// -------------------------------------------------------------------------- //
				ns_consts::EnmReturnStatus	stsSetUp = SetUpArrayQTilde(lrs_data);
				if (ns_consts::EnmReturnStatus::Success != stsSetUp)
				{
					return sts = stsSetUp;
				}

				// -------------------------------------------------------------------------- //
				// Step 12  $\eta_{\textrm{prev}} \gets LCP[1]$
				// -------------------------------------------------------------------------- //
				lrs_data.eta_prev = (*lrs_data.p_LCP)[0];
				// -------------------------------------------------------------------------- //
				// Step 13  $\lambda \gets 1$
				// -------------------------------------------------------------------------- //
				lrs_data.lambda = 1;
				// -------------------------------------------------------------------------- //
				// Step 14
				//    for $j \gets 2, L$ do
				// -------------------------------------------------------------------------- //
				for (unsigned int j = 1; j < lrs_data.p_LCP->size(); ++j)
				{
					// -------------------------------------------------------------------------- //
					// Step 15
					//    $\eta_{\textrm{current}} \gets LCP[j]$
					// -------------------------------------------------------------------------- //
					lrs_data.eta_current = (*lrs_data.p_LCP)[j];
					// -------------------------------------------------------------------------- //
					// Step 16
					// -------------------------------------------------------------------------- //
					if (lrs_data.eta_prev < lrs_data.eta_current)
					{
						// -------------------------------------------------------------------------- //
						// Step 17
						// -------------------------------------------------------------------------- //
						if (0 < lrs_data.eta_prev)
						{
							// -------------------------------------------------------------------------- //
							// Step 18
							// -------------------------------------------------------------------------- //
							(*lrs_data.p_qtilde_work)[lrs_data.eta_prev - 1] = lrs_data.lambda;
						}
						// -------------------------------------------------------------------------- //
						// Step 20
						// -------------------------------------------------------------------------- //
						lrs_data.lambda = 1;
					}
					else if (lrs_data.eta_prev == lrs_data.eta_current)
					{
						// -------------------------------------------------------------------------- //
						// Step 22
						// -------------------------------------------------------------------------- //
						if (lrs_data.eta_current == 0)
						{
							// -------------------------------------------------------------------------- //
							// Step 23
							// -------------------------------------------------------------------------- //
							lrs_data.lambda = 1;
						}
						else
						{
							// -------------------------------------------------------------------------- //
							// Step 25
							// -------------------------------------------------------------------------- //
							lrs_data.lambda += 1;
						}
					}
					else
					{
						// -------------------------------------------------------------------------- //
						// Step 28
						// -------------------------------------------------------------------------- //
						ns_consts::EnmReturnStatus	stsAccum = AccumulateLambda(lrs_data);
						if (ns_consts::EnmReturnStatus::Success != stsAccum)
						{
							// -------------------------------------------------------------------------- //
							//
							// -------------------------------------------------------------------------- //
							TearDownArrayQTilde(lrs_data);
							// -------------------------------------------------------------------------- //
							//
							// -------------------------------------------------------------------------- //
							return sts = stsAccum;
						}
						// -------------------------------------------------------------------------- //
						// Step 29
						// -------------------------------------------------------------------------- //
						ns_consts::EnmReturnStatus	stsUpdNumer = UpdateNumerator(lrs_data);
						if (ns_consts::EnmReturnStatus::Success != stsUpdNumer)
						{
							// -------------------------------------------------------------------------- //
							//
							// -------------------------------------------------------------------------- //
							TearDownArrayQTilde(lrs_data);
							// -------------------------------------------------------------------------- //
							//
							// -------------------------------------------------------------------------- //
							return sts = stsUpdNumer;
						}
						// -------------------------------------------------------------------------- //
						// Step 30
						// -------------------------------------------------------------------------- //
						ns_consts::EnmReturnStatus	stsUpdLambda = UpdateLambda(lrs_data);
						if (ns_consts::EnmReturnStatus::Success != stsUpdLambda)
						{
							// -------------------------------------------------------------------------- //
							//
							// -------------------------------------------------------------------------- //
							TearDownArrayQTilde(lrs_data);
							// -------------------------------------------------------------------------- //
							//
							// -------------------------------------------------------------------------- //
							return sts = stsUpdLambda;
						}
						// -------------------------------------------------------------------------- //
						// Step 31
						// -------------------------------------------------------------------------- //
						ns_consts::EnmReturnStatus	stsUpdQTilde = UpdateQTildeMaster(lrs_data);
						if (ns_consts::EnmReturnStatus::Success != stsUpdQTilde)
						{
							// -------------------------------------------------------------------------- //
							//
							// -------------------------------------------------------------------------- //
							TearDownArrayQTilde(lrs_data);
							// -------------------------------------------------------------------------- //
							//
							// -------------------------------------------------------------------------- //
							return sts = stsUpdQTilde;
						}
						// -------------------------------------------------------------------------- //
						// Step 32
						// -------------------------------------------------------------------------- //
						ResetQTildeWork(lrs_data);
					}
					// -------------------------------------------------------------------------- //
					// Step 34
					// -------------------------------------------------------------------------- //
					lrs_data.eta_prev = lrs_data.eta_current;
				}
				// -------------------------------------------------------------------------- //
				// part of Step 36
				// -------------------------------------------------------------------------- //
				lrs_data.eta_current = 0;
				// -------------------------------------------------------------------------- //
				// part of Step 36
				// -------------------------------------------------------------------------- //
				ns_consts::EnmReturnStatus	stsAccum = AccumulateLambda(lrs_data);
				if (ns_consts::EnmReturnStatus::Success != stsAccum)
				{
					// -------------------------------------------------------------------------- //
					//
					// -------------------------------------------------------------------------- //
					TearDownArrayQTilde(lrs_data);
					// -------------------------------------------------------------------------- //
					//
					// -------------------------------------------------------------------------- //
					return sts = stsAccum;
				}
				// -------------------------------------------------------------------------- //
				// Step 37
				// -------------------------------------------------------------------------- //
				ns_consts::EnmReturnStatus	stsUpdNumer = UpdateNumerator(lrs_data);
				if (ns_consts::EnmReturnStatus::Success != stsUpdNumer)
				{
					// -------------------------------------------------------------------------- //
					//
					// -------------------------------------------------------------------------- //
					TearDownArrayQTilde(lrs_data);
					// -------------------------------------------------------------------------- //
					//
					// -------------------------------------------------------------------------- //
					return sts = stsUpdNumer;
				}
				// -------------------------------------------------------------------------- //
				// Step 38
				// -------------------------------------------------------------------------- //
				ns_consts::EnmReturnStatus	stsUpdQTilde = UpdateQTildeMaster(lrs_data);
				if (ns_consts::EnmReturnStatus::Success != stsUpdQTilde)
				{
					// -------------------------------------------------------------------------- //
					//
					// -------------------------------------------------------------------------- //
					TearDownArrayQTilde(lrs_data);
					// -------------------------------------------------------------------------- //
					//
					// -------------------------------------------------------------------------- //
					return sts = stsUpdQTilde;
				}
				// -------------------------------------------------------------------------- //
				// Step 39
				// -------------------------------------------------------------------------- //
				uint32_t prev_index = 0;
				for (int eta = lrs_data.nu - 1; eta >= 0; --eta)
				{
					if ((*lrs_data.p_qtilde_master)[eta] < io_refData.t_6_3_6.cutoff - 1)
					{
						prev_index = eta;
					}
					else
					{
						break;
					}
				}
				// -------------------------------------------------------------------------- //
				// prev_index is 0-starting index, so add 1 by definition of u
				// -------------------------------------------------------------------------- //
				io_refData.t_6_3_6.u = prev_index + 1;
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				io_refData.t_6_3_6.nu = lrs_data.nu;
				// -------------------------------------------------------------------------- //
				// Step 40
				// -------------------------------------------------------------------------- //
				std::wstringstream ssFragmentForLaTeXPmax = std::wstringstream();
				ns_consts::EnmReturnStatus	stsCalcPHat = CalcPHat(io_refData, lrs_data, ssFragmentForLaTeXPmax);
				if (ns_consts::EnmReturnStatus::Success != stsCalcPHat)
				{
					// -------------------------------------------------------------------------- //
					//
					// -------------------------------------------------------------------------- //
					TearDownArrayQTilde(lrs_data);
					// -------------------------------------------------------------------------- //
					//
					// -------------------------------------------------------------------------- //
					return sts = stsCalcPHat;
				}
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				ns_consts::EnmReturnStatus	stsOutputDist = outputLaTeXDistribution(io_refData, lrs_data, ssFragmentForLaTeXPmax);
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				TearDownArrayQTilde(lrs_data);
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				return sts = stsOutputDist;
			}

			// -------------------------------------------------------------------------- //
			/// <summary>
			///  Implements steps 4 to 5 of 6.3.6 of NIST SP 800-90B.
			/// </summary>
			/// <remarks>
			/// </remarks>
			/// <param name="io_refData">
			/// </param>
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
			/// <param name="i_refData">
			/// </param>
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
			/// <param name="io_refData">
			/// </param>
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
				sts = steps123(io_refData);
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