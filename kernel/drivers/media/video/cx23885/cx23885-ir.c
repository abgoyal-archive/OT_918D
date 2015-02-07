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
 *  Driver for the Conexant CX23885/7/8 PCIe bridge
 *
 *  Infrared device support routines - non-input, non-vl42_subdev routines
 *
 *  Copyright (C) 2009  Andy Walls <awalls@md.metrocast.net>
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 *  02110-1301, USA.
 */

#include <media/v4l2-device.h>

#include "cx23885.h"
#include "cx23885-input.h"

#define CX23885_IR_RX_FIFO_SERVICE_REQ		0
#define CX23885_IR_RX_END_OF_RX_DETECTED	1
#define CX23885_IR_RX_HW_FIFO_OVERRUN		2
#define CX23885_IR_RX_SW_FIFO_OVERRUN		3

#define CX23885_IR_TX_FIFO_SERVICE_REQ		0


void cx23885_ir_rx_work_handler(struct work_struct *work)
{
	struct cx23885_dev *dev =
			     container_of(work, struct cx23885_dev, ir_rx_work);
	u32 events = 0;
	unsigned long *notifications = &dev->ir_rx_notifications;

	if (test_and_clear_bit(CX23885_IR_RX_SW_FIFO_OVERRUN, notifications))
		events |= V4L2_SUBDEV_IR_RX_SW_FIFO_OVERRUN;
	if (test_and_clear_bit(CX23885_IR_RX_HW_FIFO_OVERRUN, notifications))
		events |= V4L2_SUBDEV_IR_RX_HW_FIFO_OVERRUN;
	if (test_and_clear_bit(CX23885_IR_RX_END_OF_RX_DETECTED, notifications))
		events |= V4L2_SUBDEV_IR_RX_END_OF_RX_DETECTED;
	if (test_and_clear_bit(CX23885_IR_RX_FIFO_SERVICE_REQ, notifications))
		events |= V4L2_SUBDEV_IR_RX_FIFO_SERVICE_REQ;

	if (events == 0)
		return;

	if (dev->ir_input)
		cx23885_input_rx_work_handler(dev, events);
}

void cx23885_ir_tx_work_handler(struct work_struct *work)
{
	struct cx23885_dev *dev =
			     container_of(work, struct cx23885_dev, ir_tx_work);
	u32 events = 0;
	unsigned long *notifications = &dev->ir_tx_notifications;

	if (test_and_clear_bit(CX23885_IR_TX_FIFO_SERVICE_REQ, notifications))
		events |= V4L2_SUBDEV_IR_TX_FIFO_SERVICE_REQ;

	if (events == 0)
		return;

}

/* Called in an IRQ context */
void cx23885_ir_rx_v4l2_dev_notify(struct v4l2_subdev *sd, u32 events)
{
	struct cx23885_dev *dev = to_cx23885(sd->v4l2_dev);
	unsigned long *notifications = &dev->ir_rx_notifications;

	if (events & V4L2_SUBDEV_IR_RX_FIFO_SERVICE_REQ)
		set_bit(CX23885_IR_RX_FIFO_SERVICE_REQ, notifications);
	if (events & V4L2_SUBDEV_IR_RX_END_OF_RX_DETECTED)
		set_bit(CX23885_IR_RX_END_OF_RX_DETECTED, notifications);
	if (events & V4L2_SUBDEV_IR_RX_HW_FIFO_OVERRUN)
		set_bit(CX23885_IR_RX_HW_FIFO_OVERRUN, notifications);
	if (events & V4L2_SUBDEV_IR_RX_SW_FIFO_OVERRUN)
		set_bit(CX23885_IR_RX_SW_FIFO_OVERRUN, notifications);
	schedule_work(&dev->ir_rx_work);
}

/* Called in an IRQ context */
void cx23885_ir_tx_v4l2_dev_notify(struct v4l2_subdev *sd, u32 events)
{
	struct cx23885_dev *dev = to_cx23885(sd->v4l2_dev);
	unsigned long *notifications = &dev->ir_tx_notifications;

	if (events & V4L2_SUBDEV_IR_TX_FIFO_SERVICE_REQ)
		set_bit(CX23885_IR_TX_FIFO_SERVICE_REQ, notifications);
	schedule_work(&dev->ir_tx_work);
}
