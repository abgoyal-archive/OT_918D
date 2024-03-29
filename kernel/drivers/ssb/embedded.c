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
 * Sonics Silicon Backplane
 * Embedded systems support code
 *
 * Copyright 2005-2008, Broadcom Corporation
 * Copyright 2006-2008, Michael Buesch <mb@bu3sch.de>
 *
 * Licensed under the GNU/GPL. See COPYING for details.
 */

#include <linux/ssb/ssb.h>
#include <linux/ssb/ssb_embedded.h>
#include <linux/ssb/ssb_driver_pci.h>
#include <linux/ssb/ssb_driver_gige.h>
#include <linux/pci.h>

#include "ssb_private.h"


int ssb_watchdog_timer_set(struct ssb_bus *bus, u32 ticks)
{
	if (ssb_chipco_available(&bus->chipco)) {
		ssb_chipco_watchdog_timer_set(&bus->chipco, ticks);
		return 0;
	}
	if (ssb_extif_available(&bus->extif)) {
		ssb_extif_watchdog_timer_set(&bus->extif, ticks);
		return 0;
	}
	return -ENODEV;
}
EXPORT_SYMBOL(ssb_watchdog_timer_set);

u32 ssb_gpio_in(struct ssb_bus *bus, u32 mask)
{
	unsigned long flags;
	u32 res = 0;

	spin_lock_irqsave(&bus->gpio_lock, flags);
	if (ssb_chipco_available(&bus->chipco))
		res = ssb_chipco_gpio_in(&bus->chipco, mask);
	else if (ssb_extif_available(&bus->extif))
		res = ssb_extif_gpio_in(&bus->extif, mask);
	else
		SSB_WARN_ON(1);
	spin_unlock_irqrestore(&bus->gpio_lock, flags);

	return res;
}
EXPORT_SYMBOL(ssb_gpio_in);

u32 ssb_gpio_out(struct ssb_bus *bus, u32 mask, u32 value)
{
	unsigned long flags;
	u32 res = 0;

	spin_lock_irqsave(&bus->gpio_lock, flags);
	if (ssb_chipco_available(&bus->chipco))
		res = ssb_chipco_gpio_out(&bus->chipco, mask, value);
	else if (ssb_extif_available(&bus->extif))
		res = ssb_extif_gpio_out(&bus->extif, mask, value);
	else
		SSB_WARN_ON(1);
	spin_unlock_irqrestore(&bus->gpio_lock, flags);

	return res;
}
EXPORT_SYMBOL(ssb_gpio_out);

u32 ssb_gpio_outen(struct ssb_bus *bus, u32 mask, u32 value)
{
	unsigned long flags;
	u32 res = 0;

	spin_lock_irqsave(&bus->gpio_lock, flags);
	if (ssb_chipco_available(&bus->chipco))
		res = ssb_chipco_gpio_outen(&bus->chipco, mask, value);
	else if (ssb_extif_available(&bus->extif))
		res = ssb_extif_gpio_outen(&bus->extif, mask, value);
	else
		SSB_WARN_ON(1);
	spin_unlock_irqrestore(&bus->gpio_lock, flags);

	return res;
}
EXPORT_SYMBOL(ssb_gpio_outen);

u32 ssb_gpio_control(struct ssb_bus *bus, u32 mask, u32 value)
{
	unsigned long flags;
	u32 res = 0;

	spin_lock_irqsave(&bus->gpio_lock, flags);
	if (ssb_chipco_available(&bus->chipco))
		res = ssb_chipco_gpio_control(&bus->chipco, mask, value);
	spin_unlock_irqrestore(&bus->gpio_lock, flags);

	return res;
}
EXPORT_SYMBOL(ssb_gpio_control);

