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
 * Copyright (C) 2007, 2008, 2009 Apple Inc. All rights reserved.
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
#include "JSHTMLDocument.h"

#include "CharacterNames.h"
#include "Frame.h"
#include "HTMLAllCollection.h"
#include "HTMLBodyElement.h"
#include "HTMLCollection.h"
#include "HTMLDocument.h"
#include "HTMLElement.h"
#include "HTMLIFrameElement.h"
#include "HTMLNames.h"
#include "JSDOMWindow.h"
#include "JSDOMWindowCustom.h"
#include "JSDOMWindowShell.h"
#include "JSHTMLCollection.h"
#include "SegmentedString.h"
#include "Tokenizer.h"
#include <runtime/Error.h>

using namespace JSC;

namespace WebCore {

using namespace HTMLNames;

bool JSHTMLDocument::canGetItemsForName(ExecState*, HTMLDocument* document, const Identifier& propertyName)
{
    AtomicStringImpl* atomicPropertyName = AtomicString::find(propertyName);
    return atomicPropertyName && (document->hasNamedItem(atomicPropertyName) || document->hasExtraNamedItem(atomicPropertyName));
}

JSValue JSHTMLDocument::nameGetter(ExecState* exec, const Identifier& propertyName, const PropertySlot& slot)
{
    JSHTMLDocument* thisObj = static_cast<JSHTMLDocument*>(asObject(slot.slotBase()));
    HTMLDocument* document = static_cast<HTMLDocument*>(thisObj->impl());

    String name = propertyName;
    RefPtr<HTMLCollection> collection = document->documentNamedItems(name);

    unsigned length = collection->length();
    if (!length)
        return jsUndefined();

    if (length == 1) {
        Node* node = collection->firstItem();

        Frame* frame;
        if (node->hasTagName(iframeTag) && (frame = static_cast<HTMLIFrameElement*>(node)->contentFrame()))
            return toJS(exec, frame);

        return toJS(exec, node);
    } 

    return toJS(exec, collection.get());
}

// Custom attributes

JSValue JSHTMLDocument::all(ExecState* exec) const
{
    // If "all" has been overwritten, return the overwritten value
    JSValue v = getDirect(Identifier(exec, "all"));
    if (v)
        return v;

    return toJS(exec, static_cast<HTMLDocument*>(impl())->all().get());
}

void JSHTMLDocument::setAll(ExecState* exec, JSValue value)
{
    // Add "all" to the property map.
    putDirect(Identifier(exec, "all"), value);
}

// Custom functions

JSValue JSHTMLDocument::open(ExecState* exec, const ArgList& args)
{
    // For compatibility with other browsers, pass open calls with more than 2 parameters to the window.
    if (args.size() > 2) {
        Frame* frame = static_cast<HTMLDocument*>(impl())->frame();
        if (frame) {
            JSDOMWindowShell* wrapper = toJSDOMWindowShell(frame, currentWorld(exec));
            if (wrapper) {
                JSValue function = wrapper->get(exec, Identifier(exec, "open"));
                CallData callData;
                CallType callType = function.getCallData(callData);
                if (callType == CallTypeNone)
                    return throwError(exec, TypeError);
                return JSC::call(exec, function, callType, callData, wrapper, args);
            }
        }
        return jsUndefined();
    }

    // document.open clobbers the security context of the document and
    // aliases it with the active security context.
    Document* activeDocument = asJSDOMWindow(exec->lexicalGlobalObject())->impl()->document();

    // In the case of two parameters or fewer, do a normal document open.
    static_cast<HTMLDocument*>(impl())->open(activeDocument);
    return this;
}

enum NewlineRequirement { DoNotAddNewline, DoAddNewline };

static inline void documentWrite(ExecState* exec, const ArgList& args, HTMLDocument* document, NewlineRequirement addNewline)
{
    // DOM only specifies single string argument, but browsers allow multiple or no arguments.

    size_t size = args.size();

    UString firstString = args.at(0).toString(exec);
    SegmentedString segmentedString = String(firstString);
    if (size != 1) {
        if (!size)
            segmentedString.clear();
        else {
            for (size_t i = 1; i < size; ++i) {
                UString subsequentString = args.at(i).toString(exec);
                segmentedString.append(SegmentedString(String(subsequentString)));
            }
        }
    }
    if (addNewline)
        segmentedString.append(SegmentedString(&newlineCharacter, 1));

    Document* activeDocument = asJSDOMWindow(exec->lexicalGlobalObject())->impl()->document();
    document->write(segmentedString, activeDocument);
}

JSValue JSHTMLDocument::write(ExecState* exec, const ArgList& args)
{
    documentWrite(exec, args, static_cast<HTMLDocument*>(impl()), DoNotAddNewline);
    return jsUndefined();
}

JSValue JSHTMLDocument::writeln(ExecState* exec, const ArgList& args)
{
    documentWrite(exec, args, static_cast<HTMLDocument*>(impl()), DoAddNewline);
    return jsUndefined();
}

} // namespace WebCore
