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

#ifndef _ASM_POWERPC_PGALLOC_64_H
#define _ASM_POWERPC_PGALLOC_64_H
/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */

#include <linux/slab.h>
#include <linux/cpumask.h>
#include <linux/percpu.h>

struct vmemmap_backing {
	struct vmemmap_backing *list;
	unsigned long phys;
	unsigned long virt_addr;
};

/*
 * Functions that deal with pagetables that could be at any level of
 * the table need to be passed an "index_size" so they know how to
 * handle allocation.  For PTE pages (which are linked to a struct
 * page for now, and drawn from the main get_free_pages() pool), the
 * allocation size will be (2^index_size * sizeof(pointer)) and
 * allocations are drawn from the kmem_cache in PGT_CACHE(index_size).
 *
 * The maximum index size needs to be big enough to allow any
 * pagetable sizes we need, but small enough to fit in the low bits of
 * any page table pointer.  In other words all pagetables, even tiny
 * ones, must be aligned to allow at least enough low 0 bits to
 * contain this value.  This value is also used as a mask, so it must
 * be one less than a power of two.
 */
#define MAX_PGTABLE_INDEX_SIZE	0xf

extern struct kmem_cache *pgtable_cache[];
#define PGT_CACHE(shift) (pgtable_cache[(shift)-1])

static inline pgd_t *pgd_alloc(struct mm_struct *mm)
{
	return kmem_cache_alloc(PGT_CACHE(PGD_INDEX_SIZE), GFP_KERNEL);
}

static inline void pgd_free(struct mm_struct *mm, pgd_t *pgd)
{
	kmem_cache_free(PGT_CACHE(PGD_INDEX_SIZE), pgd);
}

#ifndef CONFIG_PPC_64K_PAGES

#define pgd_populate(MM, PGD, PUD)	pgd_set(PGD, PUD)

static inline pud_t *pud_alloc_one(struct mm_struct *mm, unsigned long addr)
{
	return kmem_cache_alloc(PGT_CACHE(PUD_INDEX_SIZE),
				GFP_KERNEL|__GFP_REPEAT);
}

static inline void pud_free(struct mm_struct *mm, pud_t *pud)
{
	kmem_cache_free(PGT_CACHE(PUD_INDEX_SIZE), pud);
}

static inline void pud_populate(struct mm_struct *mm, pud_t *pud, pmd_t *pmd)
{
	pud_set(pud, (unsigned long)pmd);
}

#define pmd_populate(mm, pmd, pte_page) \
	pmd_populate_kernel(mm, pmd, page_address(pte_page))
#define pmd_populate_kernel(mm, pmd, pte) pmd_set(pmd, (unsigned long)(pte))
#define pmd_pgtable(pmd) pmd_page(pmd)


#else /* CONFIG_PPC_64K_PAGES */

#define pud_populate(mm, pud, pmd)	pud_set(pud, (unsigned long)pmd)

static inline void pmd_populate_kernel(struct mm_struct *mm, pmd_t *pmd,
				       pte_t *pte)
{
	pmd_set(pmd, (unsigned long)pte);
}

#define pmd_populate(mm, pmd, pte_page) \
	pmd_populate_kernel(mm, pmd, page_address(pte_page))
#define pmd_pgtable(pmd) pmd_page(pmd)

#endif /* CONFIG_PPC_64K_PAGES */

static inline pmd_t *pmd_alloc_one(struct mm_struct *mm, unsigned long addr)
{
	return kmem_cache_alloc(PGT_CACHE(PMD_INDEX_SIZE),
				GFP_KERNEL|__GFP_REPEAT);
}

static inline void pmd_free(struct mm_struct *mm, pmd_t *pmd)
{
	kmem_cache_free(PGT_CACHE(PMD_INDEX_SIZE), pmd);
}

static inline pte_t *pte_alloc_one_kernel(struct mm_struct *mm,
					  unsigned long address)
{
        return (pte_t *)__get_free_page(GFP_KERNEL | __GFP_REPEAT | __GFP_ZERO);
}

static inline pgtable_t pte_alloc_one(struct mm_struct *mm,
					unsigned long address)
{
	struct page *page;
	pte_t *pte;

	pte = pte_alloc_one_kernel(mm, address);
	if (!pte)
		return NULL;
	page = virt_to_page(pte);
	pgtable_page_ctor(page);
	return page;
}

static inline void pgtable_free(void *table, unsigned index_size)
{
	if (!index_size)
		free_page((unsigned long)table);
	else {
		BUG_ON(index_size > MAX_PGTABLE_INDEX_SIZE);
		kmem_cache_free(PGT_CACHE(index_size), table);
	}
}

#define __pmd_free_tlb(tlb, pmd, addr)		      \
	pgtable_free_tlb(tlb, pmd, PMD_INDEX_SIZE)
#ifndef CONFIG_PPC_64K_PAGES
#define __pud_free_tlb(tlb, pud, addr)		      \
	pgtable_free_tlb(tlb, pud, PUD_INDEX_SIZE)

#endif /* CONFIG_PPC_64K_PAGES */

#define check_pgt_cache()	do { } while (0)

#endif /* _ASM_POWERPC_PGALLOC_64_H */
