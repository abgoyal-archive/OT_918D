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
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2009 Cavium Networks
 */

#include <linux/gfp.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/phy.h>

#include <asm/octeon/octeon.h>
#include <asm/octeon/cvmx-smix-defs.h>

#define DRV_VERSION "1.0"
#define DRV_DESCRIPTION "Cavium Networks Octeon SMI/MDIO driver"

struct octeon_mdiobus {
	struct mii_bus *mii_bus;
	int unit;
	int phy_irq[PHY_MAX_ADDR];
};

static int octeon_mdiobus_read(struct mii_bus *bus, int phy_id, int regnum)
{
	struct octeon_mdiobus *p = bus->priv;
	union cvmx_smix_cmd smi_cmd;
	union cvmx_smix_rd_dat smi_rd;
	int timeout = 1000;

	smi_cmd.u64 = 0;
	smi_cmd.s.phy_op = 1; /* MDIO_CLAUSE_22_READ */
	smi_cmd.s.phy_adr = phy_id;
	smi_cmd.s.reg_adr = regnum;
	cvmx_write_csr(CVMX_SMIX_CMD(p->unit), smi_cmd.u64);

	do {
		/*
		 * Wait 1000 clocks so we don't saturate the RSL bus
		 * doing reads.
		 */
		cvmx_wait(1000);
		smi_rd.u64 = cvmx_read_csr(CVMX_SMIX_RD_DAT(p->unit));
	} while (smi_rd.s.pending && --timeout);

	if (smi_rd.s.val)
		return smi_rd.s.dat;
	else
		return -EIO;
}

static int octeon_mdiobus_write(struct mii_bus *bus, int phy_id,
				int regnum, u16 val)
{
	struct octeon_mdiobus *p = bus->priv;
	union cvmx_smix_cmd smi_cmd;
	union cvmx_smix_wr_dat smi_wr;
	int timeout = 1000;

	smi_wr.u64 = 0;
	smi_wr.s.dat = val;
	cvmx_write_csr(CVMX_SMIX_WR_DAT(p->unit), smi_wr.u64);

	smi_cmd.u64 = 0;
	smi_cmd.s.phy_op = 0; /* MDIO_CLAUSE_22_WRITE */
	smi_cmd.s.phy_adr = phy_id;
	smi_cmd.s.reg_adr = regnum;
	cvmx_write_csr(CVMX_SMIX_CMD(p->unit), smi_cmd.u64);

	do {
		/*
		 * Wait 1000 clocks so we don't saturate the RSL bus
		 * doing reads.
		 */
		cvmx_wait(1000);
		smi_wr.u64 = cvmx_read_csr(CVMX_SMIX_WR_DAT(p->unit));
	} while (smi_wr.s.pending && --timeout);

	if (timeout <= 0)
		return -EIO;

	return 0;
}

static int __init octeon_mdiobus_probe(struct platform_device *pdev)
{
	struct octeon_mdiobus *bus;
	union cvmx_smix_en smi_en;
	int i;
	int err = -ENOENT;

	bus = devm_kzalloc(&pdev->dev, sizeof(*bus), GFP_KERNEL);
	if (!bus)
		return -ENOMEM;

	/* The platform_device id is our unit number.  */
	bus->unit = pdev->id;

	bus->mii_bus = mdiobus_alloc();

	if (!bus->mii_bus)
		goto err;

	smi_en.u64 = 0;
	smi_en.s.en = 1;
	cvmx_write_csr(CVMX_SMIX_EN(bus->unit), smi_en.u64);

	/*
	 * Standard Octeon evaluation boards don't support phy
	 * interrupts, we need to poll.
	 */
	for (i = 0; i < PHY_MAX_ADDR; i++)
		bus->phy_irq[i] = PHY_POLL;

	bus->mii_bus->priv = bus;
	bus->mii_bus->irq = bus->phy_irq;
	bus->mii_bus->name = "mdio-octeon";
	snprintf(bus->mii_bus->id, MII_BUS_ID_SIZE, "%x", bus->unit);
	bus->mii_bus->parent = &pdev->dev;

	bus->mii_bus->read = octeon_mdiobus_read;
	bus->mii_bus->write = octeon_mdiobus_write;

	dev_set_drvdata(&pdev->dev, bus);

	err = mdiobus_register(bus->mii_bus);
	if (err)
		goto err_register;

	dev_info(&pdev->dev, "Version " DRV_VERSION "\n");

	return 0;
err_register:
	mdiobus_free(bus->mii_bus);

err:
	devm_kfree(&pdev->dev, bus);
	smi_en.u64 = 0;
	cvmx_write_csr(CVMX_SMIX_EN(bus->unit), smi_en.u64);
	return err;
}

static int __exit octeon_mdiobus_remove(struct platform_device *pdev)
{
	struct octeon_mdiobus *bus;
	union cvmx_smix_en smi_en;

	bus = dev_get_drvdata(&pdev->dev);

	mdiobus_unregister(bus->mii_bus);
	mdiobus_free(bus->mii_bus);
	smi_en.u64 = 0;
	cvmx_write_csr(CVMX_SMIX_EN(bus->unit), smi_en.u64);
	return 0;
}

static struct platform_driver octeon_mdiobus_driver = {
	.driver = {
		.name		= "mdio-octeon",
		.owner		= THIS_MODULE,
	},
	.probe		= octeon_mdiobus_probe,
	.remove		= __exit_p(octeon_mdiobus_remove),
};

void octeon_mdiobus_force_mod_depencency(void)
{
	/* Let ethernet drivers force us to be loaded.  */
}
EXPORT_SYMBOL(octeon_mdiobus_force_mod_depencency);

static int __init octeon_mdiobus_mod_init(void)
{
	return platform_driver_register(&octeon_mdiobus_driver);
}

static void __exit octeon_mdiobus_mod_exit(void)
{
	platform_driver_unregister(&octeon_mdiobus_driver);
}

module_init(octeon_mdiobus_mod_init);
module_exit(octeon_mdiobus_mod_exit);

MODULE_DESCRIPTION(DRV_DESCRIPTION);
MODULE_VERSION(DRV_VERSION);
MODULE_AUTHOR("David Daney");
MODULE_LICENSE("GPL");
