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
 * Copyright (C) 2009  Cisco Systems, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef __ASM_MACH_POWERTV_ASIC_H_
#define __ASM_MACH_POWERTV_ASIC_H_
#include <linux/io.h>

/* ASIC types */
enum asic_type {
	ASIC_UNKNOWN,
	ASIC_ZEUS,
	ASIC_CALLIOPE,
	ASIC_CRONUS,
	ASIC_CRONUSLITE,
	ASICS
};

/* hardcoded values read from Chip Version registers */
#define CRONUS_10	0x0B4C1C20
#define CRONUS_11	0x0B4C1C21
#define CRONUSLITE_10	0x0B4C1C40

#define NAND_FLASH_BASE		0x03000000
#define CALLIOPE_IO_BASE	0x08000000
#define CRONUS_IO_BASE		0x09000000
#define ZEUS_IO_BASE		0x09000000

#define ASIC_IO_SIZE		0x01000000

/* Definitions for backward compatibility */
#define UART1_INTSTAT	uart1_intstat
#define UART1_INTEN	uart1_inten
#define UART1_CONFIG1	uart1_config1
#define UART1_CONFIG2	uart1_config2
#define UART1_DIVISORHI	uart1_divisorhi
#define UART1_DIVISORLO	uart1_divisorlo
#define UART1_DATA	uart1_data
#define UART1_STATUS	uart1_status

/* ASIC register enumeration */
union register_map_entry {
	unsigned long phys;
	u32 *virt;
};

#define REGISTER_MAP_ELEMENT(x) union register_map_entry x;
struct register_map {
#include <asm/mach-powertv/asic_reg_map.h>
};
#undef REGISTER_MAP_ELEMENT

/**
 * register_map_offset_phys - add an offset to the physical address
 * @map:	Pointer to the &struct register_map
 * @offset:	Value to add
 *
 * Only adds the base to non-zero physical addresses
 */
static inline void register_map_offset_phys(struct register_map *map,
	unsigned long offset)
{
#define REGISTER_MAP_ELEMENT(x)		do {				\
		if (map->x.phys != 0)					\
			map->x.phys += offset;				\
	} while (false);

#include <asm/mach-powertv/asic_reg_map.h>
#undef REGISTER_MAP_ELEMENT
}

/**
 * register_map_virtualize - Convert &register_map to virtual addresses
 * @map:	Pointer to &register_map to virtualize
 */
static inline void register_map_virtualize(struct register_map *map)
{
#define REGISTER_MAP_ELEMENT(x)		do {				\
		map->x.virt = (!map->x.phys) ? NULL :			\
			UNCAC_ADDR(phys_to_virt(map->x.phys));		\
	} while (false);

#include <asm/mach-powertv/asic_reg_map.h>
#undef REGISTER_MAP_ELEMENT
}

extern struct register_map _asic_register_map;

/*
 * Macros to interface to registers through their ioremapped address
 * asic_reg_phys_addr	Returns the physical address of the given register
 * asic_reg_addr	Returns the iomapped virtual address of the given
 *			register.
 */
#define asic_reg_addr(x)	(_asic_register_map.x.virt)
#define asic_reg_phys_addr(x)	(virt_to_phys((void *) CAC_ADDR(	\
					(unsigned long) asic_reg_addr(x))))

/*
 * The asic_reg macro is gone. It should be replaced by either asic_read or
 * asic_write, as appropriate.
 */

#define asic_read(x)		readl(asic_reg_addr(x))
#define asic_write(v, x)	writel(v, asic_reg_addr(x))

extern void asic_irq_init(void);
#endif
