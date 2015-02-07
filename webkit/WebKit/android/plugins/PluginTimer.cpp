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
 * Copyright (C) 2008 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "PluginTimer.h"

namespace WebCore {

    static uint32 gTimerID;

    PluginTimer::PluginTimer(PluginTimer** list, NPP instance, bool repeat,
                             void (*timerFunc)(NPP npp, uint32 timerID))
                : m_list(list),
                  m_instance(instance),
                  m_timerFunc(timerFunc),
                  m_repeat(repeat),
                  m_unscheduled(false)
    {
        m_timerID = ++gTimerID;

        m_next = *list;
        if (m_next) {
            m_next->m_prev = this;
        }
        m_prev = 0;
        *list = this;
    }
    
    PluginTimer::~PluginTimer()
    {
        if (m_next) {
            m_next->m_prev = m_prev;
        }
        if (m_prev) {
            m_prev->m_next = m_next;
        } else {
            *m_list = m_next;
        }
    }
        
    void PluginTimer::fired()
    {
        if (!m_unscheduled)
            m_timerFunc(m_instance, m_timerID);

        if (!m_repeat || m_unscheduled)
            delete this;
    }
    
    // may return null if timerID is not found
    PluginTimer* PluginTimer::Find(PluginTimer* list, uint32 timerID)
    {
        PluginTimer* curr = list;
        while (curr) {
            if (curr->m_timerID == timerID) {
                break;
            }
            curr = curr->m_next;
        }
        return curr;
    }

    ///////////////////////////////////////////////////////////////////////////
    
    PluginTimerList::~PluginTimerList()
    {
        while (m_list) {
            delete m_list;
        }
    }

    uint32 PluginTimerList::schedule(NPP instance, uint32 interval, bool repeat,
                                     void (*proc)(NPP npp, uint32 timerID))
    {        
        PluginTimer* timer = new PluginTimer(&m_list, instance, repeat, proc);
        
        double dinterval = interval * 0.001;    // milliseconds to seconds
        if (repeat) {
            timer->startRepeating(dinterval);
        } else {
            timer->startOneShot(dinterval);
        }
        return timer->timerID();
    }
    
    void PluginTimerList::unschedule(NPP instance, uint32 timerID)
    {
        // Although it looks like simply deleting the timer would work here
        // (stop() will be executed by the dtor), we cannot do this, as
        // the plugin can call us while we are in the fired() method,
        // (when we execute the timerFunc callback). Deleting the object
        // we are in would then be a rather bad move...
        PluginTimer* timer = PluginTimer::Find(m_list, timerID);
        if (timer)
            timer->unschedule();
    }
    
} // namespace WebCore