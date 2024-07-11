////////////////////////////////////////////////////////////////////////////////
// TupleHistogram.h
//
//
// Copyright (c) 2021-2023 G. G. SAKURAI <g.garland823@gmail.com>
//
////////////////////////////////////////////////////////////////////////////////
#if defined(_MSC_VER)
#pragma once
#endif

#ifndef ENTROPY_ESTIMATOR_LIB_TUPLE_HISTOGRAM_H
#define ENTROPY_ESTIMATOR_LIB_TUPLE_HISTOGRAM_H

#include "../constants.h"
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/dynamic_bitset.hpp>

namespace entropy_estimator_lib
{
	namespace estimators
	{
		namespace support
		{

			namespace ns_consts = entropy_estimator_lib::constants;
			namespace bmi = boost::multi_index;
			using boost::multi_index_container;

			struct t_bsx_bin
			{
				boost::dynamic_bitset<>		bs_x;		// x
				int				ex_cnt;		// count = number of occurrence of index value within a certain window

				t_bsx_bin(const boost::dynamic_bitset<> &i_x, int i_count) : bs_x(i_x), ex_cnt(i_count) {}

				bool operator <(t_bsx_bin const& i_refRight) const
				{
					// -------------------------------------------------------------------------- //
					// 
					// -------------------------------------------------------------------------- //
					if (ex_cnt != i_refRight.ex_cnt)
					{
						return	ex_cnt < i_refRight.ex_cnt;
					}
					else
					{
						return	bs_x < i_refRight.bs_x;
					}
				}
			};

			struct bsx_idx {};
			struct bsx_cnt {};

			typedef bmi::multi_index_container<
				t_bsx_bin,
				bmi::indexed_by<
				bmi::ordered_unique<bmi::identity<t_bsx_bin> >, //
				bmi::ordered_unique<bmi::tag<bsx_idx>, bmi::member<t_bsx_bin, boost::dynamic_bitset<>, &t_bsx_bin::bs_x> >,
				bmi::ordered_non_unique<bmi::tag<bsx_cnt>, bmi::member<t_bsx_bin, int, &t_bsx_bin::ex_cnt> >
				>
			> BitSetHistogram;

			typedef BitSetHistogram::index<bsx_idx>::type tpl_bsx_map;
			typedef BitSetHistogram::index<bsx_cnt>::type tpl_bscnt_map;

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
				BitSetHistogram& i_refHistogram);


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
				BitSetHistogram& i_refHistogram);


		}
	}
}
#endif