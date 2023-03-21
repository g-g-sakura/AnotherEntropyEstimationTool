////////////////////////////////////////////////////////////////////////////////
// 6_3_4_CompressionEstimate.cpp
//
//
//
// Copyright (c) 2021-2023 G. G. SAKURAI <g.garland823@gmail.com>
//
////////////////////////////////////////////////////////////////////////////////
#include "pch.h"
#include "6_3_4_CompressionEstimate.h"
#include "./support/checkArgs.h"
#include "./math/SpecialFunctions.h"
#include <boost/math/special_functions.hpp>
#include <boost/math/tools/roots.hpp>
#include <map>

namespace entropy_estimator_lib
{
	namespace estimators
	{
		namespace compression
		{
			namespace ns_consts = entropy_estimator_lib::constants;
			namespace ns_dt = entropy_estimator_lib::data_types;
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
			ns_consts::EnmReturnStatus outputLaTeX(ns_dt::t_data_for_estimator& io_refData,
				blitz::Array<double, 1>& i_refD)
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
				(*io_refData.p_ssLaTeXFragment) << L"\\subsection{The Compression Estimate (NIST SP 800-90B Section 6.3.4)}" << std::endl;
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
				(*io_refData.p_ssLaTeXFragment) << L"\\begin{semilogxaxis}[" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"	width=20cm," << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"	xlabel=$D_{i}$," << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"	ylabel=number of $D_{i}$," << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"	log basis x={2}" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"]" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"\\addplot coordinates {" << std::endl;
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				std::map<int, int>	mp_t_nu_counts;
				for (int i = 0; i < i_refD.length(blitz::firstDim); ++i)
				{
					std::map<int, int>::iterator	mit = mp_t_nu_counts.find((int)i_refD(i));
					if (mit == mp_t_nu_counts.end())
					{
						mp_t_nu_counts.insert(std::make_pair((int)i_refD(i), 1));
					}
					else
					{
						int count = mit->second;
						mit->second = (count + 1);
					}
				}
				int max_D = 1;
				for (std::map<int, int>::const_iterator cit = mp_t_nu_counts.cbegin(); cit != mp_t_nu_counts.cend(); cit++)
				{
					if (max_D < (cit->second))
					{
						max_D = (cit->second);
					}
					(*io_refData.p_ssLaTeXFragment) << L"(";
					(*io_refData.p_ssLaTeXFragment) << std::setw(8) << (cit->first);
					(*io_refData.p_ssLaTeXFragment) << L", " << std::setw(8) << (cit->second);
					(*io_refData.p_ssLaTeXFragment) << L")" << std::endl;
				}
				(*io_refData.p_ssLaTeXFragment) << L"};" << std::endl;
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				(*io_refData.p_ssLaTeXFragment) << L"\\addplot+[Nigelle,no marks,sharp plot,update limits=false] " << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"coordinates {(" << io_refData.t_6_3_4.x_bar << L"," << max_D << L") (" << io_refData.t_6_3_4.x_bar << L",1)}" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"node[above right] at (axis cs:" << io_refData.t_6_3_4.x_bar << L"," << max_D;
				(*io_refData.p_ssLaTeXFragment) << L") {\\shortstack{$\\bar{X}$ = " << io_refData.t_6_3_4.x_bar << L", \\," << L"$\\hat{\\sigma}=$" << io_refData.t_6_3_4.sigma_hat;
				(*io_refData.p_ssLaTeXFragment) << L"\\\\($\\rightarrow$ min-entropy = " << io_refData.t_6_3_4.t_common.min_entropy;
				(*io_refData.p_ssLaTeXFragment) << L" [bit / " << io_refData.bits_per_sample << L"-bit])}};" << std::endl;
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				(*io_refData.p_ssLaTeXFragment) << L"\\end{semilogxaxis}" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"\\end{tikzpicture}" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"\\caption{Distribution of intermediate value $D_{i}$}" << std::endl;
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
				(*io_refData.p_ssLaTeXFragment) << L"\\caption{Supplemental information for traceability (NIST SP 800-90B Section 6.3.4)}" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"\\begin{center}" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"\\begin{tabular}{|l|c|}" << std::endl;
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				(*io_refData.p_ssLaTeXFragment) << L"\\hline " << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"\\rowcolor{anotherlightblue} %%" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"Symbol				& Value ";
				(*io_refData.p_ssLaTeXFragment) << L"\\\\ \\hline " << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"$p$				& " << std::setw(8) << io_refData.t_6_3_4.p;
				(*io_refData.p_ssLaTeXFragment) << L"\\\\ \\hline " << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"$\\bar{X}$ 		& " << std::setw(8) << io_refData.t_6_3_4.x_bar;
				(*io_refData.p_ssLaTeXFragment) << L"\\\\ \\hline" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"$\\hat{\\sigma}$		& " << std::setw(8) << io_refData.t_6_3_4.sigma_hat;
				(*io_refData.p_ssLaTeXFragment) << L"\\\\ \\hline" << std::endl;
				(*io_refData.p_ssLaTeXFragment) << L"$\\bar{X}'$ 		& " << std::setw(8) << io_refData.t_6_3_4.x_bar_prime;
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
			///  Implements step 1 of 6.3.4 of NIST SP 800-90B.
			/// </summary>
			/// <remarks>
			/// </remarks>
			/// <params="o_refSprime">
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
			ns_consts::EnmReturnStatus prepareSprime(blitz::Array<ns_dt::octet, 1>& o_refSprime,
				const ns_dt::t_data_for_estimator& i_refData)
			{
				ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorUnexpected;

				int	def_d = 1000;
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				if (true == i_refData.isModeDemonstration)
				{
					// for demonstrating examples in NIST SP 800-90B 6.3.x
					def_d = 4;
				}
				if (i_refData.t_6_3_4.d != def_d)
				{
					sts = ns_consts::EnmReturnStatus::ErrorInvalidData;
					return sts;
				}
				// -------------------------------------------------------------------------- //
				// 
				// -------------------------------------------------------------------------- //
				int Loverb = i_refData.p_bzInputS->length(blitz::firstDim) / i_refData.t_6_3_4.b;
				o_refSprime.resize(Loverb);
				o_refSprime = 0;
				// -------------------------------------------------------------------------- //
				// 
				// -------------------------------------------------------------------------- //
				for (int i = 0; i < Loverb; ++i)
				{
					ns_dt::octet	s_prime_i = 0;
					for (int j = 0; j < i_refData.t_6_3_4.b; ++j)
					{
						s_prime_i <<= 1;
						s_prime_i |= (*i_refData.p_bzInputS)(i_refData.t_6_3_4.b * i + j) & 0x01;
					}
					o_refSprime(i) = s_prime_i;
				}
				// -------------------------------------------------------------------------- //
				// 
				// -------------------------------------------------------------------------- //
				return sts = ns_consts::EnmReturnStatus::Success;
			}

