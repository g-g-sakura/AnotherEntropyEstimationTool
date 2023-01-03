////////////////////////////////////////////////////////////////////////////////
// MarkovModelHistogram.h
//
//
// Copyright (c) 2021-2022 G. G. SAKURAI <g.garland823@gmail.com>
//
////////////////////////////////////////////////////////////////////////////////
#if defined(_MSC_VER)
#pragma once
#endif

#ifndef __ENTROPY_ESTIMATOR_LIB_MARKOV_MODEL_HISTOGRAM_H__
#define __ENTROPY_ESTIMATOR_LIB_MARKOV_MODEL_HISTOGRAM_H__

#include "../EntropyEstimateData.h"
#include "../constants.h"
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/composite_key.hpp>
#include <boost/dynamic_bitset.hpp>
#include <boost/range.hpp>


namespace entropy_estimator_lib
{
	namespace estimators
	{
		namespace support
		{

			namespace ns_consts = entropy_estimator_lib::constants;
			namespace ns_dt = entropy_estimator_lib::data_types;
			namespace bmi = boost::multi_index;
			using boost::multi_index_container;

			struct t_xy_bin
			{
				boost::dynamic_bitset<>		ex_x;		// x
				ns_dt::octet	ex_y;		// y
				int				ex_cnt;		// count = number of occurence of index value within a certain window

				t_xy_bin(boost::dynamic_bitset<>& i_x, ns_dt::octet i_y, int i_count) : ex_x(i_x), ex_y(i_y), ex_cnt(i_count) {}

				bool operator <(t_xy_bin const& i_refRight) const
				{
					// -------------------------------------------------------------------------- //
					// 
					// -------------------------------------------------------------------------- //
					if (ex_x == i_refRight.ex_x)
					{
						if (ex_cnt == i_refRight.ex_cnt)
						{
							return	ex_y < i_refRight.ex_y;
						}
						else
						{
							return	ex_cnt < i_refRight.ex_cnt;
						}
					}
					else
					{
						return ex_x < i_refRight.ex_x;
					}
				}
			};

			struct ex_x {};
			struct ex_y {};
			struct ex_cnt {};
			struct ex_x_y {};

			typedef bmi::multi_index_container<
				t_xy_bin,
				bmi::indexed_by<
				bmi::ordered_unique<bmi::identity<t_xy_bin> >, //
				bmi::ordered_non_unique<bmi::tag<ex_x>, bmi::member<t_xy_bin, boost::dynamic_bitset<>, &t_xy_bin::ex_x> >,
				bmi::ordered_non_unique<bmi::tag<ex_y>, bmi::member<t_xy_bin, ns_dt::octet, &t_xy_bin::ex_y> >,
				bmi::ordered_non_unique<bmi::tag<ex_cnt>, bmi::member<t_xy_bin, int, &t_xy_bin::ex_cnt> >
				>
			> MarkovModelHistogram;

			typedef MarkovModelHistogram::index<ex_x>::type ex_x_map;
			typedef MarkovModelHistogram::index<ex_y>::type ex_y_map;
			typedef MarkovModelHistogram::index<ex_cnt>::type ex_cnt_map;

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
				boost::dynamic_bitset<>& i_target_x,
				ns_dt::octet i_target_y,
				MarkovModelHistogram& i_refHistogram);


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
				boost::dynamic_bitset<>& i_target_x,
				MarkovModelHistogram& i_refHistogram);



		}
	}
}
#endif