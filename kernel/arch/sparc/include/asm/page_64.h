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

#ifndef _SPARC64_PAGE_H
#define _SPARC64_PAGE_H

#include <linux/const.h>

#if defined(CONFIG_SPARC64_PAGE_SIZE_8KB)
#define PAGE_SHIFT   13
#elif defined(CONFIG_SPARC64_PAGE_SIZE_64KB)
#define PAGE_SHIFT   16
#else
#error No page size specified in kernel configuration
#endif

#define PAGE_SIZE    (_AC(1,UL) << PAGE_SHIFT)
#define PAGE_MASK    (~(PAGE_SIZE-1))

/* Flushing for D-cache alias handling is only needed if
 * the page size is smaller than 16K.
 */
#if PAGE_SHIFT < 14
#define DCACHE_ALIASING_POSSIBLE
#endif

#if defined(CONFIG_HUGETLB_PAGE_SIZE_4MB)
#define HPAGE_SHIFT		22
#elif defined(CONFIG_HUGETLB_PAGE_SIZE_512K)
#define HPAGE_SHIFT		19
#elif defined(CONFIG_HUGETLB_PAGE_SIZE_64K)
#define HPAGE_SHIFT		16
#endif

#ifdef CONFIG_HUGETLB_PAGE
#define HPAGE_SIZE		(_AC(1,UL) << HPAGE_SHIFT)
#define HPAGE_MASK		(~(HPAGE_SIZE - 1UL))
#define HUGETLB_PAGE_ORDER	(HPAGE_SHIFT - PAGE_SHIFT)
#define HAVE_ARCH_HUGETLB_UNMAPPED_AREA
#endif

#ifndef __ASSEMBLY__

#define WANT_PAGE_VIRTUAL

extern void _clear_page(void *page);
#define clear_page(X)	_clear_page((void *)(X))
struct page;
extern void clear_user_page(void *addr, unsigned long vaddr, struct page *page);
#define copy_page(X,Y)	memcpy((void *)(X), (void *)(Y), PAGE_SIZE)
extern void copy_user_page(void *to, void *from, unsigned long vaddr, struct page *topage);

/* Unlike sparc32, sparc64's parameter passing API is more
 * sane in that structures which as small enough are passed
 * in registers instead of on the stack.  Thus, setting
 * STRICT_MM_TYPECHECKS does not generate worse code so
 * let's enable it to get the type checking.
 */

#define STRICT_MM_TYPECHECKS

#ifdef STRICT_MM_TYPECHECKS
/* These are used to make use of C type-checking.. */
typedef struct { unsigned long pte; } pte_t;
typedef struct { unsigned long iopte; } iopte_t;
typedef struct { unsigned int pmd; } pmd_t;
typedef struct { unsigned int pgd; } pgd_t;
typedef struct { unsigned long pgprot; } pgprot_t;

#define pte_val(x)	((x).pte)
#define iopte_val(x)	((x).iopte)
#define pmd_val(x)      ((x).pmd)
#define pgd_val(x)	((x).pgd)
#define pgprot_val(x)	((x).pgprot)

#define __pte(x)	((pte_t) { (x) } )
#define __iopte(x)	((iopte_t) { (x) } )
#define __pmd(x)        ((pmd_t) { (x) } )
#define __pgd(x)	((pgd_t) { (x) } )
#define __pgprot(x)	((pgprot_t) { (x) } )

#else
/* .. while these make it easier on the compiler */
typedef unsigned long pte_t;
typedef unsigned long iopte_t;
typedef unsigned int pmd_t;
typedef unsigned int pgd_t;
typedef unsigned long pgprot_t;

#define pte_val(x)	(x)
#define iopte_val(x)	(x)
#define pmd_val(x)      (x)
#define pgd_val(x)	(x)
#define pgprot_val(x)	(x)

#define __pte(x)	(x)
#define __iopte(x)	(x)
#define __pmd(x)        (x)
#define __pgd(x)	(x)
#define __pgprot(x)	(x)

#endif /* (STRICT_MM_TYPECHECKS) */

typedef struct page *pgtable_t;

#define TASK_UNMAPPED_BASE	(test_thread_flag(TIF_32BIT) ? \
				 (_AC(0x0000000070000000,UL)) : \
				 (_AC(0xfffff80000000000,UL) + (1UL << 32UL)))

#include <asm-generic/memory_model.h>

#endif /* !(__ASSEMBLY__) */

/* We used to stick this into a hard-coded global register (%g4)
 * but that does not make sense anymore.
 */
#define PAGE_OFFSET		_AC(0xFFFFF80000000000,UL)

#ifndef __ASSEMBLY__

#define __pa(x)			((unsigned long)(x) - PAGE_OFFSET)
#define __va(x)			((void *)((unsigned long) (x) + PAGE_OFFSET))

#define pfn_to_kaddr(pfn)	__va((pfn) << PAGE_SHIFT)

#define virt_to_page(kaddr)	pfn_to_page(__pa(kaddr)>>PAGE_SHIFT)

#define virt_addr_valid(kaddr)	pfn_valid(__pa(kaddr) >> PAGE_SHIFT)

#define virt_to_phys __pa
#define phys_to_virt __va

#endif /* !(__ASSEMBLY__) */

#define VM_DATA_DEFAULT_FLAGS	(VM_READ | VM_WRITE | VM_EXEC | \
				 VM_MAYREAD | VM_MAYWRITE | VM_MAYEXEC)

#include <asm-generic/getorder.h>

#endif /* _SPARC64_PAGE_H */
