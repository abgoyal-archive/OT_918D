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

/* cache-page.c: whole-page cache wrangling functions for MMU linux
 *
 * Copyright (C) 2004 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */
#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/highmem.h>
#include <linux/module.h>
#include <asm/pgalloc.h>

/*****************************************************************************/
/*
 * DCF takes a virtual address and the page may not currently have one
 * - temporarily hijack a kmap_atomic() slot and attach the page to it
 */
void flush_dcache_page(struct page *page)
{
	unsigned long dampr2;
	void *vaddr;

	dampr2 = __get_DAMPR(2);

	vaddr = kmap_atomic(page, __KM_CACHE);

	frv_dcache_writeback((unsigned long) vaddr, (unsigned long) vaddr + PAGE_SIZE);

	kunmap_atomic(vaddr, __KM_CACHE);

	if (dampr2) {
		__set_DAMPR(2, dampr2);
		__set_IAMPR(2, dampr2);
	}

} /* end flush_dcache_page() */

EXPORT_SYMBOL(flush_dcache_page);

/*****************************************************************************/
/*
 * ICI takes a virtual address and the page may not currently have one
 * - so we temporarily attach the page to a bit of virtual space so that is can be flushed
 */
void flush_icache_user_range(struct vm_area_struct *vma, struct page *page,
			     unsigned long start, unsigned long len)
{
	unsigned long dampr2;
	void *vaddr;

	dampr2 = __get_DAMPR(2);

	vaddr = kmap_atomic(page, __KM_CACHE);

	start = (start & ~PAGE_MASK) | (unsigned long) vaddr;
	frv_cache_wback_inv(start, start + len);

	kunmap_atomic(vaddr, __KM_CACHE);

	if (dampr2) {
		__set_DAMPR(2, dampr2);
		__set_IAMPR(2, dampr2);
	}

} /* end flush_icache_user_range() */

EXPORT_SYMBOL(flush_icache_user_range);
