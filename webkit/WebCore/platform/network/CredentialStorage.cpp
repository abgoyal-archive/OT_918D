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
 * Copyright (C) 2009 Apple Inc. All Rights Reserved.
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
#include "CredentialStorage.h"

#include "CString.h"
#include "Credential.h"
#include "KURL.h"
#include "ProtectionSpaceHash.h"
#include "StringHash.h"
#include <wtf/HashMap.h>
#include <wtf/HashSet.h>
#include <wtf/StdLibExtras.h>

namespace WebCore {

typedef HashMap<ProtectionSpace, Credential> ProtectionSpaceToCredentialMap;
static ProtectionSpaceToCredentialMap& protectionSpaceToCredentialMap()
{
    DEFINE_STATIC_LOCAL(ProtectionSpaceToCredentialMap, map, ());
    return map;
}

static HashSet<String>& originsWithCredentials()
{
    DEFINE_STATIC_LOCAL(HashSet<String>, set, ());
    return set;
}

typedef HashMap<String, ProtectionSpace> PathToDefaultProtectionSpaceMap;
static PathToDefaultProtectionSpaceMap& pathToDefaultProtectionSpaceMap()
{
    DEFINE_STATIC_LOCAL(PathToDefaultProtectionSpaceMap, map, ());
    return map;
}

static String originStringFromURL(const KURL& url)
{
    if (url.port())
        return url.protocol() + "://" + url.host() + String::format(":%i/", url.port());
    
    return url.protocol() + "://" + url.host() + "/";
}

static String protectionSpaceMapKeyFromURL(const KURL& url)
{
    ASSERT(url.isValid());

    // Remove the last path component that is not a directory to determine the subtree for which credentials will apply.
    // We keep a leading slash, but remove a trailing one.
    String directoryURL = url.string().substring(0, url.pathEnd());
    unsigned directoryURLPathStart = url.pathStart();
    ASSERT(directoryURL[directoryURLPathStart] == '/');
    if (directoryURL.length() > directoryURLPathStart + 1) {
        int index = directoryURL.reverseFind('/');
        ASSERT(index > 0);
        directoryURL = directoryURL.substring(0, (static_cast<unsigned>(index) != directoryURLPathStart) ? index : directoryURLPathStart + 1);
    }
    ASSERT(directoryURL.length() == directoryURLPathStart + 1 || directoryURL[directoryURL.length() - 1] != '/');

    return directoryURL;
}

void CredentialStorage::set(const Credential& credential, const ProtectionSpace& protectionSpace, const KURL& url)
{
    ASSERT(protectionSpace.isProxy() || url.protocolInHTTPFamily());
    ASSERT(protectionSpace.isProxy() || url.isValid());

    protectionSpaceToCredentialMap().set(protectionSpace, credential);
    if (!protectionSpace.isProxy()) {
        originsWithCredentials().add(originStringFromURL(url));

        ProtectionSpaceAuthenticationScheme scheme = protectionSpace.authenticationScheme();
        if (scheme == ProtectionSpaceAuthenticationSchemeHTTPBasic || scheme == ProtectionSpaceAuthenticationSchemeDefault) {
            // The map can contain both a path and its subpath - while redundant, this makes lookups faster.
            pathToDefaultProtectionSpaceMap().set(protectionSpaceMapKeyFromURL(url), protectionSpace);
        }
    }
}

Credential CredentialStorage::get(const ProtectionSpace& protectionSpace)
{
    return protectionSpaceToCredentialMap().get(protectionSpace);
}

static PathToDefaultProtectionSpaceMap::iterator findDefaultProtectionSpaceForURL(const KURL& url)
{
    ASSERT(url.protocolInHTTPFamily());
    ASSERT(url.isValid());

    PathToDefaultProtectionSpaceMap& map = pathToDefaultProtectionSpaceMap();

    // Don't spend time iterating the path for origins that don't have any credentials.
    if (!originsWithCredentials().contains(originStringFromURL(url)))
        return map.end();

    String directoryURL = protectionSpaceMapKeyFromURL(url);
    unsigned directoryURLPathStart = url.pathStart();
    while (true) {
        PathToDefaultProtectionSpaceMap::iterator iter = map.find(directoryURL);
        if (iter != map.end())
            return iter;

        if (directoryURL.length() == directoryURLPathStart + 1)  // path is "/" already, cannot shorten it any more
            return map.end();

        int index = directoryURL.reverseFind('/', -2);
        ASSERT(index > 0);
        directoryURL = directoryURL.substring(0, (static_cast<unsigned>(index) == directoryURLPathStart) ? index + 1 : index);
        ASSERT(directoryURL.length() > directoryURLPathStart);
        ASSERT(directoryURL.length() == directoryURLPathStart + 1 || directoryURL[directoryURL.length() - 1] != '/');
    }
}

bool CredentialStorage::set(const Credential& credential, const KURL& url)
{
    ASSERT(url.protocolInHTTPFamily());
    ASSERT(url.isValid());
    PathToDefaultProtectionSpaceMap::iterator iter = findDefaultProtectionSpaceForURL(url);
    if (iter == pathToDefaultProtectionSpaceMap().end())
        return false;
    ASSERT(originsWithCredentials().contains(originStringFromURL(url)));
    protectionSpaceToCredentialMap().set(iter->second, credential);
    return true;
}

Credential CredentialStorage::get(const KURL& url)
{
    PathToDefaultProtectionSpaceMap::iterator iter = findDefaultProtectionSpaceForURL(url);
    if (iter == pathToDefaultProtectionSpaceMap().end())
        return Credential();
    return protectionSpaceToCredentialMap().get(iter->second);
}

} // namespace WebCore
