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
 * Copyright (C) 2009 Gustavo Noronha Silva
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

#include <errno.h>
#include <unistd.h>
#include <glib.h>
#include <glib/gstdio.h>
#include <gtk/gtk.h>
#include <stdlib.h>
#include <webkit/webkit.h>

#if GLIB_CHECK_VERSION(2, 16, 0) && GTK_CHECK_VERSION(2, 14, 0)

static void test_network_request_create_destroy()
{
    WebKitNetworkRequest* request;
    SoupMessage* message;

    /* Test creation with URI */
    request = WEBKIT_NETWORK_REQUEST(g_object_new(WEBKIT_TYPE_NETWORK_REQUEST, "uri", "http://debian.org/", NULL));
    g_assert(WEBKIT_IS_NETWORK_REQUEST(request));
    message = webkit_network_request_get_message(request);
    g_assert(!message);
    g_object_unref(request);

    /* Test creation with SoupMessage */
    message = soup_message_new("GET", "http://debian.org/");
    request = WEBKIT_NETWORK_REQUEST(g_object_new(WEBKIT_TYPE_NETWORK_REQUEST, "message", message, NULL));
    g_assert(WEBKIT_IS_NETWORK_REQUEST(request));
    g_assert_cmpint(G_OBJECT(message)->ref_count, ==, 2);
    g_object_unref(request);
    g_assert_cmpint(G_OBJECT(message)->ref_count, ==, 1);
    g_object_unref(message);

    /* Test creation with both SoupMessage and URI */
    message = soup_message_new("GET", "http://debian.org/");
    request = WEBKIT_NETWORK_REQUEST(g_object_new(WEBKIT_TYPE_NETWORK_REQUEST, "message", message, "uri", "http://gnome.org/", NULL));
    g_assert(WEBKIT_IS_NETWORK_REQUEST(request));
    g_assert_cmpint(G_OBJECT(message)->ref_count, ==, 2);
    g_assert_cmpstr(webkit_network_request_get_uri(request), ==, "http://gnome.org/");
    g_object_unref(request);
    g_assert_cmpint(G_OBJECT(message)->ref_count, ==, 1);
    g_object_unref(message);
}

static void test_network_request_properties()
{
    WebKitNetworkRequest* request;
    SoupMessage* message;
    gchar* soupURI;

    /* Test URI is set correctly when creating with URI */
    request = webkit_network_request_new("http://debian.org/");
    g_assert(WEBKIT_IS_NETWORK_REQUEST(request));
    g_assert_cmpstr(webkit_network_request_get_uri(request), ==, "http://debian.org/");
    g_object_unref(request);

    /* Test URI is set correctly when creating with Message */
    message = soup_message_new("GET", "http://debian.org/");
    request = WEBKIT_NETWORK_REQUEST(g_object_new(WEBKIT_TYPE_NETWORK_REQUEST, "message", message, NULL));
    g_assert(WEBKIT_IS_NETWORK_REQUEST(request));
    g_object_unref(message);

    message = webkit_network_request_get_message(request);
    soupURI = soup_uri_to_string(soup_message_get_uri(message), FALSE);
    g_assert_cmpstr(soupURI, ==, "http://debian.org/");
    g_free(soupURI);

    g_assert_cmpstr(webkit_network_request_get_uri(request), ==, "http://debian.org/");
    g_object_unref(request);
}

int main(int argc, char** argv)
{
    g_thread_init(NULL);
    gtk_test_init(&argc, &argv, NULL);

    g_test_bug_base("https://bugs.webkit.org/");
    g_test_add_func("/webkit/networkrequest/createdestroy", test_network_request_create_destroy);
    g_test_add_func("/webkit/networkrequest/properties", test_network_request_properties);
    return g_test_run ();
}

#else
int main(int argc, char** argv)
{
    g_critical("You will need at least glib-2.16.0 and gtk-2.14.0 to run the unit tests. Doing nothing now.");
    return 0;
}

#endif
