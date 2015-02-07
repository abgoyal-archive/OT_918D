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
 * linux/arch/arm/mach-mmp/include/mach/regs-icu.h
 *
 *   Interrupt Control Unit
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef __ASM_MACH_ICU_H
#define __ASM_MACH_ICU_H

#include <mach/addr-map.h>

#define ICU_VIRT_BASE	(AXI_VIRT_BASE + 0x82000)
#define ICU_REG(x)	(ICU_VIRT_BASE + (x))

#define ICU_INT_CONF(n)		ICU_REG((n) << 2)
#define ICU_INT_CONF_MASK	(0xf)

/************ PXA168/PXA910 (MMP) *********************/
#define ICU_INT_CONF_AP_INT	(1 << 6)
#define ICU_INT_CONF_CP_INT	(1 << 5)
#define ICU_INT_CONF_IRQ	(1 << 4)

#define ICU_AP_FIQ_SEL_INT_NUM	ICU_REG(0x108)	/* AP FIQ Selected Interrupt */
#define ICU_AP_IRQ_SEL_INT_NUM	ICU_REG(0x10C)	/* AP IRQ Selected Interrupt */
#define ICU_AP_GBL_IRQ_MSK	ICU_REG(0x114)	/* AP Global Interrupt Mask */
#define ICU_INT_STATUS_0	ICU_REG(0x128)	/* Interrupt Stuats 0 */
#define ICU_INT_STATUS_1	ICU_REG(0x12C)	/* Interrupt Status 1 */

/************************** MMP2 ***********************/

/*
 * IRQ0/FIQ0 is routed to SP IRQ/FIQ.
 * IRQ1 is routed to PJ4 IRQ, and IRQ2 is routes to PJ4 FIQ.
 */
#define ICU_INT_ROUTE_SP_IRQ		(1 << 4)
#define ICU_INT_ROUTE_PJ4_IRQ		(1 << 5)
#define ICU_INT_ROUTE_PJ4_FIQ		(1 << 6)

#define MMP2_ICU_PJ4_IRQ_STATUS0	ICU_REG(0x138)
#define MMP2_ICU_PJ4_IRQ_STATUS1	ICU_REG(0x13c)
#define MMP2_ICU_PJ4_FIQ_STATUS0	ICU_REG(0x140)
#define MMP2_ICU_PJ4_FIQ_STATUS1	ICU_REG(0x144)

#define MMP2_ICU_INT4_STATUS		ICU_REG(0x150)
#define MMP2_ICU_INT5_STATUS		ICU_REG(0x154)
#define MMP2_ICU_INT17_STATUS		ICU_REG(0x158)
#define MMP2_ICU_INT35_STATUS		ICU_REG(0x15c)
#define MMP2_ICU_INT51_STATUS		ICU_REG(0x160)

#define MMP2_ICU_INT4_MASK		ICU_REG(0x168)
#define MMP2_ICU_INT5_MASK		ICU_REG(0x16C)
#define MMP2_ICU_INT17_MASK		ICU_REG(0x170)
#define MMP2_ICU_INT35_MASK		ICU_REG(0x174)
#define MMP2_ICU_INT51_MASK		ICU_REG(0x178)

#define MMP2_ICU_SP_IRQ_SEL		ICU_REG(0x100)
#define MMP2_ICU_PJ4_IRQ_SEL		ICU_REG(0x104)
#define MMP2_ICU_PJ4_FIQ_SEL		ICU_REG(0x108)

#define MMP2_ICU_INVERT			ICU_REG(0x164)

#define MMP2_ICU_INV_PMIC		(1 << 0)
#define MMP2_ICU_INV_PERF		(1 << 1)
#define MMP2_ICU_INV_COMMTX		(1 << 2)
#define MMP2_ICU_INV_COMMRX		(1 << 3)

#endif /* __ASM_MACH_ICU_H */
