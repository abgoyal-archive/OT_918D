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
 * (C) 1999 Lars Knoll (knoll@kde.org)
 * (C) 2000 Dirk Mueller (mueller@kde.org)
 * Copyright (C) 2004, 2005, 2006, 2009 Apple Inc. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 */

#ifndef InlineTextBox_h
#define InlineTextBox_h

#include "InlineRunBox.h"
#include "RenderText.h" // so textRenderer() can be inline

namespace WebCore {

struct CompositionUnderline;

const unsigned short cNoTruncation = USHRT_MAX;
const unsigned short cFullTruncation = USHRT_MAX - 1;

// Helper functions shared by InlineTextBox / SVGRootInlineBox
void updateGraphicsContext(GraphicsContext*, const Color& fillColor, const Color& strokeColor, float strokeThickness, ColorSpace);
Color correctedTextColor(Color textColor, Color backgroundColor);

class InlineTextBox : public InlineRunBox {
public:
    InlineTextBox(RenderObject* obj)
        : InlineRunBox(obj)
        , m_start(0)
        , m_len(0)
        , m_truncation(cNoTruncation)
    {
    }

    InlineTextBox* nextTextBox() const { return static_cast<InlineTextBox*>(nextLineBox()); }
    InlineTextBox* prevTextBox() const { return static_cast<InlineTextBox*>(prevLineBox()); }

    unsigned start() const { return m_start; }
    unsigned end() const { return m_len ? m_start + m_len - 1 : m_start; }
    unsigned len() const { return m_len; }

    void setStart(unsigned start) { m_start = start; }
    void setLen(unsigned len) { m_len = len; }

    void offsetRun(int d) { m_start += d; }

    void setFallbackFonts(const HashSet<const SimpleFontData*>&);
    void takeFallbackFonts(Vector<const SimpleFontData*>&);

    unsigned short truncation() { return m_truncation; }

private:
    virtual int selectionTop();
    virtual int selectionHeight();

public:
    virtual IntRect selectionRect(int absx, int absy, int startPos, int endPos);
    bool isSelected(int startPos, int endPos) const;
    void selectionStartEnd(int& sPos, int& ePos);

private:
    virtual void paint(RenderObject::PaintInfo&, int tx, int ty);
    virtual bool nodeAtPoint(const HitTestRequest&, HitTestResult&, int x, int y, int tx, int ty);

public:
    RenderText* textRenderer() const;

private:
    virtual void deleteLine(RenderArena*);
    virtual void extractLine();
    virtual void attachLine();

public:
    virtual RenderObject::SelectionState selectionState();

private:
    virtual void clearTruncation() { m_truncation = cNoTruncation; }
    virtual int placeEllipsisBox(bool flowIsLTR, int visibleLeftEdge, int visibleRightEdge, int ellipsisWidth, bool& foundBox);

public:
    virtual bool isLineBreak() const;

    void setSpaceAdd(int add) { m_width -= m_toAdd; m_toAdd = add; m_width += m_toAdd; }

private:
    virtual bool isInlineTextBox() { return true; }    

public:
    virtual int caretMinOffset() const;
    virtual int caretMaxOffset() const;

private:
    virtual unsigned caretMaxRenderedOffset() const;

    int textPos() const;

public:
    virtual int offsetForPosition(int x, bool includePartialGlyphs = true) const;
    virtual int positionForOffset(int offset) const;

    bool containsCaretOffset(int offset) const; // false for offset after line break

private:
    int m_start;
    unsigned short m_len;

    unsigned short m_truncation; // Where to truncate when text overflow is applied.  We use special constants to
                      // denote no truncation (the whole run paints) and full truncation (nothing paints at all).

protected:
    void paintCompositionBackground(GraphicsContext*, int tx, int ty, RenderStyle*, const Font&, int startPos, int endPos);
    void paintDocumentMarkers(GraphicsContext*, int tx, int ty, RenderStyle*, const Font&, bool background);
    void paintCompositionUnderline(GraphicsContext*, int tx, int ty, const CompositionUnderline&);
#if PLATFORM(MAC)
    void paintCustomHighlight(int tx, int ty, const AtomicString& type);
#endif

private:
    void paintDecoration(GraphicsContext*, int tx, int ty, int decoration, ShadowData*);
    void paintSelection(GraphicsContext*, int tx, int ty, RenderStyle*, const Font&);
    void paintSpellingOrGrammarMarker(GraphicsContext*, int tx, int ty, const DocumentMarker&, RenderStyle*, const Font&, bool grammar);
    void paintTextMatchMarker(GraphicsContext*, int tx, int ty, const DocumentMarker&, RenderStyle*, const Font&);
    void computeRectForReplacementMarker(int tx, int ty, const DocumentMarker&, RenderStyle*, const Font&);
};

inline RenderText* InlineTextBox::textRenderer() const
{
    return toRenderText(renderer());
}

} // namespace WebCore

#endif // InlineTextBox_h
