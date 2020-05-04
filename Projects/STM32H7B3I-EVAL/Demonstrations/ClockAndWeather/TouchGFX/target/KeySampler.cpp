#include "KeySampler.hpp"
#include "touchgfx/hal/Buttons.hpp"
#include "string.h"
#include "stm32h7b3i_eval.h"
using namespace touchgfx;

static uint8_t btnstatus[4];

void KeySampler::init()
{
    Buttons::init();

    memset(btnstatus, 0x00, sizeof(uint8_t) * 4);
}

bool KeySampler::sample(uint8_t& key)
{
    unsigned int buttonValue = Buttons::sample();

    if (buttonValue != BUTTONn)
    {
        key = buttonValue;
        return true;
    }

    return false;
}
