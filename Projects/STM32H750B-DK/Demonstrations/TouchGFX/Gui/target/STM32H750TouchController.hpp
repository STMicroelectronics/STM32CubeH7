
#ifndef STM32H750TOUCHCONTROLLER_HPP
#define STM32H750TOUCHCONTROLLER_HPP

#include <platform/driver/touch/TouchController.hpp>

namespace touchgfx
{
class STM32H750TouchController: public TouchController
{
public:
    STM32H750TouchController() {}
    virtual void init();
    virtual bool sampleTouch(int32_t& x, int32_t& y);
};

}

#endif // STM32H750TOUCHCONTROLLER_HPP
