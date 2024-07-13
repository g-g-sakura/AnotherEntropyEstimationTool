////////////////////////////////////////////////////////////////////////////////
// hostinfo.cpp
//
//
//
// Copyright (c) 2021-2024 G. G. SAKURAI <g.garland823@gmail.com>
//
////////////////////////////////////////////////////////////////////////////////
#include "hostinfo.h"
#include <Windows.h>
#include <VersionHelpers.h>
#include <intrin.h>
#include <sstream>
#include <regex>
#include <boost/algorithm/string.hpp>

namespace ns_consts = entropy_estimator_lib::constants;

enum { INFO_BUFFER_SIZE = 4096 };

enum { STDOUT_BUFFER_SIZE = 6144 };

// -------------------------------------------------------------------------- //
/// <summary>
/// </summary>
/// <remarks>
/// </remarks>
/// <param name="o_refHostInfo">
/// </param>
/// <returns>
/// </returns>
/// <precondition>
/// </precondition>
/// <postcondition>
/// </postcondition>
// -------------------------------------------------------------------------- //
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

// -------------------------------------------------------------------------- //
/// <summary>
/// </summary>
/// <remarks>
/// </remarks>
/// <param name="o_refCPUInfo">
/// </param>
/// <returns>
/// </returns>
/// <precondition>
/// </precondition>
/// <postcondition>
/// </postcondition>
// -------------------------------------------------------------------------- //
ns_consts::EnmReturnStatus getCPUInfo(std::wstring& o_refCPUInfo)
{
    TCHAR  infoBuf[INFO_BUFFER_SIZE];

    ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorUnexpected;

    memset(infoBuf, 0, sizeof(infoBuf));
    // Obtain the CPU information.
    int CPUInfo[4] = { -1 };
    char CPUBrandString[0x60];
    memset(CPUBrandString, 0, sizeof(CPUBrandString));
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

// -------------------------------------------------------------------------- //
/// <summary>
/// </summary>
/// <remarks>
/// </remarks>
/// <param name="o_refMemoryStatus">
/// </param>
/// <returns>
/// </returns>
/// <precondition>
/// </precondition>
/// <postcondition>
/// </postcondition>
// -------------------------------------------------------------------------- //
ns_consts::EnmReturnStatus getMemoryStatus(std::wstring& o_refMemoryStatus)
{
    ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorUnexpected;

    MEMORYSTATUSEX statex;
    ZeroMemory(&statex, sizeof(statex));
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

// -------------------------------------------------------------------------- //
/// <summary>
/// </summary>
/// <remarks>
/// </remarks>
/// <param name="">
/// </param>
/// <returns>
/// </returns>
/// <precondition>
/// </precondition>
/// <postcondition>
/// </postcondition>
// -------------------------------------------------------------------------- //
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

// -------------------------------------------------------------------------- //
/// <summary>
/// </summary>
/// <remarks>
/// </remarks>
/// <param name="o_refSystemType">
/// </param>
/// <returns>
/// </returns>
/// <precondition>
/// </precondition>
/// <postcondition>
/// </postcondition>
// -------------------------------------------------------------------------- //
ns_consts::EnmReturnStatus getSystemType(std::wstring& o_refSystemType)
{
    ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorUnexpected;

    std::wstringstream  wssSystemType = std::wstringstream();

    if (true == Is64bitOS())
    {
        wssSystemType << std::wstring(L"64-bit");
    }
    else
    {
        wssSystemType << std::wstring(L"32-bit");
    }

    o_refSystemType = wssSystemType.str();
    sts = ns_consts::EnmReturnStatus::Success;

    return sts;
}



typedef struct THandleContainer_
{
    HANDLE  hChildStd_OUT_Rd;
    HANDLE  hChildStd_OUT_Wr;
} THandleContainer;

// -------------------------------------------------------------------------- //
/// <summary>
/// </summary>
/// <remarks>
/// </remarks>
/// <param name="io_refContainer">
/// </param>
/// <returns>
/// </returns>
/// <precondition>
/// </precondition>
/// <postcondition>
/// </postcondition>
// -------------------------------------------------------------------------- //
ns_consts::EnmReturnStatus invokeSystemInfo(THandleContainer& io_refContainer)
{
    ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorUnexpected;

    TCHAR szCmdline[] = TEXT("cmd /C chcp 437 && systeminfo");
    PROCESS_INFORMATION pi;
    STARTUPINFO si;
    BOOL bSuccess = FALSE;

    // -------------------------------------------------------------------------- //
    // Clear members of the PROCESS_INFORMATION structure. 
    // -------------------------------------------------------------------------- //
    ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));

    // -------------------------------------------------------------------------- //
    // Set up members of the STARTUPINFO structure. 
    // This structure specifies the STDIN and STDOUT handles for redirection.
    // -------------------------------------------------------------------------- //
    ZeroMemory(&si, sizeof(STARTUPINFO));
    si.cb = sizeof(STARTUPINFO);
    si.hStdError = io_refContainer.hChildStd_OUT_Wr;
    si.hStdOutput = io_refContainer.hChildStd_OUT_Wr;
    si.dwFlags |= STARTF_USESTDHANDLES;
    // -------------------------------------------------------------------------- //
    // Set code page so that output of system info can be searched by English characters
    // -------------------------------------------------------------------------- //
    SetConsoleOutputCP(437);
    // -------------------------------------------------------------------------- //
    // Create the child process. 
    // -------------------------------------------------------------------------- //
    bSuccess = CreateProcess(NULL,
        szCmdline,     // command line 
        NULL,          // process security attributes 
        NULL,          // primary thread security attributes 
        TRUE,          // handles are inherited 
        0,             // creation flags 
        NULL,          // use parent's environment 
        NULL,          // use parent's current directory 
        &si,  // STARTUPINFO pointer 
        &pi);  // receives PROCESS_INFORMATION 

    if (!bSuccess)
        // -------------------------------------------------------------------------- //
        // If an error occurs 
        // -------------------------------------------------------------------------- //
        return sts = ns_consts::EnmReturnStatus::ErrorChildProcess;
    else
    {
        // -------------------------------------------------------------------------- //
        // Wait until the child process exits.
        // -------------------------------------------------------------------------- //
        ::WaitForSingleObject(pi.hProcess, INFINITE);

		// -------------------------------------------------------------------------- //
    	// Close handle to the child process and its primary thread.
		// -------------------------------------------------------------------------- //
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        // -------------------------------------------------------------------------- //
        // Close the handle to the stdout pipe.
        // -------------------------------------------------------------------------- //
        CloseHandle(io_refContainer.hChildStd_OUT_Wr);
    }

    return  sts = ns_consts::EnmReturnStatus::Success;
}

