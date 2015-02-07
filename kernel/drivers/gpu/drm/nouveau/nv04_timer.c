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

#include "drmP.h"
#include "drm.h"
#include "nouveau_drv.h"
#include "nouveau_drm.h"

int
nv04_timer_init(struct drm_device *dev)
{
	nv_wr32(dev, NV04_PTIMER_INTR_EN_0, 0x00000000);
	nv_wr32(dev, NV04_PTIMER_INTR_0, 0xFFFFFFFF);

	/* Just use the pre-existing values when possible for now; these regs
	 * are not written in nv (driver writer missed a /4 on the address), and
	 * writing 8 and 3 to the correct regs breaks the timings on the LVDS
	 * hardware sequencing microcode.
	 * A correct solution (involving calculations with the GPU PLL) can
	 * be done when kernel modesetting lands
	 */
	if (!nv_rd32(dev, NV04_PTIMER_NUMERATOR) ||
				!nv_rd32(dev, NV04_PTIMER_DENOMINATOR)) {
		nv_wr32(dev, NV04_PTIMER_NUMERATOR, 0x00000008);
		nv_wr32(dev, NV04_PTIMER_DENOMINATOR, 0x00000003);
	}

	return 0;
}

uint64_t
nv04_timer_read(struct drm_device *dev)
{
	uint32_t low;
	/* From kmmio dumps on nv28 this looks like how the blob does this.
	 * It reads the high dword twice, before and after.
	 * The only explanation seems to be that the 64-bit timer counter
	 * advances between high and low dword reads and may corrupt the
	 * result. Not confirmed.
	 */
	uint32_t high2 = nv_rd32(dev, NV04_PTIMER_TIME_1);
	uint32_t high1;
	do {
		high1 = high2;
		low = nv_rd32(dev, NV04_PTIMER_TIME_0);
		high2 = nv_rd32(dev, NV04_PTIMER_TIME_1);
	} while (high1 != high2);
	return (((uint64_t)high2) << 32) | (uint64_t)low;
}

void
nv04_timer_takedown(struct drm_device *dev)
{
}
