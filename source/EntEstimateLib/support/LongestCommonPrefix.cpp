////////////////////////////////////////////////////////////////////////////////
// LongestCommonPrefix.cpp
//
//
//
// Copyright (c) 2025 G. G. SAKURAI <g.garland823@gmail.com>
//
////////////////////////////////////////////////////////////////////////////////
#include <vector>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include "LongestCommonPrefix.h"
#include "../EntropyEstimateData.h"

namespace entropy_estimator_lib
{
	namespace estimators
	{
		namespace support
		{
			namespace ns_dt = entropy_estimator_lib::data_types;
			// -------------------------------------------------------------------------- //
			/// <summary>
			///  Implements 'ord' function in INTRODUCTION TO ALGORITHMS (4th Edition), The MIT Press.
			/// </summary>
			/// <remarks>
			///	 Returns ASCII code of the provided octet.
			/// </remarks>
			/// <param name="i_t">
			/// </param>
			/// <returns>
			///	 ASCII code
			/// </returns>
			/// <precondition>
			/// </precondition>
			/// <postcondition>
			/// </postcondition>
			// -------------------------------------------------------------------------- //
			int32_t		ord(ns_dt::octet i_t)
			{
				return static_cast<int32_t>(i_t);
			}

			// -------------------------------------------------------------------------- //
			/// <summary>
			///  Implements Make-Ranks(substr-rank, rank, n) in INTRODUCTION TO ALGORITHMS (4th Edition), The MIT Press.
			/// </summary>
			/// <remarks>
			/// </remarks>
			/// <param name="io_substr_rank">
			/// </param>
			/// <param name="io_rank">
			/// </param>
			/// <param name="n">
			/// </param>
			/// <returns>
			///  <c>entropy_estimator_lib::constants::EnmReturnStatus::ErrorInsufficientData</c>:  when either of the following conditions is met:
			///    <ul>
			///     <li><c>io_rank.size()</c> &lt; (n)</li>
			///     <li><c>io_substr_rank.size()</c> &lt; (n)</li>
			///    </ul>
			///  <c>entropy_estimator_lib::constants::EnmReturnStatus::Success</c>:  otherwise.
			/// </returns>
			/// <precondition>
			/// </precondition>
			/// <postcondition>
			/// </postcondition>
			// -------------------------------------------------------------------------- //
			ns_consts::EnmReturnStatus MakeRanks(std::vector<t_substr_rank>& io_substr_rank, std::valarray<uint32_t>& io_rank, uint32_t n)
			{
				ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorUnexpected;

				if (io_substr_rank.size() < (n))
				{
					return	sts = ns_consts::EnmReturnStatus::ErrorInsufficientData;
				}

				if (io_rank.size() < (n))
				{
					return	sts = ns_consts::EnmReturnStatus::ErrorInsufficientData;
				}
				// -------------------------------------------------------------------------- //
				// Step 1
				//  r = 1
				// -------------------------------------------------------------------------- //
				int32_t		r = 0;
				// -------------------------------------------------------------------------- //
				// Step 2
				//  rank[substr-rank[1].index] = r
				// -------------------------------------------------------------------------- //
				io_rank[io_substr_rank[0].index - 1] = r;
				// -------------------------------------------------------------------------- //
				// Step 3
				//  for i = 2 to n
				// -------------------------------------------------------------------------- //
				for (uint32_t i = 1; i < n; i++)
				{
					if ((io_substr_rank[i].left_rank != io_substr_rank[i - 1].left_rank)
						|| (io_substr_rank[i].right_rank != io_substr_rank[i - 1].right_rank))
					{
						r++;
					}
					io_rank[io_substr_rank[i].index - 1] = r;
				}

				return	sts = ns_consts::EnmReturnStatus::Success;
			}

