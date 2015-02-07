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
 *  Copyright (C) 2003, 2004, 2005, 2006, 2007, 2008 Apple Inc. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this library; see the file COPYING.LIB.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301, USA.
 *
 */

#include "config.h"
#include "Identifier.h"

#include "CallFrame.h"
#include <new> // for placement new
#include <string.h> // for strlen
#include <wtf/Assertions.h>
#include <wtf/FastMalloc.h>
#include <wtf/HashSet.h>

using WTF::ThreadSpecific;

namespace JSC {

typedef HashMap<const char*, RefPtr<UString::Rep>, PtrHash<const char*> > LiteralIdentifierTable;

class IdentifierTable : public FastAllocBase {
public:
    ~IdentifierTable()
    {
        HashSet<UString::Rep*>::iterator end = m_table.end();
        for (HashSet<UString::Rep*>::iterator iter = m_table.begin(); iter != end; ++iter)
            (*iter)->setIsIdentifier(false);
    }
    
    std::pair<HashSet<UString::Rep*>::iterator, bool> add(UString::Rep* value)
    {
        std::pair<HashSet<UString::Rep*>::iterator, bool> result = m_table.add(value);
        (*result.first)->setIsIdentifier(true);
        return result;
    }

    template<typename U, typename V>
    std::pair<HashSet<UString::Rep*>::iterator, bool> add(U value)
    {
        std::pair<HashSet<UString::Rep*>::iterator, bool> result = m_table.add<U, V>(value);
        (*result.first)->setIsIdentifier(true);
        return result;
    }

    void remove(UString::Rep* r) { m_table.remove(r); }

    LiteralIdentifierTable& literalTable() { return m_literalTable; }

private:
    HashSet<UString::Rep*> m_table;
    LiteralIdentifierTable m_literalTable;
};

IdentifierTable* createIdentifierTable()
{
    return new IdentifierTable;
}

void deleteIdentifierTable(IdentifierTable* table)
{
    delete table;
}

bool Identifier::equal(const UString::Rep* r, const char* s)
{
    int length = r->size();
    const UChar* d = r->data();
    for (int i = 0; i != length; ++i)
        if (d[i] != (unsigned char)s[i])
            return false;
    return s[length] == 0;
}

bool Identifier::equal(const UString::Rep* r, const UChar* s, int length)
{
    if (r->size() != length)
        return false;
    const UChar* d = r->data();
    for (int i = 0; i != length; ++i)
        if (d[i] != s[i])
            return false;
    return true;
}

struct CStringTranslator {
    static unsigned hash(const char* c)
    {
        return UString::Rep::computeHash(c);
    }

    static bool equal(UString::Rep* r, const char* s)
    {
        return Identifier::equal(r, s);
    }

    static void translate(UString::Rep*& location, const char* c, unsigned hash)
    {
        size_t length = strlen(c);
        UChar* d;
        UString::Rep* r = UString::Rep::createUninitialized(length, d).releaseRef();
        for (size_t i = 0; i != length; i++)
            d[i] = static_cast<unsigned char>(c[i]); // use unsigned char to zero-extend instead of sign-extend
        r->setHash(hash);
        location = r;
    }
};

PassRefPtr<UString::Rep> Identifier::add(JSGlobalData* globalData, const char* c)
{
    ASSERT(c);

    if (!c[0]) {
        UString::Rep::empty().hash();
        return &UString::Rep::empty();
    }
    if (!c[1])
        return add(globalData, globalData->smallStrings.singleCharacterStringRep(static_cast<unsigned char>(c[0])));

    IdentifierTable& identifierTable = *globalData->identifierTable;
    LiteralIdentifierTable& literalIdentifierTable = identifierTable.literalTable();

    const LiteralIdentifierTable::iterator& iter = literalIdentifierTable.find(c);
    if (iter != literalIdentifierTable.end())
        return iter->second;

    pair<HashSet<UString::Rep*>::iterator, bool> addResult = identifierTable.add<const char*, CStringTranslator>(c);

    // If the string is newly-translated, then we need to adopt it.
    // The boolean in the pair tells us if that is so.
    RefPtr<UString::Rep> addedString = addResult.second ? adoptRef(*addResult.first) : *addResult.first;

    literalIdentifierTable.add(c, addedString.get());

    return addedString.release();
}

PassRefPtr<UString::Rep> Identifier::add(ExecState* exec, const char* c)
{
    return add(&exec->globalData(), c);
}

struct UCharBuffer {
    const UChar* s;
    unsigned int length;
};

struct UCharBufferTranslator {
    static unsigned hash(const UCharBuffer& buf)
    {
        return UString::Rep::computeHash(buf.s, buf.length);
    }

