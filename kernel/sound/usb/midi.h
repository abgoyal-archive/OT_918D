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

#ifndef __USBMIDI_H
#define __USBMIDI_H

/* maximum number of endpoints per interface */
#define MIDI_MAX_ENDPOINTS 2

/* data for QUIRK_MIDI_FIXED_ENDPOINT */
struct snd_usb_midi_endpoint_info {
	int8_t   out_ep;	/* ep number, 0 autodetect */
	uint8_t  out_interval;	/* interval for interrupt endpoints */
	int8_t   in_ep;
	uint8_t  in_interval;
	uint16_t out_cables;	/* bitmask */
	uint16_t in_cables;	/* bitmask */
};

/* for QUIRK_MIDI_YAMAHA, data is NULL */

/* for QUIRK_MIDI_MIDIMAN, data points to a snd_usb_midi_endpoint_info
 * structure (out_cables and in_cables only) */

/* for QUIRK_COMPOSITE, data points to an array of snd_usb_audio_quirk
 * structures, terminated with .ifnum = -1 */

/* for QUIRK_AUDIO_FIXED_ENDPOINT, data points to an audioformat structure */

/* for QUIRK_AUDIO/MIDI_STANDARD_INTERFACE, data is NULL */

/* for QUIRK_AUDIO_EDIROL_UA700_UA25/UA1000, data is NULL */

/* for QUIRK_IGNORE_INTERFACE, data is NULL */

/* for QUIRK_MIDI_NOVATION and _RAW, data is NULL */

/* for QUIRK_MIDI_EMAGIC, data points to a snd_usb_midi_endpoint_info
 * structure (out_cables and in_cables only) */

/* for QUIRK_MIDI_CME, data is NULL */

/* for QUIRK_MIDI_AKAI, data is NULL */

int snd_usbmidi_create(struct snd_card *card,
		       struct usb_interface *iface,
		       struct list_head *midi_list,
		       const struct snd_usb_audio_quirk *quirk);
void snd_usbmidi_input_stop(struct list_head* p);
void snd_usbmidi_input_start(struct list_head* p);
void snd_usbmidi_disconnect(struct list_head *p);

#endif /* __USBMIDI_H */
