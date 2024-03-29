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
 * Copyright (C) 2005, 2006, 2008 Apple Inc. All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

#ifndef DeleteSelectionCommand_h
#define DeleteSelectionCommand_h

#include "CompositeEditCommand.h"

namespace WebCore {

class DeleteSelectionCommand : public CompositeEditCommand { 
public:
    static PassRefPtr<DeleteSelectionCommand> create(Document* document, bool smartDelete = false, bool mergeBlocksAfterDelete = true, bool replace = false, bool expandForSpecialElements = false)
    {
        return adoptRef(new DeleteSelectionCommand(document, smartDelete, mergeBlocksAfterDelete, replace, expandForSpecialElements));
    }
    static PassRefPtr<DeleteSelectionCommand> create(const VisibleSelection& selection, bool smartDelete = false, bool mergeBlocksAfterDelete = true, bool replace = false, bool expandForSpecialElements = false)
    {
        return adoptRef(new DeleteSelectionCommand(selection, smartDelete, mergeBlocksAfterDelete, replace, expandForSpecialElements));
    }

private:
    DeleteSelectionCommand(Document*, bool smartDelete, bool mergeBlocksAfterDelete, bool replace, bool expandForSpecialElements);
    DeleteSelectionCommand(const VisibleSelection&, bool smartDelete, bool mergeBlocksAfterDelete, bool replace, bool expandForSpecialElements);

    virtual void doApply();
    virtual EditAction editingAction() const;
    
    virtual bool preservesTypingStyle() const;

    void initializeStartEnd(Position&, Position&);
    void initializePositionData();
    void saveTypingStyleState();
    void insertPlaceholderForAncestorBlockContent();
    bool handleSpecialCaseBRDelete();
    void handleGeneralDelete();
    void fixupWhitespace();
    void mergeParagraphs();
    void removePreviouslySelectedEmptyTableRows();
    void calculateEndingPosition();
    void calculateTypingStyleAfterDelete();
    void clearTransientState();
    virtual void removeNode(PassRefPtr<Node>);
    virtual void deleteTextFromNode(PassRefPtr<Text>, unsigned, unsigned);

    bool m_hasSelectionToDelete;
    bool m_smartDelete;
    bool m_mergeBlocksAfterDelete;
    bool m_needPlaceholder;
    bool m_replace;
    bool m_expandForSpecialElements;
    bool m_pruneStartBlockIfNecessary;
    bool m_startsAtEmptyLine;

    // This data is transient and should be cleared at the end of the doApply function.
    VisibleSelection m_selectionToDelete;
    Position m_upstreamStart;
    Position m_downstreamStart;
    Position m_upstreamEnd;
    Position m_downstreamEnd;
    Position m_endingPosition;
    Position m_leadingWhitespace;
    Position m_trailingWhitespace;
    RefPtr<Node> m_startBlock;
    RefPtr<Node> m_endBlock;
    RefPtr<CSSMutableStyleDeclaration> m_typingStyle;
    RefPtr<CSSMutableStyleDeclaration> m_deleteIntoBlockquoteStyle;
    RefPtr<Node> m_startRoot;
    RefPtr<Node> m_endRoot;
    RefPtr<Node> m_startTableRow;
    RefPtr<Node> m_endTableRow;
    RefPtr<Node> m_temporaryPlaceholder;
};

} // namespace WebCore

#endif // DeleteSelectionCommand_h