u32 ssb_gpio_intmask(struct ssb_bus *bus, u32 mask, u32 value)
{
	unsigned long flags;
	u32 res = 0;

	spin_lock_irqsave(&bus->gpio_lock, flags);
	if (ssb_chipco_available(&bus->chipco))
		res = ssb_chipco_gpio_intmask(&bus->chipco, mask, value);
	else if (ssb_extif_available(&bus->extif))
		res = ssb_extif_gpio_intmask(&bus->extif, mask, value);
	else
		SSB_WARN_ON(1);
	spin_unlock_irqrestore(&bus->gpio_lock, flags);

	return res;
}
EXPORT_SYMBOL(ssb_gpio_intmask);

u32 ssb_gpio_polarity(struct ssb_bus *bus, u32 mask, u32 value)
{
	unsigned long flags;
	u32 res = 0;

	spin_lock_irqsave(&bus->gpio_lock, flags);
	if (ssb_chipco_available(&bus->chipco))
		res = ssb_chipco_gpio_polarity(&bus->chipco, mask, value);
	else if (ssb_extif_available(&bus->extif))
		res = ssb_extif_gpio_polarity(&bus->extif, mask, value);
	else
		SSB_WARN_ON(1);
	spin_unlock_irqrestore(&bus->gpio_lock, flags);

	return res;
}
EXPORT_SYMBOL(ssb_gpio_polarity);

#ifdef CONFIG_SSB_DRIVER_GIGE
static int gige_pci_init_callback(struct ssb_bus *bus, unsigned long data)
{
	struct pci_dev *pdev = (struct pci_dev *)data;
	struct ssb_device *dev;
	unsigned int i;
	int res;

	for (i = 0; i < bus->nr_devices; i++) {
		dev = &(bus->devices[i]);
		if (dev->id.coreid != SSB_DEV_ETHERNET_GBIT)
			continue;
		if (!dev->dev ||
		    !dev->dev->driver ||
		    !device_is_registered(dev->dev))
			continue;
		res = ssb_gige_pcibios_plat_dev_init(dev, pdev);
		if (res >= 0)
			return res;
	}

	return -ENODEV;
}
#endif /* CONFIG_SSB_DRIVER_GIGE */

int ssb_pcibios_plat_dev_init(struct pci_dev *dev)
{
	int err;

	err = ssb_pcicore_plat_dev_init(dev);
	if (!err)
		return 0;
#ifdef CONFIG_SSB_DRIVER_GIGE
	err = ssb_for_each_bus_call((unsigned long)dev, gige_pci_init_callback);
	if (err >= 0)
		return err;
#endif
	/* This is not a PCI device on any SSB device. */

	return -ENODEV;
}

#ifdef CONFIG_SSB_DRIVER_GIGE
static int gige_map_irq_callback(struct ssb_bus *bus, unsigned long data)
{
	const struct pci_dev *pdev = (const struct pci_dev *)data;
	struct ssb_device *dev;
	unsigned int i;
	int res;

	for (i = 0; i < bus->nr_devices; i++) {
		dev = &(bus->devices[i]);
		if (dev->id.coreid != SSB_DEV_ETHERNET_GBIT)
			continue;
		if (!dev->dev ||
		    !dev->dev->driver ||
		    !device_is_registered(dev->dev))
			continue;
		res = ssb_gige_map_irq(dev, pdev);
		if (res >= 0)
			return res;
	}

	return -ENODEV;
}
#endif /* CONFIG_SSB_DRIVER_GIGE */

int ssb_pcibios_map_irq(const struct pci_dev *dev, u8 slot, u8 pin)
{
	int res;

	/* Check if this PCI device is a device on a SSB bus or device
	 * and return the IRQ number for it. */

	res = ssb_pcicore_pcibios_map_irq(dev, slot, pin);
	if (res >= 0)
		return res;
#ifdef CONFIG_SSB_DRIVER_GIGE
	res = ssb_for_each_bus_call((unsigned long)dev, gige_map_irq_callback);
	if (res >= 0)
		return res;
#endif
	/* This is not a PCI device on any SSB device. */

	return -ENODEV;
}
