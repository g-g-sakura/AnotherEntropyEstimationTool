////////////////////////////////////////////////////////////////////////////////
// LongestCommonPrefix.h
//
//
// Copyright (c) 2025 G. G. SAKURAI <g.garland823@gmail.com>
//
////////////////////////////////////////////////////////////////////////////////
#if defined(_MSC_VER)
#pragma once
#endif

#ifndef ENTROPY_ESTIMATOR_LIB_LONGEST_COMMON_PREFIX_H
#define ENTROPY_ESTIMATOR_LIB_LONGEST_COMMON_PREFIX_H

#include "../constants.h"
#include <valarray>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/member.hpp>

namespace entropy_estimator_lib
{
	namespace estimators
	{
		namespace support
		{
			typedef struct _tsubstrrank
			{
				int32_t	left_rank;
				int32_t	right_rank;
				uint32_t	index; // $ index \in [1, n] $

				bool operator <(_tsubstrrank const& i_refRight) const
				{
					// -------------------------------------------------------------------------- //
					// 
					// -------------------------------------------------------------------------- //
					if (left_rank == i_refRight.left_rank)
					{
						return	right_rank < i_refRight.right_rank;
					}
					else
					{
						return left_rank < i_refRight.left_rank;
					}
				}
			} t_substr_rank;

			namespace ns_consts = entropy_estimator_lib::constants;

			typedef std::valarray<unsigned char>	octetarray;

			typedef std::valarray<uint32_t>	intarray;
			typedef std::valarray<uint64_t>	llarray;
			typedef std::valarray<double>	dblarray;

			typedef struct _tdata_for_tuple_counting
			{
				int		eta_prev;
				int		eta_current;
				uint32_t	eta_max;
				uint32_t	t;
				uint32_t	lambda;

				intarray* p_LCP;

				intarray* p_qtilde_master;
				intarray* p_qtilde_work;

			} t_data_for_tuple_counting;

			ns_consts::EnmReturnStatus ComputeSuffixArray(std::valarray<uint32_t>& io_SA, const octetarray& i_T, uint32_t n);

			ns_consts::EnmReturnStatus ComputeLCP(std::valarray<uint32_t>& io_LCP, const octetarray& i_T, const std::valarray<uint32_t>& i_SA, uint32_t n);

			namespace bmi = boost::multi_index;
			using boost::multi_index_container;

			struct t_lcp_bin
			{
				uint32_t		ex_W;		// W
				int				ex_num_occ;		// count = number of occurrence of index value within a certain window
				uint32_t		ex_multi;	// multiplicity

				t_lcp_bin(const uint32_t i_w, const int i_num_occ, const uint32_t i_multi) : ex_W(i_w), ex_num_occ(i_num_occ), ex_multi(i_multi) {}

				bool operator <(t_lcp_bin const& i_refRight) const
				{
					// -------------------------------------------------------------------------- //
					// 
					// -------------------------------------------------------------------------- //
					if (ex_W == i_refRight.ex_W)
					{
						if (ex_num_occ == i_refRight.ex_num_occ)
						{
							return	ex_multi > i_refRight.ex_multi;
						}
						else
						{
							return	ex_num_occ < i_refRight.ex_num_occ;
						}
					}
					else
					{
						return	ex_W < i_refRight.ex_W;
					}
				}
			};

			struct lcp_idx {};
			struct lcp_cnt {};
			struct lcp_multi {};

			typedef bmi::multi_index_container<
				t_lcp_bin,
				bmi::indexed_by<
				bmi::ordered_unique<bmi::identity<t_lcp_bin> >, //
				bmi::ordered_non_unique<bmi::tag<lcp_idx>, bmi::member<t_lcp_bin, uint32_t, &t_lcp_bin::ex_W> >,
				bmi::ordered_non_unique<bmi::tag<lcp_cnt>, bmi::member<t_lcp_bin, int, &t_lcp_bin::ex_num_occ> >
				>
			> LCPHistogram;

			typedef LCPHistogram::index<lcp_idx>::type lcp_idx_map;
			typedef LCPHistogram::index<lcp_cnt>::type lcp_cnt_map;

			typedef struct _tdata_for_LRS
			{
				int		eta_prev;
				int		eta_current;
				uint32_t	eta_max;

				uint32_t	nu;

				uint32_t	lambda;

				intarray* p_LCP;

				intarray* p_qtilde_master;
				intarray* p_qtilde_work;

				dblarray* p_sigma_dbl;

				LCPHistogram* p_hg;

			} t_data_for_LRS;


		}
	}
}

#endif