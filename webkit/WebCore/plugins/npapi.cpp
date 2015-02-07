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
 * Copyright (C) 2006, 2007 Apple Inc.  All rights reserved.
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

#include "PluginInfoStore.h"
#include "PluginMainThreadScheduler.h"
#include "PluginView.h"
#include "npruntime_internal.h"

using namespace WebCore;

// The plugin view is always the ndata of the instance,. Sometimes, plug-ins will call an instance-specific function
// with a NULL instance. To workaround this, call the last plug-in view that made a call to a plug-in.
// Currently, the current plug-in view is only set before NPP_New in PluginView::start.
// This specifically works around Flash and Shockwave. When we call NPP_New, they call NPN_Useragent with a NULL instance.
static PluginView* pluginViewForInstance(NPP instance)
{
    if (instance && instance->ndata)
        return static_cast<PluginView*>(instance->ndata);
    return PluginView::currentPluginView();
}

void* NPN_MemAlloc(uint32 size)
{
    return malloc(size);
}

void NPN_MemFree(void* ptr)
{
    free(ptr);
}

uint32 NPN_MemFlush(uint32 size)
{
    // Do nothing
    return 0;
}

void NPN_ReloadPlugins(NPBool reloadPages)
{
    refreshPlugins(reloadPages);
}

NPError NPN_RequestRead(NPStream* stream, NPByteRange* rangeList)
{
    return NPERR_STREAM_NOT_SEEKABLE;
}

NPError NPN_GetURLNotify(NPP instance, const char* url, const char* target, void* notifyData)
{
    PluginView* view = pluginViewForInstance(instance);
    if(!view)
       return NPERR_INVALID_INSTANCE_ERROR;
    return view->getURLNotify(url, target, notifyData);
}

NPError NPN_GetURL(NPP instance, const char* url, const char* target)
{
    PluginView* view = pluginViewForInstance(instance);
    if(!view)
       return NPERR_INVALID_INSTANCE_ERROR;
    return view->getURL(url, target);
}

NPError NPN_PostURLNotify(NPP instance, const char* url, const char* target, uint32 len, const char* buf, NPBool file, void* notifyData)
{
    PluginView* view = pluginViewForInstance(instance);
    if(!view)
       return NPERR_INVALID_INSTANCE_ERROR;
    return view->postURLNotify(url, target, len, buf, file, notifyData);
}

NPError NPN_PostURL(NPP instance, const char* url, const char* target, uint32 len, const char* buf, NPBool file)
{
    PluginView* view = pluginViewForInstance(instance);
    if(!view)
       return NPERR_INVALID_INSTANCE_ERROR;
    return view->postURL(url, target, len, buf, file);
}

NPError NPN_NewStream(NPP instance, NPMIMEType type, const char* target, NPStream** stream)
{
    PluginView* view = pluginViewForInstance(instance);
    if(!view)
       return NPERR_INVALID_INSTANCE_ERROR;
    return view->newStream(type, target, stream);
}

int32 NPN_Write(NPP instance, NPStream* stream, int32 len, void* buffer)
{
    PluginView* view = pluginViewForInstance(instance);
    if(!view)
       return 0;
    return view->write(stream, len, buffer);
}

NPError NPN_DestroyStream(NPP instance, NPStream* stream, NPReason reason)
{
    PluginView* view = pluginViewForInstance(instance);
    if(!view)
       return NPERR_INVALID_INSTANCE_ERROR;
    return view->destroyStream(stream, reason);
}

const char* NPN_UserAgent(NPP instance)
{
    PluginView* view = pluginViewForInstance(instance);

     if (!view)
         return PluginView::userAgentStatic();
 
    return view->userAgent();
}

void NPN_Status(NPP instance, const char* message)
{
    PluginView* view = pluginViewForInstance(instance);
    if(!view)
       return;
    view->status(message);
}

void NPN_InvalidateRect(NPP instance, NPRect* invalidRect)
{
    PluginView* view = pluginViewForInstance(instance);
    if(!view)
       return;
    view->invalidateRect(invalidRect);
}

void NPN_InvalidateRegion(NPP instance, NPRegion invalidRegion)
{
    PluginView* view = pluginViewForInstance(instance);
    if(!view)
       return;
    view->invalidateRegion(invalidRegion);
}

void NPN_ForceRedraw(NPP instance)
{
    PluginView* view = pluginViewForInstance(instance);
    if(!view)
       return;
    view->forceRedraw();
}

NPError NPN_GetValue(NPP instance, NPNVariable variable, void* value)
{
    PluginView* view = pluginViewForInstance(instance);

     if (!view)
         return PluginView::getValueStatic(variable, value);

    return pluginViewForInstance(instance)->getValue(variable, value);
}

NPError NPN_SetValue(NPP instance, NPPVariable variable, void* value)
{
    PluginView* view = pluginViewForInstance(instance);
    if(!view)
       return NPERR_INVALID_INSTANCE_ERROR;
   return view->setValue(variable, value);
}

void* NPN_GetJavaEnv()
{
    // Unsupported
    return 0;
}

void* NPN_GetJavaPeer(NPP instance)
{
    // Unsupported
    return 0;
}

void NPN_PushPopupsEnabledState(NPP instance, NPBool enabled)
{
    PluginView* view = pluginViewForInstance(instance);
    if(!view)
       return;
    view->pushPopupsEnabledState(enabled);
}

void NPN_PopPopupsEnabledState(NPP instance)
{
    PluginView* view = pluginViewForInstance(instance);
    if(!view)
       return;
    view->popPopupsEnabledState();
}

void NPN_PluginThreadAsyncCall(NPP instance, void (*func) (void *), void *userData)
{
    PluginMainThreadScheduler::scheduler().scheduleCall(instance, func, userData);
}

#ifdef PLUGIN_SCHEDULE_TIMER
uint32 NPN_ScheduleTimer(NPP instance, uint32 interval, NPBool repeat,
                         void (*timerFunc)(NPP npp, uint32 timerID))
{
    PluginView* view = pluginViewForInstance(instance);
    if(!view)
       return 0;
    return view->scheduleTimer(instance, interval,
                                                        repeat != 0, timerFunc);
}

void NPN_UnscheduleTimer(NPP instance, uint32 timerID)
{
    PluginView* view = pluginViewForInstance(instance);
    if(!view)
       return;
    view->unscheduleTimer(instance, timerID);
}
#endif


