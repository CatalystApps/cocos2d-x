#pragma once

#include "CCGestureRecognizer.h"

NS_CC_EXT_BEGIN

class CCGestureRecognizerFactory
{
public:

    enum class KnownGestures : uint8_t
    {
        LongPress = 0,
        Pan,
        Pinch,
        Swipe,
        MultipleTap,
        MAX
    };

private:
    typedef std::map<KnownGestures, std::function<CCGestureRecognizer*(void)>> FactorySignatures_t;
public:
    static CCGestureRecognizer* create(KnownGestures type);
};

NS_CC_EXT_END
