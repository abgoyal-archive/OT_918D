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
 * Copyright (C) 2006, 2007, 2009 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

#ifndef CanvasRenderingContext2D_h
#define CanvasRenderingContext2D_h

#include "AffineTransform.h"
#include "CanvasRenderingContext.h"
#include "FloatSize.h"
#include "Font.h"
#include "GraphicsTypes.h"
#include "Path.h"
#include "PlatformString.h"
#include <wtf/Vector.h>

#if PLATFORM(CG)
#include <ApplicationServices/ApplicationServices.h>
#endif

namespace WebCore {

    class CanvasGradient;
    class CanvasPattern;
    class CanvasStyle;
    class FloatRect;
    class GraphicsContext;
    class HTMLCanvasElement;
    class HTMLImageElement;
    class HTMLVideoElement;
    class ImageData;
    class KURL;
    class TextMetrics;

    typedef int ExceptionCode;

    class CanvasRenderingContext2D : public CanvasRenderingContext {
    public:
        CanvasRenderingContext2D(HTMLCanvasElement*);

        virtual ~CanvasRenderingContext2D();
        
        virtual bool is2d() const { return true; }

        CanvasStyle* strokeStyle() const;
        void setStrokeStyle(PassRefPtr<CanvasStyle>);

        CanvasStyle* fillStyle() const;
        void setFillStyle(PassRefPtr<CanvasStyle>);

        float lineWidth() const;
        void setLineWidth(float);

        String lineCap() const;
        void setLineCap(const String&);

        String lineJoin() const;
        void setLineJoin(const String&);

        float miterLimit() const;
        void setMiterLimit(float);

        float shadowOffsetX() const;
        void setShadowOffsetX(float);

        float shadowOffsetY() const;
        void setShadowOffsetY(float);

        float shadowBlur() const;
        void setShadowBlur(float);

        String shadowColor() const;
        void setShadowColor(const String&);

        float globalAlpha() const;
        void setGlobalAlpha(float);

        String globalCompositeOperation() const;
        void setGlobalCompositeOperation(const String&);

        void save();
        void restore();

        void scale(float sx, float sy);
        void rotate(float angleInRadians);
        void translate(float tx, float ty);
        void transform(float m11, float m12, float m21, float m22, float dx, float dy);
        void setTransform(float m11, float m12, float m21, float m22, float dx, float dy);

        void setStrokeColor(const String& color);
        void setStrokeColor(float grayLevel);
        void setStrokeColor(const String& color, float alpha);
        void setStrokeColor(float grayLevel, float alpha);
        void setStrokeColor(float r, float g, float b, float a);
        void setStrokeColor(float c, float m, float y, float k, float a);

        void setFillColor(const String& color);
        void setFillColor(float grayLevel);
        void setFillColor(const String& color, float alpha);
        void setFillColor(float grayLevel, float alpha);
        void setFillColor(float r, float g, float b, float a);
        void setFillColor(float c, float m, float y, float k, float a);

        void beginPath();
        void closePath();

        void moveTo(float x, float y);
        void lineTo(float x, float y);
        void quadraticCurveTo(float cpx, float cpy, float x, float y);
        void bezierCurveTo(float cp1x, float cp1y, float cp2x, float cp2y, float x, float y);
        void arcTo(float x0, float y0, float x1, float y1, float radius, ExceptionCode&);
        void arc(float x, float y, float r, float sa, float ea, bool clockwise, ExceptionCode&);
        void rect(float x, float y, float width, float height);

        void fill();
        void stroke();
        void clip();

        bool isPointInPath(const float x, const float y);

        void clearRect(float x, float y, float width, float height);
        void fillRect(float x, float y, float width, float height);
        void strokeRect(float x, float y, float width, float height);
        void strokeRect(float x, float y, float width, float height, float lineWidth);

        void setShadow(float width, float height, float blur);
        void setShadow(float width, float height, float blur, const String& color);
        void setShadow(float width, float height, float blur, float grayLevel);
        void setShadow(float width, float height, float blur, const String& color, float alpha);
        void setShadow(float width, float height, float blur, float grayLevel, float alpha);
        void setShadow(float width, float height, float blur, float r, float g, float b, float a);
        void setShadow(float width, float height, float blur, float c, float m, float y, float k, float a);

        void clearShadow();

