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

/* linux/drivers/serial/s5pv210.c
 *
 * Copyright (c) 2010 Samsung Electronics Co., Ltd.
 *		http://www.samsung.com/
 *
 * Based on drivers/serial/s3c6400.c
 *
 * Driver for Samsung S5PV210 SoC UARTs.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#include <linux/module.h>
#include <linux/ioport.h>
#include <linux/io.h>
#include <linux/platform_device.h>
#include <linux/init.h>
#include <linux/serial_core.h>
#include <linux/serial.h>

#include <asm/irq.h>
#include <mach/hardware.h>
#include <plat/regs-serial.h>
#include "samsung.h"

static int s5pv210_serial_setsource(struct uart_port *port,
					struct s3c24xx_uart_clksrc *clk)
{
	unsigned long ucon = rd_regl(port, S3C2410_UCON);

	if (strcmp(clk->name, "pclk") == 0)
		ucon &= ~S5PV210_UCON_CLKMASK;
	else if (strcmp(clk->name, "uclk1") == 0)
		ucon |= S5PV210_UCON_CLKMASK;
	else {
		printk(KERN_ERR "unknown clock source %s\n", clk->name);
		return -EINVAL;
	}

	wr_regl(port, S3C2410_UCON, ucon);
	return 0;
}


static int s5pv210_serial_getsource(struct uart_port *port,
					struct s3c24xx_uart_clksrc *clk)
{
	u32 ucon = rd_regl(port, S3C2410_UCON);

	clk->divisor = 1;

	switch (ucon & S5PV210_UCON_CLKMASK) {
	case S5PV210_UCON_PCLK:
		clk->name = "pclk";
		break;
	case S5PV210_UCON_UCLK:
		clk->name = "uclk1";
		break;
	}

	return 0;
}

static int s5pv210_serial_resetport(struct uart_port *port,
					struct s3c2410_uartcfg *cfg)
{
	unsigned long ucon = rd_regl(port, S3C2410_UCON);

	ucon &= S5PV210_UCON_CLKMASK;
	wr_regl(port, S3C2410_UCON,  ucon | cfg->ucon);
	wr_regl(port, S3C2410_ULCON, cfg->ulcon);

	/* reset both fifos */
	wr_regl(port, S3C2410_UFCON, cfg->ufcon | S3C2410_UFCON_RESETBOTH);
	wr_regl(port, S3C2410_UFCON, cfg->ufcon);

	return 0;
}

#define S5PV210_UART_DEFAULT_INFO(fifo_size)			\
		.name		= "Samsung S5PV210 UART0",	\
		.type		= PORT_S3C6400,			\
		.fifosize	= fifo_size,			\
		.has_divslot	= 1,				\
		.rx_fifomask	= S5PV210_UFSTAT_RXMASK,	\
		.rx_fifoshift	= S5PV210_UFSTAT_RXSHIFT,	\
		.rx_fifofull	= S5PV210_UFSTAT_RXFULL,	\
		.tx_fifofull	= S5PV210_UFSTAT_TXFULL,	\
		.tx_fifomask	= S5PV210_UFSTAT_TXMASK,	\
		.tx_fifoshift	= S5PV210_UFSTAT_TXSHIFT,	\
		.get_clksrc	= s5pv210_serial_getsource,	\
		.set_clksrc	= s5pv210_serial_setsource,	\
		.reset_port	= s5pv210_serial_resetport

static struct s3c24xx_uart_info s5p_port_fifo256 = {
	S5PV210_UART_DEFAULT_INFO(256),
};

static struct s3c24xx_uart_info s5p_port_fifo64 = {
	S5PV210_UART_DEFAULT_INFO(64),
};

static struct s3c24xx_uart_info s5p_port_fifo16 = {
	S5PV210_UART_DEFAULT_INFO(16),
};

static struct s3c24xx_uart_info *s5p_uart_inf[] = {
	[0] = &s5p_port_fifo256,
	[1] = &s5p_port_fifo64,
	[2] = &s5p_port_fifo16,
	[3] = &s5p_port_fifo16,
};

/* device management */
static int s5p_serial_probe(struct platform_device *pdev)
{
	return s3c24xx_serial_probe(pdev, s5p_uart_inf[pdev->id]);
}

static struct platform_driver s5p_serial_driver = {
	.probe		= s5p_serial_probe,
	.remove		= __devexit_p(s3c24xx_serial_remove),
	.driver		= {
		.name	= "s5pv210-uart",
		.owner	= THIS_MODULE,
	},
};

static int __init s5pv210_serial_console_init(void)
{
	return s3c24xx_serial_initconsole(&s5p_serial_driver, s5p_uart_inf);
}

console_initcall(s5pv210_serial_console_init);

static int __init s5p_serial_init(void)
{
	return s3c24xx_serial_init(&s5p_serial_driver, *s5p_uart_inf);
}

static void __exit s5p_serial_exit(void)
{
	platform_driver_unregister(&s5p_serial_driver);
}

module_init(s5p_serial_init);
module_exit(s5p_serial_exit);

MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:s5pv210-uart");
MODULE_DESCRIPTION("Samsung S5PV210 UART Driver support");
MODULE_AUTHOR("Thomas Abraham <thomas.ab@samsung.com>");
