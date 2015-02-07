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
 * linux/drivers/pcmcia/pxa2xx_palmtc.c
 *
 * Driver for Palm Tungsten|C PCMCIA
 *
 * Copyright (C) 2008 Alex Osborne <ato@meshy.org>
 * Copyright (C) 2009 Marek Vasut <marek.vasut@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/gpio.h>
#include <linux/delay.h>

#include <asm/mach-types.h>
#include <mach/palmtc.h>
#include "soc_common.h"

static int palmtc_pcmcia_hw_init(struct soc_pcmcia_socket *skt)
{
	int ret;

	ret = gpio_request(GPIO_NR_PALMTC_PCMCIA_POWER1, "PCMCIA PWR1");
	if (ret)
		goto err1;
	ret = gpio_direction_output(GPIO_NR_PALMTC_PCMCIA_POWER1, 0);
	if (ret)
		goto err2;

	ret = gpio_request(GPIO_NR_PALMTC_PCMCIA_POWER2, "PCMCIA PWR2");
	if (ret)
		goto err2;
	ret = gpio_direction_output(GPIO_NR_PALMTC_PCMCIA_POWER2, 0);
	if (ret)
		goto err3;

	ret = gpio_request(GPIO_NR_PALMTC_PCMCIA_POWER3, "PCMCIA PWR3");
	if (ret)
		goto err3;
	ret = gpio_direction_output(GPIO_NR_PALMTC_PCMCIA_POWER3, 0);
	if (ret)
		goto err4;

	ret = gpio_request(GPIO_NR_PALMTC_PCMCIA_RESET, "PCMCIA RST");
	if (ret)
		goto err4;
	ret = gpio_direction_output(GPIO_NR_PALMTC_PCMCIA_RESET, 1);
	if (ret)
		goto err5;

	ret = gpio_request(GPIO_NR_PALMTC_PCMCIA_READY, "PCMCIA RDY");
	if (ret)
		goto err5;
	ret = gpio_direction_input(GPIO_NR_PALMTC_PCMCIA_READY);
	if (ret)
		goto err6;

	ret = gpio_request(GPIO_NR_PALMTC_PCMCIA_PWRREADY, "PCMCIA PWRRDY");
	if (ret)
		goto err6;
	ret = gpio_direction_input(GPIO_NR_PALMTC_PCMCIA_PWRREADY);
	if (ret)
		goto err7;

	skt->socket.pci_irq = IRQ_GPIO(GPIO_NR_PALMTC_PCMCIA_READY);
	return 0;

err7:
	gpio_free(GPIO_NR_PALMTC_PCMCIA_PWRREADY);
err6:
	gpio_free(GPIO_NR_PALMTC_PCMCIA_READY);
err5:
	gpio_free(GPIO_NR_PALMTC_PCMCIA_RESET);
err4:
	gpio_free(GPIO_NR_PALMTC_PCMCIA_POWER3);
err3:
	gpio_free(GPIO_NR_PALMTC_PCMCIA_POWER2);
err2:
	gpio_free(GPIO_NR_PALMTC_PCMCIA_POWER1);
err1:
	return ret;
}

static void palmtc_pcmcia_hw_shutdown(struct soc_pcmcia_socket *skt)
{
	gpio_free(GPIO_NR_PALMTC_PCMCIA_PWRREADY);
	gpio_free(GPIO_NR_PALMTC_PCMCIA_READY);
	gpio_free(GPIO_NR_PALMTC_PCMCIA_RESET);
	gpio_free(GPIO_NR_PALMTC_PCMCIA_POWER3);
	gpio_free(GPIO_NR_PALMTC_PCMCIA_POWER2);
	gpio_free(GPIO_NR_PALMTC_PCMCIA_POWER1);
}

static void palmtc_pcmcia_socket_state(struct soc_pcmcia_socket *skt,
					struct pcmcia_state *state)
{
	state->detect = 1; /* always inserted */
	state->ready  = !!gpio_get_value(GPIO_NR_PALMTC_PCMCIA_READY);
	state->bvd1   = 1;
	state->bvd2   = 1;
	state->wrprot = 0;
	state->vs_3v  = 1;
	state->vs_Xv  = 0;
}