        void drawImage(HTMLImageElement*, float x, float y);
        void drawImage(HTMLImageElement*, float x, float y, float width, float height, ExceptionCode&);
        void drawImage(HTMLImageElement*, const FloatRect& srcRect, const FloatRect& dstRect, ExceptionCode&);
        void drawImage(HTMLCanvasElement*, float x, float y);
        void drawImage(HTMLCanvasElement*, float x, float y, float width, float height, ExceptionCode&);
        void drawImage(HTMLCanvasElement*, const FloatRect& srcRect, const FloatRect& dstRect, ExceptionCode&);
#if ENABLE(VIDEO)
        void drawImage(HTMLVideoElement*, float x, float y);
        void drawImage(HTMLVideoElement*, float x, float y, float width, float height, ExceptionCode&);
        void drawImage(HTMLVideoElement*, const FloatRect& srcRect, const FloatRect& dstRect, ExceptionCode&);
#endif

        void drawImageFromRect(HTMLImageElement*, float sx, float sy, float sw, float sh,
            float dx, float dy, float dw, float dh, const String& compositeOperation);

        void setAlpha(float);

        void setCompositeOperation(const String&);

        PassRefPtr<CanvasGradient> createLinearGradient(float x0, float y0, float x1, float y1, ExceptionCode&);
        PassRefPtr<CanvasGradient> createRadialGradient(float x0, float y0, float r0, float x1, float y1, float r1, ExceptionCode&);
        PassRefPtr<CanvasPattern> createPattern(HTMLImageElement*, const String& repetitionType, ExceptionCode&);
        PassRefPtr<CanvasPattern> createPattern(HTMLCanvasElement*, const String& repetitionType, ExceptionCode&);
        
        PassRefPtr<ImageData> createImageData(float width, float height, ExceptionCode&) const;
        PassRefPtr<ImageData> getImageData(float sx, float sy, float sw, float sh, ExceptionCode&) const;
        void putImageData(ImageData*, float dx, float dy, ExceptionCode&);
        void putImageData(ImageData*, float dx, float dy, float dirtyX, float dirtyY, float dirtyWidth, float dirtyHeight, ExceptionCode&);
        
        void reset();

        String font() const;
        void setFont(const String&);
        
        String textAlign() const;
        void setTextAlign(const String&);
        
        String textBaseline() const;
        void setTextBaseline(const String&);
        
        void fillText(const String& text, float x, float y);
        void fillText(const String& text, float x, float y, float maxWidth);
        void strokeText(const String& text, float x, float y);
        void strokeText(const String& text, float x, float y, float maxWidth);
        PassRefPtr<TextMetrics> measureText(const String& text);

        LineCap getLineCap() const { return state().m_lineCap; }
        LineJoin getLineJoin() const { return state().m_lineJoin; }

    private:
        struct State {
            State();
            
            RefPtr<CanvasStyle> m_strokeStyle;
            RefPtr<CanvasStyle> m_fillStyle;
            float m_lineWidth;
            LineCap m_lineCap;
            LineJoin m_lineJoin;
            float m_miterLimit;
            FloatSize m_shadowOffset;
            float m_shadowBlur;
            String m_shadowColor;
            float m_globalAlpha;
            CompositeOperator m_globalComposite;
            AffineTransform m_transform;
            bool m_invertibleCTM;
            
            // Text state.
            TextAlign m_textAlign;
            TextBaseline m_textBaseline;
            
            String m_unparsedFont;
            Font m_font;
            bool m_realizedFont;
        };
        Path m_path;

        State& state() { return m_stateStack.last(); }
        const State& state() const { return m_stateStack.last(); }

        void applyShadow();

        enum CanvasWillDrawOption {
            CanvasWillDrawApplyTransform = 1,
            CanvasWillDrawApplyShadow = 1 << 1,
            CanvasWillDrawApplyClip = 1 << 2,
            CanvasWillDrawApplyAll = 0xffffffff
        };
        
        void willDraw(const FloatRect&, unsigned options = CanvasWillDrawApplyAll);

        GraphicsContext* drawingContext() const;

        void applyStrokePattern();
        void applyFillPattern();

        void drawTextInternal(const String& text, float x, float y, bool fill, float maxWidth = 0, bool useMaxWidth = false);

        const Font& accessFont();

#if ENABLE(DASHBOARD_SUPPORT)
        void clearPathForDashboardBackwardCompatibilityMode();
#endif
        
        void prepareGradientForDashboard(CanvasGradient* gradient) const;
        void checkOrigin(const KURL&);
        void checkOrigin(const String&);

        Vector<State, 1> m_stateStack;
    };

} // namespace WebCore

#endif