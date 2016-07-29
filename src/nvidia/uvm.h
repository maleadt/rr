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

#ifndef RR_NVIDIA_UVM_H
#define RR_NVIDIA_UVM_H 1

#include <sys/ioctl.h>
#include <sys/types.h>

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define UVM_IOCTL_MAGIC '\0'
#define UVM_IO(nr) _IO(UVM_IOCTL_MAGIC, nr)
#define UVM_IOR(nr, type) _IOR(UVM_IOCTL_MAGIC, nr, type)
#define UVM_IOW(nr, type) _IOW(UVM_IOCTL_MAGIC, nr, type)
#define UVM_IOWR(nr, type) _IOWR(UVM_IOCTL_MAGIC, nr, type)

// handling ioctl(0x30000001): type:0 nr:0x1 dir:0 size:12288
// #define UVM_IOCTL_UNK1 _IO(0x00, 0x01)

// handling ioctl(0x27): type:0 nr:0x27 dir:0 size:0
// #define UVM_IOCTL_UNK2 _IO(0x00, 0x27)

// handling ioctl(0x7ff): type:0x7 nr:0xff dir:0 size:0
// #define UVM_IOCTL_UNK3 _IO(0x07, 0xff)

// ---

// handling ioctl(0x25): type:0 nr:0x25 dir:0 size:0
// handling ioctl(0x37): type:0 nr:0x37 dir:0 size:0 
// handling ioctl(0x17): type:0 nr:0x17 dir:0 size:0

typedef unsigned long long UvmStream;

typedef struct uvm_set_stream_running
{
    NvU32           scope; //UVM_DEBUG_V1 (UvmCounterScope)
    NvU32           name;  //UVM_DEBUG_V1 (UvmCounterName)
    NvProcessorUuid gpuid; //UVM_DEBUG_V1
    NvU32           state; //UVM_DEBUG_V1
} UvmCounterConfig;

typedef struct uvm_reserve_va
{
    NvProcessorUuid gpuUuid;
    NvU32           gpuMappingType; // UvmGpuMappingType
    NvU32           gpuCachingType; // UvmGpuCachingType
} UvmGpuMappingAttributes;


//
// UVM IOCTLs (from uvm_ioctl.h)
//

#define UVM_IOCTL_BASE 0

#define UVM_IOCTL_INITIALIZE 0x30000001
typedef struct uvm_initialize
{
    NV_STATUS rmStatus; // OUT
} uvm_initialize_t;

#define UVM_IOCTL_DEINITIALIZE 0x30000002

#define UVM_IOCTL_RESERVE_VA UVM_IO(UVM_IOCTL_BASE + 1)
typedef struct
{
    NvU64     requestedBase NV_ALIGN_BYTES(8); // IN
    NvU64     length        NV_ALIGN_BYTES(8); // IN
    NV_STATUS rmStatus;                        // OUT
} uvm_reserve_va_t;

#define UVM_IOCTL_IOCTL_RELEASE_VA UVM_IO(UVM_IOCTL_BASE + 2)
typedef struct uvm_release_va
{
    NvU64     requestedBase NV_ALIGN_BYTES(8); // IN
    NvU64     length        NV_ALIGN_BYTES(8); // IN
    NV_STATUS rmStatus;                        // OUT
} uvm_release_va_t;

#define UVM_IOCTL_REGION_COMMIT UVM_IO(UVM_IOCTL_BASE + 3)
typedef struct uvm_region_commit
{
    NvU64           requestedBase NV_ALIGN_BYTES(8); // IN
    NvU64           length        NV_ALIGN_BYTES(8); // IN
    UvmStream       streamId      NV_ALIGN_BYTES(8); // IN
    NvProcessorUuid gpuUuid;                         // IN
    NV_STATUS       rmStatus;                        // OUT
} uvm_region_commit_t;

#define UVM_IOCTL_REGION_DECOMMIT UVM_IO(UVM_IOCTL_BASE + 4)
typedef struct uvm_region_decommit
{
    NvU64     requestedBase NV_ALIGN_BYTES(8); // IN
    NvU64     length        NV_ALIGN_BYTES(8); // IN
    NV_STATUS rmStatus;                        // OUT
} uvm_region_decommit_t;

