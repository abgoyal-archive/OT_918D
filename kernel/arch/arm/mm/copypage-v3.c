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
 *  linux/arch/arm/mm/copypage-v3.c
 *
 *  Copyright (C) 1995-1999 Russell King
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/init.h>
#include <linux/highmem.h>

/*
 * ARMv3 optimised copy_user_highpage
 *
 * FIXME: do we need to handle cache stuff...
 */
static void __naked
v3_copy_user_page(void *kto, const void *kfrom)
{
	asm("\n\
	stmfd	sp!, {r4, lr}			@	2\n\
	mov	r2, %2				@	1\n\
	ldmia	%0!, {r3, r4, ip, lr}		@	4+1\n\
1:	stmia	%1!, {r3, r4, ip, lr}		@	4\n\
	ldmia	%0!, {r3, r4, ip, lr}		@	4+1\n\
	stmia	%1!, {r3, r4, ip, lr}		@	4\n\
	ldmia	%0!, {r3, r4, ip, lr}		@	4+1\n\
	stmia	%1!, {r3, r4, ip, lr}		@	4\n\
	ldmia	%0!, {r3, r4, ip, lr}		@	4\n\
	subs	r2, r2, #1			@	1\n\
	stmia	%1!, {r3, r4, ip, lr}		@	4\n\
	ldmneia	%0!, {r3, r4, ip, lr}		@	4\n\
	bne	1b				@	1\n\
	ldmfd	sp!, {r4, pc}			@	3"
	:
	: "r" (kfrom), "r" (kto), "I" (PAGE_SIZE / 64));
}

void v3_copy_user_highpage(struct page *to, struct page *from,
	unsigned long vaddr, struct vm_area_struct *vma)
{
	void *kto, *kfrom;

	kto = kmap_atomic(to, KM_USER0);
	kfrom = kmap_atomic(from, KM_USER1);
	v3_copy_user_page(kto, kfrom);
	kunmap_atomic(kfrom, KM_USER1);
	kunmap_atomic(kto, KM_USER0);
}

/*
 * ARMv3 optimised clear_user_page
 *
 * FIXME: do we need to handle cache stuff...
 */
void v3_clear_user_highpage(struct page *page, unsigned long vaddr)
{
	void *ptr, *kaddr = kmap_atomic(page, KM_USER0);
	asm volatile("\n\
	mov	r1, %2				@ 1\n\
	mov	r2, #0				@ 1\n\
	mov	r3, #0				@ 1\n\
	mov	ip, #0				@ 1\n\
	mov	lr, #0				@ 1\n\
1:	stmia	%0!, {r2, r3, ip, lr}		@ 4\n\
	stmia	%0!, {r2, r3, ip, lr}		@ 4\n\
	stmia	%0!, {r2, r3, ip, lr}		@ 4\n\
	stmia	%0!, {r2, r3, ip, lr}		@ 4\n\
	subs	r1, r1, #1			@ 1\n\
	bne	1b				@ 1"
	: "=r" (ptr)
	: "0" (kaddr), "I" (PAGE_SIZE / 64)
	: "r1", "r2", "r3", "ip", "lr");
	kunmap_atomic(kaddr, KM_USER0);
}

struct cpu_user_fns v3_user_fns __initdata = {
	.cpu_clear_user_highpage = v3_clear_user_highpage,
	.cpu_copy_user_highpage	= v3_copy_user_highpage,
};
