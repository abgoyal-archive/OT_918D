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
 * arch/arm/mach-mv78xx0/buffalo-wxl-setup.c
 *
 * Buffalo WXL (Terastation Duo) Setup routines
 *
 * sebastien requiem <sebastien@requiem.fr>
 *
 * This file is licensed under the terms of the GNU General Public
 * License version 2.  This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/ata_platform.h>
#include <linux/mv643xx_eth.h>
#include <linux/ethtool.h>
#include <linux/i2c.h>
#include <mach/mv78xx0.h>
#include <asm/mach-types.h>
#include <asm/mach/arch.h>
#include "common.h"
#include "mpp.h"


/* This arch has 2 Giga Ethernet */

static struct mv643xx_eth_platform_data db78x00_ge00_data = {
	.phy_addr	= MV643XX_ETH_PHY_ADDR(0),
};

static struct mv643xx_eth_platform_data db78x00_ge01_data = {
	.phy_addr	= MV643XX_ETH_PHY_ADDR(8),
};


/* 2 SATA controller supporting HotPlug */

static struct mv_sata_platform_data db78x00_sata_data = {
	.n_ports	= 2,
};

static struct i2c_board_info __initdata db78x00_i2c_rtc = {
	I2C_BOARD_INFO("ds1338", 0x68),
};


static unsigned int wxl_mpp_config[] __initdata = {
	MPP0_GE1_TXCLK,
	MPP1_GE1_TXCTL,
	MPP2_GE1_RXCTL,
	MPP3_GE1_RXCLK,
	MPP4_GE1_TXD0,
	MPP5_GE1_TXD1,
	MPP6_GE1_TXD2,
	MPP7_GE1_TXD3,
	MPP8_GE1_RXD0,
	MPP9_GE1_RXD1,
	MPP10_GE1_RXD2,
	MPP11_GE1_RXD3,
	MPP12_GPIO,
	MPP13_SYSRST_OUTn,
	MPP14_SATA1_ACTn,
	MPP15_SATA0_ACTn,
	MPP16_GPIO,
	MPP17_GPIO,
	MPP18_GPIO,
	MPP19_GPIO,
	MPP20_GPIO,
	MPP21_GPIO,
	MPP22_GPIO,
	MPP23_GPIO,
	MPP24_UA2_TXD,
	MPP25_UA2_RXD,
	MPP26_UA2_CTSn,
	MPP27_UA2_RTSn,
	MPP28_GPIO,
	MPP29_SYSRST_OUTn,
	MPP30_GPIO,
	MPP31_GPIO,
	MPP32_GPIO,
	MPP33_GPIO,
	MPP34_GPIO,
	MPP35_GPIO,
	MPP36_GPIO,
	MPP37_GPIO,
	MPP38_GPIO,
	MPP39_GPIO,
	MPP40_UNUSED,
	MPP41_UNUSED,
	MPP42_UNUSED,
	MPP43_UNUSED,
	MPP44_UNUSED,
	MPP45_UNUSED,
	MPP46_UNUSED,
	MPP47_UNUSED,
	MPP48_SATA1_ACTn,
	MPP49_SATA0_ACTn,
	0
};


static void __init wxl_init(void)
{
	/*
	 * Basic MV78xx0 setup. Needs to be called early.
	 */
	mv78xx0_init();
	mv78xx0_mpp_conf(wxl_mpp_config);

	/*
	 * Partition on-chip peripherals between the two CPU cores.
	 */
	mv78xx0_ehci0_init();
	mv78xx0_ehci1_init();
	mv78xx0_ehci2_init();
	mv78xx0_ge00_init(&db78x00_ge00_data);
	mv78xx0_ge01_init(&db78x00_ge01_data);
	mv78xx0_sata_init(&db78x00_sata_data);
	mv78xx0_uart0_init();
	mv78xx0_uart1_init();
	mv78xx0_uart2_init();
	mv78xx0_uart3_init();
	mv78xx0_i2c_init();
	i2c_register_board_info(0, &db78x00_i2c_rtc, 1);
}

static int __init wxl_pci_init(void)
{
	if (machine_is_terastation_wxl()) {
		/*
		 * Assign the x16 PCIe slot on the board to CPU core
		 * #0, and let CPU core #1 have the four x1 slots.
		 */
		if (mv78xx0_core_index() == 0)
			mv78xx0_pcie_init(0, 1);
		else
			mv78xx0_pcie_init(1, 0);
	}

	return 0;
}
subsys_initcall(wxl_pci_init);

MACHINE_START(TERASTATION_WXL, "Buffalo Nas WXL")
	/* Maintainer: Sebastien Requiem <sebastien@requiem.fr> */
	.phys_io	= MV78XX0_REGS_PHYS_BASE,
	.io_pg_offst	= ((MV78XX0_REGS_VIRT_BASE) >> 18) & 0xfffc,
	.boot_params	= 0x00000100,
	.init_machine	= wxl_init,
	.map_io		= mv78xx0_map_io,
	.init_irq	= mv78xx0_init_irq,
	.timer		= &mv78xx0_timer,
MACHINE_END
