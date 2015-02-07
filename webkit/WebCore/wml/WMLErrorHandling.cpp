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

/**
 * Copyright (C) 2008 Torch Mobile Inc. All rights reserved. (http://www.torchmobile.com/)
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
 *
 */

#include "config.h"

#if ENABLE(WML)
#include "WMLErrorHandling.h"

#include "Console.h"
#include "CString.h"
#include "Frame.h"
#include "Document.h"
#include "DOMWindow.h"
#include "XMLTokenizer.h"

namespace WebCore {

void reportWMLError(Document* doc, WMLErrorCode error)
{
    if (!doc || error == WMLErrorUnknown)
        return;

    String errorMessage = errorMessageForErrorCode(error);
    XMLTokenizer* tokenizer = static_cast<XMLTokenizer*>(doc->tokenizer());
    if (tokenizer && error != WMLErrorDeckNotAccessible) {
        // Some errors are reported as result of an insertedIntoDocument() call.
        // If this happened, parsing has been stopped, and the document fragment
        // is wrapped in a XHTML error document. That means insertedIntoDocument()
        // will be called again - do NOT report the error twice, that would result
        // in an infinite error reporting loop.
        if (!tokenizer->wellFormed())
            return;

        tokenizer->handleError(XMLTokenizer::fatal, errorMessage.latin1().data(), tokenizer->lineNumber(), tokenizer->columnNumber());
    } else {
        Frame* frame = doc->frame();
        if (!frame)
            return;

        DOMWindow* domWindow = frame->domWindow();
        if (!domWindow)
            return;

        Console* console = domWindow->console();
        if (!console)
            return;

        console->addMessage(WMLMessageSource, LogMessageType, ErrorMessageLevel, errorMessage, 0, String());
    }
}

String errorMessageForErrorCode(WMLErrorCode error)
{
    switch (error) {
    case WMLErrorConflictingEventBinding:
        return "Conflicting event bindings within an element.";
    case WMLErrorDeckNotAccessible:
        return "Deck not accessible.";
    case WMLErrorDuplicatedDoElement:
        return "At least two do elements share a name, which is not allowed.";
    case WMLErrorForbiddenTaskInAnchorElement:
        return "Forbidden task contained in anchor element.";
    case WMLErrorInvalidColumnsNumberInTable:
        return "A table contains an invalid number of columns.";
    case WMLErrorInvalidVariableName:
        return "A variable name contains invalid characters.";
    case WMLErrorInvalidVariableReference:
        return "A variable reference uses invalid syntax.";
    case WMLErrorInvalidVariableReferenceLocation:
        return "A variable reference is placed in an invalid location.";
    case WMLErrorMultipleAccessElements:
        return "Only one access element is allowed in a deck.";
    case WMLErrorMultipleTemplateElements:
        return "Only one template element is allowed in a deck.";
    case WMLErrorNoCardInDocument:
        return "No card contained in document.";
    case WMLErrorMultipleTimerElements:
        return "Only one timer element is allowed in a card.";
    case WMLErrorUnknown:
        return String();
    };

    ASSERT_NOT_REACHED();
    return String();
}

}

#endif