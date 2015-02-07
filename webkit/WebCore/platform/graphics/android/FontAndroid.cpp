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
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */

/*
 * Copyright 2009, The Android Open Source Project
 * Copyright (C) 2006 Apple Computer, Inc.  All rights reserved.
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

#include "config.h"

#include "EmojiFont.h"
#include "Font.h"
#include "FontData.h"
#include "FontFallbackList.h"
#include "GraphicsContext.h"
#include "GlyphBuffer.h"
#include "IntRect.h"
#include "PlatformGraphicsContext.h"
#include "SkCanvas.h"
#include "SkLayerDrawLooper.h"
#include "SkPaint.h"
#include "SkTemplates.h"
#include "SkTypeface.h"
#include "SkUtils.h"

using namespace android;

namespace WebCore {

static void updateForFont(SkPaint* paint, const SimpleFontData* font) {
    font->platformData().setupPaint(paint);
    paint->setTextEncoding(SkPaint::kGlyphID_TextEncoding);
}
    
static SkPaint* setupFill(SkPaint* paint, GraphicsContext* gc,
                          const SimpleFontData* font) {
    gc->setupFillPaint(paint);
    updateForFont(paint, font);
    return paint;
}
    
static SkPaint* setupStroke(SkPaint* paint, GraphicsContext* gc,
                            const SimpleFontData* font) {
    gc->setupStrokePaint(paint);
    updateForFont(paint, font);
    return paint;
}
    
static bool setupForText(SkPaint* paint, GraphicsContext* gc,
                         const SimpleFontData* font) {
    int mode = gc->textDrawingMode();
    
    if ((mode & (cTextFill | cTextStroke)) == (cTextFill | cTextStroke)) {
        SkLayerDrawLooper* looper = new SkLayerDrawLooper;
        paint->setLooper(looper)->unref();
        
        // we clear the looper, in case we have a shadow

        SkPaint* fillP = NULL;
        SkPaint* strokeP = NULL;
        if (gc->willStroke()) {
            strokeP = setupStroke(looper->addLayer(), gc, font);
            strokeP->setLooper(NULL);
        }
        if (gc->willFill()) {
            fillP = setupFill(looper->addLayer(), gc, font);
            fillP->setLooper(NULL);
        }

        SkPaint shadowPaint;
        SkPoint offset;
        if (gc->setupShadowPaint(&shadowPaint, &offset)) {
            SkPaint* p = looper->addLayer(offset.fX, offset.fY);
            *p = shadowPaint;
            if (strokeP && !fillP) {
                // stroke the shadow if we have stroke but no fill
                p->setStyle(SkPaint::kStroke_Style);
                p->setStrokeWidth(strokeP->getStrokeWidth());
            }
            updateForFont(p, font);
        }
    } else if (mode & cTextFill) {
        (void)setupFill(paint, gc, font);
    } else if (mode & cTextStroke) {
        (void)setupStroke(paint, gc, font);
    } else {
        return false;
    }
    return true;
}

bool Font::canReturnFallbackFontsForComplexText()
{
    return false;
}

void Font::drawGlyphs(GraphicsContext* gc, const SimpleFontData* font,
                      const GlyphBuffer& glyphBuffer,  int from, int numGlyphs,
                      const FloatPoint& point) const
{
    // compile-time assert
    SkASSERT(sizeof(GlyphBufferGlyph) == sizeof(uint16_t));

    SkPaint paint;
    if (!setupForText(&paint, gc, font)) {
        return;
    }
    
    SkScalar                    x = SkFloatToScalar(point.x());
    SkScalar                    y = SkFloatToScalar(point.y());
    const GlyphBufferGlyph*     glyphs = glyphBuffer.glyphs(from);
    const GlyphBufferAdvance*   adv = glyphBuffer.advances(from);
    SkAutoSTMalloc<32, SkPoint> storage(numGlyphs);
    SkPoint*                    pos = storage.get();
    
    SkCanvas* canvas = gc->platformContext()->mCanvas;

    /*  We need an array of [x,y,x,y,x,y,...], but webkit is giving us
        point.xy + [width, height, width, height, ...], so we have to convert
     */

    if (EmojiFont::IsAvailable()) {
        // set filtering, to make scaled images look nice(r)
        paint.setFilterBitmap(true);
        
        int localIndex = 0;
        int localCount = 0;
        for (int i = 0; i < numGlyphs; i++) {
            if (EmojiFont::IsEmojiGlyph(glyphs[i])) {
                if (localCount)
                    canvas->drawPosText(&glyphs[localIndex],
                                        localCount * sizeof(uint16_t),
                                        &pos[localIndex], paint);
                EmojiFont::Draw(canvas, glyphs[i], x, y, paint);
                // reset local index/count track for "real" glyphs
                localCount = 0;
                localIndex = i + 1;
            } else {
                pos[i].set(x, y);
                localCount += 1;
            }
            x += SkFloatToScalar(adv[i].width());
            y += SkFloatToScalar(adv[i].height());
        }
        // draw the last run of glyphs (if any)
        if (localCount)
            canvas->drawPosText(&glyphs[localIndex],
                                localCount * sizeof(uint16_t),
                                &pos[localIndex], paint);
    } else {
        for (int i = 0; i < numGlyphs; i++) {
            pos[i].set(x, y);
            x += SkFloatToScalar(adv[i].width());
            y += SkFloatToScalar(adv[i].height());
        }
        canvas->drawPosText(glyphs, numGlyphs * sizeof(uint16_t), pos, paint);
    }
}

