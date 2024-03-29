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
 * Copyright (C) 2009 Martin Robinson
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
#include "webkitwebdatabase.h"

#include "webkitprivate.h"

#include "CString.h"
#include "DatabaseDetails.h"
#include "DatabaseTracker.h"

#include <glib/gi18n-lib.h>

/**
 * SECTION:webkitwebdatabase
 * @short_description: A WebKit web application database
 *
 * #WebKitWebDatabase is a representation of a Web Database database. The
 * proposed Web Database standard introduces support for SQL databases that web
 * sites can create and access on a local computer through JavaScript.
 *
 * To get access to all databases defined by a security origin, use
 * #webkit_security_origin_get_databases. Each database has a canonical
 * name, as well as a user-friendly display name.
 * 
 * WebKit uses SQLite to create and access the local SQL databases. The location
 * of a #WebKitWebDatabase can be accessed wth #webkit_web_database_get_filename.
 * You can configure the location of all databases with
 * #webkit_set_database_directory_path.
 * 
 * For each database the web site can define an estimated size which can be
 * accessed with #webkit_web_database_get_expected_size. The current size of the
 * database in bytes is returned by #webkit_web_database_get_size.
 * 
 * For more information refer to the Web Database specification proposal at
 * http://dev.w3.org/html5/webdatabase
 */

using namespace WebKit;

enum {
    PROP_0,

    PROP_SECURITY_ORIGIN,
    PROP_NAME,
    PROP_DISPLAY_NAME,
    PROP_EXPECTED_SIZE,
    PROP_SIZE,
    PROP_PATH
};

G_DEFINE_TYPE(WebKitWebDatabase, webkit_web_database, G_TYPE_OBJECT)

struct _WebKitWebDatabasePrivate {
    WebKitSecurityOrigin* origin;
    gchar* name;
    gchar* displayName;
    gchar* filename;
};

static gchar* webkit_database_directory_path = NULL;
static guint64 webkit_default_database_quota = 5 * 1024 * 1024;

#define WEBKIT_WEB_DATABASE_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE((obj), WEBKIT_TYPE_WEB_DATABASE, WebKitWebDatabasePrivate))

static void webkit_web_database_set_security_origin(WebKitWebDatabase* webDatabase, WebKitSecurityOrigin* security_origin);

static void webkit_web_database_set_name(WebKitWebDatabase* webDatabase, const gchar* name);

static void webkit_web_database_finalize(GObject* object)
{
    WebKitWebDatabase* webDatabase = WEBKIT_WEB_DATABASE(object);
    WebKitWebDatabasePrivate* priv = webDatabase->priv;

    g_free(priv->name);
    g_free(priv->displayName);
    g_free(priv->filename);

    G_OBJECT_CLASS(webkit_web_database_parent_class)->finalize(object);
}

static void webkit_web_database_dispose(GObject* object)
{
    WebKitWebDatabase* webDatabase = WEBKIT_WEB_DATABASE(object);
    WebKitWebDatabasePrivate* priv = webDatabase->priv;

    if (priv->origin) {
        g_object_unref(priv->origin);
        priv->origin = NULL;
    }

    G_OBJECT_CLASS(webkit_web_database_parent_class)->dispose(object);
}

static void webkit_web_database_set_property(GObject* object, guint propId, const GValue* value, GParamSpec* pspec)
{
    WebKitWebDatabase* webDatabase = WEBKIT_WEB_DATABASE(object);

    switch (propId) {
    case PROP_SECURITY_ORIGIN:
        webkit_web_database_set_security_origin(webDatabase, WEBKIT_SECURITY_ORIGIN(g_value_get_object(value)));
        break;
    case PROP_NAME:
        webkit_web_database_set_name(webDatabase, g_value_get_string(value));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propId, pspec);
        break;
    }
}

