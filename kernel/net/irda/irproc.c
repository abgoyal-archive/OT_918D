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

/*********************************************************************
 *
 * Filename:      irproc.c
 * Version:       1.0
 * Description:   Various entries in the /proc file system
 * Status:        Experimental.
 * Author:        Thomas Davis, <ratbert@radiks.net>
 * Created at:    Sat Feb 21 21:33:24 1998
 * Modified at:   Sun Nov 14 08:54:54 1999
 * Modified by:   Dag Brattli <dagb@cs.uit.no>
 *
 *     Copyright (c) 1998-1999, Dag Brattli <dagb@cs.uit.no>
 *     Copyright (c) 1998, Thomas Davis, <ratbert@radiks.net>,
 *     All Rights Reserved.
 *
 *     This program is free software; you can redistribute it and/or
 *     modify it under the terms of the GNU General Public License as
 *     published by the Free Software Foundation; either version 2 of
 *     the License, or (at your option) any later version.
 *
 *     I, Thomas Davis, provide no warranty for any of this software.
 *     This material is provided "AS-IS" and at no charge.
 *
 ********************************************************************/

#include <linux/miscdevice.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/module.h>
#include <linux/init.h>
#include <net/net_namespace.h>

#include <net/irda/irda.h>
#include <net/irda/irlap.h>
#include <net/irda/irlmp.h>

extern const struct file_operations discovery_seq_fops;
extern const struct file_operations irlap_seq_fops;
extern const struct file_operations irlmp_seq_fops;
extern const struct file_operations irttp_seq_fops;
extern const struct file_operations irias_seq_fops;

struct irda_entry {
	const char *name;
	const struct file_operations *fops;
};

struct proc_dir_entry *proc_irda;
EXPORT_SYMBOL(proc_irda);

static const struct irda_entry irda_dirs[] = {
	{"discovery",	&discovery_seq_fops},
	{"irttp",	&irttp_seq_fops},
	{"irlmp",	&irlmp_seq_fops},
	{"irlap",	&irlap_seq_fops},
	{"irias",	&irias_seq_fops},
};

/*
 * Function irda_proc_register (void)
 *
 *    Register irda entry in /proc file system
 *
 */
void __init irda_proc_register(void)
{
	int i;
	struct proc_dir_entry *d;

	proc_irda = proc_mkdir("irda", init_net.proc_net);
	if (proc_irda == NULL)
		return;

	for (i = 0; i < ARRAY_SIZE(irda_dirs); i++)
		d = proc_create(irda_dirs[i].name, 0, proc_irda,
				irda_dirs[i].fops);
}

/*
 * Function irda_proc_unregister (void)
 *
 *    Unregister irda entry in /proc file system
 *
 */
void irda_proc_unregister(void)
{
	int i;

	if (proc_irda) {
		for (i=0; i<ARRAY_SIZE(irda_dirs); i++)
			remove_proc_entry(irda_dirs[i].name, proc_irda);

		remove_proc_entry("irda", init_net.proc_net);
		proc_irda = NULL;
	}
}


