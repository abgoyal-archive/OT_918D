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
 * Copyright (C) 2000 Lars Knoll (knoll@kde.org)
 * Copyright (C) 2003, 2004, 2006, 2007, 2008, 2009, 2010 Apple Inc. All right reserved.
 * Copyright (C) 2010 Google Inc. All rights reserved.
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

#include "BidiResolver.h"
#include "CharacterNames.h"
#include "InlineIterator.h"
#include "InlineTextBox.h"
#include "Logging.h"
#include "RenderArena.h"
#include "RenderInline.h"
#include "RenderListMarker.h"
#include "RenderView.h"
#include "TrailingFloatsRootInlineBox.h"
#include "break_lines.h"
#include <wtf/AlwaysInline.h>
#include <wtf/RefCountedLeakCounter.h>
#include <wtf/StdLibExtras.h>
#include <wtf/Vector.h>
#ifdef ANDROID_LAYOUT
#include "Frame.h"
#include "FrameTree.h"
#include "Settings.h"
#include "Text.h"
#include "HTMLNames.h"
#endif // ANDROID_LAYOUT

using namespace std;
using namespace WTF;
using namespace Unicode;

namespace WebCore {

// We don't let our line box tree for a single line get any deeper than this.
const unsigned cMaxLineDepth = 200;

static int getBorderPaddingMargin(RenderBoxModelObject* child, bool endOfInline)
{
    bool leftSide = (child->style()->direction() == LTR) ? !endOfInline : endOfInline;
    if (leftSide)
        return child->marginLeft() + child->paddingLeft() + child->borderLeft();
    return child->marginRight() + child->paddingRight() + child->borderRight();
}

static int inlineWidth(RenderObject* child, bool start = true, bool end = true)
{
    unsigned lineDepth = 1;
    int extraWidth = 0;
    RenderObject* parent = child->parent();
    while (parent->isInline() && !parent->isInlineBlockOrInlineTable() && lineDepth++ < cMaxLineDepth) {
        if (start && !child->previousSibling())
            extraWidth += getBorderPaddingMargin(toRenderBoxModelObject(parent), false);
        if (end && !child->nextSibling())
            extraWidth += getBorderPaddingMargin(toRenderBoxModelObject(parent), true);
        child = parent;
        parent = child->parent();
    }
    return extraWidth;
}

static void chopMidpointsAt(LineMidpointState& lineMidpointState, RenderObject* obj, unsigned pos)
{
    if (!lineMidpointState.numMidpoints)
        return;
    InlineIterator* midpoints = lineMidpointState.midpoints.data();
    for (int i = lineMidpointState.numMidpoints - 1; i >= 0; i--) {
        const InlineIterator& point = midpoints[i];
        if (point.obj == obj && point.pos == pos) {
            lineMidpointState.numMidpoints = i;
            break;
        }
    }
}

static void checkMidpoints(LineMidpointState& lineMidpointState, InlineIterator& lBreak)
{
    // Check to see if our last midpoint is a start point beyond the line break.  If so,
    // shave it off the list, and shave off a trailing space if the previous end point doesn't
    // preserve whitespace.
    if (lBreak.obj && lineMidpointState.numMidpoints && !(lineMidpointState.numMidpoints % 2)) {
        InlineIterator* midpoints = lineMidpointState.midpoints.data();
        InlineIterator& endpoint = midpoints[lineMidpointState.numMidpoints - 2];
        const InlineIterator& startpoint = midpoints[lineMidpointState.numMidpoints - 1];
        InlineIterator currpoint = endpoint;
        while (!currpoint.atEnd() && currpoint != startpoint && currpoint != lBreak)
            currpoint.increment();
        if (currpoint == lBreak) {
            // We hit the line break before the start point.  Shave off the start point.
            lineMidpointState.numMidpoints--;
            if (endpoint.obj->style()->collapseWhiteSpace()) {
                if (endpoint.obj->isText()) {
                    // Don't shave a character off the endpoint if it was from a soft hyphen.
                    RenderText* textObj = toRenderText(endpoint.obj);
                    if (endpoint.pos + 1 < textObj->textLength()) {
                        if (textObj->characters()[endpoint.pos+1] == softHyphen)
                            return;
                    } else if (startpoint.obj->isText()) {
                        RenderText *startText = toRenderText(startpoint.obj);
                        if (startText->textLength() && startText->characters()[0] == softHyphen)
                            return;
                    }
                }
                endpoint.pos--;
            }
        }
    }    
}

static void addMidpoint(LineMidpointState& lineMidpointState, const InlineIterator& midpoint)
{
    if (lineMidpointState.midpoints.size() <= lineMidpointState.numMidpoints)
        lineMidpointState.midpoints.grow(lineMidpointState.numMidpoints + 10);

    InlineIterator* midpoints = lineMidpointState.midpoints.data();
    midpoints[lineMidpointState.numMidpoints++] = midpoint;
}

void RenderBlock::appendRunsForObject(int start, int end, RenderObject* obj, InlineBidiResolver& resolver)
{
    if (start > end || obj->isFloating() ||
        (obj->isPositioned() && !obj->style()->hasStaticX() && !obj->style()->hasStaticY() && !obj->container()->isRenderInline()))
        return;

    LineMidpointState& lineMidpointState = resolver.midpointState();
    bool haveNextMidpoint = (lineMidpointState.currentMidpoint < lineMidpointState.numMidpoints);
    InlineIterator nextMidpoint;
    if (haveNextMidpoint)
        nextMidpoint = lineMidpointState.midpoints[lineMidpointState.currentMidpoint];
    if (lineMidpointState.betweenMidpoints) {
        if (!(haveNextMidpoint && nextMidpoint.obj == obj))
            return;
        // This is a new start point. Stop ignoring objects and 
        // adjust our start.
        lineMidpointState.betweenMidpoints = false;
        start = nextMidpoint.pos;
        lineMidpointState.currentMidpoint++;
        if (start < end)
            return appendRunsForObject(start, end, obj, resolver);
    } else {
        if (!haveNextMidpoint || (obj != nextMidpoint.obj)) {
            resolver.addRun(new (obj->renderArena()) BidiRun(start, end, obj, resolver.context(), resolver.dir()));
            return;
        }

        // An end midpoint has been encountered within our object.  We
        // need to go ahead and append a run with our endpoint.
        if (static_cast<int>(nextMidpoint.pos + 1) <= end) {
            lineMidpointState.betweenMidpoints = true;
            lineMidpointState.currentMidpoint++;
            if (nextMidpoint.pos != UINT_MAX) { // UINT_MAX means stop at the object and don't include any of it.
                if (static_cast<int>(nextMidpoint.pos + 1) > start)
                    resolver.addRun(new (obj->renderArena())
                        BidiRun(start, nextMidpoint.pos + 1, obj, resolver.context(), resolver.dir()));
                return appendRunsForObject(nextMidpoint.pos + 1, end, obj, resolver);
            }
        } else
           resolver.addRun(new (obj->renderArena()) BidiRun(start, end, obj, resolver.context(), resolver.dir()));
    }
}

static inline InlineBox* createInlineBoxForRenderer(RenderObject* obj, bool isRootLineBox, bool isOnlyRun = false)
{
    if (isRootLineBox)
        return toRenderBlock(obj)->createAndAppendRootInlineBox();
    
    if (obj->isText()) {
        InlineTextBox* textBox = toRenderText(obj)->createInlineTextBox();
        // We only treat a box as text for a <br> if we are on a line by ourself or in strict mode
        // (Note the use of strict mode.  In "almost strict" mode, we don't treat the box for <br> as text.)
        if (obj->isBR())
            textBox->setIsText(isOnlyRun || obj->document()->inStrictMode());
        return textBox;
    }
    
    if (obj->isBox())
        return toRenderBox(obj)->createInlineBox();
    
    return toRenderInline(obj)->createAndAppendInlineFlowBox();
}

static inline void dirtyLineBoxesForRenderer(RenderObject* o, bool fullLayout)
{
    if (o->isText()) {
        if (o->prefWidthsDirty() && o->isCounter())
            toRenderText(o)->calcPrefWidths(0); // FIXME: Counters depend on this hack. No clue why. Should be investigated and removed.
        toRenderText(o)->dirtyLineBoxes(fullLayout);
    } else
        toRenderInline(o)->dirtyLineBoxes(fullLayout);
}

InlineFlowBox* RenderBlock::createLineBoxes(RenderObject* obj, bool firstLine)
{
    // See if we have an unconstructed line box for this object that is also
    // the last item on the line.
    unsigned lineDepth = 1;
    InlineFlowBox* childBox = 0;
    InlineFlowBox* parentBox = 0;
    InlineFlowBox* result = 0;
    do {
        ASSERT(obj->isRenderInline() || obj == this);
        
        // Get the last box we made for this render object.
        parentBox = obj->isRenderInline() ? toRenderInline(obj)->lastLineBox() : toRenderBlock(obj)->lastLineBox();

        // If this box is constructed then it is from a previous line, and we need
        // to make a new box for our line.  If this box is unconstructed but it has
        // something following it on the line, then we know we have to make a new box
        // as well.  In this situation our inline has actually been split in two on
        // the same line (this can happen with very fancy language mixtures).
        bool constructedNewBox = false;
        if (!parentBox || parentBox->isConstructed() || parentBox->nextOnLine()) {
            // We need to make a new box for this render object.  Once
            // made, we need to place it at the end of the current line.
            InlineBox* newBox = createInlineBoxForRenderer(obj, obj == this);
            ASSERT(newBox->isInlineFlowBox());
            parentBox = static_cast<InlineFlowBox*>(newBox);
            parentBox->setFirstLineStyleBit(firstLine);
            constructedNewBox = true;
        }

        if (!result)
            result = parentBox;

        // If we have hit the block itself, then |box| represents the root
        // inline box for the line, and it doesn't have to be appended to any parent
        // inline.
        if (childBox)
            parentBox->addToLine(childBox);

        if (!constructedNewBox || obj == this)
            break;

        childBox = parentBox;        

        // If we've exceeded our line depth, then jump straight to the root and skip all the remaining
        // intermediate inline flows.
        obj = (++lineDepth >= cMaxLineDepth) ? this : obj->parent();

    } while (true);

    return result;
}

RootInlineBox* RenderBlock::constructLine(unsigned runCount, BidiRun* firstRun, BidiRun* lastRun, bool firstLine, bool lastLine, RenderObject* endObject)
{
    ASSERT(firstRun);

    bool rootHasSelectedChildren = false;
    InlineFlowBox* parentBox = 0;
    for (BidiRun* r = firstRun; r; r = r->next()) {
        // Create a box for our object.
        bool isOnlyRun = (runCount == 1);
        if (runCount == 2 && !r->m_object->isListMarker())
            isOnlyRun = ((style()->direction() == RTL) ? lastRun : firstRun)->m_object->isListMarker();

        InlineBox* box = createInlineBoxForRenderer(r->m_object, false, isOnlyRun);
        r->m_box = box;

        ASSERT(box);
        if (!box)
            continue;

        if (!rootHasSelectedChildren && box->renderer()->selectionState() != RenderObject::SelectionNone)
            rootHasSelectedChildren = true;

        // If we have no parent box yet, or if the run is not simply a sibling,
        // then we need to construct inline boxes as necessary to properly enclose the
        // run's inline box.
        if (!parentBox || parentBox->renderer() != r->m_object->parent())
            // Create new inline boxes all the way back to the appropriate insertion point.
            parentBox = createLineBoxes(r->m_object->parent(), firstLine);

        // Append the inline box to this line.
        parentBox->addToLine(box);

        bool visuallyOrdered = r->m_object->style()->visuallyOrdered();
        box->setBidiLevel(visuallyOrdered ? 0 : r->level());

        if (box->isInlineTextBox()) {
            InlineTextBox* text = static_cast<InlineTextBox*>(box);
            text->setStart(r->m_start);
            text->setLen(r->m_stop - r->m_start);
            text->m_dirOverride = r->dirOverride(visuallyOrdered);
        }
    }

    // We should have a root inline box.  It should be unconstructed and
    // be the last continuation of our line list.
    ASSERT(lastLineBox() && !lastLineBox()->isConstructed());

    // Set the m_selectedChildren flag on the root inline box if one of the leaf inline box
    // from the bidi runs walk above has a selection state.
    if (rootHasSelectedChildren)
        lastLineBox()->root()->setHasSelectedChildren(true);

    // Set bits on our inline flow boxes that indicate which sides should
    // paint borders/margins/padding.  This knowledge will ultimately be used when
    // we determine the horizontal positions and widths of all the inline boxes on
    // the line.
    lastLineBox()->determineSpacingForFlowBoxes(lastLine, endObject);

    // Now mark the line boxes as being constructed.
    lastLineBox()->setConstructed();

    // Return the last line.
    return lastRootBox();
}

void RenderBlock::computeHorizontalPositionsForLine(RootInlineBox* lineBox, bool firstLine, BidiRun* firstRun, BidiRun* trailingSpaceRun, bool reachedEnd)
{
    // First determine our total width.
    int availableWidth = lineWidth(height(), firstLine);
    int totWidth = lineBox->getFlowSpacingWidth();
    bool needsWordSpacing = false;
    unsigned numSpaces = 0;
    ETextAlign textAlign = style()->textAlign();

    for (BidiRun* r = firstRun; r; r = r->next()) {
        if (!r->m_box || r->m_object->isPositioned() || r->m_box->isLineBreak())
            continue; // Positioned objects are only participating to figure out their
                      // correct static x position.  They have no effect on the width.
                      // Similarly, line break boxes have no effect on the width.
        if (r->m_object->isText()) {
            RenderText* rt = toRenderText(r->m_object);

            if (textAlign == JUSTIFY && r != trailingSpaceRun) {
                const UChar* characters = rt->characters();
                for (int i = r->m_start; i < r->m_stop; i++) {
                    UChar c = characters[i];
                    if (c == ' ' || c == '\n' || c == '\t')
                        numSpaces++;
                }
            }

            if (int length = rt->textLength()) {
                if (!r->m_start && needsWordSpacing && isSpaceOrNewline(rt->characters()[r->m_start]))
                    totWidth += rt->style(firstLine)->font().wordSpacing();
                needsWordSpacing = !isSpaceOrNewline(rt->characters()[r->m_stop - 1]) && r->m_stop == length;          
            }
            HashSet<const SimpleFontData*> fallbackFonts;
            r->m_box->setWidth(rt->width(r->m_start, r->m_stop - r->m_start, totWidth, firstLine, &fallbackFonts));
            if (!fallbackFonts.isEmpty()
#if ENABLE(SVG)
                    && !isSVGText()
#endif
            ) {
                ASSERT(r->m_box->isText());
                static_cast<InlineTextBox*>(r->m_box)->setFallbackFonts(fallbackFonts);
            }
        } else if (!r->m_object->isRenderInline()) {
            RenderBox* renderBox = toRenderBox(r->m_object);
            renderBox->calcWidth();
            r->m_box->setWidth(renderBox->width());
            totWidth += renderBox->marginLeft() + renderBox->marginRight();
        }

        totWidth += r->m_box->width();
    }

    // Armed with the total width of the line (without justification),
    // we now examine our text-align property in order to determine where to position the
    // objects horizontally.  The total width of the line can be increased if we end up
    // justifying text.
    int x = leftOffset(height(), firstLine);
    switch (textAlign) {
        case LEFT:
        case WEBKIT_LEFT:
            // The direction of the block should determine what happens with wide lines.  In
            // particular with RTL blocks, wide lines should still spill out to the left.
            if (style()->direction() == LTR) {
                if (totWidth > availableWidth && trailingSpaceRun)
                    trailingSpaceRun->m_box->setWidth(max(0, trailingSpaceRun->m_box->width() - totWidth + availableWidth));
            } else {
                if (trailingSpaceRun)
                    trailingSpaceRun->m_box->setWidth(0);
                else if (totWidth > availableWidth)
                    x -= (totWidth - availableWidth);
            }
            break;
        case JUSTIFY:
            if (numSpaces && !reachedEnd && !lineBox->endsWithBreak()) {
                if (trailingSpaceRun) {
                    totWidth -= trailingSpaceRun->m_box->width();
                    trailingSpaceRun->m_box->setWidth(0);
                }
                break;
            }
            // fall through
        case TAAUTO:
            numSpaces = 0;
            // for right to left fall through to right aligned
            if (style()->direction() == LTR) {
                if (totWidth > availableWidth && trailingSpaceRun)
                    trailingSpaceRun->m_box->setWidth(max(0, trailingSpaceRun->m_box->width() - totWidth + availableWidth));
                break;
            }
        case RIGHT:
        case WEBKIT_RIGHT:
            // Wide lines spill out of the block based off direction.
            // So even if text-align is right, if direction is LTR, wide lines should overflow out of the right
            // side of the block.
            if (style()->direction() == LTR) {
                if (trailingSpaceRun) {
                    totWidth -= trailingSpaceRun->m_box->width();
                    trailingSpaceRun->m_box->setWidth(0);
                }
                if (totWidth < availableWidth)
                    x += availableWidth - totWidth;
            } else {
                if (totWidth > availableWidth && trailingSpaceRun) {
                    trailingSpaceRun->m_box->setWidth(max(0, trailingSpaceRun->m_box->width() - totWidth + availableWidth));
                    totWidth -= trailingSpaceRun->m_box->width();
                } else
                    x += availableWidth - totWidth;
            }
            break;
        case CENTER:
        case WEBKIT_CENTER:
            int trailingSpaceWidth = 0;
            if (trailingSpaceRun) {
                totWidth -= trailingSpaceRun->m_box->width();
                trailingSpaceWidth = min(trailingSpaceRun->m_box->width(), (availableWidth - totWidth + 1) / 2);
                trailingSpaceRun->m_box->setWidth(max(0, trailingSpaceWidth));
            }
            if (style()->direction() == LTR)
                x += max((availableWidth - totWidth) / 2, 0);
            else
                x += totWidth > availableWidth ? (availableWidth - totWidth) : (availableWidth - totWidth) / 2 - trailingSpaceWidth;
            break;
    }

    if (numSpaces) {
        for (BidiRun* r = firstRun; r; r = r->next()) {
            if (!r->m_box || r == trailingSpaceRun)
                continue;

            int spaceAdd = 0;
            if (r->m_object->isText()) {
                unsigned spaces = 0;
                const UChar* characters = toRenderText(r->m_object)->characters();
                for (int i = r->m_start; i < r->m_stop; i++) {
                    UChar c = characters[i];
                    if (c == ' ' || c == '\n' || c == '\t')
                        spaces++;
                }

                ASSERT(spaces <= numSpaces);

                // Only justify text if whitespace is collapsed.
                if (r->m_object->style()->collapseWhiteSpace()) {
                    spaceAdd = (availableWidth - totWidth) * spaces / numSpaces;
                    static_cast<InlineTextBox*>(r->m_box)->setSpaceAdd(spaceAdd);
                    totWidth += spaceAdd;
                }
                numSpaces -= spaces;
                if (!numSpaces)
                    break;
            }
        }
    }

    // The widths of all runs are now known.  We can now place every inline box (and
    // compute accurate widths for the inline flow boxes).
    needsWordSpacing = false;
    lineBox->placeBoxesHorizontally(x, needsWordSpacing);
}

void RenderBlock::computeVerticalPositionsForLine(RootInlineBox* lineBox, BidiRun* firstRun)
{
    setHeight(lineBox->verticallyAlignBoxes(height()));
    lineBox->setBlockHeight(height());

    // Now make sure we place replaced render objects correctly.
    for (BidiRun* r = firstRun; r; r = r->next()) {
        ASSERT(r->m_box);
        if (!r->m_box)
            continue; // Skip runs with no line boxes.

        // Align positioned boxes with the top of the line box.  This is
        // a reasonable approximation of an appropriate y position.
        if (r->m_object->isPositioned())
            r->m_box->setY(height());

        // Position is used to properly position both replaced elements and
        // to update the static normal flow x/y of positioned elements.
        if (r->m_object->isText())
            toRenderText(r->m_object)->positionLineBox(r->m_box);
        else if (r->m_object->isBox())
            toRenderBox(r->m_object)->positionLineBox(r->m_box);
    }
    // Positioned objects and zero-length text nodes destroy their boxes in
    // position(), which unnecessarily dirties the line.
    lineBox->markDirty(false);
}

// collects one line of the paragraph and transforms it to visual order
void RenderBlock::bidiReorderLine(InlineBidiResolver& resolver, const InlineIterator& end, bool previousLineBrokeCleanly)
{
    resolver.createBidiRunsForLine(end, style()->visuallyOrdered(), previousLineBrokeCleanly);
}

static inline bool isCollapsibleSpace(UChar character, RenderText* renderer)
{
    if (character == ' ' || character == '\t' || character == softHyphen)
        return true;
    if (character == '\n')
        return !renderer->style()->preserveNewline();
    if (character == noBreakSpace)
        return renderer->style()->nbspMode() == SPACE;
    return false;
}

void RenderBlock::layoutInlineChildren(bool relayoutChildren, int& repaintTop, int& repaintBottom)
{
    bool useRepaintBounds = false;
    
    m_overflow.clear();
        
    setHeight(borderTop() + paddingTop());
    int toAdd = borderBottom() + paddingBottom() + horizontalScrollbarHeight();

    // Figure out if we should clear out our line boxes.
    // FIXME: Handle resize eventually!
    bool fullLayout = !firstLineBox() || selfNeedsLayout() || relayoutChildren;
    if (fullLayout)
        lineBoxes()->deleteLineBoxes(renderArena());

    // Text truncation only kicks in if your overflow isn't visible and your text-overflow-mode isn't
    // clip.
    // FIXME: CSS3 says that descendants that are clipped must also know how to truncate.  This is insanely
    // difficult to figure out (especially in the middle of doing layout), and is really an esoteric pile of nonsense
    // anyway, so we won't worry about following the draft here.
    bool hasTextOverflow = style()->textOverflow() && hasOverflowClip();

    // Walk all the lines and delete our ellipsis line boxes if they exist.
    if (hasTextOverflow)
         deleteEllipsisLineBoxes();

    if (firstChild()) {
#ifdef ANDROID_LAYOUT
        // if we are in fitColumnToScreen mode
        // and the current object is not float:right in LTR or not float:left in RTL,
        // and text align is auto, or justify or left in LTR, or right in RTL, we
        // will wrap text around screen width so that it doesn't need to scroll
        // horizontally when reading a paragraph.
        // In case the line height is less than the font size, we skip
        // the text wrapping since this will cause text overlapping.
        // If a text has background image, we ignore text wrapping,
        // otherwise the background will be potentially messed up.
        const Settings* settings = document()->settings();
        bool doTextWrap = settings && settings->layoutAlgorithm() == Settings::kLayoutFitColumnToScreen;
        if (doTextWrap) {
            int ta = style()->textAlign();
            int dir = style()->direction();
            bool autowrap = style()->autoWrap();
            // if the RenderBlock is positioned, don't wrap text around screen
            // width as it may cause text to overlap.
            bool positioned = isPositioned();
            EFloat cssfloat = style()->floating();
            const int lineHeight = style()->computedLineHeight();
            const int fontSize = style()->fontSize();
            doTextWrap = autowrap && !positioned &&
                    (fontSize <= lineHeight) && !style()->hasBackgroundImage() &&
                    (((dir == LTR && cssfloat != FRIGHT) ||
                    (dir == RTL && cssfloat != FLEFT)) &&
                    ((ta == TAAUTO) || (ta == JUSTIFY) ||
                    ((ta == LEFT || ta == WEBKIT_LEFT) && (dir == LTR)) ||
                    ((ta == RIGHT || ta == WEBKIT_RIGHT) && (dir == RTL))));
        }
        bool hasTextToWrap = false;
#endif
        // layout replaced elements
        bool endOfInline = false;
        RenderObject* o = bidiFirst(this, 0, false);
        Vector<FloatWithRect> floats;
        bool hasInlineChild = false;
        while (o) {
            if (o->isReplaced() || o->isFloating() || o->isPositioned()) {
                RenderBox* box = toRenderBox(o);
                
                if (relayoutChildren || o->style()->width().isPercent() || o->style()->height().isPercent())
                    o->setChildNeedsLayout(true, false);
                    
                // If relayoutChildren is set and we have percentage padding, we also need to invalidate the child's pref widths.
                if (relayoutChildren && (o->style()->paddingLeft().isPercent() || o->style()->paddingRight().isPercent()))
                    o->setPrefWidthsDirty(true, false);
            
                if (o->isPositioned())
                    o->containingBlock()->insertPositionedObject(box);
                else {
#ifdef ANDROID_LAYOUT
                    // ignore text wrap for textField or menuList
                        if (doTextWrap && (o->isTextField() || o->isMenuList()))
                            doTextWrap = false;
#endif
                    if (o->isFloating())
                        floats.append(FloatWithRect(box));
                    else if (fullLayout || o->needsLayout()) // Replaced elements
                        toRenderBox(o)->dirtyLineBoxes(fullLayout);

                    o->layoutIfNeeded();
                }
            } else if (o->isText() || (o->isRenderInline() && !endOfInline)) {
                hasInlineChild = true;
                if (fullLayout || o->selfNeedsLayout())
                    dirtyLineBoxesForRenderer(o, fullLayout);
                o->setNeedsLayout(false);
#ifdef ANDROID_LAYOUT
                if (doTextWrap && !hasTextToWrap && o->isText()) {
                    Node* node = o->node();
                    // as it is very common for sites to use a serial of <a> or
                    // <li> as tabs, we don't force text to wrap if all the text
                    // are short and within an <a> or <li> tag, and only separated
                    // by short word like "|" or ";".
                    if (node && node->isTextNode() &&
                            !static_cast<Text*>(node)->containsOnlyWhitespace()) {
                        int length = static_cast<Text*>(node)->length();
                        // FIXME, need a magic number to decide it is too long to
                        // be a tab. Pick 25 for now as it covers around 160px
                        // (half of 320px) with the default font.
                        if (length > 25 || (length > 3 &&
                                (!node->parent()->hasTagName(HTMLNames::aTag) &&
                                !node->parent()->hasTagName(HTMLNames::liTag))))
                            hasTextToWrap = true;
                    }
                }
#endif
                if (!o->isText())
                    toRenderInline(o)->invalidateVerticalPosition(); // FIXME: Should do better here and not always invalidate everything.
            }
            o = bidiNext(this, o, 0, false, &endOfInline);
        }

#ifdef ANDROID_LAYOUT
        // try to make sure that inline text will not span wider than the
        // screen size unless the container has a fixed height,
        if (doTextWrap && hasTextToWrap) {
            // check all the nested containing blocks, unless it is table or
            // table-cell, to make sure there is no fixed height as it implies
            // fixed layout. If we constrain the text to fit screen, we may
            // cause text overlap with the block after.
            bool isConstrained = false;
            RenderObject* obj = this;
            while (obj) {
                if (obj->style()->height().isFixed() && (!obj->isTable() && !obj->isTableCell())) {
                    isConstrained = true;
                    break;
                }
                if (obj->isFloating() || obj->isPositioned()) {
                    // floating and absolute or fixed positioning are done out
                    // of normal flow. Don't need to worry about height any more.
                    break;
                }
                obj = obj->container();
            }
            if (!isConstrained) {
                int screenWidth = view()->frameView()->screenWidth();
                int padding = paddingLeft() + paddingRight();
                if (screenWidth > 0 && width() > (screenWidth + padding)) {
                    // limit the content width (width excluding padding) to be
                    // (screenWidth - 2 * ANDROID_FCTS_MARGIN_PADDING)
                    int maxWidth = screenWidth - 2 * ANDROID_FCTS_MARGIN_PADDING + padding;
                    setWidth(min(width(), maxWidth));
                    m_minPrefWidth = min(m_minPrefWidth, maxWidth);
                    m_maxPrefWidth = min(m_maxPrefWidth, maxWidth);

                    IntRect overflow = layoutOverflowRect();
                    if (overflow.width() > maxWidth) {
                        overflow.setWidth(maxWidth);
                        clearLayoutOverflow();
                        addLayoutOverflow(overflow);
                    }
                }
            }
        }
#endif
        // We want to skip ahead to the first dirty line
        InlineBidiResolver resolver;
        unsigned floatIndex;
        bool firstLine = true;
        bool previousLineBrokeCleanly = true;
        RootInlineBox* startLine = determineStartPosition(firstLine, fullLayout, previousLineBrokeCleanly, resolver, floats, floatIndex);

        if (fullLayout && hasInlineChild && !selfNeedsLayout()) {
            setNeedsLayout(true, false);  // Mark ourselves as needing a full layout. This way we'll repaint like
                                          // we're supposed to.
            RenderView* v = view();
            if (v && !v->doingFullRepaint() && hasLayer()) {
                // Because we waited until we were already inside layout to discover
                // that the block really needed a full layout, we missed our chance to repaint the layer
                // before layout started.  Luckily the layer has cached the repaint rect for its original
                // position and size, and so we can use that to make a repaint happen now.
                repaintUsingContainer(containerForRepaint(), layer()->repaintRect());
            }
        }

        FloatingObject* lastFloat = m_floatingObjects ? m_floatingObjects->last() : 0;

        LineMidpointState& lineMidpointState = resolver.midpointState();

        // We also find the first clean line and extract these lines.  We will add them back
        // if we determine that we're able to synchronize after handling all our dirty lines.
        InlineIterator cleanLineStart;
        BidiStatus cleanLineBidiStatus;
        int endLineYPos = 0;
        RootInlineBox* endLine = (fullLayout || !startLine) ? 
                                 0 : determineEndPosition(startLine, cleanLineStart, cleanLineBidiStatus, endLineYPos);

        if (startLine) {
            useRepaintBounds = true;
            repaintTop = height();
            repaintBottom = height();
            RenderArena* arena = renderArena();
            RootInlineBox* box = startLine;
            while (box) {
                repaintTop = min(repaintTop, box->topVisibleOverflow());
                repaintBottom = max(repaintBottom, box->bottomVisibleOverflow());
                RootInlineBox* next = box->nextRootBox();
                box->deleteLine(arena);
                box = next;
            }
        }

        InlineIterator end = resolver.position();

        if (!fullLayout && lastRootBox() && lastRootBox()->endsWithBreak()) {
            // If the last line before the start line ends with a line break that clear floats,
            // adjust the height accordingly.
            // A line break can be either the first or the last object on a line, depending on its direction.
            if (InlineBox* lastLeafChild = lastRootBox()->lastLeafChild()) {
                RenderObject* lastObject = lastLeafChild->renderer();
                if (!lastObject->isBR())
                    lastObject = lastRootBox()->firstLeafChild()->renderer();
                if (lastObject->isBR()) {
                    EClear clear = lastObject->style()->clear();
                    if (clear != CNONE)
                        newLine(clear);
                }
            }
        }

        bool endLineMatched = false;
        bool checkForEndLineMatch = endLine;
        bool checkForFloatsFromLastLine = false;
        int lastHeight = height();

        bool isLineEmpty = true;

        while (!end.atEnd()) {
            // FIXME: Is this check necessary before the first iteration or can it be moved to the end?
            if (checkForEndLineMatch && (endLineMatched = matchedEndLine(resolver, cleanLineStart, cleanLineBidiStatus, endLine, endLineYPos, repaintBottom, repaintTop)))
                break;

            lineMidpointState.reset();
            
            isLineEmpty = true;
            
            EClear clear = CNONE;
            end = findNextLineBreak(resolver, firstLine, isLineEmpty, previousLineBrokeCleanly, &clear);
            if (resolver.position().atEnd()) {
                resolver.deleteRuns();
                checkForFloatsFromLastLine = true;
                break;
            }
            ASSERT(end != resolver.position());

            if (!isLineEmpty) {
                bidiReorderLine(resolver, end, previousLineBrokeCleanly);
                ASSERT(resolver.position() == end);

                BidiRun* trailingSpaceRun = 0;
                if (!previousLineBrokeCleanly && resolver.runCount() && resolver.logicallyLastRun()->m_object->style()->breakOnlyAfterWhiteSpace()
                        && resolver.logicallyLastRun()->m_object->style()->autoWrap()) {
                    trailingSpaceRun = resolver.logicallyLastRun();
                    RenderObject* lastObject = trailingSpaceRun->m_object;
                    if (lastObject->isText()) {
                        RenderText* lastText = toRenderText(lastObject);
                        const UChar* characters = lastText->characters();
                        int firstSpace = trailingSpaceRun->stop();
                        while (firstSpace > trailingSpaceRun->start()) {
                            UChar current = characters[firstSpace - 1];
                            if (!isCollapsibleSpace(current, lastText))
                                break;
                            firstSpace--;
                        }
                        if (firstSpace == trailingSpaceRun->stop())
                            trailingSpaceRun = 0;
                        else {
                            TextDirection direction = style()->direction();
                            bool shouldReorder = trailingSpaceRun != (direction == LTR ? resolver.lastRun() : resolver.firstRun());
                            if (firstSpace != trailingSpaceRun->start()) {
                                BidiContext* baseContext = resolver.context();
                                while (BidiContext* parent = baseContext->parent())
                                    baseContext = parent;

                                BidiRun* newTrailingRun = new (renderArena()) BidiRun(firstSpace, trailingSpaceRun->m_stop, trailingSpaceRun->m_object, baseContext, OtherNeutral);
                                trailingSpaceRun->m_stop = firstSpace;
                                if (direction == LTR)
                                    resolver.addRun(newTrailingRun);
                                else
                                    resolver.prependRun(newTrailingRun);
                                trailingSpaceRun = newTrailingRun;
                                shouldReorder = false;
                            }
                            if (shouldReorder) {
                                if (direction == LTR) {
                                    resolver.moveRunToEnd(trailingSpaceRun);
                                    trailingSpaceRun->m_level = 0;
                                } else {
                                    resolver.moveRunToBeginning(trailingSpaceRun);
                                    trailingSpaceRun->m_level = 1;
                                }
                            }
                        }
                    } else
                        trailingSpaceRun = 0;
                }

                // Now that the runs have been ordered, we create the line boxes.
                // At the same time we figure out where border/padding/margin should be applied for
                // inline flow boxes.

                RootInlineBox* lineBox = 0;
                if (resolver.runCount()) {
                    lineBox = constructLine(resolver.runCount(), resolver.firstRun(), resolver.lastRun(), firstLine, !end.obj, end.obj && !end.pos ? end.obj : 0);
                    if (lineBox) {
                        lineBox->setEndsWithBreak(previousLineBrokeCleanly);

                        // Now we position all of our text runs horizontally.
                        computeHorizontalPositionsForLine(lineBox, firstLine, resolver.firstRun(), trailingSpaceRun, end.atEnd());

                        // Now position our text runs vertically.
                        computeVerticalPositionsForLine(lineBox, resolver.firstRun());

#if ENABLE(SVG)
                        // Special SVG text layout code
                        lineBox->computePerCharacterLayoutInformation();
#endif

#if PLATFORM(MAC)
                        // Highlight acts as an overflow inflation.
                        if (style()->highlight() != nullAtom)
                            lineBox->addHighlightOverflow();
#endif
                    }
                }

                resolver.deleteRuns();

                if (lineBox) {
                    lineBox->setLineBreakInfo(end.obj, end.pos, resolver.status());
                    if (useRepaintBounds) {
                        repaintTop = min(repaintTop, lineBox->topVisibleOverflow());
                        repaintBottom = max(repaintBottom, lineBox->bottomVisibleOverflow());
                    }
                }

                firstLine = false;
                newLine(clear);
            }

            if (m_floatingObjects && lastRootBox()) {
                if (lastFloat) {
                    for (FloatingObject* f = m_floatingObjects->last(); f != lastFloat; f = m_floatingObjects->prev()) {
                    }
                    m_floatingObjects->next();
                } else
                    m_floatingObjects->first();
                for (FloatingObject* f = m_floatingObjects->current(); f; f = m_floatingObjects->next()) {
                    if (f->m_bottom > lastHeight)
                        lastRootBox()->floats().append(f->m_renderer);
                    ASSERT(f->m_renderer == floats[floatIndex].object);
                    // If a float's geometry has changed, give up on syncing with clean lines.
                    if (floats[floatIndex].rect != IntRect(f->m_left, f->m_top, f->m_width, f->m_bottom - f->m_top))
                        checkForEndLineMatch = false;
                    floatIndex++;
                }
                lastFloat = m_floatingObjects->last();
            }

            lastHeight = height();
            lineMidpointState.reset();
            resolver.setPosition(end);
        }

        if (endLine) {
            if (endLineMatched) {
                // Attach all the remaining lines, and then adjust their y-positions as needed.
                int delta = height() - endLineYPos;
                for (RootInlineBox* line = endLine; line; line = line->nextRootBox()) {
                    line->attachLine();
                    if (delta) {
                        repaintTop = min(repaintTop, line->topVisibleOverflow() + min(delta, 0));
                        repaintBottom = max(repaintBottom, line->bottomVisibleOverflow() + max(delta, 0));
                        line->adjustPosition(0, delta);
                    }
                    if (Vector<RenderBox*>* cleanLineFloats = line->floatsPtr()) {
                        Vector<RenderBox*>::iterator end = cleanLineFloats->end();
                        for (Vector<RenderBox*>::iterator f = cleanLineFloats->begin(); f != end; ++f) {
                            int floatTop = (*f)->y() - (*f)->marginTop();
                            insertFloatingObject(*f);
                            setHeight(floatTop + delta);
                            positionNewFloats();
                        }
                    }
                }
                setHeight(lastRootBox()->blockHeight());
            } else {
                // Delete all the remaining lines.
                RootInlineBox* line = endLine;
                RenderArena* arena = renderArena();
                while (line) {
                    repaintTop = min(repaintTop, line->topVisibleOverflow());
                    repaintBottom = max(repaintBottom, line->bottomVisibleOverflow());
                    RootInlineBox* next = line->nextRootBox();
                    line->deleteLine(arena);
                    line = next;
                }
            }
        }
        if (m_floatingObjects && (checkForFloatsFromLastLine || positionNewFloats()) && lastRootBox()) {
            // In case we have a float on the last line, it might not be positioned up to now.
            // This has to be done before adding in the bottom border/padding, or the float will
            // include the padding incorrectly. -dwh
            if (checkForFloatsFromLastLine) {
                int bottomVisualOverflow = lastRootBox()->bottomVisualOverflow();
                int bottomLayoutOverflow = lastRootBox()->bottomLayoutOverflow();
                TrailingFloatsRootInlineBox* trailingFloatsLineBox = new (renderArena()) TrailingFloatsRootInlineBox(this);
                m_lineBoxes.appendLineBox(trailingFloatsLineBox);
                trailingFloatsLineBox->setConstructed();
                trailingFloatsLineBox->verticallyAlignBoxes(height());
                trailingFloatsLineBox->setVerticalOverflowPositions(height(), bottomLayoutOverflow, height(), bottomVisualOverflow, 0);
                trailingFloatsLineBox->setBlockHeight(height());
            }
            if (lastFloat) {
                for (FloatingObject* f = m_floatingObjects->last(); f != lastFloat; f = m_floatingObjects->prev()) {
                }
                m_floatingObjects->next();
            } else
                m_floatingObjects->first();
            for (FloatingObject* f = m_floatingObjects->current(); f; f = m_floatingObjects->next()) {
                if (f->m_bottom > lastHeight)
                    lastRootBox()->floats().append(f->m_renderer);
            }
            lastFloat = m_floatingObjects->last();
        }
        size_t floatCount = floats.size();
        // Floats that did not have layout did not repaint when we laid them out. They would have
        // painted by now if they had moved, but if they stayed at (0, 0), they still need to be
        // painted.
        for (size_t i = 0; i < floatCount; ++i) {
            if (!floats[i].everHadLayout) {
                RenderBox* f = floats[i].object;
                if (!f->x() && !f->y() && f->checkForRepaintDuringLayout())
                    f->repaint();
            }
        }
    }

    // Now add in the bottom border/padding.
    setHeight(height() + toAdd);

    if (!firstLineBox() && hasLineIfEmpty())
        setHeight(height() + lineHeight(true, true));

    // See if we have any lines that spill out of our block.  If we do, then we will possibly need to
    // truncate text.
    if (hasTextOverflow)
        checkLinesForTextOverflow();
}

RootInlineBox* RenderBlock::determineStartPosition(bool& firstLine, bool& fullLayout, bool& previousLineBrokeCleanly, 
                                                   InlineBidiResolver& resolver, Vector<FloatWithRect>& floats, unsigned& numCleanFloats)
{
    RootInlineBox* curr = 0;
    RootInlineBox* last = 0;

    bool dirtiedByFloat = false;
    if (!fullLayout) {
        size_t floatIndex = 0;
        for (curr = firstRootBox(); curr && !curr->isDirty(); curr = curr->nextRootBox()) {
            if (Vector<RenderBox*>* cleanLineFloats = curr->floatsPtr()) {
                Vector<RenderBox*>::iterator end = cleanLineFloats->end();
                for (Vector<RenderBox*>::iterator o = cleanLineFloats->begin(); o != end; ++o) {
                    RenderBox* f = *o;
                    IntSize newSize(f->width() + f->marginLeft() +f->marginRight(), f->height() + f->marginTop() + f->marginBottom());
                    ASSERT(floatIndex < floats.size());
                    if (floats[floatIndex].object != f) {
                        // A new float has been inserted before this line or before its last known float.
                        // Just do a full layout.
                        fullLayout = true;
                        break;
                    }
                    if (floats[floatIndex].rect.size() != newSize) {
                        int floatTop = floats[floatIndex].rect.y();
                        curr->markDirty();
                        markLinesDirtyInVerticalRange(curr->blockHeight(), floatTop + max(floats[floatIndex].rect.height(), newSize.height()), curr);
                        floats[floatIndex].rect.setSize(newSize);
                        dirtiedByFloat = true;
                    }
                    floatIndex++;
                }
            }
            if (dirtiedByFloat || fullLayout)
                break;
        }
        // Check if a new float has been inserted after the last known float.
        if (!curr && floatIndex < floats.size())
            fullLayout = true;
    }

    if (fullLayout) {
        // Nuke all our lines.
        if (firstRootBox()) {
            RenderArena* arena = renderArena();
            curr = firstRootBox(); 
            while (curr) {
                RootInlineBox* next = curr->nextRootBox();
                curr->deleteLine(arena);
                curr = next;
            }
            ASSERT(!firstLineBox() && !lastLineBox());
        }
    } else {
        if (curr) {
            // We have a dirty line.
            if (RootInlineBox* prevRootBox = curr->prevRootBox()) {
                // We have a previous line.
                if (!dirtiedByFloat && (!prevRootBox->endsWithBreak() || (prevRootBox->lineBreakObj()->isText() && prevRootBox->lineBreakPos() >= toRenderText(prevRootBox->lineBreakObj())->textLength())))
                    // The previous line didn't break cleanly or broke at a newline
                    // that has been deleted, so treat it as dirty too.
                    curr = prevRootBox;
            }
        } else {
            // No dirty lines were found.
            // If the last line didn't break cleanly, treat it as dirty.
            if (lastRootBox() && !lastRootBox()->endsWithBreak())
                curr = lastRootBox();
        }

        // If we have no dirty lines, then last is just the last root box.
        last = curr ? curr->prevRootBox() : lastRootBox();
    }

    numCleanFloats = 0;
    if (!floats.isEmpty()) {
        int savedHeight = height();
        // Restore floats from clean lines.
        RootInlineBox* line = firstRootBox();
        while (line != curr) {
            if (Vector<RenderBox*>* cleanLineFloats = line->floatsPtr()) {
                Vector<RenderBox*>::iterator end = cleanLineFloats->end();
                for (Vector<RenderBox*>::iterator f = cleanLineFloats->begin(); f != end; ++f) {
                    insertFloatingObject(*f);
                    setHeight((*f)->y() - (*f)->marginTop());
                    positionNewFloats();
                    ASSERT(floats[numCleanFloats].object == *f);
                    numCleanFloats++;
                }
            }
            line = line->nextRootBox();
        }
        setHeight(savedHeight);
    }

    firstLine = !last;
    previousLineBrokeCleanly = !last || last->endsWithBreak();

    RenderObject* startObj;
    int pos = 0;
    if (last) {
        setHeight(last->blockHeight());
        startObj = last->lineBreakObj();
        pos = last->lineBreakPos();
        resolver.setStatus(last->lineBreakBidiStatus());
    } else {
        bool ltr = style()->direction() == LTR
    #if ENABLE(SVG)   
            || (style()->unicodeBidi() == UBNormal && isSVGText())
    #endif
            ;

        Direction direction = ltr ? LeftToRight : RightToLeft;
        resolver.setLastStrongDir(direction);
        resolver.setLastDir(direction);
        resolver.setEorDir(direction);
        resolver.setContext(BidiContext::create(ltr ? 0 : 1, direction, style()->unicodeBidi() == Override));

        startObj = bidiFirst(this, &resolver);
    }

    resolver.setPosition(InlineIterator(this, startObj, pos));

    return curr;
}

RootInlineBox* RenderBlock::determineEndPosition(RootInlineBox* startLine, InlineIterator& cleanLineStart, BidiStatus& cleanLineBidiStatus, int& yPos)
{
    RootInlineBox* last = 0;
    if (!startLine)
        last = 0;
    else {
        for (RootInlineBox* curr = startLine->nextRootBox(); curr; curr = curr->nextRootBox()) {
            if (curr->isDirty())
                last = 0;
            else if (!last)
                last = curr;
        }
    }

    if (!last)
        return 0;

    RootInlineBox* prev = last->prevRootBox();
    cleanLineStart = InlineIterator(this, prev->lineBreakObj(), prev->lineBreakPos());
    cleanLineBidiStatus = prev->lineBreakBidiStatus();
    yPos = prev->blockHeight();

    for (RootInlineBox* line = last; line; line = line->nextRootBox())
        line->extractLine(); // Disconnect all line boxes from their render objects while preserving
                             // their connections to one another.

    return last;
}

bool RenderBlock::matchedEndLine(const InlineBidiResolver& resolver, const InlineIterator& endLineStart, const BidiStatus& endLineStatus, RootInlineBox*& endLine, int& endYPos, int& repaintBottom, int& repaintTop)
{
    if (resolver.position() == endLineStart) {
        if (resolver.status() != endLineStatus)
            return false;

        int delta = height() - endYPos;
        if (!delta || !m_floatingObjects)
            return true;

        // See if any floats end in the range along which we want to shift the lines vertically.
        int top = min(height(), endYPos);

        RootInlineBox* lastLine = endLine;
        while (RootInlineBox* nextLine = lastLine->nextRootBox())
            lastLine = nextLine;

        int bottom = lastLine->blockHeight() + abs(delta);

        for (FloatingObject* f = m_floatingObjects->first(); f; f = m_floatingObjects->next()) {
            if (f->m_bottom >= top && f->m_bottom < bottom)
                return false;
        }

        return true;
    }

    // The first clean line doesn't match, but we can check a handful of following lines to try
    // to match back up.
    static int numLines = 8; // The # of lines we're willing to match against.
    RootInlineBox* line = endLine;
    for (int i = 0; i < numLines && line; i++, line = line->nextRootBox()) {
        if (line->lineBreakObj() == resolver.position().obj && line->lineBreakPos() == resolver.position().pos) {
            // We have a match.
            if (line->lineBreakBidiStatus() != resolver.status())
                return false; // ...but the bidi state doesn't match.
            RootInlineBox* result = line->nextRootBox();

            // Set our yPos to be the block height of endLine.
            if (result)
                endYPos = line->blockHeight();

            int delta = height() - endYPos;
            if (delta && m_floatingObjects) {
                // See if any floats end in the range along which we want to shift the lines vertically.
                int top = min(height(), endYPos);

                RootInlineBox* lastLine = endLine;
                while (RootInlineBox* nextLine = lastLine->nextRootBox())
                    lastLine = nextLine;

                int bottom = lastLine->blockHeight() + abs(delta);

                for (FloatingObject* f = m_floatingObjects->first(); f; f = m_floatingObjects->next()) {
                    if (f->m_bottom >= top && f->m_bottom < bottom)
                        return false;
                }
            }

            // Now delete the lines that we failed to sync.
            RootInlineBox* boxToDelete = endLine;
            RenderArena* arena = renderArena();
            while (boxToDelete && boxToDelete != result) {
                repaintTop = min(repaintTop, boxToDelete->topVisibleOverflow());
                repaintBottom = max(repaintBottom, boxToDelete->bottomVisibleOverflow());
                RootInlineBox* next = boxToDelete->nextRootBox();
                boxToDelete->deleteLine(arena);
                boxToDelete = next;
            }

            endLine = result;
            return result;
        }
    }

    return false;
}

static inline bool skipNonBreakingSpace(const InlineIterator& it, bool isLineEmpty, bool previousLineBrokeCleanly)
{
    if (it.obj->style()->nbspMode() != SPACE || it.current() != noBreakSpace)
        return false;

    // FIXME: This is bad.  It makes nbsp inconsistent with space and won't work correctly
    // with m_minWidth/m_maxWidth.
    // Do not skip a non-breaking space if it is the first character
    // on a line after a clean line break (or on the first line, since previousLineBrokeCleanly starts off
    // |true|).
    if (isLineEmpty && previousLineBrokeCleanly)
        return false;

    return true;
}

static inline bool shouldCollapseWhiteSpace(const RenderStyle* style, bool isLineEmpty, bool previousLineBrokeCleanly)
{
    return style->collapseWhiteSpace() || (style->whiteSpace() == PRE_WRAP && (!isLineEmpty || !previousLineBrokeCleanly));
}

static inline bool shouldPreserveNewline(RenderObject* object)
{
#if ENABLE(SVG)
    if (object->isSVGText())
        return false;
#endif

    return object->style()->preserveNewline();
}

static bool inlineFlowRequiresLineBox(RenderInline* flow)
{
    // FIXME: Right now, we only allow line boxes for inlines that are truly empty.
    // We need to fix this, though, because at the very least, inlines containing only
    // ignorable whitespace should should also have line boxes. 
    return !flow->firstChild() && flow->hasHorizontalBordersPaddingOrMargin();
}

bool RenderBlock::requiresLineBox(const InlineIterator& it, bool isLineEmpty, bool previousLineBrokeCleanly)
{
    if (it.obj->isFloatingOrPositioned())
        return false;

    if (it.obj->isRenderInline() && !inlineFlowRequiresLineBox(toRenderInline(it.obj)))
        return false;

    if (!shouldCollapseWhiteSpace(it.obj->style(), isLineEmpty, previousLineBrokeCleanly) || it.obj->isBR())
        return true;

    UChar current = it.current();
    return current != ' ' && current != '\t' && current != softHyphen && (current != '\n' || shouldPreserveNewline(it.obj)) 
            && !skipNonBreakingSpace(it, isLineEmpty, previousLineBrokeCleanly);
}

bool RenderBlock::generatesLineBoxesForInlineChild(RenderObject* inlineObj, bool isLineEmpty, bool previousLineBrokeCleanly)
{
    ASSERT(inlineObj->parent() == this);

    InlineIterator it(this, inlineObj, 0);
    while (!it.atEnd() && !requiresLineBox(it, isLineEmpty, previousLineBrokeCleanly))
        it.increment();

    return !it.atEnd();
}

// FIXME: The entire concept of the skipTrailingWhitespace function is flawed, since we really need to be building
// line boxes even for containers that may ultimately collapse away.  Otherwise we'll never get positioned
// elements quite right.  In other words, we need to build this function's work into the normal line
// object iteration process.
// NB. this function will insert any floating elements that would otherwise
// be skipped but it will not position them.
void RenderBlock::skipTrailingWhitespace(InlineIterator& iterator, bool isLineEmpty, bool previousLineBrokeCleanly)
{
    while (!iterator.atEnd() && !requiresLineBox(iterator, isLineEmpty, previousLineBrokeCleanly)) {
        RenderObject* object = iterator.obj;
        if (object->isFloating()) {
            insertFloatingObject(toRenderBox(object));
        } else if (object->isPositioned()) {
            // FIXME: The math here is actually not really right.  It's a best-guess approximation that
            // will work for the common cases
            RenderObject* c = object->container();
            if (c->isRenderInline()) {
                // A relative positioned inline encloses us.  In this case, we also have to determine our
                // position as though we were an inline.  Set |staticX| and |staticY| on the relative positioned
                // inline so that we can obtain the value later.
                toRenderInline(c)->layer()->setStaticX(style()->direction() == LTR ? leftOffset(height(), false) : rightOffset(height(), false));
                toRenderInline(c)->layer()->setStaticY(height());
            }
    
            RenderBox* box = toRenderBox(object);
            if (box->style()->hasStaticX()) {
                if (box->style()->isOriginalDisplayInlineType())
                    box->layer()->setStaticX(style()->direction() == LTR ? leftOffset(height(), false) : width() - rightOffset(height(), false));
                else
                    box->layer()->setStaticX(style()->direction() == LTR ? borderLeft() + paddingLeft() : borderRight() + paddingRight());
            }
    
            if (box->style()->hasStaticY())
                box->layer()->setStaticY(height());
        }
        iterator.increment();
    }
}

int RenderBlock::skipLeadingWhitespace(InlineBidiResolver& resolver, bool firstLine, bool isLineEmpty, bool previousLineBrokeCleanly)
{
    int availableWidth = lineWidth(height(), firstLine);
    while (!resolver.position().atEnd() && !requiresLineBox(resolver.position(), isLineEmpty, previousLineBrokeCleanly)) {
        RenderObject* object = resolver.position().obj;
        if (object->isFloating()) {
            insertFloatingObject(toRenderBox(object));
            positionNewFloats();
            availableWidth = lineWidth(height(), firstLine);
        } else if (object->isPositioned()) {
            // FIXME: The math here is actually not really right.  It's a best-guess approximation that
            // will work for the common cases
            RenderObject* c = object->container();
            if (c->isRenderInline()) {
                // A relative positioned inline encloses us.  In this case, we also have to determine our
                // position as though we were an inline.  Set |staticX| and |staticY| on the relative positioned
                // inline so that we can obtain the value later.
                toRenderInline(c)->layer()->setStaticX(style()->direction() == LTR ? leftOffset(height(), firstLine) : rightOffset(height(), firstLine));
                toRenderInline(c)->layer()->setStaticY(height());
            }
    
            RenderBox* box = toRenderBox(object);
            if (box->style()->hasStaticX()) {
                if (box->style()->isOriginalDisplayInlineType())
                    box->layer()->setStaticX(style()->direction() == LTR ? leftOffset(height(), firstLine) : width() - rightOffset(height(), firstLine));
                else
                    box->layer()->setStaticX(style()->direction() == LTR ? borderLeft() + paddingLeft() : borderRight() + paddingRight());
            }
    
            if (box->style()->hasStaticY())
                box->layer()->setStaticY(height());
        }
        resolver.increment();
    }
    resolver.commitExplicitEmbedding();
    return availableWidth;
}

// This is currently just used for list markers and inline flows that have line boxes. Neither should 
// have an effect on whitespace at the start of the line. 
static bool shouldSkipWhitespaceAfterStartObject(RenderBlock* block, RenderObject* o, LineMidpointState& lineMidpointState)
{
    RenderObject* next = bidiNext(block, o);
    if (next && !next->isBR() && next->isText() && toRenderText(next)->textLength() > 0) {
        RenderText* nextText = toRenderText(next);
        UChar nextChar = nextText->characters()[0];
        if (nextText->style()->isCollapsibleWhiteSpace(nextChar)) {
            addMidpoint(lineMidpointState, InlineIterator(0, o, 0));
            return true;
        }
    }

    return false;
}

void RenderBlock::fitBelowFloats(int widthToFit, bool firstLine, int& availableWidth)
{
    ASSERT(widthToFit > availableWidth);

    int floatBottom;
    int lastFloatBottom = height();
    int newLineWidth = availableWidth;
    while (true) {
        floatBottom = nextFloatBottomBelow(lastFloatBottom);
        if (!floatBottom)
            break;

        newLineWidth = lineWidth(floatBottom, firstLine);
        lastFloatBottom = floatBottom;
        if (newLineWidth >= widthToFit)
            break;
    }

    if (newLineWidth > availableWidth) {
        setHeight(lastFloatBottom);
        availableWidth = newLineWidth;
    }
}

static inline unsigned textWidth(RenderText* text, unsigned from, unsigned len, const Font& font, int xPos, bool isFixedPitch, bool collapseWhiteSpace)
{
    if (isFixedPitch || (!from && len == text->textLength()))
        return text->width(from, len, font, xPos);
    return font.width(TextRun(text->characters() + from, len, !collapseWhiteSpace, xPos));
}

InlineIterator RenderBlock::findNextLineBreak(InlineBidiResolver& resolver, bool firstLine,  bool& isLineEmpty, bool& previousLineBrokeCleanly, 
                                              EClear* clear)
{
    ASSERT(resolver.position().block == this);

    bool appliedStartWidth = resolver.position().pos > 0;
    LineMidpointState& lineMidpointState = resolver.midpointState();
    
    int width = skipLeadingWhitespace(resolver, firstLine, isLineEmpty, previousLineBrokeCleanly);

    int w = 0;
    int tmpW = 0;

    if (resolver.position().atEnd())
        return resolver.position();

    // This variable is used only if whitespace isn't set to PRE, and it tells us whether
    // or not we are currently ignoring whitespace.
    bool ignoringSpaces = false;
    InlineIterator ignoreStart;
    
    // This variable tracks whether the very last character we saw was a space.  We use
    // this to detect when we encounter a second space so we know we have to terminate
    // a run.
    bool currentCharacterIsSpace = false;
    bool currentCharacterIsWS = false;
    RenderObject* trailingSpaceObject = 0;

    InlineIterator lBreak = resolver.position();

    RenderObject *o = resolver.position().obj;
    RenderObject *last = o;
    unsigned pos = resolver.position().pos;
    int nextBreakable = resolver.position().nextBreakablePosition;
    bool atStart = true;

    bool prevLineBrokeCleanly = previousLineBrokeCleanly;
    previousLineBrokeCleanly = false;

    bool autoWrapWasEverTrueOnLine = false;
    bool floatsFitOnLine = true;
    
    // Firefox and Opera will allow a table cell to grow to fit an image inside it under
    // very specific circumstances (in order to match common WinIE renderings). 
    // Not supporting the quirk has caused us to mis-render some real sites. (See Bugzilla 10517.) 
    bool allowImagesToBreak = !style()->htmlHacks() || !isTableCell() || !style()->width().isIntrinsicOrAuto();

    EWhiteSpace currWS = style()->whiteSpace();
    EWhiteSpace lastWS = currWS;
    while (o) {
        currWS = o->isReplaced() ? o->parent()->style()->whiteSpace() : o->style()->whiteSpace();
        lastWS = last->isReplaced() ? last->parent()->style()->whiteSpace() : last->style()->whiteSpace();
        
        bool autoWrap = RenderStyle::autoWrap(currWS);
        autoWrapWasEverTrueOnLine = autoWrapWasEverTrueOnLine || autoWrap;

#if ENABLE(SVG)
        bool preserveNewline = o->isSVGText() ? false : RenderStyle::preserveNewline(currWS);
#else
        bool preserveNewline = RenderStyle::preserveNewline(currWS);
#endif

        bool collapseWhiteSpace = RenderStyle::collapseWhiteSpace(currWS);
            
        if (o->isBR()) {
            if (w + tmpW <= width) {
                lBreak.obj = o;
                lBreak.pos = 0;
                lBreak.nextBreakablePosition = -1;
                lBreak.increment();

                // A <br> always breaks a line, so don't let the line be collapsed
                // away. Also, the space at the end of a line with a <br> does not
                // get collapsed away.  It only does this if the previous line broke
                // cleanly.  Otherwise the <br> has no effect on whether the line is
                // empty or not.
                if (prevLineBrokeCleanly)
                    isLineEmpty = false;
                trailingSpaceObject = 0;
                previousLineBrokeCleanly = true;

                if (!isLineEmpty && clear)
                    *clear = o->style()->clear();
            }
            goto end;
        }

        if (o->isFloatingOrPositioned()) {
            // add to special objects...
            if (o->isFloating()) {
                RenderBox* floatBox = toRenderBox(o);
                insertFloatingObject(floatBox);
                // check if it fits in the current line.
                // If it does, position it now, otherwise, position
                // it after moving to next line (in newLine() func)
                if (floatsFitOnLine && floatBox->width() + floatBox->marginLeft() + floatBox->marginRight() + w + tmpW <= width) {
                    positionNewFloats();
                    width = lineWidth(height(), firstLine);
                } else
                    floatsFitOnLine = false;
            } else if (o->isPositioned()) {
                // If our original display wasn't an inline type, then we can
                // go ahead and determine our static x position now.
                RenderBox* box = toRenderBox(o);
                bool isInlineType = box->style()->isOriginalDisplayInlineType();
                bool needToSetStaticX = box->style()->hasStaticX();
                if (box->style()->hasStaticX() && !isInlineType) {
                    box->layer()->setStaticX(o->parent()->style()->direction() == LTR ?
                                  borderLeft() + paddingLeft() :
                                  borderRight() + paddingRight());
                    needToSetStaticX = false;
                }

                // If our original display was an INLINE type, then we can go ahead
                // and determine our static y position now.
                bool needToSetStaticY = box->style()->hasStaticY();
                if (box->style()->hasStaticY() && isInlineType) {
                    box->layer()->setStaticY(height());
                    needToSetStaticY = false;
                }
                
                bool needToCreateLineBox = needToSetStaticX || needToSetStaticY;
                RenderObject* c = o->container();
                if (c->isRenderInline() && (!needToSetStaticX || !needToSetStaticY))
                    needToCreateLineBox = true;

                // If we're ignoring spaces, we have to stop and include this object and
                // then start ignoring spaces again.
                if (needToCreateLineBox) {
                    trailingSpaceObject = 0;
                    ignoreStart.obj = o;
                    ignoreStart.pos = 0;
                    if (ignoringSpaces) {
                        addMidpoint(lineMidpointState, ignoreStart); // Stop ignoring spaces.
                        addMidpoint(lineMidpointState, ignoreStart); // Start ignoring again.
                    }
                    
                }
            }
        } else if (o->isRenderInline()) {
            // Right now, we should only encounter empty inlines here.
            ASSERT(!o->firstChild());
    
            RenderInline* flowBox = toRenderInline(o);
            
            // Now that some inline flows have line boxes, if we are already ignoring spaces, we need 
            // to make sure that we stop to include this object and then start ignoring spaces again. 
            // If this object is at the start of the line, we need to behave like list markers and 
            // start ignoring spaces.
            if (inlineFlowRequiresLineBox(flowBox)) {
                isLineEmpty = false;
                if (ignoringSpaces) {
                    trailingSpaceObject = 0;
                    addMidpoint(lineMidpointState, InlineIterator(0, o, 0)); // Stop ignoring spaces.
                    addMidpoint(lineMidpointState, InlineIterator(0, o, 0)); // Start ignoring again.
                } else if (style()->collapseWhiteSpace() && resolver.position().obj == o
                    && shouldSkipWhitespaceAfterStartObject(this, o, lineMidpointState)) {
                    // Like with list markers, we start ignoring spaces to make sure that any 
                    // additional spaces we see will be discarded.
                    currentCharacterIsSpace = true;
                    currentCharacterIsWS = true;
                    ignoringSpaces = true;
                }
            }

            tmpW += flowBox->marginLeft() + flowBox->borderLeft() + flowBox->paddingLeft() +
                    flowBox->marginRight() + flowBox->borderRight() + flowBox->paddingRight();
        } else if (o->isReplaced()) {
            RenderBox* replacedBox = toRenderBox(o);

            // Break on replaced elements if either has normal white-space.
            if ((autoWrap || RenderStyle::autoWrap(lastWS)) && (!o->isImage() || allowImagesToBreak)) {
                w += tmpW;
                tmpW = 0;
                lBreak.obj = o;
                lBreak.pos = 0;
                lBreak.nextBreakablePosition = -1;
            }

            if (ignoringSpaces)
                addMidpoint(lineMidpointState, InlineIterator(0, o, 0));

            isLineEmpty = false;
            ignoringSpaces = false;
            currentCharacterIsSpace = false;
            currentCharacterIsWS = false;
            trailingSpaceObject = 0;

            // Optimize for a common case. If we can't find whitespace after the list
            // item, then this is all moot. -dwh
            if (o->isListMarker()) {
                if (style()->collapseWhiteSpace() && shouldSkipWhitespaceAfterStartObject(this, o, lineMidpointState)) {
                    // Like with inline flows, we start ignoring spaces to make sure that any 
                    // additional spaces we see will be discarded.
                    currentCharacterIsSpace = true;
                    currentCharacterIsWS = true;
                    ignoringSpaces = true;
                }
                if (toRenderListMarker(o)->isInside())
                    tmpW += replacedBox->width() + replacedBox->marginLeft() + replacedBox->marginRight() + inlineWidth(o);
            } else
                tmpW += replacedBox->width() + replacedBox->marginLeft() + replacedBox->marginRight() + inlineWidth(o);
        } else if (o->isText()) {
            if (!pos)
                appliedStartWidth = false;

            RenderText* t = toRenderText(o);

            int strlen = t->textLength();
            int len = strlen - pos;
            const UChar* str = t->characters();

            const Font& f = t->style(firstLine)->font();
            bool isFixedPitch = f.isFixedPitch();

            int lastSpace = pos;
            int wordSpacing = o->style()->wordSpacing();
            int lastSpaceWordSpacing = 0;

            // Non-zero only when kerning is enabled, in which case we measure words with their trailing
            // space, then subtract its width.
            int wordTrailingSpaceWidth = f.typesettingFeatures() & Kerning ? f.spaceWidth() + wordSpacing : 0;

            int wrapW = tmpW + inlineWidth(o, !appliedStartWidth, true);
            int charWidth = 0;
            bool breakNBSP = autoWrap && o->style()->nbspMode() == SPACE;
            // Auto-wrapping text should wrap in the middle of a word only if it could not wrap before the word,
            // which is only possible if the word is the first thing on the line, that is, if |w| is zero.
            bool breakWords = o->style()->breakWords() && ((autoWrap && !w) || currWS == PRE);
            bool midWordBreak = false;
            bool breakAll = o->style()->wordBreak() == BreakAllWordBreak && autoWrap;

            if (t->isWordBreak()) {
                w += tmpW;
                tmpW = 0;
                lBreak.obj = o;
                lBreak.pos = 0;
                lBreak.nextBreakablePosition = -1;
                ASSERT(!len);
            }

            while (len) {
                bool previousCharacterIsSpace = currentCharacterIsSpace;
                bool previousCharacterIsWS = currentCharacterIsWS;
                UChar c = str[pos];
                currentCharacterIsSpace = c == ' ' || c == '\t' || (!preserveNewline && (c == '\n'));

                if (!collapseWhiteSpace || !currentCharacterIsSpace)
                    isLineEmpty = false;
                
                // Check for soft hyphens.  Go ahead and ignore them.
                if (c == softHyphen) {
                    if (!ignoringSpaces) {
                        // Ignore soft hyphens
                        InlineIterator beforeSoftHyphen;
                        if (pos)
                            beforeSoftHyphen = InlineIterator(0, o, pos - 1);
                        else
                            beforeSoftHyphen = InlineIterator(0, last, last->isText() ? toRenderText(last)->textLength() - 1 : 0);
                        // Two consecutive soft hyphens. Avoid overlapping midpoints.
                        if (lineMidpointState.numMidpoints && lineMidpointState.midpoints[lineMidpointState.numMidpoints - 1].obj == o && 
                            lineMidpointState.midpoints[lineMidpointState.numMidpoints - 1].pos == pos)
                            lineMidpointState.numMidpoints--;
                        else
                            addMidpoint(lineMidpointState, beforeSoftHyphen);

                        // Add the width up to but not including the hyphen.
                        tmpW += textWidth(t, lastSpace, pos - lastSpace, f, w + tmpW, isFixedPitch, collapseWhiteSpace) + lastSpaceWordSpacing;

                        // For wrapping text only, include the hyphen.  We need to ensure it will fit
                        // on the line if it shows when we break.
                        if (autoWrap)
                            tmpW += textWidth(t, pos, 1, f, w + tmpW, isFixedPitch, collapseWhiteSpace);

                        InlineIterator afterSoftHyphen(0, o, pos);
                        afterSoftHyphen.increment();
                        addMidpoint(lineMidpointState, afterSoftHyphen);
                    }

                    pos++;
                    len--;
                    lastSpaceWordSpacing = 0;
                    lastSpace = pos; // Cheesy hack to prevent adding in widths of the run twice.
                    continue;
                }
                
                bool applyWordSpacing = false;
                
                currentCharacterIsWS = currentCharacterIsSpace || (breakNBSP && c == noBreakSpace);

                if ((breakAll || breakWords) && !midWordBreak) {
                    wrapW += charWidth;
                    charWidth = textWidth(t, pos, 1, f, w + wrapW, isFixedPitch, collapseWhiteSpace);
                    midWordBreak = w + wrapW + charWidth > width;
                }

                bool betweenWords = c == '\n' || (currWS != PRE && !atStart && isBreakable(str, pos, strlen, nextBreakable, breakNBSP));
    
                if (betweenWords || midWordBreak) {
                    bool stoppedIgnoringSpaces = false;
                    if (ignoringSpaces) {
                        if (!currentCharacterIsSpace) {
                            // Stop ignoring spaces and begin at this
                            // new point.
                            ignoringSpaces = false;
                            lastSpaceWordSpacing = 0;
                            lastSpace = pos; // e.g., "Foo    goo", don't add in any of the ignored spaces.
                            addMidpoint(lineMidpointState, InlineIterator(0, o, pos));
                            stoppedIgnoringSpaces = true;
                        } else {
                            // Just keep ignoring these spaces.
                            pos++;
                            len--;
                            continue;
                        }
                    }

                    int additionalTmpW;
                    if (wordTrailingSpaceWidth && currentCharacterIsSpace)
                        additionalTmpW = textWidth(t, lastSpace, pos + 1 - lastSpace, f, w + tmpW, isFixedPitch, collapseWhiteSpace) - wordTrailingSpaceWidth + lastSpaceWordSpacing;
                    else
                        additionalTmpW = textWidth(t, lastSpace, pos - lastSpace, f, w + tmpW, isFixedPitch, collapseWhiteSpace) + lastSpaceWordSpacing;
                    tmpW += additionalTmpW;
                    if (!appliedStartWidth) {
                        tmpW += inlineWidth(o, true, false);
                        appliedStartWidth = true;
                    }
                    
                    applyWordSpacing =  wordSpacing && currentCharacterIsSpace && !previousCharacterIsSpace;

                    if (!w && autoWrap && tmpW > width)
                        fitBelowFloats(tmpW, firstLine, width);

                    if (autoWrap || breakWords) {
                        // If we break only after white-space, consider the current character
                        // as candidate width for this line.
                        bool lineWasTooWide = false;
                        if (w + tmpW <= width && currentCharacterIsWS && o->style()->breakOnlyAfterWhiteSpace() && !midWordBreak) {
                            int charWidth = textWidth(t, pos, 1, f, w + tmpW, isFixedPitch, collapseWhiteSpace) + (applyWordSpacing ? wordSpacing : 0);
                            // Check if line is too big even without the extra space
                            // at the end of the line. If it is not, do nothing. 
                            // If the line needs the extra whitespace to be too long, 
                            // then move the line break to the space and skip all 
                            // additional whitespace.
                            if (w + tmpW + charWidth > width) {
                                lineWasTooWide = true;
                                lBreak.obj = o;
                                lBreak.pos = pos;
                                lBreak.nextBreakablePosition = nextBreakable;
                                skipTrailingWhitespace(lBreak, isLineEmpty, previousLineBrokeCleanly);
                            }
                        }
                        if (lineWasTooWide || w + tmpW > width) {
                            if (lBreak.obj && shouldPreserveNewline(lBreak.obj) && lBreak.obj->isText() && toRenderText(lBreak.obj)->textLength() && !toRenderText(lBreak.obj)->isWordBreak() && toRenderText(lBreak.obj)->characters()[lBreak.pos] == '\n') {
                                if (!stoppedIgnoringSpaces && pos > 0) {
                                    // We need to stop right before the newline and then start up again.
                                    addMidpoint(lineMidpointState, InlineIterator(0, o, pos - 1)); // Stop
                                    addMidpoint(lineMidpointState, InlineIterator(0, o, pos)); // Start
                                }
                                lBreak.increment();
                                previousLineBrokeCleanly = true;
                            }
                            goto end; // Didn't fit. Jump to the end.
                        } else {
                            if (!betweenWords || (midWordBreak && !autoWrap))
                                tmpW -= additionalTmpW;
                            if (pos > 0 && str[pos-1] == softHyphen)
                                // Subtract the width of the soft hyphen out since we fit on a line.
                                tmpW -= textWidth(t, pos - 1, 1, f, w + tmpW, isFixedPitch, collapseWhiteSpace);
                        }
                    }

                    if (c == '\n' && preserveNewline) {
                        if (!stoppedIgnoringSpaces && pos > 0) {
                            // We need to stop right before the newline and then start up again.
                            addMidpoint(lineMidpointState, InlineIterator(0, o, pos - 1)); // Stop
                            addMidpoint(lineMidpointState, InlineIterator(0, o, pos)); // Start
                        }
                        lBreak.obj = o;
                        lBreak.pos = pos;
                        lBreak.nextBreakablePosition = nextBreakable;
                        lBreak.increment();
                        previousLineBrokeCleanly = true;
                        return lBreak;
                    }

                    if (autoWrap && betweenWords) {
                        w += tmpW;
                        wrapW = 0;
                        tmpW = 0;
                        lBreak.obj = o;
                        lBreak.pos = pos;
                        lBreak.nextBreakablePosition = nextBreakable;
                        // Auto-wrapping text should not wrap in the middle of a word once it has had an
                        // opportunity to break after a word.
                        breakWords = false;
                    }
                    
                    if (midWordBreak) {
                        // Remember this as a breakable position in case
                        // adding the end width forces a break.
                        lBreak.obj = o;
                        lBreak.pos = pos;
                        lBreak.nextBreakablePosition = nextBreakable;
                        midWordBreak &= (breakWords || breakAll);
                    }

                    if (betweenWords) {
                        lastSpaceWordSpacing = applyWordSpacing ? wordSpacing : 0;
                        lastSpace = pos;
                    }
                    
                    if (!ignoringSpaces && o->style()->collapseWhiteSpace()) {
                        // If we encounter a newline, or if we encounter a
                        // second space, we need to go ahead and break up this
                        // run and enter a mode where we start collapsing spaces.
                        if (currentCharacterIsSpace && previousCharacterIsSpace) {
                            ignoringSpaces = true;
                            
                            // We just entered a mode where we are ignoring
                            // spaces. Create a midpoint to terminate the run
                            // before the second space. 
                            addMidpoint(lineMidpointState, ignoreStart);
                        }
                    }
                } else if (ignoringSpaces) {
                    // Stop ignoring spaces and begin at this
                    // new point.
                    ignoringSpaces = false;
                    lastSpaceWordSpacing = applyWordSpacing ? wordSpacing : 0;
                    lastSpace = pos; // e.g., "Foo    goo", don't add in any of the ignored spaces.
                    addMidpoint(lineMidpointState, InlineIterator(0, o, pos));
                }

                if (currentCharacterIsSpace && !previousCharacterIsSpace) {
                    ignoreStart.obj = o;
                    ignoreStart.pos = pos;
                }

                if (!currentCharacterIsWS && previousCharacterIsWS) {
                    if (autoWrap && o->style()->breakOnlyAfterWhiteSpace()) {
                        lBreak.obj = o;
                        lBreak.pos = pos;
                        lBreak.nextBreakablePosition = nextBreakable;
                    }
                }
                
                if (collapseWhiteSpace && currentCharacterIsSpace && !ignoringSpaces)
                    trailingSpaceObject = o;
                else if (!o->style()->collapseWhiteSpace() || !currentCharacterIsSpace)
                    trailingSpaceObject = 0;
                    
                pos++;
                len--;
                atStart = false;
            }

            // IMPORTANT: pos is > length here!
            if (!ignoringSpaces)
                tmpW += textWidth(t, lastSpace, pos - lastSpace, f, w + tmpW, isFixedPitch, collapseWhiteSpace) + lastSpaceWordSpacing;
            tmpW += inlineWidth(o, !appliedStartWidth, true);
        } else
            ASSERT_NOT_REACHED();

        RenderObject* next = bidiNext(this, o);
        bool checkForBreak = autoWrap;
        if (w && w + tmpW > width && lBreak.obj && currWS == NOWRAP)
            checkForBreak = true;
        else if (next && o->isText() && next->isText() && !next->isBR()) {
            if (autoWrap || (next->style()->autoWrap())) {
                if (currentCharacterIsSpace)
                    checkForBreak = true;
                else {
                    checkForBreak = false;
                    RenderText* nextText = toRenderText(next);
                    if (nextText->textLength()) {
                        UChar c = nextText->characters()[0];
                        if (c == ' ' || c == '\t' || (c == '\n' && !shouldPreserveNewline(next)))
                            // If the next item on the line is text, and if we did not end with
                            // a space, then the next text run continues our word (and so it needs to
                            // keep adding to |tmpW|.  Just update and continue.
                            checkForBreak = true;
                    } else if (nextText->isWordBreak())
                        checkForBreak = true;
                    bool willFitOnLine = w + tmpW <= width;
                    if (!willFitOnLine && !w) {
                        fitBelowFloats(tmpW, firstLine, width);
                        willFitOnLine = tmpW <= width;
                    }
                    bool canPlaceOnLine = willFitOnLine || !autoWrapWasEverTrueOnLine;
                    if (canPlaceOnLine && checkForBreak) {
                        w += tmpW;
                        tmpW = 0;
                        lBreak.obj = next;
                        lBreak.pos = 0;
                        lBreak.nextBreakablePosition = -1;
                    }
                }
            }
        }

        if (checkForBreak && (w + tmpW > width)) {
            // if we have floats, try to get below them.
            if (currentCharacterIsSpace && !ignoringSpaces && o->style()->collapseWhiteSpace())
                trailingSpaceObject = 0;

            if (w)
                goto end;

            fitBelowFloats(tmpW, firstLine, width);

            // |width| may have been adjusted because we got shoved down past a float (thus
            // giving us more room), so we need to retest, and only jump to
            // the end label if we still don't fit on the line. -dwh
            if (w + tmpW > width)
                goto end;
        }

        if (!o->isFloatingOrPositioned()) {
            last = o;
            if (last->isReplaced() && autoWrap && (!last->isImage() || allowImagesToBreak) && (!last->isListMarker() || toRenderListMarker(last)->isInside())) {
                w += tmpW;
                tmpW = 0;
                lBreak.obj = next;
                lBreak.pos = 0;
                lBreak.nextBreakablePosition = -1;
            }
        }

        o = next;
        nextBreakable = -1;

        // Clear out our character space bool, since inline <pre>s don't collapse whitespace
        // with adjacent inline normal/nowrap spans.
        if (!collapseWhiteSpace)
            currentCharacterIsSpace = false;
        
        pos = 0;
        atStart = false;
    }

    
    if (w + tmpW <= width || lastWS == NOWRAP) {
        lBreak.obj = 0;
        lBreak.pos = 0;
        lBreak.nextBreakablePosition = -1;
    }

 end:
    if (lBreak == resolver.position() && (!lBreak.obj || !lBreak.obj->isBR())) {
        // we just add as much as possible
        if (style()->whiteSpace() == PRE) {
            // FIXME: Don't really understand this case.
            if (pos != 0) {
                lBreak.obj = o;
                lBreak.pos = pos - 1;
            } else {
                lBreak.obj = last;
                lBreak.pos = last->isText() ? last->length() : 0;
                lBreak.nextBreakablePosition = -1;
            }
        } else if (lBreak.obj) {
            // Don't ever break in the middle of a word if we can help it.
            // There's no room at all. We just have to be on this line,
            // even though we'll spill out.
            lBreak.obj = o;
            lBreak.pos = pos;
            lBreak.nextBreakablePosition = -1;
        }
    }

    // make sure we consume at least one char/object.
    if (lBreak == resolver.position())
        lBreak.increment();

    // Sanity check our midpoints.
    checkMidpoints(lineMidpointState, lBreak);
        
    if (trailingSpaceObject) {
        // This object is either going to be part of the last midpoint, or it is going
        // to be the actual endpoint.  In both cases we just decrease our pos by 1 level to
        // exclude the space, allowing it to - in effect - collapse into the newline.
        if (lineMidpointState.numMidpoints % 2) {
            InlineIterator* midpoints = lineMidpointState.midpoints.data();
            midpoints[lineMidpointState.numMidpoints - 1].pos--;
        }
        //else if (lBreak.pos > 0)
        //    lBreak.pos--;
        else if (lBreak.obj == 0 && trailingSpaceObject->isText()) {
            // Add a new end midpoint that stops right at the very end.
            RenderText* text = toRenderText(trailingSpaceObject);
            unsigned length = text->textLength();
            unsigned pos = length >= 2 ? length - 2 : UINT_MAX;
            InlineIterator endMid(0, trailingSpaceObject, pos);
            addMidpoint(lineMidpointState, endMid);
        }
    }

    // We might have made lBreak an iterator that points past the end
    // of the object. Do this adjustment to make it point to the start
    // of the next object instead to avoid confusing the rest of the
    // code.
    if (lBreak.pos > 0) {
        lBreak.pos--;
        lBreak.increment();
    }

    if (lBreak.obj && lBreak.pos >= 2 && lBreak.obj->isText()) {
        // For soft hyphens on line breaks, we have to chop out the midpoints that made us
        // ignore the hyphen so that it will render at the end of the line.
        UChar c = toRenderText(lBreak.obj)->characters()[lBreak.pos - 1];
        if (c == softHyphen)
            chopMidpointsAt(lineMidpointState, lBreak.obj, lBreak.pos - 2);
    }
    
    return lBreak;
}

void RenderBlock::addOverflowFromInlineChildren()
{
    for (RootInlineBox* curr = firstRootBox(); curr; curr = curr->nextRootBox()) {
        addLayoutOverflow(curr->layoutOverflowRect());
        if (!hasOverflowClip())
            addVisualOverflow(curr->visualOverflowRect());
    }
}

void RenderBlock::deleteEllipsisLineBoxes()
{
    for (RootInlineBox* curr = firstRootBox(); curr; curr = curr->nextRootBox())
        curr->clearTruncation();
}

void RenderBlock::checkLinesForTextOverflow()
{
    // Determine the width of the ellipsis using the current font.
    // FIXME: CSS3 says this is configurable, also need to use 0x002E (FULL STOP) if horizontal ellipsis is "not renderable"
    TextRun ellipsisRun(&horizontalEllipsis, 1);
    DEFINE_STATIC_LOCAL(AtomicString, ellipsisStr, (&horizontalEllipsis, 1));
    const Font& firstLineFont = firstLineStyle()->font();
    const Font& font = style()->font();
    int firstLineEllipsisWidth = firstLineFont.width(ellipsisRun);
    int ellipsisWidth = (font == firstLineFont) ? firstLineEllipsisWidth : font.width(ellipsisRun);

    // For LTR text truncation, we want to get the right edge of our padding box, and then we want to see
    // if the right edge of a line box exceeds that.  For RTL, we use the left edge of the padding box and
    // check the left edge of the line box to see if it is less
    // Include the scrollbar for overflow blocks, which means we want to use "contentWidth()"
    bool ltr = style()->direction() == LTR;
    for (RootInlineBox* curr = firstRootBox(); curr; curr = curr->nextRootBox()) {
        int blockRightEdge = rightOffset(curr->y(), curr == firstRootBox());
        int blockLeftEdge = leftOffset(curr->y(), curr == firstRootBox());
        int lineBoxEdge = ltr ? curr->x() + curr->width() : curr->x();
        if ((ltr && lineBoxEdge > blockRightEdge) || (!ltr && lineBoxEdge < blockLeftEdge)) {
            // This line spills out of our box in the appropriate direction.  Now we need to see if the line
            // can be truncated.  In order for truncation to be possible, the line must have sufficient space to
            // accommodate our truncation string, and no replaced elements (images, tables) can overlap the ellipsis
            // space.
            int width = curr == firstRootBox() ? firstLineEllipsisWidth : ellipsisWidth;
            int blockEdge = ltr ? blockRightEdge : blockLeftEdge;
            if (curr->canAccommodateEllipsis(ltr, blockEdge, lineBoxEdge, width))
                curr->placeEllipsis(ellipsisStr, ltr, blockLeftEdge, blockRightEdge, width);
        }
    }
}

}
