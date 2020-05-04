


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
  TS_Init_t hTS;
  hTS.Width = LCD_GetXSize();
  hTS.Height = LCD_GetYSize();
  hTS.Orientation = TS_SWAP_XY | TS_SWAP_Y;
  hTS.Accuracy =5;
  
  /* Touchscreen initialization */
  BSP_TS_Init(0, &hTS);
}

bool STM32H747TouchController::sampleTouch(int32_t& x, int32_t& y)
{
    TS_State_t state = { 0 };
    BSP_TS_GetState(0,&state);
    if (state.TouchDetected)
    {
        x = state.TouchX;
        y = state.TouchY;

        return true;
    }
    return false;
}