#define UVM_IOCTL_REGION_SET_STREAM UVM_IO(UVM_IOCTL_BASE + 5)
typedef struct uvm_region_set_stream
{
    NvU64           requestedBase NV_ALIGN_BYTES(8); // IN
    NvU64           length        NV_ALIGN_BYTES(8); // IN
    UvmStream       newStreamId   NV_ALIGN_BYTES(8); // IN
    NvProcessorUuid gpuUuid;                         // IN
    NV_STATUS       rmStatus;                        // OUT
} uvm_region_set_stream_t;

#define UVM_IOCTL_SET_STREAM_RUNNING UVM_IO(UVM_IOCTL_BASE + 6)
typedef struct
{
    UvmStream  streamId NV_ALIGN_BYTES(8);  // IN
    NV_STATUS  rmStatus;                    // OUT
} uvm_set_stream_running_t;

#define UVM_IOCTL_SET_STREAM_STOPPED UVM_IO(UVM_IOCTL_BASE + 7)
#define UVM_MAX_STREAMS_PER_IOCTL_CALL 32
typedef struct uvm_set_stream_stopped
{
    UvmStream streamIdArray[UVM_MAX_STREAMS_PER_IOCTL_CALL] NV_ALIGN_BYTES(8); // IN
    NvU64     nStreams                                      NV_ALIGN_BYTES(8); // IN
    NV_STATUS rmStatus;                                                        // OUT
} uvm_set_stream_stopped_t;

#define UVM_IOCTL_MIGRATE_TO_GPU UVM_IO(UVM_IOCTL_BASE + 8)
typedef struct uvm_migrate_to_gpu
{
    NvU64           requestedBase NV_ALIGN_BYTES(8); // IN
    NvU64           length        NV_ALIGN_BYTES(8); // IN
    NvProcessorUuid gpuUuid;                         // IN
    NvU32           flags;                           // IN
    NV_STATUS       rmStatus;                        // OUT
} uvm_migrate_to_gpu_t;

#define UVM_IOCTL_RUN_TEST UVM_IO(UVM_IOCTL_BASE + 9)
typedef struct uvm_run_test
{
    NvProcessorUuid gpuUuid;     // IN
    NvU32           test;        // IN
    struct
    {
        NvProcessorUuid peerGpuUuid; // IN
        NvU32           peerId;      // IN
    } multiGpu;
    NV_STATUS      rmStatus;    // OUT
} uvm_run_test_t;

#define UVM_IOCTL_ADD_SESSION UVM_IO(UVM_IOCTL_BASE + 10)
typedef struct uvm_add_session
{
    NvU32        pidTarget;                             // IN
#ifdef __linux__
    NvP64        countersBaseAddress NV_ALIGN_BYTES(8); // IN
    NvS32        sessionIndex;                          // OUT (session index that got added)
#endif
    NV_STATUS    rmStatus;                              // OUT
} uvm_add_session_t;

#define UVM_IOCTL_REMOVE_SESSION UVM_IO(UVM_IOCTL_BASE + 11)
typedef struct uvm_remove_session
{
#ifdef __linux__
    NvS32        sessionIndex; // IN (session index to be removed)
#endif
    NV_STATUS    rmStatus;     // OUT
} uvm_remove_session_t;

#define UVM_IOCTL_ENABLE_COUNTERS UVM_IO(UVM_IOCTL_BASE + 12)
#define UVM_MAX_COUNTERS_PER_IOCTL_CALL 32
typedef struct uvm_enable_counters
{
#ifdef __linux__
    NvS32            sessionIndex;                            // IN
#endif
    UvmCounterConfig config[UVM_MAX_COUNTERS_PER_IOCTL_CALL]; // IN
    NvU32            count;                                   // IN
    NV_STATUS        rmStatus;                                // OUT
} uvm_enable_counters_t;

#define UVM_IOCTL_MAP_COUNTER UVM_IO(UVM_IOCTL_BASE + 13)
typedef struct uvm_map_counter
{
#ifdef __linux__
    NvS32           sessionIndex;                   // IN
#endif
    NvU32           scope;                          // IN (UvmCounterScope)
    NvU32           counterName;                    // IN (UvmCounterName)
    NvProcessorUuid gpuUuid;                        // IN
    NvP64           addr         NV_ALIGN_BYTES(8); // OUT
    NV_STATUS       rmStatus;                       // OUT
} uvm_map_counter_t;

