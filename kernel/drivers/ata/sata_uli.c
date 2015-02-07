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
 *  sata_uli.c - ULi Electronics SATA
 *
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *
 *  libata documentation is available via 'make {ps|pdf}docs',
 *  as Documentation/DocBook/libata.*
 *
 *  Hardware documentation available under NDA.
 *
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/gfp.h>
#include <linux/pci.h>
#include <linux/init.h>
#include <linux/blkdev.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/device.h>
#include <scsi/scsi_host.h>
#include <linux/libata.h>

#define DRV_NAME	"sata_uli"
#define DRV_VERSION	"1.3"

enum {
	uli_5289		= 0,
	uli_5287		= 1,
	uli_5281		= 2,

	uli_max_ports		= 4,

	/* PCI configuration registers */
	ULI5287_BASE		= 0x90, /* sata0 phy SCR registers */
	ULI5287_OFFS		= 0x10, /* offset from sata0->sata1 phy regs */
	ULI5281_BASE		= 0x60, /* sata0 phy SCR  registers */
	ULI5281_OFFS		= 0x60, /* offset from sata0->sata1 phy regs */
};

struct uli_priv {
	unsigned int		scr_cfg_addr[uli_max_ports];
};

static int uli_init_one(struct pci_dev *pdev, const struct pci_device_id *ent);
static int uli_scr_read(struct ata_link *link, unsigned int sc_reg, u32 *val);
static int uli_scr_write(struct ata_link *link, unsigned int sc_reg, u32 val);

static const struct pci_device_id uli_pci_tbl[] = {
	{ PCI_VDEVICE(AL, 0x5289), uli_5289 },
	{ PCI_VDEVICE(AL, 0x5287), uli_5287 },
	{ PCI_VDEVICE(AL, 0x5281), uli_5281 },

	{ }	/* terminate list */
};

static struct pci_driver uli_pci_driver = {
	.name			= DRV_NAME,
	.id_table		= uli_pci_tbl,
	.probe			= uli_init_one,
	.remove			= ata_pci_remove_one,
};

static struct scsi_host_template uli_sht = {
	ATA_BMDMA_SHT(DRV_NAME),
};

static struct ata_port_operations uli_ops = {
	.inherits		= &ata_bmdma_port_ops,
	.scr_read		= uli_scr_read,
	.scr_write		= uli_scr_write,
	.hardreset		= ATA_OP_NULL,
};

static const struct ata_port_info uli_port_info = {
	.flags		= ATA_FLAG_SATA | ATA_FLAG_NO_LEGACY |
			  ATA_FLAG_IGN_SIMPLEX,
	.pio_mask       = ATA_PIO4,
	.udma_mask      = ATA_UDMA6,
	.port_ops       = &uli_ops,
};


MODULE_AUTHOR("Peer Chen");
MODULE_DESCRIPTION("low-level driver for ULi Electronics SATA controller");
MODULE_LICENSE("GPL");
MODULE_DEVICE_TABLE(pci, uli_pci_tbl);
MODULE_VERSION(DRV_VERSION);

static unsigned int get_scr_cfg_addr(struct ata_port *ap, unsigned int sc_reg)
{
	struct uli_priv *hpriv = ap->host->private_data;
	return hpriv->scr_cfg_addr[ap->port_no] + (4 * sc_reg);
}

static u32 uli_scr_cfg_read(struct ata_link *link, unsigned int sc_reg)
{
	struct pci_dev *pdev = to_pci_dev(link->ap->host->dev);
	unsigned int cfg_addr = get_scr_cfg_addr(link->ap, sc_reg);
	u32 val;

	pci_read_config_dword(pdev, cfg_addr, &val);
	return val;
}

static void uli_scr_cfg_write(struct ata_link *link, unsigned int scr, u32 val)
{
	struct pci_dev *pdev = to_pci_dev(link->ap->host->dev);
	unsigned int cfg_addr = get_scr_cfg_addr(link->ap, scr);

	pci_write_config_dword(pdev, cfg_addr, val);
}

static int uli_scr_read(struct ata_link *link, unsigned int sc_reg, u32 *val)
{
	if (sc_reg > SCR_CONTROL)
		return -EINVAL;

	*val = uli_scr_cfg_read(link, sc_reg);
	return 0;
}

static int uli_scr_write(struct ata_link *link, unsigned int sc_reg, u32 val)
{
	if (sc_reg > SCR_CONTROL) //SCR_CONTROL=2, SCR_ERROR=1, SCR_STATUS=0
		return -EINVAL;

	uli_scr_cfg_write(link, sc_reg, val);
	return 0;
}

