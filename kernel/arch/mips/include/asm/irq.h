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
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1994 by Waldorf GMBH, written by Ralf Baechle
 * Copyright (C) 1995, 96, 97, 98, 99, 2000, 01, 02, 03 by Ralf Baechle
 */
#ifndef _ASM_IRQ_H
#define _ASM_IRQ_H

#include <linux/linkage.h>
#include <linux/smp.h>

#include <asm/mipsmtregs.h>

#include <irq.h>

#ifdef CONFIG_I8259
static inline int irq_canonicalize(int irq)
{
	return ((irq == I8259A_IRQ_BASE + 2) ? I8259A_IRQ_BASE + 9 : irq);
}
#else
#define irq_canonicalize(irq) (irq)	/* Sane hardware, sane code ... */
#endif

#ifdef CONFIG_MIPS_MT_SMTC

struct irqaction;

extern unsigned long irq_hwmask[];
extern int setup_irq_smtc(unsigned int irq, struct irqaction * new,
                          unsigned long hwmask);

static inline void smtc_im_ack_irq(unsigned int irq)
{
	if (irq_hwmask[irq] & ST0_IM)
		set_c0_status(irq_hwmask[irq] & ST0_IM);
}

#else

static inline void smtc_im_ack_irq(unsigned int irq)
{
}

#endif /* CONFIG_MIPS_MT_SMTC */

#ifdef CONFIG_MIPS_MT_SMTC_IRQAFF
#include <linux/cpumask.h>

extern int plat_set_irq_affinity(unsigned int irq,
				  const struct cpumask *affinity);
extern void smtc_forward_irq(unsigned int irq);

/*
 * IRQ affinity hook invoked at the beginning of interrupt dispatch
 * if option is enabled.
 *
 * Up through Linux 2.6.22 (at least) cpumask operations are very
 * inefficient on MIPS.  Initial prototypes of SMTC IRQ affinity
 * used a "fast path" per-IRQ-descriptor cache of affinity information
 * to reduce latency.  As there is a project afoot to optimize the
 * cpumask implementations, this version is optimistically assuming
 * that cpumask.h macro overhead is reasonable during interrupt dispatch.
 */
#define IRQ_AFFINITY_HOOK(irq)						\
do {									\
    if (!cpumask_test_cpu(smp_processor_id(), irq_desc[irq].affinity)) {\
	smtc_forward_irq(irq);						\
	irq_exit();							\
	return;								\
    }									\
} while (0)

#else /* Not doing SMTC affinity */

#define IRQ_AFFINITY_HOOK(irq) do { } while (0)

#endif /* CONFIG_MIPS_MT_SMTC_IRQAFF */

#ifdef CONFIG_MIPS_MT_SMTC_IM_BACKSTOP

/*
 * Clear interrupt mask handling "backstop" if irq_hwmask
 * entry so indicates. This implies that the ack() or end()
 * functions will take over re-enabling the low-level mask.
 * Otherwise it will be done on return from exception.
 */
#define __DO_IRQ_SMTC_HOOK(irq)						\
do {									\
	IRQ_AFFINITY_HOOK(irq);						\
	if (irq_hwmask[irq] & 0x0000ff00)				\
		write_c0_tccontext(read_c0_tccontext() &		\
				   ~(irq_hwmask[irq] & 0x0000ff00));	\
} while (0)

#define __NO_AFFINITY_IRQ_SMTC_HOOK(irq)				\
do {									\
	if (irq_hwmask[irq] & 0x0000ff00)                               \
		write_c0_tccontext(read_c0_tccontext() &		\
				   ~(irq_hwmask[irq] & 0x0000ff00));	\
} while (0)

#else

#define __DO_IRQ_SMTC_HOOK(irq)						\
do {									\
	IRQ_AFFINITY_HOOK(irq);						\
} while (0)
#define __NO_AFFINITY_IRQ_SMTC_HOOK(irq) do { } while (0)

#endif

extern void do_IRQ(unsigned int irq);

#ifdef CONFIG_MIPS_MT_SMTC_IRQAFF

extern void do_IRQ_no_affinity(unsigned int irq);

#endif /* CONFIG_MIPS_MT_SMTC_IRQAFF */

extern void arch_init_irq(void);
extern void spurious_interrupt(void);

extern int allocate_irqno(void);
extern void alloc_legacy_irqno(void);
extern void free_irqno(unsigned int irq);

/*
 * Before R2 the timer and performance counter interrupts were both fixed to
 * IE7.  Since R2 their number has to be read from the c0_intctl register.
 */
#define CP0_LEGACY_COMPARE_IRQ 7

extern int cp0_compare_irq;
extern int cp0_compare_irq_shift;
extern int cp0_perfcount_irq;

#endif /* _ASM_IRQ_H */