#define UVM_IOCTL_CREATE_EVENT_QUEUE UVM_IO(UVM_IOCTL_BASE + 14)
typedef struct uvm_create_event_queue
{
#ifdef __linux__
    NvS32                 sessionIndex;                         // IN
#endif
    NvU32                 eventQueueIndex;                      // OUT
    NvU64                 queueSize          NV_ALIGN_BYTES(8); // IN
    NvU64                 notificationCount  NV_ALIGN_BYTES(8); // IN
#if defined(WIN32) || defined(WIN64)
    NvU64                 notificationHandle NV_ALIGN_BYTES(8); // IN
#endif
    NvU32                 timeStampType;                        // IN (UvmEventTimeStampType)
    NV_STATUS             rmStatus;                             // OUT
} uvm_create_event_queue_t;

#define UVM_IOCTL_REMOVE_EVENT_QUEUE UVM_IO(UVM_IOCTL_BASE + 15)
typedef struct uvm_remove_event_queue
{
#ifdef __linux__
    NvS32         sessionIndex;       // IN
#endif
    NvU32         eventQueueIndex;    // IN
    NV_STATUS     rmStatus;           // OUT
} uvm_remove_event_queue_t;

#define UVM_IOCTL_MAP_EVENT_QUEUE UVM_IO(UVM_IOCTL_BASE + 16)
typedef struct uvm_map_event_queue
{
#ifdef __linux__
    NvS32         sessionIndex;                       // IN
#endif
    NvU32         eventQueueIndex;                    // IN
    NvP64         userRODataAddr   NV_ALIGN_BYTES(8); // IN
    NvP64         userRWDataAddr   NV_ALIGN_BYTES(8); // IN
    NvP64         readIndexAddr    NV_ALIGN_BYTES(8); // OUT
    NvP64         writeIndexAddr   NV_ALIGN_BYTES(8); // OUT
    NvP64         queueBufferAddr  NV_ALIGN_BYTES(8); // OUT
    NV_STATUS     rmStatus;                           // OUT
} uvm_map_event_queue_t;

#define UVM_IOCTL_EVENT_CTRL UVM_IO(UVM_IOCTL_BASE + 17)
typedef struct uvm_event_ctrl
{
#ifdef __linux__
    NvS32        sessionIndex;      // IN
#endif
    NvU32        eventQueueIndex;   // IN
    NvS32        eventType;         // IN
    NvU32        enable;            // IN
    NV_STATUS    rmStatus;          // OUT
} uvm_event_ctrl_t;

#define UVM_IOCTL_REGISTER_MPS_SERVER UVM_IO(UVM_IOCTL_BASE + 18)
#define UVM_MAX_GPUS 32
typedef struct uvm_register_mps_server
{
    NvProcessorUuid gpuUuidArray[UVM_MAX_GPUS];                    // IN
    NvU32           numGpus;                                       // IN
    NvU64           serverId                    NV_ALIGN_BYTES(8); // OUT
    NV_STATUS       rmStatus;                                      // OUT
} uvm_register_mps_server_t;

#define UVM_IOCTL_REGISTER_MPS_CLIENT UVM_IO(UVM_IOCTL_BASE + 19)
typedef struct uvm_register_mps_client
{
    NvU64     serverId  NV_ALIGN_BYTES(8); // IN
    NV_STATUS rmStatus;                    // OUT
} uvm_register_mps_client_t;

#define UVM_IOCTL_GET_GPU_UUID_TABLE UVM_IO(UVM_IOCTL_BASE + 20)
typedef struct uvm_get_gpu_uuid_table
{
    NvProcessorUuid gpuUuidArray[UVM_MAX_GPUS]; // OUT
    NvU32           validCount;                 // OUT
    NV_STATUS       rmStatus;                   // OUT
} uvm_get_gpu_uuid_table_t;

