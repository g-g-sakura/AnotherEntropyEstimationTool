////////////////////////////////////////////////////////////////////////////////
// parse.cpp
//
//
//
// Copyright (c) 2021-2022 G. G. SAKURAI <g.garland823@gmail.com>
//
////////////////////////////////////////////////////////////////////////////////
#include <string>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include "EntEstimateLib/support/enumerateAlphabet.h"
#include "EntEstimateLib/support/setUp.h"
#include "parse.h"

namespace bs_po = boost::program_options;
namespace bs_fs = boost::filesystem;

namespace ns_consts = entropy_estimator_lib::constants;
namespace ns_dt = entropy_estimator_lib::data_types;

ns_consts::EnmReturnStatus parse(ns_dt::t_data_for_estimator& io_refData,
    ns_dt::t_data_for_estimator& io_refDataBitString,
    IDInfoForReport& i_refInfoReport,
    int ac, wchar_t* av[], wchar_t* envp[] )
{
    // -------------------------------------------------------------------------- //
    //
    // -------------------------------------------------------------------------- //
    ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorUnexpected;
    // -------------------------------------------------------------------------- //
    //
    // -------------------------------------------------------------------------- //
    const boost::uintmax_t file_size_limit = 32 * 1024 * 1024;

    try {
        int     bits_per_sample = 8;
        int     vl = 1;
        bs_po::options_description desc("Allowed options");
        desc.add_options()
            ("help,h", "Usage is : ea_non_iid [-i|-c][-a|-t][-v][-l <index>, <samples>] -f <file_name>[-w] [-x]")
            ("file,f", bs_po::wvalue<std::wstring>(), "Must be relative path to a binary file with at least 1 million entries (samples).")
            ("bits_per_sample,w", bs_po::value<int>(&bits_per_sample)->default_value(8), "Must be between 1-8, inclusive. By default this value is 8.")
            ("initial,i", "    Initial Entropy Estimate(Section 3.1.3)"
                "Computes the initial entropy estimate H_I as described in Section 3.1.3"
                "(not accounting for H_submitter) using the entropy estimators specified in"
                "Section 6.3.  If 'bits_per_symbol' is greater than 1, the samples are also"
                "converted to bitstrings and assessed to create H_bitstring; for multi-bit symbols,"
                "two entropy estimates are computed: H_original and H_bitstring.\n"
                "Returns min(H_original, bits_per_symbol X H_bitstring). The initial entropy"
                "estimate H_I = min(H_submitter, H_original, bits_per_symbol X H_bitstring).")
            ("all,a", "    tests all bits in bitstring")
            ("truncate,t", "    truncates bitstring to 1000000 bits.")
            ("partial,l", "    <index>,<samples>\tRead the <index> substring of length <samples>."
                "Samples are assumed to be packed into 8-bit values, where the least significant 'bits_per_symbol' bits constitute the symbol.")
            ("MSb", "    Byte to bitstring conversion is performed by assuming Most Significant bit (MSb) first (default).")
            ("LSb", "    Byte to bitstring conversion is performed by assuming Least Significant bit (LSb) first.")
            ("LaTeX,x", "    Generate a report in LaTeX format.")
            ("verbose_level,v", bs_po::value<int>(&vl)->default_value(1), "Must be between 0-3, inclusive.")
            ;

        bs_po::variables_map po_vm;
        bs_po::store(bs_po::wcommand_line_parser(ac, av).options(desc).run(), po_vm);
        bs_po::notify(po_vm);

        // -------------------------------------------------------------------------- //
        // 
        // 
        // -------------------------------------------------------------------------- //
        if (po_vm.count("help")) {
            std::cout << desc << std::endl;
            return  sts = ns_consts::EnmReturnStatus::ErrorNoTask;
        }
        // -------------------------------------------------------------------------- //
        // 
        // 
        // -------------------------------------------------------------------------- //
        if (po_vm.count("bits_per_sample")) {
            if ((bits_per_sample < 1) || (8 < bits_per_sample)) {
                std::cout << "# [ERROR]: Invalid bits per symbol." << std::endl;
                return  sts;
            }
        }
        // -------------------------------------------------------------------------- //
        // 
        // 
        // -------------------------------------------------------------------------- //
        if (po_vm.count("file")) {
            const bs_fs::path file_path = po_vm["file"].as<std::wstring>(); // <<<
            std::wcout << L"# [INFO]: Opening file:\t";
            std::wcout << file_path << std::endl;

            boost::system::error_code error;
            const bool result = bs_fs::exists(file_path, error);
            if (!result || error) {
                std::cout << "# [ERROR]: Specified file was not found." << std::endl;
                return  sts;
            }
            else {
                std::cout << "# [INFO]: Specified file was found." << std::endl;
            }
            // -------------------------------------------------------------------------- //
            // full path
            // -------------------------------------------------------------------------- //
            bs_fs::path full_path = bs_fs::system_complete(file_path);
            // -------------------------------------------------------------------------- //
            // get path for future use
            // -------------------------------------------------------------------------- //
            if (nullptr != i_refInfoReport.info_source.p_path_to_entropy_input){
                *i_refInfoReport.info_source.p_path_to_entropy_input = full_path;
            }
            i_refInfoReport.info_source.tm_last_write_time = bs_fs::last_write_time(full_path);
            // -------------------------------------------------------------------------- //
            //
            // -------------------------------------------------------------------------- //
            const boost::uintmax_t size = bs_fs::file_size(file_path);

            std::cout << "Its file size: " << size << "-byte" << std::endl;
            if (file_size_limit < size)
            {
                std::cout << "# [ERROR]: Huge file is specified, so the estimation is stopped." << std::endl;
                return  sts;
            }

            memset(&io_refData, 0, sizeof(io_refData));
            ns_consts::EnmReturnStatus	sts = entropy_estimator_lib::support::setUp(io_refData);
            if (ns_consts::EnmReturnStatus::Success != sts)
            {
                return  sts;
            }
            sts = entropy_estimator_lib::support::loadSamples(io_refData, full_path);
            if (ns_consts::EnmReturnStatus::Success != sts)
            {
                return  sts;
            }
            sts = entropy_estimator_lib::support::enumerateAlphabet(io_refData);
            if (ns_consts::EnmReturnStatus::Success != sts)
            {
                return  sts;
            }
            // -------------------------------------------------------------------------- //
            //
            // -------------------------------------------------------------------------- //
            io_refData.bits_per_sample = bits_per_sample;
            // -------------------------------------------------------------------------- //
            //
            // -------------------------------------------------------------------------- //
            if (io_refData.L < 1000000)
            {
                std::cout << "# [WARNING]: data contains less than 1,000,000 samples." << std::endl;
            }
            // -------------------------------------------------------------------------- //
            // 
            // check pre-conditions for L, required from 6.3.4 of NIST SP 800-90B
            // -------------------------------------------------------------------------- //
            if ((io_refData.L * bits_per_sample) < 6012)
            {
                std::cout << "# [ERROR]: The number of samples does not meet one of pre-conditions specified in NIST SP 800-90B, so the estimation is stopped." << std::endl;
                return  sts = ns_consts::EnmReturnStatus::ErrorPreconditions;
            }
            // -------------------------------------------------------------------------- //
            // 
            // check pre-conditions for L, required from 6.3.7 of NIST SP 800-90B
            // -------------------------------------------------------------------------- //
            if ((io_refData.L) < 4097)
            {
                std::cout << io_refData.L << std::endl;
                std::cout << "# [ERROR]: The number of samples does not meet one of pre-conditions specified in NIST SP 800-90B, so the estimation is stopped." << std::endl;
                return  sts = ns_consts::EnmReturnStatus::ErrorPreconditions;
            }
            // -------------------------------------------------------------------------- //
            //
            // -------------------------------------------------------------------------- //
            if (1 < bits_per_sample)
            {
                // -------------------------------------------------------------------------- //
                //
                // -------------------------------------------------------------------------- //
                memset(&io_refDataBitString, 0, sizeof(io_refDataBitString));
                ns_consts::EnmReturnStatus	sts = entropy_estimator_lib::support::setUp(io_refDataBitString);
                if (ns_consts::EnmReturnStatus::Success != sts)
                {
                    return  sts;
                }
                // -------------------------------------------------------------------------- //
                //
                // -------------------------------------------------------------------------- //
                const uintmax_t def_number_of_bits_when_truncation = 1000000;
                uintmax_t   number_of_bits_to_be_loaded = size * bits_per_sample;
                // -------------------------------------------------------------------------- //
                // 
                // -------------------------------------------------------------------------- //
                if (po_vm.count("all")) {
                    if (po_vm.count("truncate")) {
                        std::cout << "# [ERROR]: all and truncate cannot be used simultaneously." << std::endl;
                        return  sts;
                    }
                    io_refData.isTestingAllBitsRequested = true;
                    io_refDataBitString.isTestingAllBitsRequested = true;
                    std::cout << "# [INFO]: Requesting all bits in the bitstring to be tested" << std::endl;
                }
                else if (po_vm.count("truncate"))
                {
                    io_refData.isTestingAllBitsRequested = false;
                    io_refDataBitString.isTestingAllBitsRequested = true;
                    // -------------------------------------------------------------------------- //
                    // 
                    // -------------------------------------------------------------------------- //
                    if (def_number_of_bits_when_truncation < number_of_bits_to_be_loaded)
                    {
                        number_of_bits_to_be_loaded = def_number_of_bits_when_truncation;
                        if (0 != def_number_of_bits_when_truncation % bits_per_sample)
                        {
                            number_of_bits_to_be_loaded = bits_per_sample * ((def_number_of_bits_when_truncation + bits_per_sample - 1) / bits_per_sample);
                        }
                    }
                    std::cout << "# [INFO]: Requesting first " << number_of_bits_to_be_loaded << " bits to be tested" << std::endl;
                }
                // -------------------------------------------------------------------------- //
                // 
                // 
                // -------------------------------------------------------------------------- //
                io_refData.bIsMSbFirstByteBitConversion = true;
                io_refDataBitString.bIsMSbFirstByteBitConversion = true;
                if (po_vm.count("MSb")) {
                    if (po_vm.count("LSb")) {
                        std::cout << "# [ERROR]: MSb and LSb cannot be used simultaneously." << std::endl;
                        return  sts;
                    }
                    std::cout << "# [INFO]: Byte to bitstring conversion is performed by assuming Most Significant bit (MSb) first." << std::endl;
                }
                else
                {
                    if (po_vm.count("LSb")) {
                        io_refDataBitString.bIsMSbFirstByteBitConversion = false;
                        std::cout << "# [INFO]: Byte to bitstring conversion is performed by assuming Least Significant bit (LSb) first." << std::endl;
                    }
                }
                // -------------------------------------------------------------------------- //
                //
                // -------------------------------------------------------------------------- //
                sts = entropy_estimator_lib::support::loadSamplesByInterpretation(io_refDataBitString,
                    full_path, number_of_bits_to_be_loaded, bits_per_sample);
                if (ns_consts::EnmReturnStatus::Success != sts)
                {
                    return  sts;
                }
                // -------------------------------------------------------------------------- //
                // 
                // -------------------------------------------------------------------------- //
                std::cout << "# [INFO]: Loaded " << io_refData.L << " samples of " << io_refData.k << " distinct " << bits_per_sample << "-bit-wide symbols" << std::endl;
            }
        }
        else {
            std::cout << "# [INFO]: No file is specified to process.\n";
            std::cout << "# [INFO]: Terminates entropy estimation...\n";
            return  sts = ns_consts::EnmReturnStatus::ErrorNoTask;
        }

        // -------------------------------------------------------------------------- //
        // 
        // 
        // -------------------------------------------------------------------------- //
        if (po_vm.count("LaTeX")) {
            std::cout << "# [INFO]: Generating a report in LaTeX format is requested." << std::endl;
            std::cout << "# [INFO]: The report in LaTeX format will be output in the same folder as the input file." << std::endl;
            io_refData.isGeneratingReportInLaTeXformatRequested = true;
            if (1 < bits_per_sample)
            {
                io_refDataBitString.isGeneratingReportInLaTeXformatRequested = true;
            }
        }
        // -------------------------------------------------------------------------- //
        // 
        // 
        // -------------------------------------------------------------------------- //
        if (po_vm.count("verbose_level")) {
            if ((vl < 0) || (3 < vl)) {
                std::cout << "# [ERROR]: Invalid verbose level." << std::endl;
                return  sts;
            }
            io_refData.verbose_level = vl;
            if (1 < bits_per_sample)
            {
                io_refDataBitString.verbose_level = vl;
            }
        }
        else
        {

        }
    }
    catch (std::exception& e) {
        std::cerr << "# [ERROR]: error: " << e.what() << "\n";
        return sts;
    }
    catch (...) {
        std::cerr << "# [ERROR]: Exception of unknown type!\n";
        return	sts;
    }
    return  sts = ns_consts::EnmReturnStatus::Success;
}