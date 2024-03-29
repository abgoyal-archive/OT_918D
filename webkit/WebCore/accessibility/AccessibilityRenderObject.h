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
 * Copyright (C) 2008 Apple Inc. All rights reserved.
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

#ifndef AccessibilityRenderObject_h
#define AccessibilityRenderObject_h

#include "AccessibilityObject.h"

namespace WebCore {
    
class AXObjectCache;
class Element;
class Frame;
class FrameView;
class HitTestResult;
class HTMLAnchorElement;
class HTMLAreaElement;
class HTMLElement;
class HTMLLabelElement;
class HTMLMapElement;
class HTMLSelectElement;
class IntPoint;
class IntSize;
class Node;
class RenderObject;
class RenderListBox;
class RenderTextControl;
class RenderView;
class VisibleSelection;
class String;
class Widget;
    
class AccessibilityRenderObject : public AccessibilityObject {
protected:
    AccessibilityRenderObject(RenderObject*);
public:
    static PassRefPtr<AccessibilityRenderObject> create(RenderObject*);
    virtual ~AccessibilityRenderObject();
    
    bool isAccessibilityRenderObject() const { return true; }
    
    virtual bool isAnchor() const;
    virtual bool isAttachment() const;
    virtual bool isHeading() const;
    virtual bool isLink() const;
    virtual bool isImageButton() const;
    virtual bool isImage() const;
    virtual bool isNativeImage() const;
    virtual bool isPasswordField() const;
    virtual bool isTextControl() const;
    virtual bool isNativeTextControl() const;
    virtual bool isWebArea() const;
    virtual bool isCheckboxOrRadio() const;
    virtual bool isFileUploadButton() const;
    virtual bool isInputImage() const;
    virtual bool isProgressIndicator() const;
    virtual bool isSlider() const;
    virtual bool isMenuRelated() const;
    virtual bool isMenu() const;
    virtual bool isMenuBar() const;
    virtual bool isMenuButton() const;
    virtual bool isMenuItem() const;
    virtual bool isControl() const;
    virtual bool isFieldset() const;
    virtual bool isGroup() const;

    virtual bool isEnabled() const;
    virtual bool isSelected() const;
    virtual bool isFocused() const;
    virtual bool isChecked() const;
    virtual bool isHovered() const;
    virtual bool isIndeterminate() const;
    virtual bool isLoaded() const;
    virtual bool isMultiSelectable() const;
    virtual bool isOffScreen() const;
    virtual bool isPressed() const;
    virtual bool isReadOnly() const;
    virtual bool isVisited() const;        
    virtual bool isRequired() const;
    virtual bool isLinked() const;
    virtual bool isExpanded() const;
    virtual void setIsExpanded(bool);

    const AtomicString& getAttribute(const QualifiedName&) const;
    virtual bool canSetFocusAttribute() const;
    virtual bool canSetTextRangeAttributes() const;
    virtual bool canSetValueAttribute() const;
    virtual bool canSetExpandedAttribute() const;

    virtual bool hasIntValue() const;
    
    virtual bool accessibilityIsIgnored() const;
    
    virtual int headingLevel() const;
    virtual int intValue() const;
    virtual String valueDescription() const;
    virtual float valueForRange() const;
    virtual float maxValueForRange() const;
    virtual float minValueForRange() const;
    virtual AccessibilityObject* selectedRadioButton();
    virtual AccessibilityObject* selectedTabItem();
    virtual int layoutCount() const;
    virtual double estimatedLoadingProgress() const;
    
    virtual AccessibilityObject* doAccessibilityHitTest(const IntPoint&) const;
    virtual AccessibilityObject* focusedUIElement() const;
    virtual AccessibilityObject* firstChild() const;
    virtual AccessibilityObject* lastChild() const;
    virtual AccessibilityObject* previousSibling() const;
    virtual AccessibilityObject* nextSibling() const;
    virtual AccessibilityObject* parentObject() const;
    virtual AccessibilityObject* parentObjectIfExists() const;
    virtual AccessibilityObject* observableObject() const;
    virtual void linkedUIElements(AccessibilityChildrenVector&) const;
    virtual bool exposesTitleUIElement() const;
    virtual AccessibilityObject* titleUIElement() const;
    virtual AccessibilityObject* correspondingControlForLabelElement() const;
    virtual AccessibilityObject* correspondingLabelForControlElement() const;

    virtual void ariaOwnsElements(AccessibilityChildrenVector&) const;
    virtual bool supportsARIAOwns() const;
    virtual AccessibilityRole ariaRoleAttribute() const;
    virtual bool isPresentationalChildOfAriaRole() const;
    virtual bool ariaRoleHasPresentationalChildren() const;
    void updateAccessibilityRole();
    
    virtual AXObjectCache* axObjectCache() const;
    
    virtual Element* actionElement() const;
    Element* mouseButtonListener() const;
    FrameView* frameViewIfRenderView() const;
    virtual Element* anchorElement() const;
    AccessibilityObject* menuForMenuButton() const;
    AccessibilityObject* menuButtonForMenu() const;
    
    virtual IntRect boundingBoxRect() const;
    virtual IntRect elementRect() const;
    virtual IntSize size() const;
    virtual IntPoint clickPoint() const;
    
    void setRenderer(RenderObject* renderer) { m_renderer = renderer; }
    RenderObject* renderer() const { return m_renderer; }
    RenderView* topRenderer() const;
    RenderTextControl* textControl() const;
    Document* document() const;
    FrameView* topDocumentFrameView() const;  
    HTMLLabelElement* labelElementContainer() const;
    