#if defined(WIN32) || defined(WIN64)
#define UVM_IOCTL_REGION_SET_BACKING UVM_IO(UVM_IOCTL_BASE + 21)
typedef struct uvm_region_set_backing
{
    NvProcessorUuid gpuUuid;                        // IN
    NvU32           hAllocation;                    // IN
    NvP64           vaAddr       NV_ALIGN_BYTES(8); // IN
    NvU64           regionLength NV_ALIGN_BYTES(8); // IN
    NV_STATUS       rmStatus;                       // OUT
} uvm_region_set_backing_t;

#define UVM_IOCTL_REGION_UNSET_BACKING UVM_IO(UVM_IOCTL_BASE + 22)
typedef struct uvm_region_unset_backing
{
    NvP64     vaAddr       NV_ALIGN_BYTES(8); // IN
    NvU64     regionLength NV_ALIGN_BYTES(8); // IN
    NV_STATUS rmStatus;                       // OUT
} uvm_region_unset_backing_t;

#endif

#define UVM_IOCTL_CREATE_RANGE_GROUP UVM_IO(UVM_IOCTL_BASE + 23)
typedef struct uvm_create_range_group
{
    NvU64     rangeGroupId NV_ALIGN_BYTES(8); // OUT
    NV_STATUS rmStatus;                       // OUT
} uvm_create_range_group_t;

#define UVM_IOCTL_DESTROY_RANGE_GROUP UVM_IO(UVM_IOCTL_BASE + 24)
typedef struct uvm_destroy_range_group
{
    NvU64     rangeGroupId NV_ALIGN_BYTES(8); // IN
    NV_STATUS rmStatus;                      // OUT
} uvm_destroy_range_group_t;

#define UVM_IOCTL_REGISTER_GPU_VASPACE UVM_IO(UVM_IOCTL_BASE + 25)
typedef struct uvm_register_gpu_vaspace
{
    NvProcessorUuid gpuUuid;  // IN
    NvS32           rmCtrlFd; // IN
    NvHandle        hClient;  // IN
    NvHandle        hVaSpace; // IN
    NV_STATUS       rmStatus; // OUT
} uvm_register_gpu_vaspace_t;

#define UVM_IOCTL_UNREGISTER_GPU_VASPACE UVM_IO(UVM_IOCTL_BASE + 26)
typedef struct uvm_unregister_gpu_vaspace
{
    NvProcessorUuid gpuUuid;  // IN
    NV_STATUS       rmStatus; // OUT
} uvm_unregister_gpu_vaspace_t;

#define UVM_IOCTL_REGISTER_CHANNEL UVM_IO(UVM_IOCTL_BASE + 27)
typedef struct uvm_register_channel
{
    NvProcessorUuid gpuUuid;                     // IN
    NvS32           rmCtrlFd;                    // IN
    NvHandle        hClient;                     // IN
    NvHandle        hChannel;                    // IN
    NvU64           base      NV_ALIGN_BYTES(8); // IN
    NvU64           length    NV_ALIGN_BYTES(8); // IN
    NV_STATUS       rmStatus;                    // OUT
} uvm_register_channel_t;

#define UVM_IOCTL_UNREGISTER_CHANNEL UVM_IO(UVM_IOCTL_BASE + 28)
typedef struct uvm_unregister_channel
{
    NvProcessorUuid gpuUuid;  // IN
    NvHandle        hClient;  // IN
    NvHandle        hChannel; // IN
    NV_STATUS       rmStatus; // OUT
} uvm_unregister_channel_t;

#define UVM_IOCTL_ENABLE_PEER_ACCESS UVM_IO(UVM_IOCTL_BASE + 29)
typedef struct uvm_enable_peer_access
{
    NvProcessorUuid gpuUuidA; // IN
    NvProcessorUuid gpuUuidB; // IN
    NV_STATUS  rmStatus; // OUT
} uvm_enable_peer_access_t;

#define UVM_IOCTL_DISABLE_PEER_ACCESS UVM_IO(UVM_IOCTL_BASE + 30)
typedef struct uvm_disable_peer_access
{
    NvProcessorUuid gpuUuidA; // IN
    NvProcessorUuid gpuUuidB; // IN
    NV_STATUS  rmStatus; // OUT
} uvm_disable_peer_access_t;

