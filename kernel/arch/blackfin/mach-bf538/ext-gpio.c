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
 * GPIOLIB interface for BF538/9 PORT C, D, and E GPIOs
 *
 * Copyright 2009 Analog Devices Inc.
 *
 * Licensed under the GPL-2 or later.
 */

#include <linux/module.h>
#include <linux/err.h>
#include <asm/blackfin.h>
#include <asm/gpio.h>
#include <asm/portmux.h>

#define DEFINE_REG(reg, off) \
static inline u16 read_##reg(void __iomem *port) \
	{ return bfin_read16(port + off); } \
static inline void write_##reg(void __iomem *port, u16 v) \
	{ bfin_write16(port + off, v); }

DEFINE_REG(PORTIO, 0x00)
DEFINE_REG(PORTIO_CLEAR, 0x10)
DEFINE_REG(PORTIO_SET, 0x20)
DEFINE_REG(PORTIO_DIR, 0x40)
DEFINE_REG(PORTIO_INEN, 0x50)

static void __iomem *gpio_chip_to_mmr(struct gpio_chip *chip)
{
	switch (chip->base) {
	default: /* not really needed, but keeps gcc happy */
	case GPIO_PC0: return (void __iomem *)PORTCIO;
	case GPIO_PD0: return (void __iomem *)PORTDIO;
	case GPIO_PE0: return (void __iomem *)PORTEIO;
	}
}

static int bf538_gpio_get_value(struct gpio_chip *chip, unsigned gpio)
{
	void __iomem *port = gpio_chip_to_mmr(chip);
	return !!(read_PORTIO(port) & (1u << gpio));
}

static void bf538_gpio_set_value(struct gpio_chip *chip, unsigned gpio, int value)
{
	void __iomem *port = gpio_chip_to_mmr(chip);
	if (value)
		write_PORTIO_SET(port, (1u << gpio));
	else
		write_PORTIO_CLEAR(port, (1u << gpio));
}

static int bf538_gpio_direction_input(struct gpio_chip *chip, unsigned gpio)
{
	void __iomem *port = gpio_chip_to_mmr(chip);
	write_PORTIO_DIR(port, read_PORTIO_DIR(port) & ~(1u << gpio));
	write_PORTIO_INEN(port, read_PORTIO_INEN(port) | (1u << gpio));
	return 0;
}

static int bf538_gpio_direction_output(struct gpio_chip *chip, unsigned gpio, int value)
{
	void __iomem *port = gpio_chip_to_mmr(chip);
	write_PORTIO_INEN(port, read_PORTIO_INEN(port) & ~(1u << gpio));
	bf538_gpio_set_value(port, gpio, value);
	write_PORTIO_DIR(port, read_PORTIO_DIR(port) | (1u << gpio));
	return 0;
}

static int bf538_gpio_request(struct gpio_chip *chip, unsigned gpio)
{
	return bfin_special_gpio_request(chip->base + gpio, chip->label);
}

static void bf538_gpio_free(struct gpio_chip *chip, unsigned gpio)
{
	return bfin_special_gpio_free(chip->base + gpio);
}

/* We don't set the irq fields as these banks cannot generate interrupts */

static struct gpio_chip bf538_portc_chip = {
	.label = "GPIO-PC",
	.direction_input = bf538_gpio_direction_input,
	.get = bf538_gpio_get_value,
	.direction_output = bf538_gpio_direction_output,
	.set = bf538_gpio_set_value,
	.request = bf538_gpio_request,
	.free = bf538_gpio_free,
	.base = GPIO_PC0,
	.ngpio = GPIO_PC9 - GPIO_PC0 + 1,
};

static struct gpio_chip bf538_portd_chip = {
	.label = "GPIO-PD",
	.direction_input = bf538_gpio_direction_input,
	.get = bf538_gpio_get_value,
	.direction_output = bf538_gpio_direction_output,
	.set = bf538_gpio_set_value,
	.request = bf538_gpio_request,
	.free = bf538_gpio_free,
	.base = GPIO_PD0,
	.ngpio = GPIO_PD13 - GPIO_PD0 + 1,
};

static struct gpio_chip bf538_porte_chip = {
	.label = "GPIO-PE",
	.direction_input = bf538_gpio_direction_input,
	.get = bf538_gpio_get_value,
	.direction_output = bf538_gpio_direction_output,
	.set = bf538_gpio_set_value,
	.request = bf538_gpio_request,
	.free = bf538_gpio_free,
	.base = GPIO_PE0,
	.ngpio = GPIO_PE15 - GPIO_PE0 + 1,
};

static int __init bf538_extgpio_setup(void)
{
	return gpiochip_add(&bf538_portc_chip) |
		gpiochip_add(&bf538_portd_chip) |
		gpiochip_add(&bf538_porte_chip);
}
arch_initcall(bf538_extgpio_setup);