static int uli_init_one(struct pci_dev *pdev, const struct pci_device_id *ent)
{
	static int printed_version;
	const struct ata_port_info *ppi[] = { &uli_port_info, NULL };
	unsigned int board_idx = (unsigned int) ent->driver_data;
	struct ata_host *host;
	struct uli_priv *hpriv;
	void __iomem * const *iomap;
	struct ata_ioports *ioaddr;
	int n_ports, rc;

	if (!printed_version++)
		dev_printk(KERN_INFO, &pdev->dev, "version " DRV_VERSION "\n");

	rc = pcim_enable_device(pdev);
	if (rc)
		return rc;

	n_ports = 2;
	if (board_idx == uli_5287)
		n_ports = 4;

	/* allocate the host */
	host = ata_host_alloc_pinfo(&pdev->dev, ppi, n_ports);
	if (!host)
		return -ENOMEM;

	hpriv = devm_kzalloc(&pdev->dev, sizeof(*hpriv), GFP_KERNEL);
	if (!hpriv)
		return -ENOMEM;
	host->private_data = hpriv;

	/* the first two ports are standard SFF */
	rc = ata_pci_sff_init_host(host);
	if (rc)
		return rc;

	ata_pci_bmdma_init(host);

	iomap = host->iomap;

	switch (board_idx) {
	case uli_5287:
		/* If there are four, the last two live right after
		 * the standard SFF ports.
		 */
		hpriv->scr_cfg_addr[0] = ULI5287_BASE;
		hpriv->scr_cfg_addr[1] = ULI5287_BASE + ULI5287_OFFS;

		ioaddr = &host->ports[2]->ioaddr;
		ioaddr->cmd_addr = iomap[0] + 8;
		ioaddr->altstatus_addr =
		ioaddr->ctl_addr = (void __iomem *)
			((unsigned long)iomap[1] | ATA_PCI_CTL_OFS) + 4;
		ioaddr->bmdma_addr = iomap[4] + 16;
		hpriv->scr_cfg_addr[2] = ULI5287_BASE + ULI5287_OFFS*4;
		ata_sff_std_ports(ioaddr);

		ata_port_desc(host->ports[2],
			"cmd 0x%llx ctl 0x%llx bmdma 0x%llx",
			(unsigned long long)pci_resource_start(pdev, 0) + 8,
			((unsigned long long)pci_resource_start(pdev, 1) | ATA_PCI_CTL_OFS) + 4,
			(unsigned long long)pci_resource_start(pdev, 4) + 16);

		ioaddr = &host->ports[3]->ioaddr;
		ioaddr->cmd_addr = iomap[2] + 8;
		ioaddr->altstatus_addr =
		ioaddr->ctl_addr = (void __iomem *)
			((unsigned long)iomap[3] | ATA_PCI_CTL_OFS) + 4;
		ioaddr->bmdma_addr = iomap[4] + 24;
		hpriv->scr_cfg_addr[3] = ULI5287_BASE + ULI5287_OFFS*5;
		ata_sff_std_ports(ioaddr);

		ata_port_desc(host->ports[2],
			"cmd 0x%llx ctl 0x%llx bmdma 0x%llx",
			(unsigned long long)pci_resource_start(pdev, 2) + 9,
			((unsigned long long)pci_resource_start(pdev, 3) | ATA_PCI_CTL_OFS) + 4,
			(unsigned long long)pci_resource_start(pdev, 4) + 24);

		break;

	case uli_5289:
		hpriv->scr_cfg_addr[0] = ULI5287_BASE;
		hpriv->scr_cfg_addr[1] = ULI5287_BASE + ULI5287_OFFS;
		break;

	case uli_5281:
		hpriv->scr_cfg_addr[0] = ULI5281_BASE;
		hpriv->scr_cfg_addr[1] = ULI5281_BASE + ULI5281_OFFS;
		break;

	default:
		BUG();
		break;
	}

	pci_set_master(pdev);
	pci_intx(pdev, 1);
	return ata_host_activate(host, pdev->irq, ata_bmdma_interrupt,
				 IRQF_SHARED, &uli_sht);
}

static int __init uli_init(void)
{
	return pci_register_driver(&uli_pci_driver);
}

static void __exit uli_exit(void)
{
	pci_unregister_driver(&uli_pci_driver);
}


module_init(uli_init);
module_exit(uli_exit);