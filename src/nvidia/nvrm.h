/* -*- Mode: C; tab-width: 8; c-basic-offset: 2; indent-tabs-mode: nil; -*- */

/*
 * Based on https://github.com/envytools/envytools/blob/master/demmt/nvrm_ioctl.h
 *
 * Copyright (C) 2013 Marcin Kościelnicki <koriakin@0x04.net>
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef RR_NVIDIA_NVRM_H
#define RR_NVIDIA_NVRM_H 1

#include <sys/ioctl.h>
#include <sys/types.h>

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define NVRM_IOCTL_MAGIC 'F'
#define NVRM_IO(nr) _IO(NVRM_IOCTL_MAGIC, nr)
#define NVRM_IOR(nr, type) _IOR(NVRM_IOCTL_MAGIC, nr, type)
#define NVRM_IOW(nr, type) _IOW(NVRM_IOCTL_MAGIC, nr, type)
#define NVRM_IOWR(nr, type) _IOWR(NVRM_IOCTL_MAGIC, nr, type)

typedef struct {
    NvU32    domain;        /* PCI domain number   */
    NvU8     bus;           /* PCI bus number      */
    NvU8     slot;          /* PCI slot number     */
    NvU8     function;      /* PCI function number */
    NvU16    vendor_id;     /* PCI vendor ID       */
    NvU16    device_id;     /* PCI device ID       */
    NvBool   valid;         /* validation flag     */
} nv_pci_info_t;


//
// Public IOCTLs (from nv.h)
//

#define NVRM_IOCTL_ESC_BASE 200

typedef struct nvrm_ioctl_card_info
{
    NvU16         flags;
    nv_pci_info_t pci_info;            /* PCI config information      */
    NvU32         gpu_id;
    NvU16         interrupt_line;
    NvU64         reg_address    NV_ALIGN_BYTES(8);
    NvU64         reg_size       NV_ALIGN_BYTES(8);
    NvU64         fb_address     NV_ALIGN_BYTES(8);
    NvU64         fb_size        NV_ALIGN_BYTES(8);
    NvU32         minor_number;
    NvU8          dev_name[10];  /* device names such as vmgfx[0-32] for vmkernel */
} nvrm_ioctl_card_info_t;
#define NVRM_IOCTL_CARD_INFO NVRM_IOWR(NVRM_IOCTL_ESC_BASE + 0, nvrm_ioctl_card_info_t)

typedef struct nvrm_ioctl_env_info
{
    NvU32 pat_supported;
} nvrm_ioctl_env_info_t;
#define NVRM_IOCTL_ENV_INFO NVRM_IOWR(NVRM_IOCTL_ESC_BASE + 2, nvrm_ioctl_env_info_t)

typedef struct nvrm_ioctl_alloc_os_event
{
    NvHandle hClient;
    NvHandle hDevice;
    NvHandle hOsEvent;
    NvU32    fd;
    NvU32    Status;
} nvrm_ioctl_alloc_os_event_t;
#define NVRM_IOCTL_ALLOC_OS_EVENT NVRM_IOWR(NVRM_IOCTL_ESC_BASE + 6, nvrm_ioctl_alloc_os_event_t)

typedef struct nvrm_ioctl_free_os_event
{
    NvHandle hClient;
    NvHandle hDevice;
    NvU32    fd;
    NvU32    Status;
} nvrm_ioctl_free_os_event_t;
#define NVRM_IOCTL_FREE_OS_EVENT NVRM_IOWR(NVRM_IOCTL_ESC_BASE + 7, nvrm_ioctl_free_os_event_t)

#define NV_RM_API_VERSION_STRING_LENGTH 64
typedef struct nvrm_ioctl_rm_api_version
{
    NvU32 cmd;
    NvU32 reply;
    char versionString[NV_RM_API_VERSION_STRING_LENGTH];
} nvrm_ioctl_rm_api_version_t;
#define NVRM_IOCTL_CHECK_VERSION_STR NVRM_IOWR(NVRM_IOCTL_ESC_BASE + 10, nvrm_ioctl_rm_api_version_t)


