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

/**
 * Copyright (C) 2008 Torch Mobile Inc. All rights reserved. (http://www.torchmobile.com/)
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

#include "config.h"

#if ENABLE(WML)
#include "WMLImageElement.h"

#include "CSSPropertyNames.h"
#include "CSSValueKeywords.h"
#include "HTMLElement.h"
#include "HTMLNames.h"
#include "MappedAttribute.h"
#include "RenderImage.h"
#include "WMLNames.h"
#include "WMLVariables.h"

namespace WebCore {

using namespace WMLNames;

WMLImageElement::WMLImageElement(const QualifiedName& tagName, Document* doc)
    : WMLElement(tagName, doc)
    , m_imageLoader(this)
    , m_useFallbackAttribute(false)
{
}

WMLImageElement::~WMLImageElement()
{
}

bool WMLImageElement::mapToEntry(const QualifiedName& attrName, MappedAttributeEntry& result) const
{
    if (attrName == HTMLNames::widthAttr ||
        attrName == HTMLNames::heightAttr ||
        attrName == HTMLNames::vspaceAttr ||
        attrName == HTMLNames::hspaceAttr) {
        result = eUniversal;
        return false;
    }

    if (attrName == HTMLNames::alignAttr) {
        result = eReplaced;
        return false;
    }

    return WMLElement::mapToEntry(attrName, result);
}

void WMLImageElement::parseMappedAttribute(MappedAttribute* attr)
{
    const QualifiedName& attrName = attr->name();

    if (attrName == HTMLNames::altAttr) {
        if (renderer() && renderer()->isImage())
            toRenderImage(renderer())->updateAltText();
    } else if (attrName == HTMLNames::srcAttr || attrName == localsrcAttr)
        m_imageLoader.updateFromElementIgnoringPreviousError();
    else if (attrName == HTMLNames::widthAttr)
        addCSSLength(attr, CSSPropertyWidth, attr->value());
    else if (attrName == HTMLNames::heightAttr)
        addCSSLength(attr, CSSPropertyHeight, attr->value());
    else if (attrName == HTMLNames::vspaceAttr) {
        addCSSLength(attr, CSSPropertyMarginTop, attr->value());
        addCSSLength(attr, CSSPropertyMarginBottom, attr->value());
    } else if (attrName == HTMLNames::hspaceAttr) {
        addCSSLength(attr, CSSPropertyMarginLeft, attr->value());
        addCSSLength(attr, CSSPropertyMarginRight, attr->value());
    } else if (attrName == HTMLNames::alignAttr)
        HTMLElement::addHTMLAlignmentToStyledElement(this, attr);
    else
        WMLElement::parseMappedAttribute(attr);
}

void WMLImageElement::attach()
{
    WMLElement::attach();

    if (renderer() && renderer()->isImage() && m_imageLoader.haveFiredBeforeLoadEvent()) {
        RenderImage* imageObj = toRenderImage(renderer());
        if (imageObj->hasImage())
            return;
        imageObj->setCachedImage(m_imageLoader.image());
        
        // If we have no image at all because we have no src attribute, set
        // image height and width for the alt text instead.
        if (!m_imageLoader.image() && !imageObj->cachedImage())
            imageObj->setImageSizeForAltText();
    }
}

RenderObject* WMLImageElement::createRenderer(RenderArena* arena, RenderStyle*)
{
    return new (arena) RenderImage(this);
}

void WMLImageElement::insertedIntoDocument()
{
    // If we have been inserted from a renderer-less document,
    // our loader may have not fetched the image, so do it now.
    if (!m_imageLoader.image())
        m_imageLoader.updateFromElement();

    WMLElement::insertedIntoDocument();
}

bool WMLImageElement::isURLAttribute(Attribute* attr) const
{
    return attr->name() == HTMLNames::srcAttr
           || attr->name() == localsrcAttr;
}

const QualifiedName& WMLImageElement::imageSourceAttributeName() const
{
    // Spec: Any 'localsrc' parameter specified takes precedence over the 
    // image specified in the src parameter.
    if (hasAttribute(localsrcAttr) && !m_useFallbackAttribute)
        return localsrcAttr;

    return HTMLNames::srcAttr;
}

String WMLImageElement::altText() const
{
    return substituteVariableReferences(getAttribute(HTMLNames::altAttr), document());
}

}

#endif
