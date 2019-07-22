
#include <touchgfx/hal/GPIO.hpp>

#include "stm32h750b_discovery.h"

using namespace touchgfx;

static bool gpioState[4];

void GPIO::init()
{
    BSP_LED_Init(LED1);
    BSP_LED_Init(LED2);

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
        BSP_LED_Off(LED1);
    }
    else if (id == RENDER_TIME)
    {
        BSP_LED_Off(LED2);
    }
}

void GPIO::clear(GPIO_ID id)
{
    gpioState[id] = 0;
    if (id == VSYNC_FREQ)
    {
        BSP_LED_On(LED1);
    }
    else if (id == RENDER_TIME)
    {
        BSP_LED_On(LED2);
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
