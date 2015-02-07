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
 * Copyright (C) 2009 Apple Inc. All Rights Reserved.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

#include "config.h"
#include "GeolocationServiceMock.h"

#include "Logging.h"
#include "Geolocation.h"
#include "Geoposition.h"
#include "PositionError.h"
#include "PositionOptions.h"

namespace WebCore {

GeolocationServiceMock::GeolocationServiceSet* GeolocationServiceMock::s_instances = 0;
RefPtr<Geoposition>* GeolocationServiceMock::s_lastPosition;
RefPtr<PositionError>* GeolocationServiceMock::s_lastError;

GeolocationService* GeolocationServiceMock::create(GeolocationServiceClient* client)
{
    initStatics();
    return new GeolocationServiceMock(client);
}

GeolocationServiceMock::GeolocationServiceMock(GeolocationServiceClient* client)
    : GeolocationService(client)
    , m_timer(this, &GeolocationServiceMock::timerFired)
    , m_isActive(false)
{
    s_instances->add(this);
}

GeolocationServiceMock::~GeolocationServiceMock()
{
    GeolocationServiceSet::iterator iter = s_instances->find(this);
    ASSERT(iter != s_instances->end());
    s_instances->remove(iter);
    cleanUpStatics();
}

void GeolocationServiceMock::setPosition(PassRefPtr<Geoposition> position)
{
    initStatics();
    GeolocationService::useMock();
    *s_lastPosition = position;
    *s_lastError = 0;
    makeGeolocationCallbackFromAllInstances();
}

void GeolocationServiceMock::setError(PassRefPtr<PositionError> error)
{
    initStatics();
    GeolocationService::useMock();
    *s_lastError = error;
    *s_lastPosition = 0;
    makeGeolocationCallbackFromAllInstances();
}

#if PLATFORM(ANDROID)
// TODO: Upstream to webkit.org. See https://bugs.webkit.org/show_bug.cgi?id=34082
bool GeolocationServiceMock::startUpdating(PositionOptions*, bool /* suspend */)
#else
bool GeolocationServiceMock::startUpdating(PositionOptions*)
#endif
{
    m_isActive = true;
    m_timer.startOneShot(0);
    return true;
}

void GeolocationServiceMock::stopUpdating()
{
    m_isActive = false;
}

void GeolocationServiceMock::timerFired(Timer<GeolocationServiceMock>* timer)
{
    ASSERT_UNUSED(timer, timer == &m_timer);
    makeGeolocationCallback();
}

void GeolocationServiceMock::makeGeolocationCallbackFromAllInstances()
{
    GeolocationServiceSet::const_iterator end = s_instances->end();
    for (GeolocationServiceSet::const_iterator iter = s_instances->begin(); iter != end; ++iter)
        (*iter)->makeGeolocationCallback();
}

void GeolocationServiceMock::makeGeolocationCallback()
{
    if (!m_isActive)
        return;

    if (*s_lastPosition)
        positionChanged();
    else if (*s_lastError)
        errorOccurred();
}

void GeolocationServiceMock::initStatics()
{
    if (s_instances == 0) { 
        s_instances = new GeolocationServiceSet;
        s_lastPosition = new RefPtr<Geoposition>;
        s_lastError = new RefPtr<PositionError>;
    }
}

void GeolocationServiceMock::cleanUpStatics()
{
    if (s_instances->size() == 0) {
        delete s_instances;
        s_instances = 0;
        delete s_lastPosition;
        delete s_lastError;
    }
}

} // namespace WebCore