			// -------------------------------------------------------------------------- //
			/// <summary>
			///  Implements step 3 of 6.3.4 of NIST SP 800-90B.
			/// </summary>
			/// <remarks>
			/// </remarks>
			/// <params="o_refDict">
			/// </params>
			/// <params="i_refSprime">
			/// </params>
			/// <params="i_b">
			/// </params>
			/// <params="i_d">
			/// </params>
			/// <returns>
			/// </returns>
			/// <precondition>
			/// </precondition>
			/// <postcondition>
			/// </postcondition>
			// -------------------------------------------------------------------------- //
			ns_consts::EnmReturnStatus step3(blitz::Array<int, 1>& o_refDict,
				const blitz::Array<ns_dt::octet, 1>& i_refSprime,
				const int i_b, const int i_d,
				bool bIsModeDemo)
			{
				ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorUnexpected;
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				int	def_d = 1000;
				if (true == bIsModeDemo)
				{
					// for demonstrating examples in NIST SP 800-90B 6.3.x
					def_d = 4;
				}
				if (i_d < def_d)
				{
					sts = ns_consts::EnmReturnStatus::ErrorInvalidData;
					return sts;
				}
				// -------------------------------------------------------------------------- //
				// 
				// -------------------------------------------------------------------------- //
				o_refDict.resize(2 << (i_b - 1));
				o_refDict = 0;
				// -------------------------------------------------------------------------- //
				// 
				// -------------------------------------------------------------------------- //
				for (int i = 0; i < i_d; ++i)
				{
					int index = (int)i_refSprime(i);
					o_refDict(index) = i + 1;
				}
				// -------------------------------------------------------------------------- //
				// 
				// -------------------------------------------------------------------------- //
				return sts = ns_consts::EnmReturnStatus::Success;
			}

