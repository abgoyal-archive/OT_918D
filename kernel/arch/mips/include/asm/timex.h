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
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */

/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1998, 1999, 2003 by Ralf Baechle
 */
#ifndef _ASM_TIMEX_H
#define _ASM_TIMEX_H

#ifdef __KERNEL__

#include <asm/mipsregs.h>

/*
 * This is the clock rate of the i8253 PIT.  A MIPS system may not have
 * a PIT by the symbol is used all over the kernel including some APIs.
 * So keeping it defined to the number for the PIT is the only sane thing
 * for now.
 */
#define CLOCK_TICK_RATE 1193182

extern unsigned int mips_hpt_frequency;

/*
 * Standard way to access the cycle counter.
 * Currently only used on SMP for scheduling.
 *
 * Only the low 32 bits are available as a continuously counting entity.
 * But this only means we'll force a reschedule every 8 seconds or so,
 * which isn't an evil thing.
 *
 * We know that all SMP capable CPUs have cycle counters.
 *
 * Mathieu Desnoyers <mathieu.desnoyers@polymtl.ca>
 * HAVE_GET_CYCLES makes sure that this case is handled properly :
 *
 * Ralf Baechle <ralf@linux-mips.org> :
 * This avoids us executing an mfc0 c0_count instruction on processors which
 * don't have but also on certain R4000 and R4400 versions where reading from
 * the count register just in the very moment when its value equals c0_compare
 * will result in the timer interrupt getting lost.
 */

typedef unsigned int cycles_t;

#ifdef CONFIG_HAVE_GET_CYCLES_32
static inline cycles_t get_cycles(void)
{
	return read_c0_count();
}

static inline void get_cycles_barrier(void)
{
}

static inline cycles_t get_cycles_rate(void)
{
	return mips_hpt_frequency;
}

extern int test_tsc_synchronization(void);
extern int _tsc_is_sync;
static inline int tsc_is_sync(void)
{
	return _tsc_is_sync;
}
#else
static inline cycles_t get_cycles(void)
{
	return 0;
}
static inline int test_tsc_synchronization(void)
{
	return 0;
}
static inline int tsc_is_sync(void)
{
	return 0;
}
#endif

#define DELAY_INTERRUPT 100
/*
 * Only updates 32 LSB.
 */
static inline void write_tsc(u32 val1, u32 val2)
{
	write_c0_count(val1);
	/* Arrange for an interrupt in a short while */
	write_c0_compare(read_c0_count() + DELAY_INTERRUPT);
}

/*
 * Currently unused, should update internal tsc-related timekeeping sources.
 */
static inline void mark_tsc_unstable(char *reason)
{
}

/*
 * Currently simply use the tsc_is_sync value.
 */
static inline int unsynchronized_tsc(void)
{
	return !tsc_is_sync();
}

#endif /* __KERNEL__ */

#endif /*  _ASM_TIMEX_H */
