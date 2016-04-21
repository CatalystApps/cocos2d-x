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

#include "CCPanGestureRecognizer.h"

NS_CC_EXT_BEGIN

bool CCPanGestureRecognizer::init()
{
    return true;
}

CCPanGestureRecognizer::~CCPanGestureRecognizer()
{
    
}

bool CCPanGestureRecognizer::onTouchBegan(Touch * pTouch, Event * pEvent)
{
    if (isRecognizing) {
        isRecognizing = false;
        return false;
    }
    
    Point loc = pTouch->getLocation();
    if (!isPositionBetweenBounds(loc)) return false;
    
    isRecognizing = true;
    
    CCPan * pan = CCPan::create();
    pan->location = pTouch->getLocation();
    pan->delta = Point(0,0);
    
    gestureBegan(pan);
    return true;
}

void CCPanGestureRecognizer::onTouchMoved(Touch * pTouch, Event * pEvent)
{
    CCPan * pan = CCPan::create();
    pan->location = pTouch->getLocation();
    pan->delta = pTouch->getDelta();
    gestureRecognized(pan);
}

void CCPanGestureRecognizer::onTouchEnded(Touch * pTouch, Event * pEvent)
{
    CCPan * pan = CCPan::create();
    pan->location = pTouch->getLocation();
    pan->delta = pTouch->getDelta();
    pan->cancelPropagation = cancelsTouchesInView;
    
    gestureEnded(pan);
    
    isRecognizing = false;
    
    //cancel touch over other views if necessary
    if (pan->cancelPropagation) {
        stopTouchesPropagation(pEvent);
    }
}

NS_CC_EXT_END
