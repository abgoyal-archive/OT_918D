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
 * Re-map IO memory to kernel address space so that we can access it.
 *
 * These functions should only be used when it is necessary to map a
 * physical address space into the kernel address space before ioremap()
 * can be used, e.g. early in boot before paging_init().
 *
 * Copyright (C) 2009  Matt Fleming
 */

#include <linux/vmalloc.h>
#include <linux/ioport.h>
#include <linux/module.h>
#include <linux/mm.h>
#include <linux/io.h>
#include <linux/bootmem.h>
#include <linux/proc_fs.h>
#include <asm/fixmap.h>
#include <asm/page.h>
#include <asm/pgalloc.h>
#include <asm/addrspace.h>
#include <asm/cacheflush.h>
#include <asm/tlbflush.h>
#include <asm/mmu.h>
#include <asm/mmu_context.h>

struct ioremap_map {
	void __iomem *addr;
	unsigned long size;
	unsigned long fixmap_addr;
};

static struct ioremap_map ioremap_maps[FIX_N_IOREMAPS];

void __init ioremap_fixed_init(void)
{
	struct ioremap_map *map;
	int i;

	for (i = 0; i < FIX_N_IOREMAPS; i++) {
		map = &ioremap_maps[i];
		map->fixmap_addr = __fix_to_virt(FIX_IOREMAP_BEGIN + i);
	}
}

void __init __iomem *
ioremap_fixed(phys_addr_t phys_addr, unsigned long size, pgprot_t prot)
{
	enum fixed_addresses idx0, idx;
	struct ioremap_map *map;
	unsigned int nrpages;
	unsigned long offset;
	int i, slot;

	/*
	 * Mappings have to be page-aligned
	 */
	offset = phys_addr & ~PAGE_MASK;
	phys_addr &= PAGE_MASK;
	size = PAGE_ALIGN(phys_addr + size) - phys_addr;

	slot = -1;
	for (i = 0; i < FIX_N_IOREMAPS; i++) {
		map = &ioremap_maps[i];
		if (!map->addr) {
			map->size = size;
			slot = i;
			break;
		}
	}

	if (slot < 0)
		return NULL;

	/*
	 * Mappings have to fit in the FIX_IOREMAP area.
	 */
	nrpages = size >> PAGE_SHIFT;
	if (nrpages > FIX_N_IOREMAPS)
		return NULL;

	/*
	 * Ok, go for it..
	 */
	idx0 = FIX_IOREMAP_BEGIN + slot;
	idx = idx0;
	while (nrpages > 0) {
		pgprot_val(prot) |= _PAGE_WIRED;
		__set_fixmap(idx, phys_addr, prot);
		phys_addr += PAGE_SIZE;
		idx++;
		--nrpages;
	}

	map->addr = (void __iomem *)(offset + map->fixmap_addr);
	return map->addr;
}

int iounmap_fixed(void __iomem *addr)
{
	enum fixed_addresses idx;
	struct ioremap_map *map;
	unsigned int nrpages;
	int i, slot;

	slot = -1;
	for (i = 0; i < FIX_N_IOREMAPS; i++) {
		map = &ioremap_maps[i];
		if (map->addr == addr) {
			slot = i;
			break;
		}
	}

	/*
	 * If we don't match, it's not for us.
	 */
	if (slot < 0)
		return -EINVAL;

	nrpages = map->size >> PAGE_SHIFT;

	idx = FIX_IOREMAP_BEGIN + slot + nrpages - 1;
	while (nrpages > 0) {
		__clear_fixmap(idx, __pgprot(_PAGE_WIRED));
		--idx;
		--nrpages;
	}

	map->size = 0;
	map->addr = NULL;

	return 0;
}
