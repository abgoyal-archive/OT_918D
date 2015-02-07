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
 * Shared support code for AMD K8 northbridges and derivates.
 * Copyright 2006 Andi Kleen, SUSE Labs. Subject to GPLv2.
 */
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/module.h>
#include <linux/spinlock.h>
#include <asm/k8.h>

int num_k8_northbridges;
EXPORT_SYMBOL(num_k8_northbridges);

static u32 *flush_words;

struct pci_device_id k8_nb_ids[] = {
	{ PCI_DEVICE(PCI_VENDOR_ID_AMD, PCI_DEVICE_ID_AMD_K8_NB_MISC) },
	{ PCI_DEVICE(PCI_VENDOR_ID_AMD, PCI_DEVICE_ID_AMD_10H_NB_MISC) },
	{}
};
EXPORT_SYMBOL(k8_nb_ids);

struct pci_dev **k8_northbridges;
EXPORT_SYMBOL(k8_northbridges);

static struct pci_dev *next_k8_northbridge(struct pci_dev *dev)
{
	do {
		dev = pci_get_device(PCI_ANY_ID, PCI_ANY_ID, dev);
		if (!dev)
			break;
	} while (!pci_match_id(&k8_nb_ids[0], dev));
	return dev;
}

int cache_k8_northbridges(void)
{
	int i;
	struct pci_dev *dev;

	if (num_k8_northbridges)
		return 0;

	dev = NULL;
	while ((dev = next_k8_northbridge(dev)) != NULL)
		num_k8_northbridges++;

	k8_northbridges = kmalloc((num_k8_northbridges + 1) * sizeof(void *),
				  GFP_KERNEL);
	if (!k8_northbridges)
		return -ENOMEM;

	if (!num_k8_northbridges) {
		k8_northbridges[0] = NULL;
		return 0;
	}

	flush_words = kmalloc(num_k8_northbridges * sizeof(u32), GFP_KERNEL);
	if (!flush_words) {
		kfree(k8_northbridges);
		return -ENOMEM;
	}

	dev = NULL;
	i = 0;
	while ((dev = next_k8_northbridge(dev)) != NULL) {
		k8_northbridges[i] = dev;
		pci_read_config_dword(dev, 0x9c, &flush_words[i++]);
	}
	k8_northbridges[i] = NULL;
	return 0;
}
EXPORT_SYMBOL_GPL(cache_k8_northbridges);

/* Ignores subdevice/subvendor but as far as I can figure out
   they're useless anyways */
int __init early_is_k8_nb(u32 device)
{
	struct pci_device_id *id;
	u32 vendor = device & 0xffff;
	device >>= 16;
	for (id = k8_nb_ids; id->vendor; id++)
		if (vendor == id->vendor && device == id->device)
			return 1;
	return 0;
}

void k8_flush_garts(void)
{
	int flushed, i;
	unsigned long flags;
	static DEFINE_SPINLOCK(gart_lock);

	/* Avoid races between AGP and IOMMU. In theory it's not needed
	   but I'm not sure if the hardware won't lose flush requests
	   when another is pending. This whole thing is so expensive anyways
	   that it doesn't matter to serialize more. -AK */
	spin_lock_irqsave(&gart_lock, flags);
	flushed = 0;
	for (i = 0; i < num_k8_northbridges; i++) {
		pci_write_config_dword(k8_northbridges[i], 0x9c,
				       flush_words[i]|1);
		flushed++;
	}
	for (i = 0; i < num_k8_northbridges; i++) {
		u32 w;
		/* Make sure the hardware actually executed the flush*/
		for (;;) {
			pci_read_config_dword(k8_northbridges[i],
					      0x9c, &w);
			if (!(w & 1))
				break;
			cpu_relax();
		}
	}
	spin_unlock_irqrestore(&gart_lock, flags);
	if (!flushed)
		printk("nothing to flush?\n");
}
EXPORT_SYMBOL_GPL(k8_flush_garts);

static __init int init_k8_nbs(void)
{
	int err = 0;

	err = cache_k8_northbridges();

	if (err < 0)
		printk(KERN_NOTICE "K8 NB: Cannot enumerate AMD northbridges.\n");

	return err;
}

/* This has to go after the PCI subsystem */
fs_initcall(init_k8_nbs);
