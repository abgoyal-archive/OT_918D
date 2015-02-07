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

/* pcr.c: Generic sparc64 performance counter infrastructure.
 *
 * Copyright (C) 2009 David S. Miller (davem@davemloft.net)
 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/irq.h>

#include <linux/perf_event.h>
#include <linux/ftrace.h>

#include <asm/pil.h>
#include <asm/pcr.h>
#include <asm/nmi.h>

/* This code is shared between various users of the performance
 * counters.  Users will be oprofile, pseudo-NMI watchdog, and the
 * perf_event support layer.
 */

#define PCR_SUN4U_ENABLE	(PCR_PIC_PRIV | PCR_STRACE | PCR_UTRACE)
#define PCR_N2_ENABLE		(PCR_PIC_PRIV | PCR_STRACE | PCR_UTRACE | \
				 PCR_N2_TOE_OV1 | \
				 (2 << PCR_N2_SL1_SHIFT) | \
				 (0xff << PCR_N2_MASK1_SHIFT))

u64 pcr_enable;
unsigned int picl_shift;

/* Performance counter interrupts run unmasked at PIL level 15.
 * Therefore we can't do things like wakeups and other work
 * that expects IRQ disabling to be adhered to in locking etc.
 *
 * Therefore in such situations we defer the work by signalling
 * a lower level cpu IRQ.
 */
void __irq_entry deferred_pcr_work_irq(int irq, struct pt_regs *regs)
{
	struct pt_regs *old_regs;

	clear_softint(1 << PIL_DEFERRED_PCR_WORK);

	old_regs = set_irq_regs(regs);
	irq_enter();
#ifdef CONFIG_PERF_EVENTS
	perf_event_do_pending();
#endif
	irq_exit();
	set_irq_regs(old_regs);
}

void set_perf_event_pending(void)
{
	set_softint(1 << PIL_DEFERRED_PCR_WORK);
}

const struct pcr_ops *pcr_ops;
EXPORT_SYMBOL_GPL(pcr_ops);

static u64 direct_pcr_read(void)
{
	u64 val;

	read_pcr(val);
	return val;
}

static void direct_pcr_write(u64 val)
{
	write_pcr(val);
}

static const struct pcr_ops direct_pcr_ops = {
	.read	= direct_pcr_read,
	.write	= direct_pcr_write,
};

static void n2_pcr_write(u64 val)
{
	unsigned long ret;

	ret = sun4v_niagara2_setperf(HV_N2_PERF_SPARC_CTL, val);
	if (val != HV_EOK)
		write_pcr(val);
}

static const struct pcr_ops n2_pcr_ops = {
	.read	= direct_pcr_read,
	.write	= n2_pcr_write,
};

static unsigned long perf_hsvc_group;
static unsigned long perf_hsvc_major;
static unsigned long perf_hsvc_minor;

static int __init register_perf_hsvc(void)
{
	if (tlb_type == hypervisor) {
		switch (sun4v_chip_type) {
		case SUN4V_CHIP_NIAGARA1:
			perf_hsvc_group = HV_GRP_NIAG_PERF;
			break;

		case SUN4V_CHIP_NIAGARA2:
			perf_hsvc_group = HV_GRP_N2_CPU;
			break;

		default:
			return -ENODEV;
		}


		perf_hsvc_major = 1;
		perf_hsvc_minor = 0;
		if (sun4v_hvapi_register(perf_hsvc_group,
					 perf_hsvc_major,
					 &perf_hsvc_minor)) {
			printk("perfmon: Could not register hvapi.\n");
			return -ENODEV;
		}
	}
	return 0;
}

static void __init unregister_perf_hsvc(void)
{
	if (tlb_type != hypervisor)
		return;
	sun4v_hvapi_unregister(perf_hsvc_group);
}

int __init pcr_arch_init(void)
{
	int err = register_perf_hsvc();

	if (err)
		return err;

	switch (tlb_type) {
	case hypervisor:
		pcr_ops = &n2_pcr_ops;
		pcr_enable = PCR_N2_ENABLE;
		picl_shift = 2;
		break;

	case cheetah:
	case cheetah_plus:
		pcr_ops = &direct_pcr_ops;
		pcr_enable = PCR_SUN4U_ENABLE;
		break;

	case spitfire:
		/* UltraSPARC-I/II and derivatives lack a profile
		 * counter overflow interrupt so we can't make use of
		 * their hardware currently.
		 */
		/* fallthrough */
	default:
		err = -ENODEV;
		goto out_unregister;
	}

	return nmi_init();

out_unregister:
	unregister_perf_hsvc();
	return err;
}

arch_initcall(pcr_arch_init);
