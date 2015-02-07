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
 * PCMCIA driver for SL811HS (as found in REX-CFU1U)
 * Filename: sl811_cs.c
 * Author:   Yukio Yamamoto
 *
 *  Port to sl811-hcd and 2.6.x by
 *    Botond Botyanszki <boti@rocketmail.com>
 *    Simon Pickering
 *
 *  Last update: 2005-05-12
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/ptrace.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/timer.h>
#include <linux/ioport.h>
#include <linux/platform_device.h>

#include <pcmcia/cs_types.h>
#include <pcmcia/cs.h>
#include <pcmcia/cistpl.h>
#include <pcmcia/cisreg.h>
#include <pcmcia/ds.h>

#include <linux/usb/sl811.h>

MODULE_AUTHOR("Botond Botyanszki");
MODULE_DESCRIPTION("REX-CFU1U PCMCIA driver for 2.6");
MODULE_LICENSE("GPL");


/*====================================================================*/
/* MACROS                                                             */
/*====================================================================*/

#define INFO(args...) printk(KERN_INFO "sl811_cs: " args)

/*====================================================================*/
/* VARIABLES                                                          */
/*====================================================================*/

static const char driver_name[DEV_NAME_LEN]  = "sl811_cs";

typedef struct local_info_t {
	struct pcmcia_device	*p_dev;
} local_info_t;

static void sl811_cs_release(struct pcmcia_device * link);

/*====================================================================*/

static void release_platform_dev(struct device * dev)
{
	dev_dbg(dev, "sl811_cs platform_dev release\n");
	dev->parent = NULL;
}

static struct sl811_platform_data platform_data = {
	.potpg		= 100,
	.power		= 50,		/* == 100mA */
	// .reset	= ... FIXME:  invoke CF reset on the card
};

static struct resource resources[] = {
	[0] = {
		.flags	= IORESOURCE_IRQ,
	},
	[1] = {
		// .name   = "address",
		.flags	= IORESOURCE_IO,
	},
	[2] = {
		// .name   = "data",
		.flags	= IORESOURCE_IO,
	},
};

extern struct platform_driver sl811h_driver;

static struct platform_device platform_dev = {
	.id			= -1,
	.dev = {
		.platform_data = &platform_data,
		.release       = release_platform_dev,
	},
	.resource		= resources,
	.num_resources		= ARRAY_SIZE(resources),
};

static int sl811_hc_init(struct device *parent, resource_size_t base_addr,
			 int irq)
{
	if (platform_dev.dev.parent)
		return -EBUSY;
	platform_dev.dev.parent = parent;

	/* finish seting up the platform device */
	resources[0].start = irq;

	resources[1].start = base_addr;
	resources[1].end = base_addr;

	resources[2].start = base_addr + 1;
	resources[2].end   = base_addr + 1;

	/* The driver core will probe for us.  We know sl811-hcd has been
	 * initialized already because of the link order dependency created
	 * by referencing "sl811h_driver".
	 */
	platform_dev.name = sl811h_driver.driver.name;
	return platform_device_register(&platform_dev);
}

/*====================================================================*/

static void sl811_cs_detach(struct pcmcia_device *link)
{
	dev_dbg(&link->dev, "sl811_cs_detach\n");

	sl811_cs_release(link);

	/* This points to the parent local_info_t struct */
	kfree(link->priv);
}

static void sl811_cs_release(struct pcmcia_device * link)
{
	dev_dbg(&link->dev, "sl811_cs_release\n");

	pcmcia_disable_device(link);
	platform_device_unregister(&platform_dev);
}

