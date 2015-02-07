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
 *  arch/arm/mach-realview/include/mach/memory.h
 *
 *  Copyright (C) 2003 ARM Limited
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#ifndef __ASM_ARCH_MEMORY_H
#define __ASM_ARCH_MEMORY_H

/*
 * Physical DRAM offset.
 */
#ifdef CONFIG_REALVIEW_HIGH_PHYS_OFFSET
#define PHYS_OFFSET		UL(0x70000000)
#else
#define PHYS_OFFSET		UL(0x00000000)
#endif

#if !defined(__ASSEMBLY__) && defined(CONFIG_ZONE_DMA)
extern void realview_adjust_zones(int node, unsigned long *size,
				  unsigned long *hole);
#define arch_adjust_zones(node, size, hole) \
	realview_adjust_zones(node, size, hole)

#define ISA_DMA_THRESHOLD	(PHYS_OFFSET + SZ_256M - 1)
#define MAX_DMA_ADDRESS		(PAGE_OFFSET + SZ_256M)
#endif

#ifdef CONFIG_SPARSEMEM

/*
 * Sparsemem definitions for RealView PBX.
 *
 * The RealView PBX board has another block of 512MB of RAM at 0x20000000,
 * however only the block at 0x70000000 (or the 256MB mirror at 0x00000000)
 * may be used for DMA.
 *
 * The macros below define a section size of 256MB and a non-linear virtual to
 * physical mapping:
 *
 * 256MB @ 0x00000000 -> PAGE_OFFSET
 * 512MB @ 0x20000000 -> PAGE_OFFSET + 0x10000000
 * 256MB @ 0x80000000 -> PAGE_OFFSET + 0x30000000
 */
#ifdef CONFIG_REALVIEW_HIGH_PHYS_OFFSET
#error "SPARSEMEM not available with REALVIEW_HIGH_PHYS_OFFSET"
#endif

#define MAX_PHYSMEM_BITS	32
#define SECTION_SIZE_BITS	28

/* bank page offsets */
#define PAGE_OFFSET1	(PAGE_OFFSET + 0x10000000)
#define PAGE_OFFSET2	(PAGE_OFFSET + 0x30000000)

#define __phys_to_virt(phys)						\
	((phys) >= 0x80000000 ?	(phys) - 0x80000000 + PAGE_OFFSET2 :	\
	 (phys) >= 0x20000000 ?	(phys) - 0x20000000 + PAGE_OFFSET1 :	\
	 (phys) + PAGE_OFFSET)

#define __virt_to_phys(virt)						\
	 ((virt) >= PAGE_OFFSET2 ? (virt) - PAGE_OFFSET2 + 0x80000000 :	\
	  (virt) >= PAGE_OFFSET1 ? (virt) - PAGE_OFFSET1 + 0x20000000 :	\
	  (virt) - PAGE_OFFSET)

#endif	/* CONFIG_SPARSEMEM */

#endif
