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

/* arch/arm/mach-msm/qdsp5/adsp_lpm_verify_cmd.c
 *
 * Verificion code for aDSP LPM packets from userspace.
 *
 * Copyright (c) 2008 QUALCOMM Incorporated
 * Copyright (C) 2008 Google, Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <mach/qdsp5/qdsp5lpmcmdi.h>
#include "adsp.h"

int adsp_lpm_verify_cmd(struct msm_adsp_module *module,
			 unsigned int queue_id, void *cmd_data,
			 size_t cmd_size)
{
	uint32_t cmd_id, col_height, input_row_incr, output_row_incr,
		input_size, output_size;
	uint32_t size_mask = 0x0fff;
	lpm_cmd_start *cmd;

	if (queue_id != QDSP_lpmCommandQueue) {
		printk(KERN_ERR "adsp: module %s: wrong queue id %d\n",
			module->name, queue_id);
		return -1;
	}

	cmd = (lpm_cmd_start *)cmd_data;
	cmd_id = cmd->cmd_id;

	if (cmd_id == LPM_CMD_START) {
		if (cmd_size != sizeof(lpm_cmd_start)) {
			printk(KERN_ERR "adsp: module %s: wrong size %d, expect %d\n",
				module->name, cmd_size, sizeof(lpm_cmd_start));
			return -1;
		}
		col_height = cmd->ip_data_cfg_part1 & size_mask;
		input_row_incr = cmd->ip_data_cfg_part2 & size_mask;
		output_row_incr = cmd->op_data_cfg_part1 & size_mask;
		input_size = col_height * input_row_incr;
		output_size = col_height * output_row_incr;
		if ((cmd->ip_data_cfg_part4 && adsp_pmem_fixup(module,
				    (void **)(&cmd->ip_data_cfg_part4),
				    input_size)) ||
		   (cmd->op_data_cfg_part3 && adsp_pmem_fixup(module,
				    (void **)(&cmd->op_data_cfg_part3),
				    output_size)))
			return -1;
	} else if (cmd_id > 1) {
		printk(KERN_ERR "adsp: module %s: invalid cmd_id %d\n",
			module->name, cmd_id);
		return -1;
	}
	return 0;
}

