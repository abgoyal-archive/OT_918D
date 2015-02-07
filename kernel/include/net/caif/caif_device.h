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
 * Copyright (C) ST-Ericsson AB 2010
 * Author:	Sjur Brendeland/ sjur.brandeland@stericsson.com
 * License terms: GNU General Public License (GPL) version 2
 */

#ifndef CAIF_DEVICE_H_
#define CAIF_DEVICE_H_
#include <linux/kernel.h>
#include <linux/net.h>
#include <linux/netdevice.h>
#include <linux/caif/caif_socket.h>
#include <net/caif/caif_device.h>

/**
 * struct caif_dev_common - data shared between CAIF drivers and stack.
 * @flowctrl:		Flow Control callback function. This function is
 *                      supplied by CAIF Core Stack and is used by CAIF
 *                      Link Layer to send flow-stop to CAIF Core.
 *                      The flow information will be distributed to all
 *                      clients of CAIF.
 *
 * @link_select:	Profile of device, either high-bandwidth or
 *			low-latency. This member is set by CAIF Link
 *			Layer Device in	order to indicate if this device
 *			is a high bandwidth or low latency device.
 *
 * @use_frag:		CAIF Frames may be fragmented.
 *			Is set by CAIF Link Layer in order to indicate if the
 *			interface receives fragmented frames that must be
 *			assembled by CAIF Core Layer.
 *
 * @use_fcs:		Indicate if Frame CheckSum (fcs) is used.
 *			Is set if the physical interface is
 *			using Frame Checksum on the CAIF Frames.
 *
 * @use_stx:		Indicate STart of frame eXtension (stx) in use.
 *			Is set if the CAIF Link Layer expects
 *			CAIF Frames to start with the STX byte.
 *
 * This structure is shared between the CAIF drivers and the CAIF stack.
 * It is used by the device to register its behavior.
 * CAIF Core layer must set the member flowctrl in order to supply
 * CAIF Link Layer with the flow control function.
 *
 */
 struct caif_dev_common {
	void (*flowctrl)(struct net_device *net, int on);
	enum caif_link_selector link_select;
	int use_frag;
	int use_fcs;
	int use_stx;
};

#endif	/* CAIF_DEVICE_H_ */
