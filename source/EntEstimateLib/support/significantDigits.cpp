////////////////////////////////////////////////////////////////////////////////
// significantDigits.cpp
//
//
//
// Copyright (c) 2021-2025 G. G. SAKURAI <g.garland823@gmail.com>
//
////////////////////////////////////////////////////////////////////////////////
#include "../constants.h"
#include "../EntropyEstimateData.h"
#include "../math/Equations.h"
#include "../math/SpecialFunctions.h"
#include <boost/math/tools/roots.hpp>

namespace entropy_estimator_lib
{
	namespace estimators
	{
		namespace support
		{

			namespace ns_consts = entropy_estimator_lib::constants;
			namespace ns_math = entropy_estimator_lib::estimators::math;


			// -------------------------------------------------------------------------- //
			/// <summary>
			/// </summary>
			/// <remarks>
			/// </remarks>
			/// <param name="o_ref_min_entropy_lower_bound">
			/// </param>
			/// <param name="o_ref_min_entropy_upper_bound">
			/// </param>
			/// <param name="o_ref_number_of_significant_digits">
			/// </param>
			/// <param name="i_ref_X_bar_prime">
			/// </param>
			/// <param name="i_ref_nu">
			/// </param>
			/// <returns>
			/// </returns>
			/// <precondition>
			/// </precondition>
			/// <postcondition>
			/// </postcondition>
			// -------------------------------------------------------------------------- //
			ns_consts::EnmReturnStatus evaluateSignificantDigitsSP800_90B_6_3_2(
				double& o_ref_min_entropy_lower_bound,
				double& o_ref_min_entropy_upper_bound,
				double& o_ref_number_of_significant_digits,
				const double& i_ref_X_bar_prime,
				const int& i_ref_nu)
			{
				ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorUnexpected;

				// -------------------------------------------------------------------------- //
				// 
				// -------------------------------------------------------------------------- //
				double p;
				if ((i_ref_X_bar_prime < 2.0) || (2.5 < i_ref_X_bar_prime))
				{
					p = 0.5;
				}
				else
				{
					p = 0.5 + sqrt(1.25 - 0.5 * i_ref_X_bar_prime);
				}
				const double	min_entropy = -log2(p);
				// -------------------------------------------------------------------------- //
				// 
				// -------------------------------------------------------------------------- //
				double	x_bar_plus_eps = i_ref_X_bar_prime + 3.0 / (static_cast<double>(i_ref_nu));
				double	x_bar_minus_eps = i_ref_X_bar_prime - 3.0 / (static_cast<double>(i_ref_nu));
				if ((x_bar_plus_eps < 2.0) || (2.5 < x_bar_plus_eps))
				{
					x_bar_plus_eps = 2.5;
				}
				if ((x_bar_minus_eps < 2.0) || (2.5 < x_bar_minus_eps))
				{
					x_bar_minus_eps = 2.5;
				}
				const double	p_global_plus_eps = 0.5 + sqrt(1.25 - 0.5 * x_bar_plus_eps);
				const double	p_global_minus_eps = 0.5 + sqrt(1.25 - 0.5 * x_bar_minus_eps);
				o_ref_min_entropy_lower_bound = -log2(p_global_minus_eps);
				o_ref_min_entropy_upper_bound = -log2(p_global_plus_eps);
				// -------------------------------------------------------------------------- //
				// 
				// -------------------------------------------------------------------------- //
				if ((i_ref_X_bar_prime < 2.0) || (2.5 < i_ref_X_bar_prime))
				{
					o_ref_number_of_significant_digits = 1;
				}
				else
				{
					o_ref_number_of_significant_digits = -log10(fabs((o_ref_min_entropy_lower_bound - o_ref_min_entropy_upper_bound) / min_entropy));
				}

				return	sts = ns_consts::EnmReturnStatus::Success;
			}

