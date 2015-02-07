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
 * This file is part of the XSL implementation.
 *
 * Copyright (C) 2009 Jakub Wieczorek <faw217@gmail.com>
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
 */

#include "config.h"
#include "XSLStyleSheet.h"

#if ENABLE(XSLT)

#include "DOMWindow.h"
#include "Document.h"
#include "Node.h"
#include "NotImplemented.h"
#include "XSLTProcessor.h"
#include "loader.h"

namespace WebCore {

XSLStyleSheet::XSLStyleSheet(Node* parentNode, const String& originalURL, const KURL& finalURL,  bool embedded)
    : StyleSheet(parentNode, originalURL, finalURL)
    , m_ownerDocument(parentNode->document())
    , m_embedded(embedded)
{
}

XSLStyleSheet::~XSLStyleSheet()
{
}

bool XSLStyleSheet::isLoading()
{
    notImplemented();
    return false;
}

void XSLStyleSheet::checkLoaded()
{
    if (ownerNode())
        ownerNode()->sheetLoaded();
}

void XSLStyleSheet::clearDocuments()
{
    notImplemented();
}

DocLoader* XSLStyleSheet::docLoader()
{
    if (!m_ownerDocument)
        return 0;
    return m_ownerDocument->docLoader();
}

bool XSLStyleSheet::parseString(const String& string, bool)
{
    // FIXME: Fix QXmlQuery so that it allows compiling the stylesheet before setting the document
    // to be transformed. This way we could not only check if the stylesheet is correct before using it
    // but also turn XSLStyleSheet::sheetString() into XSLStyleSheet::query() that returns a QXmlQuery.

    m_sheetString = string;
    return !m_sheetString.isEmpty();
}

void XSLStyleSheet::loadChildSheets()
{
    notImplemented();
}

void XSLStyleSheet::loadChildSheet(const String&)
{
    notImplemented();
}

void XSLStyleSheet::setParentStyleSheet(XSLStyleSheet*)
{
    notImplemented();
}

void XSLStyleSheet::markAsProcessed()
{
    notImplemented();
}

} // namespace WebCore

#endif // ENABLE(XSLT)
