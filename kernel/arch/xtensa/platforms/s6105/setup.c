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
 * s6105 control routines
 *
 * Copyright (c) 2009 emlix GmbH
 */
#include <linux/irq.h>
#include <linux/io.h>
#include <linux/gpio.h>

#include <asm/bootparam.h>

#include <variant/hardware.h>
#include <variant/gpio.h>

#include <platform/gpio.h>

void platform_halt(void)
{
	local_irq_disable();
	while (1)
		;
}

void platform_power_off(void)
{
	platform_halt();
}

void platform_restart(void)
{
	platform_halt();
}

void __init platform_setup(char **cmdline)
{
	unsigned long reg;

	reg = readl(S6_REG_GREG1 + S6_GREG1_PLLSEL);
	reg &= ~(S6_GREG1_PLLSEL_GMAC_MASK << S6_GREG1_PLLSEL_GMAC |
		S6_GREG1_PLLSEL_GMII_MASK << S6_GREG1_PLLSEL_GMII);
	reg |= S6_GREG1_PLLSEL_GMAC_125MHZ << S6_GREG1_PLLSEL_GMAC |
		S6_GREG1_PLLSEL_GMII_125MHZ << S6_GREG1_PLLSEL_GMII;
	writel(reg, S6_REG_GREG1 + S6_GREG1_PLLSEL);

	reg = readl(S6_REG_GREG1 + S6_GREG1_CLKGATE);
	reg &= ~(1 << S6_GREG1_BLOCK_SB);
	reg &= ~(1 << S6_GREG1_BLOCK_GMAC);
	writel(reg, S6_REG_GREG1 + S6_GREG1_CLKGATE);

	reg = readl(S6_REG_GREG1 + S6_GREG1_BLOCKENA);
	reg |= 1 << S6_GREG1_BLOCK_SB;
	reg |= 1 << S6_GREG1_BLOCK_GMAC;
	writel(reg, S6_REG_GREG1 + S6_GREG1_BLOCKENA);

	printk(KERN_NOTICE "S6105 on Stretch S6000 - "
		"Copyright (C) 2009 emlix GmbH <info@emlix.com>\n");
}

void __init platform_init(bp_tag_t *first)
{
	s6_gpio_init(0);
	gpio_request(GPIO_LED1_NGREEN, "led1_green");
	gpio_request(GPIO_LED1_RED, "led1_red");
	gpio_direction_output(GPIO_LED1_NGREEN, 1);
}

void platform_heartbeat(void)
{
	static unsigned int c;

	if (!(++c & 0x4F))
		gpio_direction_output(GPIO_LED1_RED, !(c & 0x10));
}
