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
 *  Copyright 2007 Sony Corporation
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; version 2 of the License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.
 *  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _ASM_POWERPC_EMULATED_OPS_H
#define _ASM_POWERPC_EMULATED_OPS_H

#include <asm/atomic.h>
#include <linux/perf_event.h>


#ifdef CONFIG_PPC_EMULATED_STATS

struct ppc_emulated_entry {
	const char *name;
	atomic_t val;
};

extern struct ppc_emulated {
#ifdef CONFIG_ALTIVEC
	struct ppc_emulated_entry altivec;
#endif
	struct ppc_emulated_entry dcba;
	struct ppc_emulated_entry dcbz;
	struct ppc_emulated_entry fp_pair;
	struct ppc_emulated_entry isel;
	struct ppc_emulated_entry mcrxr;
	struct ppc_emulated_entry mfpvr;
	struct ppc_emulated_entry multiple;
	struct ppc_emulated_entry popcntb;
	struct ppc_emulated_entry spe;
	struct ppc_emulated_entry string;
	struct ppc_emulated_entry unaligned;
#ifdef CONFIG_MATH_EMULATION
	struct ppc_emulated_entry math;
#elif defined(CONFIG_8XX_MINIMAL_FPEMU)
	struct ppc_emulated_entry 8xx;
#endif
#ifdef CONFIG_VSX
	struct ppc_emulated_entry vsx;
#endif
} ppc_emulated;

extern u32 ppc_warn_emulated;

extern void ppc_warn_emulated_print(const char *type);

#define __PPC_WARN_EMULATED(type)					 \
	do {								 \
		atomic_inc(&ppc_emulated.type.val);			 \
		if (ppc_warn_emulated)					 \
			ppc_warn_emulated_print(ppc_emulated.type.name); \
	} while (0)

#else /* !CONFIG_PPC_EMULATED_STATS */

#define __PPC_WARN_EMULATED(type)	do { } while (0)

#endif /* !CONFIG_PPC_EMULATED_STATS */

#define PPC_WARN_EMULATED(type, regs)					\
	do {								\
		perf_sw_event(PERF_COUNT_SW_EMULATION_FAULTS,		\
			1, 0, regs, 0);					\
		__PPC_WARN_EMULATED(type);				\
	} while (0)

#define PPC_WARN_ALIGNMENT(type, regs)					\
	do {								\
		perf_sw_event(PERF_COUNT_SW_ALIGNMENT_FAULTS,		\
			1, 0, regs, regs->dar);				\
		__PPC_WARN_EMULATED(type);				\
	} while (0)

#endif /* _ASM_POWERPC_EMULATED_OPS_H */