#define UVM_IOCTL_SET_RANGE_GROUP UVM_IO(UVM_IOCTL_BASE + 31)
typedef struct uvm_set_range_group
{
    NvU64     rangeGroupId  NV_ALIGN_BYTES(8); // IN
    NvU64     requestedBase NV_ALIGN_BYTES(8); // IN
    NvU64     length        NV_ALIGN_BYTES(8); // IN
    NV_STATUS rmStatus;                        // OUT
} uvm_set_range_group_t;

#define UVM_IOCTL_ALLOC UVM_IO(UVM_IOCTL_BASE + 32)
typedef struct uvm_alloc
{
    NvU64                   rangeGroupId                    NV_ALIGN_BYTES(8); // IN
    NvU64                   base                            NV_ALIGN_BYTES(8); // IN
    NvU64                   length                          NV_ALIGN_BYTES(8); // IN
    NvProcessorUuid         homeProcessorUuid;                                 // IN
    UvmGpuMappingAttributes perGpuAttributes[UVM_MAX_GPUS];                    // IN
    NvU64                   gpuAttributesCount              NV_ALIGN_BYTES(8); // IN
    NV_STATUS               rmStatus;                                          // OUT
} uvm_alloc_t;

#define UVM_IOCTL_MAP_EXTERNAL_ALLOCATION UVM_IO(UVM_IOCTL_BASE + 33)
typedef struct uvm_map_external_allocation
{
    NvU64                   base                            NV_ALIGN_BYTES(8); // IN
    NvU64                   length                          NV_ALIGN_BYTES(8); // IN
    NvU64                   offset                          NV_ALIGN_BYTES(8); // IN
    UvmGpuMappingAttributes perGpuAttributes[UVM_MAX_GPUS];                    // IN
    NvU64                   gpuAttributesCount              NV_ALIGN_BYTES(8); // IN
    NvS32                   rmCtrlFd;                                          // IN
    NvU32                   hClient;                                           // IN
    NvU32                   hMemory;                                           // IN
    NvU32                   hClientFromRm;                                     // IN
    NvU32                   hMemoryFromRm;                                     // IN
    NV_STATUS               rmStatus;                                          // OUT
} uvm_map_external_allocation_t;

#define UVM_IOCTL_FREE UVM_IO(UVM_IOCTL_BASE + 34)
typedef struct uvm_free
{
    NvU64     base      NV_ALIGN_BYTES(8); // IN
    NvU64     length    NV_ALIGN_BYTES(8); // IN
    NV_STATUS rmStatus;                    // OUT
} uvm_free_t;

#define UVM_IOCTL_MEM_MAP UVM_IO(UVM_IOCTL_BASE + 35)
typedef struct uvm_mem_map
{
    NvP64     regionBase   NV_ALIGN_BYTES(8); // IN
    NvU64     regionLength NV_ALIGN_BYTES(8); // IN
    NV_STATUS rmStatus;                       // OUT
} uvm_mem_map_t;

#define UVM_IOCTL_DEBUG_ACCESS_MEMORY UVM_IO(UVM_IOCTL_BASE + 36)
typedef struct uvm_debug_access_memory
{
#ifdef __linux__
    NvS32               sessionIndex;                    // IN
#endif
    NvU64               baseAddress   NV_ALIGN_BYTES(8); // IN
    NvU64               sizeInBytes   NV_ALIGN_BYTES(8); // IN
    NvU32               accessType;                      // IN (UvmDebugAccessType)
    NvU64               buffer        NV_ALIGN_BYTES(8); // IN/OUT
    NvBool              isBitmaskSet;                    // OUT
    NvU64               bitmask       NV_ALIGN_BYTES(8); // IN/OUT
    NV_STATUS           rmStatus;                        // OUT
} uvm_debug_access_memory_t;

#define UVM_IOCTL_REGISTER_GPU UVM_IO(UVM_IOCTL_BASE + 37)
typedef struct uvm_register_gpu
{
    NvProcessorUuid gpu_uuid; // IN
    NV_STATUS       rmStatus; // OUT
} uvm_register_gpu_t;

#define UVM_IOCTL_UNREGISTER_GPU UVM_IO(UVM_IOCTL_BASE + 38)
typedef struct uvm_unregister_gpu
{
    NvProcessorUuid gpu_uuid; // IN
    NV_STATUS       rmStatus; // OUT
} uvm_unregister_gpu_t;

