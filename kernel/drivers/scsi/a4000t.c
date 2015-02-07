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
 * Detection routine for the NCR53c710 based Amiga SCSI Controllers for Linux.
 *		Amiga Technologies A4000T SCSI controller.
 *
 * Written 1997 by Alan Hourihane <alanh@fairlite.demon.co.uk>
 * plus modifications of the 53c7xx.c driver to support the Amiga.
 *
 * Rewritten to use 53c700.c by Kars de Jong <jongk@linux-m68k.org>
 */

#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/slab.h>
#include <asm/amigahw.h>
#include <asm/amigaints.h>
#include <scsi/scsi_host.h>
#include <scsi/scsi_transport_spi.h>

#include "53c700.h"


static struct scsi_host_template a4000t_scsi_driver_template = {
	.name		= "A4000T builtin SCSI",
	.proc_name	= "A4000t",
	.this_id	= 7,
	.module		= THIS_MODULE,
};


#define A4000T_SCSI_OFFSET	0x40

static int __init amiga_a4000t_scsi_probe(struct platform_device *pdev)
{
	struct resource *res;
	phys_addr_t scsi_addr;
	struct NCR_700_Host_Parameters *hostdata;
	struct Scsi_Host *host;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res)
		return -ENODEV;

	if (!request_mem_region(res->start, resource_size(res),
				"A4000T builtin SCSI"))
		return -EBUSY;

	hostdata = kzalloc(sizeof(struct NCR_700_Host_Parameters),
			   GFP_KERNEL);
	if (!hostdata) {
		dev_err(&pdev->dev, "Failed to allocate host data\n");
		goto out_release;
	}

	scsi_addr = res->start + A4000T_SCSI_OFFSET;

	/* Fill in the required pieces of hostdata */
	hostdata->base = (void __iomem *)ZTWO_VADDR(scsi_addr);
	hostdata->clock = 50;
	hostdata->chip710 = 1;
	hostdata->dmode_extra = DMODE_FC2;
	hostdata->dcntl_extra = EA_710;

	/* and register the chip */
	host = NCR_700_detect(&a4000t_scsi_driver_template, hostdata,
			      &pdev->dev);
	if (!host) {
		dev_err(&pdev->dev,
			"No host detected; board configuration problem?\n");
		goto out_free;
	}

	host->this_id = 7;
	host->base = scsi_addr;
	host->irq = IRQ_AMIGA_PORTS;

	if (request_irq(host->irq, NCR_700_intr, IRQF_SHARED, "a4000t-scsi",
			host)) {
		dev_err(&pdev->dev, "request_irq failed\n");
		goto out_put_host;
	}

	platform_set_drvdata(pdev, host);
	scsi_scan_host(host);
	return 0;

 out_put_host:
	scsi_host_put(host);
 out_free:
	kfree(hostdata);
 out_release:
	release_mem_region(res->start, resource_size(res));
	return -ENODEV;
}

static int __exit amiga_a4000t_scsi_remove(struct platform_device *pdev)
{
	struct Scsi_Host *host = platform_get_drvdata(pdev);
	struct NCR_700_Host_Parameters *hostdata = shost_priv(host);
	struct resource *res = platform_get_resource(pdev, IORESOURCE_MEM, 0);

	scsi_remove_host(host);
	NCR_700_release(host);
	kfree(hostdata);
	free_irq(host->irq, host);
	release_mem_region(res->start, resource_size(res));
	return 0;
}

static struct platform_driver amiga_a4000t_scsi_driver = {
	.remove = __exit_p(amiga_a4000t_scsi_remove),
	.driver   = {
		.name	= "amiga-a4000t-scsi",
		.owner	= THIS_MODULE,
	},
};

static int __init amiga_a4000t_scsi_init(void)
{
	return platform_driver_probe(&amiga_a4000t_scsi_driver,
				     amiga_a4000t_scsi_probe);
}

module_init(amiga_a4000t_scsi_init);

static void __exit amiga_a4000t_scsi_exit(void)
{
	platform_driver_unregister(&amiga_a4000t_scsi_driver);
}

module_exit(amiga_a4000t_scsi_exit);

MODULE_AUTHOR("Alan Hourihane <alanh@fairlite.demon.co.uk> / "
	      "Kars de Jong <jongk@linux-m68k.org>");
MODULE_DESCRIPTION("Amiga A4000T NCR53C710 driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:amiga-a4000t-scsi");