			// -------------------------------------------------------------------------- //
			/// <summary>
			///  Implements step 4 of 6.3.4 of NIST SP 800-90B.
			/// </summary>
			/// <remarks>
			/// </remarks>
			/// <params="o_refD">
			/// </params>
			/// <params="io_refDict">
			/// </params>
			/// <params="i_refSprime">
			/// </params>
			/// <params="i_b">
			/// </params>
			/// <params="i_d">
			/// </params>
			/// <params="i_L">
			/// </params>
			/// <returns>
			/// </returns>
			/// <precondition>
			/// </precondition>
			/// <postcondition>
			/// </postcondition>
			// -------------------------------------------------------------------------- //
			ns_consts::EnmReturnStatus step4(blitz::Array<double, 1> o_refD,
				blitz::Array<int, 1>& io_refDict,
				const blitz::Array<ns_dt::octet, 1>& i_refSprime,
				const int i_b, const int i_d, int i_L,
				bool bIsModeDemo)
			{
				ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorUnexpected;
				// -------------------------------------------------------------------------- //
				// 
				// -------------------------------------------------------------------------- //
				int	def_d = 1000;
				if (true == bIsModeDemo)
				{
					// for demonstrating examples in NIST SP 800-90B 6.3.x
					def_d = 4;
				}
				if (i_d < def_d)
				{
					sts = ns_consts::EnmReturnStatus::ErrorInvalidData;
					return sts;
				}
				// -------------------------------------------------------------------------- //
				// 
				// -------------------------------------------------------------------------- //
				int nu = i_L / i_b - i_d;
				int Loverb = i_L / i_b;
				// -------------------------------------------------------------------------- //
				// 
				// -------------------------------------------------------------------------- //
				o_refD.resize(nu);
				o_refD = 0.0;
				// -------------------------------------------------------------------------- //
				// step 4-b
				//   For i from d + 1 to \lfloor L / b \rfloor
				// -------------------------------------------------------------------------- //
				for (int i = i_d; i < Loverb; ++i)
				{
					int	s_prime_i = (int)i_refSprime(i);
					// -------------------------------------------------------------------------- //
					// 
					// -------------------------------------------------------------------------- //
					if (io_refDict(s_prime_i) != 0)
					{
						// -------------------------------------------------------------------------- //
						// i. If dict[s_{i}'] is non-zero, then D_{i-d} = i - dict[s_{i}'].
						//    Update the dictionary with the index of the most recent observation, dict[s_{i}'] = i.
						// -------------------------------------------------------------------------- //
						o_refD(i - i_d) = (double)(i + 1) - (double)io_refDict(s_prime_i);
						io_refDict(s_prime_i) = i + 1;
					}
					else
					{
						// -------------------------------------------------------------------------- //
						// ii. If dict[s_{i}'] is zero, add that value to the dictionary, i.e. dict[s_{i}'] = i.
						//     Let D_{i-d} = i.
						// -------------------------------------------------------------------------- //
						io_refDict(s_prime_i) = i + 1;
						o_refD(i - i_d) = (double)(i + 1); // note that 0-offset is considered and + 1 for consistency.
					}
				}
				// -------------------------------------------------------------------------- //
				// 
				// -------------------------------------------------------------------------- //
				return sts = ns_consts::EnmReturnStatus::Success;
			}

