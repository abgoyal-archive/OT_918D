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
 * RapidIO Tsi568 switch support
 *
 * Copyright 2009-2010 Integrated Device Technology, Inc.
 * Alexandre Bounine <alexandre.bounine@idt.com>
 *  - Added EM support
 *  - Modified switch operations initialization.
 *
 * Copyright 2005 MontaVista Software, Inc.
 * Matt Porter <mporter@kernel.crashing.org>
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */

#include <linux/rio.h>
#include <linux/rio_drv.h>
#include <linux/rio_ids.h>
#include <linux/delay.h>
#include "../rio.h"

/* Global (broadcast) route registers */
#define SPBC_ROUTE_CFG_DESTID	0x10070
#define SPBC_ROUTE_CFG_PORT	0x10074

/* Per port route registers */
#define SPP_ROUTE_CFG_DESTID(n)	(0x11070 + 0x100*n)
#define SPP_ROUTE_CFG_PORT(n)	(0x11074 + 0x100*n)

#define TSI568_SP_MODE_BC	0x10004
#define  TSI568_SP_MODE_PW_DIS	0x08000000

static int
tsi568_route_add_entry(struct rio_mport *mport, u16 destid, u8 hopcount,
		       u16 table, u16 route_destid, u8 route_port)
{
	if (table == RIO_GLOBAL_TABLE) {
		rio_mport_write_config_32(mport, destid, hopcount,
					SPBC_ROUTE_CFG_DESTID, route_destid);
		rio_mport_write_config_32(mport, destid, hopcount,
					SPBC_ROUTE_CFG_PORT, route_port);
	} else {
		rio_mport_write_config_32(mport, destid, hopcount,
					SPP_ROUTE_CFG_DESTID(table),
					route_destid);
		rio_mport_write_config_32(mport, destid, hopcount,
					SPP_ROUTE_CFG_PORT(table), route_port);
	}

	udelay(10);

	return 0;
}

static int
tsi568_route_get_entry(struct rio_mport *mport, u16 destid, u8 hopcount,
		       u16 table, u16 route_destid, u8 *route_port)
{
	int ret = 0;
	u32 result;

	if (table == RIO_GLOBAL_TABLE) {
		rio_mport_write_config_32(mport, destid, hopcount,
					SPBC_ROUTE_CFG_DESTID, route_destid);
		rio_mport_read_config_32(mport, destid, hopcount,
					SPBC_ROUTE_CFG_PORT, &result);
	} else {
		rio_mport_write_config_32(mport, destid, hopcount,
					SPP_ROUTE_CFG_DESTID(table),
					route_destid);
		rio_mport_read_config_32(mport, destid, hopcount,
					SPP_ROUTE_CFG_PORT(table), &result);
	}

	*route_port = result;
	if (*route_port > 15)
		ret = -1;

	return ret;
}

static int
tsi568_route_clr_table(struct rio_mport *mport, u16 destid, u8 hopcount,
		       u16 table)
{
	u32 route_idx;
	u32 lut_size;

	lut_size = (mport->sys_size) ? 0x1ff : 0xff;

	if (table == RIO_GLOBAL_TABLE) {
		rio_mport_write_config_32(mport, destid, hopcount,
					SPBC_ROUTE_CFG_DESTID, 0x80000000);
		for (route_idx = 0; route_idx <= lut_size; route_idx++)
			rio_mport_write_config_32(mport, destid, hopcount,
						SPBC_ROUTE_CFG_PORT,
						RIO_INVALID_ROUTE);
	} else {
		rio_mport_write_config_32(mport, destid, hopcount,
					SPP_ROUTE_CFG_DESTID(table),
					0x80000000);
		for (route_idx = 0; route_idx <= lut_size; route_idx++)
			rio_mport_write_config_32(mport, destid, hopcount,
						SPP_ROUTE_CFG_PORT(table),
						RIO_INVALID_ROUTE);
	}

	return 0;
}

static int
tsi568_em_init(struct rio_dev *rdev)
{
	struct rio_mport *mport = rdev->net->hport;
	u16 destid = rdev->rswitch->destid;
	u8 hopcount = rdev->rswitch->hopcount;
	u32 regval;

	pr_debug("TSI568 %s [%d:%d]\n", __func__, destid, hopcount);

	/* Make sure that Port-Writes are disabled (for all ports) */
	rio_mport_read_config_32(mport, destid, hopcount,
			TSI568_SP_MODE_BC, &regval);
	rio_mport_write_config_32(mport, destid, hopcount,
			TSI568_SP_MODE_BC, regval | TSI568_SP_MODE_PW_DIS);

	return 0;
}

static int tsi568_switch_init(struct rio_dev *rdev, int do_enum)
{
	pr_debug("RIO: %s for %s\n", __func__, rio_name(rdev));
	rdev->rswitch->add_entry = tsi568_route_add_entry;
	rdev->rswitch->get_entry = tsi568_route_get_entry;
	rdev->rswitch->clr_table = tsi568_route_clr_table;
	rdev->rswitch->set_domain = NULL;
	rdev->rswitch->get_domain = NULL;
	rdev->rswitch->em_init = tsi568_em_init;
	rdev->rswitch->em_handle = NULL;

	return 0;
}

DECLARE_RIO_SWITCH_INIT(RIO_VID_TUNDRA, RIO_DID_TSI568, tsi568_switch_init);