#define UVM_IOCTL_PAGEABLE_MEM_ACCESS UVM_IO(UVM_IOCTL_BASE + 39)
typedef struct uvm_pageable_mem_access
{
    NvBool    pageableMemAccess; // OUT
    NV_STATUS rmStatus;          // OUT
} uvm_pageable_mem_access_t;

#define UVM_IOCTL_PREVENT_MIGRATION_RANGE_GROUPS UVM_IO(UVM_IOCTL_BASE + 40)
#define UVM_MAX_RANGE_GROUPS_PER_IOCTL_CALL 32
typedef struct uvm_prevent_migration_range_groups
{
    NvU64     rangeGroupIds[UVM_MAX_RANGE_GROUPS_PER_IOCTL_CALL] NV_ALIGN_BYTES(8); // IN
    NvU64     numGroupIds                                        NV_ALIGN_BYTES(8); // IN
    NV_STATUS rmStatus;                                                             // OUT
} uvm_prevent_migration_range_groups_t;

#define UVM_IOCTL_ALLOW_MIGRATION_RANGE_GROUPS UVM_IO(UVM_IOCTL_BASE + 41)
typedef struct uvm_allow_migration_range_groups
{
    NvU64     rangeGroupIds[UVM_MAX_RANGE_GROUPS_PER_IOCTL_CALL] NV_ALIGN_BYTES(8); // IN
    NvU64     numGroupIds                                        NV_ALIGN_BYTES(8); // IN
    NV_STATUS rmStatus;                                                             // OUT
} uvm_allow_migration_range_groups_t;

#define UVM_IOCTL_SET_PREFERRED_LOCATION UVM_IO(UVM_IOCTL_BASE + 42)
typedef struct uvm_set_preferred_location
{
    NvU64           requestedBase      NV_ALIGN_BYTES(8); // IN
    NvU64           length             NV_ALIGN_BYTES(8); // IN
    NvProcessorUuid preferredLocation;                    // IN
    NV_STATUS       rmStatus;                             // OUT
} uvm_set_preferred_location_t;

#define UVM_IOCTL_UNSET_PREFERRED_LOCATION UVM_IO(UVM_IOCTL_BASE + 43)
typedef struct uvm_unset_preferred_location
{
    NvU64     requestedBase NV_ALIGN_BYTES(8); // IN
    NvU64     length        NV_ALIGN_BYTES(8); // IN
    NV_STATUS rmStatus;                        // OUT
} uvm_unset_preferred_location_t;

#define UVM_IOCTL_ENABLE_READ_DUPLICATION UVM_IO(UVM_IOCTL_BASE + 44)
typedef struct uvm_enable_read_duplication
{
    NvU64     requestedBase NV_ALIGN_BYTES(8); // IN
    NvU64     length        NV_ALIGN_BYTES(8); // IN
    NV_STATUS rmStatus;                        // OUT
} uvm_enable_read_duplication_t;

#define UVM_IOCTL_DISABLE_READ_DUPLICATION UVM_IO(UVM_IOCTL_BASE + 45)
typedef struct uvm_disable_read_duplication
{
    NvU64     requestedBase NV_ALIGN_BYTES(8); // IN
    NvU64     length        NV_ALIGN_BYTES(8); // IN
    NV_STATUS rmStatus;                        // OUT
} uvm_disable_read_duplication_t;

#define UVM_IOCTL_SET_ACCESSED_BY UVM_IO(UVM_IOCTL_BASE + 46)
typedef struct uvm_set_accessed_by
{
    NvU64           requestedBase   NV_ALIGN_BYTES(8); // IN
    NvU64           length          NV_ALIGN_BYTES(8); // IN
    NvProcessorUuid accessedByUuid;                    // IN
    NV_STATUS       rmStatus;                          // OUT
} uvm_set_accessed_by_t;

#define UVM_IOCTL_UNSET_ACCESSED_BY UVM_IO(UVM_IOCTL_BASE + 47)
typedef struct uvm_unset_accessed_by
{
    NvU64           requestedBase   NV_ALIGN_BYTES(8); // IN
    NvU64           length          NV_ALIGN_BYTES(8); // IN
    NvProcessorUuid accessedByUuid;                    // IN
    NV_STATUS       rmStatus;                          // OUT
} uvm_unset_accessed_by_t;

