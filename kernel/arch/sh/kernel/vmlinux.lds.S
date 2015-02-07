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
 * ld script to make SuperH Linux kernel
 * Written by Niibe Yutaka and Paul Mundt
 */
#ifdef CONFIG_SUPERH64
#define LOAD_OFFSET	PAGE_OFFSET
OUTPUT_ARCH(sh:sh5)
#else
#define LOAD_OFFSET	0
OUTPUT_ARCH(sh)
#endif

#include <asm/thread_info.h>
#include <asm/cache.h>
#include <asm/vmlinux.lds.h>

#ifdef CONFIG_PMB
 #define MEMORY_OFFSET	0
#else
 #define MEMORY_OFFSET	__MEMORY_START
#endif

ENTRY(_start)
SECTIONS
{
	. = PAGE_OFFSET + MEMORY_OFFSET + CONFIG_ZERO_PAGE_OFFSET;

	_text = .;		/* Text and read-only data */

	.empty_zero_page : AT(ADDR(.empty_zero_page) - LOAD_OFFSET) {
		*(.empty_zero_page)
	} = 0

	.text : AT(ADDR(.text) - LOAD_OFFSET) {
		HEAD_TEXT
		TEXT_TEXT
		EXTRA_TEXT
		SCHED_TEXT
		LOCK_TEXT
		KPROBES_TEXT
		IRQENTRY_TEXT
		*(.fixup)
		*(.gnu.warning)
		_etext = .;		/* End of text section */
	} = 0x0009

	EXCEPTION_TABLE(16)
	NOTES

	_sdata = .;
	RO_DATA(PAGE_SIZE)
	RW_DATA_SECTION(L1_CACHE_BYTES, PAGE_SIZE, THREAD_SIZE)
	_edata = .;

	DWARF_EH_FRAME

	. = ALIGN(PAGE_SIZE);		/* Init code and data */
	__init_begin = .;
	INIT_TEXT_SECTION(PAGE_SIZE)
	INIT_DATA_SECTION(16)

	. = ALIGN(4);
	.machvec.init : AT(ADDR(.machvec.init) - LOAD_OFFSET) {
		__machvec_start = .;
		*(.machvec.init)
		__machvec_end = .;
	}

	PERCPU(PAGE_SIZE)

	/*
	 * .exit.text is discarded at runtime, not link time, to deal with
	 * references from __bug_table
	 */
	.exit.text : AT(ADDR(.exit.text) - LOAD_OFFSET) { EXIT_TEXT }
	.exit.data : AT(ADDR(.exit.data) - LOAD_OFFSET) { EXIT_DATA }

	. = ALIGN(PAGE_SIZE);
	__init_end = .;
	BSS_SECTION(0, PAGE_SIZE, 4)
	_ebss = .;			/* uClinux MTD sucks */
	_end = . ;

	STABS_DEBUG
	DWARF_DEBUG

	DISCARDS
}
