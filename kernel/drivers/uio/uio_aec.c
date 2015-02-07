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
 * uio_aec.c -- simple driver for Adrienne Electronics Corp time code PCI device
 *
 * Copyright (C) 2008 Brandon Philips <brandon@ifup.org>
 *
 *   This program is free software; you can redistribute it and/or modify it
 *   under the terms of the GNU General Public License version 2 as published
 *   by the Free Software Foundation.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License along
 *   with this program; if not, write to the Free Software Foundation, Inc., 59
 *   Temple Place, Suite 330, Boston, MA 02111-1307, USA.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/pci.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/io.h>
#include <linux/uaccess.h>
#include <linux/uio_driver.h>
#include <linux/slab.h>

#define PCI_VENDOR_ID_AEC 0xaecb
#define PCI_DEVICE_ID_AEC_VITCLTC 0x6250

#define INT_ENABLE_ADDR		0xFC
#define INT_ENABLE		0x10
#define INT_DISABLE		0x0

#define INT_MASK_ADDR		0x2E
#define INT_MASK_ALL		0x3F

#define INTA_DRVR_ADDR		0xFE
#define INTA_ENABLED_FLAG	0x08
#define INTA_FLAG		0x01

#define MAILBOX			0x0F

static struct pci_device_id ids[] = {
	{ PCI_DEVICE(PCI_VENDOR_ID_AEC, PCI_DEVICE_ID_AEC_VITCLTC), },
	{ 0, }
};
MODULE_DEVICE_TABLE(pci, ids);

static irqreturn_t aectc_irq(int irq, struct uio_info *dev_info)
{
	void __iomem *int_flag = dev_info->priv + INTA_DRVR_ADDR;
	unsigned char status = ioread8(int_flag);


	if ((status & INTA_ENABLED_FLAG) && (status & INTA_FLAG)) {
		/* application writes 0x00 to 0x2F to get next interrupt */
		status = ioread8(dev_info->priv + MAILBOX);
		return IRQ_HANDLED;
	}

	return IRQ_NONE;
}

static void print_board_data(struct pci_dev *pdev, struct uio_info *i)
{
	dev_info(&pdev->dev, "PCI-TC board vendor: %x%x number: %x%x"
		" revision: %c%c\n",
		ioread8(i->priv + 0x01),
		ioread8(i->priv + 0x00),
		ioread8(i->priv + 0x03),
		ioread8(i->priv + 0x02),
		ioread8(i->priv + 0x06),
		ioread8(i->priv + 0x07));
}

static int __devinit probe(struct pci_dev *pdev, const struct pci_device_id *id)
{
	struct uio_info *info;
	int ret;

	info = kzalloc(sizeof(struct uio_info), GFP_KERNEL);
	if (!info)
		return -ENOMEM;

	if (pci_enable_device(pdev))
		goto out_free;

	if (pci_request_regions(pdev, "aectc"))
		goto out_disable;

	info->name = "aectc";
	info->port[0].start = pci_resource_start(pdev, 0);
	if (!info->port[0].start)
		goto out_release;
	info->priv = pci_iomap(pdev, 0, 0);
	if (!info->priv)
		goto out_release;
	info->port[0].size = pci_resource_len(pdev, 0);
	info->port[0].porttype = UIO_PORT_GPIO;

	info->version = "0.0.1";
	info->irq = pdev->irq;
	info->irq_flags = IRQF_SHARED;
	info->handler = aectc_irq;

	print_board_data(pdev, info);
	ret = uio_register_device(&pdev->dev, info);
	if (ret)
		goto out_unmap;

	iowrite32(INT_ENABLE, info->priv + INT_ENABLE_ADDR);
	iowrite8(INT_MASK_ALL, info->priv + INT_MASK_ADDR);
	if (!(ioread8(info->priv + INTA_DRVR_ADDR)
			& INTA_ENABLED_FLAG))
		dev_err(&pdev->dev, "aectc: interrupts not enabled\n");

	pci_set_drvdata(pdev, info);

	return 0;

out_unmap:
	pci_iounmap(pdev, info->priv);
out_release:
	pci_release_regions(pdev);
out_disable:
	pci_disable_device(pdev);
out_free:
	kfree(info);
	return -ENODEV;
}

static void remove(struct pci_dev *pdev)
{
	struct uio_info *info = pci_get_drvdata(pdev);

	/* disable interrupts */
	iowrite8(INT_DISABLE, info->priv + INT_MASK_ADDR);
	iowrite32(INT_DISABLE, info->priv + INT_ENABLE_ADDR);
	/* read mailbox to ensure board drops irq */
	ioread8(info->priv + MAILBOX);

	uio_unregister_device(info);
	pci_release_regions(pdev);
	pci_disable_device(pdev);
	pci_set_drvdata(pdev, NULL);
	iounmap(info->priv);

	kfree(info);
}

static struct pci_driver pci_driver = {
	.name = "aectc",
	.id_table = ids,
	.probe = probe,
	.remove = remove,
};

static int __init aectc_init(void)
{
	return pci_register_driver(&pci_driver);
}

static void __exit aectc_exit(void)
{
	pci_unregister_driver(&pci_driver);
}

MODULE_LICENSE("GPL");

module_init(aectc_init);
module_exit(aectc_exit);
