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

#ifndef __MACH_CT_CA9X4_H
#define __MACH_CT_CA9X4_H

/*
 * Physical base addresses
 */
#define CT_CA9X4_CLCDC		(0x10020000)
#define CT_CA9X4_AXIRAM		(0x10060000)
#define CT_CA9X4_DMC		(0x100e0000)
#define CT_CA9X4_SMC		(0x100e1000)
#define CT_CA9X4_SCC		(0x100e2000)
#define CT_CA9X4_SP804_TIMER	(0x100e4000)
#define CT_CA9X4_SP805_WDT	(0x100e5000)
#define CT_CA9X4_TZPC		(0x100e6000)
#define CT_CA9X4_GPIO		(0x100e8000)
#define CT_CA9X4_FASTAXI	(0x100e9000)
#define CT_CA9X4_SLOWAXI	(0x100ea000)
#define CT_CA9X4_TZASC		(0x100ec000)
#define CT_CA9X4_CORESIGHT	(0x10200000)
#define CT_CA9X4_MPIC		(0x1e000000)
#define CT_CA9X4_SYSTIMER	(0x1e004000)
#define CT_CA9X4_SYSWDT		(0x1e007000)
#define CT_CA9X4_L2CC		(0x1e00a000)

#define CT_CA9X4_TIMER0		(CT_CA9X4_SP804_TIMER + 0x000)
#define CT_CA9X4_TIMER1		(CT_CA9X4_SP804_TIMER + 0x020)

#define A9_MPCORE_SCU		(CT_CA9X4_MPIC + 0x0000)
#define A9_MPCORE_GIC_CPU	(CT_CA9X4_MPIC + 0x0100)
#define A9_MPCORE_GIT		(CT_CA9X4_MPIC + 0x0200)
#define A9_MPCORE_GIC_DIST	(CT_CA9X4_MPIC + 0x1000)

/*
 * Interrupts.  Those in {} are for AMBA devices
 */
#define IRQ_CT_CA9X4_CLCDC	{ 76 }
#define IRQ_CT_CA9X4_DMC	{ -1 }
#define IRQ_CT_CA9X4_SMC	{ 77, 78 }
#define IRQ_CT_CA9X4_TIMER0	80
#define IRQ_CT_CA9X4_TIMER1	81
#define IRQ_CT_CA9X4_GPIO	{ 82 }
#define IRQ_CT_CA9X4_PMU_CPU0	92
#define IRQ_CT_CA9X4_PMU_CPU1	93
#define IRQ_CT_CA9X4_PMU_CPU2	94
#define IRQ_CT_CA9X4_PMU_CPU3	95

#endif