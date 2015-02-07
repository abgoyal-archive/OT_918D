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
 * Copyright (C) 2008 Maarten Maathuis.
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE COPYRIGHT OWNER(S) AND/OR ITS SUPPLIERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#include "drmP.h"
#include "drm_mode.h"

#define NOUVEAU_DMA_DEBUG (nouveau_reg_debug & NOUVEAU_REG_DEBUG_EVO)
#include "nouveau_reg.h"
#include "nouveau_drv.h"
#include "nouveau_crtc.h"
#include "nv50_display.h"

static void
nv50_cursor_show(struct nouveau_crtc *nv_crtc, bool update)
{
	struct drm_nouveau_private *dev_priv = nv_crtc->base.dev->dev_private;
	struct nouveau_channel *evo = dev_priv->evo;
	struct drm_device *dev = nv_crtc->base.dev;
	int ret;

	NV_DEBUG_KMS(dev, "\n");

	if (update && nv_crtc->cursor.visible)
		return;

	ret = RING_SPACE(evo, (dev_priv->chipset != 0x50 ? 5 : 3) + update * 2);
	if (ret) {
		NV_ERROR(dev, "no space while unhiding cursor\n");
		return;
	}

	if (dev_priv->chipset != 0x50) {
		BEGIN_RING(evo, 0, NV84_EVO_CRTC(nv_crtc->index, CURSOR_DMA), 1);
		OUT_RING(evo, NvEvoVRAM);
	}
	BEGIN_RING(evo, 0, NV50_EVO_CRTC(nv_crtc->index, CURSOR_CTRL), 2);
	OUT_RING(evo, NV50_EVO_CRTC_CURSOR_CTRL_SHOW);
	OUT_RING(evo, nv_crtc->cursor.offset >> 8);

	if (update) {
		BEGIN_RING(evo, 0, NV50_EVO_UPDATE, 1);
		OUT_RING(evo, 0);
		FIRE_RING(evo);
		nv_crtc->cursor.visible = true;
	}
}

static void
nv50_cursor_hide(struct nouveau_crtc *nv_crtc, bool update)
{
	struct drm_nouveau_private *dev_priv = nv_crtc->base.dev->dev_private;
	struct nouveau_channel *evo = dev_priv->evo;
	struct drm_device *dev = nv_crtc->base.dev;
	int ret;

	NV_DEBUG_KMS(dev, "\n");

	if (update && !nv_crtc->cursor.visible)
		return;

	ret = RING_SPACE(evo, (dev_priv->chipset != 0x50 ? 5 : 3) + update * 2);
	if (ret) {
		NV_ERROR(dev, "no space while hiding cursor\n");
		return;
	}
	BEGIN_RING(evo, 0, NV50_EVO_CRTC(nv_crtc->index, CURSOR_CTRL), 2);
	OUT_RING(evo, NV50_EVO_CRTC_CURSOR_CTRL_HIDE);
	OUT_RING(evo, 0);
	if (dev_priv->chipset != 0x50) {
		BEGIN_RING(evo, 0, NV84_EVO_CRTC(nv_crtc->index, CURSOR_DMA), 1);
		OUT_RING(evo, NV84_EVO_CRTC_CURSOR_DMA_HANDLE_NONE);
	}

	if (update) {
		BEGIN_RING(evo, 0, NV50_EVO_UPDATE, 1);
		OUT_RING(evo, 0);
		FIRE_RING(evo);
		nv_crtc->cursor.visible = false;
	}
}

static void
nv50_cursor_set_pos(struct nouveau_crtc *nv_crtc, int x, int y)
{
	struct drm_device *dev = nv_crtc->base.dev;

	nv_crtc->cursor_saved_x = x; nv_crtc->cursor_saved_y = y;
	nv_wr32(dev, NV50_PDISPLAY_CURSOR_USER_POS(nv_crtc->index),
		((y & 0xFFFF) << 16) | (x & 0xFFFF));
	/* Needed to make the cursor move. */
	nv_wr32(dev, NV50_PDISPLAY_CURSOR_USER_POS_CTRL(nv_crtc->index), 0);
}

static void
nv50_cursor_set_offset(struct nouveau_crtc *nv_crtc, uint32_t offset)
{
	NV_DEBUG_KMS(nv_crtc->base.dev, "\n");
	if (offset == nv_crtc->cursor.offset)
		return;

	nv_crtc->cursor.offset = offset;
	if (nv_crtc->cursor.visible) {
		nv_crtc->cursor.visible = false;
		nv_crtc->cursor.show(nv_crtc, true);
	}
}

int
nv50_cursor_init(struct nouveau_crtc *nv_crtc)
{
	nv_crtc->cursor.set_offset = nv50_cursor_set_offset;
	nv_crtc->cursor.set_pos = nv50_cursor_set_pos;
	nv_crtc->cursor.hide = nv50_cursor_hide;
	nv_crtc->cursor.show = nv50_cursor_show;
	return 0;
}

void
nv50_cursor_fini(struct nouveau_crtc *nv_crtc)
{
	struct drm_device *dev = nv_crtc->base.dev;
	int idx = nv_crtc->index;

	NV_DEBUG_KMS(dev, "\n");

	nv_wr32(dev, NV50_PDISPLAY_CURSOR_CURSOR_CTRL2(idx), 0);
	if (!nv_wait(NV50_PDISPLAY_CURSOR_CURSOR_CTRL2(idx),
		     NV50_PDISPLAY_CURSOR_CURSOR_CTRL2_STATUS, 0)) {
		NV_ERROR(dev, "timeout: CURSOR_CTRL2_STATUS == 0\n");
		NV_ERROR(dev, "CURSOR_CTRL2 = 0x%08x\n",
			 nv_rd32(dev, NV50_PDISPLAY_CURSOR_CURSOR_CTRL2(idx)));
	}
}

