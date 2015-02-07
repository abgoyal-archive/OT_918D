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
 * Copyright (C) 2007, 2008 Apple Inc. All rights reserved.
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

#ifndef Database_h
#define Database_h

#if ENABLE(DATABASE)
#include "PlatformString.h"
#include "SecurityOrigin.h"
#include "SQLiteDatabase.h"
#include "SQLTransaction.h"
#include "StringHash.h"
#include "Timer.h"
#include "VoidCallback.h"

#include <wtf/Forward.h>
#include <wtf/HashSet.h>
#include <wtf/PassRefPtr.h>
#include <wtf/RefPtr.h>
#include <wtf/Deque.h>
#else
#include "PlatformString.h"
#endif

#if ENABLE(DATABASE)
namespace WebCore {

class DatabaseAuthorizer;
class DatabaseThread;
class ScriptExecutionContext;
class SQLResultSet;
class SQLTransactionCallback;
class SQLTransactionClient;
class SQLTransactionCoordinator;
class SQLTransactionErrorCallback;
class SQLValue;

typedef int ExceptionCode;

class Database : public ThreadSafeShared<Database> {
    friend class DatabaseTransactionTask;
    friend class SQLStatement;
    friend class SQLTransaction;
public:
    static void setIsAvailable(bool);
    static bool isAvailable();

    ~Database();

// Direct support for the DOM API
    static PassRefPtr<Database> openDatabase(ScriptExecutionContext* context, const String& name, const String& expectedVersion, const String& displayName, unsigned long estimatedSize, ExceptionCode&);
    String version() const;
    void changeVersion(const String& oldVersion, const String& newVersion,
                       PassRefPtr<SQLTransactionCallback> callback, PassRefPtr<SQLTransactionErrorCallback> errorCallback,
                       PassRefPtr<VoidCallback> successCallback);
    void transaction(PassRefPtr<SQLTransactionCallback> callback, PassRefPtr<SQLTransactionErrorCallback> errorCallback,
                     PassRefPtr<VoidCallback> successCallback, bool readOnly);

// Internal engine support
    static const String& databaseInfoTableName();

    void disableAuthorizer();
    void enableAuthorizer();
    void setAuthorizerReadOnly();

    Vector<String> tableNames();

    ScriptExecutionContext* scriptExecutionContext() const { return m_scriptExecutionContext.get(); }
    SecurityOrigin* securityOrigin() const;
    String stringIdentifier() const;
    String displayName() const;
    unsigned long estimatedSize() const;
    String fileName() const;

    bool getVersionFromDatabase(String&);
    bool setVersionInDatabase(const String&);
    void setExpectedVersion(const String&);
    bool versionMatchesExpected() const;

    void markAsDeletedAndClose();
    bool deleted() const { return m_deleted; }

    void close();
    bool opened() const { return m_opened; }

    void stop();
    bool stopped() const { return m_stopped; }

    unsigned long long databaseSize() const;
    unsigned long long maximumSize() const;

// Called from DatabaseThread, must be prepared to work on the background thread
    void resetAuthorizer();
    void performPolicyChecks();

    bool performOpenAndVerify(ExceptionCode&);

    Vector<String> performGetTableNames();

    SQLTransactionClient* transactionClient() const;
    SQLTransactionCoordinator* transactionCoordinator() const;

private:
    Database(ScriptExecutionContext* context, const String& name,
             const String& expectedVersion, const String& displayName,
             unsigned long estimatedSize);

    bool openAndVerifyVersion(ExceptionCode&);

    void scheduleTransaction();
    void scheduleTransactionCallback(SQLTransaction*);
    void scheduleTransactionStep(SQLTransaction* transaction, bool immediately = false);

    Deque<RefPtr<SQLTransaction> > m_transactionQueue;
    Mutex m_transactionInProgressMutex;
    bool m_transactionInProgress;
    bool m_isTransactionQueueEnabled;

    static void deliverPendingCallback(void*);

    RefPtr<ScriptExecutionContext> m_scriptExecutionContext;
    RefPtr<SecurityOrigin> m_mainThreadSecurityOrigin;
    RefPtr<SecurityOrigin> m_databaseThreadSecurityOrigin;
    String m_name;
    int m_guid;
    String m_expectedVersion;
    String m_displayName;
    unsigned long m_estimatedSize;
    String m_filename;

    bool m_deleted;

    bool m_stopped;

    bool m_opened;

    SQLiteDatabase m_sqliteDatabase;
    RefPtr<DatabaseAuthorizer> m_databaseAuthorizer;

#ifndef NDEBUG
    String databaseDebugName() const { return m_mainThreadSecurityOrigin->toString() + "::" + m_name; }
#endif
};

} // namespace WebCore

#else

namespace WebCore {
class Database : public ThreadSafeShared<Database> {
public:
    static const String& databaseInfoTableName();
};
} // namespace WebCore

#endif // ENABLE(DATABASE)

#endif // Database_h
