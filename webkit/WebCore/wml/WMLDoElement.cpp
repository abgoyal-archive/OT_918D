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
 * Copyright (C) 2008, 2009 Torch Mobile Inc. All rights reserved. (http://www.torchmobile.com/)
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
#include "WMLDoElement.h"

#include "Event.h"
#include "EventNames.h"
#include "HTMLNames.h"
#include "KeyboardEvent.h"
#include "MappedAttribute.h"
#include "Page.h"
#include "RenderButton.h"
#include "WMLCardElement.h"
#include "WMLDocument.h"
#include "WMLTaskElement.h"
#include "WMLTimerElement.h"
#include "WMLNames.h"
#include "WMLPageState.h"
#include "WMLVariables.h"

namespace WebCore {

using namespace WMLNames;

WMLDoElement::WMLDoElement(const QualifiedName& tagName, Document* doc)
    : WMLElement(tagName, doc)
    , m_task(0)
    , m_isActive(false)
    , m_isNoop(false)
    , m_isOptional(false)
{
}

void WMLDoElement::defaultEventHandler(Event* event)
{
    if (m_isOptional)
        return;

    if (event->type() == eventNames().keypressEvent) {
        WMLElement::defaultEventHandler(event);
        return;
    }

    if (event->type() != eventNames().clickEvent && event->type() != eventNames().keydownEvent)
        return;
             
    if (event->isKeyboardEvent()
        && static_cast<KeyboardEvent*>(event)->keyIdentifier() != "Enter")
        return;

    if (m_type == "accept" || m_type == "options") {
        if (m_task)
            m_task->executeTask();
    } else if (m_type == "prev") {
        ASSERT(document()->isWMLDocument());
        WMLDocument* document = static_cast<WMLDocument*>(this->document());

        WMLPageState* pageState = wmlPageStateForDocument(document);
        if (!pageState)
            return;
    
        // Stop the timer of the current card if it is active
        if (WMLCardElement* card = document->activeCard()) {
            if (WMLTimerElement* eventTimer = card->eventTimer())
                eventTimer->stop();
        }

        pageState->page()->goBack();
    } else if (m_type == "reset") {
        WMLPageState* pageState = wmlPageStateForDocument(document());
        if (!pageState)
            return;

        pageState->reset();
    }
}

void WMLDoElement::parseMappedAttribute(MappedAttribute* attr)
{
    if (attr->name() == HTMLNames::typeAttr)
        m_type = parseValueForbiddingVariableReferences(attr->value());
    else if (attr->name() == HTMLNames::nameAttr)
        m_name = parseValueForbiddingVariableReferences(attr->value());
    else if (attr->name() == optionalAttr)
        m_isOptional = (attr->value() == "true");
    else
        WMLElement::parseMappedAttribute(attr);
}

void WMLDoElement::insertedIntoDocument()
{
    WMLElement::insertedIntoDocument();

    // Spec: An unspecified 'name' defaults to the value of the 'type' attribute.
    if (!hasAttribute(HTMLNames::nameAttr))
        m_name = m_type;

    Node* parent = parentNode();
    if (!parent || !parent->isWMLElement())
        return;

    if (WMLEventHandlingElement* eventHandlingElement = toWMLEventHandlingElement(static_cast<WMLElement*>(parent)))
        eventHandlingElement->registerDoElement(this, document());
}

void WMLDoElement::removedFromDocument()
{
    Node* parent = parentNode();

    if (parent  && parent->isWMLElement()) {
        if (WMLEventHandlingElement* eventHandlingElement = toWMLEventHandlingElement(static_cast<WMLElement*>(parent)))
            eventHandlingElement->deregisterDoElement(this);
    }

    WMLElement::removedFromDocument();
}

void WMLDoElement::attach()
{
    WMLElement::attach();

    // The call to updateFromElement() needs to go after the call through
    // to the base class's attach() because that can sometimes do a close
    // on the renderer.
    if (renderer())
        renderer()->updateFromElement();
}

RenderObject* WMLDoElement::createRenderer(RenderArena* arena, RenderStyle* style)
{
    if (!m_isActive || m_isOptional || m_isNoop)
        return 0;

    if (style) {
        style->setUnique();
        style->setBackgroundColor(Color::lightGray);
    }

    return new (arena) RenderButton(this);
}

void WMLDoElement::recalcStyle(StyleChange change)
{
    WMLElement::recalcStyle(change);

    if (renderer())
        renderer()->updateFromElement();
}

void WMLDoElement::registerTask(WMLTaskElement* task)
{
    ASSERT(!m_task);
    m_task = task;
}

void WMLDoElement::deregisterTask(WMLTaskElement* task)
{
    ASSERT_UNUSED(task, m_task == task);
    m_task = 0;
}

String WMLDoElement::label() const
{
    return parseValueSubstitutingVariableReferences(getAttribute(HTMLNames::labelAttr));
}

}

#endif
