
#include <touchgfx/hal/GPIO.hpp>

#include "stm32h7b3i_eval.h"

using namespace touchgfx;

static bool gpioState[4];

void GPIO::init()
{
}

void GPIO::clear(GPIO_ID id)
{
    gpioState[id] = 1;
}

void GPIO::set(GPIO_ID id)
{
    gpioState[id] = 0;
}

void GPIO::toggle(GPIO_ID id)
{
}

bool GPIO::get(GPIO_ID id)
{
    return gpioState[id];
}
