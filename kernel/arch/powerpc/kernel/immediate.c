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
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */

/*
 * Powerpc optimized immediate values enabling/disabling.
 *
 * Copyright 2007-2009 Mathieu Desnoyers <mathieu.desnoyers@polymtl.ca>
 *
 * Dual BSD/GPL v2 license.
 */

#include <linux/module.h>
#include <linux/immediate.h>
#include <linux/string.h>
#include <linux/kprobes.h>
#include <asm/cacheflush.h>
#include <asm/page.h>

#define LI_OPCODE_LEN	2

/**
 * arch_imv_update - update one immediate value
 * @imv: pointer of type const struct __imv to update
 * @early: early boot (1), normal (0)
 *
 * Update one immediate value. Must be called with imv_mutex held.
 */
int arch_imv_update(const struct __imv *imv, int early)
{
#ifdef CONFIG_KPROBES
	kprobe_opcode_t *insn;
	/*
	 * Fail if a kprobe has been set on this instruction.
	 * (TODO: we could eventually do better and modify all the (possibly
	 * nested) kprobes for this site if kprobes had an API for this.
	 */
	switch (imv->size) {
	case 1:	/* The uint8_t points to the 3rd byte of the
		 * instruction */
		insn = (void *)(imv->imv - 1 - LI_OPCODE_LEN);
		break;
	case 2:	insn = (void *)(imv->imv - LI_OPCODE_LEN);
		break;
	default:
	return -EINVAL;
	}

	if (unlikely(!early && *insn == BREAKPOINT_INSTRUCTION)) {
		printk(KERN_WARNING "Immediate value in conflict with kprobe. "
				    "Variable at %p, "
				    "instruction at %p, size %lu\n",
				    (void *)imv->imv,
				    (void *)imv->var, imv->size);
		return -EBUSY;
	}
#endif

	/*
	 * If the variable and the instruction have the same value, there is
	 * nothing to do.
	 */
	switch (imv->size) {
	case 1:	if (*(uint8_t *)imv->imv == *(uint8_t *)imv->var)
			return 0;
		*(uint8_t *)imv->imv = *(uint8_t *)imv->var;
		break;
	case 2:	if (*(uint16_t *)imv->imv == *(uint16_t *)imv->var)
			return 0;
		*(uint16_t *)imv->imv = *(uint16_t *)imv->var;
		break;
	default:return -EINVAL;
	}
	flush_icache_range(imv->imv, imv->imv + imv->size);
	return 0;
}
