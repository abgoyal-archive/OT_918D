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
 * Copyright 2009, The Android Open Source Project
 * Copyright (C) 2006, 2007 Apple Inc.  All rights reserved.
 * Copyright (C) 2006 Michael Emmel mike.emmel@gmail.com 
 * Copyright (C) 2007 Holger Hans Peter Freyther
 * All rights reserved.
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
#include "LocalizedStrings.h"

#include "NotImplemented.h"
#include "PlatformBridge.h"
#include "PlatformString.h"

namespace WebCore {

// The following two strings are used for File Upload form control, ie
// <input type="file">. The first is the text that appears on the button
// that when pressed, the user can browse for and select a file. The
// second string is rendered on the screen when no file has been selected.
String fileButtonChooseFileLabel()
{
    return *(PlatformBridge::globalLocalizedName(
            PlatformBridge::FileUploadLabel));
}

String fileButtonNoFileSelectedLabel()
{
    notImplemented();
    return String();
}

String contextMenuItemTagInspectElement()
{
    return String("Inspect Element");
}

String unknownFileSizeText()
{
    return String("Unknown");
}

String contextMenuItemTagOpenLinkInNewWindow()
{
    notImplemented();
    return String();
}

String contextMenuItemTagDownloadLinkToDisk()
{
    notImplemented();
    return String();
}

String contextMenuItemTagCopyLinkToClipboard()
{
    notImplemented();
    return String();
}

String contextMenuItemTagOpenImageInNewWindow()
{
    notImplemented();
    return String();
}

String contextMenuItemTagDownloadImageToDisk()
{
    notImplemented();
    return String();
}

String contextMenuItemTagCopyImageToClipboard()
{
    notImplemented();
    return String();
}

String contextMenuItemTagOpenFrameInNewWindow()
{
    notImplemented();
    return String();
}

String contextMenuItemTagCopy()
{
    notImplemented();
    return String();
}

String contextMenuItemTagGoBack()
{
    notImplemented();
    return String();
}

String contextMenuItemTagGoForward()
{
    notImplemented();
    return String();
}

String contextMenuItemTagStop()
{
    notImplemented();
    return String();
}

String contextMenuItemTagReload()
{
    notImplemented();
    return String();
}

String contextMenuItemTagCut()
{
    notImplemented();
    return String();
}

String contextMenuItemTagPaste()
{
    notImplemented();
    return String();
}

String contextMenuItemTagNoGuessesFound()
{
    notImplemented();
    return String();
}

String contextMenuItemTagIgnoreSpelling()
{
    notImplemented();
    return String();
}

String contextMenuItemTagLearnSpelling()
{
    notImplemented();
    return String();
}

String contextMenuItemTagSearchWeb()
{
    notImplemented();
    return String();
}

String contextMenuItemTagLookUpInDictionary()
{
    notImplemented();
    return String();
}

String contextMenuItemTagOpenLink()
{
    notImplemented();
    return String();
}

String contextMenuItemTagIgnoreGrammar()
{
    notImplemented();
    return String();
}

String contextMenuItemTagSpellingMenu()
{
    notImplemented();
    return String();
}

String contextMenuItemTagShowSpellingPanel(bool show)
{
    notImplemented();
    return String();
}

String contextMenuItemTagCheckSpelling()
{
    notImplemented();
    return String();
}

String contextMenuItemTagCheckSpellingWhileTyping()
{
    notImplemented();
    return String();
}

String contextMenuItemTagCheckGrammarWithSpelling()
{
    notImplemented();
    return String();
}

String contextMenuItemTagFontMenu()
{
    notImplemented();
    return String();
}

String contextMenuItemTagBold()
{
    notImplemented();
    return String();
}

String contextMenuItemTagItalic()
{
    notImplemented();
    return String();
}

String contextMenuItemTagUnderline()
{
    notImplemented();
    return String();
}

String contextMenuItemTagOutline()
{
    notImplemented();
    return String();
}

String contextMenuItemTagWritingDirectionMenu()
{
    notImplemented();
    return String();
}

String contextMenuItemTagTextDirectionMenu()
{
    notImplemented();
    return String();
}

String contextMenuItemTagDefaultDirection()
{
    notImplemented();
    return String();
}

String contextMenuItemTagLeftToRight()
{
    notImplemented();
    return String();
}

String contextMenuItemTagRightToLeft()
{
    notImplemented();
    return String();
}

String imageTitle(const String& filename, const IntSize& size)
{
    notImplemented();
    return String();
}

String mediaElementLoadingStateText()
{
    notImplemented();
    return String();
}

String mediaElementLiveBroadcastStateText()
{
    notImplemented();
    return String();
}

String localizedMediaControlElementString(const String& controlName)
{
    notImplemented();
    return String();
}

String localizedMediaControlElementHelpText(const String& controlName)
{
    notImplemented();
    return String();
}

String localizedMediaTimeDescription(const String& controlName)
{
    notImplemented();
    return String();
}

String searchableIndexIntroduction()
{
    notImplemented();
    return String();
}

String resetButtonDefaultLabel()
{
    return *(PlatformBridge::globalLocalizedName(
            PlatformBridge::ResetLabel));
}

String submitButtonDefaultLabel()
{
    return *(PlatformBridge::globalLocalizedName(
            PlatformBridge::SubmitLabel));
}

String inputElementAltText()
{
    notImplemented();
    return String();
}

String validationMessageValueMissingText()
{
    notImplemented();
    return String();
}

String validationMessageTypeMismatchText()
{
    notImplemented();
    return String();
}

String validationMessagePatternMismatchText()
{
    notImplemented();
    return String();
}

String validationMessageTooLongText()
{
    notImplemented();
    return String();
}

String validationMessageRangeUnderflowText()
{
    notImplemented();
    return String();
}

String validationMessageRangeOverflowText()
{
    notImplemented();
    return String();
}

String validationMessageStepMismatchText()
{
    notImplemented();
    return String();
}

} // namespace WebCore