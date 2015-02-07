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

#ifndef __ASM_GENERIC_IRQFLAGS_H
#define __ASM_GENERIC_IRQFLAGS_H

/*
 * All architectures should implement at least the first two functions,
 * usually inline assembly will be the best way.
 */
#ifndef RAW_IRQ_DISABLED
#define RAW_IRQ_DISABLED 0
#define RAW_IRQ_ENABLED 1
#endif

/* read interrupt enabled status */
#ifndef __raw_local_save_flags
unsigned long __raw_local_save_flags(void);
#endif

/* set interrupt enabled status */
#ifndef raw_local_irq_restore
void raw_local_irq_restore(unsigned long flags);
#endif

/* get status and disable interrupts */
#ifndef __raw_local_irq_save
static inline unsigned long __raw_local_irq_save(void)
{
	unsigned long flags;
	flags = __raw_local_save_flags();
	raw_local_irq_restore(RAW_IRQ_DISABLED);
	return flags;
}
#endif

/* test flags */
#ifndef raw_irqs_disabled_flags
static inline int raw_irqs_disabled_flags(unsigned long flags)
{
	return flags == RAW_IRQ_DISABLED;
}
#endif

/* unconditionally enable interrupts */
#ifndef raw_local_irq_enable
static inline void raw_local_irq_enable(void)
{
	raw_local_irq_restore(RAW_IRQ_ENABLED);
}
#endif

/* unconditionally disable interrupts */
#ifndef raw_local_irq_disable
static inline void raw_local_irq_disable(void)
{
	raw_local_irq_restore(RAW_IRQ_DISABLED);
}
#endif

/* test hardware interrupt enable bit */
#ifndef raw_irqs_disabled
static inline int raw_irqs_disabled(void)
{
	return raw_irqs_disabled_flags(__raw_local_save_flags());
}
#endif

#define raw_local_save_flags(flags) \
	do { (flags) = __raw_local_save_flags(); } while (0)

#define raw_local_irq_save(flags) \
	do { (flags) = __raw_local_irq_save(); } while (0)

#endif /* __ASM_GENERIC_IRQFLAGS_H */
