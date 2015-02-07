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
 * Copyright (C) 2007 by Analog Devices, Inc.
 *
 * The Inventra Controller Driver for Linux is free software; you
 * can redistribute it and/or modify it under the terms of the GNU
 * General Public License version 2 as published by the Free Software
 * Foundation.
 */

#ifndef __MUSB_BLACKFIN_H__
#define __MUSB_BLACKFIN_H__

/*
 * Blackfin specific definitions
 */

/* Anomalies notes:
 *
 *  05000450 - USB DMA Mode 1 Short Packet Data Corruption:
 *             MUSB driver is designed to transfer buffer of N * maxpacket size
 *             in DMA mode 1 and leave the rest of the data to the next
 *             transfer in DMA mode 0, so we never transmit a short packet in
 *             DMA mode 1.
 *
 *  05000463 - This anomaly doesn't affect this driver since it
 *             never uses L1 or L2 memory as data destination.
 *
 *  05000464 - This anomaly doesn't affect this driver since it
 *             never uses L1 or L2 memory as data source.
 *
 *  05000465 - The anomaly can be seen when SCLK is over 100 MHz, and there is
 *             no way to workaround for bulk endpoints.  Since the wMaxPackSize
 *             of bulk is less than or equal to 512, while the fifo size of
 *             endpoint 5, 6, 7 is 1024, the double buffer mode is enabled
 *             automatically when these endpoints are used for bulk OUT.
 *
 *  05000466 - This anomaly doesn't affect this driver since it never mixes
 *             concurrent DMA and core accesses to the TX endpoint FIFOs.
 *
 *  05000467 - The workaround for this anomaly will introduce another
 *             anomaly - 05000465.
 */

/* The Mentor USB DMA engine on BF52x (silicon v0.0 and v0.1) seems to be
 * unstable in host mode.  This may be caused by Anomaly 05000380.  After
 * digging out the root cause, we will change this number accordingly.
 * So, need to either use silicon v0.2+ or disable DMA mode in MUSB.
 */
#if ANOMALY_05000380 && defined(CONFIG_BF52x) && \
    defined(CONFIG_USB_MUSB_HDRC) && !defined(CONFIG_MUSB_PIO_ONLY)
# error "Please use PIO mode in MUSB driver on bf52x chip v0.0 and v0.1"
#endif

#undef DUMP_FIFO_DATA
#ifdef DUMP_FIFO_DATA
static void dump_fifo_data(u8 *buf, u16 len)
{
	u8 *tmp = buf;
	int i;

	for (i = 0; i < len; i++) {
		if (!(i % 16) && i)
			pr_debug("\n");
		pr_debug("%02x ", *tmp++);
	}
	pr_debug("\n");
}
#else
#define dump_fifo_data(buf, len)	do {} while (0)
#endif


#define USB_DMA_BASE		USB_DMA_INTERRUPT
#define USB_DMAx_CTRL		0x04
#define USB_DMAx_ADDR_LOW	0x08
#define USB_DMAx_ADDR_HIGH	0x0C
#define USB_DMAx_COUNT_LOW	0x10
#define USB_DMAx_COUNT_HIGH	0x14

#define USB_DMA_REG(ep, reg)	(USB_DMA_BASE + 0x20 * ep + reg)

/* Almost 1 second */
#define TIMER_DELAY	(1 * HZ)

static struct timer_list musb_conn_timer;

#endif	/* __MUSB_BLACKFIN_H__ */
