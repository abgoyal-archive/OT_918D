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
 * Copyright (C) 2009 Apple Inc. All rights reserved.
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
#include "UserContentURLPattern.h"
#include "KURL.h"
#include <wtf/StdLibExtras.h>

namespace WebCore {

bool UserContentURLPattern::matchesPatterns(const KURL& url, const Vector<String>* whitelist, const Vector<String>* blacklist)
{
    // In order for a URL to be a match it has to be present in the whitelist and not present in the blacklist.
    // If there is no whitelist at all, then all URLs are assumed to be in the whitelist.
    bool matchesWhitelist = !whitelist || whitelist->isEmpty();
    if (!matchesWhitelist) {
        for (unsigned i = 0; i < whitelist->size(); ++i) {
            UserContentURLPattern contentPattern(whitelist->at(i));
            if (contentPattern.matches(url)) {
                matchesWhitelist = true;
                break;
            }
        }
    }

    bool matchesBlacklist = false;
    if (blacklist) {
        for (unsigned i = 0; i < blacklist->size(); ++i) {
            UserContentURLPattern contentPattern(blacklist->at(i));
            if (contentPattern.matches(url)) {
                matchesBlacklist = true;
                break;
            }
        }
    }

    return matchesWhitelist && !matchesBlacklist;
}

bool UserContentURLPattern::parse(const String& pattern)
{
    DEFINE_STATIC_LOCAL(const String, schemeSeparator, ("://"));

    int schemeEndPos = pattern.find(schemeSeparator);
    if (schemeEndPos == -1)
        return false;

    m_scheme = pattern.left(schemeEndPos);

    int hostStartPos = schemeEndPos + schemeSeparator.length();
    if (hostStartPos >= static_cast<int>(pattern.length()))
        return false;

    int pathStartPos = 0;

    if (m_scheme == "file")
        pathStartPos = hostStartPos;
    else {
        int hostEndPos = pattern.find("/", hostStartPos);
        if (hostEndPos == -1)
            return false;
    
        m_host = pattern.substring(hostStartPos, hostEndPos - hostStartPos);

        // The first component can be '*', which means to match all subdomains.
        Vector<String> hostComponents;
        m_host.split(".", hostComponents); 
        if (hostComponents[0] == "*") {
            m_matchSubdomains = true;
            m_host = "";
            for (unsigned i = 1; i < hostComponents.size(); ++i) {
                m_host = m_host + hostComponents[i];
                if (i < hostComponents.size() - 1)
                    m_host = m_host + ".";
            }
        }
        
        // No other '*' can occur in the host.
        if (m_host.find("*") != -1)
            return false;

        pathStartPos = hostEndPos;
    }

    m_path = pattern.right(pattern.length() - pathStartPos);

    return true;
}

bool UserContentURLPattern::matches(const KURL& test) const
{
    if (m_invalid)
        return false;

    if (test.protocol() != m_scheme)
        return false;

    if (!matchesHost(test))
        return false;

    return matchesPath(test);
}

bool UserContentURLPattern::matchesHost(const KURL& test) const
{
    if (test.host() == m_host)
        return true;

    if (!m_matchSubdomains)
        return false;

    // If we're matching subdomains, and we have no host, that means the pattern
    // was <scheme>://*/<whatever>, so we match anything.
    if (!m_host.length())
        return true;

    // Check if the test host is a subdomain of our host.
    return test.host().endsWith(m_host, false);
}

struct MatchTester
{
    const String m_pattern;
    unsigned m_patternIndex;
    
    const String m_test;
    unsigned m_testIndex;
    
    MatchTester(const String& pattern, const String& test)
    : m_pattern(pattern)
    , m_patternIndex(0)
    , m_test(test)
    , m_testIndex(0)
    {
    }
    
    bool testStringFinished() const { return m_testIndex >= m_test.length(); }
    bool patternStringFinished() const { return m_patternIndex >= m_pattern.length(); }

    void eatWildcard()
    {
        while (!patternStringFinished()) {
            if (m_pattern[m_patternIndex] != '*')
                return;
            m_patternIndex++;
        }
    }
    
    void eatSameChars()
    {
        while (!patternStringFinished() && !testStringFinished()) {
            if (m_pattern[m_patternIndex] == '*')
                return;
            if (m_pattern[m_patternIndex] != m_test[m_testIndex])
                return;
            m_patternIndex++;
            m_testIndex++;
        }
    }

    bool test()
    {
        // Eat all the matching chars.
        eatSameChars();

        // If the string is finished, then the pattern must be empty too, or contains
        // only wildcards.
        if (testStringFinished()) {
            eatWildcard();
            if (patternStringFinished())
                return true;
            return false;
        }
  
        // Pattern is empty but not string, this is not a match.
        if (patternStringFinished())
            return false;
        
        // If we don't encounter a *, then we're hosed.
        if (m_pattern[m_patternIndex] != '*')
            return false;
        
        while (!testStringFinished()) {
            MatchTester nextMatch(*this);
            nextMatch.m_patternIndex++;
            if (nextMatch.test())
                return true;
            m_testIndex++;
        }

        // We reached the end of the string.  Let's see if the pattern contains only
        // wildcards.
        eatWildcard();
        return patternStringFinished();
    }
};

bool UserContentURLPattern::matchesPath(const KURL& test) const
{
    MatchTester match(m_path, test.path());
    return match.test();
}

} // namespace WebCore