			// -------------------------------------------------------------------------- //
			/// <summary>
			///  Implements step 5 of 6.3.4 of NIST SP 800-90B.
			/// </summary>
			/// <remarks>
			/// </remarks>
			/// <params="o_refXbar">
			/// </params>
			/// <params="o_refSigmaHat">
			/// </params>
			/// <params="o_refD">
			/// </params>
			/// <params="i_nu">
			/// </params>
			/// <returns>
			/// </returns>
			/// <precondition>
			/// </precondition>
			/// <postcondition>
			/// </postcondition>
			// -------------------------------------------------------------------------- //
			template<typename T>
			ns_consts::EnmReturnStatus step5(double &o_refXbar, double & o_refSigmaHat,
				blitz::Array<double, 1> o_refD,
				const int i_nu)
			{
				ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorUnexpected;
				// -------------------------------------------------------------------------- //
				// 
				// -------------------------------------------------------------------------- //
				blitz::Array<T, 1>	bz_log2D(i_nu);
				bz_log2D = log(o_refD) / (T)log((T)2.0);
				o_refXbar = mean(bz_log2D);
				// -------------------------------------------------------------------------- //
				// 
				// -------------------------------------------------------------------------- //
				T	sum = 0.0;
				for (int i = 0; i < i_nu; ++i)
				{
					//sum += pow(bz_log2D(i), 2.0) - pow(o_refXbar, 2.0);
					sum += (bz_log2D((T)i) + o_refXbar) * (bz_log2D((T)i) - o_refXbar);
				}
				// -------------------------------------------------------------------------- //
				// 
				// -------------------------------------------------------------------------- //
				sum += pow(o_refXbar, 2.0);
				sum /= (T)(i_nu - 1);
				o_refSigmaHat = 0.5907 * sqrt(sum);
				// -------------------------------------------------------------------------- //
				// 
				// -------------------------------------------------------------------------- //
				return sts = ns_consts::EnmReturnStatus::Success;
			}

			class RhsMinusLhsNaive
			{
			private:
				int b;
				int d;
				int L;
				int nu;
				double	lhs;
			public:
				RhsMinusLhsNaive(int i_b, int i_d, int i_L, int i_nu, double i_lhs)
					: b(i_b), d(i_d), L(i_L), nu(i_nu), lhs(i_lhs) {};

				static double F(double i_z, int i_t, int i_u);
				static double G(double z, int i_d, int i_Loverb, int i_nu);

				double operator()(double i_p)
				{
					double	denom = (pow(2.0, (double)b) - 1.0);
					double	q = (1.0 - i_p) / denom;
					double	rhs = G(i_p, d, L / b, nu) + denom * G(q, d, L / b, nu);
					return	rhs - lhs;
				}
			};
			// -------------------------------------------------------------------------- //
			/// <summary>
			///  Implements function F(z, t, u) in step 7 of NIST SP 800-90B.
			/// </summary>
			/// <remarks>
			///  not optimized version
			/// </remarks>
			/// <params="i_z">
			/// </params>
			/// <params="i_t">
			/// </params>
			/// <params="i_u">
			/// </params>
			/// <returns>
			/// </returns>
			/// <precondition>
			/// </precondition>
			/// <postcondition>
			/// </postcondition>
			// -------------------------------------------------------------------------- //
			inline double RhsMinusLhsNaive::F(double i_z, int i_t, int i_u)
			{
				double	rv = 0.0;
				if (i_u < i_t)
				{
					rv = pow(i_z, 2.0) * pow(1.0 - i_z, (double)(i_u - 1));
				}
				else if (i_u == i_t)
				{
					rv = i_z * pow(1.0 - i_z, (double)(i_t - 1));
				}
				return rv;
			}

