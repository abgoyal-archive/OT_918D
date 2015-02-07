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
 * arch/arm/mach-nuc93x/include/mach/regs-clock.h
 *
 * Copyright (c) 2008 Nuvoton technology corporation.
 *
 * Wan ZongShun <mcuos.com@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation;version 2 of the License.
 *
 */

#ifndef __ASM_ARCH_REGS_CLOCK_H
#define __ASM_ARCH_REGS_CLOCK_H

/* Clock Control Registers  */
#define CLK_BA		NUC93X_VA_CLKPWR
#define REG_CLKEN	(CLK_BA + 0x00)
#define REG_CLKSEL	(CLK_BA + 0x04)
#define REG_CLKDIV	(CLK_BA + 0x08)
#define REG_PLLCON0	(CLK_BA + 0x0C)
#define REG_PLLCON1	(CLK_BA + 0x10)
#define REG_PMCON	(CLK_BA + 0x14)
#define REG_IRQWAKECON	(CLK_BA + 0x18)
#define REG_IRQWAKEFLAG	(CLK_BA + 0x1C)
#define REG_IPSRST	(CLK_BA + 0x20)
#define REG_CLKEN1	(CLK_BA + 0x24)
#define REG_CLKDIV1	(CLK_BA + 0x28)

/* Define PLL freq setting */
#define PLL_DISABLE		0x12B63
#define	PLL_66MHZ		0x2B63
#define	PLL_100MHZ		0x4F64
#define PLL_120MHZ		0x4F63
#define	PLL_166MHZ		0x4124
#define	PLL_200MHZ		0x4F24

/* Define AHB:CPUFREQ ratio */
#define	AHB_CPUCLK_1_1		0x00
#define	AHB_CPUCLK_1_2		0x01
#define	AHB_CPUCLK_1_4		0x02
#define	AHB_CPUCLK_1_8		0x03

/* Define APB:AHB ratio */
#define APB_AHB_1_2		0x01
#define APB_AHB_1_4		0x02
#define APB_AHB_1_8		0x03

/* Define clock skew */
#define DEFAULTSKEW		0x48

#endif /*  __ASM_ARCH_REGS_CLOCK_H */
