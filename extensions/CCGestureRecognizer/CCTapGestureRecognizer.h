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

class CCTapGestureRecognizer : public CCGestureRecognizer
{
public:
    bool init();
    ~CCTapGestureRecognizer();
    CREATE_FUNC(CCTapGestureRecognizer);
    
    virtual bool onTouchBegan(cocos2d::Touch * pTouch, cocos2d::Event * pEvent);
    virtual void onTouchMoved(cocos2d::Touch * pTouch, cocos2d::Event * pEvent){};
    virtual void onTouchEnded(cocos2d::Touch * pTouch, cocos2d::Event * pEvent);
protected:
    CC_SYNTHESIZE(unsigned int, numberOfTapsRequired, NumberOfTapsRequired);
private:
    int taps;
    Point initialPosition, finalPosition;
    
    std::chrono::high_resolution_clock::time_point startTime, endTime;
    
    void stopGestureRecognition();

    const static uint32_t kTapMaxDuration;
    const static uint32_t kTapMaxDurationBetweenTaps;
    const static uint32_t kTapMaxDistance;
    const static uint32_t kTapMaxDistanceBetweenTaps;
};

NS_CC_EXT_END
