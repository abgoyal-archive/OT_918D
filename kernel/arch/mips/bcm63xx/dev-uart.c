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
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2008 Maxime Bizon <mbizon@freebox.fr>
 */

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <bcm63xx_cpu.h>

static struct resource uart0_resources[] = {
	{
		/* start & end filled at runtime */
		.flags		= IORESOURCE_MEM,
	},
	{
		/* start filled at runtime */
		.flags		= IORESOURCE_IRQ,
	},
};

static struct resource uart1_resources[] = {
	{
		/* start & end filled at runtime */
		.flags		= IORESOURCE_MEM,
	},
	{
		/* start filled at runtime */
		.flags		= IORESOURCE_IRQ,
	},
};

static struct platform_device bcm63xx_uart_devices[] = {
	{
		.name		= "bcm63xx_uart",
		.id		= 0,
		.num_resources	= ARRAY_SIZE(uart0_resources),
		.resource	= uart0_resources,
	},

	{
		.name		= "bcm63xx_uart",
		.id		= 1,
		.num_resources	= ARRAY_SIZE(uart1_resources),
		.resource	= uart1_resources,
	}
};

int __init bcm63xx_uart_register(unsigned int id)
{
	if (id >= ARRAY_SIZE(bcm63xx_uart_devices))
		return -ENODEV;

	if (id == 1 && !BCMCPU_IS_6358())
		return -ENODEV;

	if (id == 0) {
		uart0_resources[0].start = bcm63xx_regset_address(RSET_UART0);
		uart0_resources[0].end = uart0_resources[0].start +
			RSET_UART_SIZE - 1;
		uart0_resources[1].start = bcm63xx_get_irq_number(IRQ_UART0);
	}

	if (id == 1) {
		uart1_resources[0].start = bcm63xx_regset_address(RSET_UART1);
		uart1_resources[0].end = uart1_resources[0].start +
			RSET_UART_SIZE - 1;
		uart1_resources[1].start = bcm63xx_get_irq_number(IRQ_UART1);
	}

	return platform_device_register(&bcm63xx_uart_devices[id]);
}
