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

/* arch/sparc64/kernel/ktlb.S: Kernel mapping TLB miss handling.
 *
 * Copyright (C) 1995, 1997, 2005, 2008 David S. Miller <davem@davemloft.net>
 * Copyright (C) 1996 Eddie C. Dost        (ecd@brainaid.de)
 * Copyright (C) 1996 Miguel de Icaza      (miguel@nuclecu.unam.mx)
 * Copyright (C) 1996,98,99 Jakub Jelinek  (jj@sunsite.mff.cuni.cz)
 */

#include <asm/head.h>
#include <asm/asi.h>
#include <asm/page.h>
#include <asm/pgtable.h>
#include <asm/tsb.h>

	.text
	.align		32

kvmap_itlb:
	/* g6: TAG TARGET */
	mov		TLB_TAG_ACCESS, %g4
	ldxa		[%g4] ASI_IMMU, %g4

	/* sun4v_itlb_miss branches here with the missing virtual
	 * address already loaded into %g4
	 */
kvmap_itlb_4v:

kvmap_itlb_nonlinear:
	/* Catch kernel NULL pointer calls.  */
	sethi		%hi(PAGE_SIZE), %g5
	cmp		%g4, %g5
	bleu,pn		%xcc, kvmap_dtlb_longpath
	 nop

	KERN_TSB_LOOKUP_TL1(%g4, %g6, %g5, %g1, %g2, %g3, kvmap_itlb_load)

kvmap_itlb_tsb_miss:
	sethi		%hi(LOW_OBP_ADDRESS), %g5
	cmp		%g4, %g5
	blu,pn		%xcc, kvmap_itlb_vmalloc_addr
	 mov		0x1, %g5
	sllx		%g5, 32, %g5
	cmp		%g4, %g5
	blu,pn		%xcc, kvmap_itlb_obp
	 nop

kvmap_itlb_vmalloc_addr:
	KERN_PGTABLE_WALK(%g4, %g5, %g2, kvmap_itlb_longpath)

	KTSB_LOCK_TAG(%g1, %g2, %g7)

	/* Load and check PTE.  */
	ldxa		[%g5] ASI_PHYS_USE_EC, %g5
	mov		1, %g7
	sllx		%g7, TSB_TAG_INVALID_BIT, %g7
	brgez,a,pn	%g5, kvmap_itlb_longpath
	 KTSB_STORE(%g1, %g7)

	KTSB_WRITE(%g1, %g5, %g6)

	/* fallthrough to TLB load */

kvmap_itlb_load:

661:	stxa		%g5, [%g0] ASI_ITLB_DATA_IN
	retry
	.section	.sun4v_2insn_patch, "ax"
	.word		661b
	nop
	nop
	.previous

	/* For sun4v the ASI_ITLB_DATA_IN store and the retry
	 * instruction get nop'd out and we get here to branch
	 * to the sun4v tlb load code.  The registers are setup
	 * as follows:
	 *
	 * %g4: vaddr
	 * %g5: PTE
	 * %g6:	TAG
	 *
	 * The sun4v TLB load wants the PTE in %g3 so we fix that
	 * up here.
	 */
	ba,pt		%xcc, sun4v_itlb_load
	 mov		%g5, %g3

kvmap_itlb_longpath:

661:	rdpr	%pstate, %g5
	wrpr	%g5, PSTATE_AG | PSTATE_MG, %pstate
	.section .sun4v_2insn_patch, "ax"
	.word	661b
	SET_GL(1)
	nop
	.previous

	rdpr	%tpc, %g5
	ba,pt	%xcc, sparc64_realfault_common
	 mov	FAULT_CODE_ITLB, %g4

kvmap_itlb_obp:
	OBP_TRANS_LOOKUP(%g4, %g5, %g2, %g3, kvmap_itlb_longpath)

	KTSB_LOCK_TAG(%g1, %g2, %g7)

	KTSB_WRITE(%g1, %g5, %g6)

	ba,pt		%xcc, kvmap_itlb_load
	 nop

kvmap_dtlb_obp:
	OBP_TRANS_LOOKUP(%g4, %g5, %g2, %g3, kvmap_dtlb_longpath)

	KTSB_LOCK_TAG(%g1, %g2, %g7)

	KTSB_WRITE(%g1, %g5, %g6)

	ba,pt		%xcc, kvmap_dtlb_load
	 nop

	.align		32