    static bool equal(UString::Rep* str, const UCharBuffer& buf)
    {
        return Identifier::equal(str, buf.s, buf.length);
    }

    static void translate(UString::Rep*& location, const UCharBuffer& buf, unsigned hash)
    {
        UChar* d;
        UString::Rep* r = UString::Rep::createUninitialized(buf.length, d).releaseRef();
        for (unsigned i = 0; i != buf.length; i++)
            d[i] = buf.s[i];
        r->setHash(hash);
        location = r; 
    }
};

PassRefPtr<UString::Rep> Identifier::add(JSGlobalData* globalData, const UChar* s, int length)
{
    if (length == 1) {
        UChar c = s[0];
        if (c <= 0xFF)
            return add(globalData, globalData->smallStrings.singleCharacterStringRep(c));
    }
    if (!length) {
        UString::Rep::empty().hash();
        return &UString::Rep::empty();
    }
    UCharBuffer buf = {s, length}; 
    pair<HashSet<UString::Rep*>::iterator, bool> addResult = globalData->identifierTable->add<UCharBuffer, UCharBufferTranslator>(buf);

    // If the string is newly-translated, then we need to adopt it.
    // The boolean in the pair tells us if that is so.
    return addResult.second ? adoptRef(*addResult.first) : *addResult.first;
}

PassRefPtr<UString::Rep> Identifier::add(ExecState* exec, const UChar* s, int length)
{
    return add(&exec->globalData(), s, length);
}

PassRefPtr<UString::Rep> Identifier::addSlowCase(JSGlobalData* globalData, UString::Rep* r)
{
    ASSERT(!r->isIdentifier());
    if (r->size() == 1) {
        UChar c = r->data()[0];
        if (c <= 0xFF)
            r = globalData->smallStrings.singleCharacterStringRep(c);
            if (r->isIdentifier()) {
#ifndef NDEBUG
                checkSameIdentifierTable(globalData, r);
#endif
                return r;
            }
    }
    if (!r->size()) {
        UString::Rep::empty().hash();
        return &UString::Rep::empty();
    }
    return *globalData->identifierTable->add(r).first;
}

PassRefPtr<UString::Rep> Identifier::addSlowCase(ExecState* exec, UString::Rep* r)
{
    return addSlowCase(&exec->globalData(), r);
}

void Identifier::remove(UString::Rep* r)
{
    currentIdentifierTable()->remove(r);
}

#ifndef NDEBUG

void Identifier::checkSameIdentifierTable(ExecState* exec, UString::Rep*)
{
    ASSERT_UNUSED(exec, exec->globalData().identifierTable == currentIdentifierTable());
}

void Identifier::checkSameIdentifierTable(JSGlobalData* globalData, UString::Rep*)
{
    ASSERT_UNUSED(globalData, globalData->identifierTable == currentIdentifierTable());
}

#else

void Identifier::checkSameIdentifierTable(ExecState*, UString::Rep*)
{
}

void Identifier::checkSameIdentifierTable(JSGlobalData*, UString::Rep*)
{
}

#endif

ThreadSpecific<ThreadIdentifierTableData>* g_identifierTableSpecific = 0;

#if ENABLE(JSC_MULTIPLE_THREADS)

pthread_once_t createIdentifierTableSpecificOnce = PTHREAD_ONCE_INIT;
static void createIdentifierTableSpecificCallback()
{
    ASSERT(!g_identifierTableSpecific);
    g_identifierTableSpecific = new ThreadSpecific<ThreadIdentifierTableData>();
}
void createIdentifierTableSpecific()
{
    pthread_once(&createIdentifierTableSpecificOnce, createIdentifierTableSpecificCallback);
    ASSERT(g_identifierTableSpecific);
}

#else 

void createIdentifierTableSpecific()
{
    ASSERT(!g_identifierTableSpecific);
    g_identifierTableSpecific = new ThreadSpecific<ThreadIdentifierTableData>();
}

#endif

} // namespace JSC
