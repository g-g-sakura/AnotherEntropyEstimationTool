////////////////////////////////////////////////////////////////////////////////
// WindowedHistogram.cpp
//
//
//
// Copyright (c) 2021-2023 G. G. SAKURAI <g.garland823@gmail.com>
//
////////////////////////////////////////////////////////////////////////////////
#include "../pch.h"
#include "WindowedHistogram.h"

namespace entropy_estimator_lib
{
	namespace estimators
	{
		namespace support
		{

			typedef Histogram::index<cnt>::type cnt_map;

			// -------------------------------------------------------------------------- //
			/// <summary>
			/// </summary>
			/// <remarks>
			/// </remarks>
			/// <params="i_window_width">
			/// </params>
			/// <params="i_start_pos">
			/// </params>
			/// <params="i_end_pos">
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
			WindowedHistogram::WindowedHistogram(int i_window_width, int i_start_pos, int i_end_pos, const ns_dt::t_data_for_estimator& i_refData)
				: m_window_width(i_window_width), m_start_pos(i_start_pos), m_end_pos(i_end_pos)
			{
				if (i_end_pos < i_start_pos)
				{
					throw;
				}
				if (i_start_pos + i_window_width - 1 != i_end_pos)
				{
					throw;
				}
				for (int i = i_start_pos; i <= i_end_pos; ++i)
				{
					ns_dt::octet	x = (*i_refData.p_bzInputS)(i);

					// -------------------------------------------------------------------------- //
					// idx‚ðƒL[‚É‚µ‚ÄŒŸõ
					// -------------------------------------------------------------------------- //
					idx_map::iterator it = this->m_hg.get<idx>().find(x);
					if (it != this->m_hg.get<idx>().end()) {
						const int new_count = it->cnt + 1;
						this->m_hg.get<idx>().replace(it, t_bin(x, new_count, i));
					}
					else
					{
						this->m_hg.insert(t_bin((int)x, 1, i));
					}
				}
			}

			// -------------------------------------------------------------------------- //
			/// <summary>
			/// </summary>
			/// <remarks>
			/// </remarks>
			/// <params="i_refRight">
			/// </params>
			/// <returns>
			/// </returns>
			/// <precondition>
			/// </precondition>
			/// <postcondition>
			/// </postcondition>
			// -------------------------------------------------------------------------- //
			WindowedHistogram& WindowedHistogram::operator+=(WindowedHistogram const& i_refRight)
			{
				// -------------------------------------------------------------------------- //
				// 
				// -------------------------------------------------------------------------- //
				if (this->m_start_pos + this->m_window_width != i_refRight.m_start_pos)
				{
					throw;
				}
				// -------------------------------------------------------------------------- //
				// 
				// -------------------------------------------------------------------------- //
				idx_map& idx_map_out = this->m_hg.get<idx>();
				const idx_map& idx_map_right = i_refRight.m_hg.get<idx>();
				// -------------------------------------------------------------------------- //
				// 
				// -------------------------------------------------------------------------- //
				for (idx_map::reverse_iterator rit = idx_map_right.rbegin(); rit != idx_map_right.rend(); ++rit)
				{
					idx_map::iterator it_out = idx_map_out.find(rit->idx);
					if (it_out != idx_map_out.end()) {
						const int new_count = it_out->cnt + rit->cnt;
						int new_last_pos = rit->last_pos;
						if (rit->last_pos < it_out->last_pos)
						{
							new_last_pos = it_out->last_pos;
						}
						idx_map_out.replace(it_out, t_bin(rit->idx, new_count, new_last_pos));
					}
					else
					{
						idx_map_out.insert(t_bin(rit->idx, rit->cnt, rit->last_pos));
					}
				}
				// -------------------------------------------------------------------------- //
				// 
				// -------------------------------------------------------------------------- //
				this->m_window_width += i_refRight.m_window_width;
				this->m_end_pos = i_refRight.m_end_pos;
				// -------------------------------------------------------------------------- //
				// 
				// -------------------------------------------------------------------------- //
				return *this;
			}

