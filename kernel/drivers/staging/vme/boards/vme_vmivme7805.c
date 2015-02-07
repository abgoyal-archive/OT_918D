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
 * Support for the VMIVME-7805 board access to the Universe II bridge.
 *
 * Author: Arthur Benilov <arthur.benilov@iba-group.com>
 * Copyright 2010 Ion Beam Application, Inc.
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */

#include <linux/module.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/pci.h>
#include <linux/poll.h>
#include <linux/io.h>

#include "vme_vmivme7805.h"

static int __init vmic_init(void);
static int vmic_probe(struct pci_dev *, const struct pci_device_id *);
static void vmic_remove(struct pci_dev *);
static void __exit vmic_exit(void);

/** Base address to access FPGA register */
static void *vmic_base;

static char driver_name[] = "vmivme_7805";

static struct pci_device_id vmic_ids[] = {
	{ PCI_DEVICE(PCI_VENDOR_ID_VMIC, PCI_DEVICE_ID_VTIMR) },
	{ },
};

static struct pci_driver vmic_driver = {
	.name = driver_name,
	.id_table = vmic_ids,
	.probe = vmic_probe,
	.remove = vmic_remove,
};

static int __init vmic_init(void)
{
	return pci_register_driver(&vmic_driver);
}

static int vmic_probe(struct pci_dev *pdev, const struct pci_device_id *id)
{
	int retval;
	u32 data;

	/* Enable the device */
	retval = pci_enable_device(pdev);
	if (retval) {
		dev_err(&pdev->dev, "Unable to enable device\n");
		goto err;
	}

	/* Map Registers */
	retval = pci_request_regions(pdev, driver_name);
	if (retval) {
		dev_err(&pdev->dev, "Unable to reserve resources\n");
		goto err_resource;
	}

	/* Map registers in BAR 0 */
	vmic_base = ioremap_nocache(pci_resource_start(pdev, 0), 16);
	if (!vmic_base) {
		dev_err(&pdev->dev, "Unable to remap CRG region\n");
		retval = -EIO;
		goto err_remap;
	}

	/* Clear the FPGA VME IF contents */
	iowrite32(0, vmic_base + VME_CONTROL);

	/* Clear any initial BERR  */
	data = ioread32(vmic_base + VME_CONTROL) & 0x00000FFF;
	data |= BM_VME_CONTROL_BERRST;
	iowrite32(data, vmic_base + VME_CONTROL);

	/* Enable the vme interface and byte swapping */
	data = ioread32(vmic_base + VME_CONTROL) & 0x00000FFF;
	data = data | BM_VME_CONTROL_MASTER_ENDIAN |
			BM_VME_CONTROL_SLAVE_ENDIAN |
			BM_VME_CONTROL_ABLE |
			BM_VME_CONTROL_BERRI |
			BM_VME_CONTROL_BPENA |
			BM_VME_CONTROL_VBENA;
	iowrite32(data, vmic_base + VME_CONTROL);

	return 0;

err_remap:
	pci_release_regions(pdev);
err_resource:
	pci_disable_device(pdev);
err:
	return retval;
}

static void vmic_remove(struct pci_dev *pdev)
{
	iounmap(vmic_base);
	pci_release_regions(pdev);
	pci_disable_device(pdev);

}

static void __exit vmic_exit(void)
{
	pci_unregister_driver(&vmic_driver);
}

MODULE_DESCRIPTION("VMIVME-7805 board support driver");
MODULE_AUTHOR("Arthur Benilov <arthur.benilov@iba-group.com>");
MODULE_LICENSE("GPL");

module_init(vmic_init);
module_exit(vmic_exit);

