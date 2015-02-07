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
 * File:	portdrv_pci.c
 * Purpose:	PCI Express Port Bus Driver
 *
 * Copyright (C) 2004 Intel
 * Copyright (C) Tom Long Nguyen (tom.l.nguyen@intel.com)
 */

#include <linux/module.h>
#include <linux/pci.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/pm.h>
#include <linux/init.h>
#include <linux/pcieport_if.h>
#include <linux/aer.h>
#include <linux/dmi.h>

#include "portdrv.h"
#include "aer/aerdrv.h"

/*
 * Version Information
 */
#define DRIVER_VERSION "v1.0"
#define DRIVER_AUTHOR "tom.l.nguyen@intel.com"
#define DRIVER_DESC "PCIe Port Bus Driver"
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_LICENSE("GPL");

/* global data */

static int pcie_portdrv_restore_config(struct pci_dev *dev)
{
	int retval;

	retval = pci_enable_device(dev);
	if (retval)
		return retval;
	pci_set_master(dev);
	return 0;
}

#ifdef CONFIG_PM
static const struct dev_pm_ops pcie_portdrv_pm_ops = {
	.suspend	= pcie_port_device_suspend,
	.resume		= pcie_port_device_resume,
	.freeze		= pcie_port_device_suspend,
	.thaw		= pcie_port_device_resume,
	.poweroff	= pcie_port_device_suspend,
	.restore	= pcie_port_device_resume,
};

#define PCIE_PORTDRV_PM_OPS	(&pcie_portdrv_pm_ops)

#else /* !PM */

#define PCIE_PORTDRV_PM_OPS	NULL
#endif /* !PM */

/*
 * pcie_portdrv_probe - Probe PCI-Express port devices
 * @dev: PCI-Express port device being probed
 *
 * If detected invokes the pcie_port_device_register() method for
 * this port device.
 *
 */
static int __devinit pcie_portdrv_probe(struct pci_dev *dev,
					const struct pci_device_id *id)
{
	int status;

	if (!pci_is_pcie(dev) ||
	    ((dev->pcie_type != PCI_EXP_TYPE_ROOT_PORT) &&
	     (dev->pcie_type != PCI_EXP_TYPE_UPSTREAM) &&
	     (dev->pcie_type != PCI_EXP_TYPE_DOWNSTREAM)))
		return -ENODEV;

	if (!dev->irq && dev->pin) {
		dev_warn(&dev->dev, "device [%04x:%04x] has invalid IRQ; "
			 "check vendor BIOS\n", dev->vendor, dev->device);
	}
	status = pcie_port_device_register(dev);
	if (status)
		return status;

	pci_save_state(dev);

	return 0;
}

static void pcie_portdrv_remove(struct pci_dev *dev)
{
	pcie_port_device_remove(dev);
	pci_disable_device(dev);
}

static int error_detected_iter(struct device *device, void *data)
{
	struct pcie_device *pcie_device;
	struct pcie_port_service_driver *driver;
	struct aer_broadcast_data *result_data;
	pci_ers_result_t status;

	result_data = (struct aer_broadcast_data *) data;

	if (device->bus == &pcie_port_bus_type && device->driver) {
		driver = to_service_driver(device->driver);
		if (!driver ||
			!driver->err_handler ||
			!driver->err_handler->error_detected)
			return 0;

		pcie_device = to_pcie_device(device);

		/* Forward error detected message to service drivers */
		status = driver->err_handler->error_detected(
			pcie_device->port,
			result_data->state);
		result_data->result =
			merge_result(result_data->result, status);
	}

	return 0;
}

static pci_ers_result_t pcie_portdrv_error_detected(struct pci_dev *dev,
					enum pci_channel_state error)
{
	struct aer_broadcast_data data = {error, PCI_ERS_RESULT_CAN_RECOVER};
	int ret;

	/* can not fail */
	ret = device_for_each_child(&dev->dev, &data, error_detected_iter);

	return data.result;
}

static int mmio_enabled_iter(struct device *device, void *data)
{
	struct pcie_device *pcie_device;
	struct pcie_port_service_driver *driver;
	pci_ers_result_t status, *result;

	result = (pci_ers_result_t *) data;

	if (device->bus == &pcie_port_bus_type && device->driver) {
		driver = to_service_driver(device->driver);
		if (driver &&
			driver->err_handler &&
			driver->err_handler->mmio_enabled) {
			pcie_device = to_pcie_device(device);

			/* Forward error message to service drivers */
			status = driver->err_handler->mmio_enabled(
					pcie_device->port);
			*result = merge_result(*result, status);
		}
	}

	return 0;
}

