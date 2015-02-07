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
 * s6000 irq crossbar
 *
 * Copyright (c) 2009 emlix GmbH
 * Authors:	Johannes Weiner <jw@emlix.com>
 *		Oskar Schirmer <os@emlix.com>
 */
#include <linux/io.h>
#include <asm/irq.h>
#include <variant/hardware.h>

/* S6_REG_INTC */
#define INTC_STATUS	0x000
#define INTC_RAW	0x010
#define INTC_STATUS_AG	0x100
#define INTC_CFG(n)	(0x200 + 4 * (n))

/*
 * The s6000 has a crossbar that multiplexes interrupt output lines
 * from the peripherals to input lines on the xtensa core.
 *
 * We leave the mapping decisions to the platform as it depends on the
 * actually connected peripherals which distribution makes sense.
 */
extern const signed char *platform_irq_mappings[NR_IRQS];

static unsigned long scp_to_intc_enable[] = {
#define	TO_INTC_ENABLE(n)	(((n) << 1) + 1)
	TO_INTC_ENABLE(0),
	TO_INTC_ENABLE(1),
	TO_INTC_ENABLE(2),
	TO_INTC_ENABLE(3),
	TO_INTC_ENABLE(4),
	TO_INTC_ENABLE(5),
	TO_INTC_ENABLE(6),
	TO_INTC_ENABLE(7),
	TO_INTC_ENABLE(8),
	TO_INTC_ENABLE(9),
	TO_INTC_ENABLE(10),
	TO_INTC_ENABLE(11),
	TO_INTC_ENABLE(12),
	-1,
	-1,
	TO_INTC_ENABLE(13),
	-1,
	TO_INTC_ENABLE(14),
	-1,
	TO_INTC_ENABLE(15),
#undef	TO_INTC_ENABLE
};

static void irq_set(unsigned int irq, int enable)
{
	unsigned long en;
	const signed char *m = platform_irq_mappings[irq];

	if (!m)
		return;
	en = enable ? scp_to_intc_enable[irq] : 0;
	while (*m >= 0) {
		writel(en, S6_REG_INTC + INTC_CFG(*m));
		m++;
	}
}

void variant_irq_enable(unsigned int irq)
{
	irq_set(irq, 1);
}

void variant_irq_disable(unsigned int irq)
{
	irq_set(irq, 0);
}
