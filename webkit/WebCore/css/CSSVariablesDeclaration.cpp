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
 * Copyright (C) 2008 Apple Inc. All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

#include "config.h"
#include "CSSVariablesDeclaration.h"

#include "CSSParser.h"
#include "CSSRule.h"
#include "CSSValueList.h"
#include "Document.h"
#include "ExceptionCode.h"

namespace WebCore {

CSSVariablesDeclaration::CSSVariablesDeclaration(StyleBase* parent, const Vector<String>& names, const Vector<RefPtr<CSSValue> >& values)
    : StyleBase(parent)
{
    m_variableNames = names;
    ASSERT(names.size() == values.size());
    unsigned s = names.size();
    for (unsigned i = 0; i < s; ++i)
        addParsedVariable(names[i], values[i], false);
}

CSSVariablesDeclaration::~CSSVariablesDeclaration()
{
}

String CSSVariablesDeclaration::getVariableValue(const String& variableName)
{
    CSSValue* val = m_variablesMap.get(variableName).get();
    if (val)
        return val->cssText();
    return "";
}

String CSSVariablesDeclaration::removeVariable(const String& variableName, ExceptionCode&)
{
    // FIXME: The spec has this method taking an exception code but no exceptions are
    // specified as being thrown.
    RefPtr<CSSValue> val = m_variablesMap.take(variableName);
    String result = val ? val->cssText() : "";
    if (val) {
        int s = m_variableNames.size();
        for (int i = 0; i < s; ++i) {
            if (m_variableNames[i] == variableName) {
                m_variableNames.remove(i);
                i--;
                s--;
            }
        }
        
        setNeedsStyleRecalc();
    }
    
    // FIXME: Communicate this change so that the document will update.
    return result;
}

void CSSVariablesDeclaration::setVariable(const String& variableName, const String& variableValue, ExceptionCode& excCode)
{
    // Try to parse the variable value into a Value*.  If it fails we throw an exception.
    CSSParser parser(useStrictParsing());
    if (!parser.parseVariable(this, variableName, variableValue)) // If the parse succeeds, it will call addParsedVariable (our internal method for doing the add) with the parsed Value*.
        excCode = SYNTAX_ERR;
    else
        setNeedsStyleRecalc();
}

void CSSVariablesDeclaration::addParsedVariable(const String& variableName, PassRefPtr<CSSValue> variableValue, bool updateNamesList)
{
// FIXME: Disabling declarations as variable values for now since they no longer have a common base class with CSSValues.
#if 0
    variableValue->setParent(this); // Needed to connect variables that are CSSMutableStyleDeclarations, since the parent couldn't be set until now.
#endif

    // Don't leak duplicates.  For multiple variables with the same name, the last one
    // declared will win.
    CSSValue* current = m_variablesMap.take(variableName).get();
    if (!current && updateNamesList)
        m_variableNames.append(variableName);
    m_variablesMap.set(variableName, variableValue);

    // FIXME: Communicate this change so the document will update.
}

CSSValueList* CSSVariablesDeclaration::getParsedVariable(const String& variableName)
{
    CSSValue* result = m_variablesMap.get(variableName).get();
    if (result->isValueList())
        return static_cast<CSSValueList*>(result);
    return 0;
}

CSSMutableStyleDeclaration* CSSVariablesDeclaration::getParsedVariableDeclarationBlock(const String&)
{
// FIXME: Disabling declarations as variable values for now since they no longer have a common base class with CSSValues.
#if 0
    StyleBase* result = m_variablesMap.get(variableName).get();

    if (result->isMutableStyleDeclaration())
        return static_cast<CSSMutableStyleDeclaration*>(result);
#endif
    return 0;
}

unsigned CSSVariablesDeclaration::length() const
{
    return m_variableNames.size();
}

String CSSVariablesDeclaration::item(unsigned index)
{
    if (index >= m_variableNames.size())
        return "";
    return m_variableNames[index];
}

CSSRule* CSSVariablesDeclaration::parentRule()
{
    return (parent() && parent()->isRule()) ? static_cast<CSSRule*>(parent()) : 0;
}

String CSSVariablesDeclaration::cssText() const
{
    String result = "{ ";
    unsigned s = m_variableNames.size();
    for (unsigned i = 0; i < s; ++i) {
        result += m_variableNames[i] + ": ";
        result += m_variablesMap.get(m_variableNames[i])->cssText();
        if (i < s - 1)
            result += "; ";
    }
    result += " }";
    return result;
}

void CSSVariablesDeclaration::setCssText(const String&)
{
    // FIXME: It's not clear if this is actually settable.
}

void CSSVariablesDeclaration::setNeedsStyleRecalc()
{
    // FIXME: Make this much better (it has the same problem CSSMutableStyleDeclaration does).
    StyleBase* root = this;
    while (StyleBase* parent = root->parent())
        root = parent;
    if (root->isCSSStyleSheet())
        static_cast<CSSStyleSheet*>(root)->doc()->updateStyleSelector();
}

}
