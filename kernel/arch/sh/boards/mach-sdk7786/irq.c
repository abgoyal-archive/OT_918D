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

/*
 * SDK7786 FPGA IRQ Controller Support.
 *
 * Copyright (C) 2010  Matt Fleming
 * Copyright (C) 2010  Paul Mundt
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */
#include <linux/irq.h>
#include <mach/fpga.h>
#include <mach/irq.h>

enum {
	ATA_IRQ_BIT		= 1,
	SPI_BUSY_BIT		= 2,
	LIRQ5_BIT		= 3,
	LIRQ6_BIT		= 4,
	LIRQ7_BIT		= 5,
	LIRQ8_BIT		= 6,
	KEY_IRQ_BIT		= 7,
	PEN_IRQ_BIT		= 8,
	ETH_IRQ_BIT		= 9,
	RTC_ALARM_BIT		= 10,
	CRYSTAL_FAIL_BIT	= 12,
	ETH_PME_BIT		= 14,
};

void __init sdk7786_init_irq(void)
{
	unsigned int tmp;

	/* Enable priority encoding for all IRLs */
	fpga_write_reg(fpga_read_reg(INTMSR) | 0x0303, INTMSR);

	/* Clear FPGA interrupt status registers */
	fpga_write_reg(0x0000, INTASR);
	fpga_write_reg(0x0000, INTBSR);

	/* Unmask FPGA interrupts */
	tmp = fpga_read_reg(INTAMR);
	tmp &= ~(1 << ETH_IRQ_BIT);
	fpga_write_reg(tmp, INTAMR);

	plat_irq_setup_pins(IRQ_MODE_IRL7654_MASK);
	plat_irq_setup_pins(IRQ_MODE_IRL3210_MASK);
}
