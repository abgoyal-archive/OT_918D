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
 * Copyright (C) 2006		Red Hat <evan_ko@phison.com>
 *
 *  May be copied or modified under the terms of the GNU General Public License
 *
 *  [Modify History]
 *   #0001, Evan, 2008.10.22, V0.00, New release.
 *   #0002, Evan, 2008.11.01, V0.90, Test Work In Ubuntu Linux 8.04.
 *   #0003, Evan, 2008.01.08, V0.91, Change Name "PCIE-SSD" to "E-BOX".
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/pci.h>
#include <linux/init.h>
#include <linux/blkdev.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <scsi/scsi_host.h>
#include <linux/libata.h>
#include <linux/ata.h>

#define PHISON_DEBUG

#define DRV_NAME		"phison_e-box"	/* #0003 */
#define DRV_VERSION		"0.91"		/* #0003 */

#define PCI_VENDOR_ID_PHISON	0x1987
#define PCI_DEVICE_ID_PS5000	0x5000

static int phison_pre_reset(struct ata_link *link, unsigned long deadline)
{
	int ret;
	struct ata_port *ap = link->ap;

	ap->cbl = ATA_CBL_NONE;
	ret = ata_std_prereset(link, deadline);
	dev_dbg(ap->dev, "phison_pre_reset(), ret = %x\n", ret);
	return ret;
}

static struct scsi_host_template phison_sht = {
	ATA_BMDMA_SHT(DRV_NAME),
};

static struct ata_port_operations phison_ops = {
	.inherits		= &ata_bmdma_port_ops,
	.prereset		= phison_pre_reset,
};

static int phison_init_one(struct pci_dev *pdev, const struct pci_device_id *id)
{
	int ret;
	struct ata_port_info info = {
		.flags		= ATA_FLAG_NO_ATAPI,

		.pio_mask	= 0x1f,
		.mwdma_mask	= 0x07,
		.udma_mask	= ATA_UDMA5,

		.port_ops	= &phison_ops,
	};
	const struct ata_port_info *ppi[] = { &info, NULL };

	ret = ata_pci_sff_init_one(pdev, ppi, &phison_sht, NULL, 0);

	dev_dbg(&pdev->dev, "phison_init_one(), ret = %x\n", ret);

	return ret;
}

static const struct pci_device_id phison_pci_tbl[] = {
	{ PCI_VENDOR_ID_PHISON, PCI_DEVICE_ID_PS5000, PCI_ANY_ID, PCI_ANY_ID,
	  PCI_CLASS_STORAGE_IDE << 8, 0xffff00, 0 },
	{ 0, },
};
MODULE_DEVICE_TABLE(pci, phison_pci_tbl);

static struct pci_driver phison_pci_driver = {
	.name		= DRV_NAME,
	.id_table	= phison_pci_tbl,
	.probe		= phison_init_one,
	.remove		= ata_pci_remove_one,
#ifdef CONFIG_PM	/* haven't tested it. */
	.suspend	= ata_pci_device_suspend,
	.resume		= ata_pci_device_resume,
#endif
};

static int __init phison_ide_init(void)
{
	return pci_register_driver(&phison_pci_driver);
}

static void __exit phison_ide_exit(void)
{
	pci_unregister_driver(&phison_pci_driver);
}

module_init(phison_ide_init);
module_exit(phison_ide_exit);

MODULE_AUTHOR("Evan Ko");
MODULE_DESCRIPTION("PCIE driver module for PHISON PS5000 E-BOX");
MODULE_LICENSE("GPL");
MODULE_VERSION(DRV_VERSION);
