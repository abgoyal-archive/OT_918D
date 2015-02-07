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
 * Suspend support specific for s390.
 *
 * Copyright IBM Corp. 2009
 *
 * Author(s): Hans-Joachim Picht <hans@linux.vnet.ibm.com>
 */

#include <linux/pfn.h>
#include <asm/system.h>

/*
 * References to section boundaries
 */
extern const void __nosave_begin, __nosave_end;

int pfn_is_nosave(unsigned long pfn)
{
	unsigned long nosave_begin_pfn = PFN_DOWN(__pa(&__nosave_begin));
	unsigned long nosave_end_pfn = PFN_DOWN(__pa(&__nosave_end));

	/* Always save lowcore pages (LC protection might be enabled). */
	if (pfn <= LC_PAGES)
		return 0;
	if (pfn >= nosave_begin_pfn && pfn < nosave_end_pfn)
		return 1;
	/* Skip memory holes and read-only pages (NSS, DCSS, ...). */
	if (tprot(PFN_PHYS(pfn)))
		return 1;
	return 0;
}

void save_processor_state(void)
{
	/* swsusp_arch_suspend() actually saves all cpu register contents.
	 * Machine checks must be disabled since swsusp_arch_suspend() stores
	 * register contents to their lowcore save areas. That's the same
	 * place where register contents on machine checks would be saved.
	 * To avoid register corruption disable machine checks.
	 * We must also disable machine checks in the new psw mask for
	 * program checks, since swsusp_arch_suspend() may generate program
	 * checks. Disabling machine checks for all other new psw masks is
	 * just paranoia.
	 */
	local_mcck_disable();
	/* Disable lowcore protection */
	__ctl_clear_bit(0,28);
	S390_lowcore.external_new_psw.mask &= ~PSW_MASK_MCHECK;
	S390_lowcore.svc_new_psw.mask &= ~PSW_MASK_MCHECK;
	S390_lowcore.io_new_psw.mask &= ~PSW_MASK_MCHECK;
	S390_lowcore.program_new_psw.mask &= ~PSW_MASK_MCHECK;
}

void restore_processor_state(void)
{
	S390_lowcore.external_new_psw.mask |= PSW_MASK_MCHECK;
	S390_lowcore.svc_new_psw.mask |= PSW_MASK_MCHECK;
	S390_lowcore.io_new_psw.mask |= PSW_MASK_MCHECK;
	S390_lowcore.program_new_psw.mask |= PSW_MASK_MCHECK;
	/* Enable lowcore protection */
	__ctl_set_bit(0,28);
	local_mcck_enable();
}
