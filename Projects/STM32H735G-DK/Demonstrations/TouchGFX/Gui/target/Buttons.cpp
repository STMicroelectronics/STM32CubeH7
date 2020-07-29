#include <touchgfx/hal/Buttons.hpp>
#include <stdint.h>
#include "stm32h735g_discovery.h"
#include <FreeRTOS.h>
#include <queue.h>

using namespace touchgfx;

// extern "C" uint8_t BSP_PB_Init(Button_TypeDef Button, ButtonMode_TypeDef ButtonMode);
// extern "C" int32_t BSP_PB_GetState(Button_TypeDef Button);
extern QueueHandle_t ButtonEventQueue;
extern uint32_t user_button_press;
void Buttons::init()
{
}

unsigned int Buttons::sample()
{
    int result = user_button_press;

    if (user_button_press == BUTTON_USER)
    {
        user_button_press = BUTTONn;
    }

    xQueueReceive(ButtonEventQueue, &result, (TickType_t)0);

    return result;
}
