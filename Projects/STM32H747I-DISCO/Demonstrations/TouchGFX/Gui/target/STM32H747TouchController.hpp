


#ifndef STM32H747ITOUCHCONTROLLER_HPP
#define STM32H747ITOUCHCONTROLLER_HPP

#include <platform/driver/touch/TouchController.hpp>

namespace touchgfx
{
class STM32H747TouchController: public TouchController
{
public:
    STM32H747TouchController() {}
    virtual void init();
    virtual bool sampleTouch(int32_t& x, int32_t& y);
};

}

#endif // STM32H747ITOUCHCONTROLLER_HPP
