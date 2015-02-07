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

#ifndef __MMU_H
#define __MMU_H

/*
 * Privileged Space Mapping Buffer (PMB) definitions
 */
#define PMB_PASCR		0xff000070
#define PMB_IRMCR		0xff000078

#define PASCR_SE		0x80000000

#define PMB_ADDR		0xf6100000
#define PMB_DATA		0xf7100000

#define NR_PMB_ENTRIES		16

#define PMB_E_MASK		0x0000000f
#define PMB_E_SHIFT		8

#define PMB_PFN_MASK		0xff000000

#define PMB_SZ_16M		0x00000000
#define PMB_SZ_64M		0x00000010
#define PMB_SZ_128M		0x00000080
#define PMB_SZ_512M		0x00000090
#define PMB_SZ_MASK		PMB_SZ_512M
#define PMB_C			0x00000008
#define PMB_WT			0x00000001
#define PMB_UB			0x00000200
#define PMB_CACHE_MASK		(PMB_C | PMB_WT | PMB_UB)
#define PMB_V			0x00000100

#define PMB_NO_ENTRY		(-1)

#ifndef __ASSEMBLY__
#include <linux/errno.h>
#include <linux/threads.h>
#include <asm/page.h>

/* Default "unsigned long" context */
typedef unsigned long mm_context_id_t[NR_CPUS];

typedef struct {
#ifdef CONFIG_MMU
	mm_context_id_t		id;
	void			*vdso;
#else
	unsigned long		end_brk;
#endif
#ifdef CONFIG_BINFMT_ELF_FDPIC
	unsigned long		exec_fdpic_loadmap;
	unsigned long		interp_fdpic_loadmap;
#endif
} mm_context_t;

#ifdef CONFIG_PMB
/* arch/sh/mm/pmb.c */
bool __in_29bit_mode(void);

void pmb_init(void);
int pmb_bolt_mapping(unsigned long virt, phys_addr_t phys,
		     unsigned long size, pgprot_t prot);
void __iomem *pmb_remap_caller(phys_addr_t phys, unsigned long size,
			       pgprot_t prot, void *caller);
int pmb_unmap(void __iomem *addr);

#else

static inline int
pmb_bolt_mapping(unsigned long virt, phys_addr_t phys,
		 unsigned long size, pgprot_t prot)
{
	return -EINVAL;
}

static inline void __iomem *
pmb_remap_caller(phys_addr_t phys, unsigned long size,
		 pgprot_t prot, void *caller)
{
	return NULL;
}

static inline int pmb_unmap(void __iomem *addr)
{
	return -EINVAL;
}

#define pmb_init(addr)		do { } while (0)

#ifdef CONFIG_29BIT
#define __in_29bit_mode()	(1)
#else
#define __in_29bit_mode()	(0)
#endif

#endif /* CONFIG_PMB */

static inline void __iomem *
pmb_remap(phys_addr_t phys, unsigned long size, pgprot_t prot)
{
	return pmb_remap_caller(phys, size, prot, __builtin_return_address(0));
}

#endif /* __ASSEMBLY__ */

#endif /* __MMU_H */