//
// Private IOCTLs
//

typedef struct nvrm_ioctl_create_ctx {
  uint32_t handle;
  uint32_t unk04;
  uint32_t unk08;
} nvrm_ioctl_create_ctx_t;
#define NVRM_IOCTL_CREATE_CTX NVRM_IOWR(0x22, nvrm_ioctl_create_ctx_t)

typedef struct nvrm_ioctl_create_dev_obj {
  uint32_t cid;
  uint32_t handle;
  uint32_t unk08;
  uint32_t unk0c;
  uint32_t ptr;
  uint32_t unk14;
  uint32_t unk18;
  uint32_t unk1c;
} nvrm_ioctl_create_dev_obj_t;
#define NVRM_IOCTL_CREATE_DEV_OBJ NVRM_IOWR(0x23, nvrm_ioctl_create_dev_obj_t)

/* used on dev fd */
typedef struct nvrm_ioctl_create_vspace {
  uint32_t cid;
  uint32_t parent;
  uint32_t handle;
  uint32_t cls;
  uint32_t flags;
  uint32_t _pad1;
  uint64_t foffset;
  uint64_t limit;
  uint32_t status;
  uint32_t _pad2;
} nvrm_ioctl_create_vspace_t;
#define NVRM_IOCTL_CREATE_VSPACE NVRM_IOWR(0x27, nvrm_ioctl_create_vspace_t)

typedef struct nvrm_ioctl_create_simple {
  uint32_t cid;
  uint32_t parent;
  uint32_t handle;
  uint32_t cls;
  uint32_t status;
} nvrm_ioctl_create_simple_t;
#define NVRM_IOCTL_CREATE_SIMPLE NVRM_IOWR(0x28, nvrm_ioctl_create_simple_t)

typedef struct nvrm_ioctl_destroy {
  uint32_t cid;
  uint32_t parent;
  uint32_t handle;
  uint32_t status;
} nvrm_ioctl_destroy_t;
#define NVRM_IOCTL_DESTROY NVRM_IOWR(0x29, nvrm_ioctl_destroy_t)

typedef struct nvrm_ioctl_call {
  uint32_t cid;
  uint32_t handle;
  uint32_t mthd;
  uint32_t _pad;
  uint64_t addr;
  uint32_t size;
  uint32_t status;
} nvrm_ioctl_call_t;
#define NVRM_IOCTL_CALL NVRM_IOWR(0x2a, nvrm_ioctl_call_t)

typedef struct nvrm_ioctl_create {
  uint32_t cid;
  uint32_t parent;
  uint32_t handle;
  uint32_t cls;
  uint64_t addr;
  uint32_t status;
  uint32_t _pad;
} nvrm_ioctl_create_t;
#define NVRM_IOCTL_CREATE NVRM_IOWR(0x2b, nvrm_ioctl_create_t)

typedef struct nvrm_ioctl_create_drv_obj {
  uint32_t cid;
  uint32_t parent;
  uint32_t handle;
  uint32_t cls;
  uint32_t status;
} nvrm_ioctl_create_drv_obj_t;
#define NVRM_IOCTL_CREATE_DRV_OBJ NVRM_IOWR(0x2d, nvrm_ioctl_create_drv_obj_t)

/* used on dev fd */
typedef struct nvrm_ioctl_get_param {
  uint32_t cid;
  uint32_t handle; /* device */
  uint32_t key;
  uint32_t value; /* out */
  uint32_t status;
} nvrm_ioctl_get_param_t;
#define NVRM_IOCTL_GET_PARAM NVRM_IOWR(0x32, nvrm_ioctl_get_param_t)

typedef struct nvrm_ioctl_create_unk34 {
  uint32_t cid;
  uint32_t parent;
  uint32_t handle;
  uint32_t cid2;
  uint32_t handle2;
  uint32_t unk14;
  uint32_t status;
} nvrm_ioctl_create_unk34_t;
#define NVRM_IOCTL_CREATE_UNK34 NVRM_IOWR(0x34, nvrm_ioctl_create_unk34_t)

