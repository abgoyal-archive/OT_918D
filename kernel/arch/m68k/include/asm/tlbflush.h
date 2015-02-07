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

#ifndef _M68K_TLBFLUSH_H
#define _M68K_TLBFLUSH_H

#ifdef CONFIG_MMU
#ifndef CONFIG_SUN3

#include <asm/current.h>

static inline void flush_tlb_kernel_page(void *addr)
{
	if (CPU_IS_040_OR_060) {
		mm_segment_t old_fs = get_fs();
		set_fs(KERNEL_DS);
		__asm__ __volatile__(".chip 68040\n\t"
				     "pflush (%0)\n\t"
				     ".chip 68k"
				     : : "a" (addr));
		set_fs(old_fs);
	} else if (CPU_IS_020_OR_030)
		__asm__ __volatile__("pflush #4,#4,(%0)" : : "a" (addr));
}

/*
 * flush all user-space atc entries.
 */
static inline void __flush_tlb(void)
{
	if (CPU_IS_040_OR_060)
		__asm__ __volatile__(".chip 68040\n\t"
				     "pflushan\n\t"
				     ".chip 68k");
	else if (CPU_IS_020_OR_030)
		__asm__ __volatile__("pflush #0,#4");
}

static inline void __flush_tlb040_one(unsigned long addr)
{
	__asm__ __volatile__(".chip 68040\n\t"
			     "pflush (%0)\n\t"
			     ".chip 68k"
			     : : "a" (addr));
}

static inline void __flush_tlb_one(unsigned long addr)
{
	if (CPU_IS_040_OR_060)
		__flush_tlb040_one(addr);
	else if (CPU_IS_020_OR_030)
		__asm__ __volatile__("pflush #0,#4,(%0)" : : "a" (addr));
}

#define flush_tlb() __flush_tlb()

/*
 * flush all atc entries (both kernel and user-space entries).
 */
static inline void flush_tlb_all(void)
{
	if (CPU_IS_040_OR_060)
		__asm__ __volatile__(".chip 68040\n\t"
				     "pflusha\n\t"
				     ".chip 68k");
	else if (CPU_IS_020_OR_030)
		__asm__ __volatile__("pflusha");
}

static inline void flush_tlb_mm(struct mm_struct *mm)
{
	if (mm == current->active_mm)
		__flush_tlb();
}

static inline void flush_tlb_page(struct vm_area_struct *vma, unsigned long addr)
{
	if (vma->vm_mm == current->active_mm) {
		mm_segment_t old_fs = get_fs();
		set_fs(USER_DS);
		__flush_tlb_one(addr);
		set_fs(old_fs);
	}
}

static inline void flush_tlb_range(struct vm_area_struct *vma,
				   unsigned long start, unsigned long end)
{
	if (vma->vm_mm == current->active_mm)
		__flush_tlb();
}

static inline void flush_tlb_kernel_range(unsigned long start, unsigned long end)
{
	flush_tlb_all();
}

#else


/* Reserved PMEGs. */
extern char sun3_reserved_pmeg[SUN3_PMEGS_NUM];
extern unsigned long pmeg_vaddr[SUN3_PMEGS_NUM];
extern unsigned char pmeg_alloc[SUN3_PMEGS_NUM];
extern unsigned char pmeg_ctx[SUN3_PMEGS_NUM];

/* Flush all userspace mappings one by one...  (why no flush command,
   sun?) */
static inline void flush_tlb_all(void)
{
       unsigned long addr;
       unsigned char ctx, oldctx;

       oldctx = sun3_get_context();
       for(addr = 0x00000000; addr < TASK_SIZE; addr += SUN3_PMEG_SIZE) {
	       for(ctx = 0; ctx < 8; ctx++) {
		       sun3_put_context(ctx);
		       sun3_put_segmap(addr, SUN3_INVALID_PMEG);
	       }
       }

       sun3_put_context(oldctx);
       /* erase all of the userspace pmeg maps, we've clobbered them
	  all anyway */
       for(addr = 0; addr < SUN3_INVALID_PMEG; addr++) {
	       if(pmeg_alloc[addr] == 1) {
		       pmeg_alloc[addr] = 0;
		       pmeg_ctx[addr] = 0;
		       pmeg_vaddr[addr] = 0;
	       }
       }

}

