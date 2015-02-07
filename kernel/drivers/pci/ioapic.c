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
 * IOAPIC/IOxAPIC/IOSAPIC driver
 *
 * Copyright (C) 2009 Fujitsu Limited.
 * (c) Copyright 2009 Hewlett-Packard Development Company, L.P.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

/*
 * This driver manages PCI I/O APICs added by hotplug after boot.  We try to
 * claim all I/O APIC PCI devices, but those present at boot were registered
 * when we parsed the ACPI MADT, so we'll fail when we try to re-register
 * them.
 */

#include <linux/pci.h>
#include <linux/acpi.h>
#include <linux/slab.h>
#include <acpi/acpi_bus.h>

struct ioapic {
	acpi_handle	handle;
	u32		gsi_base;
};

static int ioapic_probe(struct pci_dev *dev, const struct pci_device_id *ent)
{
	acpi_handle handle;
	acpi_status status;
	unsigned long long gsb;
	struct ioapic *ioapic;
	int ret;
	char *type;
	struct resource *res;

	handle = DEVICE_ACPI_HANDLE(&dev->dev);
	if (!handle)
		return -EINVAL;

	status = acpi_evaluate_integer(handle, "_GSB", NULL, &gsb);
	if (ACPI_FAILURE(status))
		return -EINVAL;

	/*
	 * The previous code in acpiphp evaluated _MAT if _GSB failed, but
	 * ACPI spec 4.0 sec 6.2.2 requires _GSB for hot-pluggable I/O APICs.
	 */

	ioapic = kzalloc(sizeof(*ioapic), GFP_KERNEL);
	if (!ioapic)
		return -ENOMEM;

	ioapic->handle = handle;
	ioapic->gsi_base = (u32) gsb;

	if (dev->class == PCI_CLASS_SYSTEM_PIC_IOAPIC)
		type = "IOAPIC";
	else
		type = "IOxAPIC";

	ret = pci_enable_device(dev);
	if (ret < 0)
		goto exit_free;

	pci_set_master(dev);

	if (pci_request_region(dev, 0, type))
		goto exit_disable;

	res = &dev->resource[0];
	if (acpi_register_ioapic(ioapic->handle, res->start, ioapic->gsi_base))
		goto exit_release;

	pci_set_drvdata(dev, ioapic);
	dev_info(&dev->dev, "%s at %pR, GSI %u\n", type, res, ioapic->gsi_base);
	return 0;

exit_release:
	pci_release_region(dev, 0);
exit_disable:
	pci_disable_device(dev);
exit_free:
	kfree(ioapic);
	return -ENODEV;
}

static void ioapic_remove(struct pci_dev *dev)
{
	struct ioapic *ioapic = pci_get_drvdata(dev);

	acpi_unregister_ioapic(ioapic->handle, ioapic->gsi_base);
	pci_release_region(dev, 0);
	pci_disable_device(dev);
	kfree(ioapic);
}


static struct pci_device_id ioapic_devices[] = {
	{ PCI_ANY_ID, PCI_ANY_ID, PCI_ANY_ID, PCI_ANY_ID,
	  PCI_CLASS_SYSTEM_PIC_IOAPIC << 8, 0xffff00, },
	{ PCI_ANY_ID, PCI_ANY_ID, PCI_ANY_ID, PCI_ANY_ID,
	  PCI_CLASS_SYSTEM_PIC_IOXAPIC << 8, 0xffff00, },
	{ }
};

static struct pci_driver ioapic_driver = {
	.name		= "ioapic",
	.id_table	= ioapic_devices,
	.probe		= ioapic_probe,
	.remove		= __devexit_p(ioapic_remove),
};

static int __init ioapic_init(void)
{
	return pci_register_driver(&ioapic_driver);
}

static void __exit ioapic_exit(void)
{
	pci_unregister_driver(&ioapic_driver);
}

module_init(ioapic_init);
module_exit(ioapic_exit);