			// -------------------------------------------------------------------------- //
			/// <summary>
			///  Implements Compute-Suffix-Array(T, n) in INTRODUCTION TO ALGORITHMS (4th Edition), The MIT Press.
			/// </summary>
			/// <remarks>
			/// </remarks>
			/// <param name="io_SA">
			/// </param>
			/// <param name="i_T">
			/// </param>
			/// <param name="n">
			/// </param>
			/// <returns>
			///  <c>entropy_estimator_lib::constants::EnmReturnStatus::ErrorInsufficientData</c>:  when the following condition is met:
			///    <ul>
			///     <li><c>i_T.size()</c> &lt; n</li>
			///    </ul>
			///  <c>entropy_estimator_lib::constants::EnmReturnStatus::Success</c>:  otherwise.
			/// </returns>
			/// <precondition>
			/// </precondition>
			/// <postcondition>
			/// </postcondition>
			// -------------------------------------------------------------------------- //
			ns_consts::EnmReturnStatus ComputeSuffixArray(std::valarray<uint32_t>& io_SA, const octetarray& i_T, uint32_t n)
			{
				ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorUnexpected;

				if (i_T.size() < (n))
				{
					return	sts = ns_consts::EnmReturnStatus::ErrorInsufficientData;
				}

				io_SA.resize(n, (uint32_t)0);

				std::vector<t_substr_rank>	the_substr_rank(n);
				std::valarray<uint32_t>			the_rank((uint32_t)0, n);

				//dbg_print_substr_rank_header(0);

				// -------------------------------------------------------------------------- //
				// Step 2
				// -------------------------------------------------------------------------- //
				for (uint32_t i = 0; i < n; ++i)
				{
					// -------------------------------------------------------------------------- //
					// Step 3
					// -------------------------------------------------------------------------- //
					the_substr_rank[i].left_rank = ord(i_T[i]);
					// -------------------------------------------------------------------------- //
					// Step 4
					// -------------------------------------------------------------------------- //
					if (i < (n - 1))
					{
						// -------------------------------------------------------------------------- //
						// Step 5
						// -------------------------------------------------------------------------- //
						the_substr_rank[i].right_rank = ord(i_T[i + 1]);
					}
					else
					{
						// -------------------------------------------------------------------------- //
						// Step 6
						//  else substr-rank[i].right-rank = 0
						//   It is assumed that the 'smallest' right rank value is set by 0 in this step,
						//   so the value is replaced by -1.
						// -------------------------------------------------------------------------- //
						the_substr_rank[i].right_rank = -1;
					}
					// -------------------------------------------------------------------------- //
					// Step 7
					// -------------------------------------------------------------------------- //
					the_substr_rank[i].index = (i + 1);

					//dbg_print_substr_rank(the_substr_rank[i], i, 0);
				}

				// -------------------------------------------------------------------------- //
				// step 8 sort substr-rank ascending
				// -------------------------------------------------------------------------- //
				std::vector<t_substr_rank>::iterator it = the_substr_rank.begin();
				//++it;
				std::sort(it, the_substr_rank.end());

				//dbg_print_substr_rank_header(42);
				//for (uint32_t j = 0; j < n; ++j)
				//{
				//	dbg_print_substr_rank(the_substr_rank[j], j, 42);
				//}

				// -------------------------------------------------------------------------- //
				// Step 9
				// -------------------------------------------------------------------------- //
				uint32_t l = 2;

				// -------------------------------------------------------------------------- //
				// Step 10
				// -------------------------------------------------------------------------- //
				while (l < n)
				{
					// -------------------------------------------------------------------------- //
					// Step 11
					// -------------------------------------------------------------------------- //
					ns_consts::EnmReturnStatus	stsMakeRanks = MakeRanks(the_substr_rank, the_rank, n);
					if (ns_consts::EnmReturnStatus::Success != stsMakeRanks)
					{
						return	sts = stsMakeRanks;
					}

					//if ((2 == l) || (4 == l))
					//{
					//	//dbg_print_rank_header(0);
					//	for (uint32_t j = 0; j < n; ++j)
					//	{
					//		dbg_print_rank(j, the_rank[j], 0);
					//	}
					//}

					// -------------------------------------------------------------------------- //
					// Step 12
					// -------------------------------------------------------------------------- //
					for (uint32_t i = 0; i < n; ++i)
					{
						// -------------------------------------------------------------------------- //
						// Step 13
						// -------------------------------------------------------------------------- //
						the_substr_rank[i].left_rank = the_rank[i];
						// -------------------------------------------------------------------------- //
						// Step 14
						// -------------------------------------------------------------------------- //
						if ((i + l) < n)
						{
							// -------------------------------------------------------------------------- //
							// Step 15
							// -------------------------------------------------------------------------- //
							the_substr_rank[i].right_rank = the_rank[i + l];
						}
						else
						{
							// -------------------------------------------------------------------------- //
							// Step 16
							//  else substr-rank[i].right-rank = 0
							//   It is assumed that the 'smallest' right rank value is set by 0 in this step,
							//   so the value is replaced by -1.
							// -------------------------------------------------------------------------- //
							the_substr_rank[i].right_rank = -1;
						}
						// -------------------------------------------------------------------------- //
						// Step 17
						// -------------------------------------------------------------------------- //
						the_substr_rank[i].index = (i + 1);

					}
					//if ((2 == l) || (4 == l))
					//{
					//	dbg_print_substr_rank_header(8);
					//	for (uint32_t j = 0; j < n; ++j)
					//	{
					//		dbg_print_substr_rank(the_substr_rank[j], j, 8);
					//	}
					//}

					// -------------------------------------------------------------------------- //
					// Step 18
					// -------------------------------------------------------------------------- //
					it = the_substr_rank.begin();
					//++it;
					std::sort(it, the_substr_rank.end());

					//if ((2 == l) || (4 == l))
					//{
					//	dbg_print_substr_rank_header(50);
					//	for (uint32_t j = 0; j < n; ++j)
					//	{
					//		dbg_print_substr_rank(the_substr_rank[j], j, 50);
					//	}
					//}

					// -------------------------------------------------------------------------- //
					// Step 19
					// -------------------------------------------------------------------------- //
					l *= 2;
				}

				// -------------------------------------------------------------------------- //
				// Step 20
				// -------------------------------------------------------------------------- //
				for (uint32_t i = 0; i < n; ++i)
				{
					// -------------------------------------------------------------------------- //
					// Step 21
					// -------------------------------------------------------------------------- //
					io_SA[i] = the_substr_rank[i].index - 1;
				}

				// -------------------------------------------------------------------------- //
				// Step 22
				// -------------------------------------------------------------------------- //
				return	sts = ns_consts::EnmReturnStatus::Success;
			}

