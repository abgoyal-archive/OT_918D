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
 * linux/arch/arm/mach-w90x900/gpio.c
 *
 * Generic nuc900 GPIO handling
 *
 *  Wan ZongShun <mcuos.com@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/clk.h>
#include <linux/errno.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/debugfs.h>
#include <linux/seq_file.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/module.h>
#include <linux/io.h>
#include <linux/gpio.h>

#include <mach/hardware.h>

#define GPIO_BASE 		(W90X900_VA_GPIO)
#define GPIO_DIR		(0x04)
#define GPIO_OUT		(0x08)
#define GPIO_IN			(0x0C)
#define GROUPINERV		(0x10)
#define GPIO_GPIO(Nb)		(0x00000001 << (Nb))
#define to_nuc900_gpio_chip(c) container_of(c, struct nuc900_gpio_chip, chip)

#define NUC900_GPIO_CHIP(name, base_gpio, nr_gpio)			\
	{								\
		.chip = {						\
			.label		  = name,			\
			.direction_input  = nuc900_dir_input,		\
			.direction_output = nuc900_dir_output,		\
			.get		  = nuc900_gpio_get,		\
			.set		  = nuc900_gpio_set,		\
			.base		  = base_gpio,			\
			.ngpio		  = nr_gpio,			\
		}							\
	}

struct nuc900_gpio_chip {
	struct gpio_chip	chip;
	void __iomem		*regbase;	/* Base of group register*/
	spinlock_t 		gpio_lock;
};

static int nuc900_gpio_get(struct gpio_chip *chip, unsigned offset)
{
	struct nuc900_gpio_chip *nuc900_gpio = to_nuc900_gpio_chip(chip);
	void __iomem *pio = nuc900_gpio->regbase + GPIO_IN;
	unsigned int regval;

	regval = __raw_readl(pio);
	regval &= GPIO_GPIO(offset);

	return (regval != 0);
}

static void nuc900_gpio_set(struct gpio_chip *chip, unsigned offset, int val)
{
	struct nuc900_gpio_chip *nuc900_gpio = to_nuc900_gpio_chip(chip);
	void __iomem *pio = nuc900_gpio->regbase + GPIO_OUT;
	unsigned int regval;
	unsigned long flags;

	spin_lock_irqsave(&nuc900_gpio->gpio_lock, flags);

	regval = __raw_readl(pio);

	if (val)
		regval |= GPIO_GPIO(offset);
	else
		regval &= ~GPIO_GPIO(offset);

	__raw_writel(regval, pio);

	spin_unlock_irqrestore(&nuc900_gpio->gpio_lock, flags);
}

static int nuc900_dir_input(struct gpio_chip *chip, unsigned offset)
{
	struct nuc900_gpio_chip *nuc900_gpio = to_nuc900_gpio_chip(chip);
	void __iomem *pio = nuc900_gpio->regbase + GPIO_DIR;
	unsigned int regval;
	unsigned long flags;

	spin_lock_irqsave(&nuc900_gpio->gpio_lock, flags);

	regval = __raw_readl(pio);
	regval &= ~GPIO_GPIO(offset);
	__raw_writel(regval, pio);

	spin_unlock_irqrestore(&nuc900_gpio->gpio_lock, flags);

	return 0;
}

static int nuc900_dir_output(struct gpio_chip *chip, unsigned offset, int val)
{
	struct nuc900_gpio_chip *nuc900_gpio = to_nuc900_gpio_chip(chip);
	void __iomem *outreg = nuc900_gpio->regbase + GPIO_OUT;
	void __iomem *pio = nuc900_gpio->regbase + GPIO_DIR;
	unsigned int regval;
	unsigned long flags;

	spin_lock_irqsave(&nuc900_gpio->gpio_lock, flags);

	regval = __raw_readl(pio);
	regval |= GPIO_GPIO(offset);
	__raw_writel(regval, pio);

	regval = __raw_readl(outreg);

	if (val)
		regval |= GPIO_GPIO(offset);
	else
		regval &= ~GPIO_GPIO(offset);

	__raw_writel(regval, outreg);

	spin_unlock_irqrestore(&nuc900_gpio->gpio_lock, flags);

	return 0;
}

static struct nuc900_gpio_chip nuc900_gpio[] = {
	NUC900_GPIO_CHIP("GROUPC", 0, 16),
	NUC900_GPIO_CHIP("GROUPD", 16, 10),
	NUC900_GPIO_CHIP("GROUPE", 26, 14),
	NUC900_GPIO_CHIP("GROUPF", 40, 10),
	NUC900_GPIO_CHIP("GROUPG", 50, 17),
	NUC900_GPIO_CHIP("GROUPH", 67, 8),
	NUC900_GPIO_CHIP("GROUPI", 75, 17),
};

void __init nuc900_init_gpio(int nr_group)
{
	unsigned	i;
	struct nuc900_gpio_chip *gpio_chip;

	for (i = 0; i < nr_group; i++) {
		gpio_chip = &nuc900_gpio[i];
		spin_lock_init(&gpio_chip->gpio_lock);
		gpio_chip->regbase = GPIO_BASE + i * GROUPINERV;
		gpiochip_add(&gpio_chip->chip);
	}
}
