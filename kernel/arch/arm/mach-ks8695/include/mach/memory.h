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
 * arch/arm/mach-ks8695/include/mach/memory.h
 *
 * Copyright (C) 2006 Andrew Victor
 *
 * KS8695 Memory definitions
 *
 * This file is licensed under  the terms of the GNU General Public
 * License version 2. This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */

#ifndef __ASM_ARCH_MEMORY_H
#define __ASM_ARCH_MEMORY_H

#include <mach/hardware.h>

/*
 * Physical SRAM offset.
 */
#define PHYS_OFFSET		KS8695_SDRAM_PA

#ifndef __ASSEMBLY__

#ifdef CONFIG_PCI

/* PCI mappings */
#define __virt_to_bus(x)	((x) - PAGE_OFFSET + KS8695_PCIMEM_PA)
#define __bus_to_virt(x)	((x) - KS8695_PCIMEM_PA + PAGE_OFFSET)

/* Platform-bus mapping */
extern struct bus_type platform_bus_type;
#define is_lbus_device(dev)		(dev && dev->bus == &platform_bus_type)
#define __arch_dma_to_virt(dev, x)	({ (void *) (is_lbus_device(dev) ? \
					__phys_to_virt(x) : __bus_to_virt(x)); })
#define __arch_virt_to_dma(dev, x)	({ is_lbus_device(dev) ? \
					(dma_addr_t)__virt_to_phys(x) : (dma_addr_t)__virt_to_bus(x); })
#define __arch_page_to_dma(dev, x)	\
	({ dma_addr_t __dma = page_to_phys(page); \
	   if (!is_lbus_device(dev)) \
		__dma = __dma - PHYS_OFFSET + KS8695_PCIMEM_PA; \
	   __dma; })

#define __arch_dma_to_page(dev, x)	\
	({ dma_addr_t __dma = x;				\
	   if (!is_lbus_device(dev))				\
		__dma += PHYS_OFFSET - KS8695_PCIMEM_PA;	\
	   phys_to_page(__dma);					\
	})

#endif

#endif

#endif
