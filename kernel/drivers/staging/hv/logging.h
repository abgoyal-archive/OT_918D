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
 *
 * Copyright (c) 2009, Microsoft Corporation.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 59 Temple
 * Place - Suite 330, Boston, MA 02111-1307 USA.
 *
 * Authors:
 *   Haiyang Zhang <haiyangz@microsoft.com>
 *   Hank Janssen  <hjanssen@microsoft.com>
 *
 */


#ifndef _LOGGING_H_
#define _LOGGING_H_

/* #include <linux/init.h> */
/* #include <linux/module.h> */


#define VMBUS				0x0001
#define STORVSC				0x0002
#define NETVSC				0x0004
#define INPUTVSC			0x0008
#define BLKVSC				0x0010
#define VMBUS_DRV			0x0100
#define STORVSC_DRV			0x0200
#define NETVSC_DRV			0x0400
#define INPUTVSC_DRV		0x0800
#define BLKVSC_DRV			0x1000

#define ALL_MODULES			(VMBUS		|\
							STORVSC		|\
							NETVSC		|\
							INPUTVSC	|\
							BLKVSC		|\
							VMBUS_DRV	|\
							STORVSC_DRV	|\
							NETVSC_DRV	|\
							INPUTVSC_DRV|\
							BLKVSC_DRV)

/* Logging Level */
#define ERROR_LVL				3
#define WARNING_LVL				4
#define INFO_LVL				6
#define DEBUG_LVL				7
#define DEBUG_LVL_ENTEREXIT			8
#define DEBUG_RING_LVL				9

extern unsigned int vmbus_loglevel;

#define DPRINT(mod, lvl, fmt, args...) do {\
	if ((mod & (HIWORD(vmbus_loglevel))) &&	\
	    (lvl <= LOWORD(vmbus_loglevel)))	\
		printk(KERN_DEBUG #mod": %s() " fmt "\n", __func__, ## args);\
	} while (0)

#define DPRINT_DBG(mod, fmt, args...) do {\
	if ((mod & (HIWORD(vmbus_loglevel))) &&		\
	    (DEBUG_LVL <= LOWORD(vmbus_loglevel)))	\
		printk(KERN_DEBUG #mod": %s() " fmt "\n", __func__, ## args);\
	} while (0)

#define DPRINT_INFO(mod, fmt, args...) do {\
	if ((mod & (HIWORD(vmbus_loglevel))) &&		\
	    (INFO_LVL <= LOWORD(vmbus_loglevel)))	\
		printk(KERN_INFO #mod": " fmt "\n", ## args);\
	} while (0)

#define DPRINT_WARN(mod, fmt, args...) do {\
	if ((mod & (HIWORD(vmbus_loglevel))) &&		\
	    (WARNING_LVL <= LOWORD(vmbus_loglevel)))	\
		printk(KERN_WARNING #mod": WARNING! " fmt "\n", ## args);\
	} while (0)

#define DPRINT_ERR(mod, fmt, args...) do {\
	if ((mod & (HIWORD(vmbus_loglevel))) &&		\
	    (ERROR_LVL <= LOWORD(vmbus_loglevel)))	\
		printk(KERN_ERR #mod": %s() ERROR!! " fmt "\n",	\
		       __func__, ## args);\
	} while (0)

#ifdef DEBUG
#define DPRINT_ENTER(mod) do {\
	if ((mod & (HIWORD(vmbus_loglevel))) && \
	    (DEBUG_LVL_ENTEREXIT <= LOWORD(vmbus_loglevel)))	\
		printk(KERN_DEBUG "["#mod"]: %s() enter\n", __func__);\
	} while (0)

#define DPRINT_EXIT(mod) do {\
	if ((mod & (HIWORD(vmbus_loglevel))) && \
	    (DEBUG_LVL_ENTEREXIT <= LOWORD(vmbus_loglevel)))	\
		printk(KERN_DEBUG "["#mod"]: %s() exit\n", __func__);\
	} while (0)
#else
#define DPRINT_ENTER(mod)
#define DPRINT_EXIT(mod)
#endif

#endif /* _LOGGING_H_ */
