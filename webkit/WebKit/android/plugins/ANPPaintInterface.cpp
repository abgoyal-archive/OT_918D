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
 * Copyright 2008, The Android Open Source Project
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
#include "SkTypeface.h"

static ANPPaint* anp_newPaint() {
    return new ANPPaint;
}

static void anp_deletePaint(ANPPaint* paint) {
    delete paint;
}

static ANPPaintFlags anp_getFlags(const ANPPaint* paint) {
    return paint->getFlags();
}

static void anp_setFlags(ANPPaint* paint, ANPPaintFlags flags) {
    paint->setFlags(flags);
}

static ANPColor anp_getColor(const ANPPaint* paint) {
    return paint->getColor();
}

static void anp_setColor(ANPPaint* paint, ANPColor color) {
    paint->setColor(color);
}

static ANPPaintStyle anp_getStyle(const ANPPaint* paint) {
    return paint->getStyle();
}

static void anp_setStyle(ANPPaint* paint, ANPPaintStyle style) {
    paint->setStyle(static_cast<SkPaint::Style>(style));
}

static float anp_getStrokeWidth(const ANPPaint* paint) {
    return SkScalarToFloat(paint->getStrokeWidth());
}

static float anp_getStrokeMiter(const ANPPaint* paint) {
    return SkScalarToFloat(paint->getStrokeMiter());
}

static ANPPaintCap anp_getStrokeCap(const ANPPaint* paint) {
    return paint->getStrokeCap();
}

static ANPPaintJoin anp_getStrokeJoin(const ANPPaint* paint) {
    return paint->getStrokeJoin();
}

static void anp_setStrokeWidth(ANPPaint* paint, float width) {
    paint->setStrokeWidth(SkFloatToScalar(width));
}

static void anp_setStrokeMiter(ANPPaint* paint, float miter) {
    paint->setStrokeMiter(SkFloatToScalar(miter));
}

static void anp_setStrokeCap(ANPPaint* paint, ANPPaintCap cap) {
    paint->setStrokeCap(static_cast<SkPaint::Cap>(cap));
}

static void anp_setStrokeJoin(ANPPaint* paint, ANPPaintJoin join) {
    paint->setStrokeJoin(static_cast<SkPaint::Join>(join));
}

static ANPTextEncoding anp_getTextEncoding(const ANPPaint* paint) {
    return paint->getTextEncoding();
}

static ANPPaintAlign anp_getTextAlign(const ANPPaint* paint) {
    return paint->getTextAlign();
}

static float anp_getTextSize(const ANPPaint* paint) {
    return SkScalarToFloat(paint->getTextSize());
}

static float anp_getTextScaleX(const ANPPaint* paint) {
    return SkScalarToFloat(paint->getTextScaleX());
}

static float anp_getTextSkewX(const ANPPaint* paint) {
    return SkScalarToFloat(paint->getTextSkewX());
}

static ANPTypeface* anp_getTypeface(const ANPPaint* paint) {
    return reinterpret_cast<ANPTypeface*>(paint->getTypeface());
}

static void anp_setTextEncoding(ANPPaint* paint, ANPTextEncoding encoding) {
    paint->setTextEncoding(static_cast<SkPaint::TextEncoding>(encoding));
}

static void anp_setTextAlign(ANPPaint* paint, ANPPaintAlign align) {
    paint->setTextAlign(static_cast<SkPaint::Align>(align));
}

static void anp_setTextSize(ANPPaint* paint, float textSize) {
    paint->setTextSize(SkFloatToScalar(textSize));
}

static void anp_setTextScaleX(ANPPaint* paint, float scaleX) {
    paint->setTextScaleX(SkFloatToScalar(scaleX));
}

static void anp_setTextSkewX(ANPPaint* paint, float skewX) {
    paint->setTextSkewX(SkFloatToScalar(skewX));
}

static void anp_setTypeface(ANPPaint* paint, ANPTypeface* tf) {
    paint->setTypeface(tf);
}

static float anp_measureText(ANPPaint* paint, const void* text,
                             uint32_t byteLength, ANPRectF* bounds) {
    SkScalar w = paint->measureText(text, byteLength,
                                    reinterpret_cast<SkRect*>(bounds));
    return SkScalarToFloat(w);
}

/** Return the number of unichars specifed by the text.
 If widths is not null, returns the array of advance widths for each
 unichar.
 If bounds is not null, returns the array of bounds for each unichar.
 */
static int anp_getTextWidths(ANPPaint* paint, const void* text,
                       uint32_t byteLength, float widths[], ANPRectF bounds[]) {
    return paint->getTextWidths(text, byteLength, widths,
                                reinterpret_cast<SkRect*>(bounds));
}

static float anp_getFontMetrics(ANPPaint* paint, ANPFontMetrics* metrics) {
    SkPaint::FontMetrics fm;
    SkScalar spacing = paint->getFontMetrics(&fm);
    if (metrics) {
        metrics->fTop = SkScalarToFloat(fm.fTop);
        metrics->fAscent = SkScalarToFloat(fm.fAscent);
        metrics->fDescent = SkScalarToFloat(fm.fDescent);
        metrics->fBottom = SkScalarToFloat(fm.fBottom);
        metrics->fLeading = SkScalarToFloat(fm.fLeading);
    }
    return SkScalarToFloat(spacing);
}

///////////////////////////////////////////////////////////////////////////////

#define ASSIGN(obj, name)   (obj)->name = anp_##name

void ANPPaintInterfaceV0_Init(ANPInterface* value) {
    ANPPaintInterfaceV0* i = reinterpret_cast<ANPPaintInterfaceV0*>(value);
    
    ASSIGN(i, newPaint);
    ASSIGN(i, deletePaint);
    ASSIGN(i, getFlags);
    ASSIGN(i, setFlags);
    ASSIGN(i, getColor);
    ASSIGN(i, setColor);
    ASSIGN(i, getStyle);
    ASSIGN(i, setStyle);
    ASSIGN(i, getStrokeWidth);
    ASSIGN(i, getStrokeMiter);
    ASSIGN(i, getStrokeCap);
    ASSIGN(i, getStrokeJoin);
    ASSIGN(i, setStrokeWidth);
    ASSIGN(i, setStrokeMiter);
    ASSIGN(i, setStrokeCap);
    ASSIGN(i, setStrokeJoin);
    ASSIGN(i, getTextEncoding);
    ASSIGN(i, getTextAlign);
    ASSIGN(i, getTextSize);
    ASSIGN(i, getTextScaleX);
    ASSIGN(i, getTextSkewX);
    ASSIGN(i, getTypeface);
    ASSIGN(i, setTextEncoding);
    ASSIGN(i, setTextAlign);
    ASSIGN(i, setTextSize);
    ASSIGN(i, setTextScaleX);
    ASSIGN(i, setTextSkewX);
    ASSIGN(i, setTypeface);
    ASSIGN(i, measureText);
    ASSIGN(i, getTextWidths);
    ASSIGN(i, getFontMetrics);
}
