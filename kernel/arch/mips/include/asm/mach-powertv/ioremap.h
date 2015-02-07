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
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License
 *	as published by the Free Software Foundation; either version
 *	2 of the License, or (at your option) any later version.
 *
 * Portions Copyright (C)  Cisco Systems, Inc.
 */
#ifndef __ASM_MACH_POWERTV_IOREMAP_H
#define __ASM_MACH_POWERTV_IOREMAP_H

#include <linux/types.h>

#define LOW_MEM_BOUNDARY_PHYS	0x20000000
#define LOW_MEM_BOUNDARY_MASK	(~(LOW_MEM_BOUNDARY_PHYS - 1))

/*
 * The bus addresses are different than the physical addresses that
 * the processor sees by an offset. This offset varies by ASIC
 * version. Define a variable to hold the offset and some macros to
 * make the conversion simpler. */
extern unsigned long phys_to_bus_offset;

#ifdef CONFIG_HIGHMEM
#define MEM_GAP_PHYS		0x60000000
/*
 * TODO: We will use the hard code for conversion between physical and
 * bus until the bootloader releases their device tree to us.
 */
#define phys_to_bus(x) (((x) < LOW_MEM_BOUNDARY_PHYS) ? \
	((x) + phys_to_bus_offset) : (x))
#define bus_to_phys(x) (((x) < MEM_GAP_PHYS_ADDR) ? \
	((x) - phys_to_bus_offset) : (x))
#else
#define phys_to_bus(x) ((x) + phys_to_bus_offset)
#define bus_to_phys(x) ((x) - phys_to_bus_offset)
#endif

/*
 * Determine whether the address we are given is for an ASIC device
 * Params:  addr    Address to check
 * Returns: Zero if the address is not for ASIC devices, non-zero
 *      if it is.
 */
static inline int asic_is_device_addr(phys_t addr)
{
	return !((phys_t)addr & (phys_t) LOW_MEM_BOUNDARY_MASK);
}

/*
 * Determine whether the address we are given is external RAM mappable
 * into KSEG1.
 * Params:  addr    Address to check
 * Returns: Zero if the address is not for external RAM and
 */
static inline int asic_is_lowmem_ram_addr(phys_t addr)
{
	/*
	 * The RAM always starts at the following address in the processor's
	 * physical address space
	 */
	static const phys_t phys_ram_base = 0x10000000;
	phys_t bus_ram_base;

	bus_ram_base = phys_to_bus_offset + phys_ram_base;

	return addr >= bus_ram_base &&
		addr < (bus_ram_base + (LOW_MEM_BOUNDARY_PHYS - phys_ram_base));
}

/*
 * Allow physical addresses to be fixed up to help peripherals located
 * outside the low 32-bit range -- generic pass-through version.
 */
static inline phys_t fixup_bigphys_addr(phys_t phys_addr, phys_t size)
{
	return phys_addr;
}

static inline void __iomem *plat_ioremap(phys_t offset, unsigned long size,
	unsigned long flags)
{
	return NULL;
}

static inline int plat_iounmap(const volatile void __iomem *addr)
{
	return 0;
}
#endif /* __ASM_MACH_POWERTV_IOREMAP_H */
