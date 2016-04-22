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

// https://github.com/oc2pcoj/cocos2d-x-extensions

#pragma once

#include "CCGestureRecognizer.h"
#include <chrono>

NS_CC_EXT_BEGIN

typedef enum {
    kSwipeGestureRecognizerDirectionRight = 1 << 0,
    kSwipeGestureRecognizerDirectionLeft  = 1 << 1,
    kSwipeGestureRecognizerDirectionUp    = 1 << 2,
    kSwipeGestureRecognizerDirectionDown  = 1 << 3
} CCSwipeGestureRecognizerDirection;

//this class is used for storing information about the swipe gesture
class CCSwipe : public CCGesture
{
public:
    CREATE_FUNC(CCSwipe);
    CCSwipeGestureRecognizerDirection direction;
};

class CCSwipeGestureRecognizer : public CCGestureRecognizer
{
public:
    bool init();
    ~CCSwipeGestureRecognizer();
    CREATE_FUNC(CCSwipeGestureRecognizer);
    
    virtual bool onTouchBegan(cocos2d::Touch * pTouch, cocos2d::Event * pEvent);
    virtual void onTouchMoved(cocos2d::Touch * pTouch, cocos2d::Event * pEvent){};
    virtual void onTouchEnded(cocos2d::Touch * pTouch, cocos2d::Event * pEvent);
protected:
    CC_SYNTHESIZE(int, direction, Direction);
    
private:
    Point initialPosition;
    //struct cc_timeval startTime;
    std::chrono::high_resolution_clock::time_point startTime;
    bool checkSwipeDirection(const cocos2d::Point& p1, const cocos2d::Point& p2, int & dir);

    const static uint32_t kSwipeMaxDuration;
    const static uint32_t kSwipeMinDistance;
};

NS_CC_EXT_END
