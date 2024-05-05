////////////////////////////////////////////////////////////////////////////////
// setUp.cpp
//
//
//
// Copyright (c) 2021-2023 G. G. SAKURAI <g.garland823@gmail.com>
//
////////////////////////////////////////////////////////////////////////////////
#include "../pch.h"
#include "setUp.h"
#include <boost/filesystem.hpp>
#include <fstream>

namespace entropy_estimator_lib
{
	namespace support
	{
		namespace ns_consts = entropy_estimator_lib::constants;
		namespace ns_dt = entropy_estimator_lib::data_types;
		namespace bs_fs = boost::filesystem;

		// -------------------------------------------------------------------------- //
		/// <summary>
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
		ns_consts::EnmReturnStatus setUp(ns_dt::t_data_for_estimator& io_refData)
		{
			ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorUnexpected;
			// -------------------------------------------------------------------------- //
			//
			// -------------------------------------------------------------------------- //
			io_refData.p_bzInputS = nullptr;
			io_refData.p_bzSampleSpaceA = nullptr;
			// -------------------------------------------------------------------------- //
			//
			// -------------------------------------------------------------------------- //
			io_refData.p_bzInputS = new blitz::Array<ns_dt::octet, 1>(1000 * 1000);
			io_refData.p_bzSampleSpaceA = new blitz::Array<ns_dt::octet, 1>(256);
			io_refData.t_6_3_1 = { 0 };
			io_refData.t_6_3_2 = { 0 };
			io_refData.t_6_3_3 = { 0 };
			io_refData.t_6_3_4 = { 0 };
			io_refData.t_6_3_4.b = 6;
			// -------------------------------------------------------------------------- //
			//
			// -------------------------------------------------------------------------- //
			io_refData.verbose_level = 1;
			// -------------------------------------------------------------------------- //
			//
			// -------------------------------------------------------------------------- //
			if (false == io_refData.isModeDemonstration)
			{
				io_refData.t_6_3_4.d = 1000;
			}
			else
			{
				// -------------------------------------------------------------------------- //
				// for demonstrating examples in NIST SP 800-90B 6.3.x
				// -------------------------------------------------------------------------- //
				io_refData.t_6_3_4.d = 4;
			}
			io_refData.t_6_3_5 = { 0 };
			if (false == io_refData.isModeDemonstration)
			{
				io_refData.t_6_3_5.cutoff = 35;
			}
			else
			{
				// -------------------------------------------------------------------------- //
				// for demonstrating examples in NIST SP 800-90B 6.3.x
				// -------------------------------------------------------------------------- //
				io_refData.t_6_3_5.cutoff = 3;
			}
			io_refData.t_6_3_6 = { 0 };
			if (false == io_refData.isModeDemonstration)
			{
				io_refData.t_6_3_6.cutoff = 35;
			}
			else
			{
				// -------------------------------------------------------------------------- //
				// for demonstrating examples in NIST SP 800-90B 6.3.x
				// -------------------------------------------------------------------------- //
				io_refData.t_6_3_6.cutoff = 3;
			}
			io_refData.t_6_3_7 = { 0 };
			if (false == io_refData.isModeDemonstration)
			{
				io_refData.t_6_3_7.w[0] = 63;
				io_refData.t_6_3_7.w[1] = 255;
				io_refData.t_6_3_7.w[2] = 1023;
				io_refData.t_6_3_7.w[3] = 4095;
			}
			else
			{
				// -------------------------------------------------------------------------- //
				// for demonstrating examples in NIST SP 800-90B 6.3.x
				// -------------------------------------------------------------------------- //
				io_refData.t_6_3_7.w[0] = 3;
				io_refData.t_6_3_7.w[1] = 5;
				io_refData.t_6_3_7.w[2] = 7;
				io_refData.t_6_3_7.w[3] = 9;
			}
			io_refData.t_6_3_8 = { 0 };
			if (false == io_refData.isModeDemonstration)
			{
				io_refData.t_6_3_8.D = 128;
			}
			else
			{
				// -------------------------------------------------------------------------- //
				// for demonstrating examples in NIST SP 800-90B 6.3.x
				// -------------------------------------------------------------------------- //
				io_refData.t_6_3_8.D = 3;
			}
			io_refData.t_6_3_9 = { 0 };
			if (false == io_refData.isModeDemonstration)
			{
				io_refData.t_6_3_9.D = 16;
			}
			else
			{
				// -------------------------------------------------------------------------- //
				// for demonstrating examples in NIST SP 800-90B 6.3.x
				// -------------------------------------------------------------------------- //
				io_refData.t_6_3_9.D = 3;
			}
			io_refData.t_6_3_10 = { 0 };
			if (false == io_refData.isModeDemonstration)
			{
				io_refData.t_6_3_10.B = 16;
			}
			else
			{
				// -------------------------------------------------------------------------- //
				// for demonstrating examples in NIST SP 800-90B 6.3.x
				// -------------------------------------------------------------------------- //
				io_refData.t_6_3_10.B = 4;
			}
			io_refData.t_6_3_10.maxDictionarySize = 65536;
			// -------------------------------------------------------------------------- //
			//
			// -------------------------------------------------------------------------- //
			// -------------------------------------------------------------------------- //
			//
			// -------------------------------------------------------------------------- //
			return sts = ns_consts::EnmReturnStatus::Success;
		}