static pci_ers_result_t pcie_portdrv_mmio_enabled(struct pci_dev *dev)
{
	pci_ers_result_t status = PCI_ERS_RESULT_RECOVERED;
	int retval;

	/* get true return value from &status */
	retval = device_for_each_child(&dev->dev, &status, mmio_enabled_iter);
	return status;
}

static int slot_reset_iter(struct device *device, void *data)
{
	struct pcie_device *pcie_device;
	struct pcie_port_service_driver *driver;
	pci_ers_result_t status, *result;

	result = (pci_ers_result_t *) data;

	if (device->bus == &pcie_port_bus_type && device->driver) {
		driver = to_service_driver(device->driver);
		if (driver &&
			driver->err_handler &&
			driver->err_handler->slot_reset) {
			pcie_device = to_pcie_device(device);

			/* Forward error message to service drivers */
			status = driver->err_handler->slot_reset(
					pcie_device->port);
			*result = merge_result(*result, status);
		}
	}

	return 0;
}

static pci_ers_result_t pcie_portdrv_slot_reset(struct pci_dev *dev)
{
	pci_ers_result_t status = PCI_ERS_RESULT_RECOVERED;
	int retval;

	/* If fatal, restore cfg space for possible link reset at upstream */
	if (dev->error_state == pci_channel_io_frozen) {
		dev->state_saved = true;
		pci_restore_state(dev);
		pcie_portdrv_restore_config(dev);
		pci_enable_pcie_error_reporting(dev);
	}

	/* get true return value from &status */
	retval = device_for_each_child(&dev->dev, &status, slot_reset_iter);

	return status;
}

static int resume_iter(struct device *device, void *data)
{
	struct pcie_device *pcie_device;
	struct pcie_port_service_driver *driver;

	if (device->bus == &pcie_port_bus_type && device->driver) {
		driver = to_service_driver(device->driver);
		if (driver &&
			driver->err_handler &&
			driver->err_handler->resume) {
			pcie_device = to_pcie_device(device);

			/* Forward error message to service drivers */
			driver->err_handler->resume(pcie_device->port);
		}
	}

	return 0;
}

static void pcie_portdrv_err_resume(struct pci_dev *dev)
{
	int retval;
	/* nothing to do with error value, if it ever happens */
	retval = device_for_each_child(&dev->dev, NULL, resume_iter);
}

/*
 * LINUX Device Driver Model
 */
static const struct pci_device_id port_pci_ids[] = { {
	/* handle any PCI-Express port */
	PCI_DEVICE_CLASS(((PCI_CLASS_BRIDGE_PCI << 8) | 0x00), ~0),
	}, { /* end: all zeroes */ }
};
MODULE_DEVICE_TABLE(pci, port_pci_ids);

static struct pci_error_handlers pcie_portdrv_err_handler = {
		.error_detected = pcie_portdrv_error_detected,
		.mmio_enabled = pcie_portdrv_mmio_enabled,
		.slot_reset = pcie_portdrv_slot_reset,
		.resume = pcie_portdrv_err_resume,
};

static struct pci_driver pcie_portdriver = {
	.name		= "pcieport",
	.id_table	= &port_pci_ids[0],

	.probe		= pcie_portdrv_probe,
	.remove		= pcie_portdrv_remove,

	.err_handler 	= &pcie_portdrv_err_handler,

	.driver.pm 	= PCIE_PORTDRV_PM_OPS,
};

static int __init dmi_pcie_pme_disable_msi(const struct dmi_system_id *d)
{
	pr_notice("%s detected: will not use MSI for PCIe PME signaling\n",
			d->ident);
	pcie_pme_disable_msi();
	return 0;
}

static struct dmi_system_id __initdata pcie_portdrv_dmi_table[] = {
	/*
	 * Boxes that should not use MSI for PCIe PME signaling.
	 */
	{
	 .callback = dmi_pcie_pme_disable_msi,
	 .ident = "MSI Wind U-100",
	 .matches = {
		     DMI_MATCH(DMI_SYS_VENDOR,
		     		"MICRO-STAR INTERNATIONAL CO., LTD"),
		     DMI_MATCH(DMI_PRODUCT_NAME, "U-100"),
		     },
	 },
	 {}
};

static int __init pcie_portdrv_init(void)
{
	int retval;

	dmi_check_system(pcie_portdrv_dmi_table);

	retval = pcie_port_bus_register();
	if (retval) {
		printk(KERN_WARNING "PCIE: bus_register error: %d\n", retval);
		goto out;
	}
	retval = pci_register_driver(&pcie_portdriver);
	if (retval)
		pcie_port_bus_unregister();
 out:
	return retval;
}

static void __exit pcie_portdrv_exit(void)
{
	pci_unregister_driver(&pcie_portdriver);
	pcie_port_bus_unregister();
}

module_init(pcie_portdrv_init);
module_exit(pcie_portdrv_exit);