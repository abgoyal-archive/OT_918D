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

#ifndef SVGMarkerData_h
#define SVGMarkerData_h

#if ENABLE(SVG)
#include "FloatConversion.h"
#include "Path.h"
#include <wtf/MathExtras.h>

namespace WebCore {

class SVGResourceMarker;

class SVGMarkerData {
public:
    enum Type {
        Unknown = 0,
        Start,
        Mid,
        End
    };

    SVGMarkerData(const Type& type = Unknown, SVGResourceMarker* marker = 0)
        : m_type(type)
        , m_marker(marker)
    {
    }

    FloatPoint origin() const { return m_origin; }
    SVGResourceMarker* marker() const { return m_marker; }

    float currentAngle() const
    {
        FloatSize inslopeChange = m_inslopePoints[1] - m_inslopePoints[0];
        FloatSize outslopeChange = m_outslopePoints[1] - m_outslopePoints[0];

        double inslope = rad2deg(atan2(inslopeChange.height(), inslopeChange.width()));
        double outslope = rad2deg(atan2(outslopeChange.height(), outslopeChange.width()));

        double angle = 0;
        switch (m_type) {
        case Start:
            angle = outslope;
            break;
        case Mid:
            angle = (inslope + outslope) / 2;
            break;
        case End:
            angle = inslope;
            break;
        default:
            ASSERT_NOT_REACHED();
            break;
        }

        return narrowPrecisionToFloat(angle);
    }

    void updateTypeAndMarker(const Type& type, SVGResourceMarker* marker)
    {
        m_type = type;
        m_marker = marker;
    }

    void updateOutslope(const FloatPoint& point)
    {
        m_outslopePoints[0] = m_origin;
        m_outslopePoints[1] = point;
    }

    void updateMarkerDataForPathElement(const PathElement* element)
    {
        FloatPoint* points = element->points;

        switch (element->type) {
        case PathElementAddQuadCurveToPoint:
            // FIXME: https://bugs.webkit.org/show_bug.cgi?id=33115 (PathElementAddQuadCurveToPoint not handled for <marker>)
            m_origin = points[1];
            break;
        case PathElementAddCurveToPoint:
            m_inslopePoints[0] = points[1];
            m_inslopePoints[1] = points[2];
            m_origin = points[2];
            break;
        case PathElementMoveToPoint:
            m_subpathStart = points[0];
        case PathElementAddLineToPoint:
            updateInslope(points[0]);
            m_origin = points[0];
            break;
        case PathElementCloseSubpath:
            updateInslope(points[0]);
            m_origin = m_subpathStart;
            m_subpathStart = FloatPoint();
        }
    }

private:
    void updateInslope(const FloatPoint& point)
    {
        m_inslopePoints[0] = m_origin;
        m_inslopePoints[1] = point;
    }

    Type m_type;
    SVGResourceMarker* m_marker;
    FloatPoint m_origin;
    FloatPoint m_subpathStart;
    FloatPoint m_inslopePoints[2];
    FloatPoint m_outslopePoints[2];
};

}

#endif // ENABLE(SVG)
#endif // SVGMarkerData_h
