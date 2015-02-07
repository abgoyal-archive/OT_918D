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

/**
 * (C) 1999-2003 Lars Knoll (knoll@kde.org)
 * Copyright (C) 2004, 2005, 2006, 2007 Apple Inc. All rights reserved.
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
#include "CSSValueList.h"

#include "CSSParserValues.h"
#include "PlatformString.h"

namespace WebCore {

CSSValueList::CSSValueList(bool isSpaceSeparated)
    : m_isSpaceSeparated(isSpaceSeparated)
{
}

CSSValueList::CSSValueList(CSSParserValueList* list)
    : m_isSpaceSeparated(true)
{
    if (list) {
        unsigned s = list->size();
        for (unsigned i = 0; i < s; ++i) {
            CSSParserValue* v = list->valueAt(i);
            append(v->createCSSValue());
        }
    }
}

CSSValueList::~CSSValueList()
{
}

CSSValue* CSSValueList::item(unsigned index)
{
    if (index >= m_values.size())
        return 0;
    return m_values[index].get();
}

unsigned short CSSValueList::cssValueType() const
{
    return CSS_VALUE_LIST;
}

void CSSValueList::append(PassRefPtr<CSSValue> val)
{
    m_values.append(val);
}

void CSSValueList::prepend(PassRefPtr<CSSValue> val)
{
    m_values.prepend(val);
}

bool CSSValueList::removeAll(CSSValue* val)
{
    bool found = false;
    // FIXME: we should be implementing operator== to CSSValue and its derived classes
    // to make comparison more flexible and fast.
    for (size_t index = 0; index < m_values.size(); index++) {
        if (m_values.at(index)->cssText() == val->cssText()) {
            m_values.remove(index);
            found = true;
        }
    }
    
    return found;
}
    
bool CSSValueList::hasValue(CSSValue* val)
{
    // FIXME: we should be implementing operator== to CSSValue and its derived classes
    // to make comparison more flexible and fast.
    for (size_t index = 0; index < m_values.size(); index++) {
        if (m_values.at(index)->cssText() == val->cssText())
            return true;
    }
    return false;
}

PassRefPtr<CSSValueList> CSSValueList::copy()
{
    PassRefPtr<CSSValueList> newList = m_isSpaceSeparated ? createSpaceSeparated() : createCommaSeparated();
    for (size_t index = 0; index < m_values.size(); index++)
        newList->append(item(index));
    return newList;
}

String CSSValueList::cssText() const
{
    String result = "";

    unsigned size = m_values.size();
    for (unsigned i = 0; i < size; i++) {
        if (!result.isEmpty()) {
            if (m_isSpaceSeparated)
                result += " ";
            else
                result += ", ";
        }
        result += m_values[i]->cssText();
    }

    return result;
}

CSSParserValueList* CSSValueList::createParserValueList() const
{
    unsigned s = m_values.size();
    if (!s)
        return 0;
    CSSParserValueList* result = new CSSParserValueList;
    for (unsigned i = 0; i < s; ++i)
        result->addValue(m_values[i]->parserValue());
    return result;
}

void CSSValueList::addSubresourceStyleURLs(ListHashSet<KURL>& urls, const CSSStyleSheet* styleSheet)
{
    size_t size = m_values.size();
    for (size_t i = 0; i < size; ++i)
        m_values[i]->addSubresourceStyleURLs(urls, styleSheet);
}

} // namespace WebCore
