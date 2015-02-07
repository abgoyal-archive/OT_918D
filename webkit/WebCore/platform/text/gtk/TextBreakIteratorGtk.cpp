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
 * Copyright (C) 2006 Lars Knoll <lars@trolltech.com>
 * Copyright (C) 2007 Apple Inc. All rights reserved.
 * Copyright (C) 2008 Jürg Billeter <j@bitron.ch>
 * Copyright (C) 2008 Dominik Röttsches <dominik.roettsches@access-company.com>
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
 *
 */

#include "config.h"
#include "TextBreakIterator.h"

#include <pango/pango.h>
#include <wtf/gtk/GOwnPtr.h>

namespace WebCore {

enum UBreakIteratorType {
    UBRK_CHARACTER,
    UBRK_WORD,
    UBRK_LINE,
    UBRK_SENTENCE
};

class TextBreakIterator {
public:
    UBreakIteratorType m_type;
    int m_length;
    PangoLogAttr* m_logAttrs;
    int m_index;
};

static TextBreakIterator* setUpIterator(bool& createdIterator, TextBreakIterator*& iterator,
    UBreakIteratorType type, const UChar* string, int length)
{
    if (!string)
        return 0;

    if (!createdIterator) {
        iterator = new TextBreakIterator();
        createdIterator = true;
    }
    if (!iterator)
        return 0;

    long utf8len;
    GOwnPtr<char> utf8;
    utf8.set(g_utf16_to_utf8(string, length, 0, &utf8len, 0));

    // FIXME: assumes no surrogate pairs

    iterator->m_type = type;
    iterator->m_length = length;
    if (createdIterator)
        g_free(iterator->m_logAttrs);
    iterator->m_logAttrs = g_new0(PangoLogAttr, length + 1);
    iterator->m_index = -1;
    pango_get_log_attrs(utf8.get(), utf8len, -1, 0, iterator->m_logAttrs, length + 1);

    return iterator;
}

TextBreakIterator* characterBreakIterator(const UChar* string, int length)
{
    static bool createdCharacterBreakIterator = false;
    static TextBreakIterator* staticCharacterBreakIterator;
    return setUpIterator(createdCharacterBreakIterator, staticCharacterBreakIterator, UBRK_CHARACTER, string, length);
}

TextBreakIterator* cursorMovementIterator(const UChar* string, int length)
{
    // FIXME: This needs closer inspection to achieve behaviour identical to the ICU version.
    return characterBreakIterator(string, length);
}

TextBreakIterator* wordBreakIterator(const UChar* string, int length)
{
    static bool createdWordBreakIterator = false;
    static TextBreakIterator* staticWordBreakIterator;
    return setUpIterator(createdWordBreakIterator, staticWordBreakIterator, UBRK_WORD, string, length);
}

TextBreakIterator* lineBreakIterator(const UChar* string, int length)
{
    static bool createdLineBreakIterator = false;
    static TextBreakIterator* staticLineBreakIterator;
    return setUpIterator(createdLineBreakIterator, staticLineBreakIterator, UBRK_LINE, string, length);
}

TextBreakIterator* sentenceBreakIterator(const UChar* string, int length)
{
    static bool createdSentenceBreakIterator = false;
    static TextBreakIterator* staticSentenceBreakIterator;
    return setUpIterator(createdSentenceBreakIterator, staticSentenceBreakIterator, UBRK_SENTENCE, string, length);
}

int textBreakFirst(TextBreakIterator* bi)
{
    // see textBreakLast
    
    int firstCursorPosition = -1;
    int pos = 0;
    while (pos <= bi->m_length && (firstCursorPosition < 0)) {
        if (bi->m_logAttrs[pos].is_cursor_position)
            firstCursorPosition = pos;
    }
    bi->m_index = firstCursorPosition;
    return firstCursorPosition;
}

int textBreakLast(TextBreakIterator* bi)
{
    // TextBreakLast is not meant to find just any break according to bi->m_type 
    // but really the one near the last character.
    // (cmp ICU documentation for ubrk_first and ubrk_last)
    // From ICU docs for ubrk_last:
    // "Determine the index immediately beyond the last character in the text being scanned." 

    // So we should advance or traverse back based on bi->m_logAttrs cursor positions.
    // If last character position in the original string is a whitespace,
    // traverse to the left until the first non-white character position is found
    // and return the position of the first white-space char after this one.
    // Otherwise return m_length, as "the first character beyond the last" is outside our string.
    
    bool whiteSpaceAtTheEnd = true;
    int nextWhiteSpacePos = bi->m_length;
    
    int pos = bi->m_length;
    while (pos >= 0 && whiteSpaceAtTheEnd) {
        if (bi->m_logAttrs[pos].is_cursor_position) {
            if (whiteSpaceAtTheEnd = bi->m_logAttrs[pos].is_white)
                nextWhiteSpacePos = pos;
        }
        pos--;
    }
    bi->m_index = nextWhiteSpacePos;
    return nextWhiteSpacePos;
}

int textBreakNext(TextBreakIterator* bi)
{
    for (int i = bi->m_index + 1; i <= bi->m_length; i++) {

        // FIXME: UBRK_WORD case: Single multibyte characters (i.e. white space around them), such as the euro symbol €, 
        // are not marked as word_start & word_end as opposed to the way ICU does it.
        // This leads to - for example - different word selection behaviour when right clicking.

        if ((bi->m_type == UBRK_LINE && bi->m_logAttrs[i].is_line_break)
            || (bi->m_type == UBRK_WORD && (bi->m_logAttrs[i].is_word_start || bi->m_logAttrs[i].is_word_end))
            || (bi->m_type == UBRK_CHARACTER && bi->m_logAttrs[i].is_cursor_position)
            || (bi->m_type == UBRK_SENTENCE && (bi->m_logAttrs[i].is_sentence_start || bi->m_logAttrs[i].is_sentence_end)) ) {
            bi->m_index = i;
            return i;
        }
    }
    return TextBreakDone;
}

int textBreakPrevious(TextBreakIterator* bi)
{
    for (int i = bi->m_index - 1; i >= 0; i--) {
        if ((bi->m_type == UBRK_LINE && bi->m_logAttrs[i].is_line_break)
            || (bi->m_type == UBRK_WORD && (bi->m_logAttrs[i].is_word_start || bi->m_logAttrs[i].is_word_end))
            || (bi->m_type == UBRK_CHARACTER && bi->m_logAttrs[i].is_cursor_position)
            || (bi->m_type == UBRK_SENTENCE && (bi->m_logAttrs[i].is_sentence_start || bi->m_logAttrs[i].is_sentence_end)) ) {
            bi->m_index = i;
            return i;
        }
    }
    return textBreakFirst(bi);
}

int textBreakPreceding(TextBreakIterator* bi, int pos)
{
    bi->m_index = pos;
    return textBreakPrevious(bi);
}

int textBreakFollowing(TextBreakIterator* bi, int pos)
{
    if (pos < 0)
        pos = -1;
    bi->m_index = pos;
    return textBreakNext(bi);
}

int textBreakCurrent(TextBreakIterator* bi)
{
    return bi->m_index;
}

bool isTextBreak(TextBreakIterator* bi, int pos)
{
    if (bi->m_index < 0)
        return false;

    return ((bi->m_type == UBRK_LINE && bi->m_logAttrs[bi->m_index].is_line_break)
        || (bi->m_type == UBRK_WORD && bi->m_logAttrs[bi->m_index].is_word_end)
        || (bi->m_type == UBRK_CHARACTER && bi->m_logAttrs[bi->m_index].is_char_break)
        || (bi->m_type == UBRK_SENTENCE && bi->m_logAttrs[bi->m_index].is_sentence_end) );
}

}
