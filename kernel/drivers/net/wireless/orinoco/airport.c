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

/* airport.c
 *
 * A driver for "Hermes" chipset based Apple Airport wireless
 * card.
 *
 * Copyright notice & release notes in file main.c
 *
 * Note specific to airport stub:
 *
 *  0.05 : first version of the new split driver
 *  0.06 : fix possible hang on powerup, add sleep support
 */

#define DRIVER_NAME "airport"
#define PFX DRIVER_NAME ": "

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <asm/pmac_feature.h>

#include "orinoco.h"

#define AIRPORT_IO_LEN	(0x1000)	/* one page */

struct airport {
	struct macio_dev *mdev;
	void __iomem *vaddr;
	unsigned int irq;
	int irq_requested;
	int ndev_registered;
};

static int
airport_suspend(struct macio_dev *mdev, pm_message_t state)
{
	struct orinoco_private *priv = dev_get_drvdata(&mdev->ofdev.dev);
	struct net_device *dev = priv->ndev;
	struct airport *card = priv->card;
	unsigned long flags;
	int err;

	printk(KERN_DEBUG "%s: Airport entering sleep mode\n", dev->name);

	err = orinoco_lock(priv, &flags);
	if (err) {
		printk(KERN_ERR "%s: hw_unavailable on PBOOK_SLEEP_NOW\n",
		       dev->name);
		return 0;
	}

	orinoco_down(priv);
	orinoco_unlock(priv, &flags);

	disable_irq(card->irq);
	pmac_call_feature(PMAC_FTR_AIRPORT_ENABLE,
			  macio_get_of_node(mdev), 0, 0);

	return 0;
}

static int
airport_resume(struct macio_dev *mdev)
{
	struct orinoco_private *priv = dev_get_drvdata(&mdev->ofdev.dev);
	struct net_device *dev = priv->ndev;
	struct airport *card = priv->card;
	unsigned long flags;
	int err;

	printk(KERN_DEBUG "%s: Airport waking up\n", dev->name);

	pmac_call_feature(PMAC_FTR_AIRPORT_ENABLE,
			  macio_get_of_node(mdev), 0, 1);
	msleep(200);

	enable_irq(card->irq);

	priv->hw.ops->lock_irqsave(&priv->lock, &flags);
	err = orinoco_up(priv);
	priv->hw.ops->unlock_irqrestore(&priv->lock, &flags);

	return err;
}

static int
airport_detach(struct macio_dev *mdev)
{
	struct orinoco_private *priv = dev_get_drvdata(&mdev->ofdev.dev);
	struct airport *card = priv->card;

	if (card->ndev_registered)
		orinoco_if_del(priv);
	card->ndev_registered = 0;

	if (card->irq_requested)
		free_irq(card->irq, priv);
	card->irq_requested = 0;

	if (card->vaddr)
		iounmap(card->vaddr);
	card->vaddr = NULL;

	macio_release_resource(mdev, 0);

	pmac_call_feature(PMAC_FTR_AIRPORT_ENABLE,
			  macio_get_of_node(mdev), 0, 0);
	ssleep(1);

	macio_set_drvdata(mdev, NULL);
	free_orinocodev(priv);

	return 0;
}

static int airport_hard_reset(struct orinoco_private *priv)
{
	/* It would be nice to power cycle the Airport for a real hard
	 * reset, but for some reason although it appears to
	 * re-initialize properly, it falls in a screaming heap
	 * shortly afterwards. */
#if 0
	struct airport *card = priv->card;

	/* Vitally important.  If we don't do this it seems we get an
	 * interrupt somewhere during the power cycle, since
	 * hw_unavailable is already set it doesn't get ACKed, we get
	 * into an interrupt loop and the PMU decides to turn us
	 * off. */
	disable_irq(card->irq);

	pmac_call_feature(PMAC_FTR_AIRPORT_ENABLE,
			  macio_get_of_node(card->mdev), 0, 0);
	ssleep(1);
	pmac_call_feature(PMAC_FTR_AIRPORT_ENABLE,
			  macio_get_of_node(card->mdev), 0, 1);
	ssleep(1);

	enable_irq(card->irq);
	ssleep(1);
#endif

	return 0;
}

