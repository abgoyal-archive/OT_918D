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

#ifndef __NOUVEAU_GRCTX_H__
#define __NOUVEAU_GRCTX_H__

struct nouveau_grctx {
	struct drm_device *dev;

	enum {
		NOUVEAU_GRCTX_PROG,
		NOUVEAU_GRCTX_VALS
	} mode;
	void *data;

	uint32_t ctxprog_max;
	uint32_t ctxprog_len;
	uint32_t ctxprog_reg;
	int      ctxprog_label[32];
	uint32_t ctxvals_pos;
	uint32_t ctxvals_base;
};

#ifdef CP_CTX
static inline void
cp_out(struct nouveau_grctx *ctx, uint32_t inst)
{
	uint32_t *ctxprog = ctx->data;

	if (ctx->mode != NOUVEAU_GRCTX_PROG)
		return;

	BUG_ON(ctx->ctxprog_len == ctx->ctxprog_max);
	ctxprog[ctx->ctxprog_len++] = inst;
}

static inline void
cp_lsr(struct nouveau_grctx *ctx, uint32_t val)
{
	cp_out(ctx, CP_LOAD_SR | val);
}

static inline void
cp_ctx(struct nouveau_grctx *ctx, uint32_t reg, uint32_t length)
{
	ctx->ctxprog_reg = (reg - 0x00400000) >> 2;

	ctx->ctxvals_base = ctx->ctxvals_pos;
	ctx->ctxvals_pos = ctx->ctxvals_base + length;

	if (length > (CP_CTX_COUNT >> CP_CTX_COUNT_SHIFT)) {
		cp_lsr(ctx, length);
		length = 0;
	}

	cp_out(ctx, CP_CTX | (length << CP_CTX_COUNT_SHIFT) | ctx->ctxprog_reg);
}

static inline void
cp_name(struct nouveau_grctx *ctx, int name)
{
	uint32_t *ctxprog = ctx->data;
	int i;

	if (ctx->mode != NOUVEAU_GRCTX_PROG)
		return;

	ctx->ctxprog_label[name] = ctx->ctxprog_len;
	for (i = 0; i < ctx->ctxprog_len; i++) {
		if ((ctxprog[i] & 0xfff00000) != 0xff400000)
			continue;
		if ((ctxprog[i] & CP_BRA_IP) != ((name) << CP_BRA_IP_SHIFT))
			continue;
		ctxprog[i] = (ctxprog[i] & 0x00ff00ff) |
			     (ctx->ctxprog_len << CP_BRA_IP_SHIFT);
	}
}

static inline void
_cp_bra(struct nouveau_grctx *ctx, u32 mod, int flag, int state, int name)
{
	int ip = 0;

	if (mod != 2) {
		ip = ctx->ctxprog_label[name] << CP_BRA_IP_SHIFT;
		if (ip == 0)
			ip = 0xff000000 | (name << CP_BRA_IP_SHIFT);
	}

	cp_out(ctx, CP_BRA | (mod << 18) | ip | flag |
		    (state ? 0 : CP_BRA_IF_CLEAR));
}
#define cp_bra(c,f,s,n) _cp_bra((c), 0, CP_FLAG_##f, CP_FLAG_##f##_##s, n)
#ifdef CP_BRA_MOD
#define cp_cal(c,f,s,n) _cp_bra((c), 1, CP_FLAG_##f, CP_FLAG_##f##_##s, n)
#define cp_ret(c,f,s) _cp_bra((c), 2, CP_FLAG_##f, CP_FLAG_##f##_##s, 0)
#endif

static inline void
_cp_wait(struct nouveau_grctx *ctx, int flag, int state)
{
	cp_out(ctx, CP_WAIT | flag | (state ? CP_WAIT_SET : 0));
}
#define cp_wait(c,f,s) _cp_wait((c), CP_FLAG_##f, CP_FLAG_##f##_##s)

static inline void
_cp_set(struct nouveau_grctx *ctx, int flag, int state)
{
	cp_out(ctx, CP_SET | flag | (state ? CP_SET_1 : 0));
}
#define cp_set(c,f,s) _cp_set((c), CP_FLAG_##f, CP_FLAG_##f##_##s)

static inline void
cp_pos(struct nouveau_grctx *ctx, int offset)
{
	ctx->ctxvals_pos = offset;
	ctx->ctxvals_base = ctx->ctxvals_pos;

	cp_lsr(ctx, ctx->ctxvals_pos);
	cp_out(ctx, CP_SET_CONTEXT_POINTER);
}

static inline void
gr_def(struct nouveau_grctx *ctx, uint32_t reg, uint32_t val)
{
	if (ctx->mode != NOUVEAU_GRCTX_VALS)
		return;

	reg = (reg - 0x00400000) / 4;
	reg = (reg - ctx->ctxprog_reg) + ctx->ctxvals_base;

	nv_wo32(ctx->dev, ctx->data, reg, val);
}
#endif

#endif
