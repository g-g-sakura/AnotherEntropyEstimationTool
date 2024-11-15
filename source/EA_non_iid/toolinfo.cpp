////////////////////////////////////////////////////////////////////////////////
// toolinfo.cpp
//
//
// Copyright (c) 2021-2024 G. G. SAKURAI <g.garland823@gmail.com>
//
////////////////////////////////////////////////////////////////////////////////
#include "toolinfo.h"

ns_consts::EnmReturnStatus getToolInfo(const InfoAnalysisTool& i_refAnalysisTool)
{
    // -------------------------------------------------------------------------- //
    //
    // -------------------------------------------------------------------------- //
    ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorNullPointer;
    if (nullptr == i_refAnalysisTool.p_analyzer_name)
    {
        return  sts;
    }
    if (nullptr == i_refAnalysisTool.p_analyzer_versioning)
    {
        return  sts;
    }
    // -------------------------------------------------------------------------- //
    //
    // -------------------------------------------------------------------------- //
    *i_refAnalysisTool.p_analyzer_name = std::wstring(L"Another entropy estimation tool with extensions");
    // -------------------------------------------------------------------------- //
    //
    // -------------------------------------------------------------------------- //
    *i_refAnalysisTool.p_analyzer_versioning = std::wstring(L"1.0.58");
    // -------------------------------------------------------------------------- //
    //
    // -------------------------------------------------------------------------- //
    return  sts = ns_consts::EnmReturnStatus::Success;
}