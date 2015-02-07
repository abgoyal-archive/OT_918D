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
 * Copyright 1998-2008 VIA Technologies, Inc. All Rights Reserved.
 * Copyright 2001-2008 S3 Graphics, Inc. All Rights Reserved.

 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation;
 * either version 2, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTIES OR REPRESENTATIONS; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.See the GNU General Public License
 * for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc.,
 * 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef __VIAMODE_H__
#define __VIAMODE_H__

#include "global.h"

struct VPITTable {
	unsigned char Misc;
	unsigned char SR[StdSR];
	unsigned char GR[StdGR];
	unsigned char AR[StdAR];
};

struct VideoModeTable {
	struct crt_mode_table *crtc;
	int mode_array;
};

struct patch_table {
	int table_length;
	struct io_reg *io_reg_table;
};

struct res_map_refresh {
	int hres;
	int vres;
	int pixclock;
	int vmode_refresh;
};

extern int NUM_TOTAL_RES_MAP_REFRESH;
extern int NUM_TOTAL_CEA_MODES;
extern int NUM_TOTAL_CN400_ModeXregs;
extern int NUM_TOTAL_CN700_ModeXregs;
extern int NUM_TOTAL_KM400_ModeXregs;
extern int NUM_TOTAL_CX700_ModeXregs;
extern int NUM_TOTAL_VX855_ModeXregs;
extern int NUM_TOTAL_CLE266_ModeXregs;
extern int NUM_TOTAL_PATCH_MODE;

/********************/
/* Mode Table       */
/********************/

extern struct crt_mode_table CEAM1280x720[];
extern struct crt_mode_table CEAM1920x1080[];
extern struct VideoModeTable CEA_HDMI_Modes[];

extern struct res_map_refresh res_map_refresh_tbl[];
extern struct io_reg CN400_ModeXregs[];
extern struct io_reg CN700_ModeXregs[];
extern struct io_reg KM400_ModeXregs[];
extern struct io_reg CX700_ModeXregs[];
extern struct io_reg VX800_ModeXregs[];
extern struct io_reg VX855_ModeXregs[];
extern struct io_reg CLE266_ModeXregs[];
extern struct io_reg PM1024x768[];
extern struct patch_table res_patch_table[];
extern struct VPITTable VPIT;

struct VideoModeTable *viafb_get_mode(int hres, int vres);
struct VideoModeTable *viafb_get_rb_mode(int hres, int vres);

#endif /* __VIAMODE_H__ */
