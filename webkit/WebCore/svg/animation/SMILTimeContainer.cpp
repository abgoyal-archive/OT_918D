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
 * Copyright (C) 2008 Apple Inc. All Rights Reserved.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

#include "config.h"
#include "SMILTimeContainer.h"

#if ENABLE(SVG)

#include "CSSComputedStyleDeclaration.h"
#include "CSSParser.h"
#include "Document.h"
#include "SVGAnimationElement.h"
#include "SVGSMILElement.h"
#include "SVGSVGElement.h"
#include <wtf/CurrentTime.h>

using namespace std;

namespace WebCore {
    
static const double animationFrameDelay = 0.025;

SMILTimeContainer::SMILTimeContainer(SVGSVGElement* owner) 
    : m_beginTime(0)
    , m_pauseTime(0)
    , m_accumulatedPauseTime(0)
    , m_nextManualSampleTime(0)
    , m_documentOrderIndexesDirty(false)
    , m_timer(this, &SMILTimeContainer::timerFired)
    , m_ownerSVGElement(owner)
{
}
    
#if !ENABLE(SVG_ANIMATION)
void SMILTimeContainer::begin() {}
void SMILTimeContainer::pause() {}
void SMILTimeContainer::resume() {}
SMILTime SMILTimeContainer::elapsed() const { return 0; }
bool SMILTimeContainer::isPaused() const { return false; }
void SMILTimeContainer::timerFired(Timer<SMILTimeContainer>*) {}
#else
    
void SMILTimeContainer::schedule(SVGSMILElement* animation)
{
    ASSERT(animation->timeContainer() == this);
    SMILTime nextFireTime = animation->nextProgressTime();
    if (!nextFireTime.isFinite())
        return;
    m_scheduledAnimations.add(animation);
    startTimer(0);
}
    
void SMILTimeContainer::unschedule(SVGSMILElement* animation)
{
    ASSERT(animation->timeContainer() == this);

    m_scheduledAnimations.remove(animation);
}

SMILTime SMILTimeContainer::elapsed() const
{
    if (!m_beginTime)
        return 0;
    return currentTime() - m_beginTime - m_accumulatedPauseTime;
}
    
bool SMILTimeContainer::isActive() const
{
    return m_beginTime && !isPaused();
}
    
bool SMILTimeContainer::isPaused() const
{
    return m_pauseTime;
}

void SMILTimeContainer::begin()
{
    ASSERT(!m_beginTime);
    m_beginTime = currentTime();
    updateAnimations(0);
}

void SMILTimeContainer::pause()
{
    if (!m_beginTime)
        return;
    ASSERT(!isPaused());
    m_pauseTime = currentTime();
    m_timer.stop();
}

void SMILTimeContainer::resume()
{
    if (!m_beginTime)
        return;
    ASSERT(isPaused());
    m_accumulatedPauseTime += currentTime() - m_pauseTime;
    m_pauseTime = 0;
    startTimer(0);
}

void SMILTimeContainer::startTimer(SMILTime fireTime, SMILTime minimumDelay)
{
    if (!m_beginTime || isPaused())
        return;
    
    if (!fireTime.isFinite())
        return;
    
    SMILTime delay = max(fireTime - elapsed(), minimumDelay);
    m_timer.startOneShot(delay.value());
}
    
void SMILTimeContainer::timerFired(Timer<SMILTimeContainer>*)
{
    ASSERT(m_beginTime);
    ASSERT(!m_pauseTime);
    SMILTime elapsed = this->elapsed();
    updateAnimations(elapsed);
}
 
void SMILTimeContainer::updateDocumentOrderIndexes()
{
    unsigned timingElementCount = 0;
    for (Node* node = m_ownerSVGElement; node; node = node->traverseNextNode(m_ownerSVGElement)) {
        if (SVGSMILElement::isSMILElement(node))
            static_cast<SVGSMILElement*>(node)->setDocumentOrderIndex(timingElementCount++);
    }
    m_documentOrderIndexesDirty = false;
}

struct PriorityCompare {
    PriorityCompare(SMILTime elapsed) : m_elapsed(elapsed) {}
    bool operator()(SVGSMILElement* a, SVGSMILElement* b)
    {
        // FIXME: This should also consider possible timing relations between the elements.
        SMILTime aBegin = a->intervalBegin();
        SMILTime bBegin = b->intervalBegin();
        // Frozen elements need to be prioritized based on their previous interval.
        aBegin = a->isFrozen() && m_elapsed < aBegin ? a->previousIntervalBegin() : aBegin;
        bBegin = b->isFrozen() && m_elapsed < bBegin ? b->previousIntervalBegin() : bBegin;
        if (aBegin == bBegin)
            return a->documentOrderIndex() < b->documentOrderIndex();
        return aBegin < bBegin;
    }
    SMILTime m_elapsed;
};

void SMILTimeContainer::sortByPriority(Vector<SVGSMILElement*>& smilElements, SMILTime elapsed)
{
    if (m_documentOrderIndexesDirty)
        updateDocumentOrderIndexes();
    std::sort(smilElements.begin(), smilElements.end(), PriorityCompare(elapsed));
}
    
static bool applyOrderSortFunction(SVGSMILElement* a, SVGSMILElement* b)
{
    if (!a->hasTagName(SVGNames::animateTransformTag) && b->hasTagName(SVGNames::animateTransformTag))
        return true;
    return false;
}
    
static void sortByApplyOrder(Vector<SVGSMILElement*>& smilElements)
{
    std::sort(smilElements.begin(), smilElements.end(), applyOrderSortFunction);
}

String SMILTimeContainer::baseValueFor(ElementAttributePair key)
{
    // FIXME: We wouldn't need to do this if we were keeping base values around properly in DOM.
    // Currently animation overwrites them so we need to save them somewhere.
    BaseValueMap::iterator it = m_savedBaseValues.find(key);
    if (it != m_savedBaseValues.end())
        return it->second;
    
    SVGElement* target = key.first;
    String attributeName = key.second;
    ASSERT(target);
    ASSERT(!attributeName.isEmpty());
    String baseValue;
    if (SVGAnimationElement::attributeIsCSS(attributeName))
        baseValue = computedStyle(target)->getPropertyValue(cssPropertyID(attributeName));
    else
        baseValue = target->getAttribute(attributeName);
    m_savedBaseValues.add(key, baseValue);
    return baseValue;
}

void SMILTimeContainer::sampleAnimationAtTime(const String& elementId, double newTime)
{
    ASSERT(m_beginTime);
    ASSERT(!isPaused());

    // Fast-forward to the time DRT wants to sample
    m_timer.stop();
    m_nextSamplingTarget = elementId;
    m_nextManualSampleTime = newTime;

    updateAnimations(elapsed());
}

void SMILTimeContainer::updateAnimations(SMILTime elapsed)
{
    SMILTime earliersFireTime = SMILTime::unresolved();

    Vector<SVGSMILElement*> toAnimate;
    copyToVector(m_scheduledAnimations, toAnimate);

    if (m_nextManualSampleTime) {
        SMILTime samplingDiff;
        for (unsigned n = 0; n < toAnimate.size(); ++n) {
            SVGSMILElement* animation = toAnimate[n];
            ASSERT(animation->timeContainer() == this);

            SVGElement* targetElement = animation->targetElement();
            if (!targetElement || targetElement->getIDAttribute() != m_nextSamplingTarget)
                continue;

            samplingDiff = animation->intervalBegin();
            break;
        }

        elapsed = SMILTime(m_nextManualSampleTime) + samplingDiff;
        m_nextManualSampleTime = 0;
    }

    // Sort according to priority. Elements with later begin time have higher priority.
    // In case of a tie, document order decides. 
    // FIXME: This should also consider timing relationships between the elements. Dependents
    // have higher priority.
    sortByPriority(toAnimate, elapsed);
    
    // Calculate animation contributions.
    typedef HashMap<ElementAttributePair, SVGSMILElement*> ResultElementMap;
    ResultElementMap resultsElements;
    for (unsigned n = 0; n < toAnimate.size(); ++n) {
        SVGSMILElement* animation = toAnimate[n];
        ASSERT(animation->timeContainer() == this);

        SVGElement* targetElement = animation->targetElement();
        if (!targetElement)
            continue;
        String attributeName = animation->attributeName();
        if (attributeName.isEmpty()) {
            if (animation->hasTagName(SVGNames::animateMotionTag))
                attributeName = SVGNames::animateMotionTag.localName();
            else
                continue;
        }
        
        // Results are accumulated to the first animation that animates a particular element/attribute pair.
        ElementAttributePair key(targetElement, attributeName); 
        SVGSMILElement* resultElement = resultsElements.get(key);
        if (!resultElement) {
            resultElement = animation;
            resultElement->resetToBaseValue(baseValueFor(key));
            resultsElements.add(key, resultElement);
        }

        // This will calculate the contribution from the animation and add it to the resultsElement.
        animation->progress(elapsed, resultElement);

        SMILTime nextFireTime = animation->nextProgressTime();
        if (nextFireTime.isFinite())
            earliersFireTime = min(nextFireTime, earliersFireTime);
        else if (!animation->isContributing(elapsed)) {
            m_scheduledAnimations.remove(animation);
            if (m_scheduledAnimations.isEmpty())
                m_savedBaseValues.clear();
        }
    }
    
    Vector<SVGSMILElement*> animationsToApply;
    ResultElementMap::iterator end = resultsElements.end();
    for (ResultElementMap::iterator it = resultsElements.begin(); it != end; ++it)
        animationsToApply.append(it->second);

    // Sort <animateTranform> to be the last one to be applied. <animate> may change transform attribute as
    // well (directly or indirectly by modifying <use> x/y) and this way transforms combine properly.
    sortByApplyOrder(animationsToApply);
    
    // Apply results to target elements.
    for (unsigned n = 0; n < animationsToApply.size(); ++n)
        animationsToApply[n]->applyResultsToTarget();

    startTimer(earliersFireTime, animationFrameDelay);
    
    Document::updateStyleForAllDocuments();
}

#endif

}

#endif // ENABLE(SVG)