static int palmtc_wifi_powerdown(void)
{
	gpio_set_value(GPIO_NR_PALMTC_PCMCIA_RESET, 1);
	gpio_set_value(GPIO_NR_PALMTC_PCMCIA_POWER2, 0);
	mdelay(40);
	gpio_set_value(GPIO_NR_PALMTC_PCMCIA_POWER1, 0);
	return 0;
}

static int palmtc_wifi_powerup(void)
{
	int timeout = 50;

	gpio_set_value(GPIO_NR_PALMTC_PCMCIA_POWER3, 1);
	mdelay(50);

	/* Power up the card, 1.8V first, after a while 3.3V */
	gpio_set_value(GPIO_NR_PALMTC_PCMCIA_POWER1, 1);
	mdelay(100);
	gpio_set_value(GPIO_NR_PALMTC_PCMCIA_POWER2, 1);

	/* Wait till the card is ready */
	while (!gpio_get_value(GPIO_NR_PALMTC_PCMCIA_PWRREADY) &&
		timeout) {
		mdelay(1);
		timeout--;
	}

	/* Power down the WiFi in case of error */
	if (!timeout) {
		palmtc_wifi_powerdown();
		return 1;
	}

	/* Reset the card */
	gpio_set_value(GPIO_NR_PALMTC_PCMCIA_RESET, 1);
	mdelay(20);
	gpio_set_value(GPIO_NR_PALMTC_PCMCIA_RESET, 0);
	mdelay(25);

	gpio_set_value(GPIO_NR_PALMTC_PCMCIA_POWER3, 0);

	return 0;
}

static int palmtc_pcmcia_configure_socket(struct soc_pcmcia_socket *skt,
					const socket_state_t *state)
{
	int ret = 1;

	if (state->Vcc == 0)
		ret = palmtc_wifi_powerdown();
	else if (state->Vcc == 33)
		ret = palmtc_wifi_powerup();

	return ret;
}

static void palmtc_pcmcia_socket_init(struct soc_pcmcia_socket *skt)
{
}

static void palmtc_pcmcia_socket_suspend(struct soc_pcmcia_socket *skt)
{
}

static struct pcmcia_low_level palmtc_pcmcia_ops = {
	.owner			= THIS_MODULE,

	.first			= 0,
	.nr			= 1,

	.hw_init		= palmtc_pcmcia_hw_init,
	.hw_shutdown		= palmtc_pcmcia_hw_shutdown,

	.socket_state		= palmtc_pcmcia_socket_state,
	.configure_socket	= palmtc_pcmcia_configure_socket,

	.socket_init		= palmtc_pcmcia_socket_init,
	.socket_suspend		= palmtc_pcmcia_socket_suspend,
};

static struct platform_device *palmtc_pcmcia_device;

static int __init palmtc_pcmcia_init(void)
{
	int ret;

	if (!machine_is_palmtc())
		return -ENODEV;

	palmtc_pcmcia_device = platform_device_alloc("pxa2xx-pcmcia", -1);
	if (!palmtc_pcmcia_device)
		return -ENOMEM;

	ret = platform_device_add_data(palmtc_pcmcia_device, &palmtc_pcmcia_ops,
					sizeof(palmtc_pcmcia_ops));

	if (!ret)
		ret = platform_device_add(palmtc_pcmcia_device);

	if (ret)
		platform_device_put(palmtc_pcmcia_device);

	return ret;
}

static void __exit palmtc_pcmcia_exit(void)
{
	platform_device_unregister(palmtc_pcmcia_device);
}

module_init(palmtc_pcmcia_init);
module_exit(palmtc_pcmcia_exit);

MODULE_AUTHOR("Alex Osborne <ato@meshy.org>,"
	    " Marek Vasut <marek.vasut@gmail.com>");
MODULE_DESCRIPTION("PCMCIA support for Palm Tungsten|C");
MODULE_ALIAS("platform:pxa2xx-pcmcia");
MODULE_LICENSE("GPL");