			// -------------------------------------------------------------------------- //
			/// <summary>
			/// </summary>
			/// <remarks>
			/// </remarks>
			/// <param name="o_ref_min_entropy_lower_bound">
			/// </param>
			/// <param name="o_ref_min_entropy_upper_bound">
			/// </param>
			/// <param name="o_ref_number_of_significant_digits">
			/// </param>
			/// <param name="i_ref_C">
			/// </param>
			/// <param name="i_ref_N">
			/// </param>
			/// <returns>
			/// </returns>
			/// <precondition>
			/// </precondition>
			/// <postcondition>
			/// </postcondition>
			// -------------------------------------------------------------------------- //
			ns_consts::EnmReturnStatus evaluateSignificantDigitsOfMinEntropyByPglobalSP800_90B_6_3_7_6_3_10(
				double& o_ref_min_entropy_lower_bound,
				double& o_ref_min_entropy_upper_bound,
				double& o_ref_number_of_significant_digits,
				const int& i_ref_C,
				const int& i_ref_N)
			{
				ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorUnexpected;

				const double z_alpha = calc_Z_alpha(0.995);
				const double	p_global = (static_cast<double>(i_ref_C)) / (static_cast<double>(i_ref_N));
				const double	p_global_plus_eps = (static_cast<double>(i_ref_C) + 1.0) / static_cast<double>(i_ref_N);
				const double	p_global_minus_eps = (static_cast<double>(i_ref_C) - 1.0) / static_cast<double>(i_ref_N);
				const double	p_global_prime			= p_global + z_alpha * sqrt(p_global * (1.0 - p_global) / static_cast<double>(i_ref_N - 1));
				// -------------------------------------------------------------------------- //
				// 
				// -------------------------------------------------------------------------- //
				double	p_global_prime_plus_eps = p_global_plus_eps + z_alpha * sqrt(p_global_plus_eps * (1.0 - p_global_plus_eps) / static_cast<double>(i_ref_N - 1));
				if (1.0 < p_global_prime_plus_eps)
				{
					p_global_prime_plus_eps = 1.0;
				}
				const double	min_entropy_plus_eps = -log2(p_global_prime_plus_eps);
				// -------------------------------------------------------------------------- //
				// 
				// -------------------------------------------------------------------------- //
				double	p_global_prime_minus_eps= p_global_minus_eps + z_alpha * sqrt(p_global_minus_eps * (1.0 - p_global_minus_eps) / static_cast<double>(i_ref_N - 1));
				if (1.0 < p_global_prime_minus_eps)
				{
					p_global_prime_minus_eps = 1.0;
				}
				const double	min_entropy_minus_eps = -log2(p_global_prime_minus_eps);
				// -------------------------------------------------------------------------- //
				// 
				// -------------------------------------------------------------------------- //
				if (min_entropy_minus_eps < min_entropy_plus_eps)
				{
					o_ref_min_entropy_lower_bound = min_entropy_minus_eps;
					o_ref_min_entropy_upper_bound = min_entropy_plus_eps;
				}
				else
				{
					o_ref_min_entropy_lower_bound = min_entropy_plus_eps;
					o_ref_min_entropy_upper_bound = min_entropy_minus_eps;
				}
				// -------------------------------------------------------------------------- //
				// 
				// -------------------------------------------------------------------------- //
				const double	number_of_significant_digits_plus = -log10(fabs((log2(p_global_prime_plus_eps) - log2(p_global_prime)) / log2(p_global_prime)));
				const double	number_of_significant_digits_minus = -log10(fabs((log2(p_global_prime_minus_eps) - log2(p_global_prime)) / log2(p_global_prime)));
				if (number_of_significant_digits_minus < number_of_significant_digits_plus)
				{
					o_ref_number_of_significant_digits = number_of_significant_digits_minus;
				}
				else
				{
					o_ref_number_of_significant_digits = number_of_significant_digits_plus;
				}

				return	sts = ns_consts::EnmReturnStatus::Success;
			}

