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
 *  Driver for the NXP SAA7164 PCIe bridge
 *
 *  Copyright (c) 2009 Steven Toth <stoth@kernellabs.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <asm/io.h>

#include "saa7164.h"

static int i2c_xfer(struct i2c_adapter *i2c_adap, struct i2c_msg *msgs, int num)
{
	struct saa7164_i2c *bus = i2c_adap->algo_data;
	struct saa7164_dev *dev = bus->dev;
	int i, retval = 0;

	dprintk(DBGLVL_I2C, "%s(num = %d)\n", __func__, num);

	for (i = 0 ; i < num; i++) {
		dprintk(DBGLVL_I2C, "%s(num = %d) addr = 0x%02x  len = 0x%x\n",
			__func__, num, msgs[i].addr, msgs[i].len);
		if (msgs[i].flags & I2C_M_RD) {
			/* Unsupported - Yet*/
			printk(KERN_ERR "%s() Unsupported - Yet\n", __func__);
			continue;
		} else if (i + 1 < num && (msgs[i + 1].flags & I2C_M_RD) &&
			   msgs[i].addr == msgs[i + 1].addr) {
			/* write then read from same address */

			retval = saa7164_api_i2c_read(bus, msgs[i].addr,
				msgs[i].len, msgs[i].buf,
				msgs[i+1].len, msgs[i+1].buf
				);

			i++;

			if (retval < 0)
				goto err;
		} else {
			/* write */
			retval = saa7164_api_i2c_write(bus, msgs[i].addr,
				msgs[i].len, msgs[i].buf);
		}
		if (retval < 0)
			goto err;
	}
	return num;

 err:
	return retval;
}

void saa7164_call_i2c_clients(struct saa7164_i2c *bus, unsigned int cmd,
	void *arg)
{
	if (bus->i2c_rc != 0)
		return;

	i2c_clients_command(&bus->i2c_adap, cmd, arg);
}

static u32 saa7164_functionality(struct i2c_adapter *adap)
{
	return I2C_FUNC_I2C;
}

static struct i2c_algorithm saa7164_i2c_algo_template = {
	.master_xfer	= i2c_xfer,
	.functionality	= saa7164_functionality,
};

/* ----------------------------------------------------------------------- */

static struct i2c_adapter saa7164_i2c_adap_template = {
	.name              = "saa7164",
	.owner             = THIS_MODULE,
	.algo              = &saa7164_i2c_algo_template,
};

static struct i2c_client saa7164_i2c_client_template = {
	.name	= "saa7164 internal",
};

int saa7164_i2c_register(struct saa7164_i2c *bus)
{
	struct saa7164_dev *dev = bus->dev;

	dprintk(DBGLVL_I2C, "%s(bus = %d)\n", __func__, bus->nr);

	memcpy(&bus->i2c_adap, &saa7164_i2c_adap_template,
	       sizeof(bus->i2c_adap));

	memcpy(&bus->i2c_algo, &saa7164_i2c_algo_template,
	       sizeof(bus->i2c_algo));

	memcpy(&bus->i2c_client, &saa7164_i2c_client_template,
	       sizeof(bus->i2c_client));

	bus->i2c_adap.dev.parent = &dev->pci->dev;

	strlcpy(bus->i2c_adap.name, bus->dev->name,
		sizeof(bus->i2c_adap.name));

	bus->i2c_algo.data = bus;
	bus->i2c_adap.algo_data = bus;
	i2c_set_adapdata(&bus->i2c_adap, bus);
	i2c_add_adapter(&bus->i2c_adap);

	bus->i2c_client.adapter = &bus->i2c_adap;

	if (0 != bus->i2c_rc)
		printk(KERN_ERR "%s: i2c bus %d register FAILED\n",
			dev->name, bus->nr);

	return bus->i2c_rc;
}

int saa7164_i2c_unregister(struct saa7164_i2c *bus)
{
	i2c_del_adapter(&bus->i2c_adap);
	return 0;
}
