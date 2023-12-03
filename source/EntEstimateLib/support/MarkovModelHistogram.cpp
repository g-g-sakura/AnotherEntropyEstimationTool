////////////////////////////////////////////////////////////////////////////////
// MarkovModelHistogram.cpp
//
//
//
// Copyright (c) 2021-2023 G. G. SAKURAI <g.garland823@gmail.com>
//
////////////////////////////////////////////////////////////////////////////////
#include "../pch.h"
#include "MarkovModelHistogram.h"
#include <boost/range.hpp>

namespace entropy_estimator_lib
{
	namespace estimators
	{
		namespace support
		{

			// -------------------------------------------------------------------------- //
			/// <summary>
			/// </summary>
			/// <remarks>
			/// </remarks>
			/// <params="i_target_x">
			/// </params>
			/// <params="i_target_y">
			/// </params>
			/// <params="i_refHistogram">
			/// </params>
			/// <returns>
			/// </returns>
			/// <precondition>
			/// </precondition>
			/// <postcondition>
			/// </postcondition>
			// -------------------------------------------------------------------------- //
			ns_consts::EnmReturnStatus incrementXY(
				const boost::dynamic_bitset<>& i_target_x,
				ns_dt::octet i_target_y,
				MarkovModelHistogram& i_refHistogram)
			{
				ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorNotFound;

				ex_x_map& x_map_hg = i_refHistogram.get<ex_x>();

				const std::pair<ex_x_map::iterator, ex_x_map::iterator> rg_x = x_map_hg.equal_range(i_target_x);

				if (rg_x.first != rg_x.second)
				{
					sts = ns_consts::EnmReturnStatus::ErrorFirstIndexFoundButSecondIndexNotFound;
					for (ex_x_map::iterator it = rg_x.first; it != rg_x.second; ++it)
					{
						if (it->ex_y == i_target_y)
						{
							const int new_count = 1 + (it->ex_cnt);
							x_map_hg.replace(it, t_xy_bin(i_target_x, i_target_y, new_count));
							sts = ns_consts::EnmReturnStatus::Success;
							break;
						}
					}
				}
				else
				{
					sts = ns_consts::EnmReturnStatus::ErrorFirstIndexNotFound;
				}
				return	sts;
			}


			// -------------------------------------------------------------------------- //
			/// <summary>
			/// </summary>
			/// <remarks>
			/// </remarks>
			/// <params="o_refFrequent">
			/// </params>
			/// <params="i_target_x">
			/// </params>
			/// <params="i_refHistogram">
			/// </params>
			/// <returns>
			/// </returns>
			/// <precondition>
			/// </precondition>
			/// <postcondition>
			/// </postcondition>
			// -------------------------------------------------------------------------- //
			ns_consts::EnmReturnStatus getFrequent(t_xy_bin& o_refFrequent,
				const boost::dynamic_bitset<>& i_target_x,
				MarkovModelHistogram& i_refHistogram)
			{
				ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorUnexpected;

				const ex_x_map& x_map_hg = i_refHistogram.get<ex_x>();

				auto const range = x_map_hg.equal_range(i_target_x);

				if (range.first != range.second)
				{
					t_xy_bin	bin_frequent_candidate = *(range.first);
					for (auto& el : boost::make_iterator_range(range))
					{
						if (bin_frequent_candidate.ex_cnt < el.ex_cnt)
						{
							bin_frequent_candidate = el;
						}
						else if (bin_frequent_candidate.ex_cnt == el.ex_cnt)
						{
							if (bin_frequent_candidate.ex_y < el.ex_y)
							{
								bin_frequent_candidate = el;
							}
						}
					}
					o_refFrequent = bin_frequent_candidate;
					sts = ns_consts::EnmReturnStatus::Success;
				}
				else
				{
					sts = ns_consts::EnmReturnStatus::ErrorNotFound;
				}
				return sts;
			}

		}
	}
}