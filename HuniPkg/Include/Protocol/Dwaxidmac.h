/** @file
  Dmac header file.

@copyright
  INTEL CONFIDENTIAL
  Copyright 2022 Intel Corporation.

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary and
  confidential information of Intel Corporation and its suppliers and licensors,
  and is protected by worldwide copyright and trade secret laws and treaty
  provisions. No part of the Material may be used, copied, reproduced, modified,
  published, uploaded, posted, transmitted, distributed, or disclosed in any way
  without Intel's prior express written permission.

  No license under any patent, copyright, trade secret or other intellectual
  property right is granted to or conferred upon you by disclosure or delivery
  of the Materials, either expressly, by implication, inducement, estoppel or
  otherwise. Any license under such intellectual property rights must be
  express and approved by Intel in writing.

  Unless otherwise agreed by Intel in writing, you may not remove or alter
  this notice or any other notice embedded in Materials by Intel or
  Intel's suppliers or licensors in any way.

  This file contains a 'Sample Driver' and is licensed as such under the terms
  of your license agreement with Intel or your vendor. This file may be modified
  by the user, subject to the additional terms of the license agreement.

@par Specification Reference:
**/

#define EDKII_DMAC_PROTOCOL_GUID \
    {0x1ee9447d, 0x85de, 0x467e, {0x9d, 0xed, 0x33, 0x96, 0xe3, 0xa1, 0xfa, 0x58}};




#include <PiDxe.h>
#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DevicePathLib.h>
#include <Library/DxeServicesLib.h>
#include <Protocol/IoMmu.h>

#define DMA_REG_BASE        0xF7001000
#define DMA_MAX_BLOCK_SIZE  0xFFFFF

#define DMA_IDREG(c)           (DMA_REG_BASE+(0x1000*(c))+0x0)
#define DMA_VERREG(c)          (DMA_REG_BASE+(0x1000*(c))+0x8)
#define DMA_CFGREG(c)          (DMA_REG_BASE+(0x1000*(c))+0x10)
#define DMA_CHENREG(c)         (DMA_REG_BASE+(0x1000*(c))+0x18)
#define DMA_RSTREG(c)          (DMA_REG_BASE+(0x1000*(c))+0x58)

#define DMA_SAR_REG(c,ch)     (DMA_REG_BASE+(0x1000*(c))+(0x100*(ch))+0x100)
#define DMA_DAR_REG(c,ch)     (DMA_REG_BASE+(0x1000*(c))+(0x100*(ch))+0x108)
#define DMA_BLK_TS_REG(c,ch)  (DMA_REG_BASE+(0x1000*(c))+(0x100*(ch))+0x110)
#define DMA_CTL_REG(c,ch)     (DMA_REG_BASE+(0x1000*(c))+(0x100*(ch))+0x118)
#define DMA_CFG_REG(c,ch)     (DMA_REG_BASE+(0x1000*(c))+(0x100*(ch))+0x120)

typedef struct _EFI_DMAC_IO_PROTOCOL EFI_DMAC_IO_PROTOCOL;
typedef enum {
    MEM_TO_MEM_DMAC = 0x0,
    MEM_TO_PER_DMAC = 0x1,
    PER_TO_MEM_DMAC = 0x2,
} DMA_MODE;

typedef union {
    UINT64  Uint64;
    struct
    {
      UINT64 sms :1;
      UINT64 rsvd_0 :1;
      UINT64 dms :1;
      UINT64 rsvd_1 :1;
      UINT64 sinc :1;
      UINT64 rsvd_2 :1;
      UINT64 dinc :1;
      UINT64 rsvd_3 :1;
      UINT64 src_tr_width :3;
      UINT64 dst_tr_width :3;
      UINT64 src_msize :4;
      UINT64 dst_msize :4;
      UINT64 ar_cache :4;
      UINT64 aw_cache :4;
      UINT64 nonposted :1;
      UINT64 rsvd_4 :1;
      UINT64 ar_prot :3;
      UINT64 aw_prot :3;
      UINT64 arlen_en :1;
      UINT64 arlen :8;
      UINT64 awlen_en :1;
      UINT64 awlen :8;
      UINT64 src_stat_en :1;
      UINT64 dst_stat_en :1;
      UINT64 ioc_blkftr :1;
      UINT64 rsvd_5 :3;
      UINT64 shadow_last :1;
      UINT64 shadow_valid :1;
    } Bits;
}dmac_ctl_reg;

typedef union {
    UINT64  Uint64;
    struct
    {
      UINT64 src_multiblk_type :2;
      UINT64 dst_multiblk_type :2;
      UINT64 rsvd_0 :14;
      UINT64 rd_uid :4;
      UINT64 rsvd_1 :3;
      UINT64 wr_uid :4;
      UINT64 rsvd_2 :3;
      UINT64 tt_fc :3;
      UINT64 hs_sel_src :1;
      UINT64 hs_sel_dst :1;
      UINT64 src_hwhs_pol :1;
      UINT64 dst_hwhs_pol :1;
      UINT64 rsvd_3 :25;
   } Bits;
}dmac_cfg_reg;
typedef struct
{
    UINT64	src;
    UINT64	dst;
    UINT64	length;
    UINT8	dma_mode;
}dma_config;
typedef
EFI_STATUS
(EFIAPI *EFI_DMAC_TRANSFER)(
  IN EFI_DMAC_IO_PROTOCOL         *This,
  IN dma_config                   *config
);

struct _EFI_DMAC_IO_PROTOCOL {
  UINT32                      Revision;
  UINT32                      Regbase;
  EFI_DMAC_TRANSFER           Transfer;
};

extern EFI_GUID gDmatransferProtocolGuid;

