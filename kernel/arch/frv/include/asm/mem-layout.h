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

/* mem-layout.h: memory layout
 *
 * Copyright (C) 2004 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */

#ifndef _ASM_MEM_LAYOUT_H
#define _ASM_MEM_LAYOUT_H

#ifndef __ASSEMBLY__
#define __UL(X)	((unsigned long) (X))
#else
#define __UL(X)	(X)
#endif

/*
 * PAGE_SHIFT determines the page size
 */
#define PAGE_SHIFT			14

#ifndef __ASSEMBLY__
#define PAGE_SIZE			(1UL << PAGE_SHIFT)
#else
#define PAGE_SIZE			(1 << PAGE_SHIFT)
#endif

#define PAGE_MASK			(~(PAGE_SIZE-1))

/*
 * the slab must be aligned such that load- and store-double instructions don't
 * fault if used
 */
#define	ARCH_KMALLOC_MINALIGN		L1_CACHE_BYTES
#define	ARCH_SLAB_MINALIGN		L1_CACHE_BYTES

/*****************************************************************************/
/*
 * virtual memory layout from kernel's point of view
 */
#define PAGE_OFFSET			((unsigned long) &__page_offset)

#ifdef CONFIG_MMU

/* see Documentation/frv/mmu-layout.txt */
#define KERNEL_LOWMEM_START		__UL(0xc0000000)
#define KERNEL_LOWMEM_END		__UL(0xd0000000)
#define VMALLOC_START			__UL(0xd0000000)
#define VMALLOC_END			__UL(0xd8000000)
#define PKMAP_BASE			__UL(0xd8000000)
#define PKMAP_END			__UL(0xdc000000)
#define KMAP_ATOMIC_SECONDARY_FRAME	__UL(0xdc000000)
#define KMAP_ATOMIC_PRIMARY_FRAME	__UL(0xdd000000)

#endif

#define KERNEL_IO_START			__UL(0xe0000000)


/*****************************************************************************/
/*
 * memory layout from userspace's point of view
 */
#define BRK_BASE			__UL(2 * 1024 * 1024 + PAGE_SIZE)
#define STACK_TOP			__UL(2 * 1024 * 1024)
#define STACK_TOP_MAX			__UL(0xc0000000)

/* userspace process size */
#ifdef CONFIG_MMU
#define TASK_SIZE			(PAGE_OFFSET)
#else
#define TASK_SIZE			__UL(0xFFFFFFFFUL)
#endif

/* base of area at which unspecified mmaps will start */
#ifdef CONFIG_BINFMT_ELF_FDPIC
#define TASK_UNMAPPED_BASE		__UL(16 * 1024 * 1024)
#else
#define TASK_UNMAPPED_BASE		__UL(TASK_SIZE / 3)
#endif

#endif /* _ASM_MEM_LAYOUT_H */
