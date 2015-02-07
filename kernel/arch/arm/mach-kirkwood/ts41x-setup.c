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
 *
 * QNAP TS-410, TS-410U, TS-419P and TS-419U Turbo NAS Board Setup
 *
 * Copyright (C) 2009  Martin Michlmayr <tbm@cyrius.com>
 * Copyright (C) 2008  Byron Bradley <byron.bbradley@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/i2c.h>
#include <linux/mv643xx_eth.h>
#include <linux/ata_platform.h>
#include <linux/gpio_keys.h>
#include <linux/input.h>
#include <asm/mach-types.h>
#include <asm/mach/arch.h>
#include <mach/kirkwood.h>
#include "common.h"
#include "mpp.h"
#include "tsx1x-common.h"

static struct i2c_board_info __initdata qnap_ts41x_i2c_rtc = {
	I2C_BOARD_INFO("s35390a", 0x30),
};

static struct mv643xx_eth_platform_data qnap_ts41x_ge00_data = {
	.phy_addr	= MV643XX_ETH_PHY_ADDR(8),
};

static struct mv643xx_eth_platform_data qnap_ts41x_ge01_data = {
	.phy_addr	= MV643XX_ETH_PHY_ADDR(0),
};

static struct mv_sata_platform_data qnap_ts41x_sata_data = {
	.n_ports	= 2,
};

static struct gpio_keys_button qnap_ts41x_buttons[] = {
	{
		.code		= KEY_COPY,
		.gpio		= 43,
		.desc		= "USB Copy",
		.active_low	= 1,
	},
	{
		.code		= KEY_RESTART,
		.gpio		= 37,
		.desc		= "Reset",
		.active_low	= 1,
	},
};

static struct gpio_keys_platform_data qnap_ts41x_button_data = {
	.buttons	= qnap_ts41x_buttons,
	.nbuttons	= ARRAY_SIZE(qnap_ts41x_buttons),
};

static struct platform_device qnap_ts41x_button_device = {
	.name		= "gpio-keys",
	.id		= -1,
	.num_resources	= 0,
	.dev		= {
		.platform_data	= &qnap_ts41x_button_data,
	}
};

static unsigned int qnap_ts41x_mpp_config[] __initdata = {
	MPP0_SPI_SCn,
	MPP1_SPI_MOSI,
	MPP2_SPI_SCK,
	MPP3_SPI_MISO,
	MPP6_SYSRST_OUTn,
	MPP7_PEX_RST_OUTn,
	MPP8_TW_SDA,
	MPP9_TW_SCK,
	MPP10_UART0_TXD,
	MPP11_UART0_RXD,
	MPP13_UART1_TXD,	/* PIC controller */
	MPP14_UART1_RXD,	/* PIC controller */
	MPP15_SATA0_ACTn,
	MPP16_SATA1_ACTn,
	MPP20_GE1_0,
	MPP21_GE1_1,
	MPP22_GE1_2,
	MPP23_GE1_3,
	MPP24_GE1_4,
	MPP25_GE1_5,
	MPP26_GE1_6,
	MPP27_GE1_7,
	MPP30_GE1_10,
	MPP31_GE1_11,
	MPP32_GE1_12,
	MPP33_GE1_13,
	MPP36_GPIO,		/* RAM: 0: 256 MB, 1: 512 MB */
	MPP37_GPIO,		/* Reset button */
	MPP43_GPIO,		/* USB Copy button */
	MPP44_GPIO,		/* Board ID: 0: TS-419U, 1: TS-419 */
	MPP45_GPIO,		/* JP1: 0: console, 1: LCD */
	MPP46_GPIO,		/* External SATA HDD1 error indicator */
	MPP47_GPIO,		/* External SATA HDD2 error indicator */
	MPP48_GPIO,		/* External SATA HDD3 error indicator */
	MPP49_GPIO,		/* External SATA HDD4 error indicator */
	0
};

static void __init qnap_ts41x_init(void)
{
	/*
	 * Basic setup. Needs to be called early.
	 */
	kirkwood_init();
	kirkwood_mpp_conf(qnap_ts41x_mpp_config);

	kirkwood_uart0_init();
	kirkwood_uart1_init(); /* A PIC controller is connected here. */
	qnap_tsx1x_register_flash();
	kirkwood_i2c_init();
	i2c_register_board_info(0, &qnap_ts41x_i2c_rtc, 1);
	kirkwood_ge00_init(&qnap_ts41x_ge00_data);
	kirkwood_ge01_init(&qnap_ts41x_ge01_data);
	kirkwood_sata_init(&qnap_ts41x_sata_data);
	kirkwood_ehci_init();
	platform_device_register(&qnap_ts41x_button_device);

	pm_power_off = qnap_tsx1x_power_off;

}

static int __init ts41x_pci_init(void)
{
	if (machine_is_ts41x())
		kirkwood_pcie_init();

   return 0;
}
subsys_initcall(ts41x_pci_init);

MACHINE_START(TS41X, "QNAP TS-41x")
	/* Maintainer: Martin Michlmayr <tbm@cyrius.com> */
	.phys_io	= KIRKWOOD_REGS_PHYS_BASE,
	.io_pg_offst	= ((KIRKWOOD_REGS_VIRT_BASE) >> 18) & 0xfffc,
	.boot_params	= 0x00000100,
	.init_machine	= qnap_ts41x_init,
	.map_io		= kirkwood_map_io,
	.init_irq	= kirkwood_init_irq,
	.timer		= &kirkwood_timer,
MACHINE_END
