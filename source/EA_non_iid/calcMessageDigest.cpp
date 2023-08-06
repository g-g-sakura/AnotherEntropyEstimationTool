////////////////////////////////////////////////////////////////////////////////
// calcMessageDigest.cpp
//
//
//
// Copyright (c) 2021-2023 G. G. SAKURAI <g.garland823@gmail.com>
//
////////////////////////////////////////////////////////////////////////////////
#include <Windows.h>
#include <ntstatus.h>
#include <bcrypt.h>
#include <iomanip>
#include <string>
#include <sstream>
#include "calcMessageDigest.h"

namespace entropy_estimator_app
{
    // -------------------------------------------------------------------------- //
    /// <summary>
    /// 
    /// </summary>
    /// <remarks>
    ///  Calls Windows BCRYPT APIs
    /// </remarks>
    /// <param name="o_refMessageDigestInHex"></param>
    /// <param name="i_pInputMessage"></param>
    /// <param name="i_cuiInputMessageLengthInOctets"></param>
    /// <param name="i_enmHashAlgorithm"></param>
    /// <returns></returns>
    // -------------------------------------------------------------------------- //
    ns_consts::EnmReturnStatus calcMessageDigest(std::string& o_refMessageDigestInHex,
        unsigned char* i_pInputMessage,
        unsigned int i_cuiInputMessageLengthInOctets,
        constants::EnmHashAlgorithm i_enmHashAlgorithm)
    {
        ns_consts::EnmReturnStatus  rtnSts = ns_consts::EnmReturnStatus::ErrorUnexpected;

        BCRYPT_ALG_HANDLE     hash_alg_handle;
        BCRYPT_HASH_HANDLE    hash_handle;
        /// <summary>
        /// pointer to store message digest
        /// </summary>
        PBYTE   pbHash = nullptr;
        /// <summary>
        /// pointer to a working memory for hashing
        /// </summary>
        PBYTE   pbHashObject = nullptr;
        // -------------------------------------------------------------------------- //
        // 
        // -------------------------------------------------------------------------- //
        DWORD   cbData = 0;
        /// <summary>
        /// length of memory region pointed out by <c>pbHash</c> in bytes 
        /// </summary>
        DWORD   cbHash = 0; 
        DWORD   cbHashObject = 0;

        NTSTATUS    stsGetPropOutputLength;
        NTSTATUS    stsCreateHash;
        NTSTATUS    stsHashData;
        NTSTATUS    stsFinishHash;
        std::stringstream   ss = std::stringstream();
        // -------------------------------------------------------------------------- //
        // 
        // -------------------------------------------------------------------------- //
        wchar_t     wstrSHA2_256[] = BCRYPT_SHA256_ALGORITHM;
        wchar_t     wstrSHA2_384[] = BCRYPT_SHA384_ALGORITHM;
        wchar_t     wstrSHA2_512[] = BCRYPT_SHA512_ALGORITHM;
        wchar_t* pwstrHashID = wstrSHA2_256;
        switch (i_enmHashAlgorithm)
        {
        case constants::EnmHashAlgorithm::ESHA_256:
            pwstrHashID = wstrSHA2_256;
            break;
        case constants::EnmHashAlgorithm::ESHA_384:
            pwstrHashID = wstrSHA2_384;
            break;
        case constants::EnmHashAlgorithm::ESHA_512:
            pwstrHashID = wstrSHA2_512;
            break;
        default:
            return  rtnSts = ns_consts::EnmReturnStatus::ErrorInvalidData;
        }
        // -------------------------------------------------------------------------- //
        // 
        // -------------------------------------------------------------------------- //
        NTSTATUS    sts = BCryptOpenAlgorithmProvider(&hash_alg_handle,
            pwstrHashID,
            MS_PRIMITIVE_PROVIDER,
            BCRYPT_HASH_REUSABLE_FLAG);

        if (STATUS_SUCCESS != sts)
        {
            return rtnSts;
        }
        // -------------------------------------------------------------------------- //
        // 
        // -------------------------------------------------------------------------- //
        NTSTATUS    stsGetPropObjLength = BCryptGetProperty(hash_alg_handle,
            BCRYPT_OBJECT_LENGTH,
            (PBYTE)&cbHashObject,
            sizeof(DWORD),
            &cbData,
            0);
        if (STATUS_SUCCESS != stsGetPropObjLength)
        {
            rtnSts = ns_consts::EnmReturnStatus::ErrorInvalidData;
            goto BLOCK_CLEANUP_1;
        }
        // -------------------------------------------------------------------------- //
        // allocate a heap memory area for a working memory for hashing
        // -------------------------------------------------------------------------- //
        pbHashObject = (PBYTE)HeapAlloc(GetProcessHeap(), 0, cbHashObject);
        if (nullptr == pbHashObject)
        {
            rtnSts = ns_consts::EnmReturnStatus::ErrorMemoryAllocation;
            goto BLOCK_CLEANUP_1;
        }
        // -------------------------------------------------------------------------- //
        // 
        // -------------------------------------------------------------------------- //
        stsGetPropOutputLength = BCryptGetProperty(hash_alg_handle,
            BCRYPT_HASH_LENGTH,
            (PBYTE)&cbHash,
            sizeof(DWORD),
            &cbData,
            0);
        if (STATUS_SUCCESS != stsGetPropOutputLength)
        {
            rtnSts = ns_consts::EnmReturnStatus::ErrorInvalidData;
            goto BLOCK_CLEANUP_2;
        }
        // -------------------------------------------------------------------------- //
        // allocate a heap memory area for message digest
        // -------------------------------------------------------------------------- //
        pbHash = (PBYTE)HeapAlloc(GetProcessHeap(), 0, cbHash);
        if (nullptr == pbHash)
        {
            rtnSts = ns_consts::EnmReturnStatus::ErrorMemoryAllocation;
            goto BLOCK_CLEANUP_2;
        }
        // -------------------------------------------------------------------------- //
        // 
        // -------------------------------------------------------------------------- //
        stsCreateHash = BCryptCreateHash(hash_alg_handle, &hash_handle, pbHashObject, cbHashObject, NULL, 0, BCRYPT_HASH_REUSABLE_FLAG);
        if (STATUS_SUCCESS != stsCreateHash)
        {
            rtnSts = ns_consts::EnmReturnStatus::ErrorInvalidData;
            goto BLOCK_CLEANUP_3;
        }
        // -------------------------------------------------------------------------- //
        // hashing input message
        // -------------------------------------------------------------------------- //
        stsHashData = BCryptHashData(hash_handle, (PUCHAR)i_pInputMessage, i_cuiInputMessageLengthInOctets, 0);
        switch (stsHashData)
        {
        case STATUS_SUCCESS:
            break;
        default:
            rtnSts = ns_consts::EnmReturnStatus::ErrorInvalidData;
            goto BLOCK_CLEANUP_4;
        }
        // -------------------------------------------------------------------------- //
        // get message digest
        // -------------------------------------------------------------------------- //
        stsFinishHash = BCryptFinishHash(hash_handle, pbHash, cbHash, 0);
        if (STATUS_SUCCESS == stsFinishHash)
        {
            for (int i = 0; i < cbHash; ++i)
            {
                if (0 != i)
                {
                    if (0 == i % 4)
                    {
                        ss << " ";
                    }
                }
                ss << std::setfill('0') << std::right << std::setw(2) << std::hex << (int)(*(pbHash + i));
            }
            o_refMessageDigestInHex = ss.str();

            rtnSts = ns_consts::EnmReturnStatus::Success;
        }
        else
        {
            rtnSts = ns_consts::EnmReturnStatus::ErrorInvalidData;
        }

    BLOCK_CLEANUP_4:
        BCryptDestroyHash(hash_handle);
    BLOCK_CLEANUP_3:
        HeapFree(GetProcessHeap(), 0, pbHash);
    BLOCK_CLEANUP_2:
        HeapFree(GetProcessHeap(), 0, pbHashObject);
    BLOCK_CLEANUP_1:
        BCryptCloseAlgorithmProvider(hash_alg_handle, 0);

        return rtnSts;
    }
}