    virtual KURL url() const;
    virtual PlainTextRange selectedTextRange() const;
    virtual VisibleSelection selection() const;
    virtual String stringValue() const;
    virtual String ariaLabeledByAttribute() const;
    virtual String title() const;
    virtual String ariaDescribedByAttribute() const;
    virtual String accessibilityDescription() const;
    virtual String helpText() const;
    virtual String textUnderElement() const;
    virtual String text() const;
    virtual int textLength() const;
    virtual PassRefPtr<Range> ariaSelectedTextDOMRange() const;
    virtual String selectedText() const;
    virtual const AtomicString& accessKey() const;
    virtual const String& actionVerb() const;
    virtual Widget* widget() const;
    virtual Widget* widgetForAttachmentView() const;
    virtual void getDocumentLinks(AccessibilityChildrenVector&);
    virtual FrameView* documentFrameView() const;
    virtual String language() const;
    virtual unsigned hierarchicalLevel() const;

    virtual const AccessibilityChildrenVector& children();
    
    virtual void setFocused(bool);
    virtual void setSelectedTextRange(const PlainTextRange&);
    virtual void setValue(const String&);
    virtual void setSelected(bool);
    virtual void setSelectedRows(AccessibilityChildrenVector&);
    virtual void changeValueByPercent(float percentChange);
    virtual AccessibilityOrientation orientation() const;
    virtual void increment();
    virtual void decrement();
    
    virtual void detach();
    virtual void childrenChanged();
    virtual void contentChanged();
    virtual void addChildren();
    virtual bool canHaveChildren() const;
    virtual void selectedChildren(AccessibilityChildrenVector&);
    virtual void visibleChildren(AccessibilityChildrenVector&);
    virtual void tabChildren(AccessibilityChildrenVector&);
    virtual bool shouldFocusActiveDescendant() const;
    virtual AccessibilityObject* activeDescendant() const;
    virtual void handleActiveDescendantChanged();

    virtual VisiblePositionRange visiblePositionRange() const;
    virtual VisiblePositionRange visiblePositionRangeForLine(unsigned) const;
    virtual IntRect boundsForVisiblePositionRange(const VisiblePositionRange&) const;
    virtual void setSelectedVisiblePositionRange(const VisiblePositionRange&) const;
    virtual bool supportsARIAFlowTo() const;
    virtual void ariaFlowToElements(AccessibilityChildrenVector&) const;

    virtual bool supportsARIADropping();
    virtual bool supportsARIADragging();
    virtual bool isARIAGrabbed();
    virtual void setARIAGrabbed(bool);
    virtual void determineARIADropEffects(Vector<String>&);
    
    virtual VisiblePosition visiblePositionForPoint(const IntPoint&) const;
    virtual VisiblePosition visiblePositionForIndex(unsigned indexValue, bool lastIndexOK) const;    
    virtual int index(const VisiblePosition&) const;

    virtual VisiblePosition visiblePositionForIndex(int) const;
    virtual int indexForVisiblePosition(const VisiblePosition&) const;
    
    virtual PlainTextRange doAXRangeForLine(unsigned) const;
    virtual PlainTextRange doAXRangeForIndex(unsigned) const;
    
    virtual String doAXStringForRange(const PlainTextRange&) const;
    virtual IntRect doAXBoundsForRange(const PlainTextRange&) const;
    
    virtual void updateBackingStore();

    virtual String stringValueForMSAA() const;
    virtual String stringRoleForMSAA() const;
    virtual String nameForMSAA() const;
    virtual String descriptionForMSAA() const;
    virtual AccessibilityRole roleValueForMSAA() const;

protected:
    RenderObject* m_renderer;
    AccessibilityRole m_ariaRole;
    mutable bool m_childrenDirty;
    
    void setRenderObject(RenderObject* renderer) { m_renderer = renderer; }
    void ariaLabeledByElements(Vector<Element*>& elements) const;
    
    virtual bool isDetached() const { return !m_renderer; }

private:
    void ariaListboxSelectedChildren(AccessibilityChildrenVector&);
    void ariaListboxVisibleChildren(AccessibilityChildrenVector&);
    bool ariaIsHidden() const;
    bool isDescendantOfBarrenParent() const;
    bool isAllowedChildOfTree() const;
    bool hasTextAlternative() const;
    String positionalDescriptionForMSAA() const;

    Element* menuElementForMenuButton() const;
    Element* menuItemElementForMenu() const;
    AccessibilityRole determineAccessibilityRole();
    AccessibilityRole determineAriaRoleAttribute() const;

    bool isTabItemSelected() const;
    IntRect checkboxOrRadioRect() const;
    void addRadioButtonGroupMembers(AccessibilityChildrenVector& linkedUIElements) const;
    AccessibilityObject* internalLinkElement() const;
    AccessibilityObject* accessibilityImageMapHitTest(HTMLAreaElement*, const IntPoint&) const;
    AccessibilityObject* accessibilityParentForImageMap(HTMLMapElement* map) const;

    void ariaSelectedRows(AccessibilityChildrenVector&);
    
    bool elementAttributeValue(const QualifiedName&) const;
    void setElementAttributeValue(const QualifiedName&, bool);
    
    String accessibilityDescriptionForElements(Vector<Element*> &elements) const;
    void elementsFromAttribute(Vector<Element*>& elements, const QualifiedName& name) const;
    
    virtual const AtomicString& ariaLiveRegionStatus() const;
    virtual const AtomicString& ariaLiveRegionRelevant() const;
    virtual bool ariaLiveRegionAtomic() const;
    virtual bool ariaLiveRegionBusy() const;    
    
    void setNeedsToUpdateChildren() const { m_childrenDirty = true; }
    bool needsToUpdateChildren() const { return m_childrenDirty; }
    
    mutable AccessibilityRole m_roleForMSAA;
};
    
} // namespace WebCore

#endif // AccessibilityRenderObject_h
