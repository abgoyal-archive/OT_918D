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

    comedi/drivers/aio_iiro_16.c

    Driver for Acces I/O Products PC-104 AIO-IIRO-16 Digital I/O board
    Copyright (C) 2006 C&C Technologies, Inc.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

/*

Driver: aio_iiro_16
Description: Acces I/O Products PC-104 IIRO16 Relay And Isolated Input Board
Author: Zachary Ware <zach.ware@cctechnol.com>
Devices:
 [Acces I/O] PC-104 AIO12-8
Status: experimental

Configuration Options:
  [0] - I/O port base address

*/

#include "../comedidev.h"
#include <linux/ioport.h>

#define AIO_IIRO_16_SIZE	0x08
#define AIO_IIRO_16_RELAY_0_7	0x00
#define AIO_IIRO_16_INPUT_0_7	0x01
#define AIO_IIRO_16_IRQ		0x02
#define AIO_IIRO_16_RELAY_8_15	0x04
#define AIO_IIRO_16_INPUT_8_15	0x05

struct aio_iiro_16_board {
	const char *name;
	int do_;
	int di;
};

static const struct aio_iiro_16_board aio_iiro_16_boards[] = {
	{
	 .name = "aio_iiro_16",
	 .di = 16,
	 .do_ = 16},
};

#define	thisboard	((const struct aio_iiro_16_board *) dev->board_ptr)

struct aio_iiro_16_private {
	int data;
	struct pci_dev *pci_dev;
	unsigned int ao_readback[2];
};

#define	devpriv	((struct aio_iiro_16_private *) dev->private)

static int aio_iiro_16_attach(struct comedi_device *dev,
			      struct comedi_devconfig *it);

static int aio_iiro_16_detach(struct comedi_device *dev);

static struct comedi_driver driver_aio_iiro_16 = {
	.driver_name = "aio_iiro_16",
	.module = THIS_MODULE,
	.attach = aio_iiro_16_attach,
	.detach = aio_iiro_16_detach,
	.board_name = &aio_iiro_16_boards[0].name,
	.offset = sizeof(struct aio_iiro_16_board),
	.num_names = ARRAY_SIZE(aio_iiro_16_boards),
};

static int aio_iiro_16_dio_insn_bits_read(struct comedi_device *dev,
					  struct comedi_subdevice *s,
					  struct comedi_insn *insn,
					  unsigned int *data);

static int aio_iiro_16_dio_insn_bits_write(struct comedi_device *dev,
					   struct comedi_subdevice *s,
					   struct comedi_insn *insn,
					   unsigned int *data);

static int aio_iiro_16_attach(struct comedi_device *dev,
			      struct comedi_devconfig *it)
{
	int iobase;
	struct comedi_subdevice *s;

	printk(KERN_INFO "comedi%d: aio_iiro_16: ", dev->minor);

	dev->board_name = thisboard->name;

	iobase = it->options[0];

	if (!request_region(iobase, AIO_IIRO_16_SIZE, dev->board_name)) {
		printk("I/O port conflict");
		return -EIO;
	}

	dev->iobase = iobase;

	if (alloc_private(dev, sizeof(struct aio_iiro_16_private)) < 0)
		return -ENOMEM;

	if (alloc_subdevices(dev, 2) < 0)
		return -ENOMEM;

	s = dev->subdevices + 0;
	s->type = COMEDI_SUBD_DIO;
	s->subdev_flags = SDF_WRITABLE;
	s->n_chan = 16;
	s->maxdata = 1;
	s->range_table = &range_digital;
	s->insn_bits = aio_iiro_16_dio_insn_bits_write;

	s = dev->subdevices + 1;
	s->type = COMEDI_SUBD_DIO;
	s->subdev_flags = SDF_READABLE;
	s->n_chan = 16;
	s->maxdata = 1;
	s->range_table = &range_digital;
	s->insn_bits = aio_iiro_16_dio_insn_bits_read;

	printk("attached\n");

	return 1;
}

static int aio_iiro_16_detach(struct comedi_device *dev)
{
	printk(KERN_INFO "comedi%d: aio_iiro_16: remove\n", dev->minor);

	if (dev->iobase)
		release_region(dev->iobase, AIO_IIRO_16_SIZE);

	return 0;
}

static int aio_iiro_16_dio_insn_bits_write(struct comedi_device *dev,
					   struct comedi_subdevice *s,
					   struct comedi_insn *insn,
					   unsigned int *data)
{
	if (insn->n != 2)
		return -EINVAL;

	if (data[0]) {
		s->state &= ~data[0];
		s->state |= data[0] & data[1];
		outb(s->state & 0xff, dev->iobase + AIO_IIRO_16_RELAY_0_7);
		outb((s->state >> 8) & 0xff,
		     dev->iobase + AIO_IIRO_16_RELAY_8_15);
	}

	data[1] = s->state;

	return 2;
}

static int aio_iiro_16_dio_insn_bits_read(struct comedi_device *dev,
					  struct comedi_subdevice *s,
					  struct comedi_insn *insn,
					  unsigned int *data)
{
	if (insn->n != 2)
		return -EINVAL;

	data[1] = 0;
	data[1] |= inb(dev->iobase + AIO_IIRO_16_INPUT_0_7);
	data[1] |= inb(dev->iobase + AIO_IIRO_16_INPUT_8_15) << 8;

	return 2;
}

COMEDI_INITCLEANUP(driver_aio_iiro_16);
