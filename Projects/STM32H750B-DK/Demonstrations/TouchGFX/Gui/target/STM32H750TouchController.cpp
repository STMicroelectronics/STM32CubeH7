
#include "STM32H750TouchController.hpp"

extern "C"
{
#include "stm32h750b_discovery_ts.h"

    uint32_t LCD_GetXSize();
    uint32_t LCD_GetYSize();
}

using namespace touchgfx;

void STM32H750TouchController::init()
{
    BSP_TS_Init(LCD_GetXSize(), LCD_GetYSize());
}

bool STM32H750TouchController::sampleTouch(int32_t& x, int32_t& y)
{
    TS_StateTypeDef state = { 0 };
    BSP_TS_GetState(&state);
    if (state.touchDetected)
    {
        x = state.touchX[0];
        y = state.touchY[0];

        return true;
    }
    return false;
}
