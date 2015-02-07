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

/* linux/arch/arm/plat-samsung/include/plat/clock-clksrc.h
 *
 * Parts taken from arch/arm/plat-s3c64xx/clock.c
 *	Copyright 2008 Openmoko, Inc.
 *	Copyright 2008 Simtec Electronics
 *		Ben Dooks <ben@simtec.co.uk>
 *		http://armlinux.simtec.co.uk/
 *
 * Copyright 2009 Ben Dooks <ben-linux@fluff.org>
 * Copyright 2009 Harald Welte
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

/**
 * struct clksrc_sources - list of sources for a given clock
 * @sources: array of pointers to clocks
 * @nr_sources: The size of @sources
 */
struct clksrc_sources {
	unsigned int	nr_sources;
	struct clk	**sources;
};

/**
 * struct clksrc_reg - register definition for clock control bits
 * @reg: pointer to the register in virtual memory.
 * @shift: the shift in bits to where the bitfield is.
 * @size: the size in bits of the bitfield.
 *
 * This specifies the size and position of the bits we are interested
 * in within the register specified by @reg.
 */
struct clksrc_reg {
	void __iomem		*reg;
	unsigned short		shift;
	unsigned short		size;
};

/**
 * struct clksrc_clk - class of clock for newer style samsung devices.
 * @clk: the standard clock representation
 * @sources: the sources for this clock
 * @reg_src: the register definition for selecting the clock's source
 * @reg_div: the register definition for the clock's output divisor
 *
 * This clock implements the features required by the newer SoCs where
 * the standard clock block provides an input mux and a post-mux divisor
 * to provide the periperhal's clock.
 *
 * The array of @sources provides the mapping of mux position to the
 * clock, and @reg_src shows the code where to modify to change the mux
 * position. The @reg_div defines how to change the divider settings on
 * the output.
 */
struct clksrc_clk {
	struct clk		clk;
	struct clksrc_sources	*sources;

	struct clksrc_reg	reg_src;
	struct clksrc_reg	reg_div;
};

/**
 * s3c_set_clksrc() - setup the clock from the register settings
 * @clk: The clock to setup.
 * @announce: true to announce the setting to printk().
 *
 * Setup the clock from the current register settings, for when the
 * kernel boots or if it is resuming from a possibly unknown state.
 */
extern void s3c_set_clksrc(struct clksrc_clk *clk, bool announce);

/**
 * s3c_register_clksrc() register clocks from an array of clksrc clocks
 * @srcs: The array of clocks to register
 * @size: The size of the @srcs array.
 *
 * Initialise and register the array of clocks described by @srcs.
 */
extern void s3c_register_clksrc(struct clksrc_clk *srcs, int size);
