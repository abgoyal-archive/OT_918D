/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 */

/*****************************************************************************
* Copyright 2001 - 2008 Broadcom Corporation.  All rights reserved.
*
* Unless you and Broadcom execute a separate written software license
* agreement governing use of this software, this software is licensed to you
* under the terms of the GNU General Public License version 2, available at
* http://www.broadcom.com/licenses/GPLv2.php (the "GPL").
*
* Notwithstanding the above, under no circumstances may you combine this
* software in any way with any other Broadcom software provided under a
* license other than the GPL, without Broadcom's express prior written
* consent.
*****************************************************************************/

/*
*
*****************************************************************************
*
*  REG_NAND.h
*
*  PURPOSE:
*
*     This file contains definitions for the nand registers:
*
*  NOTES:
*
*****************************************************************************/

#if !defined(__ASM_ARCH_REG_NAND_H)
#define __ASM_ARCH_REG_NAND_H

/* ---- Include Files ---------------------------------------------------- */
#include <csp/reg.h>
#include <mach/reg_umi.h>

/* ---- Constants and Types ---------------------------------------------- */

#define HW_NAND_BASE       MM_IO_BASE_NAND	/* NAND Flash */

/* DMA accesses by the bootstrap need hard nonvirtual addresses */
#define REG_NAND_CMD            __REG16(HW_NAND_BASE + 0)
#define REG_NAND_ADDR           __REG16(HW_NAND_BASE + 4)

#define REG_NAND_PHYS_DATA16   (HW_NAND_BASE + 8)
#define REG_NAND_PHYS_DATA8    (HW_NAND_BASE + 8)
#define REG_NAND_DATA16         __REG16(REG_NAND_PHYS_DATA16)
#define REG_NAND_DATA8          __REG8(REG_NAND_PHYS_DATA8)

/* use appropriate offset to make sure it start at the 1K boundary */
#define REG_NAND_PHYS_DATA_DMA   (HW_NAND_BASE + 0x400)
#define REG_NAND_DATA_DMA         __REG32(REG_NAND_PHYS_DATA_DMA)

/* Linux DMA requires physical address of the data register */
#define REG_NAND_DATA16_PADDR    HW_IO_VIRT_TO_PHYS(REG_NAND_PHYS_DATA16)
#define REG_NAND_DATA8_PADDR     HW_IO_VIRT_TO_PHYS(REG_NAND_PHYS_DATA8)
#define REG_NAND_DATA_PADDR      HW_IO_VIRT_TO_PHYS(REG_NAND_PHYS_DATA_DMA)

#define NAND_BUS_16BIT()        (0)
#define NAND_BUS_8BIT()         (!NAND_BUS_16BIT())

/* Register offsets */
#define REG_NAND_CMD_OFFSET     (0)
#define REG_NAND_ADDR_OFFSET    (4)
#define REG_NAND_DATA8_OFFSET   (8)

#endif
