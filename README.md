# AnotherEntropyEstimationTool
Entropy estimation based on NIST SP 800-90B non-IID track is supported.

The proposed list of corrections to NIST SP 800-90B is drafted so that an entropy estimating tool for claiming conformance can be developed in a traceable manner, as shown in [/documentation/ProposedListOfCorrections_SP800-90B.pdf](https://github.com/g-g-sakura/AnotherEntropyEstimationTool/blob/main/documentation/ProposedListOfCorrections_SP800-90B.pdf) .

# Features
- Clarity and Traceability
  - Developed based on the documented and corrected [formulation](https://github.com/g-g-sakura/AnotherEntropyEstimationTool/blob/main/documentation/ProposedListOfCorrections_SP800-90B.pdf), to support ISO/IEC 17025 conformance.
  - Basically each function is commented in a traceable manner with [NIST SP 800-90B](https://doi.org/10.6028/NIST.SP.800-90B) or corrected [formulation](https://github.com/g-g-sakura/AnotherEntropyEstimationTool/blob/main/documentation/ProposedListOfCorrections_SP800-90B.pdf).
- Automatic report generation with graphs
  - Supports report generation capability, in XeLaTeX source file (see ```-x``` option), where relevant intermediate results are also contained to gain confidence and support characterization.

# Usage
```
.\ea_non_iid.exe -f <file name> [-w] [-x] [--MSb|--LSb]
```

- ```-f``` specifies the file name of the acquisition data from an entropy source.  The file path must be relative path to a binary file with at least 1 million samples as per [NIST SP 800-90B](https://doi.org/10.6028/NIST.SP.800-90B).
- ```-w``` specifies the number of bits per sample.  This shall be between 1-8, inclusive.  By default, this value is 8.
- ```-x``` requests to generate a XeLaTeX source file of entropy estimation report, in the same folder as the specified acquisition data (see ```-f```).
- ```--MSb``` specifies the conversion from sample to bitstring in Most Significant bit first (**default**). 
- ```--LSb``` specifies the conversion from sample to bitstring in Least Significant bit first.

# System equirements for runtime environment
- OS : Windows 10 or newer with 64-bit
- RAM : 16GB or more main memory

# System requirements for development environment
## Compiler
Visual Studio 2019 or newer, or Intel oneAPI compiler

## Prerequisite libraries
- [Boost C++ libraries](https://www.boost.org/)
- [Blitz++ libbrary](https://github.com/blitzpp/blitz)

# Why does this tool exist
- As pointed out earlier, incorrect formulation will result in incorrect result. Therefore correct formulation is really important in developing this kind of tool.
- Also cross checking with other tools is important. 
During the development of this tool, checking with other existing tool was performed, and one identified inconsistency was reported to a developer of the other tool:
https://github.com/usnistgov/SP800-90B_EntropyAssessment/issues/170
