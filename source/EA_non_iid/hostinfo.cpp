////////////////////////////////////////////////////////////////////////////////
// hostinfo.cpp
//
//
//
// Copyright (c) 2021-2022 G. G. SAKURAI <g.garland823@gmail.com>
//
////////////////////////////////////////////////////////////////////////////////
#include "hostinfo.h"
#include <Windows.h>
#include <VersionHelpers.h>
#include <intrin.h>
#include <stdlib.h>
#include <sstream>

namespace ns_consts = entropy_estimator_lib::constants;

#define INFO_BUFFER_SIZE 4096

ns_consts::EnmReturnStatus getHostInfo(std::wstring& o_refHostInfo)
{
	TCHAR  infoBuf[INFO_BUFFER_SIZE];
	DWORD  bufCharCount = INFO_BUFFER_SIZE;

	ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorUnexpected;

	memset(infoBuf, 0, sizeof(infoBuf));
	// Obtain the name of the computer.
	if (!GetComputerName(infoBuf, &bufCharCount))
	{
		return	sts;
	}

	o_refHostInfo = std::wstring(infoBuf, std::wcslen(infoBuf));

	return sts = ns_consts::EnmReturnStatus::Success;
}

ns_consts::EnmReturnStatus getCPUInfo(std::wstring& o_refCPUInfo)
{
    TCHAR  infoBuf[INFO_BUFFER_SIZE];

    ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorUnexpected;

    memset(infoBuf, 0, sizeof(infoBuf));
    // Obtain the CPU information.
    int CPUInfo[4] = { -1 };
    unsigned   nExIds, i = 0;
    char CPUBrandString[0x40];
    // Get the information associated with each extended ID.
    __cpuid(CPUInfo, 0x80000000);
    nExIds = CPUInfo[0];
    for (i = 0x80000000; i <= nExIds; ++i)
    {
        __cpuid(CPUInfo, i);
        // Interpret CPU brand string
        if (i == 0x80000002)
            memcpy(CPUBrandString, CPUInfo, sizeof(CPUInfo));
        else if (i == 0x80000003)
            memcpy(CPUBrandString + 16, CPUInfo, sizeof(CPUInfo));
        else if (i == 0x80000004)
            memcpy(CPUBrandString + 32, CPUInfo, sizeof(CPUInfo));
    }

    size_t  sz = INFO_BUFFER_SIZE;
    errno_t er = mbstowcs_s(&sz, infoBuf, CPUBrandString, sizeof(CPUBrandString));
    if (0 == er)
    {
        o_refCPUInfo = std::wstring(infoBuf, std::wcslen(infoBuf));

        sts = ns_consts::EnmReturnStatus::Success;
    }

    return sts;
}

ns_consts::EnmReturnStatus getMemoryStatus(std::wstring& o_refMemoryStatus)
{
    ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorUnexpected;

    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof(statex);
    BOOL    bMem = GlobalMemoryStatusEx(&statex);

    if (0 != bMem)
    {
        std::wstringstream ss = std::wstringstream();
        ss << (statex.ullTotalPhys / 1024) / 1024 << " MB";
        o_refMemoryStatus = ss.str();
        sts = ns_consts::EnmReturnStatus::Success;
    }

    return sts;
}

ns_consts::EnmReturnStatus getOSInfo(std::wstring& o_refOSInfo)
{
    ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorUnexpected;

    //DWORD   dwVersion[4];
    //DWORD   ProductType;
    //memset(dwVersion, 0, sizeof(dwVersion));

    //BOOL    bRetProductType = ::GetProductInfo();

    OSVERSIONINFO osvi;
    BOOL bIsWindows10orLater = FALSE;
    BOOL bIsWindows81orLater = FALSE;

    ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

    GetVersionEx(&osvi);

    bIsWindows10orLater = (osvi.dwMajorVersion >= 10);
    bIsWindows81orLater = ((osvi.dwMajorVersion > 6) ||
        ((osvi.dwMajorVersion == 6) && (osvi.dwMinorVersion >= 3)));;

    if (::IsWindowsServer())
    {

    }
    else
    {
        if (bIsWindows10orLater)
        {
            o_refOSInfo = std::wstring(L"Windows 10 or greater");
        }
        else if (bIsWindows81orLater)
        {
            o_refOSInfo = std::wstring(L"Windows 8.1 or greater");
        }
        else
        {
            o_refOSInfo = std::wstring(L"EOL OS");
        }
        sts = ns_consts::EnmReturnStatus::Success;
    }

    return sts;
}

ns_consts::EnmReturnStatus getUserInfo(std::wstring& o_refUserInfo)
{
	TCHAR  infoBuf[INFO_BUFFER_SIZE];
	DWORD  bufCharCount = INFO_BUFFER_SIZE;

	ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorUnexpected;

	memset(infoBuf, 0, sizeof(infoBuf));
	// Obtain the user name.
	if (!GetUserName(infoBuf, &bufCharCount))
	{
		return	sts;
	}

    o_refUserInfo = std::wstring(infoBuf, std::wcslen(infoBuf));

	return sts = ns_consts::EnmReturnStatus::Success;
}

ns_consts::EnmReturnStatus getEnvInfo(InfoEnv& i_refEnv)
{
    // -------------------------------------------------------------------------- //
    //
    // -------------------------------------------------------------------------- //
    ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorNullPointer;
    if (nullptr == i_refEnv.p_hostname)
    {
        return  sts;
    }
    if (nullptr == i_refEnv.p_username)
    {
        return  sts;
    }
    // -------------------------------------------------------------------------- //
    // hostname
    // -------------------------------------------------------------------------- //
    sts = getHostInfo(*i_refEnv.p_hostname);
    if (ns_consts::EnmReturnStatus::Success != sts)
    {
        return  sts;
    }
    // -------------------------------------------------------------------------- //
    // cpu information
    // -------------------------------------------------------------------------- //
    sts = getCPUInfo(*i_refEnv.p_cpuinfo);
    if (ns_consts::EnmReturnStatus::Success != sts)
    {
        return  sts;
    }
    // -------------------------------------------------------------------------- //
    // memory status
    // -------------------------------------------------------------------------- //
    sts = getMemoryStatus(*i_refEnv.p_physicalmemory);
    if (ns_consts::EnmReturnStatus::Success != sts)
    {
        return  sts;
    }
    // -------------------------------------------------------------------------- //
    // os information
    // -------------------------------------------------------------------------- //
    sts = getOSInfo(*i_refEnv.p_osinfo);
    if (ns_consts::EnmReturnStatus::Success != sts)
    {
        return  sts;
    }
    // -------------------------------------------------------------------------- //
    // username
    // -------------------------------------------------------------------------- //
    sts = getUserInfo(*i_refEnv.p_username);
    if (ns_consts::EnmReturnStatus::Success != sts)
    {
        return  sts;
    }
    // -------------------------------------------------------------------------- //
    //
    // -------------------------------------------------------------------------- //
    return  sts = ns_consts::EnmReturnStatus::Success;
}
