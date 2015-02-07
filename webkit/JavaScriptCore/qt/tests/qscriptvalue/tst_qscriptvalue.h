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
    Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies)

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

#ifndef tst_qscriptvalue_h
#define tst_qscriptvalue_h

#include "qscriptengine.h"
#include "qscriptvalue.h"
#include <QtCore/qnumeric.h>
#include <QtTest/qtest.h>

Q_DECLARE_METATYPE(QScriptValue*);
Q_DECLARE_METATYPE(QScriptValue);

class tst_QScriptValue : public QObject {
    Q_OBJECT

public:
    tst_QScriptValue();
    virtual ~tst_QScriptValue();

private slots:
    void toString_data();
    void toString();
    void copyConstructor_data();
    void copyConstructor();
    void assignOperator_data();
    void assignOperator();
    void dataSharing();
    void constructors_data();
    void constructors();
    void call();
    void ctor();

    // Generated test functions.
    void isBool_data();
    void isBool();

    void isBoolean_data();
    void isBoolean();

    void isNumber_data();
    void isNumber();

    void isFunction_data();
    void isFunction();

    void isNull_data();
    void isNull();

    void isObject_data();
    void isObject();

    void isString_data();
    void isString();

    void isUndefined_data();
    void isUndefined();

    void isValid_data();
    void isValid();

    void toNumber_data();
    void toNumber();

    void toBool_data();
    void toBool();

    void toBoolean_data();
    void toBoolean();

private:
    typedef void (tst_QScriptValue::*InitDataFunction)();
    typedef void (tst_QScriptValue::*DefineDataFunction)(const char*);
    void dataHelper(InitDataFunction init, DefineDataFunction define);
    QTestData& newRow(const char* tag);

    typedef void (tst_QScriptValue::*TestFunction)(const char*, const QScriptValue&);
    void testHelper(TestFunction fun);

    // Generated functions

    void initScriptValues();

    void isBool_initData();
    void isBool_makeData(const char* expr);
    void isBool_test(const char* expr, const QScriptValue& value);

    void isBoolean_initData();
    void isBoolean_makeData(const char* expr);
    void isBoolean_test(const char* expr, const QScriptValue& value);

    void isNumber_initData();
    void isNumber_makeData(const char* expr);
    void isNumber_test(const char* expr, const QScriptValue&);

    void isFunction_initData();
    void isFunction_makeData(const char* expr);
    void isFunction_test(const char* expr, const QScriptValue& value);

    void isNull_initData();
    void isNull_makeData(const char* expr);
    void isNull_test(const char* expr, const QScriptValue& value);

    void isObject_initData();
    void isObject_makeData(const char* expr);
    void isObject_test(const char* expr, const QScriptValue& value);

    void isString_initData();
    void isString_makeData(const char* expr);
    void isString_test(const char* expr, const QScriptValue& value);

    void isUndefined_initData();
    void isUndefined_makeData(const char* expr);
    void isUndefined_test(const char* expr, const QScriptValue& value);

    void isValid_initData();
    void isValid_makeData(const char* expr);
    void isValid_test(const char* expr, const QScriptValue& value);

    void toNumber_initData();
    void toNumber_makeData(const char*);
    void toNumber_test(const char*, const QScriptValue&);

    void toBool_initData();
    void toBool_makeData(const char*);
    void toBool_test(const char*, const QScriptValue&);

    void toBoolean_initData();
    void toBoolean_makeData(const char*);
    void toBoolean_test(const char*, const QScriptValue&);

private:
    QScriptEngine* engine;
    QHash<QString, QScriptValue> m_values;
    QString m_currentExpression;
};

#define DEFINE_TEST_FUNCTION(name) \
void tst_QScriptValue::name##_data() { dataHelper(&tst_QScriptValue::name##_initData, &tst_QScriptValue::name##_makeData); } \
void tst_QScriptValue::name() { testHelper(&tst_QScriptValue::name##_test); }



#endif // tst_qscriptvalue_h
