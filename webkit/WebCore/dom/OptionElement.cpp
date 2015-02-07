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
 * Copyright (C) 2009 Torch Mobile Inc. All rights reserved. (http://www.torchmobile.com/)
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
#include "OptionElement.h"

#include "Document.h"
#include "Element.h"
#include "HTMLNames.h"
#include "HTMLOptionElement.h"
#include "OptionGroupElement.h"
#include "ScriptElement.h"
#include "SelectElement.h"
#include <wtf/Assertions.h>

#if ENABLE(WML)
#include "WMLOptionElement.h"
#include "WMLNames.h"
#endif

namespace WebCore {

void OptionElement::setSelectedState(OptionElementData& data, Element* element, bool selected)
{
    if (data.selected() == selected)
        return;

    data.setSelected(selected);
    element->setNeedsStyleRecalc();
}

int OptionElement::optionIndex(SelectElement* selectElement, const Element* element)
{
    if (!selectElement)
        return 0;

    // Let's do this dynamically. Might be a bit slow, but we're sure
    // we won't forget to update a member variable in some cases...
    const Vector<Element*>& items = selectElement->listItems();
    int length = items.size();
    int optionIndex = 0;
    for (int i = 0; i < length; ++i) {
        if (!isOptionElement(items[i]))
            continue;
        if (items[i] == element)
            return optionIndex;
        ++optionIndex;
    }

    return 0;
}

String OptionElement::collectOptionLabelOrText(const OptionElementData& data, const Element* element)
{
    Document* document = element->document();
    String text;

    // WinIE does not use the label attribute, so as a quirk, we ignore it.
    if (!document->inCompatMode())
        text = data.label();
    if (text.isEmpty())
        text = collectOptionInnerText(element);
    return normalizeText(document, text);
}

String OptionElement::collectOptionInnerText(const Element* element)
{
    String text;
    Node* n = element->firstChild();
    while (n) {
        if (n->nodeType() == Node::TEXT_NODE || n->nodeType() == Node::CDATA_SECTION_NODE)
            text += n->nodeValue();

        // skip script content
        if (n->isElementNode() && toScriptElement(static_cast<Element*>(n)))
            n = n->traverseNextSibling(element);
        else
            n = n->traverseNextNode(element);
    }
    return text;
}

String OptionElement::normalizeText(const Document* document, const String& src)
{
    String text = document->displayStringModifiedByEncoding(src);

    // In WinIE, leading and trailing whitespace is ignored in options and optgroups. We match this behavior.
    text = text.stripWhiteSpace();

    // We want to collapse our whitespace too.  This will match other browsers.
    text = text.simplifyWhiteSpace();
    return text;
}

String OptionElement::collectOptionTextRespectingGroupLabel(const OptionElementData& data, const Element* element)
{
    Element* parentElement = static_cast<Element*>(element->parentNode());
    if (parentElement && toOptionGroupElement(parentElement))
        return "    " + collectOptionLabelOrText(data, element);

    return collectOptionLabelOrText(data, element);
}

String OptionElement::collectOptionValue(const OptionElementData& data, const Element* element)
{
    String value = data.value();
    if (!value.isNull())
        return value;

    // Use the text if the value wasn't set.
    return collectOptionInnerText(element).stripWhiteSpace();
}

// OptionElementData
OptionElementData::OptionElementData()
    : m_selected(false)
{
}

OptionElement* toOptionElement(Element* element)
{
    if (element->isHTMLElement() && element->hasTagName(HTMLNames::optionTag))
        return static_cast<HTMLOptionElement*>(element);

#if ENABLE(WML)
    if (element->isWMLElement() && element->hasTagName(WMLNames::optionTag))
        return static_cast<WMLOptionElement*>(element);
#endif

    return 0;
}

bool isOptionElement(Element* element)
{
    return element->hasLocalName(HTMLNames::optionTag)
#if ENABLE(WML)
        || element->hasLocalName(WMLNames::optionTag)
#endif
        ;
}

}
