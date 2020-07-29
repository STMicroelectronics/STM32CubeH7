
#include <touchgfx/hal/GPIO.hpp>

#include "stm32h735g_discovery.h"

using namespace touchgfx;

static bool gpioState[4];

void GPIO::init()
{
    BSP_LED_Init(LED_GREEN);
    BSP_LED_Init(LED_RED);

    for (int i = 0; i < 2; i++)
    {
        clear((GPIO_ID)i);
    }
}

void GPIO::set(GPIO_ID id)
{
    gpioState[id] = 1;
    if (id == VSYNC_FREQ)
    {
        BSP_LED_Off(LED_GREEN);
    }
    else if (id == RENDER_TIME)
    {
        BSP_LED_Off(LED_RED);
    }
}

void GPIO::clear(GPIO_ID id)
{
    gpioState[id] = 0;
    if (id == VSYNC_FREQ)
    {
        BSP_LED_On(LED_GREEN);
    }
    else if (id == RENDER_TIME)
    {
        BSP_LED_On(LED_RED);
    }
}

void GPIO::toggle(GPIO_ID id)
{
    if (get(id))
    {
        clear(id);
    }
    else
    {
        set(id);
    }
}

bool GPIO::get(GPIO_ID id)
{
    return gpioState[id];
}
