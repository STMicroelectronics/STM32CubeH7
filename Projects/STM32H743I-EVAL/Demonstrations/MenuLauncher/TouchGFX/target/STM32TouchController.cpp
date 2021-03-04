/**
  ******************************************************************************
  * File Name          : STM32TouchController.cpp
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* USER CODE BEGIN STM32TouchController */

#include <STM32TouchController.hpp>
    
#include "stm32h7xx_hal.h"
#include "stm32h743i_eval_ts.h"

void STM32TouchController::init()
{
  uint8_t result;
  int trials = 10;
  TS_Init_t hTS;

  /* Initialize the Touch screen */  
  hTS.Width = 640;
  hTS.Height = 480;
  hTS.Orientation = TS_SWAP_NONE;
  hTS.Accuracy = 5;

  do
  {
    result = BSP_TS_Init(0, &hTS);
    HAL_Delay(10);
  } while ((result != BSP_ERROR_NONE) && (trials-- > 0));
  
  assert(result == BSP_ERROR_NONE);
}

static int32_t lastX, lastY;
static uint32_t lastTime;

bool STM32TouchController::sampleTouch(int32_t& x, int32_t& y)
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

/* USER CODE END STM32TouchController */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
