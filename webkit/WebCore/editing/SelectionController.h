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
 * Copyright (C) 2004, 2005, 2006, 2007, 2008, 2009, 2010 Apple Inc. All rights reserved.
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

#ifndef SelectionController_h
#define SelectionController_h

#include "IntRect.h"
#include "Range.h"
#include "Timer.h"
#include "VisibleSelection.h"
#include <wtf/Noncopyable.h>

namespace WebCore {

class Frame;
class GraphicsContext;
class RenderObject;
class VisiblePosition;

class SelectionController : public Noncopyable {
public:
    enum EAlteration { MOVE, EXTEND };
    enum EDirection { FORWARD, BACKWARD, RIGHT, LEFT };

    SelectionController(Frame* = 0, bool isDragCaretController = false);

    Element* rootEditableElement() const { return m_selection.rootEditableElement(); }
    bool isContentEditable() const { return m_selection.isContentEditable(); }
    bool isContentRichlyEditable() const { return m_selection.isContentRichlyEditable(); }
    Node* shadowTreeRootNode() const { return m_selection.shadowTreeRootNode(); }
     
    void moveTo(const Range*, EAffinity, bool userTriggered = false);
    void moveTo(const VisiblePosition&, bool userTriggered = false);
    void moveTo(const VisiblePosition&, const VisiblePosition&, bool userTriggered = false);
    void moveTo(const Position&, EAffinity, bool userTriggered = false);
    void moveTo(const Position&, const Position&, EAffinity, bool userTriggered = false);

    const VisibleSelection& selection() const { return m_selection; }
    void setSelection(const VisibleSelection&, bool closeTyping = true, bool clearTypingStyle = true, bool userTriggered = false);
    bool setSelectedRange(Range*, EAffinity, bool closeTyping);
    void selectAll();
    void clear();
    
    // Call this after doing user-triggered selections to make it easy to delete the frame you entirely selected.
    void selectFrameElementInParentIfFullySelected();

    bool contains(const IntPoint&);

    VisibleSelection::SelectionType selectionType() const { return m_selection.selectionType(); }

    EAffinity affinity() const { return m_selection.affinity(); }

    bool modify(EAlteration, EDirection, TextGranularity, bool userTriggered = false);
    bool modify(EAlteration, int verticalDistance, bool userTriggered = false);
    bool expandUsingGranularity(TextGranularity);

    void setBase(const VisiblePosition&, bool userTriggered = false);
    void setBase(const Position&, EAffinity, bool userTriggered = false);
    void setExtent(const VisiblePosition&, bool userTriggered = false);
    void setExtent(const Position&, EAffinity, bool userTriggered = false);

    Position base() const { return m_selection.base(); }
    Position extent() const { return m_selection.extent(); }
    Position start() const { return m_selection.start(); }
    Position end() const { return m_selection.end(); }

    // Return the renderer that is responsible for painting the caret (in the selection start node)
    RenderObject* caretRenderer() const;

    // Caret rect local to the caret's renderer
    IntRect localCaretRect() const;
    // Bounds of (possibly transformed) caret in absolute coords
    IntRect absoluteCaretBounds();
    void setNeedsLayout(bool flag = true);

    void setLastChangeWasHorizontalExtension(bool b) { m_lastChangeWasHorizontalExtension = b; }
    void willBeModified(EAlteration, EDirection);
    
    bool isNone() const { return m_selection.isNone(); }
    bool isCaret() const { return m_selection.isCaret(); }
    bool isRange() const { return m_selection.isRange(); }
    bool isCaretOrRange() const { return m_selection.isCaretOrRange(); }
    bool isInPasswordField() const;
    bool isAll(StayInEditableContent stayInEditableContent = MustStayInEditableContent) const { return m_selection.isAll(stayInEditableContent); }
    
    PassRefPtr<Range> toNormalizedRange() const { return m_selection.toNormalizedRange(); }

    void debugRenderer(RenderObject*, bool selected) const;
    
    void nodeWillBeRemoved(Node*);

    void setCaretVisible(bool = true);
    void clearCaretRectIfNeeded();
    bool recomputeCaretRect(); // returns true if caret rect moved
    void invalidateCaretRect();
    void paintCaret(GraphicsContext*, int tx, int ty, const IntRect& clipRect);

    // Used to suspend caret blinking while the mouse is down.
    void setCaretBlinkingSuspended(bool suspended) { m_isCaretBlinkingSuspended = suspended; }
    bool isCaretBlinkingSuspended() const { return m_isCaretBlinkingSuspended; }

    // Focus
    void setFocused(bool);
    bool isFocused() const { return m_focused; }
    bool isFocusedAndActive() const;
    void pageActivationChanged();

    // Painting.
    void updateAppearance();

#ifndef NDEBUG
    void formatForDebugger(char* buffer, unsigned length) const;
    void showTreeForThis() const;
#endif

private:
    enum EPositionType { START, END, BASE, EXTENT };

    TextDirection directionOfEnclosingBlock();

    VisiblePosition modifyExtendingRight(TextGranularity);
    VisiblePosition modifyExtendingForward(TextGranularity);
    VisiblePosition modifyMovingRight(TextGranularity);
    VisiblePosition modifyMovingForward(TextGranularity);
    VisiblePosition modifyExtendingLeft(TextGranularity);
    VisiblePosition modifyExtendingBackward(TextGranularity);
    VisiblePosition modifyMovingLeft(TextGranularity);
    VisiblePosition modifyMovingBackward(TextGranularity);

    void layout();
    IntRect caretRepaintRect() const;

    int xPosForVerticalArrowNavigation(EPositionType);
    
    void notifyAccessibilityForSelectionChange();

    void focusedOrActiveStateChanged();
    bool caretRendersInsideNode(Node*) const;
    
    IntRect absoluteBoundsForLocalRect(const IntRect&) const;

    void caretBlinkTimerFired(Timer<SelectionController>*);

    Frame* m_frame;

    int m_xPosForVerticalArrowNavigation;

    VisibleSelection m_selection;

    Timer<SelectionController> m_caretBlinkTimer;

    IntRect m_caretRect; // caret rect in coords local to the renderer responsible for painting the caret
    IntRect m_absCaretBounds; // absolute bounding rect for the caret
    IntRect m_absoluteCaretRepaintBounds;
    
    bool m_needsLayout; // true if m_caretRect and m_absCaretBounds need to be calculated
    bool m_absCaretBoundsDirty;
    bool m_lastChangeWasHorizontalExtension;
    bool m_isDragCaretController;
    bool m_isCaretBlinkingSuspended;
    bool m_focused;
    bool m_caretVisible;
    bool m_caretPaint;
};

#if !(PLATFORM(MAC) || PLATFORM(GTK))
inline void SelectionController::notifyAccessibilityForSelectionChange()
{
}
#endif

} // namespace WebCore

#ifndef NDEBUG
// Outside the WebCore namespace for ease of invocation from gdb.
void showTree(const WebCore::SelectionController&);
void showTree(const WebCore::SelectionController*);
#endif

#endif // SelectionController_h

