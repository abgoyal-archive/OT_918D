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

#ifndef __ASM_SH_SE7724_H
#define __ASM_SH_SE7724_H

/*
 * linux/include/asm-sh/se7724.h
 *
 * Copyright (C) 2009 Renesas Solutions Corp.
 *
 * Kuninori Morimoto <morimoto.kuninori@renesas.com>
 *
 * Hitachi UL SolutionEngine 7724 Support.
 *
 * Based on se7722.h
 * Copyright (C) 2007  Nobuhiro Iwamatsu
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 */
#include <asm/addrspace.h>

/* SH Eth */
#define SH_ETH_ADDR	(0xA4600000)
#define SH_ETH_MAHR	(SH_ETH_ADDR + 0x1C0)
#define SH_ETH_MALR	(SH_ETH_ADDR + 0x1C8)

#define PA_LED		(0xba203000)	/* 8bit LED */
#define IRQ_MODE	(0xba200010)
#define IRQ0_SR		(0xba200014)
#define IRQ1_SR		(0xba200018)
#define IRQ2_SR		(0xba20001c)
#define IRQ0_MR		(0xba200020)
#define IRQ1_MR		(0xba200024)
#define IRQ2_MR		(0xba200028)

/* IRQ */
#define IRQ0_IRQ        32
#define IRQ1_IRQ        33
#define IRQ2_IRQ        34

/* Bits in IRQ012 registers */
#define SE7724_FPGA_IRQ_BASE	220

/* IRQ0 */
#define IRQ0_BASE	SE7724_FPGA_IRQ_BASE
#define IRQ0_KEY	(IRQ0_BASE + 12)
#define IRQ0_RMII	(IRQ0_BASE + 13)
#define IRQ0_SMC	(IRQ0_BASE + 14)
#define IRQ0_MASK	0x7fff
#define IRQ0_END	IRQ0_SMC
/* IRQ1 */
#define IRQ1_BASE	(IRQ0_END + 1)
#define IRQ1_TS		(IRQ1_BASE + 0)
#define IRQ1_MASK	0x0001
#define IRQ1_END	IRQ1_TS
/* IRQ2 */
#define IRQ2_BASE	(IRQ1_END + 1)
#define IRQ2_USB0	(IRQ1_BASE + 0)
#define IRQ2_USB1	(IRQ1_BASE + 1)
#define IRQ2_MASK	0x0003
#define IRQ2_END	IRQ2_USB1

#define SE7724_FPGA_IRQ_NR	(IRQ2_END - IRQ0_BASE)

/* arch/sh/boards/se/7724/irq.c */
void init_se7724_IRQ(void);

#define __IO_PREFIX		se7724
#include <asm/io_generic.h>

#endif  /* __ASM_SH_SE7724_H */
