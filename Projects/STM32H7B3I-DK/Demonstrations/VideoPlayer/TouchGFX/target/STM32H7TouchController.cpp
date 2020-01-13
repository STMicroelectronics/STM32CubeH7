
#include "STM32H7TouchController.hpp"

#include <cmsis_os.h>

extern "C"
{
#include "stm32h7b3i_discovery_ts.h"

    uint32_t LCD_GetXSize();
    uint32_t LCD_GetYSize();
}

using namespace touchgfx;

void STM32H7TouchController::init()
{
    TS_Init_t hTS;

    hTS.Orientation = TS_SWAP_XY;
    hTS.Accuracy = 8;
    hTS.Width = LCD_GetXSize();
    hTS.Height = LCD_GetYSize();
    BSP_TS_Init(0, &hTS);
    
}

bool STM32H7TouchController::sampleTouch(int32_t& x, int32_t& y)
{
    TS_State_t TS_State = { 0 };

    /* This should never fail !! */
    if(BSP_TS_GetState(0, &TS_State) != BSP_ERROR_NONE)
    {
      configASSERT(0);
    }

    if (TS_State.TouchDetected)
    {
        x = TS_State.TouchX;
        y = TS_State.TouchY;

        return true;
    }   

    return false;
}
