////////////////////////////////////////////////////////////////////////////////
// TupleHistogram.cpp
//
//
//
// Copyright (c) 2021-2025 G. G. SAKURAI <g.garland823@gmail.com>
//
////////////////////////////////////////////////////////////////////////////////
#include "TupleHistogram.h"

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
			/// <param name="o_refUpdatedCount">
			/// </param>
			/// <param name="i_target_x">
			/// </param>
			/// <param name="i_refHistogram">
			/// </param>
			/// <returns>
			/// </returns>
			/// <precondition>
			/// </precondition>
			/// <postcondition>
			/// </postcondition>
			// -------------------------------------------------------------------------- //
			ns_consts::EnmReturnStatus incrementCount(
				int& o_refUpdatedCount,
				const boost::dynamic_bitset<>& i_target_x,
				BitSetHistogram& i_refHistogram)
			{
				ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorNotFound;

				tpl_bsx_map& x_map_hg = i_refHistogram.get<bsx_idx>();

				const tpl_bsx_map::iterator it = x_map_hg.find(i_target_x);

				if (it != x_map_hg.cend())
				{
					const int new_count = 1 + (it->ex_cnt);
					x_map_hg.replace(it, t_bsx_bin(i_target_x, new_count));
					o_refUpdatedCount = new_count;
					sts = ns_consts::EnmReturnStatus::Success;
				}
				else
				{
					sts = ns_consts::EnmReturnStatus::ErrorNotFound;
				}
				return	sts;
			}


			// -------------------------------------------------------------------------- //
			/// <summary>
			/// </summary>
			/// <remarks>
			/// </remarks>
			/// <param name="o_refFrequent">
			/// </param>
			/// <param name="i_refHistogram">
			/// </param>
			/// <returns>
			/// </returns>
			/// <precondition>
			/// </precondition>
			/// <postcondition>
			/// </postcondition>
			// -------------------------------------------------------------------------- //
			ns_consts::EnmReturnStatus getFrequent(t_bsx_bin& o_refFrequent,
				BitSetHistogram& i_refHistogram)
			{
				ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorUnexpected;

				tpl_bscnt_map& cnt_map_hg = i_refHistogram.get<bsx_cnt>();

				const tpl_bscnt_map::reverse_iterator	rit = cnt_map_hg.rbegin();
				if (rit != cnt_map_hg.rend())
				{
					o_refFrequent = *rit;
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