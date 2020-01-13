
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
  TS_Init_t hTS;

  hTS.Width = LCD_GetXSize();
  hTS.Height = LCD_GetYSize();
  hTS.Orientation =TS_SWAP_XY ;
  hTS.Accuracy = 5;  
  
  BSP_TS_Init(0, &hTS);
}

bool STM32H750TouchController::sampleTouch(int32_t& x, int32_t& y)
{
    TS_State_t state = { 0 };
    BSP_TS_GetState(0, &state);
    if (state.TouchDetected)
    {
        x = state.TouchX;
        y = state.TouchY;

        return true;
    }
    return false;
}
