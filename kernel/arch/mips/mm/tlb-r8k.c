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
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1996 David S. Miller (dm@engr.sgi.com)
 * Copyright (C) 1997, 1998, 1999, 2000 Ralf Baechle ralf@gnu.org
 * Carsten Langgaard, carstenl@mips.com
 * Copyright (C) 2002 MIPS Technologies, Inc.  All rights reserved.
 */
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/smp.h>
#include <linux/mm.h>

#include <asm/cpu.h>
#include <asm/bootinfo.h>
#include <asm/mmu_context.h>
#include <asm/pgtable.h>
#include <asm/system.h>

extern void build_tlb_refill_handler(void);

#define TFP_TLB_SIZE		384
#define TFP_TLB_SET_SHIFT	7

/* CP0 hazard avoidance. */
#define BARRIER __asm__ __volatile__(".set noreorder\n\t" \
				     "nop; nop; nop; nop; nop; nop;\n\t" \
				     ".set reorder\n\t")

void local_flush_tlb_all(void)
{
	unsigned long flags;
	unsigned long old_ctx;
	int entry;

	local_irq_save(flags);
	/* Save old context and create impossible VPN2 value */
	old_ctx = read_c0_entryhi();
	write_c0_entrylo(0);

	for (entry = 0; entry < TFP_TLB_SIZE; entry++) {
		write_c0_tlbset(entry >> TFP_TLB_SET_SHIFT);
		write_c0_vaddr(entry << PAGE_SHIFT);
		write_c0_entryhi(CKSEG0 + (entry << (PAGE_SHIFT + 1)));
		mtc0_tlbw_hazard();
		tlb_write();
	}
	tlbw_use_hazard();
	write_c0_entryhi(old_ctx);
	local_irq_restore(flags);
}

void local_flush_tlb_mm(struct mm_struct *mm)
{
	int cpu = smp_processor_id();

	if (cpu_context(cpu, mm) != 0)
		drop_mmu_context(mm, cpu);
}

void local_flush_tlb_range(struct vm_area_struct *vma, unsigned long start,
	unsigned long end)
{
	struct mm_struct *mm = vma->vm_mm;
	int cpu = smp_processor_id();
	unsigned long flags;
	int oldpid, newpid, size;

	if (!cpu_context(cpu, mm))
		return;

	size = (end - start + (PAGE_SIZE - 1)) >> PAGE_SHIFT;
	size = (size + 1) >> 1;

	local_irq_save(flags);

	if (size > TFP_TLB_SIZE / 2) {
		drop_mmu_context(mm, cpu);
		goto out_restore;
	}

	oldpid = read_c0_entryhi();
	newpid = cpu_asid(cpu, mm);

	write_c0_entrylo(0);

	start &= PAGE_MASK;
	end += (PAGE_SIZE - 1);
	end &= PAGE_MASK;
	while (start < end) {
		signed long idx;

		write_c0_vaddr(start);
		write_c0_entryhi(start);
		start += PAGE_SIZE;
		tlb_probe();
		idx = read_c0_tlbset();
		if (idx < 0)
			continue;

		write_c0_entryhi(CKSEG0 + (idx << (PAGE_SHIFT + 1)));
		tlb_write();
	}
	write_c0_entryhi(oldpid);

out_restore:
	local_irq_restore(flags);
}

/* Usable for KV1 addresses only! */
void local_flush_tlb_kernel_range(unsigned long start, unsigned long end)
{
	unsigned long size, flags;

	size = (end - start + (PAGE_SIZE - 1)) >> PAGE_SHIFT;
	size = (size + 1) >> 1;

	if (size > TFP_TLB_SIZE / 2) {
		local_flush_tlb_all();
		return;
	}

	local_irq_save(flags);

	write_c0_entrylo(0);

	start &= PAGE_MASK;
	end += (PAGE_SIZE - 1);
	end &= PAGE_MASK;
	while (start < end) {
		signed long idx;

		write_c0_vaddr(start);
		write_c0_entryhi(start);
		start += PAGE_SIZE;
		tlb_probe();
		idx = read_c0_tlbset();
		if (idx < 0)
			continue;

		write_c0_entryhi(CKSEG0 + (idx << (PAGE_SHIFT + 1)));
		tlb_write();
	}

	local_irq_restore(flags);
}

void local_flush_tlb_page(struct vm_area_struct *vma, unsigned long page)
{
	int cpu = smp_processor_id();
	unsigned long flags;
	int oldpid, newpid;
	signed long idx;

	if (!cpu_context(cpu, vma->vm_mm))
		return;

	newpid = cpu_asid(cpu, vma->vm_mm);
	page &= PAGE_MASK;
	local_irq_save(flags);
	oldpid = read_c0_entryhi();
	write_c0_vaddr(page);
	write_c0_entryhi(newpid);
	tlb_probe();
	idx = read_c0_tlbset();
	if (idx < 0)
		goto finish;

	write_c0_entrylo(0);
	write_c0_entryhi(CKSEG0 + (idx << (PAGE_SHIFT + 1)));
	tlb_write();

finish:
	write_c0_entryhi(oldpid);
	local_irq_restore(flags);
}

/*
 * We will need multiple versions of update_mmu_cache(), one that just
 * updates the TLB with the new pte(s), and another which also checks
 * for the R4k "end of page" hardware bug and does the needy.
 */
void __update_tlb(struct vm_area_struct * vma, unsigned long address, pte_t pte)
{
	unsigned long flags;
	pgd_t *pgdp;
	pmd_t *pmdp;
	pte_t *ptep;
	int pid;

	/*
	 * Handle debugger faulting in for debugee.
	 */
	if (current->active_mm != vma->vm_mm)
		return;

	pid = read_c0_entryhi() & ASID_MASK;

	local_irq_save(flags);
	address &= PAGE_MASK;
	write_c0_vaddr(address);
	write_c0_entryhi(pid);
	pgdp = pgd_offset(vma->vm_mm, address);
	pmdp = pmd_offset(pgdp, address);
	ptep = pte_offset_map(pmdp, address);
	tlb_probe();

	write_c0_entrylo(pte_val(*ptep++) >> 6);
	tlb_write();

	write_c0_entryhi(pid);
	local_irq_restore(flags);
}

static void __cpuinit probe_tlb(unsigned long config)
{
	struct cpuinfo_mips *c = &current_cpu_data;

	c->tlbsize = 3 * 128;		/* 3 sets each 128 entries */
}

void __cpuinit tlb_init(void)
{
	unsigned int config = read_c0_config();
	unsigned long status;

	probe_tlb(config);

	status = read_c0_status();
	status &= ~(ST0_UPS | ST0_KPS);
#ifdef CONFIG_PAGE_SIZE_4KB
	status |= (TFP_PAGESIZE_4K << 32) | (TFP_PAGESIZE_4K << 36);
#elif defined(CONFIG_PAGE_SIZE_8KB)
	status |= (TFP_PAGESIZE_8K << 32) | (TFP_PAGESIZE_8K << 36);
#elif defined(CONFIG_PAGE_SIZE_16KB)
	status |= (TFP_PAGESIZE_16K << 32) | (TFP_PAGESIZE_16K << 36);
#elif defined(CONFIG_PAGE_SIZE_64KB)
	status |= (TFP_PAGESIZE_64K << 32) | (TFP_PAGESIZE_64K << 36);
#endif
	write_c0_status(status);

	write_c0_wired(0);

	local_flush_tlb_all();

	build_tlb_refill_handler();
}
