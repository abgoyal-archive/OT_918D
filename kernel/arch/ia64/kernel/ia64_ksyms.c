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
 * Architecture-specific kernel symbols
 *
 * Don't put any exports here unless it's defined in an assembler file.
 * All other exports should be put directly after the definition.
 */

#include <linux/module.h>

#include <linux/string.h>
EXPORT_SYMBOL(memset);
EXPORT_SYMBOL(memcpy);
EXPORT_SYMBOL(strlen);

#include<asm/pgtable.h>
EXPORT_SYMBOL_GPL(empty_zero_page);

#include <asm/checksum.h>
EXPORT_SYMBOL(ip_fast_csum);		/* hand-coded assembly */
EXPORT_SYMBOL(csum_ipv6_magic);

#include <asm/page.h>
EXPORT_SYMBOL(clear_page);
EXPORT_SYMBOL(copy_page);

#ifdef CONFIG_VIRTUAL_MEM_MAP
#include <linux/bootmem.h>
EXPORT_SYMBOL(min_low_pfn);	/* defined by bootmem.c, but not exported by generic code */
EXPORT_SYMBOL(max_low_pfn);	/* defined by bootmem.c, but not exported by generic code */
#endif

#include <asm/processor.h>
EXPORT_SYMBOL(ia64_cpu_info);
#ifdef CONFIG_SMP
EXPORT_SYMBOL(local_per_cpu_offset);
#endif

#include <asm/uaccess.h>
EXPORT_SYMBOL(__copy_user);
EXPORT_SYMBOL(__do_clear_user);
EXPORT_SYMBOL(__strlen_user);
EXPORT_SYMBOL(__strncpy_from_user);
EXPORT_SYMBOL(__strnlen_user);

/* from arch/ia64/lib */
extern void __divsi3(void);
extern void __udivsi3(void);
extern void __modsi3(void);
extern void __umodsi3(void);
extern void __divdi3(void);
extern void __udivdi3(void);
extern void __moddi3(void);
extern void __umoddi3(void);

EXPORT_SYMBOL(__divsi3);
EXPORT_SYMBOL(__udivsi3);
EXPORT_SYMBOL(__modsi3);
EXPORT_SYMBOL(__umodsi3);
EXPORT_SYMBOL(__divdi3);
EXPORT_SYMBOL(__udivdi3);
EXPORT_SYMBOL(__moddi3);
EXPORT_SYMBOL(__umoddi3);

#if defined(CONFIG_MD_RAID456) || defined(CONFIG_MD_RAID456_MODULE)
extern void xor_ia64_2(void);
extern void xor_ia64_3(void);
extern void xor_ia64_4(void);
extern void xor_ia64_5(void);

EXPORT_SYMBOL(xor_ia64_2);
EXPORT_SYMBOL(xor_ia64_3);
EXPORT_SYMBOL(xor_ia64_4);
EXPORT_SYMBOL(xor_ia64_5);
#endif

#include <asm/pal.h>
EXPORT_SYMBOL(ia64_pal_call_phys_stacked);
EXPORT_SYMBOL(ia64_pal_call_phys_static);
EXPORT_SYMBOL(ia64_pal_call_stacked);
EXPORT_SYMBOL(ia64_pal_call_static);
EXPORT_SYMBOL(ia64_load_scratch_fpregs);
EXPORT_SYMBOL(ia64_save_scratch_fpregs);

#include <asm/unwind.h>
EXPORT_SYMBOL(unw_init_running);

#if defined(CONFIG_IA64_ESI) || defined(CONFIG_IA64_ESI_MODULE)
extern void esi_call_phys (void);
EXPORT_SYMBOL_GPL(esi_call_phys);
#endif
extern char ia64_ivt[];
EXPORT_SYMBOL(ia64_ivt);

#include <asm/ftrace.h>
#ifdef CONFIG_FUNCTION_TRACER
/* mcount is defined in assembly */
EXPORT_SYMBOL(_mcount);
#endif
