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
#include "WMLElement.h"

#include "CSSPropertyNames.h"
#include "HTMLNames.h"
#include "MappedAttribute.h"
#include "RenderObject.h"
#include "WMLErrorHandling.h"
#include "WMLNames.h"
#include "WMLVariables.h"

using std::max;
using std::min;

namespace WebCore {

using namespace WMLNames;

WMLElement::WMLElement(const QualifiedName& tagName, Document* document)
    : StyledElement(tagName, document, CreateElementZeroRefCount)
{
}

PassRefPtr<WMLElement> WMLElement::create(const QualifiedName& tagName, Document* document)
{
    return new WMLElement(tagName, document);
}

bool WMLElement::mapToEntry(const QualifiedName& attrName, MappedAttributeEntry& result) const
{
    if (attrName == HTMLNames::alignAttr) {
        result = eUniversal;
        return false;
    }

    return StyledElement::mapToEntry(attrName, result);
}
    
void WMLElement::parseMappedAttribute(MappedAttribute* attr)
{
    if (attr->name() == idAttributeName()
        || attr->name() == HTMLNames::classAttr
        || attr->name() == HTMLNames::styleAttr)
        return StyledElement::parseMappedAttribute(attr);

    if (attr->name() == HTMLNames::alignAttr) {
        if (equalIgnoringCase(attr->value(), "middle"))
            addCSSProperty(attr, CSSPropertyTextAlign, "center");
        else
            addCSSProperty(attr, CSSPropertyTextAlign, attr->value());
    } else if (attr->name() == HTMLNames::tabindexAttr) {
        String indexstring = attr->value();
        if (indexstring.length()) {
            bool parsedOK;
            int tabindex = indexstring.toIntStrict(&parsedOK);
            if (parsedOK)
                // Clamp tabindex to the range of 'short' to match Firefox's behavior.
                setTabIndexExplicitly(max(static_cast<int>(std::numeric_limits<short>::min()), min(tabindex, static_cast<int>(std::numeric_limits<short>::max()))));
        }
    }
}

String WMLElement::title() const
{
    return parseValueSubstitutingVariableReferences(getAttribute(HTMLNames::titleAttr));
}

bool WMLElement::rendererIsNeeded(RenderStyle* style)
{
    return document()->documentElement() == this || style->display() != NONE;
}

RenderObject* WMLElement::createRenderer(RenderArena*, RenderStyle* style)
{
    return RenderObject::createObject(this, style);
}

String WMLElement::parseValueSubstitutingVariableReferences(const AtomicString& value, WMLErrorCode defaultErrorCode) const
{
    bool isValid = false;
    if (!containsVariableReference(value, isValid))
        return value;

    if (!isValid) {
        reportWMLError(document(), defaultErrorCode);
        return String();
    }

    return substituteVariableReferences(value, document());
}

String WMLElement::parseValueForbiddingVariableReferences(const AtomicString& value) const
{
    bool isValid = false;
    if (containsVariableReference(value, isValid)) {
        reportWMLError(document(), WMLErrorInvalidVariableReferenceLocation);
        return String();
    }

    return value;
}

}

#endif
