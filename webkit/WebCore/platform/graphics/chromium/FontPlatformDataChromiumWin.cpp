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
 * Copyright (C) 2006, 2007 Apple Computer, Inc.
 * Copyright (c) 2006, 2007, 2008, 2009, Google Inc. All rights reserved.
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
#include "FontPlatformData.h"

#include <windows.h>
#include <objidl.h>
#include <mlang.h>

#include "ChromiumBridge.h"
#include "SkiaFontWin.h"

namespace WebCore {

FontPlatformData::FontPlatformData(WTF::HashTableDeletedValueType)
    : m_font(hashTableDeletedFontValue())
    , m_size(-1)
    , m_scriptCache(0)
    , m_scriptFontProperties(0)
{
}

FontPlatformData::FontPlatformData()
    : m_font(0)
    , m_size(0)
    , m_scriptCache(0)
    , m_scriptFontProperties(0)
{
}

FontPlatformData::FontPlatformData(HFONT font, float size)
    : m_font(RefCountedHFONT::create(font))
    , m_size(size)
    , m_scriptCache(0)
    , m_scriptFontProperties(0)
{
}

// FIXME: this constructor is needed for SVG fonts but doesn't seem to do much
FontPlatformData::FontPlatformData(float size, bool bold, bool oblique)
    : m_font(0)
    , m_size(size)
    , m_scriptCache(0)
    , m_scriptFontProperties(0)
{
}

FontPlatformData::FontPlatformData(const FontPlatformData& data)
    : m_font(data.m_font)
    , m_size(data.m_size)
    , m_scriptCache(0)
    , m_scriptFontProperties(0)
{
}

FontPlatformData& FontPlatformData::operator=(const FontPlatformData& data)
{
    if (this != &data) {
        m_font = data.m_font;
        m_size = data.m_size;

        // The following fields will get re-computed if necessary.
        ScriptFreeCache(&m_scriptCache);
        m_scriptCache = 0;

        delete m_scriptFontProperties;
        m_scriptFontProperties = 0;
    } 
    return *this;
}

FontPlatformData::~FontPlatformData()
{
    ScriptFreeCache(&m_scriptCache);
    m_scriptCache = 0;

    delete m_scriptFontProperties;
    m_scriptFontProperties = 0;
}

FontPlatformData::RefCountedHFONT::~RefCountedHFONT()
{
    if (m_hfont != reinterpret_cast<HFONT>(-1)) {
        SkiaWinOutlineCache::removePathsForFont(m_hfont);
        DeleteObject(m_hfont);
    }
}

FontPlatformData::RefCountedHFONT* FontPlatformData::hashTableDeletedFontValue()
{
    static RefPtr<RefCountedHFONT> deletedValue =
        RefCountedHFONT::create(reinterpret_cast<HFONT>(-1));
    return deletedValue.get();
}

SCRIPT_FONTPROPERTIES* FontPlatformData::scriptFontProperties() const
{
    if (!m_scriptFontProperties) {
        m_scriptFontProperties = new SCRIPT_FONTPROPERTIES;
        memset(m_scriptFontProperties, 0, sizeof(SCRIPT_FONTPROPERTIES));
        m_scriptFontProperties->cBytes = sizeof(SCRIPT_FONTPROPERTIES);
        HRESULT result = ScriptGetFontProperties(0, scriptCache(),
                                                 m_scriptFontProperties);
        if (result == E_PENDING) {
            HDC dc = GetDC(0);
            HGDIOBJ oldFont = SelectObject(dc, hfont());
            HRESULT hr = ScriptGetFontProperties(dc, scriptCache(),
                                                 m_scriptFontProperties);
            if (S_OK != hr) {
                if (ChromiumBridge::ensureFontLoaded(hfont())) {
                    // FIXME: Handle gracefully the error if this call also fails.
                    hr = ScriptGetFontProperties(dc, scriptCache(),
                                                 m_scriptFontProperties);
                    if (S_OK != hr) {
                        LOG_ERROR("Unable to get the font properties after second attempt");
                    }
                }
            }

            SelectObject(dc, oldFont);
            ReleaseDC(0, dc);
        }
    }
    return m_scriptFontProperties;
}

#ifndef NDEBUG
String FontPlatformData::description() const
{
    return String();
}
#endif

}
