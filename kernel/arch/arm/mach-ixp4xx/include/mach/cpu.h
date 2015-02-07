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
 * arch/arm/mach-ixp4xx/include/mach/cpu.h
 *
 * IXP4XX cpu type detection
 *
 * Copyright (C) 2007 MontaVista Software, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#ifndef __ASM_ARCH_CPU_H__
#define __ASM_ARCH_CPU_H__

#include <asm/cputype.h>

/* Processor id value in CP15 Register 0 */
#define IXP42X_PROCESSOR_ID_VALUE	0x690541c0 /* including unused 0x690541Ex */
#define IXP42X_PROCESSOR_ID_MASK	0xffffffc0

#define IXP43X_PROCESSOR_ID_VALUE	0x69054040
#define IXP43X_PROCESSOR_ID_MASK	0xfffffff0

#define IXP46X_PROCESSOR_ID_VALUE	0x69054200 /* including IXP455 */
#define IXP46X_PROCESSOR_ID_MASK	0xfffffff0

#define cpu_is_ixp42x_rev_a0() ((read_cpuid_id() & (IXP42X_PROCESSOR_ID_MASK | 0xF)) == \
				IXP42X_PROCESSOR_ID_VALUE)
#define cpu_is_ixp42x()	((read_cpuid_id() & IXP42X_PROCESSOR_ID_MASK) == \
			 IXP42X_PROCESSOR_ID_VALUE)
#define cpu_is_ixp43x()	((read_cpuid_id() & IXP43X_PROCESSOR_ID_MASK) == \
			 IXP43X_PROCESSOR_ID_VALUE)
#define cpu_is_ixp46x()	((read_cpuid_id() & IXP46X_PROCESSOR_ID_MASK) == \
			 IXP46X_PROCESSOR_ID_VALUE)

static inline u32 ixp4xx_read_feature_bits(void)
{
	u32 val = ~*IXP4XX_EXP_CFG2;

	if (cpu_is_ixp42x_rev_a0())
		return IXP42X_FEATURE_MASK & ~(IXP4XX_FEATURE_RCOMP |
					       IXP4XX_FEATURE_AES);
	if (cpu_is_ixp42x())
		return val & IXP42X_FEATURE_MASK;
	if (cpu_is_ixp43x())
		return val & IXP43X_FEATURE_MASK;
	return val & IXP46X_FEATURE_MASK;
}

static inline void ixp4xx_write_feature_bits(u32 value)
{
	*IXP4XX_EXP_CFG2 = ~value;
}

#endif  /* _ASM_ARCH_CPU_H */
