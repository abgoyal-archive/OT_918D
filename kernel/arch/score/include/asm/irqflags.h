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

#ifndef _ASM_SCORE_IRQFLAGS_H
#define _ASM_SCORE_IRQFLAGS_H

#ifndef __ASSEMBLY__

#define raw_local_irq_save(x)			\
{						\
	__asm__ __volatile__(			\
		"mfcr	r8, cr0;"		\
		"li	r9, 0xfffffffe;"	\
		"nop;"				\
		"mv	%0, r8;"		\
		"and	r8, r8, r9;"		\
		"mtcr	r8, cr0;"		\
		"nop;"				\
		"nop;"				\
		"nop;"				\
		"nop;"				\
		"nop;"				\
		: "=r" (x)			\
		:				\
		: "r8", "r9"			\
		);				\
}

#define raw_local_irq_restore(x)		\
{						\
	__asm__ __volatile__(			\
		"mfcr	r8, cr0;"		\
		"ldi	r9, 0x1;"		\
		"and	%0, %0, r9;"		\
		"or	r8, r8, %0;"		\
		"mtcr	r8, cr0;"		\
		"nop;"				\
		"nop;"				\
		"nop;"				\
		"nop;"				\
		"nop;"				\
		:				\
		: "r"(x)			\
		: "r8", "r9"			\
		);				\
}

#define raw_local_irq_enable(void)		\
{						\
	__asm__ __volatile__(			\
		"mfcr\tr8,cr0;"			\
		"nop;"				\
		"nop;"				\
		"ori\tr8,0x1;"			\
		"mtcr\tr8,cr0;"			\
		"nop;"				\
		"nop;"				\
		"nop;"				\
		"nop;"				\
		"nop;"				\
		:				\
		:				\
		: "r8");			\
}

#define raw_local_irq_disable(void)		\
{						\
	__asm__ __volatile__(			\
		"mfcr\tr8,cr0;"			\
		"nop;"				\
		"nop;"				\
		"srli\tr8,r8,1;"		\
		"slli\tr8,r8,1;"		\
		"mtcr\tr8,cr0;"			\
		"nop;"				\
		"nop;"				\
		"nop;"				\
		"nop;"				\
		"nop;"				\
		:				\
		:				\
		: "r8");			\
}

#define raw_local_save_flags(x)			\
{						\
	__asm__ __volatile__(			\
		"mfcr	r8, cr0;"		\
		"nop;"				\
		"nop;"				\
		"mv	%0, r8;"		\
		"nop;"				\
		"nop;"				\
		"nop;"				\
		"nop;"				\
		"nop;"				\
		"ldi	r9, 0x1;"		\
		"and	%0, %0, r9;"		\
		: "=r" (x)			\
		:				\
		: "r8", "r9"			\
		);				\
}

static inline int raw_irqs_disabled_flags(unsigned long flags)
{
	return !(flags & 1);
}

#endif

#endif /* _ASM_SCORE_IRQFLAGS_H */