static void webkit_web_database_get_property(GObject* object, guint propId, GValue* value, GParamSpec* pspec)
{
    WebKitWebDatabase* webDatabase = WEBKIT_WEB_DATABASE(object);
    WebKitWebDatabasePrivate* priv = webDatabase->priv;

    switch (propId) {
    case PROP_SECURITY_ORIGIN:
        g_value_set_object(value, priv->origin);
        break;
    case PROP_NAME:
        g_value_set_string(value, webkit_web_database_get_name(webDatabase));
        break;
    case PROP_DISPLAY_NAME:
        g_value_set_string(value, webkit_web_database_get_display_name(webDatabase));
        break;
    case PROP_EXPECTED_SIZE:
        g_value_set_uint64(value, webkit_web_database_get_expected_size(webDatabase));
        break;
    case PROP_SIZE:
        g_value_set_uint64(value, webkit_web_database_get_size(webDatabase));
        break;
    case PROP_PATH:
        g_value_set_string(value, webkit_web_database_get_filename(webDatabase));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propId, pspec);
        break;
    }
}

static void webkit_web_database_class_init(WebKitWebDatabaseClass* klass)
{
    GObjectClass* gobjectClass = G_OBJECT_CLASS(klass);
    gobjectClass->dispose = webkit_web_database_dispose;
    gobjectClass->finalize = webkit_web_database_finalize;
    gobjectClass->set_property = webkit_web_database_set_property;
    gobjectClass->get_property = webkit_web_database_get_property;

     /**
      * WebKitWebDatabase:security-origin:
      *
      * The security origin of the database.
      *
      * Since: 1.1.14
      */
     g_object_class_install_property(gobjectClass, PROP_SECURITY_ORIGIN,
                                     g_param_spec_object("security-origin",
                                                         _("Security Origin"),
                                                         _("The security origin of the database"),
                                                         WEBKIT_TYPE_SECURITY_ORIGIN,
                                                         (GParamFlags) (G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY)));

     /**
      * WebKitWebDatabase:name:
      *
      * The name of the Web Database database.
      *
      * Since: 1.1.14
      */
     g_object_class_install_property(gobjectClass, PROP_NAME,
                                     g_param_spec_string("name",
                                                         _("Name"),
                                                         _("The name of the Web Database database"),
                                                         NULL,
                                                         (GParamFlags) (G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY)));

     /**
      * WebKitWebDatabase:display-name:
      *
      * The display name of the Web Database database.
      *
      * Since: 1.1.14
      */
     g_object_class_install_property(gobjectClass, PROP_DISPLAY_NAME,
                                     g_param_spec_string("display-name",
                                                         _("Display Name"),
                                                         _("The display name of the Web Storage database"),
                                                         NULL,
                                                         WEBKIT_PARAM_READABLE));

     /**
     * WebKitWebDatabase:expected-size:
     *
     * The expected size of the database in bytes as defined by the web author.
     *
     * Since: 1.1.14
     */
     g_object_class_install_property(gobjectClass, PROP_EXPECTED_SIZE,
                                     g_param_spec_uint64("expected-size",
                                                         _("Expected Size"),
                                                         _("The expected size of the Web Database database"),
                                                         0, G_MAXUINT64, 0,
                                                         WEBKIT_PARAM_READABLE));
     /**
     * WebKitWebDatabase:size:
     *
     * The current size of the database in bytes.
     *
     * Since: 1.1.14
     */
     g_object_class_install_property(gobjectClass, PROP_SIZE,
                                     g_param_spec_uint64("size",
                                                         _("Size"),
                                                         _("The current size of the Web Database database"),
                                                         0, G_MAXUINT64, 0,
                                                         WEBKIT_PARAM_READABLE));
     /**
      * WebKitWebDatabase:filename:
      *
      * The absolute filename of the Web Database database.
      *
      * Since: 1.1.14
      */
     g_object_class_install_property(gobjectClass, PROP_PATH,
                                     g_param_spec_string("filename",
                                                         _("Filename"),
                                                         _("The absolute filename of the Web Storage database"),
                                                         NULL,
                                                         WEBKIT_PARAM_READABLE));

    g_type_class_add_private(klass, sizeof(WebKitWebDatabasePrivate));
}

static void webkit_web_database_init(WebKitWebDatabase* webDatabase)
{
    webDatabase->priv = WEBKIT_WEB_DATABASE_GET_PRIVATE(webDatabase);
}

// Internal use only
static void webkit_web_database_set_security_origin(WebKitWebDatabase *webDatabase, WebKitSecurityOrigin *securityOrigin)
{
    g_return_if_fail(WEBKIT_IS_WEB_DATABASE(webDatabase));
    g_return_if_fail(WEBKIT_IS_SECURITY_ORIGIN(securityOrigin));

    WebKitWebDatabasePrivate* priv = webDatabase->priv;

    if (priv->origin)
        g_object_unref(priv->origin);

    g_object_ref(securityOrigin);
    priv->origin = securityOrigin;
}

