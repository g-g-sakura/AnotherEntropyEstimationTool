# AnotherEntropyEstimationTool
Entropy estimation based on NIST SP 800-90B non-IID track is supported.

The proposed list of corrections to NIST SP 800-90B is drafted so that an entropy estimating tool for claiming conformance can be developed in a traceable manner, as shown in [/documentation/ProposedListOfCorrections_SP800-90B.pdf](https://github.com/g-g-sakura/AnotherEntropyEstimationTool/blob/main/documentation/ProposedListOfCorrections_SP800-90B.pdf) .

## Why does this tool exist
- As pointed out earlier, incorrect formulation will result in incorrect result. Therefore correct formulation is really important in developing this kind of tool.
- Also cross checking with other tools is important. 
During the development of this tool, checking with other existing tool was performed, and one identified inconsistency due to wrap-around bug was reported to a developer of the other tool:
https://github.com/usnistgov/SP800-90B_EntropyAssessment/issues/170

# Features
- Clarity and Traceability
  - Developed based on the documented and corrected [formulation](https://github.com/g-g-sakura/AnotherEntropyEstimationTool/blob/main/documentation/ProposedListOfCorrections_SP800-90B.pdf), to support ISO/IEC 17025 conformance.
  - Basically each function is commented in a traceable manner with [NIST SP 800-90B](https://doi.org/10.6028/NIST.SP.800-90B) or corrected [formulation](https://github.com/g-g-sakura/AnotherEntropyEstimationTool/blob/main/documentation/ProposedListOfCorrections_SP800-90B.pdf).
- Automatic report generation with graphs
  - Supports report generation capability, in XeLaTeX source file (see ```-x``` option), where relevant intermediate results are also contained to gain confidence and support characterization of entropy sources.

# Usage
```
.\ea_non_iid.exe -f <file name> [-w <number of bits per sample>] [-x] [--MSb|--LSb]
```

- ```-f``` specifies the file name of the acquisition data from an entropy source.  The file path must be relative path to a binary file with at least 1 million samples as per [NIST SP 800-90B](https://doi.org/10.6028/NIST.SP.800-90B).
- ```-w``` specifies the number of bits per sample.  This shall be between 1-8, inclusive.  By default, this value is 8.
- ```-x``` requests to generate a XeLaTeX source file of entropy estimation report, in the same folder as the specified acquisition data (see ```-f```).
- ```--MSb``` specifies the conversion from sample to bitstring in Most Significant bit first (**default**). 
- ```--LSb``` specifies the conversion from sample to bitstring in Least Significant bit first.

# Examples of entropy estimation results


<table>
  <thead>
    <tr>
      <td>Input data file uploaded in <a href="https://github.com/usnistgov/SP800-90B_EntropyAssessment/tree/master/bin">usnistgov / SP800-90B_EntropyAssessment</a></td>
      <td>Analysis environment 1<br/>x64 INTEL</td>
      <td>Analysis environment 2<br/>x64 AMD</td>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td>truerand_8bit.bin</td>
      <td><a href="https://github.com/g-g-sakura/AnotherEntropyEstimationTool/blob/main/tool_validation_evidence/entropy_report_truerand_8bit_x64_INTEL.pdf">entropy_report_truerand_8bit_x64_INTEL.pdf</a></td>
      <td><a href="https://github.com/g-g-sakura/AnotherEntropyEstimationTool/blob/main/tool_validation_evidence/entropy_report_truerand_8bit_x64_AMD.pdf">entropy_report_truerand_8bit_x64_AMD.pdf</a></td>
    </tr>
    <tr>
      <td>truerand_4bit.bin</td>
      <td><a href="https://github.com/g-g-sakura/AnotherEntropyEstimationTool/blob/main/tool_validation_evidence/entropy_report_truerand_4bit_x64_INTEL.pdf">entropy_report_truerand_4bit_x64_INTEL.pdf</a></td>
      <td><a href="https://github.com/g-g-sakura/AnotherEntropyEstimationTool/blob/main/tool_validation_evidence/entropy_report_truerand_4bit_x64_AMD.pdf">entropy_report_truerand_4bit_x64_AMD.pdf</a></td>
    </tr>
    <tr>
      <td>truerand_1bit.bin</td>
      <td><a href="https://github.com/g-g-sakura/AnotherEntropyEstimationTool/blob/main/tool_validation_evidence/entropy_report_truerand_1bit_x64_INTEL.pdf">entropy_report_truerand_1bit_x64_INTEL.pdf</a></td>
      <td><a href="https://github.com/g-g-sakura/AnotherEntropyEstimationTool/blob/main/tool_validation_evidence/entropy_report_truerand_1bit_x64_AMD.pdf">entropy_report_truerand_1bit_x64_AMD.pdf</a></td>
    </tr>
    <tr>
      <td>ringOsc-nist.bin</td>
      <td><a href="https://github.com/g-g-sakura/AnotherEntropyEstimationTool/blob/main/tool_validation_evidence/entropy_report_ringOsc-nist_x64_INTEL.pdf">entropy_report_ringOsc-nist_x64_INTEL.pdf</a></td>
      <td><a href="https://github.com/g-g-sakura/AnotherEntropyEstimationTool/blob/main/tool_validation_evidence/entropy_report_ringOsc-nist_x64_AMD.pdf">entropy_report_ringOsc-nist_x64_AMD.pdf</a></td>
    </tr>
    <tr>
      <td>rand8_short.bin</td>
      <td><a href="https://github.com/g-g-sakura/AnotherEntropyEstimationTool/blob/main/tool_validation_evidence/entropy_report_rand8_short_x64_INTEL.pdf">entropy_report_rand8_short_x64_INTEL.pdf</a></td>
      <td><a href="https://github.com/g-g-sakura/AnotherEntropyEstimationTool/blob/main/tool_validation_evidence/entropy_report_rand8_short_x64_AMD.pdf">entropy_report_rand8_short_x64_AMD.pdf</a></td>
    </tr>
    <tr>
      <td>rand4_short.bin</td>
      <td><a href="https://github.com/g-g-sakura/AnotherEntropyEstimationTool/blob/main/tool_validation_evidence/entropy_report_rand4_short_x64_INTEL.pdf">entropy_report_rand4_short_x64_INTEL.pdf</a></td>
      <td><a href="https://github.com/g-g-sakura/AnotherEntropyEstimationTool/blob/main/tool_validation_evidence/entropy_report_rand4_short_x64_AMD.pdf">entropy_report_rand4_short_x64_AMD.pdf</a></td>
    </tr>
    <tr>
      <td>rand1_short.bin</td>
      <td><a href="https://github.com/g-g-sakura/AnotherEntropyEstimationTool/blob/main/tool_validation_evidence/entropy_report_rand1_short_x64_INTEL.pdf">entropy_report_rand1_short_x64_INTEL.pdf</a></td>
      <td><a href="https://github.com/g-g-sakura/AnotherEntropyEstimationTool/blob/main/tool_validation_evidence/entropy_report_rand1_short_x64_AMD.pdf">entropy_report_rand1_short_x64_AMD.pdf</a></td>
    </tr>
    <tr>
      <td>normal.bin</td>
      <td><a href="https://github.com/g-g-sakura/AnotherEntropyEstimationTool/blob/main/tool_validation_evidence/entropy_report_normal_x64_INTEL.pdf">entropy_report_normal_x64_INTEL.pdf</a></td>
      <td><a href="https://github.com/g-g-sakura/AnotherEntropyEstimationTool/blob/main/tool_validation_evidence/entropy_report_normal_x64_AMD.pdf">entropy_report_normal_x64_AMD.pdf</a></td>
    </tr>
    <tr>
      <td>data.pi.bin</td>
      <td><a href="https://github.com/g-g-sakura/AnotherEntropyEstimationTool/blob/main/tool_validation_evidence/entropy_report_data.pi_x64_INTEL.pdf">entropy_report_data.pi_x64_INTEL.pdf</a></td>
      <td><a href="https://github.com/g-g-sakura/AnotherEntropyEstimationTool/blob/main/tool_validation_evidence/entropy_report_data.pi_x64_AMD.pdf">entropy_report_data.pi_x64_AMD.pdf</a></td>
    </tr>
    <tr>
      <td>biased-random-bytes.bin</td>
      <td><a href="https://github.com/g-g-sakura/AnotherEntropyEstimationTool/blob/main/tool_validation_evidence/entropy_report_biased-random-bytes_x64_INTEL.pdf">entropy_report_biased-random-bytes_x64_INTEL.pdf</a></td>
      <td><a href="https://github.com/g-g-sakura/AnotherEntropyEstimationTool/blob/main/tool_validation_evidence/entropy_report_biased-random-bytes_x64_AMD.pdf">entropy_report_biased-random-bytes_x64_AMD.pdf</a></td>
    </tr>
    <tr>
      <td>biased-random-bits.bin</td>
      <td><a href="https://github.com/g-g-sakura/AnotherEntropyEstimationTool/blob/main/tool_validation_evidence/entropy_report_biased-random-bits_x64_INTEL.pdf">entropy_report_biased-random-bits_x64_INTEL.pdf</a></td>
      <td><a href="https://github.com/g-g-sakura/AnotherEntropyEstimationTool/blob/main/tool_validation_evidence/entropy_report_biased-random-bits_x64_AMD.pdf">entropy_report_biased-random-bits_x64_AMD.pdf</a></td>
    </tr>
  </tbody>
</table>


# System equirements for runtime environment
- OS : Windows 10 or newer with 64-bit
- RAM : 16GB or more main memory
- TeX Live (optional for compiling automatically generated XeLaTeX source file)

# System requirements for development environment
## Compiler
Visual Studio 2019 or newer, or Intel oneAPI compiler

## Prerequisite libraries
- [Boost C++ libraries](https://www.boost.org/)
  - Boost.Dynamic bitset for storing variable length bitstrings.
  - Boost.Filesystem for manipulation of file path.
  - Boost.Math for root finding and for evaluating special functions.
  - Boost.Multi-Index for t-tuples.
  - Boost.Posix Time for logging performance information.
  - Boost.Program Options for parsing command line options.
- [Blitz++ library](https://github.com/blitzpp/blitz)
  - for array indexing, referencing subarray, and for perform array reductions (e.g. mean, sum) briefly
