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
 * Line6 Linux USB driver - 0.8.0
 *
 * Copyright (C) 2004-2009 Markus Grabner (grabner@icg.tugraz.at)
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License as
 *	published by the Free Software Foundation, version 2.
 *
 */

#ifndef DRIVER_H
#define DRIVER_H


#include "config.h"

#include <linux/spinlock.h>
#include <linux/usb.h>
#include <linux/wait.h>
#include <sound/core.h>

#include "midi.h"

#define DRIVER_NAME "line6usb"

#define LINE6_TIMEOUT 1
#define LINE6_MAX_DEVICES 8
#define LINE6_BUFSIZE_LISTEN 32
#define LINE6_MESSAGE_MAXLEN 256


/*
	Line6 MIDI control commands
*/
#define LINE6_PARAM_CHANGE   0xb0
#define LINE6_PROGRAM_CHANGE 0xc0
#define LINE6_SYSEX_BEGIN    0xf0
#define LINE6_SYSEX_END      0xf7
#define LINE6_RESET          0xff

/*
	MIDI channel for messages initiated by the host
	(and eventually echoed back by the device)
*/
#define LINE6_CHANNEL_HOST   0x00

/*
	MIDI channel for messages initiated by the device
*/
#define LINE6_CHANNEL_DEVICE 0x02

#define LINE6_CHANNEL_UNKNOWN 5  /* don't know yet what this is good for */

#define LINE6_CHANNEL_MASK 0x0f


#define MISSING_CASE	\
	printk(KERN_ERR "line6usb driver bug: missing case in %s:%d\n", \
		__FILE__, __LINE__)


#define CHECK_RETURN(x)		\
do {				\
	err = x;		\
	if (err < 0)		\
		return err;	\
} while (0)


extern const unsigned char line6_midi_id[3];
extern struct usb_line6 *line6_devices[LINE6_MAX_DEVICES];
extern struct workqueue_struct *line6_workqueue;

static const int SYSEX_DATA_OFS = sizeof(line6_midi_id) + 3;
static const int SYSEX_EXTRA_SIZE = sizeof(line6_midi_id) + 4;


/**
	 Common properties of Line6 devices.
*/
struct line6_properties {
	const char *name;
	int device_bit;
	int capabilities;
};

/**
	 Common data shared by all Line6 devices.
	 Corresponds to a pair of USB endpoints.
*/
struct usb_line6 {
	/**
		 USB device.
	*/
	struct usb_device *usbdev;

	/**
		 Product id.
	*/
	int product;

	/**
		 Properties.
	*/
	const struct line6_properties *properties;

	/**
		 Interface number.
	*/
	int interface_number;

	/**
		 Interval (ms).
	*/
	int interval;

	/**
		 Maximum size of USB packet.
	*/
	int max_packet_size;

	/**
		 Device representing the USB interface.
	*/
	struct device *ifcdev;

	/**
		 Line6 sound card data structure.
		 Each device has at least MIDI or PCM.
	*/
	struct snd_card *card;

	/**
		 Line6 PCM device data structure.
	*/
	struct snd_line6_pcm *line6pcm;

	/**
		 Line6 MIDI device data structure.
	*/
	struct snd_line6_midi *line6midi;

	/**
		 USB endpoint for listening to control commands.
	*/
	int ep_control_read;

	/**
		 USB endpoint for writing control commands.
	*/
	int ep_control_write;

	/**
		 URB for listening to PODxt Pro control endpoint.
	*/
	struct urb *urb_listen;

	/**
		 Buffer for listening to PODxt Pro control endpoint.
	*/
	unsigned char *buffer_listen;

	/**
		 Buffer for message to be processed.
	*/
	unsigned char *buffer_message;

	/**
		 Length of message to be processed.
	*/
	int message_length;
};


extern char *line6_alloc_sysex_buffer(struct usb_line6 *line6, int code1,
				      int code2, int size);
extern ssize_t line6_nop_read(struct device *dev,
			      struct device_attribute *attr, char *buf);
extern ssize_t line6_nop_write(struct device *dev,
			       struct device_attribute *attr,
			       const char *buf, size_t count);
extern int line6_read_data(struct usb_line6 *line6, int address, void *data,
			   size_t datalen);
extern int line6_read_serial_number(struct usb_line6 *line6,
				    int *serial_number);
extern int line6_send_program(struct usb_line6 *line6, int value);
extern int line6_send_raw_message(struct usb_line6 *line6, const char *buffer,
				  int size);
extern int line6_send_raw_message_async(struct usb_line6 *line6,
					const char *buffer, int size);
extern int line6_send_sysex_message(struct usb_line6 *line6,
				    const char *buffer, int size);
extern ssize_t line6_set_raw(struct device *dev, struct device_attribute *attr,
			     const char *buf, size_t count);
extern int line6_transmit_parameter(struct usb_line6 *line6, int param,
				    int value);
extern int line6_write_data(struct usb_line6 *line6, int address, void *data,
			    size_t datalen);
extern void line6_write_hexdump(struct usb_line6 *line6, char dir,
				const unsigned char *buffer, int size);


#endif
