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
 * Copyright (C) 2008 Apple Inc. All rights reserved.
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
#include "JSClipboard.h"

#include "Clipboard.h"
#include "Element.h"
#include "HTMLImageElement.h"
#include "HTMLNames.h"
#include "IntPoint.h"
#include "JSNode.h"
#include "Node.h"
#include "PlatformString.h"
#include "StringHash.h"
#include <runtime/ArrayPrototype.h>
#include <runtime/Error.h>
#include <wtf/HashSet.h>

using namespace JSC;

namespace WebCore {

using namespace HTMLNames;

JSValue JSClipboard::types(ExecState* exec) const
{
    Clipboard* clipboard = impl();

    HashSet<String> types = clipboard->types();
    if (types.isEmpty())
        return jsNull();

    MarkedArgumentBuffer list;
    HashSet<String>::const_iterator end = types.end();
    for (HashSet<String>::const_iterator it = types.begin(); it != end; ++it)
        list.append(jsString(exec, UString(*it)));
    return constructArray(exec, list);
}

JSValue JSClipboard::clearData(ExecState* exec, const ArgList& args)
{
    Clipboard* clipboard = impl();

    if (args.size() == 0) {
        clipboard->clearAllData();
        return jsUndefined();
    }

    if (args.size() == 1) {
        clipboard->clearData(args.at(0).toString(exec));
        return jsUndefined();
    }

    // FIXME: It does not match the rest of the JS bindings to throw on invalid number of arguments. 
    return throwError(exec, SyntaxError, "clearData: Invalid number of arguments");
}

JSValue JSClipboard::getData(ExecState* exec, const ArgList& args)
{
    // FIXME: It does not match the rest of the JS bindings to throw on invalid number of arguments.
    if (args.size() != 1)
        return throwError(exec, SyntaxError, "getData: Invalid number of arguments");

    Clipboard* clipboard = impl();

    bool success;
    String result = clipboard->getData(args.at(0).toString(exec), success);
    if (!success)
        return jsUndefined();

    return jsString(exec, result);
}

JSValue JSClipboard::setData(ExecState* exec, const ArgList& args)
{
    Clipboard* clipboard = impl();

    // FIXME: It does not match the rest of the JS bindings to throw on invalid number of arguments. 
    if (args.size() != 2)
        return throwError(exec, SyntaxError, "setData: Invalid number of arguments");

    return jsBoolean(clipboard->setData(args.at(0).toString(exec), args.at(1).toString(exec)));
}

JSValue JSClipboard::setDragImage(ExecState* exec, const ArgList& args)
{
    Clipboard* clipboard = impl();

    if (!clipboard->isForDragging())
        return jsUndefined();

    // FIXME: It does not match the rest of the JS bindings to throw on invalid number of arguments. 
    if (args.size() != 3)
        return throwError(exec, SyntaxError, "setDragImage: Invalid number of arguments");

    int x = args.at(1).toInt32(exec);
    int y = args.at(2).toInt32(exec);

    // See if they passed us a node
    Node* node = toNode(args.at(0));
    if (!node)
        return throwError(exec, TypeError);

    // FIXME: This should probably be a TypeError. 
    if (!node->isElementNode())
        return throwError(exec, SyntaxError, "setDragImageFromElement: Invalid first argument");

    if (static_cast<Element*>(node)->hasLocalName(imgTag) && !node->inDocument())
        clipboard->setDragImage(static_cast<HTMLImageElement*>(node)->cachedImage(), IntPoint(x, y));
    else
        clipboard->setDragImageElement(node, IntPoint(x, y));

    return jsUndefined();
}

} // namespace WebCore
