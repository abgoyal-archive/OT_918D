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
#include "SVGFEConvolveMatrix.h"
#include "Filter.h"
#include "SVGRenderTreeAsText.h"

namespace WebCore {

FEConvolveMatrix::FEConvolveMatrix(FilterEffect* in, FilterEffect* in2, const FloatSize& kernelSize, 
    const float& divisor, const float& bias, const FloatSize& targetOffset, EdgeModeType edgeMode,
    const FloatPoint& kernelUnitLength, bool preserveAlpha, const Vector<float>& kernelMatrix)
    : FilterEffect()
    , m_in(in)
    , m_in2(in2)
    , m_kernelSize(kernelSize)
    , m_divisor(divisor)
    , m_bias(bias)
    , m_targetOffset(targetOffset)
    , m_edgeMode(edgeMode)
    , m_kernelUnitLength(kernelUnitLength)
    , m_preserveAlpha(preserveAlpha)
    , m_kernelMatrix(kernelMatrix)
{
}

PassRefPtr<FEConvolveMatrix> FEConvolveMatrix::create(FilterEffect* in, FilterEffect* in2, const FloatSize& kernelSize, 
    const float& divisor, const float& bias, const FloatSize& targetOffset, EdgeModeType edgeMode, 
    const FloatPoint& kernelUnitLength, bool preserveAlpha, const Vector<float>& kernelMatrix)
{
    return adoptRef(new FEConvolveMatrix(in, in2, kernelSize, divisor, bias, targetOffset, edgeMode, kernelUnitLength,
        preserveAlpha, kernelMatrix));
}


FloatSize FEConvolveMatrix::kernelSize() const
{
    return m_kernelSize;
}

void FEConvolveMatrix::setKernelSize(FloatSize kernelSize)
{
    m_kernelSize = kernelSize; 
}

const Vector<float>& FEConvolveMatrix::kernel() const
{
    return m_kernelMatrix; 
}

void FEConvolveMatrix::setKernel(const Vector<float>& kernel)
{
    m_kernelMatrix = kernel; 
}

float FEConvolveMatrix::divisor() const
{
    return m_divisor; 
}

void FEConvolveMatrix::setDivisor(float divisor)
{
    m_divisor = divisor; 
}

float FEConvolveMatrix::bias() const
{
    return m_bias; 
}

void FEConvolveMatrix::setBias(float bias)
{
    m_bias = bias; 
}

FloatSize FEConvolveMatrix::targetOffset() const
{
    return m_targetOffset; 
}

void FEConvolveMatrix::setTargetOffset(FloatSize targetOffset)
{
    m_targetOffset = targetOffset; 
}

EdgeModeType FEConvolveMatrix::edgeMode() const
{
    return m_edgeMode; 
}

void FEConvolveMatrix::setEdgeMode(EdgeModeType edgeMode)
{
    m_edgeMode = edgeMode; 
}

FloatPoint FEConvolveMatrix::kernelUnitLength() const
{
    return m_kernelUnitLength; 
}

void FEConvolveMatrix::setKernelUnitLength(FloatPoint kernelUnitLength)
{
    m_kernelUnitLength = kernelUnitLength; 
}

bool FEConvolveMatrix::preserveAlpha() const
{
    return m_preserveAlpha; 
}

void FEConvolveMatrix::setPreserveAlpha(bool preserveAlpha)
{
    m_preserveAlpha = preserveAlpha; 
}

void FEConvolveMatrix::apply(Filter*)
{
}

void FEConvolveMatrix::dump()
{
}

static TextStream& operator<<(TextStream& ts, EdgeModeType t)
{
    switch (t)
    {
        case EDGEMODE_UNKNOWN:
            ts << "UNKNOWN";break;
        case EDGEMODE_DUPLICATE:
            ts << "DUPLICATE";break;
        case EDGEMODE_WRAP:
            ts << "WRAP"; break;
        case EDGEMODE_NONE:
            ts << "NONE"; break;
    }
    return ts;
}

TextStream& FEConvolveMatrix::externalRepresentation(TextStream& ts) const
{
    ts << "[type=CONVOLVE-MATRIX] ";
    FilterEffect::externalRepresentation(ts);
    ts << " [order " << m_kernelSize << "]"
       << " [kernel matrix=" << m_kernelMatrix  << "]"
       << " [divisor=" << m_divisor << "]"
       << " [bias=" << m_bias << "]"
       << " [target " << m_targetOffset << "]"
       << " [edge mode=" << m_edgeMode << "]"
       << " [kernel unit length " << m_kernelUnitLength << "]"
       << " [preserve alpha=" << m_preserveAlpha << "]";
    return ts;
}

}; // namespace WebCore

#endif // ENABLE(SVG) && ENABLE(FILTERS)
