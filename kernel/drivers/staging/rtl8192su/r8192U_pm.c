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
   Power management interface routines.
   Written by Mariusz Matuszek.
   This code is currently just a placeholder for later work and
   does not do anything useful.

   This is part of rtl8180 OpenSource driver.
   Copyright (C) Andrea Merello 2004  <andreamrl@tiscali.it>
   Released under the terms of GPL (General Public Licence)
*/

#include "r8192U.h"
#include "r8192U_pm.h"

/*****************************************************************************/
int rtl8192U_save_state (struct pci_dev *dev, u32 state)
{
	printk(KERN_NOTICE "r8192U save state call (state %u).\n", state);
	return(-EAGAIN);
}

int rtl8192U_suspend(struct usb_interface *intf, pm_message_t state)
{
	struct net_device *dev = usb_get_intfdata(intf);

	RT_TRACE(COMP_POWER, "============> r8192U suspend call.\n");

	if(dev) {
		 if (!netif_running(dev)) {
		      printk(KERN_WARNING "netif not running, go out suspend function\n");
		      return 0;
		 }

		if (dev->netdev_ops->ndo_stop)
			dev->netdev_ops->ndo_stop(dev);

		mdelay(10);

		netif_device_detach(dev);
	}

	return 0;
}

int rtl8192U_resume (struct usb_interface *intf)
{
	struct net_device *dev = usb_get_intfdata(intf);

	RT_TRACE(COMP_POWER, "================>r8192U resume call.");

	if(dev) {
		if (!netif_running(dev)){
			printk(KERN_WARNING "netif not running, go out resume function\n");
			return 0;
		}

		netif_device_attach(dev);

		if (dev->netdev_ops->ndo_open)
			dev->netdev_ops->ndo_open(dev);
	}

        return 0;
}

int rtl8192U_enable_wake (struct pci_dev *dev, u32 state, int enable)
{
	printk(KERN_NOTICE "r8192U enable wake call (state %u, enable %d).\n",
			state, enable);
	return(-EAGAIN);
}

