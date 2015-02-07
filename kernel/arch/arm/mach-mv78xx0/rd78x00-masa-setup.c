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
 * arch/arm/mach-mv78x00/rd78x00-masa-setup.c
 *
 * Marvell RD-78x00-mASA Development Board Setup
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
#include <mach/mv78xx0.h>
#include <asm/mach-types.h>
#include <asm/mach/arch.h>
#include "common.h"

static struct mv643xx_eth_platform_data rd78x00_masa_ge00_data = {
	.phy_addr	= MV643XX_ETH_PHY_ADDR(8),
};

static struct mv643xx_eth_platform_data rd78x00_masa_ge01_data = {
	.phy_addr	= MV643XX_ETH_PHY_ADDR(9),
};

static struct mv643xx_eth_platform_data rd78x00_masa_ge10_data = {
};

static struct mv643xx_eth_platform_data rd78x00_masa_ge11_data = {
};

static struct mv_sata_platform_data rd78x00_masa_sata_data = {
	.n_ports	= 2,
};

static void __init rd78x00_masa_init(void)
{
	/*
	 * Basic MV78x00 setup. Needs to be called early.
	 */
	mv78xx0_init();

	/*
	 * Partition on-chip peripherals between the two CPU cores.
	 */
	if (mv78xx0_core_index() == 0) {
		mv78xx0_ehci0_init();
		mv78xx0_ehci1_init();
		mv78xx0_ge00_init(&rd78x00_masa_ge00_data);
		mv78xx0_ge10_init(&rd78x00_masa_ge10_data);
		mv78xx0_sata_init(&rd78x00_masa_sata_data);
		mv78xx0_uart0_init();
		mv78xx0_uart2_init();
	} else {
		mv78xx0_ehci2_init();
		mv78xx0_ge01_init(&rd78x00_masa_ge01_data);
		mv78xx0_ge11_init(&rd78x00_masa_ge11_data);
		mv78xx0_uart1_init();
		mv78xx0_uart3_init();
	}
}

static int __init rd78x00_pci_init(void)
{
	/*
	 * Assign all PCIe devices to CPU core #0.
	 */
	if (machine_is_rd78x00_masa() && mv78xx0_core_index() == 0)
		mv78xx0_pcie_init(1, 1);

	return 0;
}
subsys_initcall(rd78x00_pci_init);

MACHINE_START(RD78X00_MASA, "Marvell RD-78x00-MASA Development Board")
	/* Maintainer: Lennert Buytenhek <buytenh@marvell.com> */
	.phys_io	= MV78XX0_REGS_PHYS_BASE,
	.io_pg_offst	= ((MV78XX0_REGS_VIRT_BASE) >> 18) & 0xfffc,
	.boot_params	= 0x00000100,
	.init_machine	= rd78x00_masa_init,
	.map_io		= mv78xx0_map_io,
	.init_irq	= mv78xx0_init_irq,
	.timer		= &mv78xx0_timer,
MACHINE_END