/* Clear user TLB entries within the context named in mm */
static inline void flush_tlb_mm (struct mm_struct *mm)
{
     unsigned char oldctx;
     unsigned char seg;
     unsigned long i;

     oldctx = sun3_get_context();
     sun3_put_context(mm->context);

     for(i = 0; i < TASK_SIZE; i += SUN3_PMEG_SIZE) {
	     seg = sun3_get_segmap(i);
	     if(seg == SUN3_INVALID_PMEG)
		     continue;

	     sun3_put_segmap(i, SUN3_INVALID_PMEG);
	     pmeg_alloc[seg] = 0;
	     pmeg_ctx[seg] = 0;
	     pmeg_vaddr[seg] = 0;
     }

     sun3_put_context(oldctx);

}

/* Flush a single TLB page. In this case, we're limited to flushing a
   single PMEG */
static inline void flush_tlb_page (struct vm_area_struct *vma,
				   unsigned long addr)
{
	unsigned char oldctx;
	unsigned char i;

	oldctx = sun3_get_context();
	sun3_put_context(vma->vm_mm->context);
	addr &= ~SUN3_PMEG_MASK;
	if((i = sun3_get_segmap(addr)) != SUN3_INVALID_PMEG)
	{
		pmeg_alloc[i] = 0;
		pmeg_ctx[i] = 0;
		pmeg_vaddr[i] = 0;
		sun3_put_segmap (addr,  SUN3_INVALID_PMEG);
	}
	sun3_put_context(oldctx);

}
/* Flush a range of pages from TLB. */

static inline void flush_tlb_range (struct vm_area_struct *vma,
		      unsigned long start, unsigned long end)
{
	struct mm_struct *mm = vma->vm_mm;
	unsigned char seg, oldctx;

	start &= ~SUN3_PMEG_MASK;

	oldctx = sun3_get_context();
	sun3_put_context(mm->context);

	while(start < end)
	{
		if((seg = sun3_get_segmap(start)) == SUN3_INVALID_PMEG)
		     goto next;
		if(pmeg_ctx[seg] == mm->context) {
			pmeg_alloc[seg] = 0;
			pmeg_ctx[seg] = 0;
			pmeg_vaddr[seg] = 0;
		}
		sun3_put_segmap(start, SUN3_INVALID_PMEG);
	next:
		start += SUN3_PMEG_SIZE;
	}
}

static inline void flush_tlb_kernel_range(unsigned long start, unsigned long end)
{
	flush_tlb_all();
}

/* Flush kernel page from TLB. */
static inline void flush_tlb_kernel_page (unsigned long addr)
{
	sun3_put_segmap (addr & ~(SUN3_PMEG_SIZE - 1), SUN3_INVALID_PMEG);
}

#endif

#else /* !CONFIG_MMU */

/*
 * flush all user-space atc entries.
 */
static inline void __flush_tlb(void)
{
	BUG();
}

static inline void __flush_tlb_one(unsigned long addr)
{
	BUG();
}

#define flush_tlb() __flush_tlb()

/*
 * flush all atc entries (both kernel and user-space entries).
 */
static inline void flush_tlb_all(void)
{
	BUG();
}

static inline void flush_tlb_mm(struct mm_struct *mm)
{
	BUG();
}

static inline void flush_tlb_page(struct vm_area_struct *vma, unsigned long addr)
{
	BUG();
}

static inline void flush_tlb_range(struct mm_struct *mm,
				   unsigned long start, unsigned long end)
{
	BUG();
}

static inline void flush_tlb_kernel_page(unsigned long addr)
{
	BUG();
}

#endif /* CONFIG_MMU */

#endif /* _M68K_TLBFLUSH_H */
