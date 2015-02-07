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
 * arch/arm/mach-imx/mach-mx1ads.c
 *
 * Initially based on:
 *	linux-2.6.7-imx/arch/arm/mach-imx/scb9328.c
 *	Copyright (c) 2004 Sascha Hauer <sascha@saschahauer.de>
 *
 * 2004 (c) MontaVista Software, Inc.
 *
 * This file is licensed under the terms of the GNU General Public
 * License version 2. This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */

#include <linux/i2c.h>
#include <linux/i2c/pcf857x.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/mtd/physmap.h>

#include <asm/mach-types.h>
#include <asm/mach/arch.h>
#include <asm/mach/time.h>

#include <mach/common.h>
#include <mach/hardware.h>
#include <mach/i2c.h>
#include <mach/imx-uart.h>
#include <mach/iomux-mx1.h>
#include <mach/irqs.h>

#include "devices.h"

static int mx1ads_pins[] = {
	/* UART1 */
	PC9_PF_UART1_CTS,
	PC10_PF_UART1_RTS,
	PC11_PF_UART1_TXD,
	PC12_PF_UART1_RXD,
	/* UART2 */
	PB28_PF_UART2_CTS,
	PB29_PF_UART2_RTS,
	PB30_PF_UART2_TXD,
	PB31_PF_UART2_RXD,
	/* I2C */
	PA15_PF_I2C_SDA,
	PA16_PF_I2C_SCL,
	/* SPI */
	PC13_PF_SPI1_SPI_RDY,
	PC14_PF_SPI1_SCLK,
	PC15_PF_SPI1_SS,
	PC16_PF_SPI1_MISO,
	PC17_PF_SPI1_MOSI,
};

/*
 * UARTs platform data
 */

static struct imxuart_platform_data uart_pdata[] = {
	{
		.flags = IMXUART_HAVE_RTSCTS,
	}, {
		.flags = IMXUART_HAVE_RTSCTS,
	},
};

/*
 * Physmap flash
 */

static struct physmap_flash_data mx1ads_flash_data = {
	.width		= 4,		/* bankwidth in bytes */
};

static struct resource flash_resource = {
	.start	= IMX_CS0_PHYS,
	.end	= IMX_CS0_PHYS + SZ_32M - 1,
	.flags	= IORESOURCE_MEM,
};

static struct platform_device flash_device = {
	.name	= "physmap-flash",
	.id	= 0,
	.resource = &flash_resource,
	.num_resources = 1,
};

/*
 * I2C
 */
static struct pcf857x_platform_data pcf857x_data[] = {
	{
		.gpio_base = 4 * 32,
	}, {
		.gpio_base = 4 * 32 + 16,
	}
};

static struct imxi2c_platform_data mx1ads_i2c_data = {
	.bitrate = 100000,
};

static struct i2c_board_info mx1ads_i2c_devices[] = {
	{
		I2C_BOARD_INFO("pcf8575", 0x22),
		.platform_data = &pcf857x_data[0],
	}, {
		I2C_BOARD_INFO("pcf8575", 0x24),
		.platform_data = &pcf857x_data[1],
	},
};

/*
 * Board init
 */
static void __init mx1ads_init(void)
{
	mxc_gpio_setup_multiple_pins(mx1ads_pins,
		ARRAY_SIZE(mx1ads_pins), "mx1ads");

	/* UART */
	mxc_register_device(&imx_uart1_device, &uart_pdata[0]);
	mxc_register_device(&imx_uart2_device, &uart_pdata[1]);

	/* Physmap flash */
	mxc_register_device(&flash_device, &mx1ads_flash_data);

	/* I2C */
	i2c_register_board_info(0, mx1ads_i2c_devices,
				ARRAY_SIZE(mx1ads_i2c_devices));

	mxc_register_device(&imx_i2c_device, &mx1ads_i2c_data);
}

static void __init mx1ads_timer_init(void)
{
	mx1_clocks_init(32000);
}

struct sys_timer mx1ads_timer = {
	.init	= mx1ads_timer_init,
};

MACHINE_START(MX1ADS, "Freescale MX1ADS")
	/* Maintainer: Sascha Hauer, Pengutronix */
	.phys_io	= IMX_IO_PHYS,
	.io_pg_offst	= (IMX_IO_BASE >> 18) & 0xfffc,
	.boot_params	= MX1_PHYS_OFFSET + 0x100,
	.map_io		= mx1_map_io,
	.init_irq	= mx1_init_irq,
	.timer		= &mx1ads_timer,
	.init_machine	= mx1ads_init,
MACHINE_END

MACHINE_START(MXLADS, "Freescale MXLADS")
	.phys_io	= IMX_IO_PHYS,
	.io_pg_offst	= (IMX_IO_BASE >> 18) & 0xfffc,
	.boot_params	= MX1_PHYS_OFFSET + 0x100,
	.map_io		= mx1_map_io,
	.init_irq	= mx1_init_irq,
	.timer		= &mx1ads_timer,
	.init_machine	= mx1ads_init,
MACHINE_END