			// -------------------------------------------------------------------------- //
			/// <summary>
			/// </summary>
			/// <remarks>
			/// </remarks>
			/// <params="o_refFrequent">
			/// </params>
			/// <returns>
			/// </returns>
			/// <precondition>
			/// </precondition>
			/// <postcondition>
			/// </postcondition>
			// -------------------------------------------------------------------------- //
			ns_consts::EnmReturnStatus WindowedHistogram::getFrequent(t_bin& o_refFrequent)
			{
				ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorUnexpected;
				// -------------------------------------------------------------------------- //
				// 
				// -------------------------------------------------------------------------- //
				cnt_map& cnt_map_hg = this->m_hg.get<cnt>();
				// -------------------------------------------------------------------------- //
				// 
				// -------------------------------------------------------------------------- //
				cnt_map::reverse_iterator rit = cnt_map_hg.rbegin();
				if (rit != cnt_map_hg.rend())
				{
					t_bin	bin_frequent_candidate = *rit;
					for (rit = cnt_map_hg.rbegin(); rit != cnt_map_hg.rend(); ++rit)
					{
						if (bin_frequent_candidate.cnt < rit->cnt)
						{
							bin_frequent_candidate = *rit;
						}
						else if (bin_frequent_candidate.cnt == rit->cnt)
						{
							if (bin_frequent_candidate.last_pos < rit->last_pos)
							{
								bin_frequent_candidate = *rit;
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
				// -------------------------------------------------------------------------- //
				// 
				// -------------------------------------------------------------------------- //
				return sts;
			}

			// -------------------------------------------------------------------------- //
			/// <summary>
			/// </summary>
			/// <remarks>
			/// </remarks>
			/// <params="os">
			/// </params>
			/// <params="i_refRight">
			/// </params>
			/// <returns>
			/// </returns>
			/// <precondition>
			/// </precondition>
			/// <postcondition>
			/// </postcondition>
			// -------------------------------------------------------------------------- //
			std::ostream& operator<< (std::ostream& os, WindowedHistogram & i_refRight)
			{
				cnt_map& cnt_map_hg = i_refRight.m_hg.get<cnt>();

				for (cnt_map::reverse_iterator rit = cnt_map_hg.rbegin(); rit != cnt_map_hg.rend(); ++rit)
				{
					os << "#\t\t";
					os << "(idx, cnt, last_pos) = ";
					os << "(" << (int)rit->idx << ",\t" << rit->cnt << ",\t" << rit->last_pos << ")" << std::endl;
				}
				return	os;
			}

			// -------------------------------------------------------------------------- //
			/// <summary>
			///  Constructor of <c>HistogramArray</c>
			/// </summary>
			/// <remarks>
			/// </remarks>
			/// <params="i_window_width">
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
			HistogramArray::HistogramArray(int i_window_width, 
				const ns_dt::t_data_for_estimator& i_refData)
				: m_window_width_l1(i_window_width)
			{
				// -------------------------------------------------------------------------- //
				// 
				// -------------------------------------------------------------------------- //
				if (i_window_width < 2)
				{
					throw;
				}
				// -------------------------------------------------------------------------- //
				// 
				// -------------------------------------------------------------------------- //
				const int num_of_windows = (i_refData.p_bzInputS->length(blitz::firstDim) + i_window_width - 1) / i_window_width;
				// -------------------------------------------------------------------------- //
				// 
				// -------------------------------------------------------------------------- //
				for (int j = 0; j < num_of_windows; ++j)
				{
					int current_window_size = i_window_width;
					if ((num_of_windows - 1) == j)
					{
						current_window_size = i_refData.p_bzInputS->length(blitz::firstDim) - j * i_window_width;
					}
					const int start_position = j * i_window_width;
					int end_position = (j + 1) * i_window_width - 1;
					if ((num_of_windows - 1) == j)
					{
						end_position = i_refData.p_bzInputS->length(blitz::firstDim) - 1;
					}
					this->m_hg_array.push_back(new WindowedHistogram(current_window_size, start_position, end_position, i_refData));
				}
			}

			// -------------------------------------------------------------------------- //
			/// <summary>
			/// </summary>
			/// <remarks>
			/// </remarks>
			/// <params="">
			/// </params>
			/// <returns>
			/// </returns>
			/// <precondition>
			/// </precondition>
			/// <postcondition>
			/// </postcondition>
			// -------------------------------------------------------------------------- //
			HistogramArray::~HistogramArray()
			{
				for (size_t j = 0; j < this->m_hg_array.size(); ++j)
				{
					WindowedHistogram* p_whg = this->m_hg_array[j];
					delete p_whg;
				}
			}

			// -------------------------------------------------------------------------- //
			/// <summary>
			/// </summary>
			/// <remarks>
			/// </remarks>
			/// <params="o_refFrequent">
			/// </params>
			/// <params="start_pos">
			/// </params>
			/// <params="end_pos">
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
			ns_consts::EnmReturnStatus HistogramArray::getFrequent(t_bin& o_refFrequent,
				int start_pos, 
				int end_pos, 
				const ns_dt::t_data_for_estimator& i_refData) const
			{
				ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorUnexpected;

				if (end_pos < start_pos)
				{
					return	sts = ns_consts::EnmReturnStatus::ErrorInvalidData;
				}
				// -------------------------------------------------------------------------- //
				// window indexes to look up this WindowedHistogram
				// -------------------------------------------------------------------------- //
				int index_start_window_l1 = (start_pos + this->m_window_width_l1 - 1) / this->m_window_width_l1;
				int index_end_window_l1 = (end_pos + 1) / this->m_window_width_l1 - 1;
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				if (index_start_window_l1 <= index_end_window_l1)
				{
					// -------------------------------------------------------------------------- //
					// if there is one or more windows
					// -------------------------------------------------------------------------- //
					WindowedHistogram	whg(*(this->m_hg_array.at(index_start_window_l1)));
					for (int j = index_start_window_l1 + 1; j <= index_end_window_l1; ++j)
					{
						// -------------------------------------------------------------------------- //
						// if there are two or more windows
						// -------------------------------------------------------------------------- //
						whg += WindowedHistogram(*(this->m_hg_array.at(j)));
					}
					// -------------------------------------------------------------------------- //
					// sizes before and after window(s)
					// -------------------------------------------------------------------------- //
					int size_pre_window_l1 = this->m_window_width_l1 * index_start_window_l1 - start_pos;
					int size_post_window_l1 = end_pos - (this->m_window_width_l1 * (index_end_window_l1 + 1) - 1);
					// -------------------------------------------------------------------------- //
					// if there is at least one sample after the window(s).
					// -------------------------------------------------------------------------- //
					if (0 < size_post_window_l1)
					{
						whg += WindowedHistogram(size_post_window_l1, this->m_window_width_l1 * (index_end_window_l1 + 1), end_pos, i_refData);
					}
					// -------------------------------------------------------------------------- //
					// if there is at least one sample before the window(s).
					// -------------------------------------------------------------------------- //
					if (0 < size_pre_window_l1)
					{
						WindowedHistogram	pre_whg(size_pre_window_l1, start_pos, this->m_window_width_l1 * index_start_window_l1 - 1, i_refData);
						pre_whg += whg;
						// -------------------------------------------------------------------------- //
						// if there is at least one sample before the window(s), call whg to get frequent value.
						// -------------------------------------------------------------------------- //
						sts = pre_whg.getFrequent(o_refFrequent);
					}
					else
					{
						// -------------------------------------------------------------------------- //
						// if there is no sample before the window(s), call whg to get frequent value.
						// -------------------------------------------------------------------------- //
						sts = whg.getFrequent(o_refFrequent);
					}
				}
				else
				{
					// -------------------------------------------------------------------------- //
					//
					// -------------------------------------------------------------------------- //
					WindowedHistogram	whg(end_pos - start_pos + 1, start_pos, end_pos, i_refData);
					sts = whg.getFrequent(o_refFrequent);
				}
				return sts;
			}

			// -------------------------------------------------------------------------- //
			/// <summary>
			/// </summary>
			/// <remarks>
			/// </remarks>
			/// <params="os">
			/// </params>
			/// <params="i_refRight">
			/// </params>
			/// <returns>
			/// </returns>
			/// <precondition>
			/// </precondition>
			/// <postcondition>
			/// </postcondition>
			// -------------------------------------------------------------------------- //
			std::ostream& operator<< (std::ostream& os, const HistogramArray& i_refRight)
			{
				for (size_t j = 0; j < i_refRight.m_hg_array.size(); ++j)
				{
					std::cout << "#" << j << "-th histogram:\t";
					std::cout << *(i_refRight.m_hg_array[j]);
					std::cout << std::endl;
				}
				return	os;
			}

			// -------------------------------------------------------------------------- //
			/// <summary>
			///  Constructor of <c>HistogramArrayL2</c>
			/// </summary>
			/// <remarks>
			/// </remarks>
			/// <params="i_window_width_1">
			/// </params>
			/// <params="i_window_width_2">
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
			HistogramArrayL2::HistogramArrayL2(int i_window_width_1, int i_window_width_2,
				const ns_dt::t_data_for_estimator& i_refData)
			{
				m_window_widths[0] = i_window_width_1;
				m_window_widths[1] = i_window_width_2;
				// -------------------------------------------------------------------------- //
				// 
				// -------------------------------------------------------------------------- //
				if (i_window_width_1 < 2)
				{
					throw;
				}
				if (i_window_width_2 < 2)
				{
					throw;
				}
				if (i_window_width_2 <= i_window_width_1)
				{
					throw;
				}
				if (0 != (i_window_width_2 % i_window_width_1))
				{
					// -------------------------------------------------------------------------- //
					/// here <c>i_window_width_2</c> is not integral multiple of <c>i_window_width_1</c>.
					// -------------------------------------------------------------------------- //
					throw;
				}
				// -------------------------------------------------------------------------- //
				// 
				// -------------------------------------------------------------------------- //
				for (int i = 0; i < 2; ++i)
				{
					const int* p_width = nullptr;
					std::vector<WindowedHistogram*>* p_whg = nullptr;
					switch (i)
					{
					case 0:
						p_width = &(m_window_widths[0]);
						p_whg = &(this->m_hg_array_l1);
						break;
					case 1:
						p_width = &(m_window_widths[1]);
						p_whg = &(this->m_hg_array_l2);
						break;
					default:
						// should not reach here
						throw;
					}
					// -------------------------------------------------------------------------- //
					// 
					// -------------------------------------------------------------------------- //
					const int num_of_windows = (i_refData.p_bzInputS->length(blitz::firstDim) + (*p_width) - 1) / (*p_width);
					// -------------------------------------------------------------------------- //
					// 
					// -------------------------------------------------------------------------- //
					for (int j = 0; j < num_of_windows; ++j)
					{
						int current_window_size = (*p_width);
						if ((num_of_windows - 1) == j)
						{
							current_window_size = i_refData.p_bzInputS->length(blitz::firstDim) - j * (*p_width);
						}
						const int start_position = j * (*p_width);
						int end_position = (j + 1) * (*p_width) - 1;
						if ((num_of_windows - 1) == j)
						{
							end_position = i_refData.p_bzInputS->length(blitz::firstDim) - 1;
						}
						p_whg->push_back(new WindowedHistogram(current_window_size, start_position, end_position, i_refData));
					}
				}
			}

			// -------------------------------------------------------------------------- //
			/// <summary>
			/// </summary>
			/// <remarks>
			/// </remarks>
			/// <params="">
			/// </params>
			/// <returns>
			/// </returns>
			/// <precondition>
			/// </precondition>
			/// <postcondition>
			/// </postcondition>
			// -------------------------------------------------------------------------- //
			HistogramArrayL2::~HistogramArrayL2()
			{
				// -------------------------------------------------------------------------- //
				// 
				// -------------------------------------------------------------------------- //
				for (int i = 0; i < 2; ++i)
				{
					const int* p_width = nullptr;
					std::vector<WindowedHistogram*>* p_vwhg = nullptr;
					switch (i)
					{
					case 0:
						p_width = &(m_window_widths[0]);
						p_vwhg = &(this->m_hg_array_l1);
						break;
					case 1:
						p_width = &(m_window_widths[1]);
						p_vwhg = &(this->m_hg_array_l2);
						break;
					default:
						// should not reach here
						break;
					}
					// -------------------------------------------------------------------------- //
					// 
					// -------------------------------------------------------------------------- //
					for (size_t j = 0; j < p_vwhg->size(); ++j)
					{
						WindowedHistogram* p_whg = (*p_vwhg)[j];
						delete p_whg;
					}
				}
			}

			// -------------------------------------------------------------------------- //
			/// <summary>
			/// </summary>
			/// <remarks>
			/// </remarks>
			/// <params="o_refFrequent">
			/// </params>
			/// <params="start_pos">
			/// </params>
			/// <params="end_pos">
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
			ns_consts::EnmReturnStatus HistogramArrayL2::getFrequent(t_bin& o_refFrequent,
				int start_pos,
				int end_pos,
				const ns_dt::t_data_for_estimator& i_refData) const
			{
				ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorUnexpected;
				// -------------------------------------------------------------------------- //
				// 
				// -------------------------------------------------------------------------- //
				if (end_pos < start_pos)
				{
					return	sts = ns_consts::EnmReturnStatus::ErrorInvalidData;
				}
				if (this->m_window_widths[0] < 1)
				{
					return	sts = ns_consts::EnmReturnStatus::ErrorInvalidData;
				}
				if (this->m_window_widths[1] < 1)
				{
					return	sts = ns_consts::EnmReturnStatus::ErrorInvalidData;
				}
				// -------------------------------------------------------------------------- //
				// 
				// -------------------------------------------------------------------------- //
				const int multiplier = this->m_window_widths[1] / this->m_window_widths[0];
				// -------------------------------------------------------------------------- //
				// window indexes to look up this WindowedHistogram
				// layer 2
				// -------------------------------------------------------------------------- //
				const int index_start_window_l2 = (start_pos + this->m_window_widths[1] - 1) / this->m_window_widths[1];
				const int index_end_window_l2 = (end_pos + 1) / this->m_window_widths[1] - 1;
				// -------------------------------------------------------------------------- //
				// layer 1
				// -------------------------------------------------------------------------- //
				const int index_start_window_l1 = (start_pos + this->m_window_widths[0] - 1) / this->m_window_widths[0];
				int index_non_overwrapping_end_window_l1 = multiplier * index_start_window_l2 - 1;
				int index_non_overwrapping_start_window_l1 = multiplier * (index_end_window_l2 + 1);
				const int index_end_window_l1 = (end_pos + 1) / this->m_window_widths[0] - 1;
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				WindowedHistogram*	p_wh = nullptr;
				// -------------------------------------------------------------------------- //
				//
				// -------------------------------------------------------------------------- //
				if (index_start_window_l1 <= index_end_window_l1)
				{
					// -------------------------------------------------------------------------- //
					// sizes before and after window(s)
					// -------------------------------------------------------------------------- //
					const int size_pre_window_l1 = this->m_window_widths[0] * index_start_window_l1 - start_pos;
					const int size_post_window_l1 = end_pos - (this->m_window_widths[0] * (index_end_window_l1 + 1) - 1);
					// -------------------------------------------------------------------------- //
					// if there is at least one sample before the window(s).
					// -------------------------------------------------------------------------- //
					if (0 < size_pre_window_l1)
					{
						p_wh = new WindowedHistogram(size_pre_window_l1, start_pos, this->m_window_widths[0] * index_start_window_l1 - 1, i_refData);
					}
					// -------------------------------------------------------------------------- //
					// if there is one or more windows
					// -------------------------------------------------------------------------- //
					if (index_start_window_l2 <= index_end_window_l2)
					{
						for (int j = index_start_window_l1; j <= index_non_overwrapping_end_window_l1; ++j)
						{
							// -------------------------------------------------------------------------- //
							// if there are two or more windows
							// -------------------------------------------------------------------------- //
							if (nullptr == p_wh)
							{
								p_wh = new WindowedHistogram(*(this->m_hg_array_l1.at(j)));
							}
							else
							{
								(*p_wh) += (*(this->m_hg_array_l1.at(j)));
							}
						}
						for (int j = index_start_window_l2; j <= index_end_window_l2; ++j)
						{
							// -------------------------------------------------------------------------- //
							// if there are two or more windows
							// -------------------------------------------------------------------------- //
							if (nullptr == p_wh)
							{
								p_wh = new WindowedHistogram(*(this->m_hg_array_l2.at(j)));
							}
							else
							{
								(*p_wh) += (*(this->m_hg_array_l2.at(j)));
							}
						}
						for (int j = index_non_overwrapping_start_window_l1; j <= index_end_window_l1; ++j)
						{
							// -------------------------------------------------------------------------- //
							// if there are two or more windows
							// -------------------------------------------------------------------------- //
							if (nullptr == p_wh)
							{
								p_wh = new WindowedHistogram(*(this->m_hg_array_l1.at(j)));
							}
							else
							{
								(*p_wh) += (*(this->m_hg_array_l1.at(j)));
							}
						}
					}
					else
					{
						for (int j = index_start_window_l1; j <= index_end_window_l1; ++j)
						{
							// -------------------------------------------------------------------------- //
							// if there are two or more windows
							// -------------------------------------------------------------------------- //
							if (nullptr == p_wh)
							{
								p_wh = new WindowedHistogram(*(this->m_hg_array_l1.at(j)));
							}
							else
							{
								(*p_wh) += (*(this->m_hg_array_l1.at(j)));
							}
						}
					}
					// -------------------------------------------------------------------------- //
					// if there is at least one sample after the window(s).
					// -------------------------------------------------------------------------- //
					if (0 < size_post_window_l1)
					{
						(*p_wh) += WindowedHistogram(size_post_window_l1, this->m_window_widths[0] * (index_end_window_l1 + 1), end_pos, i_refData);
					}
				}
				else
				{
					// -------------------------------------------------------------------------- //
					//
					// -------------------------------------------------------------------------- //
					p_wh = new WindowedHistogram(end_pos - start_pos + 1, start_pos, end_pos, i_refData);
				}
				if (nullptr != p_wh)
				{
					sts = p_wh->getFrequent(o_refFrequent);
					delete p_wh;
					p_wh = nullptr;
				}
				else
				{
					sts = ns_consts::EnmReturnStatus::ErrorNullPointer;
				}
				return sts;
			}

			// -------------------------------------------------------------------------- //
			/// <summary>
			/// </summary>
			/// <remarks>
			/// </remarks>
			/// <params="os">
			/// </params>
			/// <params="i_refRight">
			/// </params>
			/// <returns>
			/// </returns>
			/// <precondition>
			/// </precondition>
			/// <postcondition>
			/// </postcondition>
			// -------------------------------------------------------------------------- //
			std::ostream& operator<< (std::ostream& os, HistogramArrayL2& i_refRight)
			{
				// -------------------------------------------------------------------------- //
				// 
				// -------------------------------------------------------------------------- //
				for (int i = 0; i < 2; ++i)
				{
					const int* p_width = nullptr;
					std::vector<WindowedHistogram*>* p_vwhg = nullptr;
					switch (i)
					{
					case 0:
						p_width = &(i_refRight.m_window_widths[0]);
						p_vwhg = &(i_refRight.m_hg_array_l1);
						break;
					case 1:
						p_width = &(i_refRight.m_window_widths[1]);
						p_vwhg = &(i_refRight.m_hg_array_l2);
						break;
					default:
						// should not reach here
						throw;
					}
					// -------------------------------------------------------------------------- //
					// 
					// -------------------------------------------------------------------------- //
					std::cout << "#" << std::endl;
					std::cout << "# information of " << (i + 1) << "-th layer histogram" << std::endl;
					std::cout << "#\t" << (i + 1) << "-th layer window size:\t" << i_refRight.m_window_widths[i] << std::endl;
					std::cout << "#" << std::endl;
					// -------------------------------------------------------------------------- //
					// 
					// -------------------------------------------------------------------------- //
					for (size_t j = 0; j < p_vwhg->size(); ++j)
					{
						std::cout << "# " << j << "-th histogram:\t" << std::endl;
						std::cout << *((*p_vwhg)[j]);
						std::cout << std::endl;
					}
				}
				return	os;
			}

		}
	}
}