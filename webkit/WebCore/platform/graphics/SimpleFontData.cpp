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
 * Copyright (C) 2005, 2008 Apple Computer, Inc.  All rights reserved.
 * Copyright (C) 2006 Alexey Proskuryakov
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer. 
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution. 
 * 3.  Neither the name of Apple Computer, Inc. ("Apple") nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "SimpleFontData.h"

#include "Font.h"
#include "FontCache.h"

#if ENABLE(SVG_FONTS)
#include "SVGFontData.h"
#include "SVGFontElement.h"
#include "SVGFontFaceElement.h"
#include "SVGGlyphElement.h"
#endif

#include <wtf/MathExtras.h>
#include <wtf/UnusedParam.h>

using namespace std;

namespace WebCore {

SimpleFontData::SimpleFontData(const FontPlatformData& f, bool customFont, bool loading, SVGFontData* svgFontData)
    : m_maxCharWidth(-1)
    , m_avgCharWidth(-1)
    , m_unitsPerEm(defaultUnitsPerEm)
    , m_platformData(f)
    , m_treatAsFixedPitch(false)
#if ENABLE(SVG_FONTS)
    , m_svgFontData(svgFontData)
#endif
    , m_isCustomFont(customFont)
    , m_isLoading(loading)
    , m_smallCapsFontData(0)
{
#if !ENABLE(SVG_FONTS)
    UNUSED_PARAM(svgFontData);
#else
    if (SVGFontFaceElement* svgFontFaceElement = svgFontData ? svgFontData->svgFontFaceElement() : 0) {
        m_unitsPerEm = svgFontFaceElement->unitsPerEm();

        double scale = f.size();
        if (m_unitsPerEm)
            scale /= m_unitsPerEm;

        m_ascent = static_cast<int>(svgFontFaceElement->ascent() * scale);
        m_descent = static_cast<int>(svgFontFaceElement->descent() * scale);
        m_xHeight = static_cast<int>(svgFontFaceElement->xHeight() * scale);
        m_lineGap = 0.1f * f.size();
        m_lineSpacing = m_ascent + m_descent + m_lineGap;

        SVGFontElement* associatedFontElement = svgFontFaceElement->associatedFontElement();

        Vector<SVGGlyphIdentifier> spaceGlyphs;
        associatedFontElement->getGlyphIdentifiersForString(String(" ", 1), spaceGlyphs);
        m_spaceWidth = spaceGlyphs.isEmpty() ? m_xHeight : static_cast<float>(spaceGlyphs.first().horizontalAdvanceX * scale);

        Vector<SVGGlyphIdentifier> numeralZeroGlyphs;
        associatedFontElement->getGlyphIdentifiersForString(String("0", 1), numeralZeroGlyphs);
        m_avgCharWidth = numeralZeroGlyphs.isEmpty() ? m_spaceWidth : static_cast<float>(numeralZeroGlyphs.first().horizontalAdvanceX * scale);

        Vector<SVGGlyphIdentifier> letterWGlyphs;
        associatedFontElement->getGlyphIdentifiersForString(String("W", 1), letterWGlyphs);
        m_maxCharWidth = letterWGlyphs.isEmpty() ? m_ascent : static_cast<float>(letterWGlyphs.first().horizontalAdvanceX * scale);

        // FIXME: is there a way we can get the space glyph from the SVGGlyphIdentifier above?
        m_spaceGlyph = 0;
        determinePitch();
        m_adjustedSpaceWidth = roundf(m_spaceWidth);
        m_missingGlyphData.fontData = this;
        m_missingGlyphData.glyph = 0;
        return;
    }
#endif

    platformInit();
    platformGlyphInit();
    platformCharWidthInit();
}

#if !PLATFORM(QT)
// Estimates of avgCharWidth and maxCharWidth for platforms that don't support accessing these values from the font.
void SimpleFontData::initCharWidths()
{
    GlyphPage* glyphPageZero = GlyphPageTreeNode::getRootChild(this, 0)->page();

    // Treat the width of a '0' as the avgCharWidth.
    if (m_avgCharWidth <= 0.f && glyphPageZero) {
        static const UChar32 digitZeroChar = '0';
        Glyph digitZeroGlyph = glyphPageZero->glyphDataForCharacter(digitZeroChar).glyph;
        if (digitZeroGlyph)
            m_avgCharWidth = widthForGlyph(digitZeroGlyph);
    }

    // If we can't retrieve the width of a '0', fall back to the x height.
    if (m_avgCharWidth <= 0.f)
        m_avgCharWidth = m_xHeight;

    if (m_maxCharWidth <= 0.f)
        m_maxCharWidth = max<float>(m_avgCharWidth, m_ascent);
}

void SimpleFontData::platformGlyphInit()
{
    GlyphPage* glyphPageZero = GlyphPageTreeNode::getRootChild(this, 0)->page();
    if (!glyphPageZero) {
        LOG_ERROR("Failed to get glyph page zero.");
        m_spaceGlyph = 0;
        m_spaceWidth = 0;
        m_adjustedSpaceWidth = 0;
        determinePitch();
        m_missingGlyphData.fontData = this;
        m_missingGlyphData.glyph = 0;
        return;
    }

    // Nasty hack to determine if we should round or ceil space widths.
    // If the font is monospace or fake monospace we ceil to ensure that 
    // every character and the space are the same width.  Otherwise we round.
    m_spaceGlyph = glyphPageZero->glyphDataForCharacter(' ').glyph;
    float width = widthForGlyph(m_spaceGlyph);
    m_spaceWidth = width;
    determinePitch();
    m_adjustedSpaceWidth = m_treatAsFixedPitch ? ceilf(width) : roundf(width);

    // Force the glyph for ZERO WIDTH SPACE to have zero width, unless it is shared with SPACE.
    // Helvetica is an example of a non-zero width ZERO WIDTH SPACE glyph.
    // See <http://bugs.webkit.org/show_bug.cgi?id=13178>
    // Ask for the glyph for 0 to avoid paging in ZERO WIDTH SPACE. Control characters, including 0,
    // are mapped to the ZERO WIDTH SPACE glyph.
    Glyph zeroWidthSpaceGlyph = glyphPageZero->glyphDataForCharacter(0).glyph;
    if (zeroWidthSpaceGlyph) {
        if (zeroWidthSpaceGlyph != m_spaceGlyph)
            m_glyphToWidthMap.setWidthForGlyph(zeroWidthSpaceGlyph, 0);
        else
            LOG_ERROR("Font maps SPACE and ZERO WIDTH SPACE to the same glyph. Glyph width not overridden.");
    }

    m_missingGlyphData.fontData = this;
    m_missingGlyphData.glyph = 0;
}
#endif

SimpleFontData::~SimpleFontData()
{
#if ENABLE(SVG_FONTS)
    if (!m_svgFontData || !m_svgFontData->svgFontFaceElement())
#endif
        platformDestroy();

    if (!isCustomFont()) {
        if (m_smallCapsFontData)
            fontCache()->releaseFontData(m_smallCapsFontData);
        GlyphPageTreeNode::pruneTreeFontData(this);
    }
}

const SimpleFontData* SimpleFontData::fontDataForCharacter(UChar32) const
{
    return this;
}

bool SimpleFontData::isSegmented() const
{
    return false;
}

#ifndef NDEBUG
String SimpleFontData::description() const
{
    if (isSVGFont())
        return "[SVG font]";
    if (isCustomFont())
        return "[custom font]";

    return platformData().description();
}
#endif

} // namespace WebCore
