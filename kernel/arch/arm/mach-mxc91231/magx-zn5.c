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
 * Copyright 2009 Dmitriy Taychenachev <dimichxp@gmail.com>
 *
 * This file is released under the GPLv2 or later.
 */

#include <linux/irq.h>
#include <linux/init.h>
#include <linux/device.h>

#include <asm/mach-types.h>
#include <asm/mach/time.h>
#include <asm/mach/arch.h>

#include <mach/common.h>
#include <mach/hardware.h>
#include <mach/iomux-mxc91231.h>
#include <mach/mmc.h>
#include <mach/imx-uart.h>

#include "devices.h"

static struct imxuart_platform_data uart_pdata = {
};

static struct imxmmc_platform_data sdhc_pdata = {
};

static void __init zn5_init(void)
{
	pm_power_off = mxc91231_power_off;

	mxc_iomux_alloc_pin(MXC91231_PIN_SP_USB_DAT_VP__RXD2, "uart2-rx");
	mxc_iomux_alloc_pin(MXC91231_PIN_SP_USB_SE0_VM__TXD2, "uart2-tx");

	mxc_register_device(&mxc_uart_device1, &uart_pdata);
	mxc_register_device(&mxc_uart_device0, &uart_pdata);

	mxc_register_device(&mxc_sdhc_device0, &sdhc_pdata);

	mxc_register_device(&mxc_wdog_device0, NULL);

	return;
}

static void __init zn5_timer_init(void)
{
	mxc91231_clocks_init(26000000); /* 26mhz ckih */
}

struct sys_timer zn5_timer = {
	.init = zn5_timer_init,
};

MACHINE_START(MAGX_ZN5, "Motorola Zn5")
	.phys_io	= MXC91231_AIPS1_BASE_ADDR,
	.io_pg_offst	= ((MXC91231_AIPS1_BASE_ADDR_VIRT) >> 18) & 0xfffc,
	.boot_params	= MXC91231_PHYS_OFFSET + 0x100,
	.map_io		= mxc91231_map_io,
	.init_irq	= mxc91231_init_irq,
	.timer		= &zn5_timer,
	.init_machine	= zn5_init,
MACHINE_END
