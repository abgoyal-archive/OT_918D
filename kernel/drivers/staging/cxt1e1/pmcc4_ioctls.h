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

/* RCSid: $Header: /home/rickd/projects/pmcc4/include/pmcc4_ioctls.h,v 2.0 2005/09/28 00:10:09 rickd PMCC4_3_1B $
 */

#ifndef _INC_PMCC4_IOCTLS_H_
#define _INC_PMCC4_IOCTLS_H_

/*-----------------------------------------------------------------------------
 * pmcc4_ioctls.h -
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
 *
 * For further information, contact via email: support@sbei.com
 * SBE, Inc.  San Ramon, California  U.S.A.
 *-----------------------------------------------------------------------------
 * RCS info:
 * RCS revision: $Revision: 2.0 $
 * Last changed on $Date: 2005/09/28 00:10:09 $
 * Changed by $Author: rickd $
 *-----------------------------------------------------------------------------
 * $Log: pmcc4_ioctls.h,v $
 * Revision 2.0  2005/09/28 00:10:09  rickd
 * Add GNU license info. Switch Ioctls to sbe_ioc.h usage.
 *
 * Revision 1.2  2005/04/28 23:43:03  rickd
 * Add RCS tracking heading.
 *
 *-----------------------------------------------------------------------------
 */

#include "sbew_ioc.h"

enum
{
    // C4_GET_PORT = 0,
    // C4_SET_PORT,
    // C4_GET_CHAN,
    // C4_SET_CHAN,
    C4_DEL_CHAN = 0,
    // C4_CREATE_CHAN,
    // C4_GET_CHAN_STATS,
    // C4_RESET,
    // C4_DEBUG,
    C4_RESET_STATS,
    C4_LOOP_PORT,
    C4_RW_FRMR,
    C4_RW_MSYC,
    C4_RW_PLD
};

#define C4_GET_PORT          SBE_IOC_PORT_GET
#define C4_SET_PORT          SBE_IOC_PORT_SET
#define C4_GET_CHAN          SBE_IOC_CHAN_GET
#define C4_SET_CHAN          SBE_IOC_CHAN_SET
// #define C4_DEL_CHAN          XXX
#define C4_CREATE_CHAN       SBE_IOC_CHAN_NEW
#define C4_GET_CHAN_STATS    SBE_IOC_CHAN_GET_STAT
#define C4_RESET             SBE_IOC_RESET_DEV
#define C4_DEBUG             SBE_IOC_LOGLEVEL
// #define C4_RESET_STATS       XXX
// #define C4_LOOP_PORT         XXX
// #define C4_RW_FRMR           XXX
// #define C4_RW_MSYC           XXX
// #define C4_RW_PLD            XXX

struct c4_chan_stats_wrap
{
    int         channum;
    struct sbecom_chan_stats stats;
};

#endif   /* _INC_PMCC4_IOCTLS_H_ */
