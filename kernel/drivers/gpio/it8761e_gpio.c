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
 *  it8761_gpio.c - GPIO interface for IT8761E Super I/O chip
 *
 *  Author: Denis Turischev <denis@compulab.co.il>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License 2 as published
 *  by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/io.h>
#include <linux/errno.h>
#include <linux/ioport.h>

#include <linux/gpio.h>

#define SIO_CHIP_ID		0x8761
#define CHIP_ID_HIGH_BYTE	0x20
#define CHIP_ID_LOW_BYTE	0x21

static u8 ports[2] = { 0x2e, 0x4e };
static u8 port;

static DEFINE_SPINLOCK(sio_lock);

#define GPIO_NAME		"it8761-gpio"
#define GPIO_BA_HIGH_BYTE	0x60
#define GPIO_BA_LOW_BYTE	0x61
#define GPIO_IOSIZE		4
#define GPIO1X_IO		0xf0
#define GPIO2X_IO		0xf1

static u16 gpio_ba;

static u8 read_reg(u8 addr, u8 port)
{
	outb(addr, port);
	return inb(port + 1);
}

static void write_reg(u8 data, u8 addr, u8 port)
{
	outb(addr, port);
	outb(data, port + 1);
}

static void enter_conf_mode(u8 port)
{
	outb(0x87, port);
	outb(0x61, port);
	outb(0x55, port);
	outb((port == 0x2e) ? 0x55 : 0xaa, port);
}

static void exit_conf_mode(u8 port)
{
	outb(0x2, port);
	outb(0x2, port + 1);
}

static void enter_gpio_mode(u8 port)
{
	write_reg(0x2, 0x7, port);
}

static int it8761e_gpio_get(struct gpio_chip *gc, unsigned gpio_num)
{
	u16 reg;
	u8 bit;

	bit = gpio_num % 8;
	reg = (gpio_num >= 8) ? gpio_ba + 1 : gpio_ba;

	return !!(inb(reg) & (1 << bit));
}

static int it8761e_gpio_direction_in(struct gpio_chip *gc, unsigned gpio_num)
{
	u8 curr_dirs;
	u8 io_reg, bit;

	bit = gpio_num % 8;
	io_reg = (gpio_num >= 8) ? GPIO2X_IO : GPIO1X_IO;

	spin_lock(&sio_lock);

	enter_conf_mode(port);
	enter_gpio_mode(port);

	curr_dirs = read_reg(io_reg, port);

	if (curr_dirs & (1 << bit))
		write_reg(curr_dirs & ~(1 << bit), io_reg, port);

	exit_conf_mode(port);

	spin_unlock(&sio_lock);
	return 0;
}

static void it8761e_gpio_set(struct gpio_chip *gc,
				unsigned gpio_num, int val)
{
	u8 curr_vals, bit;
	u16 reg;

	bit = gpio_num % 8;
	reg = (gpio_num >= 8) ? gpio_ba + 1 : gpio_ba;

	spin_lock(&sio_lock);

	curr_vals = inb(reg);
	if (val)
		outb(curr_vals | (1 << bit) , reg);
	else
		outb(curr_vals & ~(1 << bit), reg);

	spin_unlock(&sio_lock);
}

static int it8761e_gpio_direction_out(struct gpio_chip *gc,
					unsigned gpio_num, int val)
{
	u8 curr_dirs, io_reg, bit;

	bit = gpio_num % 8;
	io_reg = (gpio_num >= 8) ? GPIO2X_IO : GPIO1X_IO;

	it8761e_gpio_set(gc, gpio_num, val);

	spin_lock(&sio_lock);

	enter_conf_mode(port);
	enter_gpio_mode(port);

	curr_dirs = read_reg(io_reg, port);

	if (!(curr_dirs & (1 << bit)))
		write_reg(curr_dirs | (1 << bit), io_reg, port);

	exit_conf_mode(port);

	spin_unlock(&sio_lock);
	return 0;
}

static struct gpio_chip it8761e_gpio_chip = {
	.label			= GPIO_NAME,
	.owner			= THIS_MODULE,
	.get			= it8761e_gpio_get,
	.direction_input	= it8761e_gpio_direction_in,
	.set			= it8761e_gpio_set,
	.direction_output	= it8761e_gpio_direction_out,
};

static int __init it8761e_gpio_init(void)
{
	int i, id, err;

	/* chip and port detection */
	for (i = 0; i < ARRAY_SIZE(ports); i++) {
		spin_lock(&sio_lock);
		enter_conf_mode(ports[i]);

		id = (read_reg(CHIP_ID_HIGH_BYTE, ports[i]) << 8) +
				read_reg(CHIP_ID_LOW_BYTE, ports[i]);

		exit_conf_mode(ports[i]);
		spin_unlock(&sio_lock);

		if (id == SIO_CHIP_ID) {
			port = ports[i];
			break;
		}
	}

	if (!port)
		return -ENODEV;

	/* fetch GPIO base address */
	enter_conf_mode(port);
	enter_gpio_mode(port);
	gpio_ba = (read_reg(GPIO_BA_HIGH_BYTE, port) << 8) +
				read_reg(GPIO_BA_LOW_BYTE, port);
	exit_conf_mode(port);

	if (!request_region(gpio_ba, GPIO_IOSIZE, GPIO_NAME))
		return -EBUSY;

	it8761e_gpio_chip.base = -1;
	it8761e_gpio_chip.ngpio = 16;

	err = gpiochip_add(&it8761e_gpio_chip);
	if (err < 0)
		goto gpiochip_add_err;

	return 0;

gpiochip_add_err:
	release_region(gpio_ba, GPIO_IOSIZE);
	gpio_ba = 0;
	return err;
}

static void __exit it8761e_gpio_exit(void)
{
	if (gpio_ba) {
		int ret = gpiochip_remove(&it8761e_gpio_chip);

		WARN(ret, "%s(): gpiochip_remove() failed, ret=%d\n",
				__func__, ret);

		release_region(gpio_ba, GPIO_IOSIZE);
		gpio_ba = 0;
	}
}
module_init(it8761e_gpio_init);
module_exit(it8761e_gpio_exit);

MODULE_AUTHOR("Denis Turischev <denis@compulab.co.il>");
MODULE_DESCRIPTION("GPIO interface for IT8761E Super I/O chip");
MODULE_LICENSE("GPL");