static int sl811_cs_config_check(struct pcmcia_device *p_dev,
				 cistpl_cftable_entry_t *cfg,
				 cistpl_cftable_entry_t *dflt,
				 unsigned int vcc,
				 void *priv_data)
{
	if (cfg->index == 0)
		return -ENODEV;

	/* Use power settings for Vcc and Vpp if present */
	/*  Note that the CIS values need to be rescaled */
	if (cfg->vcc.present & (1<<CISTPL_POWER_VNOM)) {
		if (cfg->vcc.param[CISTPL_POWER_VNOM]/10000 != vcc)
			return -ENODEV;
	} else if (dflt->vcc.present & (1<<CISTPL_POWER_VNOM)) {
		if (dflt->vcc.param[CISTPL_POWER_VNOM]/10000 != vcc)
			return -ENODEV;
		}

	if (cfg->vpp1.present & (1<<CISTPL_POWER_VNOM))
		p_dev->conf.Vpp =
			cfg->vpp1.param[CISTPL_POWER_VNOM]/10000;
	else if (dflt->vpp1.present & (1<<CISTPL_POWER_VNOM))
		p_dev->conf.Vpp =
			dflt->vpp1.param[CISTPL_POWER_VNOM]/10000;

	/* we need an interrupt */
	p_dev->conf.Attributes |= CONF_ENABLE_IRQ;

	/* IO window settings */
	p_dev->io.NumPorts1 = p_dev->io.NumPorts2 = 0;
	if ((cfg->io.nwin > 0) || (dflt->io.nwin > 0)) {
		cistpl_io_t *io = (cfg->io.nwin) ? &cfg->io : &dflt->io;

		p_dev->io.Attributes1 = IO_DATA_PATH_WIDTH_8;
		p_dev->io.IOAddrLines = io->flags & CISTPL_IO_LINES_MASK;
		p_dev->io.BasePort1 = io->win[0].base;
		p_dev->io.NumPorts1 = io->win[0].len;

		return pcmcia_request_io(p_dev, &p_dev->io);
	}
	pcmcia_disable_device(p_dev);
	return -ENODEV;
}


static int sl811_cs_config(struct pcmcia_device *link)
{
	struct device		*parent = &link->dev;
	int			ret;

	dev_dbg(&link->dev, "sl811_cs_config\n");

	if (pcmcia_loop_config(link, sl811_cs_config_check, NULL))
		goto failed;

	/* require an IRQ and two registers */
	if (!link->io.NumPorts1 || link->io.NumPorts1 < 2)
		goto failed;

	if (!link->irq)
		goto failed;

	ret = pcmcia_request_configuration(link, &link->conf);
	if (ret)
		goto failed;

	dev_info(&link->dev, "index 0x%02x: ",
		link->conf.ConfigIndex);
	if (link->conf.Vpp)
		printk(", Vpp %d.%d", link->conf.Vpp/10, link->conf.Vpp%10);
	printk(", irq %d", link->irq);
	printk(", io 0x%04x-0x%04x", link->io.BasePort1,
	       link->io.BasePort1+link->io.NumPorts1-1);
	printk("\n");

	if (sl811_hc_init(parent, link->io.BasePort1, link->irq)
			< 0) {
failed:
		printk(KERN_WARNING "sl811_cs_config failed\n");
		sl811_cs_release(link);
		return  -ENODEV;
	}
	return 0;
}

static int sl811_cs_probe(struct pcmcia_device *link)
{
	local_info_t *local;

	local = kzalloc(sizeof(local_info_t), GFP_KERNEL);
	if (!local)
		return -ENOMEM;
	local->p_dev = link;
	link->priv = local;

	link->conf.Attributes = 0;
	link->conf.IntType = INT_MEMORY_AND_IO;

	return sl811_cs_config(link);
}

static struct pcmcia_device_id sl811_ids[] = {
	PCMCIA_DEVICE_MANF_CARD(0xc015, 0x0001), /* RATOC USB HOST CF+ Card */
	PCMCIA_DEVICE_NULL,
};
MODULE_DEVICE_TABLE(pcmcia, sl811_ids);

static struct pcmcia_driver sl811_cs_driver = {
	.owner		= THIS_MODULE,
	.drv		= {
		.name	= (char *)driver_name,
	},
	.probe		= sl811_cs_probe,
	.remove		= sl811_cs_detach,
	.id_table	= sl811_ids,
};

/*====================================================================*/

static int __init init_sl811_cs(void)
{
	return pcmcia_register_driver(&sl811_cs_driver);
}
module_init(init_sl811_cs);

static void __exit exit_sl811_cs(void)
{
	pcmcia_unregister_driver(&sl811_cs_driver);
}
module_exit(exit_sl811_cs);
