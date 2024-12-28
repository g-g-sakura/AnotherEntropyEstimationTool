////////////////////////////////////////////////////////////////////////////////
// LaTeX.cpp
//
//
//
// Copyright (c) 2024 G. G. SAKURAI <g.garland823@gmail.com>
//
////////////////////////////////////////////////////////////////////////////////
#include "../pch.h"
#include "LaTeX.h"
// -------------------------------------------------------------------------- //
/// <summary>
/// </summary>
/// <remarks>
/// </remarks>
/// <param name="i_refCommentSummary">
/// </param>
/// <returns>
/// </returns>
/// <precondition>
/// </precondition>
/// <postcondition>
/// </postcondition>
// -------------------------------------------------------------------------- //
namespace entropy_estimator_lib
{
	namespace support
	{
		namespace ns_consts = entropy_estimator_lib::constants;

		// -------------------------------------------------------------------------- //
		/// <summary>
		/// </summary>
		/// <remarks>
		/// </remarks>
		/// <param name="o_refLaTeXFragment">
		/// </param>
		/// <param name="i_refComment">
		/// </param>
		/// <returns>
		/// </returns>
		/// <precondition>
		/// </precondition>
		/// <postcondition>
		/// </postcondition>
		// -------------------------------------------------------------------------- //
		ns_consts::EnmReturnStatus getLaTeXCommentBlock(std::wstring& o_refLaTeXFragment, const std::wstring& i_refComment)
		{
			ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorUnexpected;

			// -------------------------------------------------------------------------- //
			// 
			// -------------------------------------------------------------------------- //
			std::wstringstream	ss = std::wstringstream();
			// -------------------------------------------------------------------------- //
			// 
			// -------------------------------------------------------------------------- //
			ss << L"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << L"\n";
			ss << L"%%%%%%" << L"\n";
			ss << L"%%%%%%";
			if (false == i_refComment.empty())
			{
				ss << L" " << i_refComment;
			}
			ss << L"\n";
			ss << L"%%%%%%" << L"\n";
			ss << L"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << L"\n";
			// -------------------------------------------------------------------------- //
			// 
			// -------------------------------------------------------------------------- //
			o_refLaTeXFragment = ss.str();
			// -------------------------------------------------------------------------- //
			// 
			// -------------------------------------------------------------------------- //
			return  sts = ns_consts::EnmReturnStatus::Success;
		}

		enum class EnmLaTeXSectionLevel
		{
			EnmLaTeXSection = 0,
			EnmLaTeXSubsection = 1,
			EnmLaTeXSubsubsection = 2,
			EnmLaTeXParagraph = 3,
			EnmLaTeXSubparagraph = 4,
		};

		// -------------------------------------------------------------------------- //
		/// <summary>
		/// </summary>
		/// <remarks>
		/// </remarks>
		/// <param name="o_refSection">
		/// </param>
		/// <param name="i_refSectionTitle">
		/// </param>
		/// <param name="i_refLabel">
		/// </param>
		/// <param name="i_section_level">
		/// </param>
		/// <returns>
		/// </returns>
		/// <precondition>
		/// </precondition>
		/// <postcondition>
		/// </postcondition>
		// -------------------------------------------------------------------------- //
		ns_consts::EnmReturnStatus getLaTeXSectionWithLeveling(std::wstring& o_refSection,
			const std::wstring& i_refSectionTitle,
			const std::wstring& i_refLabel,
			EnmLaTeXSectionLevel i_section_level)
		{
			ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorUnexpected;

			// -------------------------------------------------------------------------- //
			// 
			// -------------------------------------------------------------------------- //
			std::wstringstream	ssSection = std::wstringstream();
			// -------------------------------------------------------------------------- //
			// 
			// -------------------------------------------------------------------------- //
			std::wstring	strComment = std::wstring();
			sts = getLaTeXCommentBlock(strComment, i_refSectionTitle);
			if (ns_consts::EnmReturnStatus::Success != sts)
			{
				return  sts;
			}
			ssSection << strComment;
			// -------------------------------------------------------------------------- //
			// 
			// -------------------------------------------------------------------------- //
			switch (i_section_level)
			{
			case EnmLaTeXSectionLevel::EnmLaTeXSection:
				ssSection << L"\\section";
				break;
			case EnmLaTeXSectionLevel::EnmLaTeXSubsection:
				ssSection << L"\\subsection";
				break;
			case EnmLaTeXSectionLevel::EnmLaTeXSubsubsection:
				ssSection << L"\\subsubsection";
				break;
			case EnmLaTeXSectionLevel::EnmLaTeXParagraph:
				ssSection << L"\\paragraph";
				break;
			case EnmLaTeXSectionLevel::EnmLaTeXSubparagraph:
				ssSection << L"\\subparagraph";
				break;
			default:
				break;
			}
			ssSection << L"{";
			if (false == i_refSectionTitle.empty())
			{
				ssSection << i_refSectionTitle;
			}
			ssSection << L"}";
			// -------------------------------------------------------------------------- //
			// 
			// -------------------------------------------------------------------------- //
			if (false == i_refLabel.empty())
			{
				ssSection << L"\\label{" << i_refLabel << L"}" << L"\n";
			}
			// -------------------------------------------------------------------------- //
			// 
			// -------------------------------------------------------------------------- //
			o_refSection = ssSection.str();
			// -------------------------------------------------------------------------- //
			// 
			// -------------------------------------------------------------------------- //
			return  sts = ns_consts::EnmReturnStatus::Success;
		}

