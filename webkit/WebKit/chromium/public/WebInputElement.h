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
 * Copyright (C) 2009 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef WebInputElement_h
#define WebInputElement_h

#include "WebElement.h"

#if WEBKIT_IMPLEMENTATION
namespace WebCore { class HTMLInputElement; }
namespace WTF { template <typename T> class PassRefPtr; }
#endif

namespace WebKit {

    // Provides readonly access to some properties of a DOM input element node.
    class WebInputElement : public WebElement {
    public:
        WebInputElement() : WebElement() { }
        WebInputElement(const WebInputElement& n) : WebElement(n) { }

        WebInputElement& operator=(const WebInputElement& n) { WebElement::assign(n); return *this; }
        WEBKIT_API void assign(const WebInputElement& n) { WebElement::assign(n); }

#if WEBKIT_IMPLEMENTATION
        WebInputElement(const WTF::PassRefPtr<WebCore::HTMLInputElement>&);
        WebInputElement& operator=(const WTF::PassRefPtr<WebCore::HTMLInputElement>&);
        operator WTF::PassRefPtr<WebCore::HTMLInputElement>() const;
#endif

        enum InputType {
            Text = 0,
            Password,
            IsIndex,
            CheckBox,
            Radio,
            Submit,
            Reset,
            File,
            Hidden,
            Image,
            Button,
            Search,
            Range,
            Email,
            Number,
            Telephone,
            URL,
            Color,
            Date,
            DateTime,
            DateTimeLocal,
            Month,
            Time,
            Week
        };

        WEBKIT_API bool autoComplete() const;
        WEBKIT_API bool isEnabledFormControl() const;
        WEBKIT_API InputType inputType() const;
        WEBKIT_API WebString formControlType() const;
        WEBKIT_API bool isActivatedSubmit() const;
        WEBKIT_API void setActivatedSubmit(bool);
        WEBKIT_API void setValue(const WebString& value);
        WEBKIT_API WebString value() const;
        WEBKIT_API void setAutofilled(bool);
        WEBKIT_API void dispatchFormControlChangeEvent();
        WEBKIT_API void setSelectionRange(int, int);
        WEBKIT_API WebString name() const;
        // Returns the name that should be used for the specified |element| when
        // storing autofill data.  This is either the field name or its id, an empty
        // string if it has no name and no id.
        WEBKIT_API WebString nameForAutofill() const;
    };

} // namespace WebKit

#endif
