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
#include "SVGFETurbulence.h"
#include "SVGRenderTreeAsText.h"
#include "Filter.h"

namespace WebCore {

FETurbulence::FETurbulence(TurbulanceType type, const float& baseFrequencyX, const float& baseFrequencyY,
    const int& numOctaves, const float& seed, bool stitchTiles)
    : FilterEffect()
    , m_type(type)
    , m_baseFrequencyX(baseFrequencyX)
    , m_baseFrequencyY(baseFrequencyY)
    , m_numOctaves(numOctaves)
    , m_seed(seed)
    , m_stitchTiles(stitchTiles)
{
}

PassRefPtr<FETurbulence> FETurbulence::create(TurbulanceType type, const float& baseFrequencyX, const float& baseFrequencyY,
    const int& numOctaves, const float& seed, bool stitchTiles)
{
    return adoptRef(new FETurbulence(type, baseFrequencyX, baseFrequencyY, numOctaves, seed, stitchTiles));
}

TurbulanceType FETurbulence::type() const
{
    return m_type;
}

void FETurbulence::setType(TurbulanceType type)
{
    m_type = type;
}

float FETurbulence::baseFrequencyY() const
{
    return m_baseFrequencyY;
}

void FETurbulence::setBaseFrequencyY(float baseFrequencyY)
{
    m_baseFrequencyY = baseFrequencyY;
}

float FETurbulence::baseFrequencyX() const
{
    return m_baseFrequencyX;
}

void FETurbulence::setBaseFrequencyX(float baseFrequencyX)
{
       m_baseFrequencyX = baseFrequencyX;
}

float FETurbulence::seed() const
{
    return m_seed; 
}

void FETurbulence::setSeed(float seed)
{
    m_seed = seed;
}

int FETurbulence::numOctaves() const
{
    return m_numOctaves;
}

void FETurbulence::setNumOctaves(bool numOctaves)
{
    m_numOctaves = numOctaves;
}

bool FETurbulence::stitchTiles() const
{
    return m_stitchTiles;
}

void FETurbulence::setStitchTiles(bool stitch)
{
    m_stitchTiles = stitch;
}

void FETurbulence::apply(Filter*)
{
}

void FETurbulence::dump()
{
}

static TextStream& operator<<(TextStream& ts, TurbulanceType t)
{
    switch (t)
    {
        case FETURBULENCE_TYPE_UNKNOWN:
            ts << "UNKNOWN"; break;
        case FETURBULENCE_TYPE_TURBULENCE:
            ts << "TURBULANCE"; break;
        case FETURBULENCE_TYPE_FRACTALNOISE:
            ts << "NOISE"; break;
    }
    return ts;
}

TextStream& FETurbulence::externalRepresentation(TextStream& ts) const
{
    ts << "[type=TURBULENCE] ";
    FilterEffect::externalRepresentation(ts);
    ts << " [turbulence type=" << type() << "]"
        << " [base frequency x=" << baseFrequencyX() << " y=" << baseFrequencyY() << "]"
        << " [seed=" << seed() << "]"
        << " [num octaves=" << numOctaves() << "]"
        << " [stitch tiles=" << stitchTiles() << "]";

    return ts;
}

} // namespace WebCore

#endif // ENABLE(SVG) && ENABLE(FILTERS)
