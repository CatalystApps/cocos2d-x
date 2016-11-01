/****************************************************************************
Copyright (c) 2013 Artavazd Barseghyan

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#include "CCLongPressGestureRecognizer.h"

NS_CC_EXT_BEGIN

const float CCLongPressGestureRecognizer::kLongPressMinDuration = 0.5f;
const float CCLongPressGestureRecognizer::kLongPressDistanceTolerance = 20.0f;

bool CCLongPressGestureRecognizer::init()
{
    isRecognizing = false;
    currTouch = NULL;
    
    setMinimumPressDuration(kLongPressMinDuration);
    
    return true;
}

CCLongPressGestureRecognizer::~CCLongPressGestureRecognizer()
{
    
}

bool CCLongPressGestureRecognizer::onTouchBegan(Touch * pTouch, Event * pEvent)
{
    if (isRecognizing)
    {
        stopGestureRecognition();
        return false;
    }
    
    origLocation = currLocation = pTouch->getLocation();
    // CCLOG("x pos is %f y pos is %f", origLocation.x, origLocation.y);
    // if (!isPositionBetweenBounds(origLocation)) return false;
    auto target = pEvent->getCurrentTarget();

    if (!isPointInNode(currLocation))
        return false;

    currTouch = pTouch;
    
    CCLOG("SCHEDULED");
    schedule(schedule_selector(CCLongPressGestureRecognizer::timerDidEnd), minimumPressDuration, 1, false);
    
    isRecognizing = true;

    CCLongPress* longPress = CCLongPress::create();
    longPress->location = currLocation;
    longPress->cancelPropagation = cancelsTouchesInView;

    gestureBegan(longPress);

    return true;
}

void CCLongPressGestureRecognizer::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    currLocation = pTouch->getLocation();
}

void CCLongPressGestureRecognizer::onTouchEnded(Touch * pTouch, Event * pEvent)
{
    CCLongPress * longPress = CCLongPress::create();
    longPress->location = currLocation;
    longPress->cancelPropagation = cancelsTouchesInView;

    gestureEnded(longPress);

    if (longPress->cancelPropagation && !isRecognizing) // stop propagation if gesture already has been recognized
        stopTouchesPropagation(pEvent);

    stopGestureRecognition();
}

void CCLongPressGestureRecognizer::setParent(cocos2d::Node* p)
{
    Layer::setParent(p);

    if (p != nullptr)
    {
        cocos2d::Rect innerBB = p->getBoundingBox();
        innerBB = RectApplyAffineTransform(innerBB, getParentToNodeAffineTransform());
        setContentSize(innerBB.size);
        setPosition(innerBB.getMinX(), innerBB.getMinY());
        frame = innerBB;
        CCLOG("x pos is %f y pos is %f, w %f and h %f", frame.origin.x, frame.origin.y, frame.size.width, frame.size.height);
    }
}

void CCLongPressGestureRecognizer::stopGestureRecognition()
{
    if (!isRecognizing) return;
    
    currTouch = NULL;
    unschedule(schedule_selector(CCLongPressGestureRecognizer::timerDidEnd));
    isRecognizing = false;
}

void CCLongPressGestureRecognizer::timerDidEnd(float dt)
{
    // check if the current touch is near the original touch
    float distance = currLocation.getDistance(origLocation);
    if (distance > kLongPressDistanceTolerance)
    {
        stopGestureRecognition();
        return;
    }
    
    CCLongPress * longPress = CCLongPress::create();
    longPress->location = currLocation;
    longPress->cancelPropagation = cancelsTouchesInView;
    
    gestureRecognized(longPress);

    stopGestureRecognition();
}

NS_CC_EXT_END