		// -------------------------------------------------------------------------- //
		/// <summary>
		/// </summary>
		/// <remarks>
		/// </remarks>
		/// <params="io_refData">
		/// </params>
		/// <params="i_refFullPath">
		/// </params>
		/// <returns>
		/// </returns>
		/// <precondition>
		/// </precondition>
		/// <postcondition>
		/// </postcondition>
		// -------------------------------------------------------------------------- //
		ns_consts::EnmReturnStatus loadSamples(ns_dt::t_data_for_estimator& io_refData,
			const bs_fs::path& i_refFullPath)
		{
			ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorUnexpected;

			// -------------------------------------------------------------------------- //
			//
			// -------------------------------------------------------------------------- //
			if (io_refData.p_bzInputS == nullptr)
			{
				return	sts = ns_consts::EnmReturnStatus::ErrorNullPointer;
			}
			const boost::uintmax_t size = bs_fs::file_size(i_refFullPath);
			// -------------------------------------------------------------------------- //
			//
			// -------------------------------------------------------------------------- //
			io_refData.p_bzInputS->resize(size);
			(*io_refData.p_bzInputS) = 0;
			// -------------------------------------------------------------------------- //
			//
			// -------------------------------------------------------------------------- //
			std::ifstream	file(i_refFullPath.wstring(), std::ios::in | std::ios::binary);
			if (!file)
			{
				return sts = ns_consts::EnmReturnStatus::ErrorFileIO;
			}
			// -------------------------------------------------------------------------- //
			//
			// -------------------------------------------------------------------------- //
			file.read((char*)io_refData.p_bzInputS->data(), size);
			// -------------------------------------------------------------------------- //
			//
			// -------------------------------------------------------------------------- //
			io_refData.p_bzInputS->resizeAndPreserve(size);
			// -------------------------------------------------------------------------- //
			//
			// -------------------------------------------------------------------------- //
			io_refData.L = size;
			// -------------------------------------------------------------------------- //
			//
			// -------------------------------------------------------------------------- //
			return sts = ns_consts::EnmReturnStatus::Success;
		}