kvmap_dtlb_tsb4m_load:
	KTSB_LOCK_TAG(%g1, %g2, %g7)
	KTSB_WRITE(%g1, %g5, %g6)
	ba,pt		%xcc, kvmap_dtlb_load
	 nop

kvmap_dtlb:
	/* %g6: TAG TARGET */
	mov		TLB_TAG_ACCESS, %g4
	ldxa		[%g4] ASI_DMMU, %g4

	/* sun4v_dtlb_miss branches here with the missing virtual
	 * address already loaded into %g4
	 */
kvmap_dtlb_4v:
	brgez,pn	%g4, kvmap_dtlb_nonlinear
	 nop

#ifdef CONFIG_DEBUG_PAGEALLOC
	/* Index through the base page size TSB even for linear
	 * mappings when using page allocation debugging.
	 */
	KERN_TSB_LOOKUP_TL1(%g4, %g6, %g5, %g1, %g2, %g3, kvmap_dtlb_load)
#else
	/* Correct TAG_TARGET is already in %g6, check 4mb TSB.  */
	KERN_TSB4M_LOOKUP_TL1(%g6, %g5, %g1, %g2, %g3, kvmap_dtlb_load)
#endif
	/* TSB entry address left in %g1, lookup linear PTE.
	 * Must preserve %g1 and %g6 (TAG).
	 */
kvmap_dtlb_tsb4m_miss:
	/* Clear the PAGE_OFFSET top virtual bits, shift
	 * down to get PFN, and make sure PFN is in range.
	 */
	sllx		%g4, 21, %g5

	/* Check to see if we know about valid memory at the 4MB
	 * chunk this physical address will reside within.
	 */
	srlx		%g5, 21 + 41, %g2
	brnz,pn		%g2, kvmap_dtlb_longpath
	 nop

	/* This unconditional branch and delay-slot nop gets patched
	 * by the sethi sequence once the bitmap is properly setup.
	 */
	.globl		valid_addr_bitmap_insn
valid_addr_bitmap_insn:
	ba,pt		%xcc, 2f
	 nop
	.subsection	2
	.globl		valid_addr_bitmap_patch
valid_addr_bitmap_patch:
	sethi		%hi(sparc64_valid_addr_bitmap), %g7
	or		%g7, %lo(sparc64_valid_addr_bitmap), %g7
	.previous

	srlx		%g5, 21 + 22, %g2
	srlx		%g2, 6, %g5
	and		%g2, 63, %g2
	sllx		%g5, 3, %g5
	ldx		[%g7 + %g5], %g5
	mov		1, %g7
	sllx		%g7, %g2, %g7
	andcc		%g5, %g7, %g0
	be,pn		%xcc, kvmap_dtlb_longpath

2:	 sethi		%hi(kpte_linear_bitmap), %g2
	or		%g2, %lo(kpte_linear_bitmap), %g2

	/* Get the 256MB physical address index. */
	sllx		%g4, 21, %g5
	mov		1, %g7
	srlx		%g5, 21 + 28, %g5

	/* Don't try this at home kids... this depends upon srlx
	 * only taking the low 6 bits of the shift count in %g5.
	 */
	sllx		%g7, %g5, %g7

	/* Divide by 64 to get the offset into the bitmask.  */
	srlx		%g5, 6, %g5
	sllx		%g5, 3, %g5

	/* kern_linear_pte_xor[((mask & bit) ? 1 : 0)] */
	ldx		[%g2 + %g5], %g2
	andcc		%g2, %g7, %g0
	sethi		%hi(kern_linear_pte_xor), %g5
	or		%g5, %lo(kern_linear_pte_xor), %g5
	bne,a,pt	%xcc, 1f
	 add		%g5, 8, %g5

1:	ldx		[%g5], %g2

	.globl		kvmap_linear_patch
kvmap_linear_patch:
	ba,pt		%xcc, kvmap_dtlb_tsb4m_load
	 xor		%g2, %g4, %g5

