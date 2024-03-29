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
 * Copyright (C) 2006, 2008 Nikolas Zimmermann <zimmermann@kde.org>
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

#include "config.h"

#if ENABLE(SVG)
#include "SVGResource.h"

#include "RenderPath.h"
#include "SVGElement.h"
#include "SVGStyledElement.h"
#include <wtf/HashSet.h>
#include <wtf/StdLibExtras.h>

namespace WebCore {

typedef HashSet<SVGResource*> ResourceSet;

static ResourceSet& resourceSet()
{
    DEFINE_STATIC_LOCAL(ResourceSet, set, ());
    return set;
}

SVGResource::SVGResource()
{
    ASSERT(!resourceSet().contains(this));
    resourceSet().add(this);
}


SVGResource::~SVGResource()
{
    ASSERT(resourceSet().contains(this));
    resourceSet().remove(this);
}

void SVGResource::invalidate()
{
    HashSet<SVGStyledElement*>::const_iterator it = m_clients.begin();
    const HashSet<SVGStyledElement*>::const_iterator end = m_clients.end();

    for (; it != end; ++it) {
        SVGStyledElement* cur = *it;

        if (cur->renderer())
            cur->renderer()->setNeedsLayout(true);

        cur->invalidateResourcesInAncestorChain();
    }
}

void SVGResource::invalidateClients(HashSet<SVGStyledElement*> clients)
{
    HashSet<SVGStyledElement*>::const_iterator it = clients.begin();
    const HashSet<SVGStyledElement*>::const_iterator end = clients.end();

    for (; it != end; ++it) {
        SVGStyledElement* cur = *it;

        if (cur->renderer())
            cur->renderer()->setNeedsLayout(true);

        cur->invalidateResourcesInAncestorChain();
    }
}

void SVGResource::removeClient(SVGStyledElement* item) 
{
    ResourceSet::iterator it = resourceSet().begin();
    ResourceSet::iterator end = resourceSet().end();

    for (; it != end; ++it) {
        SVGResource* resource = *it;
        if (!resource->m_clients.contains(item))
            continue;
        resource->m_clients.remove(item);
    }
}

void SVGResource::addClient(SVGStyledElement* item)
{
    if (m_clients.contains(item))
        return;

    m_clients.add(item);
}

TextStream& SVGResource::externalRepresentation(TextStream& ts) const
{
    return ts;
}

SVGResource* getResourceById(Document* document, const AtomicString& id, const RenderObject* object)
{
    if (id.isEmpty())
        return 0;

    Element* element = document->getElementById(id);
    SVGElement* svgElement = 0;
    if (element && element->isSVGElement())
        svgElement = static_cast<SVGElement*>(element);

    if (svgElement && svgElement->isStyled())
        return static_cast<SVGStyledElement*>(svgElement)->canvasResource(object);

    return 0;
}

TextStream& operator<<(TextStream& ts, const SVGResource& r)
{
    return r.externalRepresentation(ts);
}

}

#endif
