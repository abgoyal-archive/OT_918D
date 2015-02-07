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

#if ENABLE(FILTERS)
#include "FEColorMatrix.h"

#include "CanvasPixelArray.h"
#include "Filter.h"
#include "GraphicsContext.h"
#include "ImageData.h"
#include <math.h>
#include <wtf/MathExtras.h>

namespace WebCore {

FEColorMatrix::FEColorMatrix(FilterEffect* in, ColorMatrixType type, const Vector<float>& values)
    : FilterEffect()
    , m_in(in)
    , m_type(type)
    , m_values(values)
{
}

PassRefPtr<FEColorMatrix> FEColorMatrix::create(FilterEffect* in, ColorMatrixType type, const Vector<float>& values)
{
    return adoptRef(new FEColorMatrix(in, type, values));
}

ColorMatrixType FEColorMatrix::type() const
{
    return m_type;
}

void FEColorMatrix::setType(ColorMatrixType type)
{
    m_type = type;
}

const Vector<float>& FEColorMatrix::values() const
{
    return m_values;
}

void FEColorMatrix::setValues(const Vector<float> &values)
{
    m_values = values;
}

inline void matrix(double& red, double& green, double& blue, double& alpha, const Vector<float>& values)
{
    double r = values[0]  * red + values[1]  * green + values[2]  * blue + values[3]  * alpha;
    double g = values[5]  * red + values[6]  * green + values[7]  * blue + values[8]  * alpha;
    double b = values[10]  * red + values[11]  * green + values[12] * blue + values[13] * alpha;
    double a = values[15] * red + values[16] * green + values[17] * blue + values[18] * alpha;

    red = r;
    green = g;
    blue = b;
    alpha = a;
}

inline void saturate(double& red, double& green, double& blue, const float& s)
{
    double r = (0.213 + 0.787 * s) * red + (0.715 - 0.715 * s) * green + (0.072 - 0.072 * s) * blue;
    double g = (0.213 - 0.213 * s) * red + (0.715 + 0.285 * s) * green + (0.072 - 0.072 * s) * blue;
    double b = (0.213 - 0.213 * s) * red + (0.715 - 0.715 * s) * green + (0.072 + 0.928 * s) * blue;

    red = r;
    green = g;
    blue = b;
}

inline void huerotate(double& red, double& green, double& blue, const float& hue)
{
    double cosHue = cos(hue * piDouble / 180); 
    double sinHue = sin(hue * piDouble / 180); 
    double r = red   * (0.213 + cosHue * 0.787 - sinHue * 0.213) +
               green * (0.715 - cosHue * 0.715 - sinHue * 0.715) +
               blue  * (0.072 - cosHue * 0.072 + sinHue * 0.928);
    double g = red   * (0.213 - cosHue * 0.213 + sinHue * 0.143) +
               green * (0.715 + cosHue * 0.285 + sinHue * 0.140) +
               blue  * (0.072 - cosHue * 0.072 - sinHue * 0.283);
    double b = red   * (0.213 - cosHue * 0.213 - sinHue * 0.787) +
               green * (0.715 - cosHue * 0.715 + sinHue * 0.715) +
               blue  * (0.072 + cosHue * 0.928 + sinHue * 0.072);

    red = r;
    green = g;
    blue = b;
}

inline void luminance(double& red, double& green, double& blue, double& alpha)
{
    alpha = 0.2125 * red + 0.7154 * green + 0.0721 * blue;
    red = 0.;
    green = 0.;
    blue = 0.;
}

template<ColorMatrixType filterType>
void effectType(const PassRefPtr<CanvasPixelArray>& srcPixelArray, PassRefPtr<ImageData>& imageData, const Vector<float>& values)
{
    for (unsigned pixelOffset = 0; pixelOffset < srcPixelArray->length(); pixelOffset++) {
        unsigned pixelByteOffset = pixelOffset * 4;

        unsigned char r = 0, g = 0, b = 0, a = 0;
        srcPixelArray->get(pixelByteOffset, r);
        srcPixelArray->get(pixelByteOffset + 1, g);
        srcPixelArray->get(pixelByteOffset + 2, b);
        srcPixelArray->get(pixelByteOffset + 3, a);

        double red = r, green = g, blue = b, alpha = a;
        
        switch (filterType) {
            case FECOLORMATRIX_TYPE_MATRIX:
                matrix(red, green, blue, alpha, values);
                break;
            case FECOLORMATRIX_TYPE_SATURATE: 
                saturate(red, green, blue, values[0]);
                break;
            case FECOLORMATRIX_TYPE_HUEROTATE:
                huerotate(red, green, blue, values[0]);
                break;
            case FECOLORMATRIX_TYPE_LUMINANCETOALPHA:
                luminance(red, green, blue, alpha);
                break;
        }

        imageData->data()->set(pixelByteOffset, red);
        imageData->data()->set(pixelByteOffset + 1, green);
        imageData->data()->set(pixelByteOffset + 2, blue);
        imageData->data()->set(pixelByteOffset + 3, alpha);
    }
}

void FEColorMatrix::apply(Filter* filter)
{
    m_in->apply(filter);
    if (!m_in->resultImage())
        return;

    GraphicsContext* filterContext = getEffectContext();
    if (!filterContext)
        return;

    filterContext->drawImage(m_in->resultImage()->image(), DeviceColorSpace, calculateDrawingRect(m_in->scaledSubRegion()));

    IntRect imageRect(IntPoint(), resultImage()->size());
    PassRefPtr<ImageData> imageData(resultImage()->getUnmultipliedImageData(imageRect));
    PassRefPtr<CanvasPixelArray> srcPixelArray(imageData->data());

    switch (m_type) {
        case FECOLORMATRIX_TYPE_UNKNOWN:
            break;
        case FECOLORMATRIX_TYPE_MATRIX:
            effectType<FECOLORMATRIX_TYPE_MATRIX>(srcPixelArray, imageData, m_values);
            break;
        case FECOLORMATRIX_TYPE_SATURATE: 
            effectType<FECOLORMATRIX_TYPE_SATURATE>(srcPixelArray, imageData, m_values);
            break;
        case FECOLORMATRIX_TYPE_HUEROTATE:
            effectType<FECOLORMATRIX_TYPE_HUEROTATE>(srcPixelArray, imageData, m_values);
            break;
        case FECOLORMATRIX_TYPE_LUMINANCETOALPHA:
            effectType<FECOLORMATRIX_TYPE_LUMINANCETOALPHA>(srcPixelArray, imageData, m_values);
            setIsAlphaImage(true);
            break;
    }

    resultImage()->putUnmultipliedImageData(imageData.get(), imageRect, IntPoint());
}

void FEColorMatrix::dump()
{
}

} // namespace WebCore

#endif // ENABLE(FILTERS)
