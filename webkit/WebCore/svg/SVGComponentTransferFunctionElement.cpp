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
    Copyright (C) 2004, 2005, 2007 Nikolas Zimmermann <zimmermann@kde.org>
                  2004, 2005, 2006 Rob Buis <buis@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#include "config.h"

#if ENABLE(SVG) && ENABLE(FILTERS)
#include "SVGComponentTransferFunctionElement.h"

#include "MappedAttribute.h"
#include "SVGFEComponentTransferElement.h"
#include "SVGNames.h"
#include "SVGNumberList.h"

namespace WebCore {

SVGComponentTransferFunctionElement::SVGComponentTransferFunctionElement(const QualifiedName& tagName, Document* doc)
    : SVGElement(tagName, doc)
    , m_type(FECOMPONENTTRANSFER_TYPE_UNKNOWN)
    , m_tableValues(SVGNumberList::create(SVGNames::tableValuesAttr))
    , m_slope(1.0f)
    , m_amplitude(1.0f)
    , m_exponent(1.0f)
{
}

SVGComponentTransferFunctionElement::~SVGComponentTransferFunctionElement()
{
}

void SVGComponentTransferFunctionElement::parseMappedAttribute(MappedAttribute* attr)
{
    const String& value = attr->value();
    if (attr->name() == SVGNames::typeAttr) {
        if (value == "identity")
            setTypeBaseValue(FECOMPONENTTRANSFER_TYPE_IDENTITY);
        else if (value == "table")
            setTypeBaseValue(FECOMPONENTTRANSFER_TYPE_TABLE);
        else if (value == "discrete")
            setTypeBaseValue(FECOMPONENTTRANSFER_TYPE_DISCRETE);
        else if (value == "linear")
            setTypeBaseValue(FECOMPONENTTRANSFER_TYPE_LINEAR);
        else if (value == "gamma")
            setTypeBaseValue(FECOMPONENTTRANSFER_TYPE_GAMMA);
    }
    else if (attr->name() == SVGNames::tableValuesAttr)
        tableValuesBaseValue()->parse(value);
    else if (attr->name() == SVGNames::slopeAttr)
        setSlopeBaseValue(value.toFloat());
    else if (attr->name() == SVGNames::interceptAttr)
        setInterceptBaseValue(value.toFloat());
    else if (attr->name() == SVGNames::amplitudeAttr)
        setAmplitudeBaseValue(value.toFloat());
    else if (attr->name() == SVGNames::exponentAttr)
        setExponentBaseValue(value.toFloat());
    else if (attr->name() == SVGNames::offsetAttr)
        setOffsetBaseValue(value.toFloat());
    else
        SVGElement::parseMappedAttribute(attr);
}

void SVGComponentTransferFunctionElement::synchronizeProperty(const QualifiedName& attrName)
{
    SVGElement::synchronizeProperty(attrName);

    if (attrName == anyQName()) {
        synchronizeType();
        synchronizeTableValues();
        synchronizeSlope();
        synchronizeIntercept();
        synchronizeAmplitude();
        synchronizeExponent();
        synchronizeOffset();
        return;
    }

    if (attrName == SVGNames::typeAttr)
        synchronizeType();
    else if (attrName == SVGNames::tableValuesAttr)
        synchronizeTableValues();
    else if (attrName == SVGNames::slopeAttr)
        synchronizeSlope();
    else if (attrName == SVGNames::interceptAttr)
        synchronizeIntercept();
    else if (attrName == SVGNames::amplitudeAttr)
        synchronizeAmplitude();
    else if (attrName == SVGNames::exponentAttr)
        synchronizeExponent();
    else if (attrName == SVGNames::offsetAttr)
        synchronizeOffset();
}

ComponentTransferFunction SVGComponentTransferFunctionElement::transferFunction() const
{
    ComponentTransferFunction func;
    func.type = (ComponentTransferType) type();
    func.slope = slope();
    func.intercept = intercept();
    func.amplitude = amplitude();
    func.exponent = exponent();
    func.offset = offset();
    SVGNumberList* numbers = tableValues();

    ExceptionCode ec = 0;
    unsigned int nr = numbers->numberOfItems();
    for (unsigned int i = 0; i < nr; i++)
        func.tableValues.append(numbers->getItem(i, ec));
    return func;
}

}

#endif // ENABLE(SVG)
