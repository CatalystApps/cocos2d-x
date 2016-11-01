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

NS_CC_EXT_BEGIN

//this class is used for storing information about the pan gesture
class CCLongPress : public CCGesture
{
public:
    CREATE_FUNC(CCLongPress);
};

class CCLongPressGestureRecognizer : public CCGestureRecognizer
{
public:
    bool init();
    ~CCLongPressGestureRecognizer();
    CREATE_FUNC(CCLongPressGestureRecognizer);
    void timerDidEnd(float dt);
    
    virtual bool onTouchBegan(cocos2d::Touch * pTouch, cocos2d::Event * pEvent);
    virtual void onTouchMoved(cocos2d::Touch * pTouch, cocos2d::Event * pEvent);
    virtual void onTouchEnded(cocos2d::Touch * pTouch, cocos2d::Event * pEvent);

    virtual void setParent(cocos2d::Node*p) override;
protected:
    CC_SYNTHESIZE(float, minimumPressDuration, MinimumPressDuration);
private:
    cocos2d::Point origLocation, currLocation;
    cocos2d::Touch * currTouch;
    
    void stopGestureRecognition();

    const static float kLongPressMinDuration;
    const static float kLongPressDistanceTolerance;
};

NS_CC_EXT_END
