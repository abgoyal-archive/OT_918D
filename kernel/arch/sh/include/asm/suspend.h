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

#ifndef _ASM_SH_SUSPEND_H
#define _ASM_SH_SUSPEND_H

#ifndef __ASSEMBLY__
#include <linux/notifier.h>
static inline int arch_prepare_suspend(void) { return 0; }

#include <asm/ptrace.h>

struct swsusp_arch_regs {
	struct pt_regs user_regs;
	unsigned long bank1_regs[8];
};

void sh_mobile_call_standby(unsigned long mode);

#ifdef CONFIG_CPU_IDLE
void sh_mobile_setup_cpuidle(void);
#else
static inline void sh_mobile_setup_cpuidle(void) {}
#endif

/* notifier chains for pre/post sleep hooks */
extern struct atomic_notifier_head sh_mobile_pre_sleep_notifier_list;
extern struct atomic_notifier_head sh_mobile_post_sleep_notifier_list;

/* priority levels for notifiers */
#define SH_MOBILE_SLEEP_BOARD	0
#define SH_MOBILE_SLEEP_CPU	1
#define SH_MOBILE_PRE(x)	(x)
#define SH_MOBILE_POST(x)	(-(x))

/* board code registration function for self-refresh assembly snippets */
void sh_mobile_register_self_refresh(unsigned long flags,
				     void *pre_start, void *pre_end,
				     void *post_start, void *post_end);

/* register structure for address/data information */
struct sh_sleep_regs {
	unsigned long stbcr;
	unsigned long bar;

	/* MMU */
	unsigned long pteh;
	unsigned long ptel;
	unsigned long ttb;
	unsigned long tea;
	unsigned long mmucr;
	unsigned long ptea;
	unsigned long pascr;
	unsigned long irmcr;

	/* Cache */
	unsigned long ccr;
	unsigned long ramcr;
};

/* data area for low-level sleep code */
struct sh_sleep_data {
	/* current sleep mode (SUSP_SH_...) */
	unsigned long mode;

	/* addresses of board specific self-refresh snippets */
	unsigned long sf_pre;
	unsigned long sf_post;

	/* address of resume code */
	unsigned long resume;

	/* register state saved and restored by the assembly code */
	unsigned long vbr;
	unsigned long spc;
	unsigned long sr;
	unsigned long sp;

	/* structure for keeping register addresses */
	struct sh_sleep_regs addr;

	/* structure for saving/restoring register state */
	struct sh_sleep_regs data;
};

/* a bitmap of supported sleep modes (SUSP_SH..) */
extern unsigned long sh_mobile_sleep_supported;

#endif

/* flags passed to assembly suspend code */
#define SUSP_SH_SLEEP		(1 << 0) /* Regular sleep mode */
#define SUSP_SH_STANDBY		(1 << 1) /* SH-Mobile Software standby mode */
#define SUSP_SH_RSTANDBY	(1 << 2) /* SH-Mobile R-standby mode */
#define SUSP_SH_USTANDBY	(1 << 3) /* SH-Mobile U-standby mode */
#define SUSP_SH_SF		(1 << 4) /* Enable self-refresh */
#define SUSP_SH_MMU		(1 << 5) /* Save/restore MMU and cache */
#define SUSP_SH_REGS		(1 << 6) /* Save/restore registers */

#endif /* _ASM_SH_SUSPEND_H */
