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
 * mediabay.h: definitions for using the media bay
 * on PowerBook 3400 and similar computers.
 *
 * Copyright (C) 1997 Paul Mackerras.
 */
#ifndef _PPC_MEDIABAY_H
#define _PPC_MEDIABAY_H

#ifdef __KERNEL__

#define MB_FD		0	/* media bay contains floppy drive (automatic eject ?) */
#define MB_FD1		1	/* media bay contains floppy drive (manual eject ?) */
#define MB_SOUND	2	/* sound device ? */
#define MB_CD		3	/* media bay contains ATA drive such as CD or ZIP */
#define MB_PCI		5	/* media bay contains a PCI device */
#define MB_POWER	6	/* media bay contains a Power device (???) */
#define MB_NO		7	/* media bay contains nothing */

struct macio_dev;

#ifdef CONFIG_PMAC_MEDIABAY

/* Check the content type of the bay, returns MB_NO if the bay is still
 * transitionning
 */
extern int check_media_bay(struct macio_dev *bay);

/* The ATA driver uses the calls below to temporarily hold on the
 * media bay callbacks while initializing the interface
 */
extern void lock_media_bay(struct macio_dev *bay);
extern void unlock_media_bay(struct macio_dev *bay);

#else

static inline int check_media_bay(struct macio_dev *bay)
{
	return MB_NO;
}

static inline void lock_media_bay(struct macio_dev *bay) { }
static inline void unlock_media_bay(struct macio_dev *bay) { }

#endif

#endif /* __KERNEL__ */
#endif /* _PPC_MEDIABAY_H */
