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
 * board initialization code should put one of these into dev->platform_data
 * and place the isp1362 onto platform_bus.
 */

#ifndef __LINUX_USB_ISP1362_H__
#define __LINUX_USB_ISP1362_H__

struct isp1362_platform_data {
	/* Enable internal pulldown resistors on downstream ports */
	unsigned sel15Kres:1;
	/* Clock cannot be stopped */
	unsigned clknotstop:1;
	/* On-chip overcurrent protection */
	unsigned oc_enable:1;
	/* INT output polarity */
	unsigned int_act_high:1;
	/* INT edge or level triggered */
	unsigned int_edge_triggered:1;
	/* DREQ output polarity */
	unsigned dreq_act_high:1;
	/* DACK input polarity */
	unsigned dack_act_high:1;
	/* chip can be resumed via H_WAKEUP pin */
	unsigned remote_wakeup_connected:1;
	/* Switch or not to switch (keep always powered) */
	unsigned no_power_switching:1;
	/* Ganged port power switching (0) or individual port power switching (1) */
	unsigned power_switching_mode:1;
	/* Given port_power, msec/2 after power on till power good */
	u8 potpg;
	/* Hardware reset set/clear */
	void (*reset) (struct device *dev, int set);
	/* Clock start/stop */
	void (*clock) (struct device *dev, int start);
	/* Inter-io delay (ns). The chip is picky about access timings; it
	 * expects at least:
	 * 110ns delay between consecutive accesses to DATA_REG,
	 * 300ns delay between access to ADDR_REG and DATA_REG (registers)
	 * 462ns delay between access to ADDR_REG and DATA_REG (buffer memory)
	 * WE MUST NOT be activated during these intervals (even without CS!)
	 */
	void (*delay) (struct device *dev, unsigned int delay);
};

#endif