			// -------------------------------------------------------------------------- //
			/// <summary>
			///  Implements function G(z) in step 7 of NIST SP 800-90B.
			/// </summary>
			/// <remarks>
			///  not optimized version
			/// </remarks>
			/// <params="z">
			/// </params>
			/// <params="i_d">
			/// </params>
			/// <params="i_Loverb">
			/// </params>
			/// <params="i_nu">
			/// </params>
			/// <returns>
			/// </returns>
			/// <precondition>
			/// </precondition>
			/// <postcondition>
			/// </postcondition>
			// -------------------------------------------------------------------------- //
			double RhsMinusLhsNaive::G(double z, int i_d, int i_Loverb, int i_nu)
			{
				double	rv = 0.0;
				// -------------------------------------------------------------------------- //
				// for (int t = (i_d + 1); t <= floor(L / b); ++t)
				//     Note here that NIST SP 800-90B is mistakenly drafted
				//     and the upper bound of summation, L, should be replaced by floor(L / b).
				// -------------------------------------------------------------------------- //
				for (int t = (i_d + 1); t <= i_Loverb; ++t)
				{
					for (int u = 2; u <= t; ++u)
					{
						// note here that u == 1 does not contribute because log2(1) = 0.
						rv += log2((double)u) * F(z, t, u);
					}
				}
				rv /= (double)i_nu;
				return rv;
			}


			class RhsMinusLhs
			{
			private:
				int b;
				int d;
				int L;
				int nu;
				double	lhs;
			public:
				RhsMinusLhs(int i_b, int i_d, int i_L, int i_nu, double i_lhs)
					: b(i_b), d(i_d), L(i_L), nu(i_nu), lhs(i_lhs) {};

				static double G(double z, int i_d, int i_Loverb, int i_nu);

				double operator()(double i_p)
				{
					double	denom = (pow(2.0, (double)b) - 1.0);
					double	q = (1.0 - i_p) / denom;
					double	rhs = G(i_p, d, L / b, nu) + denom * G(q, d, L / b, nu);
					return	rhs - lhs;
				}
			};


			// -------------------------------------------------------------------------- //
			/// <summary>
			///  Implements function G(z) in step 7 of NIST SP 800-90B.
			/// </summary>
			/// <remarks>
			///  Optimized through formula transformation
			/// </remarks>
			/// <params="z">
			/// </params>
			/// <params="i_d">
			/// </params>
			/// <params="i_Loverb">
			/// </params>
			/// <params="i_nu">
			/// </params>
			/// <returns>
			/// </returns>
			/// <precondition>
			/// </precondition>
			/// <postcondition>
			/// </postcondition>
			// -------------------------------------------------------------------------- //
			double RhsMinusLhs::G(double z, int i_d, int i_Loverb, int i_nu)
			{
				double	rv = 0.0;
				// -------------------------------------------------------------------------- //
				// for (int t = (i_d + 1); t <= floor(L / b); ++t)
				//     Note here that NIST SP 800-90B is mistakenly drafted
				//     and the upper bound of summation, L, should be replaced by floor(L / b).
				// -------------------------------------------------------------------------- //
				// sum group 1
				// -------------------------------------------------------------------------- //
				double sum_grp1 = 0.0;
				for (int u = 2; u <= i_d; ++u)
				{
					// note here that u == 1 does not contribute because log2(1) = 0.
					// so the sum over u starts from 2
					sum_grp1 += log2((double)u) * pow(1.0 - z, u - 1);
				}
				sum_grp1 *= pow(z, 2.0);
				rv += sum_grp1;
				// -------------------------------------------------------------------------- //
				// sum group 2
				// -------------------------------------------------------------------------- //
				double sum_grp2 = 0.0;
				for (int u = (i_d + 1); u <= i_Loverb; ++u)
				{
					sum_grp2 += log2((double)u) * pow(1.0 - z, u - 1);
				}
				sum_grp2 *= z / (double)i_nu;
				rv += sum_grp2;
				// -------------------------------------------------------------------------- //
				// sum group 3
				// -------------------------------------------------------------------------- //
				double sum_grp3 = 0.0;
				for (int u = (i_d + 1); u < i_Loverb; ++u)
				{
					sum_grp3 += ((double)(i_Loverb - u)) * log2((double)u) * pow(1.0 - z, u - 1);
				}
				sum_grp3 *= pow(z, 2.0) / (double)i_nu;
				rv += sum_grp3;
				// -------------------------------------------------------------------------- //
				// 
				// -------------------------------------------------------------------------- //
				return rv;
			}

			class RhsDerivative
			{
			private:
				int b;
				int d;
				int L;
				int nu;
			public:
				RhsDerivative(int i_b, int i_d, int i_L, int i_nu)
					: b(i_b), d(i_d), L(i_L), nu(i_nu) {};

