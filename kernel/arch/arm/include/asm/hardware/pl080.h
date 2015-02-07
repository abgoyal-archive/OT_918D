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

/* arch/arm/include/asm/hardware/pl080.h
 *
 * Copyright 2008 Openmoko, Inc.
 * Copyright 2008 Simtec Electronics
 *      http://armlinux.simtec.co.uk/
 *      Ben Dooks <ben@simtec.co.uk>
 *
 * ARM PrimeCell PL080 DMA controller
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

/* Note, there are some Samsung updates to this controller block which
 * make it not entierly compatible with the PL080 specification from
 * ARM. When in doubt, check the Samsung documentation first.
 *
 * The Samsung defines are PL080S, and add an extra controll register,
 * the ability to move more than 2^11 counts of data and some extra
 * OneNAND features.
*/

#define PL080_INT_STATUS			(0x00)
#define PL080_TC_STATUS				(0x04)
#define PL080_TC_CLEAR				(0x08)
#define PL080_ERR_STATUS			(0x0C)
#define PL080_ERR_CLEAR				(0x10)
#define PL080_RAW_TC_STATUS			(0x14)
#define PL080_RAW_ERR_STATUS			(0x18)
#define PL080_EN_CHAN				(0x1c)
#define PL080_SOFT_BREQ				(0x20)
#define PL080_SOFT_SREQ				(0x24)
#define PL080_SOFT_LBREQ			(0x28)
#define PL080_SOFT_LSREQ			(0x2C)

#define PL080_CONFIG				(0x30)
#define PL080_CONFIG_M2_BE			(1 << 2)
#define PL080_CONFIG_M1_BE			(1 << 1)
#define PL080_CONFIG_ENABLE			(1 << 0)

#define PL080_SYNC				(0x34)

/* Per channel configuration registers */

#define PL008_Cx_STRIDE				(0x20)
#define PL080_Cx_BASE(x)			((0x100 + (x * 0x20)))
#define PL080_Cx_SRC_ADDR(x)			((0x100 + (x * 0x20)))
#define PL080_Cx_DST_ADDR(x)			((0x104 + (x * 0x20)))
#define PL080_Cx_LLI(x)				((0x108 + (x * 0x20)))
#define PL080_Cx_CONTROL(x)			((0x10C + (x * 0x20)))
#define PL080_Cx_CONFIG(x)			((0x110 + (x * 0x20)))
#define PL080S_Cx_CONTROL2(x)			((0x110 + (x * 0x20)))
#define PL080S_Cx_CONFIG(x)			((0x114 + (x * 0x20)))

#define PL080_CH_SRC_ADDR			(0x00)
#define PL080_CH_DST_ADDR			(0x04)
#define PL080_CH_LLI				(0x08)
#define PL080_CH_CONTROL			(0x0C)
#define PL080_CH_CONFIG				(0x10)
#define PL080S_CH_CONTROL2			(0x10)
#define PL080S_CH_CONFIG			(0x14)

#define PL080_LLI_ADDR_MASK			(0x3fffffff << 2)
#define PL080_LLI_ADDR_SHIFT			(2)
#define PL080_LLI_LM_AHB2			(1 << 0)

#define PL080_CONTROL_TC_IRQ_EN			(1 << 31)
#define PL080_CONTROL_PROT_MASK			(0x7 << 28)
#define PL080_CONTROL_PROT_SHIFT		(28)
#define PL080_CONTROL_PROT_SYS			(1 << 28)
#define PL080_CONTROL_DST_INCR			(1 << 27)
#define PL080_CONTROL_SRC_INCR			(1 << 26)
#define PL080_CONTROL_DST_AHB2			(1 << 25)
#define PL080_CONTROL_SRC_AHB2			(1 << 24)
#define PL080_CONTROL_DWIDTH_MASK		(0x7 << 21)
#define PL080_CONTROL_DWIDTH_SHIFT		(21)
#define PL080_CONTROL_SWIDTH_MASK		(0x7 << 18)
#define PL080_CONTROL_SWIDTH_SHIFT		(18)
#define PL080_CONTROL_DB_SIZE_MASK		(0x7 << 15)
#define PL080_CONTROL_DB_SIZE_SHIFT		(15)
#define PL080_CONTROL_SB_SIZE_MASK		(0x7 << 12)
#define PL080_CONTROL_SB_SIZE_SHIFT		(12)
#define PL080_CONTROL_TRANSFER_SIZE_MASK	(0xfff << 0)
#define PL080_CONTROL_TRANSFER_SIZE_SHIFT	(0)

#define PL080_BSIZE_1				(0x0)
#define PL080_BSIZE_4				(0x1)
#define PL080_BSIZE_8				(0x2)
#define PL080_BSIZE_16				(0x3)
#define PL080_BSIZE_32				(0x4)
#define PL080_BSIZE_64				(0x5)
#define PL080_BSIZE_128				(0x6)
#define PL080_BSIZE_256				(0x7)

#define PL080_WIDTH_8BIT			(0x0)
#define PL080_WIDTH_16BIT			(0x1)
#define PL080_WIDTH_32BIT			(0x2)

#define PL080_CONFIG_HALT			(1 << 18)
#define PL080_CONFIG_ACTIVE			(1 << 17)  /* RO */
#define PL080_CONFIG_LOCK			(1 << 16)
#define PL080_CONFIG_TC_IRQ_MASK		(1 << 15)
#define PL080_CONFIG_ERR_IRQ_MASK		(1 << 14)
#define PL080_CONFIG_FLOW_CONTROL_MASK		(0x7 << 11)
#define PL080_CONFIG_FLOW_CONTROL_SHIFT		(11)
#define PL080_CONFIG_DST_SEL_MASK		(0xf << 6)
#define PL080_CONFIG_DST_SEL_SHIFT		(6)
#define PL080_CONFIG_SRC_SEL_MASK		(0xf << 1)
#define PL080_CONFIG_SRC_SEL_SHIFT		(1)
#define PL080_CONFIG_ENABLE			(1 << 0)

#define PL080_FLOW_MEM2MEM			(0x0)
#define PL080_FLOW_MEM2PER			(0x1)
#define PL080_FLOW_PER2MEM			(0x2)
#define PL080_FLOW_SRC2DST			(0x3)
#define PL080_FLOW_SRC2DST_DST			(0x4)
#define PL080_FLOW_MEM2PER_PER			(0x5)
#define PL080_FLOW_PER2MEM_PER			(0x6)
#define PL080_FLOW_SRC2DST_SRC			(0x7)

/* DMA linked list chain structure */

struct pl080_lli {
	u32	src_addr;
	u32	dst_addr;
	u32	next_lli;
	u32	control0;
};

struct pl080s_lli {
	u32	src_addr;
	u32	dst_addr;
	u32	next_lli;
	u32	control0;
	u32	control1;
};

