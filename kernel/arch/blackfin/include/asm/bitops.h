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
 * Copyright 2004-2009 Analog Devices Inc.
 *
 * Licensed under the GPL-2 or later.
 */

#ifndef _BLACKFIN_BITOPS_H
#define _BLACKFIN_BITOPS_H

#include <linux/compiler.h>

#include <asm-generic/bitops/__ffs.h>
#include <asm-generic/bitops/ffz.h>
#include <asm-generic/bitops/fls.h>
#include <asm-generic/bitops/__fls.h>
#include <asm-generic/bitops/fls64.h>
#include <asm-generic/bitops/find.h>

#ifndef _LINUX_BITOPS_H
#error only <linux/bitops.h> can be included directly
#endif

#include <asm-generic/bitops/sched.h>
#include <asm-generic/bitops/ffs.h>
#include <asm-generic/bitops/lock.h>
#include <asm-generic/bitops/ext2-non-atomic.h>
#include <asm-generic/bitops/ext2-atomic.h>
#include <asm-generic/bitops/minix.h>

#ifndef CONFIG_SMP
#include <linux/irqflags.h>

/*
 * clear_bit may not imply a memory barrier
 */
#ifndef smp_mb__before_clear_bit
#define smp_mb__before_clear_bit()	smp_mb()
#define smp_mb__after_clear_bit()	smp_mb()
#endif
#include <asm-generic/bitops/atomic.h>
#include <asm-generic/bitops/non-atomic.h>
#else

#include <asm/byteorder.h>	/* swab32 */
#include <linux/linkage.h>

asmlinkage int __raw_bit_set_asm(volatile unsigned long *addr, int nr);

asmlinkage int __raw_bit_clear_asm(volatile unsigned long *addr, int nr);

asmlinkage int __raw_bit_toggle_asm(volatile unsigned long *addr, int nr);

asmlinkage int __raw_bit_test_set_asm(volatile unsigned long *addr, int nr);

asmlinkage int __raw_bit_test_clear_asm(volatile unsigned long *addr, int nr);

asmlinkage int __raw_bit_test_toggle_asm(volatile unsigned long *addr, int nr);

asmlinkage int __raw_bit_test_asm(const volatile unsigned long *addr, int nr);

static inline void set_bit(int nr, volatile unsigned long *addr)
{
	volatile unsigned long *a = addr + (nr >> 5);
	__raw_bit_set_asm(a, nr & 0x1f);
}

static inline void clear_bit(int nr, volatile unsigned long *addr)
{
	volatile unsigned long *a = addr + (nr >> 5);
	__raw_bit_clear_asm(a, nr & 0x1f);
}

static inline void change_bit(int nr, volatile unsigned long *addr)
{
	volatile unsigned long *a = addr + (nr >> 5);
	__raw_bit_toggle_asm(a, nr & 0x1f);
}

static inline int test_bit(int nr, const volatile unsigned long *addr)
{
	volatile const unsigned long *a = addr + (nr >> 5);
	return __raw_bit_test_asm(a, nr & 0x1f) != 0;
}

static inline int test_and_set_bit(int nr, volatile unsigned long *addr)
{
	volatile unsigned long *a = addr + (nr >> 5);
	return __raw_bit_test_set_asm(a, nr & 0x1f);
}

static inline int test_and_clear_bit(int nr, volatile unsigned long *addr)
{
	volatile unsigned long *a = addr + (nr >> 5);
	return __raw_bit_test_clear_asm(a, nr & 0x1f);
}

static inline int test_and_change_bit(int nr, volatile unsigned long *addr)
{
	volatile unsigned long *a = addr + (nr >> 5);
	return __raw_bit_test_toggle_asm(a, nr & 0x1f);
}

/*
 * clear_bit() doesn't provide any barrier for the compiler.
 */
#define smp_mb__before_clear_bit()	barrier()
#define smp_mb__after_clear_bit()	barrier()

#include <asm-generic/bitops/non-atomic.h>

#endif /* CONFIG_SMP */

/*
 * hweightN: returns the hamming weight (i.e. the number
 * of bits set) of a N-bit word
 */

static inline unsigned int hweight32(unsigned int w)
{
	unsigned int res;

	__asm__ ("%0.l = ONES %0;"
		"%0 = %0.l (Z);"
		: "=d" (res) : "d" (w));
	return res;
}

static inline unsigned int hweight64(__u64 w)
{
	return hweight32((unsigned int)(w >> 32)) + hweight32((unsigned int)w);
}

static inline unsigned int hweight16(unsigned int w)
{
	return hweight32(w & 0xffff);
}

static inline unsigned int hweight8(unsigned int w)
{
	return hweight32(w & 0xff);
}

#endif				/* _BLACKFIN_BITOPS_H */
