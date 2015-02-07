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
    Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies)

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

#ifndef qscriptengine_p_h
#define qscriptengine_p_h

#include "qscriptconverter_p.h"
#include "qscriptengine.h"
#include "qscriptvalue.h"
#include <JavaScriptCore/JavaScript.h>
#include <QtCore/qshareddata.h>
#include <QtCore/qstring.h>

class QScriptEngine;

class QScriptEnginePrivate : public QSharedData {
public:
    static QScriptEnginePtr get(const QScriptEngine* q) { Q_ASSERT(q); return q->d_ptr; }
    static QScriptEngine* get(const QScriptEnginePrivate* d) { Q_ASSERT(d); return d->q_ptr; }

    QScriptEnginePrivate(const QScriptEngine*);
    ~QScriptEnginePrivate();

    QScriptValuePrivate* evaluate(const QString& program, const QString& fileName, int lineNumber);
    inline void collectGarbage();

    inline JSValueRef makeJSValue(double number) const;
    inline JSValueRef makeJSValue(int number) const;
    inline JSValueRef makeJSValue(uint number) const;
    inline JSValueRef makeJSValue(const QString& string) const;
    inline JSValueRef makeJSValue(bool number) const;
    inline JSValueRef makeJSValue(QScriptValue::SpecialValue value) const;

    inline JSGlobalContextRef context() const;
private:
    QScriptEngine* q_ptr;
    JSGlobalContextRef m_context;
};

void QScriptEnginePrivate::collectGarbage()
{
    JSGarbageCollect(m_context);
}

JSValueRef QScriptEnginePrivate::makeJSValue(double number) const
{
    return JSValueMakeNumber(m_context, number);
}

JSValueRef QScriptEnginePrivate::makeJSValue(int number) const
{
    return JSValueMakeNumber(m_context, number);
}

JSValueRef QScriptEnginePrivate::makeJSValue(uint number) const
{
    return JSValueMakeNumber(m_context, number);
}

JSValueRef QScriptEnginePrivate::makeJSValue(const QString& string) const
{
    return JSValueMakeString(m_context, QScriptConverter::toString(string));
}

JSValueRef QScriptEnginePrivate::makeJSValue(bool value) const
{
    return JSValueMakeBoolean(m_context, value);
}

JSValueRef QScriptEnginePrivate::makeJSValue(QScriptValue::SpecialValue value) const
{
    if (value == QScriptValue::NullValue)
        return JSValueMakeNull(m_context);
    return JSValueMakeUndefined(m_context);
}

JSGlobalContextRef QScriptEnginePrivate::context() const
{
    return m_context;
}

#endif
