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
 * Basic KB3310B Embedded Controller support for the YeeLoong 2F netbook
 *
 *  Copyright (C) 2008 Lemote Inc.
 *  Author: liujl <liujl@lemote.com>, 2008-04-20
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <linux/module.h>
#include <linux/spinlock.h>
#include <linux/delay.h>

#include "ec_kb3310b.h"

static DEFINE_SPINLOCK(index_access_lock);
static DEFINE_SPINLOCK(port_access_lock);

unsigned char ec_read(unsigned short addr)
{
	unsigned char value;
	unsigned long flags;

	spin_lock_irqsave(&index_access_lock, flags);
	outb((addr & 0xff00) >> 8, EC_IO_PORT_HIGH);
	outb((addr & 0x00ff), EC_IO_PORT_LOW);
	value = inb(EC_IO_PORT_DATA);
	spin_unlock_irqrestore(&index_access_lock, flags);

	return value;
}
EXPORT_SYMBOL_GPL(ec_read);

void ec_write(unsigned short addr, unsigned char val)
{
	unsigned long flags;

	spin_lock_irqsave(&index_access_lock, flags);
	outb((addr & 0xff00) >> 8, EC_IO_PORT_HIGH);
	outb((addr & 0x00ff), EC_IO_PORT_LOW);
	outb(val, EC_IO_PORT_DATA);
	/*  flush the write action */
	inb(EC_IO_PORT_DATA);
	spin_unlock_irqrestore(&index_access_lock, flags);

	return;
}
EXPORT_SYMBOL_GPL(ec_write);

/*
 * This function is used for EC command writes and corresponding status queries.
 */
int ec_query_seq(unsigned char cmd)
{
	int timeout;
	unsigned char status;
	unsigned long flags;
	int ret = 0;

	spin_lock_irqsave(&port_access_lock, flags);

	/* make chip goto reset mode */
	udelay(EC_REG_DELAY);
	outb(cmd, EC_CMD_PORT);
	udelay(EC_REG_DELAY);

	/* check if the command is received by ec */
	timeout = EC_CMD_TIMEOUT;
	status = inb(EC_STS_PORT);
	while (timeout-- && (status & (1 << 1))) {
		status = inb(EC_STS_PORT);
		udelay(EC_REG_DELAY);
	}

	spin_unlock_irqrestore(&port_access_lock, flags);

	if (timeout <= 0) {
		printk(KERN_ERR "%s: deadable error : timeout...\n", __func__);
		ret = -EINVAL;
	} else
		printk(KERN_INFO
			   "(%x/%d)ec issued command %d status : 0x%x\n",
			   timeout, EC_CMD_TIMEOUT - timeout, cmd, status);

	return ret;
}
EXPORT_SYMBOL_GPL(ec_query_seq);

/*
 * Send query command to EC to get the proper event number
 */
int ec_query_event_num(void)
{
	return ec_query_seq(CMD_GET_EVENT_NUM);
}
EXPORT_SYMBOL(ec_query_event_num);

/*
 * Get event number from EC
 *
 * NOTE: This routine must follow the query_event_num function in the
 * interrupt.
 */
int ec_get_event_num(void)
{
	int timeout = 100;
	unsigned char value;
	unsigned char status;

	udelay(EC_REG_DELAY);
	status = inb(EC_STS_PORT);
	udelay(EC_REG_DELAY);
	while (timeout-- && !(status & (1 << 0))) {
		status = inb(EC_STS_PORT);
		udelay(EC_REG_DELAY);
	}
	if (timeout <= 0) {
		pr_info("%s: get event number timeout.\n", __func__);

		return -EINVAL;
	}
	value = inb(EC_DAT_PORT);
	udelay(EC_REG_DELAY);

	return value;
}
EXPORT_SYMBOL(ec_get_event_num);
