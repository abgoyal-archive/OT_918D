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

#ifndef _ASM_POWERPC_DELAY_H
#define _ASM_POWERPC_DELAY_H
#ifdef __KERNEL__

#include <asm/time.h>

/*
 * Copyright 1996, Paul Mackerras.
 * Copyright (C) 2009 Freescale Semiconductor, Inc. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 *
 * PPC64 Support added by Dave Engebretsen, Todd Inglett, Mike Corrigan,
 * Anton Blanchard.
 */

extern void __delay(unsigned long loops);
extern void udelay(unsigned long usecs);

/*
 * On shared processor machines the generic implementation of mdelay can
 * result in large errors. While each iteration of the loop inside mdelay
 * is supposed to take 1ms, the hypervisor could sleep our partition for
 * longer (eg 10ms). With the right timing these errors can add up.
 *
 * Since there is no 32bit overflow issue on 64bit kernels, just call
 * udelay directly.
 */
#ifdef CONFIG_PPC64
#define mdelay(n)	udelay((n) * 1000)
#endif

/**
 * spin_event_timeout - spin until a condition gets true or a timeout elapses
 * @condition: a C expression to evalate
 * @timeout: timeout, in microseconds
 * @delay: the number of microseconds to delay between each evaluation of
 *         @condition
 *
 * The process spins until the condition evaluates to true (non-zero) or the
 * timeout elapses.  The return value of this macro is the value of
 * @condition when the loop terminates. This allows you to determine the cause
 * of the loop terminates.  If the return value is zero, then you know a
 * timeout has occurred.
 *
 * This primary purpose of this macro is to poll on a hardware register
 * until a status bit changes.  The timeout ensures that the loop still
 * terminates even if the bit never changes.  The delay is for devices that
 * need a delay in between successive reads.
 *
 * gcc will optimize out the if-statement if @delay is a constant.
 */
#define spin_event_timeout(condition, timeout, delay)                          \
({                                                                             \
	typeof(condition) __ret;                                               \
	unsigned long __loops = tb_ticks_per_usec * timeout;                   \
	unsigned long __start = get_tbl();                                     \
	while (!(__ret = (condition)) && (tb_ticks_since(__start) <= __loops)) \
		if (delay)                                                     \
			udelay(delay);                                         \
		else                                                           \
			cpu_relax();                                           \
	if (!__ret)                                                            \
		__ret = (condition);                                           \
	__ret;		                                                       \
})

#endif /* __KERNEL__ */
#endif /* _ASM_POWERPC_DELAY_H */
