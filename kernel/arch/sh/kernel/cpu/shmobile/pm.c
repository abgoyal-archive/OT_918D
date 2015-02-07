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
 * arch/sh/kernel/cpu/shmobile/pm.c
 *
 * Power management support code for SuperH Mobile
 *
 *  Copyright (C) 2009 Magnus Damm
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/io.h>
#include <linux/suspend.h>
#include <asm/suspend.h>
#include <asm/uaccess.h>
#include <asm/cacheflush.h>

/*
 * Notifier lists for pre/post sleep notification
 */
ATOMIC_NOTIFIER_HEAD(sh_mobile_pre_sleep_notifier_list);
ATOMIC_NOTIFIER_HEAD(sh_mobile_post_sleep_notifier_list);

/*
 * Sleep modes available on SuperH Mobile:
 *
 * Sleep mode is just plain "sleep" instruction
 * Sleep Self-Refresh mode is above plus RAM put in Self-Refresh
 * Standby Self-Refresh mode is above plus stopped clocks
 */
#define SUSP_MODE_SLEEP		(SUSP_SH_SLEEP)
#define SUSP_MODE_SLEEP_SF	(SUSP_SH_SLEEP | SUSP_SH_SF)
#define SUSP_MODE_STANDBY_SF	(SUSP_SH_STANDBY | SUSP_SH_SF)
#define SUSP_MODE_RSTANDBY_SF \
	(SUSP_SH_RSTANDBY | SUSP_SH_MMU | SUSP_SH_REGS | SUSP_SH_SF)
 /*
  * U-standby mode is unsupported since it needs bootloader hacks
  */

#ifdef CONFIG_CPU_SUBTYPE_SH7724
#define RAM_BASE 0xfd800000 /* RSMEM */
#else
#define RAM_BASE 0xe5200000 /* ILRAM */
#endif

void sh_mobile_call_standby(unsigned long mode)
{
	void *onchip_mem = (void *)RAM_BASE;
	struct sh_sleep_data *sdp = onchip_mem;
	void (*standby_onchip_mem)(unsigned long, unsigned long);

	/* code located directly after data structure */
	standby_onchip_mem = (void *)(sdp + 1);

	atomic_notifier_call_chain(&sh_mobile_pre_sleep_notifier_list,
				   mode, NULL);

	/* flush the caches if MMU flag is set */
	if (mode & SUSP_SH_MMU)
		flush_cache_all();

	/* Let assembly snippet in on-chip memory handle the rest */
	standby_onchip_mem(mode, RAM_BASE);

	atomic_notifier_call_chain(&sh_mobile_post_sleep_notifier_list,
				   mode, NULL);
}

extern char sh_mobile_sleep_enter_start;
extern char sh_mobile_sleep_enter_end;

extern char sh_mobile_sleep_resume_start;
extern char sh_mobile_sleep_resume_end;

unsigned long sh_mobile_sleep_supported = SUSP_SH_SLEEP;

void sh_mobile_register_self_refresh(unsigned long flags,
				     void *pre_start, void *pre_end,
				     void *post_start, void *post_end)
{
	void *onchip_mem = (void *)RAM_BASE;
	void *vp;
	struct sh_sleep_data *sdp;
	int n;

	/* part 0: data area */
	sdp = onchip_mem;
	sdp->addr.stbcr = 0xa4150020; /* STBCR */
	sdp->addr.bar = 0xa4150040; /* BAR */
	sdp->addr.pteh = 0xff000000; /* PTEH */
	sdp->addr.ptel = 0xff000004; /* PTEL */
	sdp->addr.ttb = 0xff000008; /* TTB */
	sdp->addr.tea = 0xff00000c; /* TEA */
	sdp->addr.mmucr = 0xff000010; /* MMUCR */
	sdp->addr.ptea = 0xff000034; /* PTEA */
	sdp->addr.pascr = 0xff000070; /* PASCR */
	sdp->addr.irmcr = 0xff000078; /* IRMCR */
	sdp->addr.ccr = 0xff00001c; /* CCR */
	sdp->addr.ramcr = 0xff000074; /* RAMCR */
	vp = sdp + 1;

	/* part 1: common code to enter sleep mode */
	n = &sh_mobile_sleep_enter_end - &sh_mobile_sleep_enter_start;
	memcpy(vp, &sh_mobile_sleep_enter_start, n);
	vp += roundup(n, 4);

	/* part 2: board specific code to enter self-refresh mode */
	n = pre_end - pre_start;
	memcpy(vp, pre_start, n);
	sdp->sf_pre = (unsigned long)vp;
	vp += roundup(n, 4);

	/* part 3: board specific code to resume from self-refresh mode */
	n = post_end - post_start;
	memcpy(vp, post_start, n);
	sdp->sf_post = (unsigned long)vp;
	vp += roundup(n, 4);

	/* part 4: common code to resume from sleep mode */
	WARN_ON(vp > (onchip_mem + 0x600));
	vp = onchip_mem + 0x600; /* located at interrupt vector */
	n = &sh_mobile_sleep_resume_end - &sh_mobile_sleep_resume_start;
	memcpy(vp, &sh_mobile_sleep_resume_start, n);
	sdp->resume = (unsigned long)vp;

	sh_mobile_sleep_supported |= flags;
}

static int sh_pm_enter(suspend_state_t state)
{
	if (!(sh_mobile_sleep_supported & SUSP_MODE_STANDBY_SF))
		return -ENXIO;

	local_irq_disable();
	set_bl_bit();
	sh_mobile_call_standby(SUSP_MODE_STANDBY_SF);
	local_irq_disable();
	clear_bl_bit();
	return 0;
}

static struct platform_suspend_ops sh_pm_ops = {
	.enter          = sh_pm_enter,
	.valid          = suspend_valid_only_mem,
};

static int __init sh_pm_init(void)
{
	suspend_set_ops(&sh_pm_ops);
	sh_mobile_setup_cpuidle();
	return 0;
}

late_initcall(sh_pm_init);
