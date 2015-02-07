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
 * TLB flushing operations for SH with an MMU.
 *
 *  Copyright (C) 1999  Niibe Yutaka
 *  Copyright (C) 2003  Paul Mundt
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */
#include <linux/mm.h>
#include <asm/mmu_context.h>
#include <asm/tlbflush.h>

void local_flush_tlb_page(struct vm_area_struct *vma, unsigned long page)
{
	unsigned int cpu = smp_processor_id();

	if (vma->vm_mm && cpu_context(cpu, vma->vm_mm) != NO_CONTEXT) {
		unsigned long flags;
		unsigned long asid;
		unsigned long saved_asid = MMU_NO_ASID;

		asid = cpu_asid(cpu, vma->vm_mm);
		page &= PAGE_MASK;

		local_irq_save(flags);
		if (vma->vm_mm != current->mm) {
			saved_asid = get_asid();
			set_asid(asid);
		}
		local_flush_tlb_one(asid, page);
		if (saved_asid != MMU_NO_ASID)
			set_asid(saved_asid);
		local_irq_restore(flags);
	}
}

void local_flush_tlb_range(struct vm_area_struct *vma, unsigned long start,
			   unsigned long end)
{
	struct mm_struct *mm = vma->vm_mm;
	unsigned int cpu = smp_processor_id();

	if (cpu_context(cpu, mm) != NO_CONTEXT) {
		unsigned long flags;
		int size;

		local_irq_save(flags);
		size = (end - start + (PAGE_SIZE - 1)) >> PAGE_SHIFT;
		if (size > (MMU_NTLB_ENTRIES/4)) { /* Too many TLB to flush */
			cpu_context(cpu, mm) = NO_CONTEXT;
			if (mm == current->mm)
				activate_context(mm, cpu);
		} else {
			unsigned long asid;
			unsigned long saved_asid = MMU_NO_ASID;

			asid = cpu_asid(cpu, mm);
			start &= PAGE_MASK;
			end += (PAGE_SIZE - 1);
			end &= PAGE_MASK;
			if (mm != current->mm) {
				saved_asid = get_asid();
				set_asid(asid);
			}
			while (start < end) {
				local_flush_tlb_one(asid, start);
				start += PAGE_SIZE;
			}
			if (saved_asid != MMU_NO_ASID)
				set_asid(saved_asid);
		}
		local_irq_restore(flags);
	}
}

void local_flush_tlb_kernel_range(unsigned long start, unsigned long end)
{
	unsigned int cpu = smp_processor_id();
	unsigned long flags;
	int size;

	local_irq_save(flags);
	size = (end - start + (PAGE_SIZE - 1)) >> PAGE_SHIFT;
	if (size > (MMU_NTLB_ENTRIES/4)) { /* Too many TLB to flush */
		local_flush_tlb_all();
	} else {
		unsigned long asid;
		unsigned long saved_asid = get_asid();

		asid = cpu_asid(cpu, &init_mm);
		start &= PAGE_MASK;
		end += (PAGE_SIZE - 1);
		end &= PAGE_MASK;
		set_asid(asid);
		while (start < end) {
			local_flush_tlb_one(asid, start);
			start += PAGE_SIZE;
		}
		set_asid(saved_asid);
	}
	local_irq_restore(flags);
}

void local_flush_tlb_mm(struct mm_struct *mm)
{
	unsigned int cpu = smp_processor_id();

	/* Invalidate all TLB of this process. */
	/* Instead of invalidating each TLB, we get new MMU context. */
	if (cpu_context(cpu, mm) != NO_CONTEXT) {
		unsigned long flags;

		local_irq_save(flags);
		cpu_context(cpu, mm) = NO_CONTEXT;
		if (mm == current->mm)
			activate_context(mm, cpu);
		local_irq_restore(flags);
	}
}
