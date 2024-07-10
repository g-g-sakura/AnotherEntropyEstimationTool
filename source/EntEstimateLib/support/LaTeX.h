////////////////////////////////////////////////////////////////////////////////
// LaTeX.h
//
//
// Copyright (c) 2024 G. G. SAKURAI <g.garland823@gmail.com>
//
////////////////////////////////////////////////////////////////////////////////
#if defined(_MSC_VER)
#pragma once
#endif

#ifndef ENTROPY_ESTIMATOR_LIB_LATEX_H
#define ENTROPY_ESTIMATOR_LIB_LATEX_H

#include "../constants.h"
#include <sstream>

namespace entropy_estimator_lib
{
	namespace support
	{
		namespace ns_consts = entropy_estimator_lib::constants;

		ns_consts::EnmReturnStatus getLaTeXSubsection(std::wstring& o_refSubsection, const std::wstring& i_refSubsectionTitle);
		ns_consts::EnmReturnStatus getLaTeXSubsubsection(std::wstring& o_refSubsubsection, const std::wstring& i_refSubsubsectionTitle);
		ns_consts::EnmReturnStatus getLaTeXSubsubsectionTraceability(std::wstring& o_refSubsubsection);
	}
}
#endif