kvmap_dtlb_vmalloc_addr:
	KERN_PGTABLE_WALK(%g4, %g5, %g2, kvmap_dtlb_longpath)

	KTSB_LOCK_TAG(%g1, %g2, %g7)

	/* Load and check PTE.  */
	ldxa		[%g5] ASI_PHYS_USE_EC, %g5
	mov		1, %g7
	sllx		%g7, TSB_TAG_INVALID_BIT, %g7
	brgez,a,pn	%g5, kvmap_dtlb_longpath
	 KTSB_STORE(%g1, %g7)

	KTSB_WRITE(%g1, %g5, %g6)

	/* fallthrough to TLB load */

kvmap_dtlb_load:

661:	stxa		%g5, [%g0] ASI_DTLB_DATA_IN	! Reload TLB
	retry
	.section	.sun4v_2insn_patch, "ax"
	.word		661b
	nop
	nop
	.previous

	/* For sun4v the ASI_DTLB_DATA_IN store and the retry
	 * instruction get nop'd out and we get here to branch
	 * to the sun4v tlb load code.  The registers are setup
	 * as follows:
	 *
	 * %g4: vaddr
	 * %g5: PTE
	 * %g6:	TAG
	 *
	 * The sun4v TLB load wants the PTE in %g3 so we fix that
	 * up here.
	 */
	ba,pt		%xcc, sun4v_dtlb_load
	 mov		%g5, %g3

#ifdef CONFIG_SPARSEMEM_VMEMMAP
kvmap_vmemmap:
	sub		%g4, %g5, %g5
	srlx		%g5, 22, %g5
	sethi		%hi(vmemmap_table), %g1
	sllx		%g5, 3, %g5
	or		%g1, %lo(vmemmap_table), %g1
	ba,pt		%xcc, kvmap_dtlb_load
	 ldx		[%g1 + %g5], %g5
#endif

kvmap_dtlb_nonlinear:
	/* Catch kernel NULL pointer derefs.  */
	sethi		%hi(PAGE_SIZE), %g5
	cmp		%g4, %g5
	bleu,pn		%xcc, kvmap_dtlb_longpath
	 nop

#ifdef CONFIG_SPARSEMEM_VMEMMAP
	/* Do not use the TSB for vmemmap.  */
	mov		(VMEMMAP_BASE >> 40), %g5
	sllx		%g5, 40, %g5
	cmp		%g4,%g5
	bgeu,pn		%xcc, kvmap_vmemmap
	 nop
#endif

	KERN_TSB_LOOKUP_TL1(%g4, %g6, %g5, %g1, %g2, %g3, kvmap_dtlb_load)

kvmap_dtlb_tsbmiss:
	sethi		%hi(MODULES_VADDR), %g5
	cmp		%g4, %g5
	blu,pn		%xcc, kvmap_dtlb_longpath
	 mov		(VMALLOC_END >> 40), %g5
	sllx		%g5, 40, %g5
	cmp		%g4, %g5
	bgeu,pn		%xcc, kvmap_dtlb_longpath
	 nop

kvmap_check_obp:
	sethi		%hi(LOW_OBP_ADDRESS), %g5
	cmp		%g4, %g5
	blu,pn		%xcc, kvmap_dtlb_vmalloc_addr
	 mov		0x1, %g5
	sllx		%g5, 32, %g5
	cmp		%g4, %g5
	blu,pn		%xcc, kvmap_dtlb_obp
	 nop
	ba,pt		%xcc, kvmap_dtlb_vmalloc_addr
	 nop

kvmap_dtlb_longpath:

661:	rdpr	%pstate, %g5
	wrpr	%g5, PSTATE_AG | PSTATE_MG, %pstate
	.section .sun4v_2insn_patch, "ax"
	.word	661b
	SET_GL(1)
	ldxa		[%g0] ASI_SCRATCHPAD, %g5
	.previous

	rdpr	%tl, %g3
	cmp	%g3, 1

661:	mov	TLB_TAG_ACCESS, %g4
	ldxa	[%g4] ASI_DMMU, %g5
	.section .sun4v_2insn_patch, "ax"
	.word	661b
	ldx	[%g5 + HV_FAULT_D_ADDR_OFFSET], %g5
	nop
	.previous

	be,pt	%xcc, sparc64_realfault_common
	 mov	FAULT_CODE_DTLB, %g4
	ba,pt	%xcc, winfix_trampoline
	 nop