// -------------------------------------------------------------------------- //
/// <summary>
/// </summary>
/// <remarks>
/// </remarks>
/// <param name="o_refOSName">
/// </param>
/// <param name="o_refOSVersion">
/// </param>
/// <param name="io_container">
/// </param>
/// <returns>
/// </returns>
/// <precondition>
/// </precondition>
/// <postcondition>
/// </postcondition>
// -------------------------------------------------------------------------- //
ns_consts::EnmReturnStatus readOSNameAndVersion(std::string& o_OS_name, std::string& o_OS_version, THandleContainer& io_container)
{
    ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorUnexpected;

    DWORD dwRead;
    CHAR chBuf[STDOUT_BUFFER_SIZE];
    BOOL bSuccess = FALSE;
    // -------------------------------------------------------------------------- //
    // regular expressions for OS name and OS version
    // -------------------------------------------------------------------------- //
    std::string     experOSName = std::string("^OS Name:\\s+([\\w\\s\\./\\(\\)\\[\\]]+)$");
    std::string     experOSVersion = std::string("^OS Version:\\s+([\\w\\s\\./\\(\\)\\[\\]]+)$");

    ZeroMemory(chBuf, sizeof(chBuf));
    // -------------------------------------------------------------------------- //
    // read stdout and get OS name and OS version
    // -------------------------------------------------------------------------- //
    bSuccess = ReadFile(io_container.hChildStd_OUT_Rd, chBuf, STDOUT_BUFFER_SIZE, &dwRead, NULL);
    if (!bSuccess || dwRead == 0)
    {
        return sts = ns_consts::EnmReturnStatus::ErrorFileIO;
    }

    std::string strRead = std::string(chBuf, STDOUT_BUFFER_SIZE);
    // -------------------------------------------------------------------------- //
    // get OS name
    // -------------------------------------------------------------------------- //
    std::smatch m;
    if (true == std::regex_search(strRead, m, std::regex(experOSName)))
    {
        o_OS_name = m[1].str();
        boost::algorithm::trim_right(o_OS_name);
    }
    // -------------------------------------------------------------------------- //
    // get OS version
    // -------------------------------------------------------------------------- //
    if (true == std::regex_search(strRead, m, std::regex(experOSVersion)))
    {
        o_OS_version = m[1].str();
        boost::algorithm::trim_right(o_OS_version);
    }

    return  sts = ns_consts::EnmReturnStatus::Success;
}

