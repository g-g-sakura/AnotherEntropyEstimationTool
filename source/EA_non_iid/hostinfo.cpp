////////////////////////////////////////////////////////////////////////////////
// hostinfo.cpp
//
//
//
// Copyright (c) 2021-2023 G. G. SAKURAI <g.garland823@gmail.com>
//
////////////////////////////////////////////////////////////////////////////////
#include "hostinfo.h"
#include <Windows.h>
#include <VersionHelpers.h>
#include <intrin.h>
#include <sstream>

namespace ns_consts = entropy_estimator_lib::constants;

enum { INFO_BUFFER_SIZE = 4096 };

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
    char CPUBrandString[0x40];
    // Get the information associated with each extended ID.
    __cpuid(CPUInfo, 0x80000000);
    unsigned    nExIds = CPUInfo[0];
    for (unsigned i = 0x80000000; i <= nExIds; ++i)
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
    const errno_t er = mbstowcs_s(&sz, infoBuf, CPUBrandString, sizeof(CPUBrandString));
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
    const BOOL    bMem = GlobalMemoryStatusEx(&statex);

    if (0 != bMem)
    {
        std::wstringstream ss = std::wstringstream();
        ss << (statex.ullTotalPhys / 1024) / 1024 << " MiB";
        o_refMemoryStatus = ss.str();
        sts = ns_consts::EnmReturnStatus::Success;
    }

    return sts;
}

bool    Is64bitOS()
{
    {
#if defined(_WIN64)
        return true;  // 64-bit
#elif defined(_WIN32)
        // Two possibilities: 32-bit process on 32-bit OS, or 32-bit process on 64-bit OS
        BOOL bIsWoW = FALSE;
        if (0 == IsWow64Process(GetCurrentProcess(), &bIsWoW))
        {
            if (FALSE == bIsWoW)
            {
                return  false;  // 32-bit process on 32-bit OS
            }
            else
            {
                return  true;   // 32-bit process on 64-bit OS
            }
        }
#else
        return false; //
#endif
    }
}

ns_consts::EnmReturnStatus getOSInfo(std::wstring& o_refOSInfo)
{
    ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorUnexpected;

    std::wstringstream  wssOSInfo = std::wstringstream();

    if (::IsWindowsServer())
    {

    }
    else
    {
        if (IsWindows10OrGreater())
        {
            wssOSInfo << std::wstring(L"Windows 10 or greater");
        }
        else
        {
            wssOSInfo << std::wstring(L"EOL OS");
        }

        if (true == Is64bitOS())
        {
            wssOSInfo << std::wstring(L" 64-bit");
        }
        else
        {
            wssOSInfo << std::wstring(L" 32-bit");
        }

        o_refOSInfo = wssOSInfo.str();
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
	// Obtain the username.
	if (!GetUserName(infoBuf, &bufCharCount))
	{
		return	sts;
	}

    o_refUserInfo = std::wstring(infoBuf, std::wcslen(infoBuf));

	return sts = ns_consts::EnmReturnStatus::Success;
}

ns_consts::EnmReturnStatus getEnvInfo(const InfoEnv& i_refEnv)
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
