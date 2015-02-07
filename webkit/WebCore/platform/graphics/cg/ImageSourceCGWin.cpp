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
#include "ImageSourceCG.h"

#include "StdLibExtras.h" 
#include "StringHash.h"
#include <wtf/HashMap.h>

namespace WebCore {

String MIMETypeForImageSourceType(const String& type)
{
    String mimeType;
    // FIXME: This approach of taking a UTI like public.type and giving back 
    // a MIME type like image/type will work for common image UTIs like jpeg, 
    // png, tiff, gif but won't work for UTIs like: public.jpeg-2000,
    // public.xbitmap-image, com.apple.quicktime-image, and others.
    if (int dotLocation = type.reverseFind('.'))
        mimeType = "image/" + type.substring(dotLocation + 1);
    return mimeType;
}

String preferredExtensionForImageSourceType(const String& type)
{
    if (type.isEmpty())
        return String();

    typedef HashMap<String, String> StringMap;
    DEFINE_STATIC_LOCAL(StringMap, UTIMap, ());
    if (UTIMap.isEmpty()) {
        UTIMap.add("public.html", "html");
        UTIMap.add("public.jpeg", "jpeg");
        UTIMap.add("public.jpeg-2000", "jp2");
        UTIMap.add("public.plain-text", "txt");
        UTIMap.add("public.png", "png");
        UTIMap.add("public.tiff", "tiff");
        UTIMap.add("public.xbitmap-image", "xbm");
        UTIMap.add("public.xml", "xml");
        UTIMap.add("com.adobe.illustrator.ai-image", "ai");
        UTIMap.add("com.adobe.pdf", "pdf");
        UTIMap.add("com.adobe.photoshop-image", "psd");
        UTIMap.add("com.adobe.postscript", "ps");
        UTIMap.add("com.apple.icns", "icns");
        UTIMap.add("com.apple.macpaint-image", "pntg");
        UTIMap.add("com.apple.pict", "pict");
        UTIMap.add("com.apple.quicktime-image", "qtif");
        UTIMap.add("com.apple.webarchive", "webarchive");
        UTIMap.add("com.compuserve.gif", "gif");        
        UTIMap.add("com.ilm.openexr-image", "exr");
        UTIMap.add("com.kodak.flashpix-image", "fpx");
        UTIMap.add("com.microsoft.bmp", "bmp");
        UTIMap.add("com.microsoft.ico", "ico");
        UTIMap.add("com.netscape.javascript-source", "js");
        UTIMap.add("com.sgi.sgi-image", "sgi");
        UTIMap.add("com.truevision.tga-image", "tga");
    }
    return UTIMap.get(type);
}

} // namespace WebCore