// -------------------------------------------------------------------------- //
/// <summary>
/// </summary>
/// <remarks>
/// </remarks>
/// <param name="o_refOSName">
/// </param>
/// <param name="o_refOSVersion">
/// </param>
/// <returns>
/// </returns>
/// <precondition>
/// </precondition>
/// <postcondition>
/// </postcondition>
// -------------------------------------------------------------------------- //
ns_consts::EnmReturnStatus getOSNameAndVersion(std::wstring& o_refOSName, std::wstring& o_refOSVersion)
{
    ns_consts::EnmReturnStatus	sts = ns_consts::EnmReturnStatus::ErrorUnexpected;

    SECURITY_ATTRIBUTES sa;
    THandleContainer    container;
    ZeroMemory(&sa, sizeof(sa));
    ZeroMemory(&container, sizeof(container));

    // Set the bInheritHandle flag so pipe handles are inherited. 
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.bInheritHandle = TRUE;
    sa.lpSecurityDescriptor = NULL;

    // -------------------------------------------------------------------------- //
    // Create a pipe for stdout
    // -------------------------------------------------------------------------- //
    if (!CreatePipe(&container.hChildStd_OUT_Rd, &container.hChildStd_OUT_Wr, &sa, 0))
    {
        return sts;
    }
    // -------------------------------------------------------------------------- //
    // Ensure the read handle to the pipe for STDOUT is not inherited.
    // -------------------------------------------------------------------------- //
    if (!SetHandleInformation(container.hChildStd_OUT_Rd, HANDLE_FLAG_INHERIT, 0))
    {
        return sts;
    }
    // -------------------------------------------------------------------------- //
    // invoke cmd.exe and invoke systeminfo
    // -------------------------------------------------------------------------- //
    sts = invokeSystemInfo(container);
    if (ns_consts::EnmReturnStatus::Success != sts)
    {
        return  sts;
    }
    // -------------------------------------------------------------------------- //
    // read stdout and get OS name and OS version
    // -------------------------------------------------------------------------- //
    std::string     strOSName = std::string();
    std::string     strOSVersion = std::string();
    sts = readOSNameAndVersion(strOSName, strOSVersion, container);
    if (ns_consts::EnmReturnStatus::Success != sts)
    {
        return  sts;
    }
    // -------------------------------------------------------------------------- //
    // convert to wchar_t
    // -------------------------------------------------------------------------- //
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, strOSName.data(), static_cast<int>(strOSName.size()), nullptr, 0);
    o_refOSName.resize(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, strOSName.data(), static_cast<int>(strOSName.size()), o_refOSName.data(), size_needed);

    size_needed = MultiByteToWideChar(CP_UTF8, 0, strOSVersion.data(), static_cast<int>(strOSVersion.size()), nullptr, 0);
    o_refOSVersion.resize(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, strOSVersion.data(), static_cast<int>(strOSVersion.size()), o_refOSVersion.data(), size_needed);
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
/// <param name="o_refUserInfo">
/// </param>
/// <returns>
/// </returns>
/// <precondition>
/// </precondition>
/// <postcondition>
/// </postcondition>
// -------------------------------------------------------------------------- //
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

// -------------------------------------------------------------------------- //
/// <summary>
/// </summary>
/// <remarks>
/// </remarks>
/// <param name="i_refEnv">
/// </param>
/// <returns>
/// </returns>
/// <precondition>
/// </precondition>
/// <postcondition>
/// </postcondition>
// -------------------------------------------------------------------------- //
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
    // os name and version
    // -------------------------------------------------------------------------- //
    sts = getOSNameAndVersion(*i_refEnv.p_osname, *i_refEnv.p_osversion);
    if (ns_consts::EnmReturnStatus::Success != sts)
    {
        return  sts;
    }
    // -------------------------------------------------------------------------- //
    // system type (either 64-bit or 32-bit)
    // -------------------------------------------------------------------------- //
    sts = getSystemType(*i_refEnv.p_system_type);
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
