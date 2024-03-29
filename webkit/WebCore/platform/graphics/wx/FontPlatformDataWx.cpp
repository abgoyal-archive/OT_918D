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
 * Copyright (C) 2007 Kevin Ollivier <kevino@theolliviers.com>
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
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

#include "config.h"
#include "FontPlatformData.h"

#include "FontDescription.h"
#include "PlatformString.h"
#include <wx/defs.h>
#include <wx/gdicmn.h>
#include <wx/font.h>

namespace WebCore {

static wxFontFamily fontFamilyToWxFontFamily(const int family)
{
    switch (family) {
        case FontDescription::StandardFamily:
            return wxFONTFAMILY_DEFAULT;
        case FontDescription::SerifFamily:
            return wxFONTFAMILY_ROMAN;
        case FontDescription::SansSerifFamily:
            return wxFONTFAMILY_MODERN;
        case FontDescription::MonospaceFamily:
            return wxFONTFAMILY_TELETYPE; // TODO: Check these are equivalent
        case FontDescription::CursiveFamily:
            return wxFONTFAMILY_SCRIPT;
        case FontDescription::FantasyFamily:
            return wxFONTFAMILY_DECORATIVE;
        default:
            return wxFONTFAMILY_DEFAULT;
    }
}

static wxFontWeight fontWeightToWxFontWeight(FontWeight weight)
{
    if (weight >= FontWeight600)
        return wxFONTWEIGHT_BOLD;

    if (weight <= FontWeight300)
        return wxFONTWEIGHT_LIGHT;
    
    return wxFONTWEIGHT_NORMAL;
} 

static int italicToWxFontStyle(bool isItalic)
{
    if (isItalic)
        return wxFONTSTYLE_ITALIC;
    
    return wxFONTSTYLE_NORMAL;
}

FontPlatformData::FontPlatformData(const FontDescription& desc, const AtomicString& family)
{
// NB: The Windows wxFont constructor has two forms, one taking a wxSize (with pixels)
// and one taking an int (points). When points are used, Windows calculates
// a pixel size using an algorithm which causes the size to be way off. However,
// this is a moot issue on Linux and Mac as they only accept the point argument. So,
// we use the pixel size constructor on Windows, but we use point size on Linux and Mac.
#if __WXMSW__
    m_font = new FontHolder(new wxFont(   wxSize(0, -desc.computedPixelSize()), 
                                fontFamilyToWxFontFamily(desc.genericFamily()), 
                                italicToWxFontStyle(desc.italic()),
                                fontWeightToWxFontWeight(desc.weight()),
                                false,
                                family.string()
                            )
                        ); 
#else
    m_font = new FontHolder(new wxFont(   desc.computedPixelSize(), 
                                fontFamilyToWxFontFamily(desc.genericFamily()), 
                                italicToWxFontStyle(desc.italic()),
                                fontWeightToWxFontWeight(desc.weight()),
                                false,
                                family.string()
                            )
                        ); 
#endif
    m_fontState = VALID;
     
}

unsigned FontPlatformData::computeHash() const {
        wxFont* thisFont = m_font->font();
        ASSERT(thisFont && thisFont->IsOk());
        
        // make a hash that is unique for this font, but not globally unique - that is,
        // a font whose properties are equal should generate the same hash
        uintptr_t hashCodes[6] = { thisFont->GetPointSize(), thisFont->GetFamily(), thisFont->GetStyle(), 
                                    thisFont->GetWeight(), thisFont->GetUnderlined(), 
                                    StringImpl::computeHash(thisFont->GetFaceName().utf8_str()) };
        
        return StringImpl::computeHash(reinterpret_cast<UChar*>(hashCodes), sizeof(hashCodes) / sizeof(UChar));
}

FontPlatformData::~FontPlatformData()
{
    m_fontState = UNINITIALIZED;
    m_font = 0;
}

#ifndef NDEBUG
String FontPlatformData::description() const
{
    return String();
}
#endif

#if OS(WINDOWS)
bool FontPlatformData::useGDI() const
{
    return true;
}

HFONT FontPlatformData::hfont() const
{
    return static_cast<HFONT>(m_font->font()->GetHFONT());
}
#endif

}
