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
 * Copyright (C) 2009 Lemote Inc.
 * Author: Wu Zhangjin, wuzhangjin@gmail.com
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */
#include <asm/bootinfo.h>

#include <loongson.h>

void __init mach_prom_init_machtype(void)
{
	/* We share the same kernel image file among Lemote 2F family
	 * of machines, and provide the machtype= kernel command line
	 * to users to indicate their machine, this command line will
	 * be passed by the latest PMON automatically. and fortunately,
	 * up to now, we can get the machine type from the PMON_VER=
	 * commandline directly except the NAS machine, In the old
	 * machines, this will help the users a lot.
	 *
	 * If no "machtype=" passed, get machine type from "PMON_VER=".
	 * 	PMON_VER=LM8089		Lemote 8.9'' netbook
	 * 	         LM8101		Lemote 10.1'' netbook
	 * 	(The above two netbooks have the same kernel support)
	 *	         LM6XXX		Lemote FuLoong(2F) box series
	 *	         LM9XXX		Lemote LynLoong PC series
	 */
	if (strstr(arcs_cmdline, "PMON_VER=LM")) {
		if (strstr(arcs_cmdline, "PMON_VER=LM8"))
			mips_machtype = MACH_LEMOTE_YL2F89;
		else if (strstr(arcs_cmdline, "PMON_VER=LM6"))
			mips_machtype = MACH_LEMOTE_FL2F;
		else if (strstr(arcs_cmdline, "PMON_VER=LM9"))
			mips_machtype = MACH_LEMOTE_LL2F;
		else
			mips_machtype = MACH_LEMOTE_NAS;

		strcat(arcs_cmdline, " machtype=");
		strcat(arcs_cmdline, get_system_type());
		strcat(arcs_cmdline, " ");
	}
}
