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

#ifndef _ASM_GENERIC_LOCAL_H
#define _ASM_GENERIC_LOCAL_H

#include <linux/percpu.h>
#include <asm/atomic.h>
#include <asm/types.h>

/*
 * A signed long type for operations which are atomic for a single CPU.
 * Usually used in combination with per-cpu variables.
 *
 * This is the default implementation, which uses atomic_long_t.  Which is
 * rather pointless.  The whole point behind local_t is that some processors
 * can perform atomic adds and subtracts in a manner which is atomic wrt IRQs
 * running on this CPU.  local_t allows exploitation of such capabilities.
 */

/* Implement in terms of atomics. */

/* Don't use typedef: don't want them to be mixed with atomic_t's. */
typedef struct
{
	atomic_long_t a;
} local_t;

#define LOCAL_INIT(i)	{ ATOMIC_LONG_INIT(i) }

#define local_read(l)	atomic_long_read(&(l)->a)
#define local_set(l,i)	atomic_long_set((&(l)->a),(i))
#define local_inc(l)	atomic_long_inc(&(l)->a)
#define local_dec(l)	atomic_long_dec(&(l)->a)
#define local_add(i,l)	atomic_long_add((i),(&(l)->a))
#define local_sub(i,l)	atomic_long_sub((i),(&(l)->a))

#define local_sub_and_test(i, l) atomic_long_sub_and_test((i), (&(l)->a))
#define local_dec_and_test(l) atomic_long_dec_and_test(&(l)->a)
#define local_inc_and_test(l) atomic_long_inc_and_test(&(l)->a)
#define local_add_negative(i, l) atomic_long_add_negative((i), (&(l)->a))
#define local_add_return(i, l) atomic_long_add_return((i), (&(l)->a))
#define local_sub_return(i, l) atomic_long_sub_return((i), (&(l)->a))
#define local_inc_return(l) atomic_long_inc_return(&(l)->a)

#define local_cmpxchg(l, o, n) atomic_long_cmpxchg((&(l)->a), (o), (n))
#define local_xchg(l, n) atomic_long_xchg((&(l)->a), (n))
#define local_add_unless(l, _a, u) atomic_long_add_unless((&(l)->a), (_a), (u))
#define local_inc_not_zero(l) atomic_long_inc_not_zero(&(l)->a)

/* Non-atomic variants, ie. preemption disabled and won't be touched
 * in interrupt, etc.  Some archs can optimize this case well. */
#define __local_inc(l)		local_set((l), local_read(l) + 1)
#define __local_dec(l)		local_set((l), local_read(l) - 1)
#define __local_add(i,l)	local_set((l), local_read(l) + (i))
#define __local_sub(i,l)	local_set((l), local_read(l) - (i))

#endif /* _ASM_GENERIC_LOCAL_H */
