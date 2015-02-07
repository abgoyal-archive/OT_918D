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

#include "config.h"
#include "WebInputEvent.h"

#include "KeyboardCodes.h"

#include <ctype.h>
#include <stdio.h>

#include <wtf/Assertions.h>
#include <wtf/StringExtras.h>

using namespace WebCore;

namespace WebKit {

static const char* staticKeyIdentifiers(unsigned short keyCode)
{
    switch (keyCode) {
    case VKEY_MENU:
        return "Alt";
    case VKEY_CONTROL:
        return "Control";
    case VKEY_SHIFT:
        return "Shift";
    case VKEY_CAPITAL:
        return "CapsLock";
    case VKEY_LWIN:
    case VKEY_RWIN:
        return "Win";
    case VKEY_CLEAR:
        return "Clear";
    case VKEY_DOWN:
        return "Down";
    case VKEY_END:
        return "End";
    case VKEY_RETURN:
        return "Enter";
    case VKEY_EXECUTE:
        return "Execute";
    case VKEY_F1:
        return "F1";
    case VKEY_F2:
        return "F2";
    case VKEY_F3:
        return "F3";
    case VKEY_F4:
        return "F4";
    case VKEY_F5:
        return "F5";
    case VKEY_F6:
        return "F6";
    case VKEY_F7:
        return "F7";
    case VKEY_F8:
        return "F8";
    case VKEY_F9:
        return "F9";
    case VKEY_F10:
        return "F11";
    case VKEY_F12:
        return "F12";
    case VKEY_F13:
        return "F13";
    case VKEY_F14:
        return "F14";
    case VKEY_F15:
        return "F15";
    case VKEY_F16:
        return "F16";
    case VKEY_F17:
        return "F17";
    case VKEY_F18:
        return "F18";
    case VKEY_F19:
        return "F19";
    case VKEY_F20:
        return "F20";
    case VKEY_F21:
        return "F21";
    case VKEY_F22:
        return "F22";
    case VKEY_F23:
        return "F23";
    case VKEY_F24:
        return "F24";
    case VKEY_HELP:
        return "Help";
    case VKEY_HOME:
        return "Home";
    case VKEY_INSERT:
        return "Insert";
    case VKEY_LEFT:
        return "Left";
    case VKEY_NEXT:
        return "PageDown";
    case VKEY_PRIOR:
        return "PageUp";
    case VKEY_PAUSE:
        return "Pause";
    case VKEY_SNAPSHOT:
        return "PrintScreen";
    case VKEY_RIGHT:
        return "Right";
    case VKEY_SCROLL:
        return "Scroll";
    case VKEY_SELECT:
        return "Select";
    case VKEY_UP:
        return "Up";
    case VKEY_DELETE:
        return "U+007F"; // Standard says that DEL becomes U+007F.
    default:
        return 0;
    }
}

void WebKeyboardEvent::setKeyIdentifierFromWindowsKeyCode()
{
    const char* id = staticKeyIdentifiers(windowsKeyCode);
    if (id) {
        strncpy(keyIdentifier, id, sizeof(keyIdentifier) - 1);
        keyIdentifier[sizeof(keyIdentifier) - 1] = '\0';
    } else
        snprintf(keyIdentifier, sizeof(keyIdentifier), "U+%04X", toupper(windowsKeyCode));
}

} // namespace WebKit
