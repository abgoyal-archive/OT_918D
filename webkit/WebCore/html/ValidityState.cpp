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
 * This file is part of the WebKit project.
 *
 * Copyright (C) 2009 Michelangelo De Simone <micdesim@gmail.com>
 * Copyright (C) 2010 Apple Inc. All rights reserved.
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
 *
 */

#include "config.h"
#include "ValidityState.h"

#include "HTMLInputElement.h"
#include "HTMLNames.h"
#include "KURL.h"
#include "LocalizedStrings.h"
#include "RegularExpression.h"
#include <wtf/StdLibExtras.h>

namespace WebCore {

using namespace HTMLNames;

static const char emailPattern[] =
    "[a-z0-9!#$%&'*+/=?^_`{|}~.-]+" // local part
    "@"
    "[a-z0-9-]+(\\.[a-z0-9-]+)+"; // domain part

String ValidityState::validationMessage() const
{
    if (!m_control->willValidate())
        return String();

    if (customError())
        return m_customErrorMessage;
    if (valueMissing())
        return validationMessageValueMissingText();
    if (typeMismatch())
        return validationMessageTypeMismatchText();
    if (patternMismatch())
        return validationMessagePatternMismatchText();
    if (tooLong())
        return validationMessageTooLongText();
    if (rangeUnderflow())
        return validationMessageRangeUnderflowText();
    if (rangeOverflow())
        return validationMessageRangeOverflowText();
    if (stepMismatch())
        return validationMessageStepMismatchText();

    return String();
}

bool ValidityState::typeMismatch() const
{
    if (!m_control->hasTagName(inputTag))
        return false;

    HTMLInputElement* input = static_cast<HTMLInputElement*>(m_control);
    String value = input->value();

    if (value.isEmpty())
        return false;

    switch (input->inputType()) {
    case HTMLInputElement::COLOR:
        return !isValidColorString(value);
    case HTMLInputElement::NUMBER:
        return !HTMLInputElement::formStringToDouble(value, 0);
    case HTMLInputElement::URL:
        return !KURL(KURL(), value).isValid();
    case HTMLInputElement::EMAIL: {
        if (!input->multiple())
            return !isValidEmailAddress(value);
        Vector<String> addresses;
        value.split(',', addresses);
        for (unsigned i = 0; i < addresses.size(); ++i) {
            if (!isValidEmailAddress(addresses[i]))
                return true;
        }
        return false;
    }
    case HTMLInputElement::DATE:
    case HTMLInputElement::DATETIME:
    case HTMLInputElement::DATETIMELOCAL:
    case HTMLInputElement::MONTH:
    case HTMLInputElement::TIME:
    case HTMLInputElement::WEEK:
        return !HTMLInputElement::formStringToDateComponents(input->inputType(), value, 0);
    case HTMLInputElement::BUTTON:
    case HTMLInputElement::CHECKBOX:
    case HTMLInputElement::FILE:
    case HTMLInputElement::HIDDEN:
    case HTMLInputElement::IMAGE:
    case HTMLInputElement::ISINDEX:
    case HTMLInputElement::PASSWORD:
    case HTMLInputElement::RADIO:
    case HTMLInputElement::RANGE:
    case HTMLInputElement::RESET:
    case HTMLInputElement::SEARCH:
    case HTMLInputElement::SUBMIT:
    case HTMLInputElement::TELEPHONE: // FIXME: Is there validation for <input type=telephone>?
    case HTMLInputElement::TEXT:
        return false;
    }

    ASSERT_NOT_REACHED();
    return false;
}

bool ValidityState::rangeUnderflow() const
{
    if (!m_control->hasTagName(inputTag))
        return false;
    return static_cast<HTMLInputElement*>(m_control)->rangeUnderflow();
}

bool ValidityState::rangeOverflow() const
{
    if (!m_control->hasTagName(inputTag))
        return false;
    return static_cast<HTMLInputElement*>(m_control)->rangeOverflow();
}

bool ValidityState::stepMismatch() const
{
    if (!m_control->hasTagName(inputTag))
        return false;
    return static_cast<HTMLInputElement*>(m_control)->stepMismatch();
}

bool ValidityState::valid() const
{
    bool someError = typeMismatch() || stepMismatch() || rangeUnderflow() || rangeOverflow()
        || tooLong() || patternMismatch() || valueMissing() || customError();
    return !someError;
}

bool ValidityState::isValidColorString(const String& value)
{
    if (value.isEmpty())
        return false;
    if (value[0] == '#') {
        // We don't accept #rgb and #aarrggbb formats.
        if (value.length() != 7)
            return false;
    }
    Color color(value);  // This accepts named colors such as "white".
    return color.isValid() && !color.hasAlpha();
}

bool ValidityState::isValidEmailAddress(const String& address)
{
    int addressLength = address.length();
    if (!addressLength)
        return false;

    DEFINE_STATIC_LOCAL(const RegularExpression, regExp, (emailPattern, TextCaseInsensitive));

    int matchLength;
    int matchOffset = regExp.match(address, 0, &matchLength);

    return matchOffset == 0 && matchLength == addressLength;
}

} // namespace
