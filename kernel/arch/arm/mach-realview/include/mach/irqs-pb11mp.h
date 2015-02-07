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
 * arch/arm/mach-realview/include/mach/irqs-pb11mp.h
 *
 * Copyright (C) 2008 ARM Limited
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */

#ifndef __MACH_IRQS_PB11MP_H
#define __MACH_IRQS_PB11MP_H

#define IRQ_TC11MP_GIC_START			32
#define IRQ_PB11MP_GIC_START			64

/*
 * ARM11MPCore test chip interrupt sources (primary GIC on the test chip)
 */
#define IRQ_TC11MP_AACI		(IRQ_TC11MP_GIC_START + 0)
#define IRQ_TC11MP_TIMER0_1	(IRQ_TC11MP_GIC_START + 1)
#define IRQ_TC11MP_TIMER2_3	(IRQ_TC11MP_GIC_START + 2)
#define IRQ_TC11MP_USB		(IRQ_TC11MP_GIC_START + 3)
#define IRQ_TC11MP_UART0	(IRQ_TC11MP_GIC_START + 4)
#define IRQ_TC11MP_UART1	(IRQ_TC11MP_GIC_START + 5)
#define IRQ_TC11MP_RTC		(IRQ_TC11MP_GIC_START + 6)
#define IRQ_TC11MP_KMI0		(IRQ_TC11MP_GIC_START + 7)
#define IRQ_TC11MP_KMI1		(IRQ_TC11MP_GIC_START + 8)
#define IRQ_TC11MP_ETH		(IRQ_TC11MP_GIC_START + 9)
#define IRQ_TC11MP_PB_IRQ1	(IRQ_TC11MP_GIC_START + 10)		/* main GIC */
#define IRQ_TC11MP_PB_IRQ2	(IRQ_TC11MP_GIC_START + 11)		/* tile GIC */
#define IRQ_TC11MP_PB_FIQ1	(IRQ_TC11MP_GIC_START + 12)		/* main GIC */
#define IRQ_TC11MP_PB_FIQ2	(IRQ_TC11MP_GIC_START + 13)		/* tile GIC */
#define IRQ_TC11MP_MMCI0A	(IRQ_TC11MP_GIC_START + 14)
#define IRQ_TC11MP_MMCI0B	(IRQ_TC11MP_GIC_START + 15)

#define IRQ_TC11MP_PMU_CPU0	(IRQ_TC11MP_GIC_START + 17)
#define IRQ_TC11MP_PMU_CPU1	(IRQ_TC11MP_GIC_START + 18)
#define IRQ_TC11MP_PMU_CPU2	(IRQ_TC11MP_GIC_START + 19)
#define IRQ_TC11MP_PMU_CPU3	(IRQ_TC11MP_GIC_START + 20)
#define IRQ_TC11MP_PMU_SCU0	(IRQ_TC11MP_GIC_START + 21)
#define IRQ_TC11MP_PMU_SCU1	(IRQ_TC11MP_GIC_START + 22)
#define IRQ_TC11MP_PMU_SCU2	(IRQ_TC11MP_GIC_START + 23)
#define IRQ_TC11MP_PMU_SCU3	(IRQ_TC11MP_GIC_START + 24)
#define IRQ_TC11MP_PMU_SCU4	(IRQ_TC11MP_GIC_START + 25)
#define IRQ_TC11MP_PMU_SCU5	(IRQ_TC11MP_GIC_START + 26)
#define IRQ_TC11MP_PMU_SCU6	(IRQ_TC11MP_GIC_START + 27)
#define IRQ_TC11MP_PMU_SCU7	(IRQ_TC11MP_GIC_START + 28)

#define IRQ_TC11MP_L220_EVENT	(IRQ_TC11MP_GIC_START + 29)
#define IRQ_TC11MP_L220_SLAVE	(IRQ_TC11MP_GIC_START + 30)
#define IRQ_TC11MP_L220_DECODE	(IRQ_TC11MP_GIC_START + 31)

/*
 * RealView PB11MPCore GIC interrupt sources (secondary GIC on the board)
 */
