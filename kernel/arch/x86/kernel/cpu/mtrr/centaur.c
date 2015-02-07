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

#include <linux/init.h>
#include <linux/mm.h>

#include <asm/mtrr.h>
#include <asm/msr.h>

#include "mtrr.h"

static struct {
	unsigned long high;
	unsigned long low;
} centaur_mcr[8];

static u8 centaur_mcr_reserved;
static u8 centaur_mcr_type;	/* 0 for winchip, 1 for winchip2 */

/**
 * centaur_get_free_region - Get a free MTRR.
 *
 * @base: The starting (base) address of the region.
 * @size: The size (in bytes) of the region.
 *
 * Returns: the index of the region on success, else -1 on error.
 */
static int
centaur_get_free_region(unsigned long base, unsigned long size, int replace_reg)
{
	unsigned long lbase, lsize;
	mtrr_type ltype;
	int i, max;

	max = num_var_ranges;
	if (replace_reg >= 0 && replace_reg < max)
		return replace_reg;

	for (i = 0; i < max; ++i) {
		if (centaur_mcr_reserved & (1 << i))
			continue;
		mtrr_if->get(i, &lbase, &lsize, &ltype);
		if (lsize == 0)
			return i;
	}

	return -ENOSPC;
}

/*
 * Report boot time MCR setups
 */
void mtrr_centaur_report_mcr(int mcr, u32 lo, u32 hi)
{
	centaur_mcr[mcr].low = lo;
	centaur_mcr[mcr].high = hi;
}

static void
centaur_get_mcr(unsigned int reg, unsigned long *base,
		unsigned long *size, mtrr_type * type)
{
	*base = centaur_mcr[reg].high >> PAGE_SHIFT;
	*size = -(centaur_mcr[reg].low & 0xfffff000) >> PAGE_SHIFT;
	*type = MTRR_TYPE_WRCOMB;		/* write-combining  */

	if (centaur_mcr_type == 1 && ((centaur_mcr[reg].low & 31) & 2))
		*type = MTRR_TYPE_UNCACHABLE;
	if (centaur_mcr_type == 1 && (centaur_mcr[reg].low & 31) == 25)
		*type = MTRR_TYPE_WRBACK;
	if (centaur_mcr_type == 0 && (centaur_mcr[reg].low & 31) == 31)
		*type = MTRR_TYPE_WRBACK;
}

static void
centaur_set_mcr(unsigned int reg, unsigned long base,
		unsigned long size, mtrr_type type)
{
	unsigned long low, high;

	if (size == 0) {
		/* Disable */
		high = low = 0;
	} else {
		high = base << PAGE_SHIFT;
		if (centaur_mcr_type == 0) {
			/* Only support write-combining... */
			low = -size << PAGE_SHIFT | 0x1f;
		} else {
			if (type == MTRR_TYPE_UNCACHABLE)
				low = -size << PAGE_SHIFT | 0x02; /* NC */
			else
				low = -size << PAGE_SHIFT | 0x09; /* WWO, WC */
		}
	}
	centaur_mcr[reg].high = high;
	centaur_mcr[reg].low = low;
	wrmsr(MSR_IDT_MCR0 + reg, low, high);
}

static int
centaur_validate_add_page(unsigned long base, unsigned long size, unsigned int type)
{
	/*
	 * FIXME: Winchip2 supports uncached
	 */
	if (type != MTRR_TYPE_WRCOMB &&
	    (centaur_mcr_type == 0 || type != MTRR_TYPE_UNCACHABLE)) {
		pr_warning("mtrr: only write-combining%s supported\n",
			   centaur_mcr_type ? " and uncacheable are" : " is");
		return -EINVAL;
	}
	return 0;
}

static const struct mtrr_ops centaur_mtrr_ops = {
	.vendor            = X86_VENDOR_CENTAUR,
	.set               = centaur_set_mcr,
	.get               = centaur_get_mcr,
	.get_free_region   = centaur_get_free_region,
	.validate_add_page = centaur_validate_add_page,
	.have_wrcomb       = positive_have_wrcomb,
};

int __init centaur_init_mtrr(void)
{
	set_mtrr_ops(&centaur_mtrr_ops);
	return 0;
}
