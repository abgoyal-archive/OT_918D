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
    Copyright (C) Research In Motion Limited 2010. All rights reserved.
                  2004, 2005, 2007 Nikolas Zimmermann <zimmermann@kde.org>
                  2004, 2005, 2008 Rob Buis <buis@kde.org>
                  2005, 2007 Eric Seidel <eric@webkit.org>
                  2009 Google, Inc.

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

#include "config.h"

#if ENABLE(SVG)
#include "SVGMarkerLayoutInfo.h"

#include "RenderSVGViewportContainer.h"
#include "SVGResourceMarker.h"

namespace WebCore {

SVGMarkerLayoutInfo::SVGMarkerLayoutInfo()
    : m_midMarker(0)
    , m_elementIndex(0)
    , m_strokeWidth(0)
{
}

SVGMarkerLayoutInfo::~SVGMarkerLayoutInfo()
{
}

static inline void processStartAndMidMarkers(void* infoPtr, const PathElement* element)
{
    SVGMarkerLayoutInfo& info = *reinterpret_cast<SVGMarkerLayoutInfo*>(infoPtr);
    SVGMarkerData& markerData = info.markerData();
    int& elementIndex = info.elementIndex();

    // First update the outslope for the previous element
    markerData.updateOutslope(element->points[0]);

    // Draw the marker for the previous element
    SVGResourceMarker* marker = markerData.marker();
    if (elementIndex > 0 && marker)
        info.addLayoutedMarker(marker, markerData.origin(), markerData.currentAngle());

    // Update our marker data for this element
    markerData.updateMarkerDataForPathElement(element);

    // After drawing the start marker, switch to drawing mid markers
    if (elementIndex == 1)
        markerData.updateTypeAndMarker(SVGMarkerData::Mid, info.midMarker());

    ++elementIndex;
}

FloatRect SVGMarkerLayoutInfo::calculateBoundaries(SVGResourceMarker* startMarker, SVGResourceMarker* midMarker, SVGResourceMarker* endMarker, float strokeWidth, const Path& path)
{
    m_layout.clear();
    m_midMarker = midMarker;
    m_strokeWidth = strokeWidth;
    m_elementIndex = 0;
    m_markerData = SVGMarkerData(SVGMarkerData::Start, startMarker);
    path.apply(this, processStartAndMidMarkers);

    if (endMarker) {
        m_markerData.updateTypeAndMarker(SVGMarkerData::End, endMarker);
        addLayoutedMarker(endMarker, m_markerData.origin(), m_markerData.currentAngle());
    }

    if (m_layout.isEmpty())
        return FloatRect();

    Vector<MarkerLayout>::iterator it = m_layout.begin();
    Vector<MarkerLayout>::iterator end = m_layout.end();

    FloatRect bounds;
    for (; it != end; ++it) {
        MarkerLayout& layout = *it;

        RenderSVGViewportContainer* markerContent = layout.marker->renderer();
        ASSERT(markerContent);

        bounds.unite(markerContent->markerBoundaries(layout.matrix));
    }

    return bounds;
}

void SVGMarkerLayoutInfo::drawMarkers(RenderObject::PaintInfo& paintInfo)
{
    if (m_layout.isEmpty())
        return;

    Vector<MarkerLayout>::iterator it = m_layout.begin();
    Vector<MarkerLayout>::iterator end = m_layout.end();

    for (; it != end; ++it) {
        MarkerLayout& layout = *it;
        layout.marker->draw(paintInfo, layout.matrix);
    }
}

void SVGMarkerLayoutInfo::addLayoutedMarker(SVGResourceMarker* marker, const FloatPoint& origin, float angle)
{
    ASSERT(marker);
    m_layout.append(MarkerLayout(marker, marker->markerTransformation(origin, angle, m_strokeWidth)));
}

}

#endif // ENABLE(SVG)
