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

#ifndef __LIS3LV02D_H_
#define __LIS3LV02D_H_

struct lis3lv02d_platform_data {
	/* please note: the 'click' feature is only supported for
	 * LIS[32]02DL variants of the chip and will be ignored for
	 * others */
#define LIS3_CLICK_SINGLE_X	(1 << 0)
#define LIS3_CLICK_DOUBLE_X	(1 << 1)
#define LIS3_CLICK_SINGLE_Y	(1 << 2)
#define LIS3_CLICK_DOUBLE_Y	(1 << 3)
#define LIS3_CLICK_SINGLE_Z	(1 << 4)
#define LIS3_CLICK_DOUBLE_Z	(1 << 5)
	unsigned char click_flags;
	unsigned char click_thresh_x;
	unsigned char click_thresh_y;
	unsigned char click_thresh_z;
	unsigned char click_time_limit;
	unsigned char click_latency;
	unsigned char click_window;

#define LIS3_IRQ1_DISABLE	(0 << 0)
#define LIS3_IRQ1_FF_WU_1	(1 << 0)
#define LIS3_IRQ1_FF_WU_2	(2 << 0)
#define LIS3_IRQ1_FF_WU_12	(3 << 0)
#define LIS3_IRQ1_DATA_READY	(4 << 0)
#define LIS3_IRQ1_CLICK		(7 << 0)
#define LIS3_IRQ1_MASK		(7 << 0)
#define LIS3_IRQ2_DISABLE	(0 << 3)
#define LIS3_IRQ2_FF_WU_1	(1 << 3)
#define LIS3_IRQ2_FF_WU_2	(2 << 3)
#define LIS3_IRQ2_FF_WU_12	(3 << 3)
#define LIS3_IRQ2_DATA_READY	(4 << 3)
#define LIS3_IRQ2_CLICK		(7 << 3)
#define LIS3_IRQ2_MASK		(7 << 3)
#define LIS3_IRQ_OPEN_DRAIN	(1 << 6)
#define LIS3_IRQ_ACTIVE_LOW	(1 << 7)
	unsigned char irq_cfg;

#define LIS3_WAKEUP_X_LO	(1 << 0)
#define LIS3_WAKEUP_X_HI	(1 << 1)
#define LIS3_WAKEUP_Y_LO	(1 << 2)
#define LIS3_WAKEUP_Y_HI	(1 << 3)
#define LIS3_WAKEUP_Z_LO	(1 << 4)
#define LIS3_WAKEUP_Z_HI	(1 << 5)
	unsigned char wakeup_flags;
	unsigned char wakeup_thresh;
	unsigned char wakeup_flags2;
	unsigned char wakeup_thresh2;
#define LIS3_HIPASS_CUTFF_8HZ   0
#define LIS3_HIPASS_CUTFF_4HZ   1
#define LIS3_HIPASS_CUTFF_2HZ   2
#define LIS3_HIPASS_CUTFF_1HZ   3
#define LIS3_HIPASS1_DISABLE    (1 << 2)
#define LIS3_HIPASS2_DISABLE    (1 << 3)
	unsigned char hipass_ctrl;
#define LIS3_NO_MAP		0
#define LIS3_DEV_X		1
#define LIS3_DEV_Y		2
#define LIS3_DEV_Z		3
#define LIS3_INV_DEV_X	       -1
#define LIS3_INV_DEV_Y	       -2
#define LIS3_INV_DEV_Z	       -3
	s8 axis_x;
	s8 axis_y;
	s8 axis_z;
	int (*setup_resources)(void);
	int (*release_resources)(void);
	/* Limits for selftest are specified in chip data sheet */
	s16 st_min_limits[3]; /* min pass limit x, y, z */
	s16 st_max_limits[3]; /* max pass limit x, y, z */
	int irq2;
};

#endif /* __LIS3LV02D_H_ */
