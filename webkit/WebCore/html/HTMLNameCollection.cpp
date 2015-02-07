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
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 1999 Antti Koivisto (koivisto@kde.org)
 * Copyright (C) 2003, 2004, 2005, 2006, 2007 Apple Inc. All rights reserved.
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
#include "HTMLNameCollection.h"

#include "Element.h"
#include "HTMLDocument.h"
#include "HTMLNames.h"
#include "HTMLObjectElement.h"

namespace WebCore {

using namespace HTMLNames;

HTMLNameCollection::HTMLNameCollection(PassRefPtr<Document> document, CollectionType type, const String& name)
    : HTMLCollection(document.get(), type, document->nameCollectionInfo(type, name))
    , m_name(name)
{
}

Element* HTMLNameCollection::itemAfter(Element* previous) const
{
    ASSERT(previous != base());

    Node* current;
    if (!previous)
        current = base()->firstChild();
    else
        current = previous->traverseNextNode(base());

    for (; current; current = current->traverseNextNode(base())) {
        if (!current->isElementNode())
            continue;
        Element* e = static_cast<Element*>(current);
        switch (type()) {
            case WindowNamedItems:
                // find only images, forms, applets, embeds and objects by name, 
                // but anything by id
                if (e->hasTagName(imgTag) ||
                    e->hasTagName(formTag) ||
                    e->hasTagName(appletTag) ||
                    e->hasTagName(embedTag) ||
                    e->hasTagName(objectTag))
                    if (e->getAttribute(nameAttr) == m_name)
                        return e;
                if (e->getAttribute(e->idAttributeName()) == m_name)
                    return e;
                break;
            case DocumentNamedItems:
                // find images, forms, applets, embeds, objects and iframes by name, 
                // applets and object by id, and images by id but only if they have
                // a name attribute (this very strange rule matches IE)
                if (e->hasTagName(formTag) || e->hasTagName(embedTag) || e->hasTagName(iframeTag)) {
                    if (e->getAttribute(nameAttr) == m_name)
                        return e;
                } else if (e->hasTagName(appletTag)) {
                    if (e->getAttribute(nameAttr) == m_name || e->getAttribute(e->idAttributeName()) == m_name)
                        return e;
                } else if (e->hasTagName(objectTag)) {
                    if ((e->getAttribute(nameAttr) == m_name || e->getAttribute(e->idAttributeName()) == m_name)
                            && static_cast<HTMLObjectElement*>(e)->isDocNamedItem())
                        return e;
                } else if (e->hasTagName(imgTag)) {
                    if (e->getAttribute(nameAttr) == m_name || (e->getAttribute(e->idAttributeName()) == m_name && e->hasAttribute(nameAttr)))
                        return e;
                }
                break;
        default:
            ASSERT_NOT_REACHED();
        }
    }

    return 0;
}

}
