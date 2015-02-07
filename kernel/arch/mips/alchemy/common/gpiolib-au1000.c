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
 *  Copyright (C) 2007-2009, OpenWrt.org, Florian Fainelli <florian@openwrt.org>
 *  	GPIOLIB support for Au1000, Au1500, Au1100, Au1550 and Au12x0.
 *
 *  This program is free software; you can redistribute	 it and/or modify it
 *  under  the terms of	 the GNU General  Public License as published by the
 *  Free Software Foundation;  either version 2 of the	License, or (at your
 *  option) any later version.
 *
 *  THIS  SOFTWARE  IS PROVIDED	  ``AS	IS'' AND   ANY	EXPRESS OR IMPLIED
 *  WARRANTIES,	  INCLUDING, BUT NOT  LIMITED  TO, THE IMPLIED WARRANTIES OF
 *  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN
 *  NO	EVENT  SHALL   THE AUTHOR  BE	 LIABLE FOR ANY	  DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 *  NOT LIMITED	  TO, PROCUREMENT OF  SUBSTITUTE GOODS	OR SERVICES; LOSS OF
 *  USE, DATA,	OR PROFITS; OR	BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 *  ANY THEORY OF LIABILITY, WHETHER IN	 CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 *  THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *  You should have received a copy of the  GNU General Public License along
 *  with this program; if not, write  to the Free Software Foundation, Inc.,
 *  675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *  Notes :
 * 	au1000 SoC have only one GPIO block : GPIO1
 * 	Au1100, Au15x0, Au12x0 have a second one : GPIO2
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/platform_device.h>
#include <linux/gpio.h>

#include <asm/mach-au1x00/au1000.h>
#include <asm/mach-au1x00/gpio.h>

static int gpio2_get(struct gpio_chip *chip, unsigned offset)
{
	return alchemy_gpio2_get_value(offset + ALCHEMY_GPIO2_BASE);
}

static void gpio2_set(struct gpio_chip *chip, unsigned offset, int value)
{
	alchemy_gpio2_set_value(offset + ALCHEMY_GPIO2_BASE, value);
}

static int gpio2_direction_input(struct gpio_chip *chip, unsigned offset)
{
	return alchemy_gpio2_direction_input(offset + ALCHEMY_GPIO2_BASE);
}

static int gpio2_direction_output(struct gpio_chip *chip, unsigned offset,
				  int value)
{
	return alchemy_gpio2_direction_output(offset + ALCHEMY_GPIO2_BASE,
						value);
}

static int gpio2_to_irq(struct gpio_chip *chip, unsigned offset)
{
	return alchemy_gpio2_to_irq(offset + ALCHEMY_GPIO2_BASE);
}


static int gpio1_get(struct gpio_chip *chip, unsigned offset)
{
	return alchemy_gpio1_get_value(offset + ALCHEMY_GPIO1_BASE);
}

static void gpio1_set(struct gpio_chip *chip,
				unsigned offset, int value)
{
	alchemy_gpio1_set_value(offset + ALCHEMY_GPIO1_BASE, value);
}

static int gpio1_direction_input(struct gpio_chip *chip, unsigned offset)
{
	return alchemy_gpio1_direction_input(offset + ALCHEMY_GPIO1_BASE);
}

static int gpio1_direction_output(struct gpio_chip *chip,
					unsigned offset, int value)
{
	return alchemy_gpio1_direction_output(offset + ALCHEMY_GPIO1_BASE,
					     value);
}

static int gpio1_to_irq(struct gpio_chip *chip, unsigned offset)
{
	return alchemy_gpio1_to_irq(offset + ALCHEMY_GPIO1_BASE);
}

struct gpio_chip alchemy_gpio_chip[] = {
	[0] = {
		.label			= "alchemy-gpio1",
		.direction_input	= gpio1_direction_input,
		.direction_output	= gpio1_direction_output,
		.get			= gpio1_get,
		.set			= gpio1_set,
		.to_irq			= gpio1_to_irq,
		.base			= ALCHEMY_GPIO1_BASE,
		.ngpio			= ALCHEMY_GPIO1_NUM,
	},
	[1] = {
		.label                  = "alchemy-gpio2",
		.direction_input        = gpio2_direction_input,
		.direction_output       = gpio2_direction_output,
		.get                    = gpio2_get,
		.set                    = gpio2_set,
		.to_irq			= gpio2_to_irq,
		.base                   = ALCHEMY_GPIO2_BASE,
		.ngpio                  = ALCHEMY_GPIO2_NUM,
	},
};

static int __init alchemy_gpiolib_init(void)
{
	gpiochip_add(&alchemy_gpio_chip[0]);
	if (alchemy_get_cputype() != ALCHEMY_CPU_AU1000)
		gpiochip_add(&alchemy_gpio_chip[1]);

	return 0;
}
arch_initcall(alchemy_gpiolib_init);
