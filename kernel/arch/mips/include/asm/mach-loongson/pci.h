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
 * Copyright (c) 2008 Zhang Le <r0bertz@gentoo.org>
 * Copyright (c) 2009 Wu Zhangjin <wuzhangjin@gmail.com>
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at your
 * option) any later version.
 */

#ifndef __ASM_MACH_LOONGSON_PCI_H_
#define __ASM_MACH_LOONGSON_PCI_H_

extern struct pci_ops loongson_pci_ops;

/* this is an offset from mips_io_port_base */
#define LOONGSON_PCI_IO_START	0x00004000UL

#ifdef CONFIG_CPU_SUPPORTS_ADDRWINCFG

/*
 * we use address window2 to map cpu address space to pci space
 * window2: cpu [1G, 2G] -> pci [1G, 2G]
 * why not use window 0 & 1? because they are used by cpu when booting.
 * window0: cpu [0, 256M] -> ddr [0, 256M]
 * window1: cpu [256M, 512M] -> pci [256M, 512M]
 */

/* the smallest LOONGSON_CPU_MEM_SRC can be 512M */
#define LOONGSON_CPU_MEM_SRC	0x40000000ul		/* 1G */
#define LOONGSON_PCI_MEM_DST	LOONGSON_CPU_MEM_SRC

#define LOONGSON_PCI_MEM_START	LOONGSON_PCI_MEM_DST
#define LOONGSON_PCI_MEM_END	(0x80000000ul-1)	/* 2G */

#define MMAP_CPUTOPCI_SIZE	(LOONGSON_PCI_MEM_END - \
					LOONGSON_PCI_MEM_START + 1)

#else	/* loongson2f/32bit & loongson2e */

/* this pci memory space is mapped by pcimap in pci.c */
#define LOONGSON_PCI_MEM_START	LOONGSON_PCILO1_BASE
#define LOONGSON_PCI_MEM_END	(LOONGSON_PCILO1_BASE + 0x04000000 * 2)
/* this is an offset from mips_io_port_base */
#define LOONGSON_PCI_IO_START	0x00004000UL

#endif	/* !CONFIG_CPU_SUPPORTS_ADDRWINCFG */

#endif /* !__ASM_MACH_LOONGSON_PCI_H_ */
