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
 * Generic support for queying CPU info
 *
 * Copyright (C) 2007-2009 Michal Simek <monstr@monstr.eu>
 * Copyright (C) 2007-2009 PetaLogix
 * Copyright (C) 2007 John Williams <jwilliams@itee.uq.edu.au>
 *
 * This file is subject to the terms and conditions of the GNU General
 * Public License. See the file COPYING in the main directory of this
 * archive for more details.
 */

#ifndef _ASM_MICROBLAZE_CPUINFO_H
#define _ASM_MICROBLAZE_CPUINFO_H

#include <asm/prom.h>

/* CPU Version and FPGA Family code conversion table type */
struct cpu_ver_key {
	const char *s;
	const unsigned k;
};

extern const struct cpu_ver_key cpu_ver_lookup[];

struct family_string_key {
	const char *s;
	const unsigned k;
};

extern const struct family_string_key family_string_lookup[];

struct cpuinfo {
	/* Core CPU configuration */
	u32 use_instr;
	u32 use_mult;
	u32 use_fpu;
	u32 use_exc;
	u32 ver_code;
	u32 mmu;

	/* CPU caches */
	u32 use_icache;
	u32 icache_tagbits;
	u32 icache_write;
	u32 icache_line_length;
	u32 icache_size;
	unsigned long icache_base;
	unsigned long icache_high;

	u32 use_dcache;
	u32 dcache_tagbits;
	u32 dcache_write;
	u32 dcache_line_length;
	u32 dcache_size;
	u32 dcache_wb;
	unsigned long dcache_base;
	unsigned long dcache_high;

	/* Bus connections */
	u32 use_dopb;
	u32 use_iopb;
	u32 use_dlmb;
	u32 use_ilmb;
	u32 num_fsl;

	/* CPU interrupt line info */
	u32 irq_edge;
	u32 irq_positive;

	u32 area_optimised;

	/* HW debug support */
	u32 hw_debug;
	u32 num_pc_brk;
	u32 num_rd_brk;
	u32 num_wr_brk;
	u32 cpu_clock_freq; /* store real freq of cpu */
	u32 freq_div_hz; /* store freq/HZ */

	/* FPGA family */
	u32 fpga_family_code;

	/* User define */
	u32 pvr_user1;
	u32 pvr_user2;
};

extern struct cpuinfo cpuinfo;

/* fwd declarations of the various CPUinfo populators */
void setup_cpuinfo(void);

void set_cpuinfo_static(struct cpuinfo *ci, struct device_node *cpu);
void set_cpuinfo_pvr_full(struct cpuinfo *ci, struct device_node *cpu);

static inline unsigned int fcpu(struct device_node *cpu, char *n)
{
	int *val;
	return (val = (int *) of_get_property(cpu, n, NULL)) ? *val : 0;
}

#endif /* _ASM_MICROBLAZE_CPUINFO_H */
