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

/* ld script to make the Linux/CRIS kernel
 * Authors: Bjorn Wesen (bjornw@axis.com)
 *
 * It is VERY DANGEROUS to fiddle around with the symbols in this
 * script. It is for example quite vital that all generated sections
 * that are used are actually named here, otherwise the linker will
 * put them at the end, where the init stuff is which is FREED after
 * the kernel has booted.
 */

#include <asm-generic/vmlinux.lds.h>
#include <asm/page.h>

#ifdef CONFIG_ETRAX_VMEM_SIZE
#define __CONFIG_ETRAX_VMEM_SIZE CONFIG_ETRAX_VMEM_SIZE
#else
#define __CONFIG_ETRAX_VMEM_SIZE 0
#endif


jiffies = jiffies_64;
SECTIONS
{
	. = DRAM_VIRTUAL_BASE;
	dram_start = .;
#ifdef CONFIG_ETRAX_ARCH_V10
	ibr_start = .;
#else
	ebp_start = .;
	/* The boot section is only necessary until the VCS top */
	/* level testbench includes both flash and DRAM. */
	.boot : { *(.boot) }
#endif

	/* see head.S and pages reserved at the start */
	. = DRAM_VIRTUAL_BASE + 0x4000;

	_text = .;			/* Text and read-only data. */
	text_start = .;			/* Lots of aliases. */
	_stext = .;
	__stext = .;
	.text : {
		TEXT_TEXT
		SCHED_TEXT
		LOCK_TEXT
		*(.fixup)
		*(.text.__*)
	}

	_etext = . ;			/* End of text section. */
	__etext = .;

	EXCEPTION_TABLE(4)

	RODATA

	. = ALIGN (4);
	___data_start = . ;
	__Sdata = . ;
	.data : {			/* Data */
		DATA_DATA
	}
	__edata = . ;			/* End of data section. */
	_edata = . ;

	INIT_TASK_DATA_SECTION(PAGE_SIZE)

	. = ALIGN(PAGE_SIZE);		/* Init code and data. */
	__init_begin = .;
	INIT_TEXT_SECTION(PAGE_SIZE)
	.init.data : { INIT_DATA }
	.init.setup : { INIT_SETUP(16) }
#ifdef CONFIG_ETRAX_ARCH_V32
	__start___param = .;
	__param : { *(__param) }
	__stop___param = .;
#endif
	.initcall.init : {
		INIT_CALLS
	}

	.con_initcall.init : {
		CON_INITCALL
	}
	SECURITY_INIT

#ifdef CONFIG_ETRAX_ARCH_V10
#ifdef CONFIG_BLK_DEV_INITRD
	.init.ramfs : {
		__initramfs_start = .;
		*(.init.ramfs)
		__initramfs_end = .;
	}
#endif
#endif
	__vmlinux_end = .;		/* Last address of the physical file. */
#ifdef CONFIG_ETRAX_ARCH_V32
	PERCPU(PAGE_SIZE)

	.init.ramfs : {
		INIT_RAM_FS
	}
#endif

	/*
	 * We fill to the next page, so we can discard all init
	 * pages without needing to consider what payload might be
	 * appended to the kernel image.
	 */
	. = ALIGN(PAGE_SIZE);

	__init_end = .;

	__data_end = . ;		/* Move to _edata ? */
	BSS_SECTION(0, 0, 0)

	. =  ALIGN (0x20);
	_end = .;
	__end = .;

	dram_end = dram_start + (CONFIG_ETRAX_DRAM_SIZE - __CONFIG_ETRAX_VMEM_SIZE)*1024*1024;

	DISCARDS
}
