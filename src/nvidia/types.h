/* -*- Mode: C; tab-width: 8; c-basic-offset: 2; indent-tabs-mode: nil; -*- */

/*******************************************************************************
    Copyright (c) 2013 NVidia Corporation

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to
    deal in the Software without restriction, including without limitation the
    rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
    sell copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

        The above copyright notice and this permission notice shall be
        included in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.
*******************************************************************************/

#ifndef RR_NVIDIA_TYPES_H
#define RR_NVIDIA_TYPES_H 1

#include <sys/ioctl.h>
#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__GNUC__) || defined(__clang__) || defined(NV_QNX) || defined(NV_HOS)
#define NV_ALIGN_BYTES(size) __attribute__ ((aligned (size)))
#elif defined(__arm)
#define NV_ALIGN_BYTES(size) __align(ALIGN)
#endif

typedef uint8_t NvU8;
typedef uint16_t NvU16;
typedef uint64_t NvU64;
typedef int64_t NvS64;
typedef uint32_t NvU32;
typedef int32_t NvS32;

typedef NvU8 NvBool;
typedef NvU64 NvP64;
typedef NvU32 NV_STATUS;

#define NV_UUID_LEN 16
typedef struct nv_uuid
{
    NvU8 uuid[NV_UUID_LEN];

} NvUuid;
typedef NvUuid NvSystemUuid;
typedef NvUuid NvProcessorUuid;

typedef NvU32 NvHandle;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* RR_NVIDIA_UVM_H */
