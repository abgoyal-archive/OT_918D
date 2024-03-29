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

#ifndef _ASM_X86_IMMEDIATE_H
#define _ASM_X86_IMMEDIATE_H

/*
 * Immediate values. x86 architecture optimizations.
 *
 * (C) Copyright 2006 Mathieu Desnoyers <mathieu.desnoyers@polymtl.ca>
 *
 * Dual BSD/GPL v2 license.
 */

#include <asm/asm.h>

struct __imv {
	unsigned long var;	/* Pointer to the identifier variable of the
				 * immediate value
				 */
	unsigned long imv;	/*
				 * Pointer to the memory location of the
				 * immediate value within the instruction.
				 */
	unsigned char size;	/* Type size. */
	unsigned char insn_size;/* Instruction size. */
} __attribute__ ((packed));

/**
 * imv_read - read immediate variable
 * @name: immediate value name
 *
 * Reads the value of @name.
 * Optimized version of the immediate.
 * Do not use in __init and __exit functions. Use _imv_read() instead.
 * If size is bigger than the architecture long size, fall back on a memory
 * read.
 *
 * Make sure to populate the initial static 64 bits opcode with a value
 * what will generate an instruction with 8 bytes immediate value (not the REX.W
 * prefixed one that loads a sign extended 32 bits immediate value in a r64
 * register).
 *
 * Create the instruction in a discarded section to calculate its size. This is
 * how we can align the beginning of the instruction on an address that will
 * permit atomic modification of the immediate value without knowing the size of
 * the opcode used by the compiler. The operand size is known in advance.
 */
#define imv_read(name)							\
	({								\
		__typeof__(name##__imv) value;				\
		BUILD_BUG_ON(sizeof(value) > 8);			\
		switch (sizeof(value)) {				\
		case 1:							\
			asm(".section __discard,\"\",@progbits\n\t"	\
				"1:\n\t"				\
				"mov $0,%0\n\t"				\
				"2:\n\t"				\
				".previous\n\t"				\
				".section __imv,\"aw\",@progbits\n\t"	\
				_ASM_PTR "%c1, (3f)-%c2\n\t"		\
				".byte %c2, (2b-1b)\n\t"		\
				".previous\n\t"				\
				"mov $0,%0\n\t"				\
				"3:\n\t"				\
				: "=q" (value)				\
				: "i" (&name##__imv),			\
				  "i" (sizeof(value)));			\
			break;						\
		case 2:							\
		case 4:							\
			asm(".section __discard,\"\",@progbits\n\t"	\
				"1:\n\t"				\
				"mov $0,%0\n\t"				\
				"2:\n\t"				\
				".previous\n\t"				\
				".section __imv,\"aw\",@progbits\n\t"	\
				_ASM_PTR "%c1, (3f)-%c2\n\t"		\
				".byte %c2, (2b-1b)\n\t"		\
				".previous\n\t"				\
				".org . + ((-.-(2b-1b)) & (%c2-1)), 0x90\n\t" \
				"mov $0,%0\n\t"				\
				"3:\n\t"				\
				: "=r" (value)				\
				: "i" (&name##__imv),			\
				  "i" (sizeof(value)));			\
			break;						\
		case 8:							\
			if (sizeof(long) < 8) {				\
				value = name##__imv;			\
				break;					\
			}						\
			asm(".section __discard,\"\",@progbits\n\t"	\
				"1:\n\t"				\
				"mov $0xFEFEFEFE01010101,%0\n\t"	\
				"2:\n\t"				\
				".previous\n\t"				\
				".section __imv,\"aw\",@progbits\n\t"	\
				_ASM_PTR "%c1, (3f)-%c2\n\t"		\
				".byte %c2, (2b-1b)\n\t"		\
				".previous\n\t"				\
				".org . + ((-.-(2b-1b)) & (%c2-1)), 0x90\n\t" \
				"mov $0xFEFEFEFE01010101,%0\n\t" 	\
				"3:\n\t"				\
				: "=r" (value)				\
				: "i" (&name##__imv),			\
				  "i" (sizeof(value)));			\
			break;						\
		};							\
		value;							\
	})

extern int arch_imv_update(const struct __imv *imv, int early);

#endif /* _ASM_X86_IMMEDIATE_H */
