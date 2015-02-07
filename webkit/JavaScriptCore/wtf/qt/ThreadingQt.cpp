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
 * Copyright (C) 2007 Apple Inc. All rights reserved.
 * Copyright (C) 2007 Justin Haygood (jhaygood@reaktix.com)
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
#include "config.h"
#include "Threading.h"

#if !ENABLE(SINGLE_THREADED)

#include "CurrentTime.h"
#include "HashMap.h"
#include "MainThread.h"
#include "RandomNumberSeed.h"

#include <QCoreApplication>
#include <QMutex>
#include <QThread>
#include <QWaitCondition>

namespace WTF {

class ThreadPrivate : public QThread {
public:
    ThreadPrivate(ThreadFunction entryPoint, void* data);
    void run();
    void* getReturnValue() { return m_returnValue; }
private:
    void* m_data;
    ThreadFunction m_entryPoint;
    void* m_returnValue;
};

ThreadPrivate::ThreadPrivate(ThreadFunction entryPoint, void* data) 
    : m_data(data)
    , m_entryPoint(entryPoint)
    , m_returnValue(0)
{
}

void ThreadPrivate::run()
{
    m_returnValue = m_entryPoint(m_data);
}

class ThreadMonitor : public QObject {
    Q_OBJECT
public:
    static ThreadMonitor * instance()
    {
        static ThreadMonitor *instance = new ThreadMonitor();
        return instance;
    }

public Q_SLOTS:
    void threadFinished()
    {
        sender()->deleteLater();
    }
};

static Mutex* atomicallyInitializedStaticMutex;

static ThreadIdentifier mainThreadIdentifier;

static Mutex& threadMapMutex()
{
    static Mutex mutex;
    return mutex;
}

static HashMap<ThreadIdentifier, QThread*>& threadMap()
{
    static HashMap<ThreadIdentifier, QThread*> map;
    return map;
}

static ThreadIdentifier identifierByQthreadHandle(QThread*& thread)
{
    MutexLocker locker(threadMapMutex());

    HashMap<ThreadIdentifier, QThread*>::iterator i = threadMap().begin();
    for (; i != threadMap().end(); ++i) {
        if (i->second == thread)
            return i->first;
    }

    return 0;
}

static ThreadIdentifier establishIdentifierForThread(QThread*& thread)
{
    ASSERT(!identifierByQthreadHandle(thread));

    MutexLocker locker(threadMapMutex());

    static ThreadIdentifier identifierCount = 1;

    threadMap().add(identifierCount, thread);

    return identifierCount++;
}

static void clearThreadForIdentifier(ThreadIdentifier id)
{
    MutexLocker locker(threadMapMutex());

    ASSERT(threadMap().contains(id));

    threadMap().remove(id);
}

static QThread* threadForIdentifier(ThreadIdentifier id)
{
    MutexLocker locker(threadMapMutex());

    return threadMap().get(id);
}

void initializeThreading()
{
    if (!atomicallyInitializedStaticMutex) {
        atomicallyInitializedStaticMutex = new Mutex;
        threadMapMutex();
        initializeRandomNumberGenerator();
        QThread* mainThread = QCoreApplication::instance()->thread();
        mainThreadIdentifier = identifierByQthreadHandle(mainThread);
        if (!mainThreadIdentifier)
            mainThreadIdentifier = establishIdentifierForThread(mainThread);
        initializeMainThread();
    }
}

void lockAtomicallyInitializedStaticMutex()
{
    ASSERT(atomicallyInitializedStaticMutex);
    atomicallyInitializedStaticMutex->lock();
}

void unlockAtomicallyInitializedStaticMutex()
{
    atomicallyInitializedStaticMutex->unlock();
}

ThreadIdentifier createThreadInternal(ThreadFunction entryPoint, void* data, const char*)
{
    ThreadPrivate* thread = new ThreadPrivate(entryPoint, data);
    if (!thread) {
        LOG_ERROR("Failed to create thread at entry point %p with data %p", entryPoint, data);
        return 0;
    }

    QObject::connect(thread, SIGNAL(finished()), ThreadMonitor::instance(), SLOT(threadFinished()));

    thread->start();

    QThread* threadRef = static_cast<QThread*>(thread);

    return establishIdentifierForThread(threadRef);
}

void initializeCurrentThreadInternal(const char*)
{
}

int waitForThreadCompletion(ThreadIdentifier threadID, void** result)
{
    ASSERT(threadID);

    QThread* thread = threadForIdentifier(threadID);

    bool res = thread->wait();

    clearThreadForIdentifier(threadID);
    if (result)
        *result = static_cast<ThreadPrivate*>(thread)->getReturnValue();

    return !res;
}

void detachThread(ThreadIdentifier threadID)
{
    ASSERT(threadID);
    clearThreadForIdentifier(threadID);
}

ThreadIdentifier currentThread()
{
    QThread* currentThread = QThread::currentThread();
    if (ThreadIdentifier id = identifierByQthreadHandle(currentThread))
        return id;
    return establishIdentifierForThread(currentThread);
}

bool isMainThread()
{
    return QThread::currentThread() == QCoreApplication::instance()->thread();
}

Mutex::Mutex()
    : m_mutex(new QMutex())
{
}

Mutex::~Mutex()
{
    delete m_mutex;
}

void Mutex::lock()
{
    m_mutex->lock();
}

bool Mutex::tryLock()
{
    return m_mutex->tryLock();
}

void Mutex::unlock()
{
    m_mutex->unlock();
}

ThreadCondition::ThreadCondition()
    : m_condition(new QWaitCondition())
{
}

ThreadCondition::~ThreadCondition()
{
    delete m_condition;
}

void ThreadCondition::wait(Mutex& mutex)
{
    m_condition->wait(mutex.impl());
}

bool ThreadCondition::timedWait(Mutex& mutex, double absoluteTime)
{
    double currentTime = WTF::currentTime();

    // Time is in the past - return immediately.
    if (absoluteTime < currentTime)
        return false;

    // Time is too far in the future (and would overflow unsigned long) - wait forever.
    if (absoluteTime - currentTime > static_cast<double>(INT_MAX) / 1000.0) {
        wait(mutex);
        return true;
    }

    double intervalMilliseconds = (absoluteTime - currentTime) * 1000.0;
    return m_condition->wait(mutex.impl(), static_cast<unsigned long>(intervalMilliseconds));
}

void ThreadCondition::signal()
{
    m_condition->wakeOne();
}

void ThreadCondition::broadcast()
{
    m_condition->wakeAll();
}

} // namespace WebCore

#include "ThreadingQt.moc"

#endif