FloatRect Font::selectionRectForComplexText(const TextRun& run,
                                const IntPoint& point, int h, int from, int to) const
{
    SkPaint              paint;
    SkScalar             width, left;
    SkPaint::FontMetrics metrics;

    primaryFont()->platformData().setupPaint(&paint);

    SkScalar spacing = paint.getFontMetrics(&metrics);

    float beforeWidth = SkScalarToFloat(paint.measureText(run.characters(), from << 1));
    float afterWidth = SkScalarToFloat(paint.measureText(run.characters(), to << 1));

    if (run.rtl()) {
    	float totalWidth = SkScalarToFloat(paint.measureText(run.characters(), run.length() << 1));
    	return FloatRect(point.x() + floorf(totalWidth - afterWidth),
    	                 point.y(),
                         roundf(totalWidth - beforeWidth) - floorf(totalWidth - afterWidth),
    	                 //roundf(SkScalarToFloat(spacing))); // Don't compute rect height, but use h.
                         h);
    } else {
        return FloatRect(point.x() + floorf(beforeWidth),
                         point.y(),
                         roundf(afterWidth) - floorf(beforeWidth),
                         //roundf(SkScalarToFloat(spacing))); // Don't compute rect height, but use h.
                         h);
    }
}

void Font::drawComplexText(GraphicsContext* gc, TextRun const& run,
                           FloatPoint const& point, int, int) const
{
    SkCanvas*   canvas = gc->platformContext()->mCanvas;
    SkPaint     paint;

    if (!setupForText(&paint, gc, primaryFont())) {
        return;
    }

    // go to chars, instead of glyphs, which was set by setupForText()
    paint.setTextEncoding(SkPaint::kUTF16_TextEncoding);

    canvas->drawText(run.characters(), run.length() << 1,
                     SkFloatToScalar(point.x()), SkFloatToScalar(point.y()),
                     paint);
}

float Font::floatWidthForComplexText(const TextRun& run, HashSet<const SimpleFontData*>*) const
{
    SkPaint paint;

    primaryFont()->platformData().setupPaint(&paint);

//printf("--------- complext measure %d chars\n", run.to() - run.from());

    SkScalar width = paint.measureText(run.characters(), run.length() << 1);
    return SkScalarToFloat(width);
}

int Font::offsetForPositionForComplexText(const TextRun& run, int x,
                                          bool includePartialGlyphs) const
{
    SkPaint                         paint;
    int                             count = run.length();
    SkAutoSTMalloc<64, SkScalar>    storage(count);
    SkScalar*                       widths = storage.get();
    SkAutoSTMalloc<64, int>    storageClusterIndex(count);
    int*                       clusterIndex = storageClusterIndex.get();

    primaryFont()->platformData().setupPaint(&paint);

    //count = paint.getTextWidths(run.characters(), count << 1, widths);
    count = paint.getComplexTextWidths(run.characters(), count << 1, widths, clusterIndex);

    if (count > 0)
    {
        SkScalar pos = 0;
        for (int i = 0; i < count; i++)
        {
            if (x < SkScalarRound(pos + SkScalarHalf(widths[i])))
                return clusterIndex[i];
            pos += widths[i];
        }
    }
    return run.length();
}

}