			// -------------------------------------------------------------------------- //
			/// <summary>
			///  Implements Compute-LCP(T, SA, n) in INTRODUCTION TO ALGORITHMS (4th Edition), The MIT Press.
			/// </summary>
			/// <remarks>
			/// </remarks>
			/// <param name="io_LCP">
			/// </param>
			/// <param name="i_T">
			/// </param>
			/// <param name="i_SA">
			/// </param>
			/// <param name="n">
			/// </param>
			/// <returns>
			///  <c>entropy_estimator_lib::constants::EnmReturnStatus::ErrorInsufficientData</c>:  when either of the following conditions is met:
			///    <ul>
			///     <li><c>i_T.size()</c> &lt; (n + 1)</li>
			///     <li><c>i_SA.size()</c> &lt; (n + 1)</li>
			///    </ul>
			///  <c>entropy_estimator_lib::constants::EnmReturnStatus::Success</c>:  otherwise.
			/// </returns>
			/// <precondition>
			/// </precondition>
			/// <postcondition>
			/// </postcondition>
			// -------------------------------------------------------------------------- //
			ns_consts::EnmReturnStatus ComputeLCP(std::valarray<uint32_t>& io_LCP, const octetarray& i_T, const std::valarray<uint32_t>& i_SA, uint32_t n)
			{
				ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorUnexpected;

				if (i_T.size() < n)
				{
					return	sts = ns_consts::EnmReturnStatus::ErrorInsufficientData;
				}

				if (i_SA.size() < (n))
				{
					return	sts = ns_consts::EnmReturnStatus::ErrorInsufficientData;
				}

				std::valarray<uint32_t>			the_rank((uint32_t)0, n);
				io_LCP.resize(n, (uint32_t)0);

				// -------------------------------------------------------------------------- //
				// Step 2
				// -------------------------------------------------------------------------- //
				for (uint32_t i = 0; i < n; ++i)
				{
					// -------------------------------------------------------------------------- //
					// Step 3
					// -------------------------------------------------------------------------- //
					the_rank[i_SA[i]] = i;
				}
				// -------------------------------------------------------------------------- //
				// Step 4
				// -------------------------------------------------------------------------- //
				io_LCP[0] = 0;

				// -------------------------------------------------------------------------- //
				// Step 5
				// -------------------------------------------------------------------------- //
				uint32_t l = 0;

				// -------------------------------------------------------------------------- //
				// Step 6
				// -------------------------------------------------------------------------- //
				for (uint32_t i = 0; i < n; ++i)
				{
					// -------------------------------------------------------------------------- //
					// Step 7
					// -------------------------------------------------------------------------- //
					if (0 < the_rank[i])
					{
						// -------------------------------------------------------------------------- //
						// Step 8
						// -------------------------------------------------------------------------- //
						const uint32_t j = i_SA[the_rank[i] - 1];
						// -------------------------------------------------------------------------- //
						// Step 9. m = max {i, j}
						// -------------------------------------------------------------------------- //
						uint32_t m = i;
						if (i < j)
						{
							m = j;
						}
						// -------------------------------------------------------------------------- //
						// Step 10
						// -------------------------------------------------------------------------- //
						while (((m + l) < n) && (i_T[i + l] == i_T[j + l]))
						{
							// -------------------------------------------------------------------------- //
							// Step 11
							// -------------------------------------------------------------------------- //
							l++;
						}

						// -------------------------------------------------------------------------- //
						// Step 12
						// -------------------------------------------------------------------------- //
						io_LCP[the_rank[i]] = l;
						if (0 < l)
						{
							// -------------------------------------------------------------------------- //
							// Step 14
							// -------------------------------------------------------------------------- //
							l--;
						}
					}
				}

				// -------------------------------------------------------------------------- //
				// Step 15
				// -------------------------------------------------------------------------- //
				return	sts = ns_consts::EnmReturnStatus::Success;
			}
		}
	}
}