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
 * Coldfire generic GPIO support.
 *
 * (C) Copyright 2009, Steven King <sfking@fdwdc.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sysdev.h>

#include <asm/gpio.h>
#include <asm/pinmux.h>
#include <asm/mcfgpio.h>

#define MCF_CHIP(chip) container_of(chip, struct mcf_gpio_chip, gpio_chip)

int mcf_gpio_direction_input(struct gpio_chip *chip, unsigned offset)
{
	unsigned long flags;
	MCFGPIO_PORTTYPE dir;
	struct mcf_gpio_chip *mcf_chip = MCF_CHIP(chip);

	local_irq_save(flags);
	dir = mcfgpio_read(mcf_chip->pddr);
	dir &= ~mcfgpio_bit(chip->base + offset);
	mcfgpio_write(dir, mcf_chip->pddr);
	local_irq_restore(flags);

	return 0;
}

int mcf_gpio_get_value(struct gpio_chip *chip, unsigned offset)
{
	struct mcf_gpio_chip *mcf_chip = MCF_CHIP(chip);

	return mcfgpio_read(mcf_chip->ppdr) & mcfgpio_bit(chip->base + offset);
}

int mcf_gpio_direction_output(struct gpio_chip *chip, unsigned offset,
		int value)
{
	unsigned long flags;
	MCFGPIO_PORTTYPE data;
	struct mcf_gpio_chip *mcf_chip = MCF_CHIP(chip);

	local_irq_save(flags);
	/* write the value to the output latch */
	data = mcfgpio_read(mcf_chip->podr);
	if (value)
		data |= mcfgpio_bit(chip->base + offset);
	else
		data &= ~mcfgpio_bit(chip->base + offset);
	mcfgpio_write(data, mcf_chip->podr);

	/* now set the direction to output */
	data = mcfgpio_read(mcf_chip->pddr);
	data |= mcfgpio_bit(chip->base + offset);
	mcfgpio_write(data, mcf_chip->pddr);
	local_irq_restore(flags);

	return 0;
}

void mcf_gpio_set_value(struct gpio_chip *chip, unsigned offset, int value)
{
	struct mcf_gpio_chip *mcf_chip = MCF_CHIP(chip);

	unsigned long flags;
	MCFGPIO_PORTTYPE data;

	local_irq_save(flags);
	data = mcfgpio_read(mcf_chip->podr);
	if (value)
		data |= mcfgpio_bit(chip->base + offset);
	else
		data &= ~mcfgpio_bit(chip->base + offset);
	mcfgpio_write(data, mcf_chip->podr);
	local_irq_restore(flags);
}

void mcf_gpio_set_value_fast(struct gpio_chip *chip, unsigned offset, int value)
{
	struct mcf_gpio_chip *mcf_chip = MCF_CHIP(chip);

	if (value)
		mcfgpio_write(mcfgpio_bit(chip->base + offset), mcf_chip->setr);
	else
		mcfgpio_write(~mcfgpio_bit(chip->base + offset), mcf_chip->clrr);
}

int mcf_gpio_request(struct gpio_chip *chip, unsigned offset)
{
	struct mcf_gpio_chip *mcf_chip = MCF_CHIP(chip);

	return mcf_chip->gpio_to_pinmux ?
		mcf_pinmux_request(mcf_chip->gpio_to_pinmux[offset], 0) : 0;
}

void mcf_gpio_free(struct gpio_chip *chip, unsigned offset)
{
	struct mcf_gpio_chip *mcf_chip = MCF_CHIP(chip);

	mcf_gpio_direction_input(chip, offset);

	if (mcf_chip->gpio_to_pinmux)
		mcf_pinmux_release(mcf_chip->gpio_to_pinmux[offset], 0);
}

struct sysdev_class mcf_gpio_sysclass = {
	.name	= "gpio",
};

static int __init mcf_gpio_sysinit(void)
{
	return sysdev_class_register(&mcf_gpio_sysclass);
}

core_initcall(mcf_gpio_sysinit);
