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

#include <linux/kernel.h>
#include <linux/pci.h>
#include <linux/platform_device.h>
#include <linux/mtd/physmap.h>
#include <linux/spi/flash.h>
#include <linux/spi/spi.h>
#include <linux/spi/orion_spi.h>
#include <linux/serial_reg.h>
#include <mach/kirkwood.h>
#include "common.h"

/*
 * QNAP TS-x1x Boards flash
 */

/****************************************************************************
 * 16 MiB NOR flash. The struct mtd_partition is not in the same order as the
 *     partitions on the device because we want to keep compatability with
 *     the QNAP firmware.
 * Layout as used by QNAP:
 *  0x00000000-0x00080000 : "U-Boot"
 *  0x00200000-0x00400000 : "Kernel"
 *  0x00400000-0x00d00000 : "RootFS"
 *  0x00d00000-0x01000000 : "RootFS2"
 *  0x00080000-0x000c0000 : "U-Boot Config"
 *  0x000c0000-0x00200000 : "NAS Config"
 *
 * We'll use "RootFS1" instead of "RootFS" to stay compatible with the layout
 * used by the QNAP TS-109/TS-209.
 *
 ***************************************************************************/

struct mtd_partition qnap_tsx1x_partitions[] = {
	{
		.name		= "U-Boot",
		.size		= 0x00080000,
		.offset		= 0,
		.mask_flags	= MTD_WRITEABLE,
	}, {
		.name		= "Kernel",
		.size		= 0x00200000,
		.offset		= 0x00200000,
	}, {
		.name		= "RootFS1",
		.size		= 0x00900000,
		.offset		= 0x00400000,
	}, {
		.name		= "RootFS2",
		.size		= 0x00300000,
		.offset		= 0x00d00000,
	}, {
		.name		= "U-Boot Config",
		.size		= 0x00040000,
		.offset		= 0x00080000,
	}, {
		.name		= "NAS Config",
		.size		= 0x00140000,
		.offset		= 0x000c0000,
	},
};

const struct flash_platform_data qnap_tsx1x_flash = {
	.type		= "m25p128",
	.name		= "spi_flash",
	.parts		= qnap_tsx1x_partitions,
	.nr_parts	= ARRAY_SIZE(qnap_tsx1x_partitions),
};

struct spi_board_info __initdata qnap_tsx1x_spi_slave_info[] = {
	{
		.modalias	= "m25p80",
		.platform_data	= &qnap_tsx1x_flash,
		.irq		= -1,
		.max_speed_hz	= 20000000,
		.bus_num	= 0,
		.chip_select	= 0,
	},
};

void __init qnap_tsx1x_register_flash(void)
{
	spi_register_board_info(qnap_tsx1x_spi_slave_info,
				ARRAY_SIZE(qnap_tsx1x_spi_slave_info));
	kirkwood_spi_init();
}


/*****************************************************************************
 * QNAP TS-x1x specific power off method via UART1-attached PIC
 ****************************************************************************/

#define UART1_REG(x)	(UART1_VIRT_BASE + ((UART_##x) << 2))

void qnap_tsx1x_power_off(void)
{
	/* 19200 baud divisor */
	const unsigned divisor = ((kirkwood_tclk + (8 * 19200)) / (16 * 19200));

	pr_info("%s: triggering power-off...\n", __func__);

	/* hijack UART1 and reset into sane state (19200,8n1) */
	writel(0x83, UART1_REG(LCR));
	writel(divisor & 0xff, UART1_REG(DLL));
	writel((divisor >> 8) & 0xff, UART1_REG(DLM));
	writel(0x03, UART1_REG(LCR));
	writel(0x00, UART1_REG(IER));
	writel(0x00, UART1_REG(FCR));
	writel(0x00, UART1_REG(MCR));

	/* send the power-off command 'A' to PIC */
	writel('A', UART1_REG(TX));
}