		// -------------------------------------------------------------------------- //
		/// <summary>
		/// </summary>
		/// <remarks>
		/// </remarks>
		/// <param name="o_refSection">
		/// </param>
		/// <param name="i_refSectionTitle">
		/// </param>
		/// <param name="i_refLabel">
		/// </param>
		/// <returns>
		/// </returns>
		/// <precondition>
		/// </precondition>
		/// <postcondition>
		/// </postcondition>
		// -------------------------------------------------------------------------- //
		ns_consts::EnmReturnStatus getLaTeXSection(std::wstring& o_refSection,
			const std::wstring& i_refSectionTitle,
			const std::wstring& i_refLabel)
		{
			// -------------------------------------------------------------------------- //
			// 
			// -------------------------------------------------------------------------- //
			return  getLaTeXSectionWithLeveling(o_refSection, 
				i_refSectionTitle, 
				i_refLabel, 
				EnmLaTeXSectionLevel::EnmLaTeXSection);
		}

		// -------------------------------------------------------------------------- //
		/// <summary>
		/// </summary>
		/// <remarks>
		/// </remarks>
		/// <param name="o_refSubsection">
		/// </param>
		/// <param name="i_refSubsectionTitle">
		/// </param>
		/// <param name="i_refLabel">
		/// </param>
		/// <returns>
		/// </returns>
		/// <precondition>
		/// </precondition>
		/// <postcondition>
		/// </postcondition>
		// -------------------------------------------------------------------------- //
		ns_consts::EnmReturnStatus getLaTeXSubsection(std::wstring& o_refSubsection, 
			const std::wstring& i_refSubsectionTitle,
			const std::wstring& i_refLabel)
		{
			// -------------------------------------------------------------------------- //
			// 
			// -------------------------------------------------------------------------- //
			return  getLaTeXSectionWithLeveling(o_refSubsection, 
				i_refSubsectionTitle, 
				i_refLabel, 
				EnmLaTeXSectionLevel::EnmLaTeXSubsection);
		}

		// -------------------------------------------------------------------------- //
		/// <summary>
		/// </summary>
		/// <remarks>
		/// </remarks>
		/// <param name="o_refSubsubsection">
		/// </param>
		/// <param name="i_refSubsubsectionTitle">
		/// </param>
		/// <param name="i_refLabel">
		/// </param>
		/// <returns>
		/// </returns>
		/// <precondition>
		/// </precondition>
		/// <postcondition>
		/// </postcondition>
		// -------------------------------------------------------------------------- //
		ns_consts::EnmReturnStatus getLaTeXSubsubsection(std::wstring& o_refSubsubsection, 
			const std::wstring& i_refSubsubsectionTitle,
			const std::wstring& i_refLabel)
		{
			// -------------------------------------------------------------------------- //
			// 
			// -------------------------------------------------------------------------- //
			return  getLaTeXSectionWithLeveling(o_refSubsubsection,
				i_refSubsubsectionTitle,
				i_refLabel, 
				EnmLaTeXSectionLevel::EnmLaTeXSubsubsection);
		}

		// -------------------------------------------------------------------------- //
		/// <summary>
		/// </summary>
		/// <remarks>
		/// </remarks>
		/// <param name="o_refSubsubsection">
		/// </param>
		/// <param name="i_refLabel">
		/// </param>
		/// <returns>
		/// </returns>
		/// <precondition>
		/// </precondition>
		/// <postcondition>
		/// </postcondition>
		// -------------------------------------------------------------------------- //
		ns_consts::EnmReturnStatus getLaTeXSubsubsectionTraceability(std::wstring& o_refSubsubsection,
			const std::wstring& i_refLabel)
		{
			std::wstring	strSubsubsectionTitle = std::wstring(L"Supplemental information for traceability");
			return getLaTeXSubsubsection(o_refSubsubsection, strSubsubsectionTitle, i_refLabel);
		}

	}
}
