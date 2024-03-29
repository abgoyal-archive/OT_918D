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
    saa7146.h - definitions philips saa7146 based cards
    Copyright (C) 1999 Nathan Laredo (laredo@gnu.org)

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#ifndef __SAA7146__
#define __SAA7146__

#define SAA7146_VERSION_CODE 0x000101

#include <linux/types.h>
#include <linux/wait.h>

#ifndef O_NONCAP
#define O_NONCAP	O_TRUNC
#endif

#define MAX_GBUFFERS	2
#define FBUF_SIZE	0x190000

#ifdef __KERNEL__

struct saa7146_window
{
	int x, y;
	ushort width, height;
	ushort bpp, bpl;
	ushort swidth, sheight;
	short cropx, cropy;
	ushort cropwidth, cropheight;
	unsigned long vidadr;
	int color_fmt;
	ushort depth;
};

/*  Per-open data for handling multiple opens on one device */
struct device_open
{
	int	     isopen;
	int	     noncapturing;
	struct saa7146  *dev;
};
#define MAX_OPENS 3

struct saa7146
{
	struct video_device video_dev;
	struct video_picture picture;
	struct video_audio audio_dev;
	struct video_info vidinfo;
	int user;
	int cap;
	int capuser;
	int irqstate;		/* irq routine is state driven */
	int writemode;
	int playmode;
	unsigned int nr;
	unsigned long irq;          /* IRQ used by SAA7146 card */
	unsigned short id;
	unsigned char revision;
	unsigned char boardcfg[64];	/* 64 bytes of config from eeprom */
	unsigned long saa7146_adr;   /* bus address of IO mem from PCI BIOS */
	struct saa7146_window win;
	unsigned char __iomem *saa7146_mem; /* pointer to mapped IO memory */
	struct device_open open_data[MAX_OPENS];
#define MAX_MARKS 16
	/* for a/v sync */
	int endmark[MAX_MARKS], endmarkhead, endmarktail;
	u32 *dmaRPS1, *pageRPS1, *dmaRPS2, *pageRPS2, *dmavid1, *dmavid2,
		*dmavid3, *dmaa1in, *dmaa1out, *dmaa2in, *dmaa2out,
		*pagedebi, *pagevid1, *pagevid2, *pagevid3, *pagea1in,
		*pagea1out, *pagea2in, *pagea2out;
	wait_queue_head_t i2cq, debiq, audq, vidq;
	u8  *vidbuf, *audbuf, *osdbuf, *dmadebi;
	int audhead, vidhead, osdhead, audtail, vidtail, osdtail;
	spinlock_t lock;	/* the device lock */
};
#endif

#ifdef _ALPHA_SAA7146
#define saawrite(dat,adr)    writel((dat), saa->saa7146_adr+(adr))
#define saaread(adr)         readl(saa->saa7146_adr+(adr))
#else
#define saawrite(dat,adr)    writel((dat), saa->saa7146_mem+(adr))
#define saaread(adr)         readl(saa->saa7146_mem+(adr))
#endif

#define saaand(dat,adr)      saawrite((dat) & saaread(adr), adr)
#define saaor(dat,adr)       saawrite((dat) | saaread(adr), adr)
#define saaaor(dat,mask,adr) saawrite((dat) | ((mask) & saaread(adr)), adr)

/* bitmask of attached hardware found */
#define SAA7146_UNKNOWN		0x00000000
#define SAA7146_SAA7111		0x00000001
#define SAA7146_SAA7121		0x00000002
#define SAA7146_IBMMPEG		0x00000004

#endif
