


#include "STM32H747TouchController.hpp"

extern "C"
{
#ifdef USE_STM32H747I_DISCO
#include "stm32h747i_discovery_ts.h"
#else
#include "stm32h747i_eval_ts.h"
#endif
uint32_t LCD_GetXSize();
uint32_t LCD_GetYSize();
}

using namespace touchgfx;

void STM32H747TouchController::init()
{
    BSP_TS_Init(LCD_GetXSize(), LCD_GetYSize());
}

bool STM32H747TouchController::sampleTouch(int32_t& x, int32_t& y)
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