static int
airport_attach(struct macio_dev *mdev, const struct of_device_id *match)
{
	struct orinoco_private *priv;
	struct airport *card;
	unsigned long phys_addr;
	hermes_t *hw;

	if (macio_resource_count(mdev) < 1 || macio_irq_count(mdev) < 1) {
		printk(KERN_ERR PFX "Wrong interrupt/addresses in OF tree\n");
		return -ENODEV;
	}

	/* Allocate space for private device-specific data */
	priv = alloc_orinocodev(sizeof(*card), &mdev->ofdev.dev,
				airport_hard_reset, NULL);
	if (!priv) {
		printk(KERN_ERR PFX "Cannot allocate network device\n");
		return -ENODEV;
	}
	card = priv->card;

	hw = &priv->hw;
	card->mdev = mdev;

	if (macio_request_resource(mdev, 0, DRIVER_NAME)) {
		printk(KERN_ERR PFX "can't request IO resource !\n");
		free_orinocodev(priv);
		return -EBUSY;
	}

	macio_set_drvdata(mdev, priv);

	/* Setup interrupts & base address */
	card->irq = macio_irq(mdev, 0);
	phys_addr = macio_resource_start(mdev, 0);  /* Physical address */
	printk(KERN_DEBUG PFX "Physical address %lx\n", phys_addr);
	card->vaddr = ioremap(phys_addr, AIRPORT_IO_LEN);
	if (!card->vaddr) {
		printk(KERN_ERR PFX "ioremap() failed\n");
		goto failed;
	}

	hermes_struct_init(hw, card->vaddr, HERMES_16BIT_REGSPACING);

	/* Power up card */
	pmac_call_feature(PMAC_FTR_AIRPORT_ENABLE,
			  macio_get_of_node(mdev), 0, 1);
	ssleep(1);

	/* Reset it before we get the interrupt */
	hw->ops->init(hw);

	if (request_irq(card->irq, orinoco_interrupt, 0, DRIVER_NAME, priv)) {
		printk(KERN_ERR PFX "Couldn't get IRQ %d\n", card->irq);
		goto failed;
	}
	card->irq_requested = 1;

	/* Initialise the main driver */
	if (orinoco_init(priv) != 0) {
		printk(KERN_ERR PFX "orinoco_init() failed\n");
		goto failed;
	}

	/* Register an interface with the stack */
	if (orinoco_if_add(priv, phys_addr, card->irq, NULL) != 0) {
		printk(KERN_ERR PFX "orinoco_if_add() failed\n");
		goto failed;
	}
	card->ndev_registered = 1;
	return 0;
 failed:
	airport_detach(mdev);
	return -ENODEV;
}				/* airport_attach */


static char version[] __initdata = DRIVER_NAME " " DRIVER_VERSION
	" (Benjamin Herrenschmidt <benh@kernel.crashing.org>)";
MODULE_AUTHOR("Benjamin Herrenschmidt <benh@kernel.crashing.org>");
MODULE_DESCRIPTION("Driver for the Apple Airport wireless card.");
MODULE_LICENSE("Dual MPL/GPL");

static struct of_device_id airport_match[] =
{
	{
	.name 		= "radio",
	},
	{},
};

MODULE_DEVICE_TABLE(of, airport_match);

static struct macio_driver airport_driver = {
	.driver = {
		.name 		= DRIVER_NAME,
		.owner		= THIS_MODULE,
		.of_match_table	= airport_match,
	},
	.probe		= airport_attach,
	.remove		= airport_detach,
	.suspend	= airport_suspend,
	.resume		= airport_resume,
};

static int __init
init_airport(void)
{
	printk(KERN_DEBUG "%s\n", version);

	return macio_register_driver(&airport_driver);
}

static void __exit
exit_airport(void)
{
	macio_unregister_driver(&airport_driver);
}

module_init(init_airport);
module_exit(exit_airport);
