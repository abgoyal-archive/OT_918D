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

#ifndef _BLACKFIN_BUG_H
#define _BLACKFIN_BUG_H

#ifdef CONFIG_BUG

/*
 * This can be any undefined 16-bit opcode, meaning
 * ((opcode & 0xc000) != 0xc000)
 * Anything from 0x0001 to 0x000A (inclusive) will work
 */
#define BFIN_BUG_OPCODE	0x0001

#ifdef CONFIG_DEBUG_BUGVERBOSE

#define _BUG_OR_WARN(flags)						\
	asm volatile(							\
		"1:	.hword	%0\n"					\
		"	.section __bug_table,\"a\",@progbits\n"		\
		"2:	.long	1b\n"					\
		"	.long	%1\n"					\
		"	.short	%2\n"					\
		"	.short	%3\n"					\
		"	.org	2b + %4\n"				\
		"	.previous"					\
		:							\
		: "i"(BFIN_BUG_OPCODE), "i"(__FILE__),			\
		  "i"(__LINE__), "i"(flags),				\
		  "i"(sizeof(struct bug_entry)))

#else

#define _BUG_OR_WARN(flags)						\
	asm volatile(							\
		"1:	.hword	%0\n"					\
		"	.section __bug_table,\"a\",@progbits\n"		\
		"2:	.long	1b\n"					\
		"	.short	%1\n"					\
		"	.org	2b + %2\n"				\
		"	.previous"					\
		:							\
		: "i"(BFIN_BUG_OPCODE), "i"(flags),			\
		  "i"(sizeof(struct bug_entry)))

#endif /* CONFIG_DEBUG_BUGVERBOSE */

#define BUG()								\
	do {								\
		_BUG_OR_WARN(0);					\
		unreachable();						\
	} while (0)

#define WARN_ON(condition)							\
	({								\
		int __ret_warn_on = !!(condition);			\
		if (unlikely(__ret_warn_on))				\
			_BUG_OR_WARN(BUGFLAG_WARNING);			\
		unlikely(__ret_warn_on);				\
	})

#define HAVE_ARCH_BUG
#define HAVE_ARCH_WARN_ON

#endif

#include <asm-generic/bug.h>

#endif