#define UVM_IOCTL_MIGRATE UVM_IO(UVM_IOCTL_BASE + 51)
typedef struct uvm_migrate
{
    NvU64           base               NV_ALIGN_BYTES(8); // IN
    NvU64           length             NV_ALIGN_BYTES(8); // IN
    NvProcessorUuid destinationUuid;                      // IN
    NvU32           flags;                                // IN
    NvU64           semaphoreAddress   NV_ALIGN_BYTES(8); // IN
    NvU32           semaphorePayload;                     // IN
    NV_STATUS       rmStatus;                             // OUT
} uvm_migrate_t;

#define UVM_IOCTL_MIGRATE_RANGE_GROUP UVM_IO(UVM_IOCTL_BASE + 53)
typedef struct uvm_migrate_range_group
{
    NvU64           rangeGroupId       NV_ALIGN_BYTES(8); // IN
    NvProcessorUuid destinationUuid;                      // IN
    NV_STATUS       rmStatus;                             // OUT
} uvm_migrate_range_group_t;

#define UVM_IOCTL_ENABLE_SYSTEM_WIDE_ATOMICS UVM_IO(UVM_IOCTL_BASE + 54)
typedef struct uvm_enable_system_wide_atomics
{
    NvProcessorUuid gpu_uuid; // IN
    NV_STATUS       rmStatus; // OUT
} uvm_enable_system_wide_atomics_t;

#define UVM_IOCTL_DISABLE_SYSTEM_WIDE_ATOMICS UVM_IO(UVM_IOCTL_BASE + 55)
typedef struct uvm_disable_system_wide_atomics
{
    NvProcessorUuid gpu_uuid; // IN
    NV_STATUS       rmStatus; // OUT
} uvm_disable_system_wide_atomics_t;

#define UVM_IOCTL_TOOLS_INIT_EVENT_TRACKER UVM_IO(UVM_IOCTL_BASE + 56)
typedef struct uvm_tools_init_event_tracker
{
    NvU64           queueBuffer        NV_ALIGN_BYTES(8); // IN
    NvU64           queueBufferSize    NV_ALIGN_BYTES(8); // IN
    NvU64           controlBuffer      NV_ALIGN_BYTES(8); // IN
    NvProcessorUuid processor;                            // IN
    NvU32           allProcessors;                        // IN
    NvU32           uvmFd;                                // IN
    NV_STATUS       rmStatus;                             // OUT
} uvm_tools_init_event_tracker_t;

#define UVM_IOCTL_TOOLS_SET_NOTIFICATION_THRESHOLD UVM_IO(UVM_IOCTL_BASE + 57)
typedef struct uvm_tools_set_notification_threshold
{
    NvU32     notificationThreshold;                       // IN
    NV_STATUS rmStatus;                                    // OUT
} uvm_tools_set_notification_threshold_t;

#define UVM_IOCTL_TOOLS_EVENT_QUEUE_ENABLE_EVENTS UVM_IO(UVM_IOCTL_BASE + 58)
typedef struct uvm_tools_event_queue_enable_events
{
    NvU64     eventTypeFlags            NV_ALIGN_BYTES(8); // IN
    NV_STATUS rmStatus;                                    // OUT
} uvm_tools_event_queue_enable_events_t;

#define UVM_IOCTL_TOOLS_EVENT_QUEUE_DISABLE_EVENTS UVM_IO(UVM_IOCTL_BASE + 59)
typedef struct uvm_tools_event_queue_disable_events
{
    NvU64     eventTypeFlags            NV_ALIGN_BYTES(8); // IN
    NV_STATUS rmStatus;                                    // OUT
} uvm_tools_event_queue_disable_events_t;

#define UVM_IOCTL_TOOLS_ENABLE_COUNTERS UVM_IO(UVM_IOCTL_BASE + 60)
typedef struct uvm_tools_enable_counters
{
    NvU64     counterTypeFlags          NV_ALIGN_BYTES(8); // IN
    NV_STATUS rmStatus;                                    // OUT
} uvm_tools_enable_counters_t;

