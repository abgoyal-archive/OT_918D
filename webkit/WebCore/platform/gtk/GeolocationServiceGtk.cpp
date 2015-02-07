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
 * Copyright (C) 2008 Holger Hans Peter Freyther
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
 */

#include "config.h"
#include "GeolocationServiceGtk.h"

#include "CString.h"
#include "GOwnPtr.h"
#include "NotImplemented.h"
#include "PositionOptions.h"

namespace WTF {
    template<> void freeOwnedGPtr<GeoclueAccuracy>(GeoclueAccuracy* accuracy)
    {
        if (!accuracy)
            return;

        geoclue_accuracy_free(accuracy);
    }
}

namespace WebCore {

GeolocationService* GeolocationServiceGtk::create(GeolocationServiceClient* client)
{
    return new GeolocationServiceGtk(client);
}

GeolocationService::FactoryFunction* GeolocationService::s_factoryFunction = &GeolocationServiceGtk::create;

GeolocationServiceGtk::GeolocationServiceGtk(GeolocationServiceClient* client)
    : GeolocationService(client)
    , m_geoclueClient(0)
    , m_geocluePosition(0)
    , m_latitude(0.0)
    , m_longitude(0.0)
    , m_altitude(0.0)
    , m_altitudeAccuracy(0.0)
    , m_timestamp(0)
{
}

GeolocationServiceGtk::~GeolocationServiceGtk()
{
    if (m_geoclueClient)
        g_object_unref(m_geoclueClient);

    if (m_geocluePosition)
        g_object_unref(m_geocluePosition);
}

//
// 1.) Initialize Geoclue with our requirements
// 2.) Try to get a GeocluePosition
// 3.) Update the Information and get the current position
//
// TODO: Also get GeoclueVelocity but there is no master client
//       API for that.
//
bool GeolocationServiceGtk::startUpdating(PositionOptions* options)
{
    ASSERT(!m_geoclueClient);

    m_lastPosition = 0;
    m_lastError = 0;

    GOwnPtr<GError> error;
    GeoclueMaster* master = geoclue_master_get_default();
    GeoclueMasterClient* client = geoclue_master_create_client(master, 0, 0);
    g_object_unref(master);

    if (!client) {
        setError(PositionError::POSITION_UNAVAILABLE, "Could not connect to location provider.");
        return false;
    }

    GeoclueAccuracyLevel accuracyLevel = GEOCLUE_ACCURACY_LEVEL_LOCALITY;
    int timeout = 0;
    if (options) {
        accuracyLevel = options->enableHighAccuracy() ? GEOCLUE_ACCURACY_LEVEL_DETAILED : GEOCLUE_ACCURACY_LEVEL_LOCALITY;
        timeout = options->timeout();
    }

    gboolean result = geoclue_master_client_set_requirements(client, accuracyLevel, timeout,
                                                             true, GEOCLUE_RESOURCE_ALL, &error.outPtr());

    if (!result) {
        setError(PositionError::POSITION_UNAVAILABLE, error->message);
        g_object_unref(client);
        return false;
    }

    m_geocluePosition = geoclue_master_client_create_position(client, &error.outPtr());
    if (!m_geocluePosition) {
        setError(PositionError::POSITION_UNAVAILABLE, error->message);
        g_object_unref(client);
        return false;
    }

    g_signal_connect(G_OBJECT(m_geocluePosition), "position-changed",
                     G_CALLBACK(position_changed), this);

    m_geoclueClient = client;
    updateLocationInformation();

    return true;
}

void GeolocationServiceGtk::stopUpdating()
{
    if (!m_geoclueClient)
        return;

    g_object_unref(m_geocluePosition);
    g_object_unref(m_geoclueClient);

    m_geocluePosition = 0;
    m_geoclueClient = 0;
}

void GeolocationServiceGtk::suspend()
{
    // not available with geoclue
    notImplemented();
}

void GeolocationServiceGtk::resume()
{
    // not available with geoclue
    notImplemented();
}

Geoposition* GeolocationServiceGtk::lastPosition() const
{
    return m_lastPosition.get();
}

PositionError* GeolocationServiceGtk::lastError() const
{
    return m_lastError.get();
}

void GeolocationServiceGtk::updateLocationInformation()
{
    ASSERT(m_geocluePosition);

    GOwnPtr<GError> error;
    GOwnPtr<GeoclueAccuracy> accuracy;

    GeocluePositionFields fields = geoclue_position_get_position(m_geocluePosition, &m_timestamp,
                                                                 &m_latitude, &m_longitude,
                                                                 &m_altitude, &accuracy.outPtr(),
                                                                 &error.outPtr());
    if (error) {
        setError(PositionError::POSITION_UNAVAILABLE, error->message);
        return;
    } else if (!(fields & GEOCLUE_POSITION_FIELDS_LATITUDE && fields & GEOCLUE_POSITION_FIELDS_LONGITUDE)) {
        setError(PositionError::POSITION_UNAVAILABLE, "Position could not be determined.");
        return;
    }


}

void GeolocationServiceGtk::updatePosition()
{
    m_lastError = 0;

    RefPtr<Coordinates> coordinates = Coordinates::create(m_latitude, m_longitude,
                                                          true, m_altitude, m_accuracy,
                                                          true, m_altitudeAccuracy, false, 0.0, false, 0.0);
    m_lastPosition = Geoposition::create(coordinates.release(), m_timestamp * 1000.0);
    positionChanged();
}

void GeolocationServiceGtk::position_changed(GeocluePosition*, GeocluePositionFields fields, int timestamp, double latitude, double longitude, double altitude, GeoclueAccuracy* accuracy, GeolocationServiceGtk* that)
{
    if (!(fields & GEOCLUE_POSITION_FIELDS_LATITUDE && fields & GEOCLUE_POSITION_FIELDS_LONGITUDE)) {
        that->setError(PositionError::POSITION_UNAVAILABLE, "Position could not be determined.");
        return;
    }

    that->m_timestamp = timestamp;
    that->m_latitude = latitude;
    that->m_longitude = longitude;
    that->m_altitude = altitude;

    GeoclueAccuracyLevel level;
    geoclue_accuracy_get_details(accuracy, &level, &that->m_accuracy, &that->m_altitudeAccuracy);
    that->updatePosition();
}

void GeolocationServiceGtk::setError(PositionError::ErrorCode errorCode, const char* message)
{
    m_lastPosition = 0;
    m_lastError = PositionError::create(errorCode, String::fromUTF8(message));
}

}
