
#include <EXC7200TouchController.hpp>

#include "stm32h7xx.h"
#include "stm32h743i_eval.h"
#include "stm32h743i_eval_lcd.h"
#include "stm32h743i_eval_ts.h"

void EXC7200TouchController::init()
{
  TS_Init_t hTS;

  /* Initialize the Touch screen */  
  hTS.Width = 640;
  hTS.Height = 480;
  hTS.Orientation = TS_SWAP_NONE;
  hTS.Accuracy = 5;
  
  BSP_TS_Init(0, &hTS);

}

static int32_t lastX, lastY;
static uint32_t lastTime;

bool EXC7200TouchController::sampleTouch(int32_t& x, int32_t& y)
{
    TS_State_t state;
    unsigned int time;
    BSP_TS_GetState(0, &state);
    if (state.TouchDetected)
    {
        x = lastX = state.TouchX;
        y = lastY = state.TouchY;
        lastTime = HAL_GetTick();
        return true;
    }

    time = HAL_GetTick();
    if (((lastTime + 120) > time)
        && (lastX > 0) && (lastY > 0)) {
        x = lastX;
        y = lastY;
        return true;
    }

    return false;
}
