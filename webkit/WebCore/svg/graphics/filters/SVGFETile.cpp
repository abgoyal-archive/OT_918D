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
    Copyright (C) 2008 Alex Mathews <possessedpenguinbob@gmail.com>
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
#include "SVGFETile.h"

#include "AffineTransform.h"
#include "Filter.h"
#include "GraphicsContext.h"
#include "Pattern.h"
#include "SVGRenderTreeAsText.h"

namespace WebCore {

FETile::FETile(FilterEffect* in)
    : FilterEffect()
    , m_in(in)
{
}

PassRefPtr<FETile> FETile::create(FilterEffect* in)
{
    return adoptRef(new FETile(in));
}

FloatRect FETile::uniteChildEffectSubregions(Filter* filter)
{
    m_in->calculateEffectRect(filter);
    return filter->filterRegion();
}

void FETile::apply(Filter* filter)
{
    m_in->apply(filter);
    if (!m_in->resultImage())
        return;

    GraphicsContext* filterContext = getEffectContext();
    if (!filterContext)
        return;

    setIsAlphaImage(m_in->isAlphaImage());

    IntRect tileRect = enclosingIntRect(m_in->scaledSubRegion());

    // Source input needs more attention. It has the size of the filterRegion but gives the
    // size of the cutted sourceImage back. This is part of the specification and optimization.
    if (m_in->isSourceInput()) {
        FloatRect filterRegion = filter->filterRegion();
        filterRegion.scale(filter->filterResolution().width(), filter->filterResolution().height());
        tileRect = enclosingIntRect(filterRegion);
    }

    OwnPtr<ImageBuffer> tileImage = ImageBuffer::create(tileRect.size());
    GraphicsContext* tileImageContext = tileImage->context();
    tileImageContext->drawImage(m_in->resultImage()->image(), DeviceColorSpace, IntPoint());
    RefPtr<Pattern> pattern = Pattern::create(tileImage->image(), true, true);

    AffineTransform matrix;
    matrix.translate(m_in->scaledSubRegion().x() - scaledSubRegion().x(), m_in->scaledSubRegion().y() - scaledSubRegion().y());
    pattern.get()->setPatternSpaceTransform(matrix);

    filterContext->setFillPattern(pattern);
    filterContext->fillRect(FloatRect(FloatPoint(), scaledSubRegion().size()));
}

void FETile::dump()
{
}

TextStream& FETile::externalRepresentation(TextStream& ts) const
{
    ts << "[type=TILE]";
    FilterEffect::externalRepresentation(ts);
    return ts;
}

} // namespace WebCore

#endif // ENABLE(SVG) && ENABLE(FILTERS)

