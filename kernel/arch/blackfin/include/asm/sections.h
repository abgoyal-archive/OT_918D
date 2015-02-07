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
 * Copyright 2004-2009 Analog Devices Inc.
 *
 * Licensed under the GPL-2 or later.
 */

#ifndef _BLACKFIN_SECTIONS_H
#define _BLACKFIN_SECTIONS_H

/* only used when MTD_UCLINUX */
extern unsigned long memory_mtd_start, memory_mtd_end, mtd_size;

extern unsigned long _ramstart, _ramend, _rambase;
extern unsigned long memory_start, memory_end, physical_mem_end;

/*
 * The weak markings on the lengths might seem weird, but this is required
 * in order to make gcc accept the fact that these may actually have a value
 * of 0 (since they aren't actually addresses, but sizes of sections).
 */
extern char _stext_l1[], _etext_l1[], _text_l1_lma[], __weak _text_l1_len[];
extern char _sdata_l1[], _edata_l1[], _sbss_l1[], _ebss_l1[],
	_data_l1_lma[], __weak _data_l1_len[];
#ifdef CONFIG_ROMKERNEL
extern char _data_lma[], _data_len[], _sinitdata[], _einitdata[], _init_data_lma[], _init_data_len[];
#endif
extern char _sdata_b_l1[], _edata_b_l1[], _sbss_b_l1[], _ebss_b_l1[],
	_data_b_l1_lma[], __weak _data_b_l1_len[];
extern char _stext_l2[], _etext_l2[], _sdata_l2[], _edata_l2[],
	_sbss_l2[], _ebss_l2[], _l2_lma[], __weak _l2_len[];

#include <asm/mem_map.h>

/* Blackfin systems have discontinuous memory map and no virtualized memory */
static inline int arch_is_kernel_text(unsigned long addr)
{
	return
		(L1_CODE_LENGTH &&
		 addr >= (unsigned long)_stext_l1 &&
		 addr <  (unsigned long)_etext_l1)
		||
		(L2_LENGTH &&
		 addr >= (unsigned long)_stext_l2 &&
		 addr <  (unsigned long)_etext_l2);
}
#define arch_is_kernel_text(addr) arch_is_kernel_text(addr)

static inline int arch_is_kernel_data(unsigned long addr)
{
	return
		(L1_DATA_A_LENGTH &&
		 addr >= (unsigned long)_sdata_l1 &&
		 addr <  (unsigned long)_ebss_l1)
		||
		(L1_DATA_B_LENGTH &&
		 addr >= (unsigned long)_sdata_b_l1 &&
		 addr <  (unsigned long)_ebss_b_l1)
		||
		(L2_LENGTH &&
		 addr >= (unsigned long)_sdata_l2 &&
		 addr <  (unsigned long)_ebss_l2);
}
#define arch_is_kernel_data(addr) arch_is_kernel_data(addr)

#include <asm-generic/sections.h>

#endif
