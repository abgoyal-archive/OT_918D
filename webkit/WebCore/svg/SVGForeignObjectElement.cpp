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
    Copyright (C) 2006 Apple Computer, Inc.
              (C) 2008 Nikolas Zimmermann <zimmermann@kde.org>

    This file is part of the WebKit project

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

#if ENABLE(SVG) && ENABLE(SVG_FOREIGN_OBJECT)
#include "SVGForeignObjectElement.h"

#include "CSSPropertyNames.h"
#include "MappedAttribute.h"
#include "RenderForeignObject.h"
#include "SVGLength.h"
#include "SVGNames.h"
#include <wtf/Assertions.h>

namespace WebCore {

SVGForeignObjectElement::SVGForeignObjectElement(const QualifiedName& tagName, Document *doc)
    : SVGStyledTransformableElement(tagName, doc)
    , SVGTests()
    , SVGLangSpace()
    , SVGExternalResourcesRequired()
    , m_x(LengthModeWidth)
    , m_y(LengthModeHeight)
    , m_width(LengthModeWidth)
    , m_height(LengthModeHeight)
{
}

SVGForeignObjectElement::~SVGForeignObjectElement()
{
}

void SVGForeignObjectElement::parseMappedAttribute(MappedAttribute* attr)
{
    const AtomicString& value = attr->value();
    if (attr->name() == SVGNames::xAttr)
        setXBaseValue(SVGLength(LengthModeWidth, value));
    else if (attr->name() == SVGNames::yAttr)
        setYBaseValue(SVGLength(LengthModeHeight, value));
    else if (attr->name() == SVGNames::widthAttr)
        setWidthBaseValue(SVGLength(LengthModeWidth, value));
    else if (attr->name() == SVGNames::heightAttr)
        setHeightBaseValue(SVGLength(LengthModeHeight, value));
    else {
        if (SVGTests::parseMappedAttribute(attr))
            return;
        if (SVGLangSpace::parseMappedAttribute(attr))
            return;
        if (SVGExternalResourcesRequired::parseMappedAttribute(attr))
            return;
        SVGStyledTransformableElement::parseMappedAttribute(attr);
    }
}

// TODO: Move this function in some SVG*Element base class, as SVGSVGElement / SVGImageElement will need the same logic!

// This function mimics addCSSProperty and StyledElement::attributeChanged.
// In HTML code, you'd always call addCSSProperty from your derived parseMappedAttribute()
// function - though in SVG code we need to move this logic into svgAttributeChanged, in
// order to support SVG DOM changes (which don't use the parseMappedAttribute/attributeChanged).
// If we'd ignore SVG DOM, we could use _exactly_ the same logic as HTML.
static inline void addCSSPropertyAndNotifyAttributeMap(StyledElement* element, const QualifiedName& name, int cssProperty, const String& value)
{
    ASSERT(element);

    if (!element)
        return;

    NamedMappedAttrMap* attrs = element->mappedAttributes();
    ASSERT(attrs);

    if (!attrs)
        return;

    Attribute* attr = attrs->getAttributeItem(name);
    if (!attr || !attr->isMappedAttribute())
        return;

    MappedAttribute* mappedAttr = static_cast<MappedAttribute*>(attr);

    // This logic is only meant to be used for entries that have to be parsed and are mapped to eNone. Assert that.
    MappedAttributeEntry entry;
    bool needToParse = element->mapToEntry(mappedAttr->name(), entry);

    ASSERT(needToParse);
    ASSERT(entry == eNone);

    if (!needToParse || entry != eNone) 
        return;

    if (mappedAttr->decl()) {
        mappedAttr->setDecl(0);
        attrs->declRemoved();
    }

    element->setNeedsStyleRecalc();
    element->addCSSProperty(mappedAttr, cssProperty, value);

    if (CSSMappedAttributeDeclaration* decl = mappedAttr->decl()) {
        // Add the decl to the table in the appropriate spot.
        element->setMappedAttributeDecl(entry, mappedAttr, decl);

        decl->setMappedState(entry, mappedAttr->name(), mappedAttr->value());
        decl->setParent(0);
        decl->setNode(0);

        attrs->declAdded();
    }
}

void SVGForeignObjectElement::svgAttributeChanged(const QualifiedName& attrName)
{
    SVGStyledTransformableElement::svgAttributeChanged(attrName);

    if (attrName == SVGNames::widthAttr) {
        addCSSPropertyAndNotifyAttributeMap(this, attrName, CSSPropertyWidth, width().valueAsString());
        return;
    } else if (attrName == SVGNames::heightAttr) {
        addCSSPropertyAndNotifyAttributeMap(this, attrName, CSSPropertyHeight, height().valueAsString());
        return;
    }

    if (!renderer())
        return;

    if (attrName == SVGNames::xAttr || attrName == SVGNames::yAttr ||
        SVGTests::isKnownAttribute(attrName) ||
        SVGLangSpace::isKnownAttribute(attrName) ||
        SVGExternalResourcesRequired::isKnownAttribute(attrName) ||
        SVGStyledTransformableElement::isKnownAttribute(attrName))
        renderer()->setNeedsLayout(true);
}

void SVGForeignObjectElement::synchronizeProperty(const QualifiedName& attrName)
{
    SVGStyledTransformableElement::synchronizeProperty(attrName);

    if (attrName == anyQName()) {
        synchronizeX();
        synchronizeY();
        synchronizeWidth();
        synchronizeHeight();
        synchronizeExternalResourcesRequired();
        synchronizeHref();
        return;
    }

    if (attrName == SVGNames::xAttr)
        synchronizeX();
    else if (attrName == SVGNames::yAttr)
        synchronizeY();
    else if (attrName == SVGNames::widthAttr)
        synchronizeWidth();
    else if (attrName == SVGNames::heightAttr)
        synchronizeHeight();
    else if (SVGExternalResourcesRequired::isKnownAttribute(attrName))
        synchronizeExternalResourcesRequired();
    else if (SVGURIReference::isKnownAttribute(attrName))
        synchronizeHref();
}

RenderObject* SVGForeignObjectElement::createRenderer(RenderArena* arena, RenderStyle*)
{
    return new (arena) RenderForeignObject(this);
}

bool SVGForeignObjectElement::childShouldCreateRenderer(Node* child) const
{
    // Skip over SVG rules which disallow non-SVG kids
    return StyledElement::childShouldCreateRenderer(child);
}

} // namespace WebCore

#endif // ENABLE(SVG) && ENABLE(SVG_FOREIGN_OBJECT)