				static long double GDash(double z, int i_d, int i_Loverb, int i_nu);

				long double operator()(double i_p)
				{
					long double	denom = (pow(2.0, (double)b) - 1.0);
					long double	q = (1.0 - i_p) / denom;
					long double	rhs = GDash(i_p, d, L / b, nu) - GDash(q, d, L / b, nu);
					return	rhs;
				}
			};

			// -------------------------------------------------------------------------- //
			/// <summary>
			///  Implements function dG(z)/dz in step 7 of NIST SP 800-90B.
			/// </summary>
			/// <remarks>
			///  Optimized through formula transformation
			/// </remarks>
			/// <params="z">
			/// </params>
			/// <params="i_d">
			/// </params>
			/// <params="i_Loverb">
			/// </params>
			/// <params="i_nu">
			/// </params>
			/// <returns>
			/// </returns>
			/// <precondition>
			/// </precondition>
			/// <postcondition>
			/// </postcondition>
			// -------------------------------------------------------------------------- //
			long double RhsDerivative::GDash(double z, int i_d, int i_Loverb, int i_nu)
			{
				long double	rv = 0.0;
				// -------------------------------------------------------------------------- //
				// for (int t = (i_d + 1); t <= floor(L / b); ++t)
				//     Note here that NIST SP 800-90B is mistakenly drafted
				//     and the upper bound of summation, L, should be replaced by floor(L / b).
				// -------------------------------------------------------------------------- //
				// sum group 1
				// -------------------------------------------------------------------------- //
				long double sum_grp1 = 0.0;
				for (int u = 2; u <= i_d; ++u)
				{
					// note here that u == 1 does not contribute because log2(1) = 0.
					// so the sum over u starts from 2
					sum_grp1 += log2((double)u) * pow(1.0 - z, u - 2) * (2.0 - ((double)(u + 1)) * z);
				}
				sum_grp1 *= z;
				rv += sum_grp1;
				// -------------------------------------------------------------------------- //
				// sum group 2
				// -------------------------------------------------------------------------- //
				long double sum_grp2 = 0.0;
				for (int u = (i_d + 1); u <= i_Loverb; ++u)
				{
					sum_grp2 += log2((double)u) * pow(1.0 - z, u - 2) * (1.0 - ((double)u) * z);
				}
				sum_grp2 /= (double)i_nu;
				rv += sum_grp2;
				// -------------------------------------------------------------------------- //
				// sum group 3
				// -------------------------------------------------------------------------- //
				long double sum_grp3 = 0.0;
				for (int u = (i_d + 1); u < i_Loverb; ++u)
				{
					sum_grp3 += ((double)(i_Loverb - u)) * log2((double)u) * pow(1.0 - z, u - 2) * (2.0 - ((double)(u + 1)) * z);
				}
				sum_grp3 *= z / (double)i_nu;
				rv += sum_grp3;
				// -------------------------------------------------------------------------- //
				// 
				// -------------------------------------------------------------------------- //
				return rv;
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
				if (true == i_refData.isModeDemonstration)
				{
					if (i_refData.L < 48)
					{
						sts = ns_consts::EnmReturnStatus::ErrorInvalidData;
						return sts;
					}
				}
				else
				{
					if (i_refData.L < 6012)
					{
						sts = ns_consts::EnmReturnStatus::ErrorInvalidData;
						return sts;
					}
				}

				return sts = ns_consts::EnmReturnStatus::Success;
			}

			// -------------------------------------------------------------------------- //
			/// <summary>
			///  Implements The Compression Estimate (see 6.3.4 of NIST SP 800-90B).
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

