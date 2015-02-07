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
 * Copyright 2008 Freescale Semiconductor, Inc. All Rights Reserved.
 * Copyright 2008 Embedded Alley Solutions, Inc All Rights Reserved.
 */

/*
 * The code contained herein is licensed under the GNU General Public
 * License. You may obtain a copy of the GNU General Public License
 * Version 2 or later at the following locations:
 *
 * http://www.opensource.org/licenses/gpl-license.html
 * http://www.gnu.org/copyleft/gpl.html
 */
#ifndef __ASM_PLAT_PLATFORM_H
#define __ASM_PLAT_PLATFORM_H

#ifndef __ASSEMBLER__
#include <linux/io.h>
#endif
#include <asm/sizes.h>

/* Virtual address where registers are mapped */
#define STMP3XXX_REGS_PHBASE	0x80000000
#ifdef __ASSEMBLER__
#define STMP3XXX_REGS_BASE	0xF0000000
#else
#define STMP3XXX_REGS_BASE	(void __iomem *)0xF0000000
#endif
#define STMP3XXX_REGS_SIZE	SZ_1M

/* Virtual address where OCRAM is mapped */
#define STMP3XXX_OCRAM_PHBASE	0x00000000
#ifdef __ASSEMBLER__
#define STMP3XXX_OCRAM_BASE	0xf1000000
#else
#define STMP3XXX_OCRAM_BASE	(void __iomem *)0xf1000000
#endif
#define STMP3XXX_OCRAM_SIZE	(32 * SZ_1K)

#ifdef CONFIG_ARCH_STMP37XX
#define IRQ_PRIORITY_REG_RD	HW_ICOLL_PRIORITYn_RD
#define IRQ_PRIORITY_REG_WR	HW_ICOLL_PRIORITYn_WR
#endif

#ifdef CONFIG_ARCH_STMP378X
#define IRQ_PRIORITY_REG_RD	HW_ICOLL_INTERRUPTn_RD
#define IRQ_PRIORITY_REG_WR	HW_ICOLL_INTERRUPTn_WR
#endif

#define HW_STMP3XXX_SET		0x04
#define HW_STMP3XXX_CLR		0x08
#define HW_STMP3XXX_TOG		0x0c

#ifndef __ASSEMBLER__
static inline void stmp3xxx_clearl(u32 v, void __iomem *r)
{
	__raw_writel(v, r + HW_STMP3XXX_CLR);
}

static inline void stmp3xxx_setl(u32 v, void __iomem *r)
{
	__raw_writel(v, r + HW_STMP3XXX_SET);
}
#endif

#define BF(value, field) (((value) << BP_##field) & BM_##field)

#endif /* __ASM_ARCH_PLATFORM_H */