			// -------------------------------------------------------------------------- //
			/// <summary>
			/// </summary>
			/// <remarks>
			/// </remarks>
			/// <param name="o_ref_min_entropy_lower_bound">
			/// </param>
			/// <param name="o_ref_min_entropy_upper_bound">
			/// </param>
			/// <param name="o_ref_number_of_significant_digits">
			/// </param>
			/// <param name="i_ref_r">
			/// </param>
			/// <param name="i_ref_N">
			/// </param>
			/// <param name="i_ref_k">
			/// </param>
			/// <returns>
			/// </returns>
			/// <precondition>
			/// </precondition>
			/// <postcondition>
			/// </postcondition>
			// -------------------------------------------------------------------------- //
			ns_consts::EnmReturnStatus evaluateSignificantDigitsOfMinEntropyByPlocalSP800_90B_6_3_7_6_3_10(
				double& o_ref_min_entropy_lower_bound,
				double& o_ref_min_entropy_upper_bound,
				double& o_ref_number_of_significant_digits,
				const int& i_ref_r,
				const int& i_ref_N,
				const unsigned int& i_ref_k)
			{
				ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorUnexpected;

				double	array_p_local[3] = {0.0, 0.0, 0.0};
				double	min_entropy_possibilities[3] = { 0.0, 0.0, 0.0 };
				// -------------------------------------------------------------------------- //
				// 
				// -------------------------------------------------------------------------- //
				if (i_ref_r <= 2)
				{
					for (int j = 0; j < 2; ++j)
					{
						const int r = i_ref_r + j;
						ns_math::RhsMinusLhs	fnc(r, i_ref_N, 0.99);
						double	lowerbound = pow(0.5, 20.0);
						double	upperbound = 1.0 - pow(0.5, 20.0);
						const boost::math::tools::eps_tolerance<double> tol(std::numeric_limits<double>::digits - 4);
						boost::uintmax_t it = 64;
						const std::pair<double, double>	rg = boost::math::tools::toms748_solve(fnc, lowerbound, upperbound, tol, it);
						// -------------------------------------------------------------------------- //
						//
						// -------------------------------------------------------------------------- //
						array_p_local[j + 1] = rg.first;
						// -------------------------------------------------------------------------- //
						//
						// -------------------------------------------------------------------------- //
						if (array_p_local[j + 1] < (1.0 / static_cast<double>(i_ref_k)))
						{
							array_p_local[j + 1] = (1.0 / static_cast<double>(i_ref_k));
						}
						min_entropy_possibilities[j + 1] = -log2(array_p_local[j + 1]);
					}
					// -------------------------------------------------------------------------- //
					//
					// -------------------------------------------------------------------------- //
					if (min_entropy_possibilities[1] < min_entropy_possibilities[2])
					{
						o_ref_min_entropy_lower_bound = min_entropy_possibilities[1];
						o_ref_min_entropy_upper_bound = min_entropy_possibilities[2];
					}
					else
					{
						o_ref_min_entropy_lower_bound = min_entropy_possibilities[2];
						o_ref_min_entropy_upper_bound = min_entropy_possibilities[1];
					}
					// -------------------------------------------------------------------------- //
					//
					// -------------------------------------------------------------------------- //
					const double	number_of_significant_digits_plus = -log10(fabs((log2(array_p_local[2]) - log2(array_p_local[1])) / log2(array_p_local[1])));
					// -------------------------------------------------------------------------- //
					//
					// -------------------------------------------------------------------------- //
					o_ref_number_of_significant_digits = number_of_significant_digits_plus;
				}
				else {
					for (int j = 0; j < 3; ++j)
					{
						const int r = i_ref_r - 1 + j;
						ns_math::RhsMinusLhs	fnc(r, i_ref_N, 0.99);
						double	lowerbound = pow(0.5, 20.0);
						double	upperbound = 1.0 - pow(0.5, 20.0);
						const boost::math::tools::eps_tolerance<double> tol(std::numeric_limits<double>::digits - 4);
						boost::uintmax_t it = 64;
						const std::pair<double, double>	rg = boost::math::tools::toms748_solve(fnc, lowerbound, upperbound, tol, it);
						// -------------------------------------------------------------------------- //
						//
						// -------------------------------------------------------------------------- //
						array_p_local[j] = rg.first;
						// -------------------------------------------------------------------------- //
						//
						// -------------------------------------------------------------------------- //
						if (array_p_local[j] < (1.0 / static_cast<double>(i_ref_k)))
						{
							array_p_local[j] = (1.0 / static_cast<double>(i_ref_k));
						}
						// -------------------------------------------------------------------------- //
						//
						// -------------------------------------------------------------------------- //
						min_entropy_possibilities[j] = -log2(array_p_local[j]);
					}
					// -------------------------------------------------------------------------- //
					// lower bound, upper bound
					// -------------------------------------------------------------------------- //
					if (min_entropy_possibilities[1] < min_entropy_possibilities[2])
					{
						if (min_entropy_possibilities[0] < min_entropy_possibilities[1])
						{
							// min_entropy_possibilities[0] is minimum
							o_ref_min_entropy_lower_bound = min_entropy_possibilities[0];
							// min_entropy_possibilities[2] is maximum
							o_ref_min_entropy_upper_bound = min_entropy_possibilities[2];
						}
						else
						{
							// min_entropy_possibilities[1] is minimum
							o_ref_min_entropy_lower_bound = min_entropy_possibilities[1];
							if (min_entropy_possibilities[0] < min_entropy_possibilities[2])
							{
								// min_entropy_possibilities[2] is maximum
								o_ref_min_entropy_upper_bound = min_entropy_possibilities[2];
							}
							else
							{
								// min_entropy_possibilities[0] is maximum
								o_ref_min_entropy_upper_bound = min_entropy_possibilities[0];

							}
						}
					}
					else
					{
						if (min_entropy_possibilities[0] < min_entropy_possibilities[2])
						{
							// min_entropy_possibilities[0] is minimum
							o_ref_min_entropy_lower_bound = min_entropy_possibilities[0];
						}
						else
						{
							// min_entropy_possibilities[2] is minimum
							o_ref_min_entropy_lower_bound = min_entropy_possibilities[2];
							if (min_entropy_possibilities[0] < min_entropy_possibilities[1])
							{
								// min_entropy_possibilities[1] is maximum
								o_ref_min_entropy_upper_bound = min_entropy_possibilities[1];
							}
							else
							{
								// min_entropy_possibilities[0] is maximum
								o_ref_min_entropy_upper_bound = min_entropy_possibilities[0];
							}
						}
					}
					// -------------------------------------------------------------------------- //
					//
					// -------------------------------------------------------------------------- //
					const double	number_of_significant_digits_plus = -log10(fabs((log2(array_p_local[2]) - log2(array_p_local[1])) / log2(array_p_local[1])));
					const double	number_of_significant_digits_minus = -log10(fabs((log2(array_p_local[0]) - log2(array_p_local[1])) / log2(array_p_local[1])));
					if (number_of_significant_digits_minus < number_of_significant_digits_plus)
					{
						o_ref_number_of_significant_digits = number_of_significant_digits_minus;
					}
					else
					{
						o_ref_number_of_significant_digits = number_of_significant_digits_plus;
					}
				}

				return	sts = ns_consts::EnmReturnStatus::Success;
			}

		}
	}
}