/* used on dev fd */
typedef struct nvrm_ioctl_query {
  uint32_t cid;
  uint32_t handle;
  uint32_t query;
  uint32_t size;
  uint64_t ptr;
  uint32_t status;
  uint32_t _pad;
} nvrm_ioctl_query_t;
#define NVRM_IOCTL_QUERY NVRM_IOWR(0x37, nvrm_ioctl_query_t)

typedef struct nvrm_ioctl_unk38 {
  uint32_t cid;
  uint32_t handle;
  uint32_t unk08;
  uint32_t size;
  uint64_t ptr;
  uint32_t status;
  uint32_t _pad;
} nvrm_ioctl_unk38_t;
#define NVRM_IOCTL_UNK38 NVRM_IOWR(0x38, nvrm_ioctl_unk38_t)

typedef struct nvrm_ioctl_sched_fifo {
  uint32_t cid;
  uint32_t dev;
  uint32_t handle; // NVRM_FIFO_* handle / something else
  uint32_t cnt;    // 0 when handle is a fifo, >0 otherwise
  uint64_t ptr1;
  uint64_t ptr2;
  uint64_t ptr3;
  uint32_t unk28;
  uint32_t unk2c;
  uint32_t status;
  uint32_t _pad;
} nvrm_ioctl_sched_fifo_t;
#define NVRM_IOCTL_SCHED_FIFO NVRM_IOWR(0x41, nvrm_ioctl_sched_fifo_t)

typedef struct nvrm_ioctl_disp_unk48 {
  uint32_t cid;
  uint32_t handle;
  uint32_t unk08;
  uint32_t _pad;
} nvrm_ioctl_disp_unk48_t;
#define NVRM_IOCTL_DISP_UNK48 NVRM_IOWR(0x48, nvrm_ioctl_disp_unk48_t)

typedef struct nvrm_ioctl_memory {
  uint32_t cid;
  uint32_t parent;
  uint32_t cls;
  uint32_t unk0c;
  uint32_t status;
  uint32_t unk14;
  uint64_t vram_total;
  uint64_t vram_free;
  uint32_t vspace;
  uint32_t handle;
  uint32_t unk30;
  uint32_t flags1;
  uint64_t unk38;
  uint32_t flags2;
  uint32_t unk44;
  uint64_t unk48;
  uint32_t flags3;
  uint32_t unk54;
  uint64_t unk58;
  uint64_t size;
  uint64_t align;
  uint64_t base;
  uint64_t limit;
  uint64_t unk80;
  uint64_t unk88;
  uint64_t unk90;
  uint64_t unk98;
} nvrm_ioctl_memory_t;
typedef struct nvrm_ioctl_memory2 {
  uint32_t cid;
  uint32_t parent;
  uint32_t cls;
  uint32_t unk0c;
  uint32_t status;
  uint32_t unk14;
  uint64_t vram_total;
  uint64_t vram_free;
  uint32_t vspace;
  uint32_t handle;
  uint32_t unk30[32];
} nvrm_ioctl_memory2_t;
#define NVRM_IOCTL_MEMORY NVRM_IOWR(0x4a, nvrm_ioctl_memory_t)
#define NVRM_IOCTL_MEMORY2 NVRM_IOWR(0x4a, nvrm_ioctl_memory2_t)

typedef struct nvrm_ioctl_config {
  uint32_t cid;
  uint32_t handle;
  uint64_t unk08;
  uint64_t unk10;
  uint64_t slen;
  uint64_t sptr;
  uint64_t unk28;
  uint64_t unk30;
  uint64_t unk38; /* out */
  uint32_t status;
  uint32_t _pad;
} nvrm_ioctl_config_t;
#define NVRM_IOCTL_CONFIG NVRM_IOWR(0x4d, nvrm_ioctl_config_t)

