
#ifndef STM32H7TOUCHCONTROLLER_HPP
#define STM32H7TOUCHCONTROLLER_HPP

#include <platform/driver/touch/TouchController.hpp>

namespace touchgfx
{
class STM32H7TouchController: public TouchController
{
public:
    STM32H7TouchController() {}
    virtual void init();
    virtual bool sampleTouch(int32_t& x, int32_t& y);
};

}

#endif // STM32H7TOUCHCONTROLLER_HPP
