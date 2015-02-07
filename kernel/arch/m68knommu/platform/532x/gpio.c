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
 * Coldfire generic GPIO support
 *
 * (C) Copyright 2009, Steven King <sfking@fdwdc.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
*/

#include <linux/kernel.h>
#include <linux/init.h>

#include <asm/coldfire.h>
#include <asm/mcfsim.h>
#include <asm/mcfgpio.h>

static struct mcf_gpio_chip mcf_gpio_chips[] = {
	{
		.gpio_chip			= {
			.label			= "PIRQ",
			.request		= mcf_gpio_request,
			.free			= mcf_gpio_free,
			.direction_input	= mcf_gpio_direction_input,
			.direction_output	= mcf_gpio_direction_output,
			.get			= mcf_gpio_get_value,
			.set			= mcf_gpio_set_value,
			.ngpio			= 8,
		},
		.pddr				= MCFEPORT_EPDDR,
		.podr				= MCFEPORT_EPDR,
		.ppdr				= MCFEPORT_EPPDR,
	},
	{
		.gpio_chip			= {
			.label			= "FECH",
			.request		= mcf_gpio_request,
			.free			= mcf_gpio_free,
			.direction_input	= mcf_gpio_direction_input,
			.direction_output	= mcf_gpio_direction_output,
			.get			= mcf_gpio_get_value,
			.set			= mcf_gpio_set_value_fast,
			.base			= 8,
			.ngpio			= 8,
		},
		.pddr				= MCFGPIO_PDDR_FECH,
		.podr				= MCFGPIO_PODR_FECH,
		.ppdr				= MCFGPIO_PPDSDR_FECH,
		.setr				= MCFGPIO_PPDSDR_FECH,
		.clrr				= MCFGPIO_PCLRR_FECH,
	},
	{
		.gpio_chip			= {
			.label			= "FECL",
			.request		= mcf_gpio_request,
			.free			= mcf_gpio_free,
			.direction_input	= mcf_gpio_direction_input,
			.direction_output	= mcf_gpio_direction_output,
			.get			= mcf_gpio_get_value,
			.set			= mcf_gpio_set_value_fast,
			.base			= 16,
			.ngpio			= 8,
		},
		.pddr				= MCFGPIO_PDDR_FECL,
		.podr				= MCFGPIO_PODR_FECL,
		.ppdr				= MCFGPIO_PPDSDR_FECL,
		.setr				= MCFGPIO_PPDSDR_FECL,
		.clrr				= MCFGPIO_PCLRR_FECL,
	},
	{
		.gpio_chip			= {
			.label			= "SSI",
			.request		= mcf_gpio_request,
			.free			= mcf_gpio_free,
			.direction_input	= mcf_gpio_direction_input,
			.direction_output	= mcf_gpio_direction_output,
			.get			= mcf_gpio_get_value,
			.set			= mcf_gpio_set_value_fast,
			.base			= 24,
			.ngpio			= 5,
		},
		.pddr				= MCFGPIO_PDDR_SSI,
		.podr				= MCFGPIO_PODR_SSI,
		.ppdr				= MCFGPIO_PPDSDR_SSI,
		.setr				= MCFGPIO_PPDSDR_SSI,
		.clrr				= MCFGPIO_PCLRR_SSI,
	},
	{
		.gpio_chip			= {
			.label			= "BUSCTL",
			.request		= mcf_gpio_request,
			.free			= mcf_gpio_free,
			.direction_input	= mcf_gpio_direction_input,
			.direction_output	= mcf_gpio_direction_output,
			.get			= mcf_gpio_get_value,
			.set			= mcf_gpio_set_value_fast,
			.base			= 32,
			.ngpio			= 4,
		},
		.pddr				= MCFGPIO_PDDR_BUSCTL,
		.podr				= MCFGPIO_PODR_BUSCTL,
		.ppdr				= MCFGPIO_PPDSDR_BUSCTL,
		.setr				= MCFGPIO_PPDSDR_BUSCTL,
		.clrr				= MCFGPIO_PCLRR_BUSCTL,
	},
	{
		.gpio_chip			= {
			.label			= "BE",
			.request		= mcf_gpio_request,
			.free			= mcf_gpio_free,
			.direction_input	= mcf_gpio_direction_input,
			.direction_output	= mcf_gpio_direction_output,
			.get			= mcf_gpio_get_value,
			.set			= mcf_gpio_set_value_fast,
			.base			= 40,
			.ngpio			= 4,
		},
		.pddr				= MCFGPIO_PDDR_BE,
		.podr				= MCFGPIO_PODR_BE,
		.ppdr				= MCFGPIO_PPDSDR_BE,
		.setr				= MCFGPIO_PPDSDR_BE,
		.clrr				= MCFGPIO_PCLRR_BE,
	},
	{
		.gpio_chip			= {
			.label			= "CS",
			.request		= mcf_gpio_request,
			.free			= mcf_gpio_free,
			.direction_input	= mcf_gpio_direction_input,
			.direction_output	= mcf_gpio_direction_output,
			.get			= mcf_gpio_get_value,
			.set			= mcf_gpio_set_value_fast,
			.base			= 49,
			.ngpio			= 5,
		},
		.pddr				= MCFGPIO_PDDR_CS,
		.podr				= MCFGPIO_PODR_CS,
		.ppdr				= MCFGPIO_PPDSDR_CS,
		.setr				= MCFGPIO_PPDSDR_CS,
		.clrr				= MCFGPIO_PCLRR_CS,
	},
	{
		.gpio_chip			= {
			.label			= "PWM",
			.request		= mcf_gpio_request,
			.free			= mcf_gpio_free,
			.direction_input	= mcf_gpio_direction_input,
			.direction_output	= mcf_gpio_direction_output,
			.get			= mcf_gpio_get_value,
			.set			= mcf_gpio_set_value_fast,
			.base			= 58,
			.ngpio			= 4,
		},
		.pddr				= MCFGPIO_PDDR_PWM,
		.podr				= MCFGPIO_PODR_PWM,
		.ppdr				= MCFGPIO_PPDSDR_PWM,
		.setr				= MCFGPIO_PPDSDR_PWM,
		.clrr				= MCFGPIO_PCLRR_PWM,
	},
	{
		.gpio_chip			= {
			.label			= "FECI2C",
			.request		= mcf_gpio_request,
			.free			= mcf_gpio_free,
			.direction_input	= mcf_gpio_direction_input,
			.direction_output	= mcf_gpio_direction_output,
			.get			= mcf_gpio_get_value,
			.set			= mcf_gpio_set_value_fast,
			.base			= 64,
			.ngpio			= 4,
		},
		.pddr				= MCFGPIO_PDDR_FECI2C,
		.podr				= MCFGPIO_PODR_FECI2C,
		.ppdr				= MCFGPIO_PPDSDR_FECI2C,
		.setr				= MCFGPIO_PPDSDR_FECI2C,
		.clrr				= MCFGPIO_PCLRR_FECI2C,
	},
	{
		.gpio_chip			= {
			.label			= "UART",
			.request		= mcf_gpio_request,
			.free			= mcf_gpio_free,
			.direction_input	= mcf_gpio_direction_input,
			.direction_output	= mcf_gpio_direction_output,
			.get			= mcf_gpio_get_value,
			.set			= mcf_gpio_set_value_fast,
			.base			= 72,
			.ngpio			= 8,
		},
		.pddr				= MCFGPIO_PDDR_UART,
		.podr				= MCFGPIO_PODR_UART,
		.ppdr				= MCFGPIO_PPDSDR_UART,
		.setr				= MCFGPIO_PPDSDR_UART,
		.clrr				= MCFGPIO_PCLRR_UART,
	},
	{
		.gpio_chip			= {
			.label			= "QSPI",
			.request		= mcf_gpio_request,
			.free			= mcf_gpio_free,
			.direction_input	= mcf_gpio_direction_input,
			.direction_output	= mcf_gpio_direction_output,
			.get			= mcf_gpio_get_value,
			.set			= mcf_gpio_set_value_fast,
			.base			= 80,
			.ngpio			= 6,
		},
		.pddr				= MCFGPIO_PDDR_QSPI,
		.podr				= MCFGPIO_PODR_QSPI,
		.ppdr				= MCFGPIO_PPDSDR_QSPI,
		.setr				= MCFGPIO_PPDSDR_QSPI,
		.clrr				= MCFGPIO_PCLRR_QSPI,
	},
	{
		.gpio_chip			= {
			.label			= "TIMER",
			.request		= mcf_gpio_request,
			.free			= mcf_gpio_free,
			.direction_input	= mcf_gpio_direction_input,
			.direction_output	= mcf_gpio_direction_output,
			.get			= mcf_gpio_get_value,
			.set			= mcf_gpio_set_value_fast,
			.base			= 88,
			.ngpio			= 4,
		},
		.pddr				= MCFGPIO_PDDR_TIMER,
		.podr				= MCFGPIO_PODR_TIMER,
		.ppdr				= MCFGPIO_PPDSDR_TIMER,
		.setr				= MCFGPIO_PPDSDR_TIMER,
		.clrr				= MCFGPIO_PCLRR_TIMER,
	},
	{
		.gpio_chip			= {
			.label			= "LCDDATAH",
			.request		= mcf_gpio_request,
			.free			= mcf_gpio_free,
			.direction_input	= mcf_gpio_direction_input,
			.direction_output	= mcf_gpio_direction_output,
			.get			= mcf_gpio_get_value,
			.set			= mcf_gpio_set_value_fast,
			.base			= 96,
			.ngpio			= 2,
		},
		.pddr				= MCFGPIO_PDDR_LCDDATAH,
		.podr				= MCFGPIO_PODR_LCDDATAH,
		.ppdr				= MCFGPIO_PPDSDR_LCDDATAH,
		.setr				= MCFGPIO_PPDSDR_LCDDATAH,
		.clrr				= MCFGPIO_PCLRR_LCDDATAH,
	},
	{
		.gpio_chip			= {
			.label			= "LCDDATAM",
			.request		= mcf_gpio_request,
			.free			= mcf_gpio_free,
			.direction_input	= mcf_gpio_direction_input,
			.direction_output	= mcf_gpio_direction_output,
			.get			= mcf_gpio_get_value,
			.set			= mcf_gpio_set_value_fast,
			.base			= 104,
			.ngpio			= 8,
		},
		.pddr				= MCFGPIO_PDDR_LCDDATAM,
		.podr				= MCFGPIO_PODR_LCDDATAM,
		.ppdr				= MCFGPIO_PPDSDR_LCDDATAM,
		.setr				= MCFGPIO_PPDSDR_LCDDATAM,
		.clrr				= MCFGPIO_PCLRR_LCDDATAM,
	},
	{
		.gpio_chip			= {
			.label			= "LCDDATAL",
			.request		= mcf_gpio_request,
			.free			= mcf_gpio_free,
			.direction_input	= mcf_gpio_direction_input,
			.direction_output	= mcf_gpio_direction_output,
			.get			= mcf_gpio_get_value,
			.set			= mcf_gpio_set_value_fast,
			.base			= 112,
			.ngpio			= 8,
		},
		.pddr				= MCFGPIO_PDDR_LCDDATAL,
		.podr				= MCFGPIO_PODR_LCDDATAL,
		.ppdr				= MCFGPIO_PPDSDR_LCDDATAL,
		.setr				= MCFGPIO_PPDSDR_LCDDATAL,
		.clrr				= MCFGPIO_PCLRR_LCDDATAL,
	},
	{
		.gpio_chip			= {
			.label			= "LCDCTLH",
			.request		= mcf_gpio_request,
			.free			= mcf_gpio_free,
			.direction_input	= mcf_gpio_direction_input,
			.direction_output	= mcf_gpio_direction_output,
			.get			= mcf_gpio_get_value,
			.set			= mcf_gpio_set_value_fast,
			.base			= 120,
			.ngpio			= 1,
		},
		.pddr				= MCFGPIO_PDDR_LCDCTLH,
		.podr				= MCFGPIO_PODR_LCDCTLH,
		.ppdr				= MCFGPIO_PPDSDR_LCDCTLH,
		.setr				= MCFGPIO_PPDSDR_LCDCTLH,
		.clrr				= MCFGPIO_PCLRR_LCDCTLH,
	},
	{
		.gpio_chip			= {
			.label			= "LCDCTLL",
			.request		= mcf_gpio_request,
			.free			= mcf_gpio_free,
			.direction_input	= mcf_gpio_direction_input,
			.direction_output	= mcf_gpio_direction_output,
			.get			= mcf_gpio_get_value,
			.set			= mcf_gpio_set_value_fast,
			.base			= 128,
			.ngpio			= 8,
		},
		.pddr				= MCFGPIO_PDDR_LCDCTLL,
		.podr				= MCFGPIO_PODR_LCDCTLL,
		.ppdr				= MCFGPIO_PPDSDR_LCDCTLL,
		.setr				= MCFGPIO_PPDSDR_LCDCTLL,
		.clrr				= MCFGPIO_PCLRR_LCDCTLL,
	},
};

static int __init mcf_gpio_init(void)
{
	unsigned i = 0;
	while (i < ARRAY_SIZE(mcf_gpio_chips))
		(void)gpiochip_add((struct gpio_chip *)&mcf_gpio_chips[i++]);
	return 0;
}

core_initcall(mcf_gpio_init);
