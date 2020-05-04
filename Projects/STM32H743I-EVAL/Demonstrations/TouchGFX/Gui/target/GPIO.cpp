
#include <touchgfx/hal/GPIO.hpp>

#ifdef STM32H747xx
#include "stm32h747i_eval.h"
#else
#include "stm32h743i_eval.h"
#endif

using namespace touchgfx;

static bool ledRenderTimeState;

static bool isRenderTime(GPIO::GPIO_ID id)
{
    return id == GPIO::RENDER_TIME;
}

static bool isVsync(GPIO::GPIO_ID id)
{
    return id == GPIO::VSYNC_FREQ;
}

void GPIO::init()
{
    BSP_LED_Init(LED_GREEN);
    clear(GPIO::RENDER_TIME);

    //BSP_LED_Init(LED_BLUE);
    clear(GPIO::VSYNC_FREQ);
}

void GPIO::set(GPIO::GPIO_ID id)
{
    if (isRenderTime(id))
    {
        ledRenderTimeState = 1;
        BSP_LED_On(LED_GREEN);
    }

    if (isVsync(id))
    {
        //BSP_LED_On(LED_BLUE);
    }
}

void GPIO::clear(GPIO::GPIO_ID id)
{
    if (isRenderTime(id))
    {
        ledRenderTimeState = 0;
        BSP_LED_Off(LED_GREEN);
    }

    if (isVsync(id))
    {
        //BSP_LED_Off(LED_BLUE);
    }
}

void GPIO::toggle(GPIO::GPIO_ID id)
{
    if (isRenderTime(id))
    {
        BSP_LED_Toggle(LED_GREEN);
    }

    if (isVsync(id))
    {
        //BSP_LED_Toggle(LED_BLUE);
    }
}

bool GPIO::get(GPIO::GPIO_ID id)
{
    if (isRenderTime(id))
    {
        return ledRenderTimeState;
    }
    return 0;
}
