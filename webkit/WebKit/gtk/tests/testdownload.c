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
 * Copyright (C) 2009 Christian Dywan <christian@twotoasts.de>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2,1 of the License, or (at your option) any later version.
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
#include <glib/gstdio.h>
#include <webkit/webkit.h>

#if GTK_CHECK_VERSION(2, 14, 0)

GMainLoop* loop;
char* temporaryFilename = NULL;
WebKitDownload* theDownload = NULL;

static void
test_webkit_download_create(void)
{
    WebKitNetworkRequest* request;
    WebKitDownload* download;
    const gchar* uri = "http://example.com";
    gchar* tmpDir;

    request = webkit_network_request_new(uri);
    download = webkit_download_new(request);
    g_object_unref(request);
    g_assert_cmpstr(webkit_download_get_uri(download), ==, uri);
    g_assert(webkit_download_get_network_request(download) == request);
    g_assert(g_strrstr(uri, webkit_download_get_suggested_filename(download)));
    g_assert(webkit_download_get_status(download) == WEBKIT_DOWNLOAD_STATUS_CREATED);
    g_assert(!webkit_download_get_total_size(download));
    g_assert(!webkit_download_get_current_size(download));
    g_assert(!webkit_download_get_progress(download));
    g_assert(!webkit_download_get_elapsed_time(download));
    tmpDir = g_filename_to_uri(g_get_tmp_dir(), NULL, NULL);
    webkit_download_set_destination_uri(download, tmpDir);
    g_assert_cmpstr(tmpDir, ==, webkit_download_get_destination_uri(download));;
    g_free(tmpDir);
    g_object_unref(download);
}

static gboolean
navigation_policy_decision_requested_cb(WebKitWebView* web_view,
                                        WebKitWebFrame* web_frame,
                                        WebKitNetworkRequest* request,
                                        WebKitWebNavigationAction* action,
                                        WebKitWebPolicyDecision* decision,
                                        gpointer data)
{
    webkit_web_policy_decision_download(decision);
    return TRUE;
}

static void
notify_status_cb(GObject* object, GParamSpec* pspec, gpointer data)
{
    WebKitDownload* download = WEBKIT_DOWNLOAD(object);
    switch (webkit_download_get_status(download)) {
    case WEBKIT_DOWNLOAD_STATUS_FINISHED:
    case WEBKIT_DOWNLOAD_STATUS_ERROR:
        g_main_loop_quit(loop);
        break;
    case WEBKIT_DOWNLOAD_STATUS_CANCELLED:
        g_assert_not_reached();
        break;
    default:
        break;
    }
}

static gboolean
download_requested_cb(WebKitWebView* web_view,
                      WebKitDownload* download,
                      gboolean* beenThere)
{
    theDownload = download;
    *beenThere = TRUE;
    if (temporaryFilename) {
        gchar *uri = g_filename_to_uri(temporaryFilename, NULL, NULL);
        if (uri)
            webkit_download_set_destination_uri(download, uri);
        g_free(uri);
    }

    g_signal_connect(download, "notify::status",
                     G_CALLBACK(notify_status_cb), NULL);

    return TRUE;
}

static gboolean
set_filename(gchar* filename)
{
    gchar *uri = g_filename_to_uri(filename, NULL, NULL);
    webkit_download_set_destination_uri(theDownload, uri);
    g_free(uri);
    temporaryFilename = filename;
    webkit_download_start(theDownload);
    return FALSE;
}

static void
test_webkit_download_perform(gboolean asynch)
{
    WebKitWebView* webView = WEBKIT_WEB_VIEW(webkit_web_view_new());

    g_object_ref_sink(G_OBJECT(webView));

    g_signal_connect(webView, "navigation-policy-decision-requested",
                     G_CALLBACK(navigation_policy_decision_requested_cb),
                     NULL);

    gboolean beenThere = FALSE;
    g_signal_connect(webView, "download-requested",
                     G_CALLBACK(download_requested_cb),
                     &beenThere);

    /* Preparation; FIXME: we should move this code to a test
     * utilities file, because we have a very similar one in
     * testwebframe.c */
    GError *error = NULL;
    gchar* filename;
    int fd = g_file_open_tmp("webkit-testwebdownload-XXXXXX", &filename, &error);
    close(fd);

    if (error)
        g_critical("Failed to open a temporary file for writing: %s.", error->message);

    if (g_unlink(filename) == -1)
        g_critical("Failed to delete the temporary file: %s.", g_strerror(errno));

    if (asynch)
        g_idle_add((GSourceFunc)set_filename, filename);
    else
        temporaryFilename = filename;

    theDownload = NULL;

    loop = g_main_loop_new(NULL, TRUE);
    webkit_web_view_load_uri(webView, "http://gnome.org/");
    g_main_loop_run(loop);

    g_assert_cmpint(beenThere, ==, TRUE);

    g_assert_cmpint(g_file_test(temporaryFilename, G_FILE_TEST_IS_REGULAR), ==, TRUE);

    g_unlink(temporaryFilename);
    g_free(temporaryFilename);
    g_main_loop_unref(loop);
    g_object_unref(webView);
}

static void
test_webkit_download_synch(void)
{
    test_webkit_download_perform(FALSE);
}

static void
test_webkit_download_asynch(void)
{
    test_webkit_download_perform(TRUE);
}

int main(int argc, char** argv)
{
    g_thread_init(NULL);
    gtk_test_init(&argc, &argv, NULL);

    g_test_bug_base("https://bugs.webkit.org/");
    g_test_add_func("/webkit/download/create", test_webkit_download_create);
    g_test_add_func("/webkit/download/synch", test_webkit_download_synch);
    g_test_add_func("/webkit/download/asynch", test_webkit_download_asynch);
    return g_test_run ();
}

#else

int main(int argc, char** argv)
{
    g_critical("You will need at least GTK+ 2.14.0 to run the unit tests.");
    return 0;
}

#endif
