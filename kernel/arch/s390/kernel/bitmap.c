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
 *    Bitmaps for set_bit, clear_bit, test_and_set_bit, ...
 *    See include/asm/{bitops.h|posix_types.h} for details
 *
 *    Copyright IBM Corp. 1999,2009
 *    Author(s): Martin Schwidefsky <schwidefsky@de.ibm.com>,
 */

#include <linux/bitops.h>
#include <linux/module.h>

const char _oi_bitmap[] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };
EXPORT_SYMBOL(_oi_bitmap);

const char _ni_bitmap[] = { 0xfe, 0xfd, 0xfb, 0xf7, 0xef, 0xdf, 0xbf, 0x7f };
EXPORT_SYMBOL(_ni_bitmap);

const char _zb_findmap[] = {
	0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,
	0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,5,
	0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,
	0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,6,
	0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,
	0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,5,
	0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,
	0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,7,
	0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,
	0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,5,
	0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,
	0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,6,
	0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,
	0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,5,
	0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,
	0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,8 };
EXPORT_SYMBOL(_zb_findmap);

const char _sb_findmap[] = {
	8,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
	4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
	5,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
	4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
	6,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
	4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
	5,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
	4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
	7,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
	4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
	5,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
	4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
	6,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
	4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
	5,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
	4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0 };
EXPORT_SYMBOL(_sb_findmap);
