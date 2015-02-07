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
 *  Copyright (C) 2001 Peter Kelly (pmk@post.com)
 *  Copyright (C) 2007, 2008, 2009 Apple Inc. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "config.h"
#include "JSNodeFilterCondition.h"

#include "JSNode.h"
#include "JSNodeFilter.h"
#include "NodeFilter.h"
#include <runtime/JSLock.h>

namespace WebCore {

using namespace JSC;

ASSERT_CLASS_FITS_IN_CELL(JSNodeFilterCondition);

JSNodeFilterCondition::JSNodeFilterCondition(JSValue filter)
    : m_filter(filter)
{
}

void JSNodeFilterCondition::markAggregate(MarkStack& markStack)
{
    markStack.append(m_filter);
}

short JSNodeFilterCondition::acceptNode(JSC::ExecState* exec, Node* filterNode) const
{
    JSLock lock(SilenceAssertionsOnly);

    CallData callData;
    CallType callType = m_filter.getCallData(callData);
    if (callType == CallTypeNone)
        return NodeFilter::FILTER_ACCEPT;

   // The exec argument here should only be null if this was called from a
   // non-JavaScript language, and this is a JavaScript filter, and the document
   // in question is not associated with the frame. In that case, we're going to
   // behave incorrectly, and just reject nodes instead of calling the filter function.
   // To fix that we'd need to come up with a way to find a suitable JavaScript
   // execution context for the filter function to run in.
    if (!exec)
        return NodeFilter::FILTER_REJECT;

    MarkedArgumentBuffer args;
    // FIXME: The node should have the prototype chain that came from its document, not
    // whatever prototype chain might be on the window this filter came from. Bug 27662
    args.append(toJS(exec, deprecatedGlobalObjectForPrototype(exec), filterNode));
    if (exec->hadException())
        return NodeFilter::FILTER_REJECT;

    JSValue result = JSC::call(exec, m_filter, callType, callData, m_filter, args);
    if (exec->hadException())
        return NodeFilter::FILTER_REJECT;

    int intResult = result.toInt32(exec);
    if (exec->hadException())
        return NodeFilter::FILTER_REJECT;

    return intResult;
}

} // namespace WebCore
