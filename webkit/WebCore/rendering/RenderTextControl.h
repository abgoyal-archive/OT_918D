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
 * Copyright (C) 2006, 2007 Apple Inc. All rights reserved.
 *           (C) 2008 Torch Mobile Inc. All rights reserved. (http://www.torchmobile.com/) 
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

#ifndef RenderTextControl_h
#define RenderTextControl_h

#include "RenderBlock.h"

namespace WebCore {

class VisibleSelection;
class TextControlInnerElement;
class TextControlInnerTextElement;

class RenderTextControl : public RenderBlock {
public:
    virtual ~RenderTextControl();

    bool wasChangedSinceLastChangeEvent() const { return m_wasChangedSinceLastChangeEvent; }
    void setChangedSinceLastChangeEvent(bool wasChangedSinceLastChangeEvent) { m_wasChangedSinceLastChangeEvent = wasChangedSinceLastChangeEvent; }

    bool lastChangeWasUserEdit() const { return m_lastChangeWasUserEdit; }
    void setLastChangeWasUserEdit(bool lastChangeWasUserEdit);

    int selectionStart();
    int selectionEnd();
    void setSelectionStart(int);
    void setSelectionEnd(int);
    void select();
    void setSelectionRange(int start, int end);
    VisibleSelection selection(int start, int end) const;

    virtual void subtreeHasChanged();
    String text();
    String textWithHardLineBreaks();
    void selectionChanged(bool userTriggered);

    VisiblePosition visiblePositionForIndex(int index);
    int indexForVisiblePosition(const VisiblePosition&);

    void updatePlaceholderVisibility(bool, bool);

protected:
    RenderTextControl(Node*, bool);

    int scrollbarThickness() const;
    void adjustInnerTextStyle(const RenderStyle* startStyle, RenderStyle* textBlockStyle) const;
    void setInnerTextValue(const String&);

    virtual void styleDidChange(StyleDifference, const RenderStyle* oldStyle);

    void createSubtreeIfNeeded(TextControlInnerElement* innerBlock);
    void hitInnerTextElement(HitTestResult&, int x, int y, int tx, int ty);
    void forwardEvent(Event*);

    int textBlockWidth() const;
    int textBlockHeight() const;

    virtual int preferredContentWidth(float charWidth) const = 0;
    virtual void adjustControlHeightBasedOnLineHeight(int lineHeight) = 0;
    virtual void cacheSelection(int start, int end) = 0;
    virtual PassRefPtr<RenderStyle> createInnerTextStyle(const RenderStyle* startStyle) const = 0;
    virtual RenderStyle* textBaseStyle() const = 0;

    virtual void updateFromElement();
    virtual void calcHeight();

    friend class TextIterator;
    HTMLElement* innerTextElement() const;

    bool m_placeholderVisible;

private:
    virtual const char* renderName() const { return "RenderTextControl"; }
    virtual bool isTextControl() const { return true; }
    virtual bool hasControlClip() const { return false; }
    virtual IntRect controlClipRect(int tx, int ty) const;
    virtual void calcPrefWidths();
    virtual void removeLeftoverAnonymousBlock(RenderBlock*) { }
    virtual bool canHaveChildren() const { return false; }
    virtual bool avoidsFloats() const { return true; }
    void setInnerTextStyle(PassRefPtr<RenderStyle>);
    
    virtual void addFocusRingRects(Vector<IntRect>&, int tx, int ty);

    virtual bool canBeProgramaticallyScrolled(bool) const { return true; }

    String finishText(Vector<UChar>&) const;

    bool m_wasChangedSinceLastChangeEvent;
    bool m_lastChangeWasUserEdit;
    RefPtr<TextControlInnerTextElement> m_innerText;
};

inline RenderTextControl* toRenderTextControl(RenderObject* object)
{ 
    ASSERT(!object || object->isTextControl());
    return static_cast<RenderTextControl*>(object);
}

inline const RenderTextControl* toRenderTextControl(const RenderObject* object)
{ 
    ASSERT(!object || object->isTextControl());
    return static_cast<const RenderTextControl*>(object);
}

// This will catch anyone doing an unnecessary cast.
void toRenderTextControl(const RenderTextControl*);

} // namespace WebCore

#endif // RenderTextControl_h