		// -------------------------------------------------------------------------- //
		/// <summary>
		/// </summary>
		/// <remarks>
		/// </remarks>
		/// <params="io_refData">
		/// </params>
		/// <params="i_refFullPath">
		/// </params>
		/// <params="i_number_of_bits_to_be_loaded">
		/// </params>
		/// <params="i_bits_per_sample">
		/// </params>
		/// <returns>
		/// </returns>
		/// <precondition>
		/// </precondition>
		/// <postcondition>
		/// </postcondition>
		// -------------------------------------------------------------------------- //
		ns_consts::EnmReturnStatus loadSamplesByInterpretation(ns_dt::t_data_for_estimator& io_refData,
			const bs_fs::path& i_refFullPath,
			uintmax_t i_number_of_bits_to_be_loaded,
			unsigned int i_bits_per_sample)
		{
			ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorUnexpected;

			// -------------------------------------------------------------------------- //
			//
			// -------------------------------------------------------------------------- //
			if (io_refData.p_bzInputS == nullptr)
			{
				return	sts = ns_consts::EnmReturnStatus::ErrorNullPointer;
			}
			if (io_refData.p_bzSampleSpaceA == nullptr)
			{
				return	sts = ns_consts::EnmReturnStatus::ErrorNullPointer;
			}
			// -------------------------------------------------------------------------- //
			//
			// -------------------------------------------------------------------------- //
			if ((i_bits_per_sample <= 1) || (8 < i_bits_per_sample))
			{
				return	sts = ns_consts::EnmReturnStatus::ErrorDomain;
			}
			const boost::uintmax_t size = bs_fs::file_size(i_refFullPath);
			// -------------------------------------------------------------------------- //
			//
			// -------------------------------------------------------------------------- //
			uintmax_t size_to_read = (i_number_of_bits_to_be_loaded + i_bits_per_sample - 1) / i_bits_per_sample;
			if (size < size_to_read)
			{
				// -------------------------------------------------------------------------- //
				// warning, i.e. all bits in the bitstring is tested
				// -------------------------------------------------------------------------- //
				size_to_read = size;
			}
			else
			{
				// -------------------------------------------------------------------------- //
				// truncate
				// -------------------------------------------------------------------------- //

			}
			// -------------------------------------------------------------------------- //
			//
			// -------------------------------------------------------------------------- //
			blitz::Array<ns_dt::octet, 1>	bz_original(size_to_read);
			bz_original = 0;
			// -------------------------------------------------------------------------- //
			//
			// -------------------------------------------------------------------------- //
			std::ifstream	file(i_refFullPath.wstring(), std::ios::in | std::ios::binary);
			if (!file)
			{
				return sts = ns_consts::EnmReturnStatus::ErrorFileIO;
			}
			// -------------------------------------------------------------------------- //
			//
			// -------------------------------------------------------------------------- //
			file.read((char*)bz_original.data(), size_to_read);
			// -------------------------------------------------------------------------- //
			//
			// -------------------------------------------------------------------------- //
			io_refData.p_bzInputS->resize(size_to_read * i_bits_per_sample);
			*(io_refData.p_bzInputS) = 0;
			// -------------------------------------------------------------------------- //
			//
			// -------------------------------------------------------------------------- //
			if (io_refData.bIsMSbFirstByteBitConversion == true)
			{
				// -------------------------------------------------------------------------- //
				// MSb first
				// -------------------------------------------------------------------------- //
				for (uintmax_t i = 0; i < size_to_read; ++i)
				{
					for (unsigned int j = 0; j < i_bits_per_sample; ++j)
					{
						// -------------------------------------------------------------------------- //
						// assuming MSb first
						// -------------------------------------------------------------------------- //
						const int shift_width = (static_cast<int>(i_bits_per_sample) - 1) - static_cast<int>(j);
						if (shift_width < 0)
						{
							return	sts;
						}
						// -------------------------------------------------------------------------- //
						/// here, <c>shift_witdth</c> is greater than or equal to 0
						// -------------------------------------------------------------------------- //
						(*(io_refData.p_bzInputS))(i_bits_per_sample * i + j) = (bz_original(i) >> shift_width) & 0x01;
					}
				}
			}
			else
			{
				// -------------------------------------------------------------------------- //
				// LSb first
				// -------------------------------------------------------------------------- //
				for (uintmax_t i = 0; i < size_to_read; ++i)
				{
					for (unsigned int j = 0; j < i_bits_per_sample; ++j)
					{
						// -------------------------------------------------------------------------- //
						// assuming LSb first
						// -------------------------------------------------------------------------- //
						const unsigned int shift_width = j;
						// -------------------------------------------------------------------------- //
						/// here, <c>shift_witdth</c> is greater than or equal to 0
						// -------------------------------------------------------------------------- //
						(*(io_refData.p_bzInputS))(i_bits_per_sample * i + j) = (bz_original(i) >> shift_width) & 0x01;
					}
				}
			}
			// -------------------------------------------------------------------------- //
			//
			// -------------------------------------------------------------------------- //
			bz_original.resize(0);
			io_refData.L = size_to_read * i_bits_per_sample;
			// -------------------------------------------------------------------------- //
			//
			// -------------------------------------------------------------------------- //
			io_refData.k = 2;
			io_refData.p_bzSampleSpaceA->resize(2);
			(*io_refData.p_bzSampleSpaceA)(0) = 0;
			(*io_refData.p_bzSampleSpaceA)(1) = 1;
			io_refData.bits_per_sample = 1;
			// -------------------------------------------------------------------------- //
			//
			// -------------------------------------------------------------------------- //
			return sts = ns_consts::EnmReturnStatus::Success;
		}
	}
}