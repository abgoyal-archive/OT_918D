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
 * Standard Hot Plug Controller Driver
 *
 * Copyright (C) 1995,2001 Compaq Computer Corporation
 * Copyright (C) 2001 Greg Kroah-Hartman (greg@kroah.com)
 * Copyright (C) 2001 IBM Corp.
 * Copyright (C) 2003-2004 Intel Corporation
 *
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE, GOOD TITLE or
 * NON INFRINGEMENT.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * Send feedback to <greg@kroah.com>, <kristen.c.accardi@intel.com>
 *
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/pci.h>
#include "../pci.h"
#include "shpchp.h"

int __ref shpchp_configure_device(struct slot *p_slot)
{
	struct pci_dev *dev;
	struct pci_bus *parent = p_slot->ctrl->pci_dev->subordinate;
	int num, fn;
	struct controller *ctrl = p_slot->ctrl;

	dev = pci_get_slot(parent, PCI_DEVFN(p_slot->device, 0));
	if (dev) {
		ctrl_err(ctrl, "Device %s already exists "
			 "at %04x:%02x:%02x, cannot hot-add\n", pci_name(dev),
			 pci_domain_nr(parent), p_slot->bus, p_slot->device);
		pci_dev_put(dev);
		return -EINVAL;
	}

	num = pci_scan_slot(parent, PCI_DEVFN(p_slot->device, 0));
	if (num == 0) {
		ctrl_err(ctrl, "No new device found\n");
		return -ENODEV;
	}

	for (fn = 0; fn < 8; fn++) {
		dev = pci_get_slot(parent, PCI_DEVFN(p_slot->device, fn));
		if (!dev)
			continue;
		if ((dev->class >> 16) == PCI_BASE_CLASS_DISPLAY) {
			ctrl_err(ctrl, "Cannot hot-add display device %s\n",
				 pci_name(dev));
			pci_dev_put(dev);
			continue;
		}
		if ((dev->hdr_type == PCI_HEADER_TYPE_BRIDGE) ||
				(dev->hdr_type == PCI_HEADER_TYPE_CARDBUS)) {
			/* Find an unused bus number for the new bridge */
			struct pci_bus *child;
			unsigned char busnr, start = parent->secondary;
			unsigned char end = parent->subordinate;
			for (busnr = start; busnr <= end; busnr++) {
				if (!pci_find_bus(pci_domain_nr(parent),
							busnr))
					break;
			}
			if (busnr > end) {
				ctrl_err(ctrl,
					 "No free bus for hot-added bridge\n");
				pci_dev_put(dev);
				continue;
			}
			child = pci_add_new_bus(parent, dev, busnr);
			if (!child) {
				ctrl_err(ctrl, "Cannot add new bus for %s\n",
					 pci_name(dev));
				pci_dev_put(dev);
				continue;
			}
			child->subordinate = pci_do_scan_bus(child);
			pci_bus_size_bridges(child);
		}
		pci_configure_slot(dev);
		pci_dev_put(dev);
	}

	pci_bus_assign_resources(parent);
	pci_bus_add_devices(parent);
	pci_enable_bridges(parent);
	return 0;
}

int shpchp_unconfigure_device(struct slot *p_slot)
{
	int rc = 0;
	int j;
	u8 bctl = 0;
	struct pci_bus *parent = p_slot->ctrl->pci_dev->subordinate;
	struct controller *ctrl = p_slot->ctrl;

	ctrl_dbg(ctrl, "%s: domain:bus:dev = %04x:%02x:%02x\n",
		 __func__, pci_domain_nr(parent), p_slot->bus, p_slot->device);

	for (j=0; j<8 ; j++) {
		struct pci_dev* temp = pci_get_slot(parent,
				(p_slot->device << 3) | j);
		if (!temp)
			continue;
		if ((temp->class >> 16) == PCI_BASE_CLASS_DISPLAY) {
			ctrl_err(ctrl, "Cannot remove display device %s\n",
				 pci_name(temp));
			pci_dev_put(temp);
			continue;
		}
		if (temp->hdr_type == PCI_HEADER_TYPE_BRIDGE) {
			pci_read_config_byte(temp, PCI_BRIDGE_CONTROL, &bctl);
			if (bctl & PCI_BRIDGE_CTL_VGA) {
				ctrl_err(ctrl,
					 "Cannot remove display device %s\n",
					 pci_name(temp));
				pci_dev_put(temp);
				continue;
			}
		}
		pci_remove_bus_device(temp);
		pci_dev_put(temp);
	}
	return rc;
}

