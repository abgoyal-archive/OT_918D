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
 * ngene-i2c.c: nGene PCIe bridge driver i2c functions
 *
 * Copyright (C) 2005-2007 Micronas
 *
 * Copyright (C) 2008-2009 Ralph Metzler <rjkm@metzlerbros.de>
 *                         Modifications for new nGene firmware,
 *                         support for EEPROM-copying,
 *                         support for new dual DVB-S2 card prototype
 *
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 only, as published by the Free Software Foundation.
 *
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA
 * Or, point your browser to http://www.gnu.org/copyleft/gpl.html
 */

/* FIXME - some of these can probably be removed */
#include <linux/module.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/poll.h>
#include <linux/io.h>
#include <asm/div64.h>
#include <linux/pci.h>
#include <linux/pci_ids.h>
#include <linux/smp_lock.h>
#include <linux/timer.h>
#include <linux/byteorder/generic.h>
#include <linux/firmware.h>
#include <linux/vmalloc.h>

#include "ngene.h"

/* Firmware command for i2c operations */
static int ngene_command_i2c_read(struct ngene *dev, u8 adr,
			   u8 *out, u8 outlen, u8 *in, u8 inlen, int flag)
{
	struct ngene_command com;

	com.cmd.hdr.Opcode = CMD_I2C_READ;
	com.cmd.hdr.Length = outlen + 3;
	com.cmd.I2CRead.Device = adr << 1;
	memcpy(com.cmd.I2CRead.Data, out, outlen);
	com.cmd.I2CRead.Data[outlen] = inlen;
	com.cmd.I2CRead.Data[outlen + 1] = 0;
	com.in_len = outlen + 3;
	com.out_len = inlen + 1;

	if (ngene_command(dev, &com) < 0)
		return -EIO;

	if ((com.cmd.raw8[0] >> 1) != adr)
		return -EIO;

	if (flag)
		memcpy(in, com.cmd.raw8, inlen + 1);
	else
		memcpy(in, com.cmd.raw8 + 1, inlen);
	return 0;
}

static int ngene_command_i2c_write(struct ngene *dev, u8 adr,
				   u8 *out, u8 outlen)
{
	struct ngene_command com;


	com.cmd.hdr.Opcode = CMD_I2C_WRITE;
	com.cmd.hdr.Length = outlen + 1;
	com.cmd.I2CRead.Device = adr << 1;
	memcpy(com.cmd.I2CRead.Data, out, outlen);
	com.in_len = outlen + 1;
	com.out_len = 1;

	if (ngene_command(dev, &com) < 0)
		return -EIO;

	if (com.cmd.raw8[0] == 1)
		return -EIO;

	return 0;
}

static void ngene_i2c_set_bus(struct ngene *dev, int bus)
{
	if (!(dev->card_info->i2c_access & 2))
		return;
	if (dev->i2c_current_bus == bus)
		return;

	switch (bus) {
	case 0:
		ngene_command_gpio_set(dev, 3, 0);
		ngene_command_gpio_set(dev, 2, 1);
		break;

	case 1:
		ngene_command_gpio_set(dev, 2, 0);
		ngene_command_gpio_set(dev, 3, 1);
		break;
	}
	dev->i2c_current_bus = bus;
}

static int ngene_i2c_master_xfer(struct i2c_adapter *adapter,
				 struct i2c_msg msg[], int num)
{
	struct ngene_channel *chan =
		(struct ngene_channel *)i2c_get_adapdata(adapter);
	struct ngene *dev = chan->dev;

	down(&dev->i2c_switch_mutex);
	ngene_i2c_set_bus(dev, chan->number);

	if (num == 2 && msg[1].flags & I2C_M_RD && !(msg[0].flags & I2C_M_RD))
		if (!ngene_command_i2c_read(dev, msg[0].addr,
					    msg[0].buf, msg[0].len,
					    msg[1].buf, msg[1].len, 0))
			goto done;

	if (num == 1 && !(msg[0].flags & I2C_M_RD))
		if (!ngene_command_i2c_write(dev, msg[0].addr,
					     msg[0].buf, msg[0].len))
			goto done;
	if (num == 1 && (msg[0].flags & I2C_M_RD))
		if (!ngene_command_i2c_read(dev, msg[0].addr, NULL, 0,
					    msg[0].buf, msg[0].len, 0))
			goto done;

	up(&dev->i2c_switch_mutex);
	return -EIO;

done:
	up(&dev->i2c_switch_mutex);
	return num;
}


static u32 ngene_i2c_functionality(struct i2c_adapter *adap)
{
	return I2C_FUNC_SMBUS_EMUL;
}

static struct i2c_algorithm ngene_i2c_algo = {
	.master_xfer = ngene_i2c_master_xfer,
	.functionality = ngene_i2c_functionality,
};

int ngene_i2c_init(struct ngene *dev, int dev_nr)
{
	struct i2c_adapter *adap = &(dev->channel[dev_nr].i2c_adapter);

	i2c_set_adapdata(adap, &(dev->channel[dev_nr]));
	adap->class = I2C_CLASS_TV_DIGITAL | I2C_CLASS_TV_ANALOG;

	strcpy(adap->name, "nGene");

	adap->algo = &ngene_i2c_algo;
	adap->algo_data = (void *)&(dev->channel[dev_nr]);
	adap->dev.parent = &dev->pci_dev->dev;

	return i2c_add_adapter(adap);
}