static void webkit_web_database_set_name(WebKitWebDatabase* webDatabase, const gchar* name)
{
    g_return_if_fail(WEBKIT_IS_WEB_DATABASE(webDatabase));

    WebKitWebDatabasePrivate* priv = webDatabase->priv;
    g_free(priv->name);
    priv->name = g_strdup(name);
}

/**
 * webkit_web_database_get_security_origin:
 * @web_database: a #WebKitWebDatabase
 *
 * Returns the security origin of the #WebKitWebDatabase.
 *
 * Returns: the security origin of the database
 *
 * Since: 1.1.14
 **/
WebKitSecurityOrigin* webkit_web_database_get_security_origin(WebKitWebDatabase* webDatabase)
{
    g_return_val_if_fail(WEBKIT_IS_WEB_DATABASE(webDatabase), NULL);
    WebKitWebDatabasePrivate* priv = webDatabase->priv;

    return priv->origin;
}

/**
 * webkit_web_database_get_name:
 * @web_database: a #WebKitWebDatabase
 *
 * Returns the canonical name of the #WebKitWebDatabase.
 *
 * Returns: the name of the database
 *
 * Since: 1.1.14
 **/
G_CONST_RETURN gchar* webkit_web_database_get_name(WebKitWebDatabase* webDatabase)
{
    g_return_val_if_fail(WEBKIT_IS_WEB_DATABASE(webDatabase), NULL);
    WebKitWebDatabasePrivate* priv = webDatabase->priv;

    return priv->name;
}

/**
 * webkit_web_database_get_display_name:
 * @web_database: a #WebKitWebDatabase
 *
 * Returns the name of the #WebKitWebDatabase as seen by the user.
 *
 * Returns: the name of the database as seen by the user.
 *
 * Since: 1.1.14
 **/
G_CONST_RETURN gchar* webkit_web_database_get_display_name(WebKitWebDatabase* webDatabase)
{
    g_return_val_if_fail(WEBKIT_IS_WEB_DATABASE(webDatabase), NULL);

#if ENABLE(DATABASE)
    WebKitWebDatabasePrivate* priv = webDatabase->priv;
    WebCore::DatabaseDetails details = WebCore::DatabaseTracker::tracker().detailsForNameAndOrigin(priv->name, core(priv->origin));
    WebCore::String displayName =  details.displayName();

    if (displayName.isEmpty())
        return "";

    g_free(priv->displayName);
    priv->displayName = g_strdup(displayName.utf8().data());
    return priv->displayName;
#else
    return "";
#endif
}

/**
 * webkit_web_database_get_expected_size:
 * @web_database: a #WebKitWebDatabase
 *
 * Returns the expected size of the #WebKitWebDatabase in bytes as defined by the
 * web author. The Web Database standard allows web authors to specify an expected
 * size of the database to optimize the user experience.
 *
 * Returns: the expected size of the database in bytes
 *
 * Since: 1.1.14
 **/
guint64 webkit_web_database_get_expected_size(WebKitWebDatabase* webDatabase)
{
    g_return_val_if_fail(WEBKIT_IS_WEB_DATABASE(webDatabase), 0);

#if ENABLE(DATABASE)
    WebKitWebDatabasePrivate* priv = webDatabase->priv;
    WebCore::DatabaseDetails details = WebCore::DatabaseTracker::tracker().detailsForNameAndOrigin(priv->name, core(priv->origin));
    return details.expectedUsage();
#else
    return 0;
#endif
}

/**
 * webkit_web_database_get_size:
 * @web_database: a #WebKitWebDatabase
 *
 * Returns the actual size of the #WebKitWebDatabase space on disk in bytes.
 *
 * Returns: the actual size of the database in bytes
 *
 * Since: 1.1.14
 **/