#define UVM_IOCTL_TOOLS_DISABLE_COUNTERS UVM_IO(UVM_IOCTL_BASE + 61)
typedef struct uvm_tools_disable_counters
{
    NvU64     counterTypeFlags          NV_ALIGN_BYTES(8); // IN
    NV_STATUS rmStatus;                                    // OUT
} uvm_tools_disable_counters_t;

#define UVM_IOCTL_TOOLS_READ_PROCESS_MEMORY UVM_IO(UVM_IOCTL_BASE + 62)
typedef struct uvm_tools_read_process_memory
{
    NvU64     buffer                    NV_ALIGN_BYTES(8); // IN
    NvU64     size                      NV_ALIGN_BYTES(8); // IN
    NvU64     targetVa                  NV_ALIGN_BYTES(8); // IN
    NvU64     bytesRead                 NV_ALIGN_BYTES(8); // OUT
    NV_STATUS rmStatus;                                    // OUT
} uvm_tools_read_process_memory_t;

#define UVM_IOCTL_TOOLS_WRITE_PROCESS_MEMORY UVM_IO(UVM_IOCTL_BASE + 63)
typedef struct uvm_tools_write_process_memory
{
    NvU64     buffer                    NV_ALIGN_BYTES(8); // IN
    NvU64     size                      NV_ALIGN_BYTES(8); // IN
    NvU64     targetVa                  NV_ALIGN_BYTES(8); // IN
    NvU64     bytesWritten              NV_ALIGN_BYTES(8); // OUT
    NV_STATUS rmStatus;                                    // OUT
} uvm_tools_write_process_memory_t;

#define UVM_IOCTL_TOOLS_GET_PROCESSOR_UUID_TABLE UVM_IO(UVM_IOCTL_BASE + 64)
typedef struct uvm_tools_get_processor_uuid_table
{
    NvU64     tablePtr                 NV_ALIGN_BYTES(8); // IN
    NvU32     count;                                      // IN/OUT
    NV_STATUS rmStatus;                                   // OUT
} uvm_tools_get_processor_uuid_table_t;


#define UVM_IOCTL_MAP_DYNAMIC_PARALLELISM_REGION UVM_IO(UVM_IOCTL_BASE + 65)
typedef struct uvm_map_dynamic_parallelism_region
{
    NvU64                   base                            NV_ALIGN_BYTES(8); // IN
    NvU64                   length                          NV_ALIGN_BYTES(8); // IN
    NvProcessorUuid         gpuUuid;                                           // IN
    NV_STATUS               rmStatus;                                          // OUT
} uvm_map_dynamic_parallelism_region_t;

#define UVM_IOCTL_UNMAP_EXTERNAL_ALLOCATION UVM_IO(UVM_IOCTL_BASE + 66)
typedef struct uvm_unmap_external_allocation
{
    NvU64                   base                            NV_ALIGN_BYTES(8); // IN
    NvProcessorUuid         gpuUuid;                                           // IN
    NV_STATUS               rmStatus;                                          // OUT
} uvm_unmap_external_allocation_t;


#define UVM_IOCTL_TOOLS_FLUSH_EVENTS UVM_IO(UVM_IOCTL_BASE + 67)
typedef struct uvm_tools_flush_events
{
    NV_STATUS rmStatus;                                   // OUT
} uvm_tools_flush_events_t;

#define UVM_IOCTL_ALLOC_SEMAPHORE_POOL UVM_IO(UVM_IOCTL_BASE + 68)
typedef struct uvm_alloc_semaphore_pool
{
    NvU64                   base                            NV_ALIGN_BYTES(8); // IN
    NvU64                   length                          NV_ALIGN_BYTES(8); // IN
    UvmGpuMappingAttributes perGpuAttributes[UVM_MAX_GPUS];                    // IN
    NvU64                   gpuAttributesCount              NV_ALIGN_BYTES(8); // IN
    NV_STATUS               rmStatus;                                          // OUT
} uvm_alloc_semaphore_pool_t;

#define UVM_IOCTL_IS_8_SUPPORTED UVM_IO(UVM_IOCTL_BASE + 2047)
typedef struct uvm_is_8_supported
{
    NvU32     is8Supported; // OUT
    NV_STATUS rmStatus;     // OUT
} uvm_is_8_supported_t;


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* RR_NVIDIA_UVM_H */