				double z_alpha = calc_Z_alpha(0.995);
				// -------------------------------------------------------------------------- //
				// Step 1.
				//  Let b = 6. Create a new sequence, S' = (s'_{1}, \ldots, s'_{\floor(L / b)}), 
				//  by dividing S into non-overlapping b-bit blocks.
				//  If L is not a multiple of b, discard the extra data.
				// -------------------------------------------------------------------------- //
				blitz::Array<ns_dt::octet, 1>	bz_S_prime(io_refData.p_bzInputS->length(blitz::firstDim)/io_refData.t_6_3_4.b);
				sts = prepareSprime(bz_S_prime, io_refData);
				if (ns_consts::EnmReturnStatus::Success != sts)
				{
					return sts;
				}
				// -------------------------------------------------------------------------- //
				// Step 2.
				// Partition the dataset, S', into two disjoint groups. 
				// These two groups will form the dictionary and the test data.
				//   a.Create the dictionary from the first d = 1000 elements of S', (s'_{1}, \ldots, s'_{d}).
				//   b.Use the remaining nu = \floor (L / b) - d observations, (s'_{d+1}, \ldots, s'_{\floor(L/b)}), for testing.
				// -------------------------------------------------------------------------- //
				int nu = io_refData.p_bzInputS->length(blitz::firstDim) / io_refData.t_6_3_4.b - io_refData.t_6_3_4.d;
				// -------------------------------------------------------------------------- //
				// Step 3.
				// Initialize the dictionary dict to an all zero array of size 2^b. 
				// For i from 1 to d, let dict[s'_{i}] = i. 
				// The value of dict[s'_{i}] is the index of the last occurrence of each s'_{i} in the dictionary.
				// -------------------------------------------------------------------------- //
				blitz::Array<int, 1> bz_dict(2 << (io_refData.t_6_3_4.b - 1));
				bz_dict = 0;
				sts = step3(bz_dict, bz_S_prime, io_refData.t_6_3_4.b, io_refData.t_6_3_4.d,
					io_refData.isModeDemonstration);
				if (ns_consts::EnmReturnStatus::Success != sts)
				{
					return sts;
				}
				// -------------------------------------------------------------------------- //
				// Step 4.
				// Run the test data against the dictionary created in Step 2.
				//   a. Let D be a list of length \nu.
				//   b. For i from d + 1 to \floor (L /b):
				//      i. If dict[s'_{i}] is non-zero, then D_{i-d} = i- dict[s'_{i}]. 
				//         Update the dictionary with the index of the most recent observation, dict[s'_{i}]=i.
				//     ii. If dict[s_i] is zero, add that value to the dictionary, i.e., dict[s'_{i}] = i. 
				//         Let D_{i - d} = i.
				// -------------------------------------------------------------------------- //
				blitz::Array<double, 1>	bz_D(nu);
				bz_D = 0.0;
				sts = step4(bz_D, bz_dict, bz_S_prime, io_refData.t_6_3_4.b, io_refData.t_6_3_4.d, io_refData.p_bzInputS->length(blitz::firstDim),
					io_refData.isModeDemonstration);
				if (ns_consts::EnmReturnStatus::Success != sts)
				{
					return sts;
				}
				// -------------------------------------------------------------------------- //
				// Step 5.
				// Calculate the sample mean, \bar{X}, and sample standard deviation, \hat{\sigma}, 
				//  of (log2(D_1), \ldots, log2(D_{\nu})).
				// -------------------------------------------------------------------------- //
				double x_bar = 0.0, sigma_hat = 0.0;
				sts = step5<long double>(x_bar, sigma_hat, bz_D, nu);
				if (ns_consts::EnmReturnStatus::Success != sts)
				{
					return sts;
				}
				// -------------------------------------------------------------------------- //
				// Step 6.
				// Compute the lower-bound of the confidence interval for the mean, based on a normal distribution using
				// -------------------------------------------------------------------------- //
				double	x_bar_prime = x_bar - z_alpha * sigma_hat / sqrt((double)nu);
				// -------------------------------------------------------------------------- //
				// before performing a binary search, evaluate the derivative of r.h.s. of step 7 to find its maximum value.
				// There exist cases where X'(=l.h.s.) is greater than the maximum value of r.h.s., and such cases do not yield a solution.
				// -------------------------------------------------------------------------- //
				double	lowerbound = pow(0.5, io_refData.t_6_3_4.b);
				double	upperbound = 1.0;
				boost::math::tools::eps_tolerance<double> tol(std::numeric_limits<double>::digits - 5);
				const boost::uintmax_t max_it = 128;
				boost::uintmax_t it = max_it;
				double	rhs_max = 0.0;
				RhsDerivative	rhs_derivative(io_refData.t_6_3_4.b, io_refData.t_6_3_4.d, io_refData.p_bzInputS->length(blitz::firstDim), nu);
				RhsMinusLhs	rhs(io_refData.t_6_3_4.b, io_refData.t_6_3_4.d, io_refData.p_bzInputS->length(blitz::firstDim), nu, 0.0);
				try
				{
					std::pair<long double, long double>	r_dash = boost::math::tools::bisect(rhs_derivative, lowerbound, upperbound, tol, it);
					rhs_max = rhs(r_dash.first);
				}
				catch (const std::domain_error& e)
				{
					rhs_max = rhs(lowerbound);
				}
				catch (const std::overflow_error& e)
				{
					rhs_max = rhs(lowerbound);
				}
				catch (const std::underflow_error& e)
				{
					rhs_max = rhs(lowerbound);
				}
				catch (const boost::math::evaluation_error& e)
				{
					rhs_max = rhs(lowerbound);
				}
				// -------------------------------------------------------------------------- //
				// Step 7.
				// Using a binary search, solve for the parameter p, such that the following equation is true:
				// -------------------------------------------------------------------------- //
				if ((x_bar_prime < 0.0) || (rhs_max < x_bar_prime))
				{
					io_refData.t_6_3_4.x_bar = x_bar;
					io_refData.t_6_3_4.sigma_hat = sigma_hat;
					io_refData.t_6_3_4.x_bar_prime = x_bar_prime;
					io_refData.t_6_3_4.p = pow(0.5, io_refData.t_6_3_4.b);
					io_refData.t_6_3_4.t_common.min_entropy = 1.0;
					io_refData.t_6_3_4.bIsRootFound = false;
				}
				else
				{
					RhsMinusLhs	fnc(io_refData.t_6_3_4.b, io_refData.t_6_3_4.d, io_refData.p_bzInputS->length(blitz::firstDim), nu, x_bar_prime);
					try
					{
						it = max_it;
						std::pair<double, double>	r = boost::math::tools::toms748_solve(fnc, lowerbound, upperbound, tol, it);
						// -------------------------------------------------------------------------- //
						// Step 8.
						// If the binary search yields a solution, then the min-entropy is the negative logarithm of the parameter, p:
						// -------------------------------------------------------------------------- //
						io_refData.t_6_3_4.x_bar = x_bar;
						io_refData.t_6_3_4.sigma_hat = sigma_hat;
						io_refData.t_6_3_4.x_bar_prime = x_bar_prime;
						if (tol(r.first, r.second) == true)
						{
							io_refData.t_6_3_4.p = r.second;
							io_refData.t_6_3_4.t_common.min_entropy = -log2(r.second) / (double)io_refData.t_6_3_4.b;
							io_refData.t_6_3_4.bIsRootFound = true;
						}
						else
						{
							io_refData.t_6_3_4.p = pow(0.5, io_refData.t_6_3_4.b);
							io_refData.t_6_3_4.t_common.min_entropy = 1.0;
							io_refData.t_6_3_4.bIsRootFound = false;
						}
					}
					catch (const std::domain_error& e)
					{
						io_refData.t_6_3_4.x_bar = x_bar;
						io_refData.t_6_3_4.sigma_hat = sigma_hat;
						io_refData.t_6_3_4.x_bar_prime = x_bar_prime;
						io_refData.t_6_3_4.p = pow(0.5, io_refData.t_6_3_4.b);
						io_refData.t_6_3_4.t_common.min_entropy = 1.0;
						io_refData.t_6_3_4.bIsRootFound = false;
					}
				}
				// -------------------------------------------------------------------------- //
				// output LaTeX
				// -------------------------------------------------------------------------- //
				if (io_refData.isGeneratingReportInLaTeXformatRequested)
				{
					// -------------------------------------------------------------------------- //
					//
					// -------------------------------------------------------------------------- //
					outputLaTeX(io_refData, bz_D);
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