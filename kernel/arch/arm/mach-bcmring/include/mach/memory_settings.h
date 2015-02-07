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
* Copyright 2004 - 2008 Broadcom Corporation.  All rights reserved.
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

#ifndef MEMORY_SETTINGS_H
#define MEMORY_SETTINGS_H

/* ---- Include Files ---------------------------------------- */
/* ---- Constants and Types ---------------------------------- */

/* Memory devices */
/* NAND Flash timing for 166 MHz setting */
#define HW_CFG_NAND_tBTA  (5 << 16)	/* Bus turnaround cycle (n)        0-7  (30 ns) */
#define HW_CFG_NAND_tWP   (4 << 11)	/* Write pulse width cycle (n+1)   0-31 (25 ns) */
#define HW_CFG_NAND_tWR   (1 << 9)	/* Write recovery cycle (n+1)      0-3  (10 ns) */
#define HW_CFG_NAND_tAS   (0 << 7)	/* Write address setup cycle (n+1) 0-3  ( 0 ns) */
#define HW_CFG_NAND_tOE   (3 << 5)	/* Output enable delay cycle (n)   0-3  (15 ns) */
#define HW_CFG_NAND_tRC   (7 << 0)	/* Read access cycle (n+2)         0-31 (50 ns) */

#define HW_CFG_NAND_TCR (HW_CFG_NAND_tBTA \
	| HW_CFG_NAND_tWP  \
	| HW_CFG_NAND_tWR  \
	| HW_CFG_NAND_tAS  \
	| HW_CFG_NAND_tOE  \
	| HW_CFG_NAND_tRC)

/* NOR Flash timing for 166 MHz setting */
#define HW_CFG_NOR_TPRC_TWLC (0 << 19)	/* Page read access cycle / Burst write latency (n+2 / n+1) (max 25ns) */
#define HW_CFG_NOR_TBTA      (0 << 16)	/* Bus turnaround cycle (n)                                 (DNA)      */
#define HW_CFG_NOR_TWP       (6 << 11)	/* Write pulse width cycle (n+1)                            (35ns)     */
#define HW_CFG_NOR_TWR       (0 << 9)	/* Write recovery cycle (n+1)                               (0ns)      */
#define HW_CFG_NOR_TAS       (0 << 7)	/* Write address setup cycle (n+1)                          (0ns)      */
#define HW_CFG_NOR_TOE       (0 << 5)	/* Output enable delay cycle (n)                            (max 25ns) */
#define HW_CFG_NOR_TRC_TLC   (0x10 << 0)	/* Read access cycle / Burst read latency (n+2 / n+1)       (100ns)    */

#define HW_CFG_FLASH0_TCR (HW_CFG_NOR_TPRC_TWLC \
	| HW_CFG_NOR_TBTA      \
	| HW_CFG_NOR_TWP       \
	| HW_CFG_NOR_TWR       \
	| HW_CFG_NOR_TAS       \
	| HW_CFG_NOR_TOE       \
	| HW_CFG_NOR_TRC_TLC)

#define HW_CFG_FLASH1_TCR    HW_CFG_FLASH0_TCR
#define HW_CFG_FLASH2_TCR    HW_CFG_FLASH0_TCR

/* SDRAM Settings */
/* #define HW_CFG_SDRAM_CAS_LATENCY        5    Default 5, Values [3..6] */
/* #define HW_CFG_SDRAM_CHIP_SELECT_CNT    1    Default 1, Vaules [1..2] */
/* #define HW_CFG_SDRAM_SPEED_GRADE        667  Default 667, Values [400,533,667,800] */
/* #define HW_CFG_SDRAM_WIDTH_BITS         16   Default 16, Vaules [8,16] */
#define HW_CFG_SDRAM_SIZE_BYTES         0x10000000	/* Total memory, not per device size */

/* ---- Variable Externs ------------------------------------- */
/* ---- Function Prototypes ---------------------------------- */

#endif /* MEMORY_SETTINGS_H */
