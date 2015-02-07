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
 * Copyright (C) 2007-2009 Michal Simek <monstr@monstr.eu>
 * Copyright (C) 2007-2009 PetaLogix
 * Copyright (C) 2007 John Williams <john.williams@petalogix.com>
 * based on v850 version which was
 * Copyright (C) 2001,02,03 NEC Electronics Corporation
 * Copyright (C) 2001,02,03 Miles Bader <miles@gnu.org>
 *
 * This file is subject to the terms and conditions of the GNU General
 * Public License. See the file COPYING in the main directory of this
 * archive for more details.
 *
 */

#ifndef _ASM_MICROBLAZE_CACHEFLUSH_H
#define _ASM_MICROBLAZE_CACHEFLUSH_H

/* Somebody depends on this; sigh... */
#include <linux/mm.h>

/* Look at Documentation/cachetlb.txt */

/*
 * Cache handling functions.
 * Microblaze has a write-through data cache, meaning that the data cache
 * never needs to be flushed.  The only flushing operations that are
 * implemented are to invalidate the instruction cache.  These are called
 * after loading a user application into memory, we must invalidate the
 * instruction cache to make sure we don't fetch old, bad code.
 */

/* struct cache, d=dcache, i=icache, fl = flush, iv = invalidate,
 * suffix r = range */
struct scache {
	/* icache */
	void (*ie)(void); /* enable */
	void (*id)(void); /* disable */
	void (*ifl)(void); /* flush */
	void (*iflr)(unsigned long a, unsigned long b);
	void (*iin)(void); /* invalidate */
	void (*iinr)(unsigned long a, unsigned long b);
	/* dcache */
	void (*de)(void); /* enable */
	void (*dd)(void); /* disable */
	void (*dfl)(void); /* flush */
	void (*dflr)(unsigned long a, unsigned long b);
	void (*din)(void); /* invalidate */
	void (*dinr)(unsigned long a, unsigned long b);
};

/* microblaze cache */
extern struct scache *mbc;

void microblaze_cache_init(void);

#define enable_icache()					mbc->ie();
#define disable_icache()				mbc->id();
#define flush_icache()					mbc->ifl();
#define flush_icache_range(start, end)			mbc->iflr(start, end);
#define invalidate_icache()				mbc->iin();
#define invalidate_icache_range(start, end)		mbc->iinr(start, end);


#define flush_icache_user_range(vma, pg, adr, len)	flush_icache();
#define flush_icache_page(vma, pg)			do { } while (0)

#define enable_dcache()					mbc->de();
#define disable_dcache()				mbc->dd();
/* FIXME for LL-temac driver */
#define invalidate_dcache()				mbc->din();
#define invalidate_dcache_range(start, end)		mbc->dinr(start, end);
#define flush_dcache()					mbc->dfl();
#define flush_dcache_range(start, end)			mbc->dflr(start, end);

#define ARCH_IMPLEMENTS_FLUSH_DCACHE_PAGE 0
/* D-cache aliasing problem can't happen - cache is between MMU and ram */
#define flush_dcache_page(page)			do { } while (0)
#define flush_dcache_mmap_lock(mapping)		do { } while (0)
#define flush_dcache_mmap_unlock(mapping)	do { } while (0)


#define flush_cache_dup_mm(mm)				do { } while (0)
#define flush_cache_vmap(start, end)			do { } while (0)
#define flush_cache_vunmap(start, end)			do { } while (0)
#define flush_cache_mm(mm)			do { } while (0)
#define flush_cache_page(vma, vmaddr, pfn)	do { } while (0)

/* MS: kgdb code use this macro, wrong len with FLASH */
#if 0
#define flush_cache_range(vma, start, len)	{	\
	flush_icache_range((unsigned) (start), (unsigned) (start) + (len)); \
	flush_dcache_range((unsigned) (start), (unsigned) (start) + (len)); \
}
#endif

#define flush_cache_range(vma, start, len) do { } while (0)

#define copy_to_user_page(vma, page, vaddr, dst, src, len)		\
do {									\
	memcpy((dst), (src), (len));					\
	flush_icache_range((unsigned) (dst), (unsigned) (dst) + (len));	\
} while (0)

#define copy_from_user_page(vma, page, vaddr, dst, src, len)		\
do {									\
	memcpy((dst), (src), (len));					\
} while (0)

#endif /* _ASM_MICROBLAZE_CACHEFLUSH_H */
