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
 *  arch/arm/mach-rpc/include/mach/io.h
 *
 *  Copyright (C) 1997 Russell King
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Modifications:
 *  06-Dec-1997	RMK	Created.
 */
#ifndef __ASM_ARM_ARCH_IO_H
#define __ASM_ARM_ARCH_IO_H

#include <mach/hardware.h>

#define IO_SPACE_LIMIT 0xffffffff

/*
 * We use two different types of addressing - PC style addresses, and ARM
 * addresses.  PC style accesses the PC hardware with the normal PC IO
 * addresses, eg 0x3f8 for serial#1.  ARM addresses are 0x80000000+
 * and are translated to the start of IO.  Note that all addresses are
 * shifted left!
 */
#define __PORT_PCIO(x)	(!((x) & 0x80000000))

/*
 * Dynamic IO functions.
 */
static inline void __outb (unsigned int value, unsigned int port)
{
	unsigned long temp;
	__asm__ __volatile__(
	"tst	%2, #0x80000000\n\t"
	"mov	%0, %4\n\t"
	"addeq	%0, %0, %3\n\t"
	"strb	%1, [%0, %2, lsl #2]	@ outb"
	: "=&r" (temp)
	: "r" (value), "r" (port), "Ir" (PCIO_BASE - IO_BASE), "Ir" (IO_BASE)
	: "cc");
}

static inline void __outw (unsigned int value, unsigned int port)
{
	unsigned long temp;
	__asm__ __volatile__(
	"tst	%2, #0x80000000\n\t"
	"mov	%0, %4\n\t"
	"addeq	%0, %0, %3\n\t"
	"str	%1, [%0, %2, lsl #2]	@ outw"
	: "=&r" (temp)
	: "r" (value|value<<16), "r" (port), "Ir" (PCIO_BASE - IO_BASE), "Ir" (IO_BASE)
	: "cc");
}

static inline void __outl (unsigned int value, unsigned int port)
{
	unsigned long temp;
	__asm__ __volatile__(
	"tst	%2, #0x80000000\n\t"
	"mov	%0, %4\n\t"
	"addeq	%0, %0, %3\n\t"
	"str	%1, [%0, %2, lsl #2]	@ outl"
	: "=&r" (temp)
	: "r" (value), "r" (port), "Ir" (PCIO_BASE - IO_BASE), "Ir" (IO_BASE)
	: "cc");
}

#define DECLARE_DYN_IN(sz,fnsuffix,instr)					\
static inline unsigned sz __in##fnsuffix (unsigned int port)		\
{										\
	unsigned long temp, value;						\
	__asm__ __volatile__(							\
	"tst	%2, #0x80000000\n\t"						\
	"mov	%0, %4\n\t"							\
	"addeq	%0, %0, %3\n\t"							\
	"ldr" instr "	%1, [%0, %2, lsl #2]	@ in" #fnsuffix			\
	: "=&r" (temp), "=r" (value)						\
	: "r" (port), "Ir" (PCIO_BASE - IO_BASE), "Ir" (IO_BASE)		\
	: "cc");								\
	return (unsigned sz)value;						\
}

static inline void __iomem *__deprecated __ioaddr(unsigned int port)
{
	void __iomem *ret;
	if (__PORT_PCIO(port))
		ret = PCIO_BASE;
	else
		ret = IO_BASE;
	return ret + (port << 2);
}

#define DECLARE_IO(sz,fnsuffix,instr)	\
	DECLARE_DYN_IN(sz,fnsuffix,instr)

DECLARE_IO(char,b,"b")
DECLARE_IO(short,w,"")
DECLARE_IO(int,l,"")

#undef DECLARE_IO
#undef DECLARE_DYN_IN

/*
 * Constant address IO functions
 *
 * These have to be macros for the 'J' constraint to work -
 * +/-4096 immediate operand.
 */
#define __outbc(value,port)							\
({										\
	if (__PORT_PCIO((port)))						\
		__asm__ __volatile__(						\
		"strb	%0, [%1, %2]	@ outbc"				\
		: : "r" (value), "r" (PCIO_BASE), "Jr" ((port) << 2));		\
	else									\
		__asm__ __volatile__(						\
		"strb	%0, [%1, %2]	@ outbc"				\
		: : "r" (value), "r" (IO_BASE), "r" ((port) << 2));		\
})

