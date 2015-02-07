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
 *  Copyright (C) 2007-2009 Torch Mobile Inc.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this library; see the file COPYING.LIB.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301, USA.
 *
 */

#include "config.h"
#include "GlyphPageTreeNode.h"

#include "Font.h"
#include "FontCache.h"
#include "FontData.h"
#include "SimpleFontData.h"

namespace WebCore {

DWORD getKnownFontCodePages(const wchar_t* family);

typedef unsigned (*funcGetCharCodePages)(unsigned short c, unsigned& lastPos);
funcGetCharCodePages getCharCodePages = 0;

bool GlyphPage::fill(unsigned offset, unsigned length, UChar* buffer, unsigned bufferLength, const SimpleFontData* fontData)
{
    if (length != bufferLength)
        return false;

    if (fontData->platformData().hfont()) {
        DWORD fontCodePages = fontData->platformData().codePages();
        if (fontCodePages) {
            if (getCharCodePages) {
                unsigned lastPos = 0;
                for (unsigned i = 0; i < bufferLength; ++i) {
                    DWORD actualCodePages = getCharCodePages(buffer[i], lastPos);
                    if (!actualCodePages || (actualCodePages & fontCodePages))
                        setGlyphDataForIndex(offset + i, buffer[i], fontData);
                    else
                        setGlyphDataForIndex(offset + i, buffer[i], 0);
                }
                return true;
#if defined(IMLANG_FONT_LINK) && (IMLANG_FONT_LINK == 2)
            } else if (IMLangFontLink2* langFontLink = fontCache()->getFontLinkInterface()) {
#else
            } else if (IMLangFontLink* langFontLink = fontCache()->getFontLinkInterface()) {
#endif
                for (unsigned i = 0; i < bufferLength; ++i) {
                    DWORD actualCodePages;
                    langFontLink->GetCharCodePages(buffer[i], &actualCodePages);
                    if (!actualCodePages || (actualCodePages & fontCodePages))
                        setGlyphDataForIndex(offset + i, buffer[i], fontData);
                    else
                        setGlyphDataForIndex(offset + i, buffer[i], 0);
                }
                return true;
            }
        }
    }

    for (unsigned i = 0; i < length; ++i)
        setGlyphDataForIndex(offset + i, buffer[i], fontData);

    return true;
}

}

