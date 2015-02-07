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
    Copyright (C) 2004, 2005 Nikolas Zimmermann <wildfox@kde.org>
                  2004, 2005, 2006 Rob Buis <buis@kde.org>
    Copyright (C) 2009 Google, Inc.  All rights reserved.

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#include "config.h"
#if ENABLE(SVG)

#include "SVGLocatable.h"

#include "AffineTransform.h"
#include "RenderPath.h"
#include "SVGException.h"
#include "SVGSVGElement.h"

namespace WebCore {

SVGLocatable::SVGLocatable()
{
}

SVGLocatable::~SVGLocatable()
{
}

static bool isViewportElement(Node* node)
{
    return (node->hasTagName(SVGNames::svgTag)
        || node->hasTagName(SVGNames::symbolTag)
#if ENABLE(SVG_FOREIGN_OBJECT)
        || node->hasTagName(SVGNames::foreignObjectTag)
#endif
        || node->hasTagName(SVGNames::imageTag));
}

SVGElement* SVGLocatable::nearestViewportElement(const SVGElement* element)
{
    ASSERT(element);
    for (Node* n = element->parentNode(); n && !n->isDocumentNode(); n = n->parentNode()) {
        if (isViewportElement(n))
            return static_cast<SVGElement*>(n);
    }

    return 0;
}

SVGElement* SVGLocatable::farthestViewportElement(const SVGElement* element)
{
    ASSERT(element);
    SVGElement* farthest = 0;
    for (Node* n = element->parentNode(); n && !n->isDocumentNode(); n = n->parentNode()) {
        if (isViewportElement(n))
            farthest = static_cast<SVGElement*>(n);
    }
    return farthest;
}

FloatRect SVGLocatable::getBBox(const SVGElement* element)
{
    element->document()->updateLayoutIgnorePendingStylesheets();

    // FIXME: Eventually we should support getBBox for detached elements.
    if (!element->renderer())
        return FloatRect();

    return element->renderer()->objectBoundingBox();
}

AffineTransform SVGLocatable::getCTM(const SVGElement* element)
{
    ASSERT(element);
    AffineTransform ctm;

    Node* parent = element->parentNode();
    if (parent && parent->isSVGElement()) {
        SVGElement* parentElement = static_cast<SVGElement*>(parent);
        if (parentElement && parentElement->isStyledLocatable()) {
            AffineTransform parentCTM = static_cast<SVGStyledLocatableElement*>(parentElement)->getCTM();
            ctm = parentCTM * ctm;
        }
    }

    return ctm;
}

AffineTransform SVGLocatable::getScreenCTM(const SVGElement* element)
{
    ASSERT(element);
    AffineTransform ctm;

    Node* parent = element->parentNode();
    if (parent && parent->isSVGElement()) {
        SVGElement* parentElement = static_cast<SVGElement*>(parent);
        if (parentElement && parentElement->isStyledLocatable()) {
            AffineTransform parentCTM = static_cast<SVGStyledLocatableElement*>(parentElement)->getScreenCTM();
            ctm = parentCTM * ctm;
        }
    }

    return ctm;
}

AffineTransform SVGLocatable::getTransformToElement(SVGElement* target, ExceptionCode& ec) const
{
    AffineTransform ctm = getCTM();

    if (target && target->isStyledLocatable()) {
        AffineTransform targetCTM = static_cast<SVGStyledLocatableElement*>(target)->getCTM();
        if (!targetCTM.isInvertible()) {
            ec = SVGException::SVG_MATRIX_NOT_INVERTABLE;
            return ctm;
        }
        ctm *= targetCTM.inverse();
    }

    return ctm;
}

}

#endif // ENABLE(SVG)

// vim:ts=4:noet
