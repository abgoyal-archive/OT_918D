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
 *   Copyright (c) 2006,2007 Daniel Mack
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
*/

#include <linux/usb.h>
#include <linux/gfp.h>
#include <sound/rawmidi.h>
#include <sound/core.h>
#include <sound/pcm.h>

#include "device.h"
#include "midi.h"

static int snd_usb_caiaq_midi_input_open(struct snd_rawmidi_substream *substream)
{
	return 0;
}

static int snd_usb_caiaq_midi_input_close(struct snd_rawmidi_substream *substream)
{
	return 0;
}

static void snd_usb_caiaq_midi_input_trigger(struct snd_rawmidi_substream *substream, int up)
{
	struct snd_usb_caiaqdev *dev = substream->rmidi->private_data;

	if (!dev)
		return;

	dev->midi_receive_substream = up ? substream : NULL;
}


static int snd_usb_caiaq_midi_output_open(struct snd_rawmidi_substream *substream)
{
	return 0;
}

static int snd_usb_caiaq_midi_output_close(struct snd_rawmidi_substream *substream)
{
	struct snd_usb_caiaqdev *dev = substream->rmidi->private_data;
	if (dev->midi_out_active) {
		usb_kill_urb(&dev->midi_out_urb);
		dev->midi_out_active = 0;
	}
	return 0;
}

static void snd_usb_caiaq_midi_send(struct snd_usb_caiaqdev *dev,
				    struct snd_rawmidi_substream *substream)
{
	int len, ret;

	dev->midi_out_buf[0] = EP1_CMD_MIDI_WRITE;
	dev->midi_out_buf[1] = 0; /* port */
	len = snd_rawmidi_transmit(substream, dev->midi_out_buf + 3,
				   EP1_BUFSIZE - 3);

	if (len <= 0)
		return;

	dev->midi_out_buf[2] = len;
	dev->midi_out_urb.transfer_buffer_length = len+3;

	ret = usb_submit_urb(&dev->midi_out_urb, GFP_ATOMIC);
	if (ret < 0)
		log("snd_usb_caiaq_midi_send(%p): usb_submit_urb() failed,"
		    "ret=%d, len=%d\n",
		    substream, ret, len);
	else
		dev->midi_out_active = 1;
}

static void snd_usb_caiaq_midi_output_trigger(struct snd_rawmidi_substream *substream, int up)
{
	struct snd_usb_caiaqdev *dev = substream->rmidi->private_data;

	if (up) {
		dev->midi_out_substream = substream;
		if (!dev->midi_out_active)
			snd_usb_caiaq_midi_send(dev, substream);
	} else {
		dev->midi_out_substream = NULL;
	}
}


static struct snd_rawmidi_ops snd_usb_caiaq_midi_output =
{
	.open =		snd_usb_caiaq_midi_output_open,
	.close =	snd_usb_caiaq_midi_output_close,
	.trigger =      snd_usb_caiaq_midi_output_trigger,
};

static struct snd_rawmidi_ops snd_usb_caiaq_midi_input =
{
	.open =		snd_usb_caiaq_midi_input_open,
	.close =	snd_usb_caiaq_midi_input_close,
	.trigger =      snd_usb_caiaq_midi_input_trigger,
};

void snd_usb_caiaq_midi_handle_input(struct snd_usb_caiaqdev *dev,
				     int port, const char *buf, int len)
{
	if (!dev->midi_receive_substream)
		return;

	snd_rawmidi_receive(dev->midi_receive_substream, buf, len);
}

int snd_usb_caiaq_midi_init(struct snd_usb_caiaqdev *device)
{
	int ret;
	struct snd_rawmidi *rmidi;

	ret = snd_rawmidi_new(device->chip.card, device->product_name, 0,
					device->spec.num_midi_out,
					device->spec.num_midi_in,
					&rmidi);

	if (ret < 0)
		return ret;

	strcpy(rmidi->name, device->product_name);

	rmidi->info_flags = SNDRV_RAWMIDI_INFO_DUPLEX;
	rmidi->private_data = device;

	if (device->spec.num_midi_out > 0) {
		rmidi->info_flags |= SNDRV_RAWMIDI_INFO_OUTPUT;
		snd_rawmidi_set_ops(rmidi, SNDRV_RAWMIDI_STREAM_OUTPUT,
				    &snd_usb_caiaq_midi_output);
	}

	if (device->spec.num_midi_in > 0) {
		rmidi->info_flags |= SNDRV_RAWMIDI_INFO_INPUT;
		snd_rawmidi_set_ops(rmidi, SNDRV_RAWMIDI_STREAM_INPUT,
				    &snd_usb_caiaq_midi_input);
	}

	device->rmidi = rmidi;

	return 0;
}

void snd_usb_caiaq_midi_output_done(struct urb* urb)
{
	struct snd_usb_caiaqdev *dev = urb->context;

	dev->midi_out_active = 0;
	if (urb->status != 0)
		return;

	if (!dev->midi_out_substream)
		return;

	snd_usb_caiaq_midi_send(dev, dev->midi_out_substream);
}

