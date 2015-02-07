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
    Copyright (C) 2004, 2005, 2006, 2007 Nikolas Zimmermann <zimmermann@kde.org>
                  2004, 2005 Rob Buis <buis@kde.org>
                  2005 Eric Seidel <eric@webkit.org>
                  2009 Dirk Schulze <krit@webkit.org>

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

#if ENABLE(SVG) && ENABLE(FILTERS)
#include "SVGFEDisplacementMap.h"

#include "CanvasPixelArray.h"
#include "Filter.h"
#include "GraphicsContext.h"
#include "ImageData.h"
#include "SVGRenderTreeAsText.h"

namespace WebCore {

FEDisplacementMap::FEDisplacementMap(FilterEffect* in, FilterEffect* in2, ChannelSelectorType xChannelSelector,
    ChannelSelectorType yChannelSelector, const float& scale)
    : FilterEffect()
    , m_in(in)
    , m_in2(in2)
    , m_xChannelSelector(xChannelSelector)
    , m_yChannelSelector(yChannelSelector)
    , m_scale(scale)
{
}

PassRefPtr<FEDisplacementMap> FEDisplacementMap::create(FilterEffect* in, FilterEffect* in2,
    ChannelSelectorType xChannelSelector, ChannelSelectorType yChannelSelector, const float& scale)
{
    return adoptRef(new FEDisplacementMap(in, in2, xChannelSelector, yChannelSelector, scale));
}

ChannelSelectorType FEDisplacementMap::xChannelSelector() const
{
    return m_xChannelSelector;
}

void FEDisplacementMap::setXChannelSelector(const ChannelSelectorType xChannelSelector)
{
    m_xChannelSelector = xChannelSelector;
}

ChannelSelectorType FEDisplacementMap::yChannelSelector() const
{
    return m_yChannelSelector;
}

void FEDisplacementMap::setYChannelSelector(const ChannelSelectorType yChannelSelector)
{
    m_yChannelSelector = yChannelSelector;
}

float FEDisplacementMap::scale() const
{
    return m_scale;
}

void FEDisplacementMap::setScale(float scale)
{
    m_scale = scale;
}

void FEDisplacementMap::apply(Filter* filter)
{
    m_in->apply(filter);
    m_in2->apply(filter);
    if (!m_in->resultImage() || !m_in2->resultImage())
        return;

    if (m_xChannelSelector == CHANNEL_UNKNOWN || m_yChannelSelector == CHANNEL_UNKNOWN)
        return;

    if (!getEffectContext())
        return;

    IntRect effectADrawingRect = calculateDrawingIntRect(m_in->scaledSubRegion());
    RefPtr<CanvasPixelArray> srcPixelArrayA(m_in->resultImage()->getPremultipliedImageData(effectADrawingRect)->data());

    IntRect effectBDrawingRect = calculateDrawingIntRect(m_in2->scaledSubRegion());
    RefPtr<CanvasPixelArray> srcPixelArrayB(m_in2->resultImage()->getUnmultipliedImageData(effectBDrawingRect)->data());

    IntRect imageRect(IntPoint(), resultImage()->size());
    RefPtr<ImageData> imageData = ImageData::create(imageRect.width(), imageRect.height());

    ASSERT(srcPixelArrayA->length() == srcPixelArrayB->length());

    float scaleX = m_scale / 255.f * filter->filterResolution().width();
    float scaleY = m_scale / 255.f * filter->filterResolution().height();
    float scaleAdjustmentX = (0.5f - 0.5f * m_scale) * filter->filterResolution().width();
    float scaleAdjustmentY = (0.5f - 0.5f * m_scale) * filter->filterResolution().height();
    int stride = imageRect.width() * 4;
    for (int y = 0; y < imageRect.height(); ++y) {
        int line = y * stride;
        for (int x = 0; x < imageRect.width(); ++x) {
            int dstIndex = line + x * 4;
            int srcX = x + static_cast<int>(scaleX * srcPixelArrayB->get(dstIndex + m_xChannelSelector - 1) + scaleAdjustmentX);
            int srcY = y + static_cast<int>(scaleY * srcPixelArrayB->get(dstIndex + m_yChannelSelector - 1) + scaleAdjustmentY);
            for (unsigned channel = 0; channel < 4; ++channel) {
                if (srcX < 0 || srcX >= imageRect.width() || srcY < 0 || srcY >= imageRect.height())
                    imageData->data()->set(dstIndex + channel, static_cast<unsigned char>(0));
                else {
                    unsigned char pixelValue = srcPixelArrayA->get(srcY * stride + srcX * 4 + channel);
                    imageData->data()->set(dstIndex + channel, pixelValue);
                }
            }

        }
    }
    resultImage()->putPremultipliedImageData(imageData.get(), imageRect, IntPoint());
}

void FEDisplacementMap::dump()
{
}

static TextStream& operator<<(TextStream& ts, ChannelSelectorType t)
{
    switch (t)
    {
        case CHANNEL_UNKNOWN:
            ts << "UNKNOWN"; break;
        case CHANNEL_R:
            ts << "RED"; break;
        case CHANNEL_G:
            ts << "GREEN"; break;
        case CHANNEL_B:
            ts << "BLUE"; break;
        case CHANNEL_A:
            ts << "ALPHA"; break;
    }
    return ts;
}

TextStream& FEDisplacementMap::externalRepresentation(TextStream& ts) const
{
    ts << "[type=DISPLACEMENT-MAP] ";
    FilterEffect::externalRepresentation(ts);
    ts << " [in2=" << m_in2.get() << "]"
        << " [scale=" << m_scale << "]"
        << " [x channel selector=" << m_xChannelSelector << "]"
        << " [y channel selector=" << m_yChannelSelector << "]";
    return ts;
}

} // namespace WebCore

#endif // ENABLE(SVG) && ENABLE(FILTERS)
