////////////////////////////////////////////////////////////////////////////////
// WindowedHistogram.h
//
//
// Copyright (c) 2021-2022 G. G. SAKURAI <g.garland823@gmail.com>
//
////////////////////////////////////////////////////////////////////////////////
#if defined(_MSC_VER)
#pragma once
#endif

#ifndef __ENTROPY_ESTIMATOR_LIB_WINDOWED_HISTOGRAM_H__
#define __ENTROPY_ESTIMATOR_LIB_WINDOWED_HISTOGRAM_H__

#include "../EntropyEstimateData.h"
#include "../constants.h"
#include "IHistogram.h"
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/composite_key.hpp>
#include <boost/multiprecision/cpp_int.hpp>
#include <vector>

namespace entropy_estimator_lib
{
	namespace estimators
	{
		namespace support
		{
			namespace ns_consts = entropy_estimator_lib::constants;
			namespace ns_dt = entropy_estimator_lib::data_types;
			namespace ns_spt = entropy_estimator_lib::support;
			namespace bmi = boost::multi_index;
			using boost::multi_index_container;

			struct t_bin
			{
				int				idx;		// index
				int				cnt;		// count = number of occurence of index value within a certain window
				int				last_pos;	// last position

				t_bin(int i_index, int i_count, int i_last_pos) : idx(i_index), cnt(i_count), last_pos(i_last_pos) {}

				bool operator <(t_bin const& i_refRight) const
				{
					if (cnt == i_refRight.cnt)
					{
						if (last_pos != i_refRight.last_pos)
						{
							return last_pos < i_refRight.last_pos;
						}
						else
						{
							return idx < i_refRight.idx;
						}
					}
					else
					{
						return cnt < i_refRight.cnt;
					}
				}
			};

			struct idx {};
			struct cnt {};
			struct last_pos {};

			typedef bmi::multi_index_container<
				t_bin,
				bmi::indexed_by<
				bmi::ordered_unique<bmi::identity<t_bin> >, //
				bmi::ordered_unique<bmi::tag<idx>, bmi::member<t_bin, int, &t_bin::idx> >,
				bmi::ordered_non_unique<bmi::tag<cnt>, bmi::member<t_bin, int, &t_bin::cnt> >,
				bmi::ordered_non_unique<bmi::tag<last_pos>, bmi::member<t_bin, int, &t_bin::last_pos> >
				>
			> Histogram;

			typedef Histogram::index<idx>::type idx_map;

			class WindowedHistogram
			{
			public:
				int		m_window_width;
				int		m_start_pos;
				int		m_end_pos;
				Histogram	m_hg;

				WindowedHistogram(int window_width, int start_pos, int end_pos, const ns_dt::t_data_for_estimator& i_refData);
				~WindowedHistogram(void) {};

				WindowedHistogram& operator+=(const WindowedHistogram & i_refRight);

				ns_consts::EnmReturnStatus getFrequent(t_bin& o_refFrequent);

				friend std::ostream & operator<< (std::ostream& os, WindowedHistogram & i_refRight);
			private:
			};

			class HistogramArray : public ns_spt::IHistogram<t_bin>
			{
			public:
				int		m_window_width_l1;
				std::vector<WindowedHistogram*>	m_hg_array;

				HistogramArray(int window_width, const ns_dt::t_data_for_estimator& i_refData);
				~HistogramArray(void);

				ns_consts::EnmReturnStatus getFrequent(t_bin& o_refFrequent, int start_pos, int end_pos, const ns_dt::t_data_for_estimator& i_refData) const;

				friend std::ostream& operator<< (std::ostream& os, HistogramArray& i_refRight);
			};

			class HistogramArrayL2 : public ns_spt::IHistogram<t_bin>
			{
			public:
				int		m_window_widths[2];
				std::vector<WindowedHistogram*>	m_hg_array_l1;
				std::vector<WindowedHistogram*>	m_hg_array_l2;

				HistogramArrayL2(int i_window_width_1, int i_window_width_2, const ns_dt::t_data_for_estimator& i_refData);
				~HistogramArrayL2(void);

				ns_consts::EnmReturnStatus getFrequent(t_bin& o_refFrequent, int start_pos, int end_pos, const ns_dt::t_data_for_estimator& i_refData) const;

				friend std::ostream& operator<< (std::ostream& os, HistogramArrayL2& i_refRight);
			};
		}
	}
}
#endif