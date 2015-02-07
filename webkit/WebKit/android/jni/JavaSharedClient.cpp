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
 * Copyright 2007, The Android Open Source Project
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
#include "JavaSharedClient.h"
#include "TimerClient.h"
#include "SkDeque.h"
#include "SkThread.h"

namespace android {
    TimerClient* JavaSharedClient::GetTimerClient()
    {
        return gTimerClient;
    }

    CookieClient* JavaSharedClient::GetCookieClient()
    {
        return gCookieClient;
    }

    PluginClient* JavaSharedClient::GetPluginClient()
    {
        return gPluginClient;
    }

    KeyGeneratorClient* JavaSharedClient::GetKeyGeneratorClient()
    {
        return gKeyGeneratorClient;
    }

    void JavaSharedClient::SetTimerClient(TimerClient* client)
    {
        gTimerClient = client;
    }

    void JavaSharedClient::SetCookieClient(CookieClient* client)
    {
        gCookieClient = client;
    }

    void JavaSharedClient::SetPluginClient(PluginClient* client)
    {
        gPluginClient = client;
    }

    void JavaSharedClient::SetKeyGeneratorClient(KeyGeneratorClient* client)
    {
        gKeyGeneratorClient = client;
    }

    TimerClient*    JavaSharedClient::gTimerClient = NULL;
    CookieClient*   JavaSharedClient::gCookieClient = NULL;
    PluginClient*   JavaSharedClient::gPluginClient = NULL;
    KeyGeneratorClient* JavaSharedClient::gKeyGeneratorClient = NULL;

    ///////////////////////////////////////////////////////////////////////////
    
    struct FuncPtrRec {
        void (*fProc)(void* payload);
        void* fPayload;
    };
    
    static SkMutex gFuncPtrQMutex;
    static SkDeque gFuncPtrQ(sizeof(FuncPtrRec));

    void JavaSharedClient::EnqueueFunctionPtr(void (*proc)(void* payload),
                                              void* payload)
    {
        gFuncPtrQMutex.acquire();

        FuncPtrRec* rec = (FuncPtrRec*)gFuncPtrQ.push_back();
        rec->fProc = proc;
        rec->fPayload = payload;
        
        gFuncPtrQMutex.release();
        
        gTimerClient->signalServiceFuncPtrQueue();
    }

    void JavaSharedClient::ServiceFunctionPtrQueue()
    {
        for (;;) {
            void (*proc)(void*) = 0;
            void* payload = 0;
            const FuncPtrRec* rec;
            
            // we have to copy the proc/payload (if present). we do this so we
            // don't call the proc inside the mutex (possible deadlock!)
            gFuncPtrQMutex.acquire();
            rec = (const FuncPtrRec*)gFuncPtrQ.front();
            if (rec) {
                proc = rec->fProc;
                payload = rec->fPayload;
                gFuncPtrQ.pop_front();
            }
            gFuncPtrQMutex.release();
            
            if (!rec)
                break;
            proc(payload);
        }
    }
}
