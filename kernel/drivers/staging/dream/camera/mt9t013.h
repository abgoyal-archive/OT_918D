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
 * Copyright (C) 2008-2009 QUALCOMM Incorporated.
 */

#ifndef MT9T013_H
#define MT9T013_H

#include <linux/types.h>

struct reg_struct {
	uint16_t vt_pix_clk_div;        /*  0x0300 */
	uint16_t vt_sys_clk_div;        /*  0x0302 */
	uint16_t pre_pll_clk_div;       /*  0x0304 */
	uint16_t pll_multiplier;        /*  0x0306 */
	uint16_t op_pix_clk_div;        /*  0x0308 */
	uint16_t op_sys_clk_div;        /*  0x030A */
	uint16_t scale_m;               /*  0x0404 */
	uint16_t row_speed;             /*  0x3016 */
	uint16_t x_addr_start;          /*  0x3004 */
	uint16_t x_addr_end;            /*  0x3008 */
	uint16_t y_addr_start;        	/*  0x3002 */
	uint16_t y_addr_end;            /*  0x3006 */
	uint16_t read_mode;             /*  0x3040 */
	uint16_t x_output_size;         /*  0x034C */
	uint16_t y_output_size;         /*  0x034E */
	uint16_t line_length_pck;       /*  0x300C */
	uint16_t frame_length_lines;	/*  0x300A */
	uint16_t coarse_int_time; 		/*  0x3012 */
	uint16_t fine_int_time;   		/*  0x3014 */
};

struct mt9t013_i2c_reg_conf {
	unsigned short waddr;
	unsigned short wdata;
};

struct mt9t013_reg {
	struct reg_struct *reg_pat;
	uint16_t reg_pat_size;
	struct mt9t013_i2c_reg_conf *ttbl;
	uint16_t ttbl_size;
	struct mt9t013_i2c_reg_conf *lctbl;
	uint16_t lctbl_size;
	struct mt9t013_i2c_reg_conf *rftbl;
	uint16_t rftbl_size;
};

#endif /* #define MT9T013_H */
