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
 * uClinux flat-format executables
 *
 * Copyright (C) 2005 John Williams <jwilliams@itee.uq.edu.au>
 *
 * This file is subject to the terms and conditions of the GNU General
 * Public License. See the file COPYING in the main directory of this
 * archive for more details.
 */

#ifndef _ASM_MICROBLAZE_FLAT_H
#define _ASM_MICROBLAZE_FLAT_H

#include <asm/unaligned.h>

#define	flat_argvp_envp_on_stack()	0
#define	flat_old_ram_flag(flags)	(flags)
#define	flat_reloc_valid(reloc, size)	((reloc) <= (size))
#define	flat_set_persistent(relval, p)		0

/*
 * Microblaze works a little differently from other arches, because
 * of the MICROBLAZE_64 reloc type. Here, a 32 bit address is split
 * over two instructions, an 'imm' instruction which provides the top
 * 16 bits, then the instruction "proper" which provides the low 16
 * bits.
 */

/*
 * Crack open a symbol reference and extract the address to be
 * relocated. rp is a potentially unaligned pointer to the
 * reference
 */

static inline unsigned long
flat_get_addr_from_rp(unsigned long *rp, unsigned long relval,
			unsigned long flags, unsigned long *persistent)
{
	unsigned long addr;
	(void)flags;

	/* Is it a split 64/32 reference? */
	if (relval & 0x80000000) {
		/* Grab the two halves of the reference */
		unsigned long val_hi, val_lo;

		val_hi = get_unaligned(rp);
		val_lo = get_unaligned(rp+1);

		/* Crack the address out */
		addr = ((val_hi & 0xffff) << 16) + (val_lo & 0xffff);
	} else {
		/* Get the address straight out */
		addr = get_unaligned(rp);
	}

	return addr;
}

/*
 * Insert an address into the symbol reference at rp. rp is potentially
 * unaligned.
 */

static inline void
flat_put_addr_at_rp(unsigned long *rp, unsigned long addr, unsigned long relval)
{
	/* Is this a split 64/32 reloc? */
	if (relval & 0x80000000) {
		/* Get the two "halves" */
		unsigned long val_hi = get_unaligned(rp);
		unsigned long val_lo = get_unaligned(rp + 1);

		/* insert the address */
		val_hi = (val_hi & 0xffff0000) | addr >> 16;
		val_lo = (val_lo & 0xffff0000) | (addr & 0xffff);

		/* store the two halves back into memory */
		put_unaligned(val_hi, rp);
		put_unaligned(val_lo, rp+1);
	} else {
		/* Put it straight in, no messing around */
		put_unaligned(addr, rp);
	}
}

#define	flat_get_relocate_addr(rel)	(rel & 0x7fffffff)

#endif /* _ASM_MICROBLAZE_FLAT_H */
