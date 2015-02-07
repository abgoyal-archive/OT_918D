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

/* linux/arch/arm/plat-s5p/dev-uart.c
 *
 * Copyright (c) 2009 Samsung Electronics Co., Ltd.
 *		http://www.samsung.com/
 *
 * Base S5P UART resource and device definitions
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/interrupt.h>
#include <linux/list.h>
#include <linux/platform_device.h>

#include <asm/mach/arch.h>
#include <asm/mach/irq.h>
#include <mach/hardware.h>
#include <mach/map.h>

#include <plat/devs.h>

 /* Serial port registrations */

static struct resource s5p_uart0_resource[] = {
	[0] = {
		.start	= S5P_PA_UART0,
		.end	= S5P_PA_UART0 + S5P_SZ_UART,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= IRQ_S5P_UART_RX0,
		.end	= IRQ_S5P_UART_RX0,
		.flags	= IORESOURCE_IRQ,
	},
	[2] = {
		.start	= IRQ_S5P_UART_TX0,
		.end	= IRQ_S5P_UART_TX0,
		.flags	= IORESOURCE_IRQ,
	},
	[3] = {
		.start	= IRQ_S5P_UART_ERR0,
		.end	= IRQ_S5P_UART_ERR0,
		.flags	= IORESOURCE_IRQ,
	}
};

static struct resource s5p_uart1_resource[] = {
	[0] = {
		.start	= S5P_PA_UART1,
		.end	= S5P_PA_UART1 + S5P_SZ_UART,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= IRQ_S5P_UART_RX1,
		.end	= IRQ_S5P_UART_RX1,
		.flags	= IORESOURCE_IRQ,
	},
	[2] = {
		.start	= IRQ_S5P_UART_TX1,
		.end	= IRQ_S5P_UART_TX1,
		.flags	= IORESOURCE_IRQ,
	},
	[3] = {
		.start	= IRQ_S5P_UART_ERR1,
		.end	= IRQ_S5P_UART_ERR1,
		.flags	= IORESOURCE_IRQ,
	},
};

static struct resource s5p_uart2_resource[] = {
	[0] = {
		.start	= S5P_PA_UART2,
		.end	= S5P_PA_UART2 + S5P_SZ_UART,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= IRQ_S5P_UART_RX2,
		.end	= IRQ_S5P_UART_RX2,
		.flags	= IORESOURCE_IRQ,
	},
	[2] = {
		.start	= IRQ_S5P_UART_TX2,
		.end	= IRQ_S5P_UART_TX2,
		.flags	= IORESOURCE_IRQ,
	},
	[3] = {
		.start	= IRQ_S5P_UART_ERR2,
		.end	= IRQ_S5P_UART_ERR2,
		.flags	= IORESOURCE_IRQ,
	},
};

static struct resource s5p_uart3_resource[] = {
#if CONFIG_SERIAL_SAMSUNG_UARTS > 3
	[0] = {
		.start	= S5P_PA_UART3,
		.end	= S5P_PA_UART3 + S5P_SZ_UART,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= IRQ_S5P_UART_RX3,
		.end	= IRQ_S5P_UART_RX3,
		.flags	= IORESOURCE_IRQ,
	},
	[2] = {
		.start	= IRQ_S5P_UART_TX3,
		.end	= IRQ_S5P_UART_TX3,
		.flags	= IORESOURCE_IRQ,
	},
	[3] = {
		.start	= IRQ_S5P_UART_ERR3,
		.end	= IRQ_S5P_UART_ERR3,
		.flags	= IORESOURCE_IRQ,
	},
#endif
};

struct s3c24xx_uart_resources s5p_uart_resources[] __initdata = {
	[0] = {
		.resources	= s5p_uart0_resource,
		.nr_resources	= ARRAY_SIZE(s5p_uart0_resource),
	},
	[1] = {
		.resources	= s5p_uart1_resource,
		.nr_resources	= ARRAY_SIZE(s5p_uart1_resource),
	},
	[2] = {
		.resources	= s5p_uart2_resource,
		.nr_resources	= ARRAY_SIZE(s5p_uart2_resource),
	},
	[3] = {
		.resources	= s5p_uart3_resource,
		.nr_resources	= ARRAY_SIZE(s5p_uart3_resource),
	},
};
