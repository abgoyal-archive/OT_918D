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
 * Copyright (C) 2009 Kevin Ollivier.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer. 
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution. 
 * 3.  Neither the name of Apple Computer, Inc. ("Apple") nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "WebSettings.h"

#include "PlatformString.h"
#include "Settings.h"

void wxWebSettings::SetDefaultFixedFontSize(int size)
{
    if (m_settings)
        m_settings->setDefaultFixedFontSize(size);
}
    
int wxWebSettings::GetDefaultFixedFontSize() const
{
    if (m_settings)
        return m_settings->defaultFixedFontSize();
        
    return 0;
}
    
void wxWebSettings::SetDefaultFontSize(int size)
{
    if (m_settings)
        m_settings->setDefaultFontSize(size);
}

int wxWebSettings::GetDefaultFontSize() const
{
    if (m_settings)
        return m_settings->defaultFontSize();
        
    return 0;
}
    
void wxWebSettings::SetMinimumFontSize(int size)
{
    if (m_settings)
        m_settings->setMinimumFontSize(size);
}

int wxWebSettings::GetMinimumFontSize() const
{
    if (m_settings)
        return m_settings->minimumFontSize();
        
    return 0;
}
    
void wxWebSettings::SetLoadsImagesAutomatically(bool loadAutomatically)
{
    if (m_settings)
        m_settings->setLoadsImagesAutomatically(loadAutomatically);
}
    
bool wxWebSettings::LoadsImagesAutomatically() const
{
    if (m_settings)
        return m_settings->loadsImagesAutomatically();
        
    return false;
}
    
void wxWebSettings::SetJavaScriptEnabled(bool enabled)
{
    if (m_settings)
        m_settings->setJavaScriptEnabled(enabled);
}

bool wxWebSettings::IsJavaScriptEnabled() const
{
    if (m_settings)
        return m_settings->isJavaScriptEnabled();
        
    return false;
}
    
void wxWebSettings::SetDatabasesEnabled(bool enabled)
{
    if (m_settings)
        m_settings->setDatabasesEnabled(enabled);
}
    
bool wxWebSettings::AreDatabasesEnabled() const
{
    if (m_settings)
        return m_settings->databasesEnabled();
        
    return false;
}
    
void wxWebSettings::SetLocalStoragePath(const wxString& path)
{
    if (m_settings)
        m_settings->setLocalStorageDatabasePath(path);
}

wxString wxWebSettings::GetLocalStoragePath() const
{
    if (m_settings)
        return m_settings->localStorageDatabasePath();
        
    return wxEmptyString;
}

void wxWebSettings::SetEditableLinkBehavior(wxEditableLinkBehavior behavior)
{
    WebCore::EditableLinkBehavior webCoreBehavior;
    if (m_settings) {
        switch (behavior) {
        case wxEditableLinkAlwaysLive:
            webCoreBehavior = WebCore::EditableLinkAlwaysLive;
            break;
        case wxEditableLinkOnlyLiveWithShiftKey:
            webCoreBehavior = WebCore::EditableLinkOnlyLiveWithShiftKey;
            break;
        case wxEditableLinkLiveWhenNotFocused:
            webCoreBehavior = WebCore::EditableLinkLiveWhenNotFocused;
            break;
        case wxEditableLinkNeverLive:
            webCoreBehavior = WebCore::EditableLinkNeverLive;
            break;
        default:
            webCoreBehavior = WebCore::EditableLinkDefaultBehavior;
        }
        m_settings->setEditableLinkBehavior(webCoreBehavior);
    }
}

wxEditableLinkBehavior wxWebSettings::GetEditableLinkBehavior() const
{
    wxEditableLinkBehavior behavior = wxEditableLinkDefaultBehavior;
    if (m_settings) {
        WebCore::EditableLinkBehavior webCoreBehavior = m_settings->editableLinkBehavior();
        switch (webCoreBehavior) {
        case WebCore::EditableLinkAlwaysLive:
            behavior = wxEditableLinkAlwaysLive;
            break;
        case WebCore::EditableLinkOnlyLiveWithShiftKey:
            behavior = wxEditableLinkOnlyLiveWithShiftKey;
            break;
        case WebCore::EditableLinkLiveWhenNotFocused:
            behavior = wxEditableLinkLiveWhenNotFocused;
            break;
        case WebCore::EditableLinkNeverLive:
            behavior = wxEditableLinkNeverLive;
            break;
        default:
            behavior = wxEditableLinkDefaultBehavior;
        }
    }
    return behavior;
}

void wxWebSettings::SetPluginsEnabled(bool enabled)
{
    if (m_settings)
        m_settings->setPluginsEnabled(enabled);
}  

bool wxWebSettings::ArePluginsEnabled() const
{
    if (m_settings)
        return m_settings->arePluginsEnabled();
        
    return false;
}
