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

#include "CCPinchGestureRecognizer.h"

NS_CC_EXT_BEGIN

const float CCPinchGestureRecognizer::kPinchThreshold = 2.0f;

bool CCPinchGestureRecognizer::init()
{
    touchNumber = 0;
    lastDistance = 0;
    
    touches = new std::vector<cocos2d::Touch*>();
    
    return true;
}

CCPinchGestureRecognizer::~CCPinchGestureRecognizer()
{
    delete touches;
}

bool CCPinchGestureRecognizer::onTouchBegan(cocos2d::Touch * pTouch, cocos2d::Event * pEvent)
{
    if (isRecognizing || !isPositionBetweenBounds(pTouch->getLocation())) {
        return false;
    }
    
    touchNumber++;
    touches->push_back(pTouch);
    
    //start recognizing after that 2 fingers are touching
    if (touchNumber==2) {
        isRecognizing = true;
    }
    
    return true;
}

void CCPinchGestureRecognizer::onTouchMoved(cocos2d::Touch * pTouch, cocos2d::Event * pEvent)
{
    if (!isRecognizing) {
        return;
    }
    
    cocos2d::Touch * touch1 = (cocos2d::Touch*)touches->at(0);
    cocos2d::Touch * touch2 = (cocos2d::Touch*)touches->at(1);
    cocos2d::Point delta1 = touch1->getDelta();
    cocos2d::Point delta2 = touch2->getDelta();
    
    if ((fabs(delta1.x)<kPinchThreshold && fabs(delta1.y)<kPinchThreshold) || (fabs(delta2.x)<kPinchThreshold && fabs(delta2.y)<kPinchThreshold)) {
        return;
    }
    
    float distance = distanceBetweenPoints(touch1->getLocation(), touch2->getLocation());
    if (!lastDistance) {
        lastDistance = distance;
        return;
    }
    
    CCPinch * pinch = CCPinch::create();
    
    //decide type of pinch
    if (lastDistance<=distance) {
        pinch->type = kPinchGestureRecognizerTypeOpen;
    }
    else {
        pinch->type = kPinchGestureRecognizerTypeClose;
    }
    
    if ((delta1.x>0 && delta2.x<0) || (delta1.x<0 && delta2.x>0) || (delta1.y>0 && delta2.y<0) || (delta1.y<0 && delta2.y>0)) {
        gestureRecognized(pinch);
    }
}

void CCPinchGestureRecognizer::onTouchEnded(cocos2d::Touch * pTouch, cocos2d::Event * pEvent)
{
    isRecognizing = false;
    lastDistance = 0;
    touchNumber--;
    //touches(pTouch);
    
    //cancel touch over other views if necessary
    if (cancelsTouchesInView) {
        //stopTouchesPropagation(createSetWithTouch(pTouch), pEvent);
    }
}

NS_CC_EXT_END
