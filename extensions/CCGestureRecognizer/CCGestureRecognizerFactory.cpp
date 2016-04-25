#include "CCGestureRecognizerFactory.h"

#include "CCLongPressGestureRecognizer.h"
#include "CCPanGestureRecognizer.h"
#include "CCPinchGestureRecognizer.h"
#include "CCSwipeGestureRecognizer.h"
#include "CCTapGestureRecognizer.h"

NS_CC_EXT_BEGIN

CCGestureRecognizer* CCGestureRecognizerFactory::create(KnownGestures type)
{
    static FactorySignatures_t signatures;

    static std::once_flag once;
    std::call_once(once, []()
    {
        signatures[KnownGestures::LongPress] = CCLongPressGestureRecognizer::create;
        signatures[KnownGestures::Pan] = CCPanGestureRecognizer::create;
        signatures[KnownGestures::Pinch] = CCPinchGestureRecognizer::create;
        signatures[KnownGestures::Swipe] = CCSwipeGestureRecognizer::create;
        signatures[KnownGestures::MultipleTap] = CCTapGestureRecognizer::create;
    });

    FactorySignatures_t::iterator it = signatures.find(type);

    if (it != signatures.end())
    {
        return it->second();
    }

    return nullptr;
}

NS_CC_EXT_END
