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

#ifndef __ASM_SH_HWBLK_H
#define __ASM_SH_HWBLK_H

#include <asm/clock.h>
#include <asm/io.h>

#define HWBLK_CNT_USAGE 0
#define HWBLK_CNT_IDLE 1
#define HWBLK_CNT_DEVICES 2
#define HWBLK_CNT_NR 3

#define HWBLK_AREA_FLAG_PARENT (1 << 0) /* valid parent */

#define HWBLK_AREA(_flags, _parent)		\
{						\
	.flags = _flags,			\
	.parent = _parent,			\
}

struct hwblk_area {
	int cnt[HWBLK_CNT_NR];
	unsigned char parent;
	unsigned char flags;
};

#define HWBLK(_mstp, _bit, _area)		\
{						\
	.mstp = (void __iomem *)_mstp,		\
	.bit = _bit,				\
	.area = _area,				\
}

struct hwblk {
	void __iomem *mstp;
	unsigned char bit;
	unsigned char area;
	int cnt[HWBLK_CNT_NR];
};

struct hwblk_info {
	struct hwblk_area *areas;
	int nr_areas;
	struct hwblk *hwblks;
	int nr_hwblks;
};

/* Should be defined by processor-specific code */
int arch_hwblk_init(void);
int arch_hwblk_sleep_mode(void);

int hwblk_register(struct hwblk_info *info);
int hwblk_init(void);

void hwblk_enable(struct hwblk_info *info, int hwblk);
void hwblk_disable(struct hwblk_info *info, int hwblk);

void hwblk_cnt_inc(struct hwblk_info *info, int hwblk, int cnt);
void hwblk_cnt_dec(struct hwblk_info *info, int hwblk, int cnt);

/* allow clocks to enable and disable hardware blocks */
#define SH_HWBLK_CLK(_hwblk, _parent, _flags)	\
[_hwblk] = {					\
	.parent		= _parent,		\
	.arch_flags	= _hwblk,		\
	.flags		= _flags,		\
}

int sh_hwblk_clk_register(struct clk *clks, int nr);

#endif /* __ASM_SH_HWBLK_H */
