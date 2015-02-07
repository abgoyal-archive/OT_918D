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

#ifndef _INC_PMCC4_SYSDEP_H_
#define _INC_PMCC4_SYSDEP_H_

/*-----------------------------------------------------------------------------
 * pmcc4_sysdep.h -
 *
 * Copyright (C) 2005  SBE, Inc.
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
 */

/* reduce multiple autoconf entries to a single definition */

#ifdef CONFIG_SBE_PMCC4_HDLC_V7_MODULE
#undef CONFIG_SBE_PMCC4_HDLC_V7
#define CONFIG_SBE_PMCC4_HDLC_V7  1
#endif

#ifdef CONFIG_SBE_PMCC4_NCOMM_MODULE
#undef CONFIG_SBE_PMCC4_NCOMM
#define CONFIG_SBE_PMCC4_NCOMM  1
#endif


/* FLUSH MACROS - if using ioremap_nocache(), then these can be NOOPS,
 * otherwise a memory barrier needs to be inserted.
 */

#define FLUSH_PCI_READ()     rmb()
#define FLUSH_PCI_WRITE()    wmb()
#define FLUSH_MEM_READ()     rmb()
#define FLUSH_MEM_WRITE()    wmb()


/*
 * System dependent callbacks routines, not inlined...
 * For inlined system dependent routines, see include/sbecom_inlinux_linux.h
 */

/*
 * passes received memory token back to the system, <user> is parameter from
 * sd_new_chan() used to create the channel which the data arrived on
 */

void sd_recv_consume(void *token, size_t len, void *user);

void        sd_disable_xmit (void *user);
void        sd_enable_xmit (void *user);
int         sd_line_is_ok (void *user);
void        sd_line_is_up (void *user);
void        sd_line_is_down (void *user);
int         sd_queue_stopped (void *user);

#endif                          /*** _INC_PMCC4_SYSDEP_H_ ***/