#define IRQ_PB11MP_WATCHDOG	(IRQ_PB11MP_GIC_START + 0)	/* Watchdog timer */
#define IRQ_PB11MP_SOFT		(IRQ_PB11MP_GIC_START + 1)	/* Software interrupt */
#define IRQ_PB11MP_COMMRx	(IRQ_PB11MP_GIC_START + 2)	/* Debug Comm Rx interrupt */
#define IRQ_PB11MP_COMMTx	(IRQ_PB11MP_GIC_START + 3)	/* Debug Comm Tx interrupt */
#define IRQ_PB11MP_GPIO0	(IRQ_PB11MP_GIC_START + 6)	/* GPIO 0 */
#define IRQ_PB11MP_GPIO1	(IRQ_PB11MP_GIC_START + 7)	/* GPIO 1 */
#define IRQ_PB11MP_GPIO2	(IRQ_PB11MP_GIC_START + 8)	/* GPIO 2 */
								/* 9 reserved */
#define IRQ_PB11MP_RTC_GIC1	(IRQ_PB11MP_GIC_START + 10)	/* Real Time Clock */
#define IRQ_PB11MP_SSP		(IRQ_PB11MP_GIC_START + 11)	/* Synchronous Serial Port */
#define IRQ_PB11MP_UART0_GIC1	(IRQ_PB11MP_GIC_START + 12)	/* UART 0 on development chip */
#define IRQ_PB11MP_UART1_GIC1	(IRQ_PB11MP_GIC_START + 13)	/* UART 1 on development chip */
#define IRQ_PB11MP_UART2	(IRQ_PB11MP_GIC_START + 14)	/* UART 2 on development chip */
#define IRQ_PB11MP_UART3	(IRQ_PB11MP_GIC_START + 15)	/* UART 3 on development chip */
#define IRQ_PB11MP_SCI		(IRQ_PB11MP_GIC_START + 16)	/* Smart Card Interface */
#define IRQ_PB11MP_MMCI0A_GIC1	(IRQ_PB11MP_GIC_START + 17)	/* Multimedia Card 0A */
#define IRQ_PB11MP_MMCI0B_GIC1	(IRQ_PB11MP_GIC_START + 18)	/* Multimedia Card 0B */
#define IRQ_PB11MP_AACI_GIC1	(IRQ_PB11MP_GIC_START + 19)	/* Audio Codec */
#define IRQ_PB11MP_KMI0_GIC1	(IRQ_PB11MP_GIC_START + 20)	/* Keyboard/Mouse port 0 */
#define IRQ_PB11MP_KMI1_GIC1	(IRQ_PB11MP_GIC_START + 21)	/* Keyboard/Mouse port 1 */
#define IRQ_PB11MP_CHARLCD	(IRQ_PB11MP_GIC_START + 22)	/* Character LCD */
#define IRQ_PB11MP_CLCD		(IRQ_PB11MP_GIC_START + 23)	/* CLCD controller */
#define IRQ_PB11MP_DMAC		(IRQ_PB11MP_GIC_START + 24)	/* DMA controller */
#define IRQ_PB11MP_PWRFAIL	(IRQ_PB11MP_GIC_START + 25)	/* Power failure */
#define IRQ_PB11MP_PISMO	(IRQ_PB11MP_GIC_START + 26)	/* PISMO interface */
#define IRQ_PB11MP_DoC		(IRQ_PB11MP_GIC_START + 27)	/* Disk on Chip memory controller */
#define IRQ_PB11MP_ETH_GIC1	(IRQ_PB11MP_GIC_START + 28)	/* Ethernet controller */
#define IRQ_PB11MP_USB_GIC1	(IRQ_PB11MP_GIC_START + 29)	/* USB controller */
#define IRQ_PB11MP_TSPEN	(IRQ_PB11MP_GIC_START + 30)	/* Touchscreen pen */
#define IRQ_PB11MP_TSKPAD	(IRQ_PB11MP_GIC_START + 31)	/* Touchscreen keypad */

#define IRQ_PB11MP_SMC		-1
#define IRQ_PB11MP_SCTL		-1

#define NR_GIC_PB11MP		2

/*
 * Only define NR_IRQS if less than NR_IRQS_PB11MP
 */
#define NR_IRQS_PB11MP		(IRQ_TC11MP_GIC_START + 96)

#if defined(CONFIG_MACH_REALVIEW_PB11MP)

#if !defined(NR_IRQS) || (NR_IRQS < NR_IRQS_PB11MP)
#undef NR_IRQS
#define NR_IRQS			NR_IRQS_PB11MP
#endif

#if !defined(MAX_GIC_NR) || (MAX_GIC_NR < NR_GIC_PB11MP)
#undef MAX_GIC_NR
#define MAX_GIC_NR		NR_GIC_PB11MP
#endif

#endif	/* CONFIG_MACH_REALVIEW_PB11MP */

#endif	/* __MACH_IRQS_PB11MP_H */
