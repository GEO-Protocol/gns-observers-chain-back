/**
 * This file is part of GEO Protocol.
 * It is subject to the license terms in the LICENSE.md file found in the top-level directory
 * of this distribution and at https://github.com/GEO-Protocol/GEO-network-client/blob/master/LICENSE.md
 *
 * No part of GEO Protocol, including this file, may be copied, modified, propagated, or distributed
 * except according to the terms contained in the LICENSE.md file.
 */

#ifndef GNS_OBSERVERS_CHAIN_BACK_MEMORYUTILS_H
#define GNS_OBSERVERS_CHAIN_BACK_MEMORYUTILS_H


#include "../Types.h"

#include <cstdint>
#include <memory>
#include <malloc.h>


using namespace std;


inline BytesShared tryMalloc(
    size_t bytesCount)
{
    BytesShared bytesShared(
        (byte *)malloc(bytesCount),
        free);

    if (bytesShared == nullptr) {
        // todo: throw MemoryError
        throw std::bad_alloc();
    }

    return bytesShared;
}

inline BytesShared tryCalloc(
    size_t bytesCount)
{

    BytesShared bytesShared(
        (byte *)calloc(
            bytesCount,
            sizeof(byte)),
        free);

    if (bytesShared == nullptr) {
        // todo: throw MemoryError
        throw std::bad_alloc();
    }

    return bytesShared;
}

#endif //GNS_OBSERVERS_CHAIN_BACK_MEMORYUTILS_H
