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
 * Copyright (C) ST-Ericsson SA 2010
 *
 * Author: Rabin Vincent <rabin.vincent@stericsson.com> for ST-Ericsson
 * License terms: GNU General Public License (GPL) version 2
 */

#include <linux/platform_device.h>
#include <linux/amba/bus.h>
#include <linux/io.h>
#include <linux/clk.h>

#include <asm/hardware/cache-l2x0.h>
#include <asm/hardware/gic.h>
#include <asm/mach/map.h>
#include <asm/localtimer.h>

#include <plat/mtu.h>
#include <mach/hardware.h>
#include <mach/setup.h>
#include <mach/devices.h>

#include "clock.h"

static struct map_desc ux500_io_desc[] __initdata = {
	__IO_DEV_DESC(UX500_UART0_BASE, SZ_4K),
	__IO_DEV_DESC(UX500_UART2_BASE, SZ_4K),

	__IO_DEV_DESC(UX500_GIC_CPU_BASE, SZ_4K),
	__IO_DEV_DESC(UX500_GIC_DIST_BASE, SZ_4K),
	__IO_DEV_DESC(UX500_L2CC_BASE, SZ_4K),
	__IO_DEV_DESC(UX500_TWD_BASE, SZ_4K),
	__IO_DEV_DESC(UX500_SCU_BASE, SZ_4K),

	__IO_DEV_DESC(UX500_CLKRST1_BASE, SZ_4K),
	__IO_DEV_DESC(UX500_CLKRST2_BASE, SZ_4K),
	__IO_DEV_DESC(UX500_CLKRST3_BASE, SZ_4K),
	__IO_DEV_DESC(UX500_CLKRST5_BASE, SZ_4K),
	__IO_DEV_DESC(UX500_CLKRST6_BASE, SZ_4K),

	__IO_DEV_DESC(UX500_MTU0_BASE, SZ_4K),
	__IO_DEV_DESC(UX500_MTU1_BASE, SZ_4K),

	__IO_DEV_DESC(UX500_BACKUPRAM0_BASE, SZ_8K),
};

static struct amba_device *ux500_amba_devs[] __initdata = {
	&ux500_pl031_device,
};

void __init ux500_map_io(void)
{
	iotable_init(ux500_io_desc, ARRAY_SIZE(ux500_io_desc));
}

void __init ux500_init_devices(void)
{
	amba_add_devices(ux500_amba_devs, ARRAY_SIZE(ux500_amba_devs));
}

void __init ux500_init_irq(void)
{
	gic_dist_init(0, __io_address(UX500_GIC_DIST_BASE), 29);
	gic_cpu_init(0, __io_address(UX500_GIC_CPU_BASE));

	/*
	 * Init clocks here so that they are available for system timer
	 * initialization.
	 */
	clk_init();
}

#ifdef CONFIG_CACHE_L2X0
static int ux500_l2x0_init(void)
{
	void __iomem *l2x0_base;

	l2x0_base = __io_address(UX500_L2CC_BASE);

	/* 64KB way size, 8 way associativity, force WA */
	l2x0_init(l2x0_base, 0x3e060000, 0xc0000fff);

	return 0;
}
early_initcall(ux500_l2x0_init);
#endif

static void __init ux500_timer_init(void)
{
#ifdef CONFIG_LOCAL_TIMERS
	/* Setup the local timer base */
	twd_base = __io_address(UX500_TWD_BASE);
#endif
	/* Setup the MTU base */
	if (cpu_is_u8500ed())
		mtu_base = __io_address(U8500_MTU0_BASE_ED);
	else
		mtu_base = __io_address(UX500_MTU0_BASE);

	nmdk_timer_init();
}

struct sys_timer ux500_timer = {
	.init	= ux500_timer_init,
};
