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
 * Copyright (c) 1996, 2003 VIA Networking Technologies, Inc.
 * All rights reserved.
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
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * File: iwctl.h
 *
 * Purpose:
 *
 * Author: Lyndon Chen
 *
 * Date: May 21, 2004
 *
 */

#ifndef __IWCTL_H__
#define __IWCTL_H__

#include "device.h"

/*---------------------  Export Definitions -------------------------*/


/*---------------------  Export Classes  ----------------------------*/

/*---------------------  Export Variables  --------------------------*/

/*---------------------  Export Functions  --------------------------*/

struct iw_statistics *iwctl_get_wireless_stats (struct net_device *dev);


int iwctl_siwap(struct net_device *dev,
             struct iw_request_info *info,
			 struct sockaddr *wrq,
             char *extra);

int iwctl_giwrange(struct net_device *dev,
             struct iw_request_info *info,
             struct iw_point *wrq,
             char *extra);


int iwctl_giwmode(struct net_device *dev,
             struct iw_request_info *info,
             __u32 *wmode,
             char *extra);

int iwctl_siwmode(struct net_device *dev,
             struct iw_request_info *info,
             __u32 *wmode,
             char *extra);

int iwctl_giwfreq(struct net_device *dev,
             struct iw_request_info *info,
             struct iw_freq *wrq,
             char *extra);

int iwctl_siwfreq(struct net_device *dev,
             struct iw_request_info *info,
             struct iw_freq *wrq,
             char *extra);

int iwctl_giwname(struct net_device *dev,
			 struct iw_request_info *info,
			 char *wrq,
			 char *extra);

int iwctl_giwnwid(struct net_device *dev,
             struct iw_request_info *info,
			 struct iw_param *wrq,
                   char *extra) ;

int iwctl_giwsens(struct net_device *dev,
			 struct iw_request_info *info,
			 struct iw_param *wrq,
			 char *extra);

int iwctl_giwap(struct net_device *dev,
             struct iw_request_info *info,
			 struct sockaddr *wrq,
             char *extra);

int iwctl_giwaplist(struct net_device *dev,
             struct iw_request_info *info,
             struct iw_point *wrq,
             char *extra);

int iwctl_siwessid(struct net_device *dev,
             struct iw_request_info *info,
             struct iw_point *wrq,
             char *extra);

int iwctl_giwessid(struct net_device *dev,
             struct iw_request_info *info,
             struct iw_point *wrq,
             char *extra);

int iwctl_siwrate(struct net_device *dev,
             struct iw_request_info *info,
			 struct iw_param *wrq,
             char *extra);

int iwctl_giwrate(struct net_device *dev,
             struct iw_request_info *info,
             struct iw_param *wrq,
             char *extra);

int iwctl_siwrts(struct net_device *dev,
             struct iw_request_info *info,
			 struct iw_param *wrq,
             char *extra);


int iwctl_giwrts(struct net_device *dev,
             struct iw_request_info *info,
			 struct iw_param *wrq,
             char *extra);

int iwctl_siwfrag(struct net_device *dev,
             struct iw_request_info *info,
			 struct iw_param *wrq,
             char *extra);

int iwctl_giwfrag(struct net_device *dev,
             struct iw_request_info *info,
			 struct iw_param *wrq,
             char *extra);

int iwctl_siwretry(struct net_device *dev,
             struct iw_request_info *info,
			 struct iw_param *wrq,
             char *extra);

int iwctl_giwretry(struct net_device *dev,
             struct iw_request_info *info,
			 struct iw_param *wrq,
             char *extra);

int iwctl_siwencode(struct net_device *dev,
             struct iw_request_info *info,
             struct iw_point *wrq,
             char *extra);

int iwctl_giwencode(struct net_device *dev,
             struct iw_request_info *info,
             struct iw_point *wrq,
             char *extra);

int iwctl_siwpower(struct net_device *dev,
             struct iw_request_info *info,
			 struct iw_param *wrq,
             char *extra);

int iwctl_giwpower(struct net_device *dev,
             struct iw_request_info *info,
			 struct iw_param *wrq,
             char *extra);

int iwctl_giwscan(struct net_device *dev,
             struct iw_request_info *info,
			 struct iw_point *wrq,
             char *extra);

int iwctl_siwscan(struct net_device *dev,
             struct iw_request_info *info,
			 struct iw_param *wrq,
             char *extra);

//2008-0409-07, <Add> by Einsn Liu
#ifdef WPA_SUPPLICANT_DRIVER_WEXT_SUPPORT
int iwctl_siwauth(struct net_device *dev,
			  struct iw_request_info *info,
			  struct iw_param *wrq,
			  char *extra);

int iwctl_giwauth(struct net_device *dev,
			  struct iw_request_info *info,
			  struct iw_param *wrq,
			  char *extra);

int iwctl_siwgenie(struct net_device *dev,
			  struct iw_request_info *info,
			  struct iw_point *wrq,
			  char *extra);

int iwctl_giwgenie(struct net_device *dev,
			  struct iw_request_info *info,
			  struct iw_point *wrq,
			  char *extra);

int iwctl_siwencodeext(struct net_device *dev,
             struct iw_request_info *info,
             struct iw_point *wrq,
             char *extra);

int iwctl_giwencodeext(struct net_device *dev,
             struct iw_request_info *info,
             struct iw_point *wrq,
             char *extra);

int iwctl_siwmlme(struct net_device *dev,
			struct iw_request_info * info,
			struct iw_point *wrq,
			char *extra);
#endif // #ifdef WPA_SUPPLICANT_DRIVER_WEXT_SUPPORT
//End Add -- //2008-0409-07, <Add> by Einsn Liu


extern const struct iw_handler_def	iwctl_handler_def;
extern const struct iw_priv_args	iwctl_private_args;

#endif // __IWCTL_H__



