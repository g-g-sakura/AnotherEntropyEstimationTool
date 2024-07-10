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
/// <params="i_refCommentSummary">
/// </params>
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
		/// <params="o_refSubsection">
		/// </params>
		/// <params="i_refSubsectionTitle">
		/// </params>
		/// <returns>
		/// </returns>
		/// <precondition>
		/// </precondition>
		/// <postcondition>
		/// </postcondition>
		// -------------------------------------------------------------------------- //
		ns_consts::EnmReturnStatus getLaTeXSubsection(std::wstring& o_refSubsection, const std::wstring& i_refSubsectionTitle)
		{
			ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorUnexpected;

			// -------------------------------------------------------------------------- //
			// 
			// -------------------------------------------------------------------------- //
			std::wstringstream	ssSubsection = std::wstringstream();
			// -------------------------------------------------------------------------- //
			// 
			// -------------------------------------------------------------------------- //
			ssSubsection << L"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << L"\n";
			ssSubsection << L"%%%%%%" << L"\n";
			ssSubsection << L"%%%%%%";
			if (false == i_refSubsectionTitle.empty())
			{
				ssSubsection << L" " << i_refSubsectionTitle;
			}
			ssSubsection << L"\n";
			ssSubsection << L"%%%%%%" << L"\n";
			ssSubsection << L"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << L"\n";
			// -------------------------------------------------------------------------- //
			// 
			// -------------------------------------------------------------------------- //
			ssSubsection << L"\\subsection{";
			if (false == i_refSubsectionTitle.empty())
			{
				ssSubsection << i_refSubsectionTitle;
			}
			ssSubsection << L"}";

			o_refSubsection = ssSubsection.str();
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
		/// <params="o_refSubsubsection">
		/// </params>
		/// <params="i_refSubsubsectionTitle">
		/// </params>
		/// <returns>
		/// </returns>
		/// <precondition>
		/// </precondition>
		/// <postcondition>
		/// </postcondition>
		// -------------------------------------------------------------------------- //
		ns_consts::EnmReturnStatus getLaTeXSubsubsection(std::wstring& o_refSubsubsection, const std::wstring& i_refSubsubsectionTitle)
		{
			ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorUnexpected;

			// -------------------------------------------------------------------------- //
			// 
			// -------------------------------------------------------------------------- //
			std::wstringstream	ssSubsubsection = std::wstringstream();
			// -------------------------------------------------------------------------- //
			// 
			// -------------------------------------------------------------------------- //
			ssSubsubsection << L"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << L"\n";
			ssSubsubsection << L"%%%%%%" << L"\n";
			ssSubsubsection << L"%%%%%%";
			if (false == i_refSubsubsectionTitle.empty())
			{
				ssSubsubsection << L" " << i_refSubsubsectionTitle;
			}
			ssSubsubsection << L"\n";
			ssSubsubsection << L"%%%%%%" << L"\n";
			ssSubsubsection << L"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << L"\n";
			// -------------------------------------------------------------------------- //
			// 
			// -------------------------------------------------------------------------- //
			ssSubsubsection << L"\\subsection{";
			if (false == i_refSubsubsectionTitle.empty())
			{
				ssSubsubsection << i_refSubsubsectionTitle;
			}
			ssSubsubsection << L"}\n";

			o_refSubsubsection = ssSubsubsection.str();
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
		/// <params="o_refSubsection">
		/// </params>
		/// <returns>
		/// </returns>
		/// <precondition>
		/// </precondition>
		/// <postcondition>
		/// </postcondition>
		// -------------------------------------------------------------------------- //
		ns_consts::EnmReturnStatus getLaTeXSubsubsectionTraceability(std::wstring& o_refSubsubsection)
		{
			std::wstring	strSubsubsectionTitle = std::wstring(L"Supplemental information for traceability");
			return getLaTeXSubsubsection(o_refSubsubsection, strSubsubsectionTitle);
		}

	}
}
