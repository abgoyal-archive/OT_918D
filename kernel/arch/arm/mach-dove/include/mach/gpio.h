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
 * arch/arm/mach-dove/include/mach/gpio.h
 *
 * This file is licensed under the terms of the GNU General Public
 * License version 2.  This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */

#ifndef __ASM_ARCH_GPIO_H
#define __ASM_ARCH_GPIO_H

#include <asm/errno.h>
#include <mach/irqs.h>
#include <plat/gpio.h>
#include <asm-generic/gpio.h>		/* cansleep wrappers */

#define GPIO_MAX	64

#define GPIO_BASE_LO		(DOVE_GPIO_VIRT_BASE + 0x00)
#define GPIO_BASE_HI		(DOVE_GPIO_VIRT_BASE + 0x20)

#define GPIO_BASE(pin)		((pin < 32) ? GPIO_BASE_LO : GPIO_BASE_HI)

#define GPIO_OUT(pin)		(GPIO_BASE(pin) + 0x00)
#define GPIO_IO_CONF(pin)	(GPIO_BASE(pin) + 0x04)
#define GPIO_BLINK_EN(pin)	(GPIO_BASE(pin) + 0x08)
#define GPIO_IN_POL(pin)	(GPIO_BASE(pin) + 0x0c)
#define GPIO_DATA_IN(pin)	(GPIO_BASE(pin) + 0x10)
#define GPIO_EDGE_CAUSE(pin)	(GPIO_BASE(pin) + 0x14)
#define GPIO_EDGE_MASK(pin)	(GPIO_BASE(pin) + 0x18)
#define GPIO_LEVEL_MASK(pin)	(GPIO_BASE(pin) + 0x1c)

static inline int gpio_to_irq(int pin)
{
	if (pin < NR_GPIO_IRQS)
		return pin + IRQ_DOVE_GPIO_START;

	return -EINVAL;
}

static inline int irq_to_gpio(int irq)
{
	if (IRQ_DOVE_GPIO_START < irq && irq < NR_IRQS)
		return irq - IRQ_DOVE_GPIO_START;

	return -EINVAL;
}

#endif