#define __inbc(port)								\
({										\
	unsigned char result;							\
	if (__PORT_PCIO((port)))						\
		__asm__ __volatile__(						\
		"ldrb	%0, [%1, %2]	@ inbc"					\
		: "=r" (result) : "r" (PCIO_BASE), "Jr" ((port) << 2));		\
	else									\
		__asm__ __volatile__(						\
		"ldrb	%0, [%1, %2]	@ inbc"					\
		: "=r" (result) : "r" (IO_BASE), "r" ((port) << 2));		\
	result;									\
})

#define __outwc(value,port)							\
({										\
	unsigned long __v = value;						\
	if (__PORT_PCIO((port)))						\
		__asm__ __volatile__(						\
		"str	%0, [%1, %2]	@ outwc"				\
		: : "r" (__v|__v<<16), "r" (PCIO_BASE), "Jr" ((port) << 2));	\
	else									\
		__asm__ __volatile__(						\
		"str	%0, [%1, %2]	@ outwc"				\
		: : "r" (__v|__v<<16), "r" (IO_BASE), "r" ((port) << 2));		\
})

#define __inwc(port)								\
({										\
	unsigned short result;							\
	if (__PORT_PCIO((port)))						\
		__asm__ __volatile__(						\
		"ldr	%0, [%1, %2]	@ inwc"					\
		: "=r" (result) : "r" (PCIO_BASE), "Jr" ((port) << 2));		\
	else									\
		__asm__ __volatile__(						\
		"ldr	%0, [%1, %2]	@ inwc"					\
		: "=r" (result) : "r" (IO_BASE), "r" ((port) << 2));		\
	result & 0xffff;							\
})

#define __outlc(value,port)							\
({										\
	unsigned long __v = value;						\
	if (__PORT_PCIO((port)))						\
		__asm__ __volatile__(						\
		"str	%0, [%1, %2]	@ outlc"				\
		: : "r" (__v), "r" (PCIO_BASE), "Jr" ((port) << 2));		\
	else									\
		__asm__ __volatile__(						\
		"str	%0, [%1, %2]	@ outlc"				\
		: : "r" (__v), "r" (IO_BASE), "r" ((port) << 2));		\
})

#define __inlc(port)								\
({										\
	unsigned long result;							\
	if (__PORT_PCIO((port)))						\
		__asm__ __volatile__(						\
		"ldr	%0, [%1, %2]	@ inlc"					\
		: "=r" (result) : "r" (PCIO_BASE), "Jr" ((port) << 2));		\
	else									\
		__asm__ __volatile__(						\
		"ldr	%0, [%1, %2]	@ inlc"					\
		: "=r" (result) : "r" (IO_BASE), "r" ((port) << 2));		\
	result;									\
})

#define inb(p)	 	(__builtin_constant_p((p)) ? __inbc(p)    : __inb(p))
#define inw(p)	 	(__builtin_constant_p((p)) ? __inwc(p)    : __inw(p))
#define inl(p)	 	(__builtin_constant_p((p)) ? __inlc(p)    : __inl(p))
#define outb(v,p)	(__builtin_constant_p((p)) ? __outbc(v,p) : __outb(v,p))
#define outw(v,p)	(__builtin_constant_p((p)) ? __outwc(v,p) : __outw(v,p))
#define outl(v,p)	(__builtin_constant_p((p)) ? __outlc(v,p) : __outl(v,p))

/* the following macro is deprecated */
#define ioaddr(port)	((unsigned long)__ioaddr((port)))

#define insb(p,d,l)	__raw_readsb(__ioaddr(p),d,l)
#define insw(p,d,l)	__raw_readsw(__ioaddr(p),d,l)

#define outsb(p,d,l)	__raw_writesb(__ioaddr(p),d,l)
#define outsw(p,d,l)	__raw_writesw(__ioaddr(p),d,l)

/*
 * 1:1 mapping for ioremapped regions.
 */
#define __mem_pci(x)	(x)

#endif
