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
 * arch/arm/mach-spear6xx/spear6xx.c
 *
 * SPEAr6XX machines common source file
 *
 * Copyright (C) 2009 ST Microelectronics
 * Rajeev Kumar<rajeev-dlh.kumar@st.com>
 *
 * This file is licensed under the terms of the GNU General Public
 * License version 2. This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */

#include <linux/types.h>
#include <linux/amba/pl061.h>
#include <linux/ptrace.h>
#include <linux/io.h>
#include <asm/hardware/vic.h>
#include <asm/irq.h>
#include <asm/mach/arch.h>
#include <mach/irqs.h>
#include <mach/generic.h>
#include <mach/spear.h>

/* Add spear6xx machines common devices here */
/* uart device registeration */
struct amba_device uart_device[] = {
	{
		.dev = {
			.init_name = "uart0",
		},
		.res = {
			.start = SPEAR6XX_ICM1_UART0_BASE,
			.end = SPEAR6XX_ICM1_UART0_BASE +
				SPEAR6XX_ICM1_UART0_SIZE - 1,
			.flags = IORESOURCE_MEM,
		},
		.irq = {IRQ_UART_0, NO_IRQ},
	}, {
		.dev = {
			.init_name = "uart1",
		},
		.res = {
			.start = SPEAR6XX_ICM1_UART1_BASE,
			.end = SPEAR6XX_ICM1_UART1_BASE +
				SPEAR6XX_ICM1_UART1_SIZE - 1,
			.flags = IORESOURCE_MEM,
		},
		.irq = {IRQ_UART_1, NO_IRQ},
	}
};

/* gpio device registeration */
static struct pl061_platform_data gpio_plat_data[] = {
	{
		.gpio_base	= 0,
		.irq_base	= SPEAR_GPIO0_INT_BASE,
	}, {
		.gpio_base	= 8,
		.irq_base	= SPEAR_GPIO1_INT_BASE,
	}, {
		.gpio_base	= 16,
		.irq_base	= SPEAR_GPIO2_INT_BASE,
	},
};

struct amba_device gpio_device[] = {
	{
		.dev = {
			.init_name = "gpio0",
			.platform_data = &gpio_plat_data[0],
		},
		.res = {
			.start = SPEAR6XX_CPU_GPIO_BASE,
			.end = SPEAR6XX_CPU_GPIO_BASE +
				SPEAR6XX_CPU_GPIO_SIZE - 1,
			.flags = IORESOURCE_MEM,
		},
		.irq = {IRQ_LOCAL_GPIO, NO_IRQ},
	}, {
		.dev = {
			.init_name = "gpio1",
			.platform_data = &gpio_plat_data[1],
		},
		.res = {
			.start = SPEAR6XX_ICM3_GPIO_BASE,
			.end = SPEAR6XX_ICM3_GPIO_BASE +
				SPEAR6XX_ICM3_GPIO_SIZE - 1,
			.flags = IORESOURCE_MEM,
		},
		.irq = {IRQ_BASIC_GPIO, NO_IRQ},
	}, {
		.dev = {
			.init_name = "gpio2",
			.platform_data = &gpio_plat_data[2],
		},
		.res = {
			.start = SPEAR6XX_ICM2_GPIO_BASE,
			.end = SPEAR6XX_ICM2_GPIO_BASE +
				SPEAR6XX_ICM2_GPIO_SIZE - 1,
			.flags = IORESOURCE_MEM,
		},
		.irq = {IRQ_APPL_GPIO, NO_IRQ},
	}
};

/* This will add devices, and do machine specific tasks */
void __init spear6xx_init(void)
{
	/* nothing to do for now */
}

/* This will initialize vic */
void __init spear6xx_init_irq(void)
{
	vic_init((void __iomem *)VA_SPEAR6XX_CPU_VIC_PRI_BASE, 0, ~0, 0);
	vic_init((void __iomem *)VA_SPEAR6XX_CPU_VIC_SEC_BASE, 32, ~0, 0);
}

/* Following will create static virtual/physical mappings */
static struct map_desc spear6xx_io_desc[] __initdata = {
	{
		.virtual	= VA_SPEAR6XX_ICM1_UART0_BASE,
		.pfn		= __phys_to_pfn(SPEAR6XX_ICM1_UART0_BASE),
		.length		= SPEAR6XX_ICM1_UART0_SIZE,
		.type		= MT_DEVICE
	}, {
		.virtual	= VA_SPEAR6XX_CPU_VIC_PRI_BASE,
		.pfn		= __phys_to_pfn(SPEAR6XX_CPU_VIC_PRI_BASE),
		.length		= SPEAR6XX_CPU_VIC_PRI_SIZE,
		.type		= MT_DEVICE
	}, {
		.virtual	= VA_SPEAR6XX_CPU_VIC_SEC_BASE,
		.pfn		= __phys_to_pfn(SPEAR6XX_CPU_VIC_SEC_BASE),
		.length		= SPEAR6XX_CPU_VIC_SEC_SIZE,
		.type		= MT_DEVICE
	}, {
		.virtual	= VA_SPEAR6XX_ICM3_SYS_CTRL_BASE,
		.pfn		= __phys_to_pfn(SPEAR6XX_ICM3_SYS_CTRL_BASE),
		.length		= SPEAR6XX_ICM3_MISC_REG_BASE,
		.type		= MT_DEVICE
	}, {
		.virtual	= VA_SPEAR6XX_ICM3_MISC_REG_BASE,
		.pfn		= __phys_to_pfn(SPEAR6XX_ICM3_MISC_REG_BASE),
		.length		= SPEAR6XX_ICM3_MISC_REG_SIZE,
		.type		= MT_DEVICE
	},
};

/* This will create static memory mapping for selected devices */
void __init spear6xx_map_io(void)
{
	iotable_init(spear6xx_io_desc, ARRAY_SIZE(spear6xx_io_desc));

	/* This will initialize clock framework */
	clk_init();
}