guint64 webkit_web_database_get_size(WebKitWebDatabase* webDatabase)
{
    g_return_val_if_fail(WEBKIT_IS_WEB_DATABASE(webDatabase), 0);

#if ENABLE(DATABASE)
    WebKitWebDatabasePrivate* priv = webDatabase->priv;
    WebCore::DatabaseDetails details = WebCore::DatabaseTracker::tracker().detailsForNameAndOrigin(priv->name, core(priv->origin));
    return details.currentUsage();
#else
    return 0;
#endif
}

/**
 * webkit_web_database_get_filename:
 * @web_database: a #WebKitWebDatabase
 *
 * Returns the absolute filename to the #WebKitWebDatabase file on disk.
 *
 * Returns: the absolute filename of the database
 *
 * Since: 1.1.14
 **/
G_CONST_RETURN gchar* webkit_web_database_get_filename(WebKitWebDatabase* webDatabase)
{
    g_return_val_if_fail(WEBKIT_IS_WEB_DATABASE(webDatabase), NULL);

#if ENABLE(DATABASE)
    WebKitWebDatabasePrivate* priv = webDatabase->priv;
    WebCore::String coreName = WebCore::String::fromUTF8(priv->name);
    WebCore::String corePath = WebCore::DatabaseTracker::tracker().fullPathForDatabase(core(priv->origin), coreName);

    if (corePath.isEmpty())
        return"";

    g_free(priv->filename);
    priv->filename = g_strdup(corePath.utf8().data());
    return priv->filename;

#else
    return "";
#endif
}

/**
 * webkit_web_database_remove:
 * @web_database: a #WebKitWebDatabase
 *
 * Removes the #WebKitWebDatabase from its security origin and destroys all data
 * stored in the database.
 *
 * Since: 1.1.14
 **/
void webkit_web_database_remove(WebKitWebDatabase* webDatabase)
{
    g_return_if_fail(WEBKIT_IS_WEB_DATABASE(webDatabase));

#if ENABLE(DATABASE)
    WebKitWebDatabasePrivate* priv = webDatabase->priv;
    WebCore::DatabaseTracker::tracker().deleteDatabase(core(priv->origin), priv->name);
#endif
}

/**
 * webkit_remove_all_web_databases:
 *
 * Removes all web databases from the current database directory path.
 *
 * Since: 1.1.14
 **/
void webkit_remove_all_web_databases()
{
#if ENABLE(DATABASE)
    WebCore::DatabaseTracker::tracker().deleteAllDatabases();
#endif
}

/**
 * webkit_get_web_database_directory_path:
 *
 * Returns the current path to the directory WebKit will write Web 
 * Database databases. By default this path will be in the user data
 * directory.
 *
 * Returns: the current database directory path
 *
 * Since: 1.1.14
 **/
G_CONST_RETURN gchar* webkit_get_web_database_directory_path()
{
#if ENABLE(DATABASE)
    WebCore::String path = WebCore::DatabaseTracker::tracker().databaseDirectoryPath();

    if (path.isEmpty())
        return "";

    g_free(webkit_database_directory_path);
    webkit_database_directory_path = g_strdup(path.utf8().data());
    return webkit_database_directory_path;
#else
    return "";
#endif
}

/**
 * webkit_set_web_database_directory_path:
 * @path: the new database directory path
 *
 * Sets the current path to the directory WebKit will write Web 
 * Database databases. 
 *
 * Since: 1.1.14
 **/
void webkit_set_web_database_directory_path(const gchar* path)
{
#if ENABLE(DATABASE)
    WebCore::String corePath = WebCore::String::fromUTF8(path);
    WebCore::DatabaseTracker::tracker().setDatabaseDirectoryPath(corePath);

    g_free(webkit_database_directory_path);
    webkit_database_directory_path = g_strdup(corePath.utf8().data());
#endif
}

/**
 * webkit_get_default_web_database_quota:
 *
 * Returns the default quota for Web Database databases. By default
 * this value is 5MB.
 
 * Returns: the current default database quota in bytes
 *
 * Since: 1.1.14
 **/
guint64 webkit_get_default_web_database_quota()
{
    return webkit_default_database_quota;
}

/**
 * webkit_set_default_web_database_quota:
 * @default_quota: the new default database quota
 *
 * Sets the current path to the directory WebKit will write Web 
 * Database databases. 
 *
 * Since: 1.1.14
 **/
void webkit_set_default_web_database_quota(guint64 defaultQuota)
{
    webkit_default_database_quota = defaultQuota;
}
