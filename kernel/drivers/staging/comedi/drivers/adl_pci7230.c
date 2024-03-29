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
    comedi/drivers/adl_pci7230.c

    Hardware comedi driver fot PCI7230 Adlink card
    Copyright (C) 2010 David Fernandez <dfcastelao@gmail.com>

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
Driver: adl_pci7230
Description: Driver for the Adlink PCI-7230 32 ch. isolated digital io board
Devices: [ADLink] PCI-7230 (adl_pci7230)
Author: David Fernandez <dfcastelao@gmail.com>
Status: experimental
Updated: Mon, 14 Apr 2008 15:08:14 +0100

Configuration Options:
  [0] - PCI bus of device (optional)
  [1] - PCI slot of device (optional)
  If bus/slot is not specified, the first supported
  PCI device found will be used.
*/

#include "../comedidev.h"
#include <linux/kernel.h>
#include "comedi_pci.h"

#define PCI7230_DI      0x00
#define PCI7230_DO	    0x00

#define PCI_DEVICE_ID_PCI7230 0x7230

static DEFINE_PCI_DEVICE_TABLE(adl_pci7230_pci_table) = {
	{
		PCI_VENDOR_ID_ADLINK,
		PCI_DEVICE_ID_PCI7230,
		PCI_ANY_ID,
		PCI_ANY_ID,
		0,
		0,
		0
	},
	{0}
};

MODULE_DEVICE_TABLE(pci, adl_pci7230_pci_table);

struct adl_pci7230_private {
	int data;
	struct pci_dev *pci_dev;
};

#define devpriv ((struct adl_pci7230_private *)dev->private)

static int adl_pci7230_attach(struct comedi_device *dev,
	struct comedi_devconfig *it);
static int adl_pci7230_detach(struct comedi_device *dev);
static struct comedi_driver driver_adl_pci7230 = {
	.driver_name = "adl_pci7230",
	.module = THIS_MODULE,
	.attach = adl_pci7230_attach,
	.detach = adl_pci7230_detach,
};

/* Digital IO */

static int adl_pci7230_di_insn_bits(struct comedi_device *dev,
	struct comedi_subdevice *s,
	struct comedi_insn *insn,
	unsigned int *data);

static int adl_pci7230_do_insn_bits(struct comedi_device *dev,
	struct comedi_subdevice *s,
	struct comedi_insn *insn,
	unsigned int *data);

static int adl_pci7230_attach(struct comedi_device *dev,
	struct comedi_devconfig *it)
{
	struct pci_dev *pcidev;
	struct comedi_subdevice *s;
	int bus, slot;

	printk(KERN_INFO "comedi%d: adl_pci7230\n", dev->minor);

	dev->board_name = "pci7230";
	bus = it->options[0];
	slot = it->options[1];

	if (alloc_private(dev, sizeof(struct adl_pci7230_private)) < 0)
		return -ENOMEM;

	if (alloc_subdevices(dev, 2) < 0)
		return -ENOMEM;

	for (pcidev = pci_get_device(PCI_ANY_ID, PCI_ANY_ID, NULL);
		pcidev != NULL;
		pcidev = pci_get_device(PCI_ANY_ID, PCI_ANY_ID, pcidev)) {

		if (pcidev->vendor == PCI_VENDOR_ID_ADLINK &&
			pcidev->device == PCI_DEVICE_ID_PCI7230) {
			if (bus || slot) {
				/* requested particular bus/slot */
				if (pcidev->bus->number != bus ||
					PCI_SLOT(pcidev->devfn) != slot) {
					continue;
				}
			}
			devpriv->pci_dev = pcidev;
			break;
		}
	}
	if (pcidev == NULL) {
		printk(KERN_ERR "comedi%d: no supported board found! (req. bus/slot : %d/%d)\n",
			dev->minor, bus, slot);
		return -EIO;
	}
	if (comedi_pci_enable(pcidev, "adl_pci7230") < 0) {
		printk(KERN_ERR "comedi%d: Failed to enable PCI device and request regions\n",
			dev->minor);
		return -EIO;
	}
	dev->iobase = pci_resource_start(pcidev, 2);
	printk(KERN_DEBUG "comedi: base addr %4lx\n", dev->iobase);

	s = dev->subdevices + 0;
	/* Isolated do */
	s->type = COMEDI_SUBD_DO;
	s->subdev_flags = SDF_WRITABLE | SDF_GROUND | SDF_COMMON;
	s->n_chan = 16;
	s->maxdata = 1;
	s->range_table = &range_digital;
	s->insn_bits = adl_pci7230_do_insn_bits;

	s = dev->subdevices + 1;
	/* Isolated di */
	s->type = COMEDI_SUBD_DI;
	s->subdev_flags = SDF_READABLE | SDF_GROUND | SDF_COMMON;
	s->n_chan = 16;
	s->maxdata = 1;
	s->range_table = &range_digital;
	s->insn_bits = adl_pci7230_di_insn_bits;

	printk(KERN_DEBUG "comedi: attached\n");

	return 1;
}

static int adl_pci7230_detach(struct comedi_device *dev)
{
	printk(KERN_DEBUG "comedi%d: pci7230: remove\n", dev->minor);

	if (devpriv && devpriv->pci_dev) {
		if (dev->iobase)
			comedi_pci_disable(devpriv->pci_dev);
		pci_dev_put(devpriv->pci_dev);
	}

	return 0;
}

static int adl_pci7230_do_insn_bits(struct comedi_device *dev,
	struct comedi_subdevice *s,
	struct comedi_insn *insn,
	unsigned int *data)
{
	if (insn->n != 2)
		return -EINVAL;

	if (data[0]) {
		s->state &= ~data[0];
		s->state |= (data[0] & data[1]);

		outl((s->state  << 16) & 0xffffffff, dev->iobase + PCI7230_DO);
	}

	return 2;
}

static int adl_pci7230_di_insn_bits(struct comedi_device *dev,
	struct comedi_subdevice *s,
	struct comedi_insn *insn,
	unsigned int *data)
{
	if (insn->n != 2)
		return -EINVAL;

	data[1] = inl(dev->iobase + PCI7230_DI) & 0xffffffff;

	return 2;
}

COMEDI_PCI_INITCLEANUP(driver_adl_pci7230, adl_pci7230_pci_table);
