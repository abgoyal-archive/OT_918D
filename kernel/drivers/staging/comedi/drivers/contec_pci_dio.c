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
    comedi/drivers/contec_pci_dio.c

    COMEDI - Linux Control and Measurement Device Interface
    Copyright (C) 2000 David A. Schleef <ds@schleef.org>

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
Driver: contec_pci_dio
Description: Contec PIO1616L digital I/O board
Devices: [Contec] PIO1616L (contec_pci_dio)
Author: Stefano Rivoir <s.rivoir@gts.it>
Updated: Wed, 27 Jun 2007 13:00:06 +0100
Status: works

Configuration Options:
  [0] - PCI bus of device (optional)
  [1] - PCI slot of device (optional)
  If bus/slot is not specified, the first supported
  PCI device found will be used.
*/

#include "../comedidev.h"

#include "comedi_pci.h"

enum contec_model {
	PIO1616L = 0,
};

struct contec_board {
	const char *name;
	int model;
	int in_ports;
	int out_ports;
	int in_offs;
	int out_offs;
	int out_boffs;
};
static const struct contec_board contec_boards[] = {
	{"PIO1616L", PIO1616L, 16, 16, 0, 2, 10},
};

#define PCI_DEVICE_ID_PIO1616L 0x8172
static DEFINE_PCI_DEVICE_TABLE(contec_pci_table) = {
	{
	PCI_VENDOR_ID_CONTEC, PCI_DEVICE_ID_PIO1616L, PCI_ANY_ID,
		    PCI_ANY_ID, 0, 0, PIO1616L}, {
	0}
};

MODULE_DEVICE_TABLE(pci, contec_pci_table);

#define thisboard ((const struct contec_board *)dev->board_ptr)

struct contec_private {
	int data;

	struct pci_dev *pci_dev;

};

#define devpriv ((struct contec_private *)dev->private)

static int contec_attach(struct comedi_device *dev,
			 struct comedi_devconfig *it);
static int contec_detach(struct comedi_device *dev);
static struct comedi_driver driver_contec = {
	.driver_name = "contec_pci_dio",
	.module = THIS_MODULE,
	.attach = contec_attach,
	.detach = contec_detach,
};

/* Classic digital IO */
static int contec_di_insn_bits(struct comedi_device *dev,
			       struct comedi_subdevice *s,
			       struct comedi_insn *insn, unsigned int *data);
static int contec_do_insn_bits(struct comedi_device *dev,
			       struct comedi_subdevice *s,
			       struct comedi_insn *insn, unsigned int *data);

#if 0
static int contec_cmdtest(struct comedi_device *dev, struct comedi_subdevice *s,
			  struct comedi_cmd *cmd);

static int contec_ns_to_timer(unsigned int *ns, int round);
#endif

static int contec_attach(struct comedi_device *dev, struct comedi_devconfig *it)
{
	struct pci_dev *pcidev;
	struct comedi_subdevice *s;

	printk("comedi%d: contec: ", dev->minor);

	dev->board_name = thisboard->name;

	if (alloc_private(dev, sizeof(struct contec_private)) < 0)
		return -ENOMEM;

	if (alloc_subdevices(dev, 2) < 0)
		return -ENOMEM;

	for (pcidev = pci_get_device(PCI_ANY_ID, PCI_ANY_ID, NULL);
	     pcidev != NULL;
	     pcidev = pci_get_device(PCI_ANY_ID, PCI_ANY_ID, pcidev)) {

		if (pcidev->vendor == PCI_VENDOR_ID_CONTEC &&
		    pcidev->device == PCI_DEVICE_ID_PIO1616L) {
			if (it->options[0] || it->options[1]) {
				/* Check bus and slot. */
				if (it->options[0] != pcidev->bus->number ||
				    it->options[1] != PCI_SLOT(pcidev->devfn)) {
					continue;
				}
			}
			devpriv->pci_dev = pcidev;
			if (comedi_pci_enable(pcidev, "contec_pci_dio")) {
				printk
				    ("error enabling PCI device and request regions!\n");
				return -EIO;
			}
			dev->iobase = pci_resource_start(pcidev, 0);
			printk(" base addr %lx ", dev->iobase);

			dev->board_ptr = contec_boards + 0;

			s = dev->subdevices + 0;

			s->type = COMEDI_SUBD_DI;
			s->subdev_flags = SDF_READABLE;
			s->n_chan = 16;
			s->maxdata = 1;
			s->range_table = &range_digital;
			s->insn_bits = contec_di_insn_bits;

			s = dev->subdevices + 1;
			s->type = COMEDI_SUBD_DO;
			s->subdev_flags = SDF_WRITABLE;
			s->n_chan = 16;
			s->maxdata = 1;
			s->range_table = &range_digital;
			s->insn_bits = contec_do_insn_bits;

			printk("attached\n");

			return 1;
		}
	}

	printk("card not present!\n");

	return -EIO;
}

static int contec_detach(struct comedi_device *dev)
{
	printk("comedi%d: contec: remove\n", dev->minor);

	if (devpriv && devpriv->pci_dev) {
		if (dev->iobase)
			comedi_pci_disable(devpriv->pci_dev);
		pci_dev_put(devpriv->pci_dev);
	}

	return 0;
}

#if 0
static int contec_cmdtest(struct comedi_device *dev, struct comedi_subdevice *s,
			  struct comedi_cmd *cmd)
{
	printk("contec_cmdtest called\n");
	return 0;
}

static int contec_ns_to_timer(unsigned int *ns, int round)
{
	return *ns;
}
#endif

static int contec_do_insn_bits(struct comedi_device *dev,
			       struct comedi_subdevice *s,
			       struct comedi_insn *insn, unsigned int *data)
{

	printk("contec_do_insn_bits called\n");
	printk(" data: %d %d\n", data[0], data[1]);

	if (insn->n != 2)
		return -EINVAL;

	if (data[0]) {
		s->state &= ~data[0];
		s->state |= data[0] & data[1];
		printk("  out: %d on %lx\n", s->state,
		       dev->iobase + thisboard->out_offs);
		outw(s->state, dev->iobase + thisboard->out_offs);
	}
	return 2;
}

static int contec_di_insn_bits(struct comedi_device *dev,
			       struct comedi_subdevice *s,
			       struct comedi_insn *insn, unsigned int *data)
{

	printk("contec_di_insn_bits called\n");
	printk(" data: %d %d\n", data[0], data[1]);

	if (insn->n != 2)
		return -EINVAL;

	data[1] = inw(dev->iobase + thisboard->in_offs);

	return 2;
}

COMEDI_PCI_INITCLEANUP(driver_contec, contec_pci_table);
