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

/* cache.h:  Cache specific code for the Sparc.  These include flushing
 *           and direct tag/data line access.
 *
 * Copyright (C) 1995, 2007 David S. Miller (davem@davemloft.net)
 */

#ifndef _SPARC_CACHE_H
#define _SPARC_CACHE_H

#define ARCH_SLAB_MINALIGN	__alignof__(unsigned long long)

#define L1_CACHE_SHIFT 5
#define L1_CACHE_BYTES 32

#ifdef CONFIG_SPARC32
#define SMP_CACHE_BYTES_SHIFT 5
#else
#define SMP_CACHE_BYTES_SHIFT 6
#endif

#define SMP_CACHE_BYTES (1 << SMP_CACHE_BYTES_SHIFT)

#define __read_mostly __attribute__((__section__(".data..read_mostly")))

#ifdef CONFIG_SPARC32
#include <asm/asi.h>

/* Direct access to the instruction cache is provided through and
 * alternate address space.  The IDC bit must be off in the ICCR on
 * HyperSparcs for these accesses to work.  The code below does not do
 * any checking, the caller must do so.  These routines are for
 * diagnostics only, but could end up being useful.  Use with care.
 * Also, you are asking for trouble if you execute these in one of the
 * three instructions following a %asr/%psr access or modification.
 */

/* First, cache-tag access. */
static inline unsigned int get_icache_tag(int setnum, int tagnum)
{
	unsigned int vaddr, retval;

	vaddr = ((setnum&1) << 12) | ((tagnum&0x7f) << 5);
	__asm__ __volatile__("lda [%1] %2, %0\n\t" :
			     "=r" (retval) :
			     "r" (vaddr), "i" (ASI_M_TXTC_TAG));
	return retval;
}

static inline void put_icache_tag(int setnum, int tagnum, unsigned int entry)
{
	unsigned int vaddr;

	vaddr = ((setnum&1) << 12) | ((tagnum&0x7f) << 5);
	__asm__ __volatile__("sta %0, [%1] %2\n\t" : :
			     "r" (entry), "r" (vaddr), "i" (ASI_M_TXTC_TAG) :
			     "memory");
}

/* Second cache-data access.  The data is returned two-32bit quantities
 * at a time.
 */
static inline void get_icache_data(int setnum, int tagnum, int subblock,
				       unsigned int *data)
{
	unsigned int value1, value2, vaddr;

	vaddr = ((setnum&0x1) << 12) | ((tagnum&0x7f) << 5) |
		((subblock&0x3) << 3);
	__asm__ __volatile__("ldda [%2] %3, %%g2\n\t"
			     "or %%g0, %%g2, %0\n\t"
			     "or %%g0, %%g3, %1\n\t" :
			     "=r" (value1), "=r" (value2) :
			     "r" (vaddr), "i" (ASI_M_TXTC_DATA) :
			     "g2", "g3");
	data[0] = value1; data[1] = value2;
}

static inline void put_icache_data(int setnum, int tagnum, int subblock,
				       unsigned int *data)
{
	unsigned int value1, value2, vaddr;

	vaddr = ((setnum&0x1) << 12) | ((tagnum&0x7f) << 5) |
		((subblock&0x3) << 3);
	value1 = data[0]; value2 = data[1];
	__asm__ __volatile__("or %%g0, %0, %%g2\n\t"
			     "or %%g0, %1, %%g3\n\t"
			     "stda %%g2, [%2] %3\n\t" : :
			     "r" (value1), "r" (value2), 
			     "r" (vaddr), "i" (ASI_M_TXTC_DATA) :
			     "g2", "g3", "memory" /* no joke */);
}

/* Different types of flushes with the ICACHE.  Some of the flushes
 * affect both the ICACHE and the external cache.  Others only clear
 * the ICACHE entries on the cpu itself.  V8's (most) allow
 * granularity of flushes on the packet (element in line), whole line,
 * and entire cache (ie. all lines) level.  The ICACHE only flushes are
 * ROSS HyperSparc specific and are in ross.h
 */

/* Flushes which clear out both the on-chip and external caches */
static inline void flush_ei_page(unsigned int addr)
{
	__asm__ __volatile__("sta %%g0, [%0] %1\n\t" : :
			     "r" (addr), "i" (ASI_M_FLUSH_PAGE) :
			     "memory");
}

static inline void flush_ei_seg(unsigned int addr)
{
	__asm__ __volatile__("sta %%g0, [%0] %1\n\t" : :
			     "r" (addr), "i" (ASI_M_FLUSH_SEG) :
			     "memory");
}

static inline void flush_ei_region(unsigned int addr)
{
	__asm__ __volatile__("sta %%g0, [%0] %1\n\t" : :
			     "r" (addr), "i" (ASI_M_FLUSH_REGION) :
			     "memory");
}

static inline void flush_ei_ctx(unsigned int addr)
{
	__asm__ __volatile__("sta %%g0, [%0] %1\n\t" : :
			     "r" (addr), "i" (ASI_M_FLUSH_CTX) :
			     "memory");
}

static inline void flush_ei_user(unsigned int addr)
{
	__asm__ __volatile__("sta %%g0, [%0] %1\n\t" : :
			     "r" (addr), "i" (ASI_M_FLUSH_USER) :
			     "memory");
}
#endif /* CONFIG_SPARC32 */

#endif /* !(_SPARC_CACHE_H) */
