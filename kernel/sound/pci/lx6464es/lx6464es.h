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

/* -*- linux-c -*- *
 *
 * ALSA driver for the digigram lx6464es interface
 *
 * Copyright (c) 2009 Tim Blechmann <tim@klingt.org>
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 */

#ifndef LX6464ES_H
#define LX6464ES_H

#include <linux/spinlock.h>
#include <asm/atomic.h>

#include <sound/core.h>
#include <sound/pcm.h>

#include "lx_core.h"

#define LXP "LX6464ES: "

enum {
    ES_cmd_free         = 0,    /* no command executing */
    ES_cmd_processing   = 1,	/* execution of a read/write command */
    ES_read_pending     = 2,    /* a asynchron read command is pending */
    ES_read_finishing   = 3,    /* a read command has finished waiting (set by
				 * Interrupt or CancelIrp) */
};

enum lx_stream_status {
	LX_STREAM_STATUS_FREE,
/* 	LX_STREAM_STATUS_OPEN, */
	LX_STREAM_STATUS_SCHEDULE_RUN,
/* 	LX_STREAM_STATUS_STARTED, */
	LX_STREAM_STATUS_RUNNING,
	LX_STREAM_STATUS_SCHEDULE_STOP,
/* 	LX_STREAM_STATUS_STOPPED, */
/* 	LX_STREAM_STATUS_PAUSED */
};


struct lx_stream {
	struct snd_pcm_substream  *stream;
	snd_pcm_uframes_t          frame_pos;
	enum lx_stream_status      status; /* free, open, running, draining
					    * pause */
	int                        is_capture:1;
};


struct lx6464es {
	struct snd_card        *card;
	struct pci_dev         *pci;
	int			irq;

	spinlock_t		lock;        /* interrupt spinlock */
	struct mutex            setup_mutex; /* mutex used in hw_params, open
					      * and close */

	struct tasklet_struct   trigger_tasklet; /* trigger tasklet */
	struct tasklet_struct   tasklet_capture;
	struct tasklet_struct   tasklet_playback;

	/* ports */
	unsigned long		port_plx;	   /* io port (size=256) */
	void __iomem           *port_plx_remapped; /* remapped plx port */
	void __iomem           *port_dsp_bar;      /* memory port (32-bit,
						    * non-prefetchable,
						    * size=8K) */

	/* messaging */
	spinlock_t		msg_lock;          /* message spinlock */
	struct lx_rmh           rmh;

	/* configuration */
	uint			freq_ratio : 2;
	uint                    playback_mute : 1;
	uint                    hardware_running[2];
	u32                     board_sample_rate; /* sample rate read from
						    * board */
	u16                     pcm_granularity;   /* board blocksize */

	/* dma */
	struct snd_dma_buffer   capture_dma_buf;
	struct snd_dma_buffer   playback_dma_buf;

	/* pcm */
	struct snd_pcm         *pcm;

	/* streams */
	struct lx_stream        capture_stream;
	struct lx_stream        playback_stream;
};


#endif /* LX6464ES_H */
