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
 * Filesystem based user-mode API to USB Gadget controller hardware
 *
 * Other than ep0 operations, most things are done by read() and write()
 * on endpoint files found in one directory.  They are configured by
 * writing descriptors, and then may be used for normal stream style
 * i/o requests.  When ep0 is configured, the device can enumerate;
 * when it's closed, the device disconnects from usb.  Operations on
 * ep0 require ioctl() operations.
 *
 * Configuration and device descriptors get written to /dev/gadget/$CHIP,
 * which may then be used to read usb_gadgetfs_event structs.  The driver
 * may activate endpoints as it handles SET_CONFIGURATION setup events,
 * or earlier; writing endpoint descriptors to /dev/gadget/$ENDPOINT
 * then performing data transfers by reading or writing.
 */

#ifndef __LINUX_USB_GADGETFS_H
#define __LINUX_USB_GADGETFS_H

#include <linux/types.h>
#include <linux/ioctl.h>

#include <linux/usb/ch9.h>

/*
 * Events are delivered on the ep0 file descriptor, when the user mode driver
 * reads from this file descriptor after writing the descriptors.  Don't
 * stop polling this descriptor.
 */

enum usb_gadgetfs_event_type {
	GADGETFS_NOP = 0,

	GADGETFS_CONNECT,
	GADGETFS_DISCONNECT,
	GADGETFS_SETUP,
	GADGETFS_SUSPEND,
	/* and likely more ! */
};

/* NOTE:  this structure must stay the same size and layout on
 * both 32-bit and 64-bit kernels.
 */
struct usb_gadgetfs_event {
	union {
		/* NOP, DISCONNECT, SUSPEND: nothing
		 * ... some hardware can't report disconnection
		 */

		/* CONNECT: just the speed */
		enum usb_device_speed	speed;

		/* SETUP: packet; DATA phase i/o precedes next event
		 *(setup.bmRequestType & USB_DIR_IN) flags direction
		 * ... includes SET_CONFIGURATION, SET_INTERFACE
		 */
		struct usb_ctrlrequest	setup;
	} u;
	enum usb_gadgetfs_event_type	type;
};


/* The 'g' code is also used by printer gadget ioctl requests.
 * Don't add any colliding codes to either driver, and keep
 * them in unique ranges (size 0x20 for now).
 */

/* endpoint ioctls */

/* IN transfers may be reported to the gadget driver as complete
 *	when the fifo is loaded, before the host reads the data;
 * OUT transfers may be reported to the host's "client" driver as
 *	complete when they're sitting in the FIFO unread.
 * THIS returns how many bytes are "unclaimed" in the endpoint fifo
 * (needed for precise fault handling, when the hardware allows it)
 */
#define	GADGETFS_FIFO_STATUS	_IO('g', 1)

/* discards any unclaimed data in the fifo. */
#define	GADGETFS_FIFO_FLUSH	_IO('g', 2)

/* resets endpoint halt+toggle; used to implement set_interface.
 * some hardware (like pxa2xx) can't support this.
 */
#define	GADGETFS_CLEAR_HALT	_IO('g', 3)

#endif /* __LINUX_USB_GADGETFS_H */