typedef struct nvrm_ioctl_unk4d_old {
  uint32_t unk00;
  uint32_t unk04;
  uint32_t unk08;
  uint32_t unk0c;
  uint32_t unk10;
  uint32_t unk14;
  uint32_t ptr;
  uint32_t unk1c;
  uint32_t unk20;
  uint32_t unk24;
  uint32_t unk28;
  uint32_t unk2c;
  uint32_t unk30;
  uint32_t unk34;
  uint32_t unk38;
  uint32_t unk3c;
} nvrm_ioctl_unk4d_old_t;
#define NVRM_IOCTL_UNK4D_OLD NVRM_IOWR(0x4d, nvrm_ioctl_unk4d_old_t)

typedef struct nvrm_ioctl_host_map {
  uint32_t cid;
  uint32_t subdev;
  uint32_t handle;
  uint32_t _pad;
  uint64_t base;
  uint64_t limit;
  uint64_t foffset;
  uint32_t status;
  uint32_t unk;
} nvrm_ioctl_host_map_t;
#define NVRM_IOCTL_HOST_MAP NVRM_IOWR(0x4e, nvrm_ioctl_host_map_t)

typedef struct nvrm_ioctl_host_unmap {
  uint32_t cid;
  uint32_t subdev;
  uint32_t handle;
  uint32_t _pad;
  uint64_t foffset;
  uint32_t status;
  uint32_t _pad2;
} nvrm_ioctl_host_unmap_t;
#define NVRM_IOCTL_HOST_UNMAP NVRM_IOWR(0x4f, nvrm_ioctl_host_unmap_t)

typedef struct nvrm_ioctl_unk52 {
  uint64_t ptr;
  uint32_t unk08; /*cnt?*/
  uint32_t status;
} nvrm_ioctl_unk52_t;
#define NVRM_IOCTL_UNK52 NVRM_IOWR(0x52, nvrm_ioctl_unk52_t)

typedef struct nvrm_ioctl_create_dma {
  uint32_t cid;
  uint32_t handle;
  uint32_t cls;
  uint32_t flags;
  uint32_t _pad1;
  uint32_t parent;
  uint64_t base;
  uint64_t limit;
  uint32_t status;
  uint32_t _pad2;
} nvrm_ioctl_create_dma_t;
#define NVRM_IOCTL_CREATE_DMA NVRM_IOWR(0x54, nvrm_ioctl_create_dma_t)

typedef struct nvrm_ioctl_vspace_map {
  uint32_t cid;
  uint32_t dev;
  uint32_t vspace;
  uint32_t handle;
  uint64_t base;
  uint64_t size;
  uint32_t flags;
  uint32_t _pad1;
  uint64_t addr;
  uint32_t status;
  uint32_t _pad2;
} nvrm_ioctl_vspace_map_t;
#define NVRM_IOCTL_VSPACE_MAP NVRM_IOWR(0x57, nvrm_ioctl_vspace_map_t)

typedef struct nvrm_ioctl_vspace_unmap {
  uint32_t cid;
  uint32_t dev;
  uint32_t vspace;
  uint32_t handle;
  uint64_t unk10;
  uint64_t addr;
  uint32_t status;
  uint32_t _pad;
} nvrm_ioctl_vspace_unmap_t;
#define NVRM_IOCTL_VSPACE_UNMAP NVRM_IOWR(0x58, nvrm_ioctl_vspace_unmap_t)

typedef struct nvrm_ioctl_bind {
  uint32_t cid;
  uint32_t target;
  uint32_t handle;
  uint32_t status;
} nvrm_ioctl_bind_t;
#define NVRM_IOCTL_BIND NVRM_IOWR(0x59, nvrm_ioctl_bind_t)

typedef struct nvrm_ioctl_unk5e {
  uint32_t cid;
  uint32_t subdev;
  uint32_t handle;
  uint32_t _pad1;
  uint64_t foffset;
  uint64_t ptr; /* to just-mmapped thing */
  uint32_t status;
  uint32_t _pad2;
} nvrm_ioctl_unk5e_t;
#define NVRM_IOCTL_UNK5E NVRM_IOWR(0x5e, nvrm_ioctl_unk5e_t)


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* RR_NVIDIA_NVRM_H */
