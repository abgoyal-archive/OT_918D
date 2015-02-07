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
 * Copyright (C) 2003 Lars Knoll (knoll@kde.org)
 * Copyright (C) 2004, 2005, 2006, 2008 Apple Inc. All rights reserved.
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
 */

#include "config.h"
#include "CSSParserValues.h"
#include "CSSPrimitiveValue.h"
#include "CSSFunctionValue.h"
#include "CSSQuirkPrimitiveValue.h"

namespace WebCore {

bool CSSParserValue::isVariable() const
{
    return unit == CSSPrimitiveValue::CSS_PARSER_VARIABLE_FUNCTION_SYNTAX;
}

CSSParserValueList::~CSSParserValueList()
{
    size_t numValues = m_values.size();
    for (size_t i = 0; i < numValues; i++) {
        if (m_values[i].unit == CSSParserValue::Function)
            delete m_values[i].function;
    }
}

void CSSParserValueList::addValue(const CSSParserValue& v)
{
    if (v.unit == CSSPrimitiveValue::CSS_PARSER_VARIABLE_FUNCTION_SYNTAX) // isVariable() is not inlined. This is hot.
        m_variablesCount++;
    m_values.append(v);
}
    
void CSSParserValueList::deleteValueAt(unsigned i)
{ 
    if (m_values[i].isVariable())
        m_variablesCount--;
    m_values.remove(i);
}

PassRefPtr<CSSValue> CSSParserValue::createCSSValue()
{
    RefPtr<CSSValue> parsedValue;
    if (id)
        parsedValue = CSSPrimitiveValue::createIdentifier(id);
    else if (unit == CSSPrimitiveValue::CSS_IDENT)
        parsedValue = CSSPrimitiveValue::create(string, CSSPrimitiveValue::CSS_PARSER_IDENTIFIER);
    else if (unit == CSSPrimitiveValue::CSS_NUMBER && isInt)
        parsedValue = CSSPrimitiveValue::create(fValue, CSSPrimitiveValue::CSS_PARSER_INTEGER);
    else if (unit == CSSParserValue::Operator) {
        RefPtr<CSSPrimitiveValue> primitiveValue = CSSPrimitiveValue::createIdentifier(iValue);
        primitiveValue->setPrimitiveType(CSSPrimitiveValue::CSS_PARSER_OPERATOR);
        parsedValue = primitiveValue;
    } else if (unit == CSSParserValue::Function)
        parsedValue = CSSFunctionValue::create(function);
    else if (unit == CSSPrimitiveValue::CSS_STRING || unit == CSSPrimitiveValue::CSS_URI || unit == CSSPrimitiveValue::CSS_PARSER_HEXCOLOR || isVariable())
        parsedValue = CSSPrimitiveValue::create(string, (CSSPrimitiveValue::UnitTypes)unit);
    else if (unit >= CSSPrimitiveValue::CSS_NUMBER && unit <= CSSPrimitiveValue::CSS_KHZ)
        parsedValue = CSSPrimitiveValue::create(fValue, (CSSPrimitiveValue::UnitTypes)unit);
    else if (unit >= CSSPrimitiveValue::CSS_TURN && unit <= CSSPrimitiveValue::CSS_REMS) // CSS3 Values and Units
        parsedValue = CSSPrimitiveValue::create(fValue, (CSSPrimitiveValue::UnitTypes)unit);
    else if (unit >= CSSParserValue::Q_EMS)
        parsedValue = CSSQuirkPrimitiveValue::create(fValue, CSSPrimitiveValue::CSS_EMS);
    return parsedValue;
}

}

