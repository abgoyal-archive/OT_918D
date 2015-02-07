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
 * Copyright (C) 2009 PetaLogix
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License. See the file "COPYING" in the main directory of this archive
 * for more details.
 */

#include <linux/interrupt.h>
#include <asm/selfmod.h>

#undef DEBUG

#if __GNUC__ > 3
#error GCC 4 unsupported SELFMOD. Please disable SELFMOD from menuconfig.
#endif

#define OPCODE_IMM		0xB0000000
#define OPCODE_LWI		0xE8000000
#define OPCODE_LWI_MASK		0xEC000000
#define OPCODE_RTSD		0xB60F0008 /* return from func: rtsd r15, 8 */
#define OPCODE_ADDIK		0x30000000
#define OPCODE_ADDIK_MASK	0xFC000000

#define IMM_BASE	(OPCODE_IMM | (BARRIER_BASE_ADDR >> 16))
#define LWI_BASE	(OPCODE_LWI | (BARRIER_BASE_ADDR & 0x0000ff00))
#define LWI_BASE_MASK	(OPCODE_LWI_MASK | (BARRIER_BASE_ADDR & 0x0000ff00))
#define ADDIK_BASE	(OPCODE_ADDIK | (BARRIER_BASE_ADDR & 0x0000ff00))
#define ADDIK_BASE_MASK	(OPCODE_ADDIK_MASK | (BARRIER_BASE_ADDR & 0x0000ff00))

#define MODIFY_INSTR {						\
	pr_debug("%s: curr instr, (%d):0x%x, next(%d):0x%x\n",		\
		__func__, i, addr[i], i + 1, addr[i + 1]);		\
	addr[i] = OPCODE_IMM + (base >> 16);				\
	/* keep instruction opcode and add only last 16bits */		\
	addr[i + 1] = (addr[i + 1] & 0xffff00ff) + (base & 0xffff);	\
	__invalidate_icache(addr[i]);					\
	__invalidate_icache(addr[i + 1]);				\
	pr_debug("%s: hack instr, (%d):0x%x, next(%d):0x%x\n",		\
		__func__, i, addr[i], i + 1, addr[i + 1]); }

/* NOTE
 * self-modified part of code for improvement of interrupt controller
 * save instruction in interrupt rutine
 */
void selfmod_function(const int *arr_fce, const unsigned int base)
{
	unsigned int flags, i, j, *addr = NULL;

	local_irq_save(flags);
	__disable_icache();

	/* zero terminated array */
	for (j = 0; arr_fce[j] != 0; j++) {
		/* get start address of function */
		addr = (unsigned int *) arr_fce[j];
		pr_debug("%s: func(%d) at 0x%x\n",
					__func__, j, (unsigned int) addr);
		for (i = 0; ; i++) {
			pr_debug("%s: instruction code at %d: 0x%x\n",
						__func__, i, addr[i]);
			if (addr[i] == IMM_BASE) {
				/* detecting of lwi (0xE8) or swi (0xF8) instr
				 * I can detect both opcode with one mask */
				if ((addr[i + 1] & LWI_BASE_MASK) == LWI_BASE) {
					MODIFY_INSTR;
				} else /* detection addik for ack */
				if ((addr[i + 1] & ADDIK_BASE_MASK) ==
								ADDIK_BASE) {
					MODIFY_INSTR;
				}
			} else if (addr[i] == OPCODE_RTSD) {
				/* return from function means end of function */
				pr_debug("%s: end of array %d\n", __func__, i);
				break;
			}
		}
	}
	local_irq_restore(flags);
} /* end of self-modified code */
