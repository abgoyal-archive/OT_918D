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
    Copyright (C) 2004, 2005, 2007 Nikolas Zimmermann <zimmermann@kde.org>
                  2004, 2005, 2007 Rob Buis <buis@kde.org>
    Copyright (C) 2009 Google, Inc.  All rights reserved.
    Copyright (C) 2009 Apple Inc. All rights reserved.

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    aint with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#ifndef RenderSVGRoot_h
#define RenderSVGRoot_h

#if ENABLE(SVG)
#include "RenderBox.h"
#include "FloatRect.h"
#include "SVGRenderSupport.h"

namespace WebCore {

class SVGStyledElement;
class AffineTransform;

class RenderSVGRoot : public RenderBox, protected SVGRenderBase {
public:
    RenderSVGRoot(SVGStyledElement*);

    const RenderObjectChildList* children() const { return &m_children; }
    RenderObjectChildList* children() { return &m_children; }

private:
    virtual RenderObjectChildList* virtualChildren() { return children(); }
    virtual const RenderObjectChildList* virtualChildren() const { return children(); }

    virtual bool isSVGRoot() const { return true; }
    virtual const char* renderName() const { return "RenderSVGRoot"; }

    virtual int lineHeight(bool b, bool isRootLineBox = false) const;
    virtual int baselinePosition(bool b, bool isRootLineBox = false) const;
    virtual void calcPrefWidths();

    virtual void layout();
    virtual void paint(PaintInfo&, int parentX, int parentY);

    virtual const AffineTransform& localToParentTransform() const;

    bool fillContains(const FloatPoint&) const;
    bool strokeContains(const FloatPoint&) const;

    virtual FloatRect objectBoundingBox() const;
    virtual FloatRect strokeBoundingBox() const { return computeContainerBoundingBox(this, true); }
    virtual FloatRect repaintRectInLocalCoordinates() const;

    // FIXME: This override should be removed.
    virtual AffineTransform localTransform() const;

    virtual bool nodeAtPoint(const HitTestRequest&, HitTestResult&, int x, int y, int tx, int ty, HitTestAction);

    virtual void computeRectForRepaint(RenderBoxModelObject* repaintContainer, IntRect& repaintRect, bool fixed);

    virtual void mapLocalToContainer(RenderBoxModelObject* repaintContainer, bool useTransforms, bool fixed, TransformState&) const;

    void calcViewport();

    bool selfWillPaint() const;

    IntSize parentOriginToBorderBox() const;
    IntSize borderOriginToContentBox() const;
    AffineTransform localToRepaintContainerTransform(const IntPoint& parentOriginInContainer) const;
    AffineTransform localToBorderBoxTransform() const;

    RenderObjectChildList m_children;
    FloatSize m_viewportSize;
    mutable AffineTransform m_localToParentTransform;
};

inline RenderSVGRoot* toRenderSVGRoot(RenderObject* object)
{ 
    ASSERT(!object || object->isSVGRoot());
    return static_cast<RenderSVGRoot*>(object);
}

inline const RenderSVGRoot* toRenderSVGRoot(const RenderObject* object)
{ 
    ASSERT(!object || object->isSVGRoot());
    return static_cast<const RenderSVGRoot*>(object);
}

// This will catch anyone doing an unnecessary cast.
void toRenderSVGRoot(const RenderSVGRoot*);

} // namespace WebCore

#endif // ENABLE(SVG)
#endif // RenderSVGRoot_h

// vim:ts=4:noet