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

#ifndef _ASM_POWERPC_PTE_BOOK3E_H
#define _ASM_POWERPC_PTE_BOOK3E_H
#ifdef __KERNEL__

/* PTE bit definitions for processors compliant to the Book3E
 * architecture 2.06 or later. The position of the PTE bits
 * matches the HW definition of the optional Embedded Page Table
 * category.
 */

/* Architected bits */
#define _PAGE_PRESENT	0x000001 /* software: pte contains a translation */
#define _PAGE_FILE	0x000002 /* (!present only) software: pte holds file offset */
#define _PAGE_SW1	0x000002
#define _PAGE_BAP_SR	0x000004
#define _PAGE_BAP_UR	0x000008
#define _PAGE_BAP_SW	0x000010
#define _PAGE_BAP_UW	0x000020
#define _PAGE_BAP_SX	0x000040
#define _PAGE_BAP_UX	0x000080
#define _PAGE_PSIZE_MSK	0x000f00
#define _PAGE_PSIZE_4K	0x000200
#define _PAGE_PSIZE_8K	0x000300
#define _PAGE_PSIZE_16K	0x000400
#define _PAGE_PSIZE_32K	0x000500
#define _PAGE_PSIZE_64K	0x000600
#define _PAGE_PSIZE_128K	0x000700
#define _PAGE_PSIZE_256K	0x000800
#define _PAGE_PSIZE_512K	0x000900
#define _PAGE_PSIZE_1M	0x000a00
#define _PAGE_PSIZE_2M	0x000b00
#define _PAGE_PSIZE_4M	0x000c00
#define _PAGE_PSIZE_8M	0x000d00
#define _PAGE_PSIZE_16M	0x000e00
#define _PAGE_PSIZE_32M	0x000f00
#define _PAGE_DIRTY	0x001000 /* C: page changed */
#define _PAGE_SW0	0x002000
#define _PAGE_U3	0x004000
#define _PAGE_U2	0x008000
#define _PAGE_U1	0x010000
#define _PAGE_U0	0x020000
#define _PAGE_ACCESSED	0x040000
#define _PAGE_LENDIAN	0x080000
#define _PAGE_GUARDED	0x100000
#define _PAGE_COHERENT	0x200000 /* M: enforce memory coherence */
#define _PAGE_NO_CACHE	0x400000 /* I: cache inhibit */
#define _PAGE_WRITETHRU	0x800000 /* W: cache write-through */

/* "Higher level" linux bit combinations */
#define _PAGE_EXEC		_PAGE_BAP_UX /* .. and was cache cleaned */
#define _PAGE_RW		(_PAGE_BAP_SW | _PAGE_BAP_UW) /* User write permission */
#define _PAGE_KERNEL_RW		(_PAGE_BAP_SW | _PAGE_BAP_SR | _PAGE_DIRTY)
#define _PAGE_KERNEL_RO		(_PAGE_BAP_SR)
#define _PAGE_KERNEL_RWX	(_PAGE_BAP_SW | _PAGE_BAP_SR | _PAGE_DIRTY | _PAGE_BAP_SX)
#define _PAGE_KERNEL_ROX	(_PAGE_BAP_SR | _PAGE_BAP_SX)
#define _PAGE_USER		(_PAGE_BAP_UR | _PAGE_BAP_SR) /* Can be read */

#define _PAGE_HASHPTE	0
#define _PAGE_BUSY	0

#define _PAGE_SPECIAL	_PAGE_SW0

/* Flags to be preserved on PTE modifications */
#define _PAGE_HPTEFLAGS	_PAGE_BUSY

/* Base page size */
#ifdef CONFIG_PPC_64K_PAGES
#define _PAGE_PSIZE	_PAGE_PSIZE_64K
#define PTE_RPN_SHIFT	(28)
#else
#define _PAGE_PSIZE	_PAGE_PSIZE_4K
#define	PTE_RPN_SHIFT	(24)
#endif

/* On 32-bit, we never clear the top part of the PTE */
#ifdef CONFIG_PPC32
#define _PTE_NONE_MASK	0xffffffff00000000ULL
#define _PMD_PRESENT	0
#define _PMD_PRESENT_MASK (PAGE_MASK)
#define _PMD_BAD	(~PAGE_MASK)
#endif

#endif /* __KERNEL__ */
#endif /*  _ASM_POWERPC_PTE_FSL_BOOKE_H */
