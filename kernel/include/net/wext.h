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

#ifndef __NET_WEXT_H
#define __NET_WEXT_H

#include <net/iw_handler.h>

struct net;

#ifdef CONFIG_WEXT_CORE
extern int wext_handle_ioctl(struct net *net, struct ifreq *ifr, unsigned int cmd,
			     void __user *arg);
extern int compat_wext_handle_ioctl(struct net *net, unsigned int cmd,
				    unsigned long arg);

extern struct iw_statistics *get_wireless_stats(struct net_device *dev);
extern int call_commit_handler(struct net_device *dev);
#else
static inline int wext_handle_ioctl(struct net *net, struct ifreq *ifr, unsigned int cmd,
				    void __user *arg)
{
	return -EINVAL;
}
static inline int compat_wext_handle_ioctl(struct net *net, unsigned int cmd,
					   unsigned long arg)
{
	return -EINVAL;
}
#endif

#ifdef CONFIG_WEXT_PROC
extern int wext_proc_init(struct net *net);
extern void wext_proc_exit(struct net *net);
#else
static inline int wext_proc_init(struct net *net)
{
	return 0;
}
static inline void wext_proc_exit(struct net *net)
{
	return;
}
#endif

#ifdef CONFIG_WEXT_PRIV
int ioctl_private_call(struct net_device *dev, struct iwreq *iwr,
		       unsigned int cmd, struct iw_request_info *info,
		       iw_handler handler);
int compat_private_call(struct net_device *dev, struct iwreq *iwr,
			unsigned int cmd, struct iw_request_info *info,
			iw_handler handler);
int iw_handler_get_private(struct net_device *		dev,
			   struct iw_request_info *	info,
			   union iwreq_data *		wrqu,
			   char *			extra);
#else
#define ioctl_private_call NULL
#define compat_private_call NULL
#endif


#endif /* __NET_WEXT_H */
