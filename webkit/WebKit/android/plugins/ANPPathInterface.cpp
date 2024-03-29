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
 * Copyright 2009, The Android Open Source Project
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

// must include config.h first for webkit to fiddle with new/delete
#include "config.h"
#include "SkANP.h"

static ANPPath* anp_newPath() {
    return new ANPPath;
}

static void anp_deletePath(ANPPath* path) {
    delete path;
}

static void anp_copy(ANPPath* dst, const ANPPath* src) {
    *dst = *src;
}

static bool anp_equal(const ANPPath* p0, const ANPPath* p1) {
    return *p0 == *p1;
}

static void anp_reset(ANPPath* path) {
    path->reset();
}

static bool anp_isEmpty(const ANPPath* path) {
    return path->isEmpty();
}

static void anp_getBounds(const ANPPath* path, ANPRectF* bounds) {
    SkANP::SetRect(bounds, path->getBounds());
}

static void anp_moveTo(ANPPath* path, float x, float y) {
    path->moveTo(SkFloatToScalar(x), SkFloatToScalar(y));
}

static void anp_lineTo(ANPPath* path, float x, float y) {
    path->lineTo(SkFloatToScalar(x), SkFloatToScalar(y));
}

static void anp_quadTo(ANPPath* path, float x0, float y0, float x1, float y1) {
    path->quadTo(SkFloatToScalar(x0), SkFloatToScalar(y0),
                 SkFloatToScalar(x1), SkFloatToScalar(y1));
}

static void anp_cubicTo(ANPPath* path, float x0, float y0,
                        float x1, float y1, float x2, float y2) {
    path->cubicTo(SkFloatToScalar(x0), SkFloatToScalar(y0),
                  SkFloatToScalar(x1), SkFloatToScalar(y1),
                  SkFloatToScalar(x2), SkFloatToScalar(y2));
}

static void anp_close(ANPPath* path) {
    path->close();
}

static void anp_offset(ANPPath* path, float dx, float dy, ANPPath* dst) {
    path->offset(SkFloatToScalar(dx), SkFloatToScalar(dy), dst);
}

static void anp_transform(ANPPath* src, const ANPMatrix* matrix,
                          ANPPath* dst) {
    src->transform(*matrix, dst);
}

///////////////////////////////////////////////////////////////////////////////

#define ASSIGN(obj, name)   (obj)->name = anp_##name

void ANPPathInterfaceV0_Init(ANPInterface* value) {
    ANPPathInterfaceV0* i = reinterpret_cast<ANPPathInterfaceV0*>(value);

    ASSIGN(i, newPath);
    ASSIGN(i, deletePath);
    ASSIGN(i, copy);
    ASSIGN(i, equal);
    ASSIGN(i, reset);
    ASSIGN(i, isEmpty);
    ASSIGN(i, getBounds);
    ASSIGN(i, moveTo);
    ASSIGN(i, lineTo);
    ASSIGN(i, quadTo);
    ASSIGN(i, cubicTo);
    ASSIGN(i, close);
    ASSIGN(i, offset);
    ASSIGN(i, transform);
}
