/**
  ******************************************************************************
  * @file    Demonstration/STM32H745-Discovery_Demo/CM7/Src/touchscreen.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use the touchscreen driver.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "touchscreen.h"
#include <stdlib.h> 
/** @addtogroup STM32H7xx_HAL_Demonstration
  * @{
  */

/** @addtogroup Oscilloscope_SignalsGenerator
  * @{
  */


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/


/* Private Structures and Enumerations ------------------------------------------------------------*/

/* Global variables ---------------------------------------------------------*/
TS_State_t  TS_State;
TS_Init_t   hTS;

uint32_t AutoMode = 0;
uint32_t AutoModeCountDown = 100;
uint32_t AutoChangeDelay = 0;
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

uint32_t SG_InitTouchScreen(void)
{  
  uint32_t ret = DEMO_OK;
  int32_t probeStatus;
  uint32_t Instance = 0;
  uint32_t x_size, y_size;

  BSP_LCD_GetXSize(0, &x_size);
  BSP_LCD_GetYSize(0, &y_size);

  hTS.Width = x_size;
  hTS.Height = y_size;
  probeStatus = GT911_Probe(Instance);
  if (probeStatus == BSP_ERROR_NONE)
  {
    hTS.Orientation = TS_SWAP_NONE;
  }
  else
  {
    probeStatus = FT5336_Probe(Instance);
    if (probeStatus == BSP_ERROR_NONE)
    {
      hTS.Orientation = TS_SWAP_XY;
    }
  }
  hTS.Accuracy = 5;

  /* Touchscreen initialization */
  if(BSP_TS_Init(0, &hTS) != BSP_ERROR_NONE)
  {
    ret = DEMO_ERROR;
  }
  
  return ret;
}

/**
  * @brief  Get wave buttons position
  * @param  None
  * @retval None
  */
SG_WaveButtonIndex_e SG_GetWaveButtonIndex(SG_SwitchButtonWidgetTypeDef *pButton, uint32_t buttonsNumber)
{
  uint16_t x, y;
  uint8_t buttonIndex;
  SG_WaveButtonIndex_e waveButtonIndex = SG_MAX_WAVE_BUTTON_INDEX;
  
  /* Check the touch screen coordinates */
  BSP_TS_GetState(0, &TS_State);

  if(TS_State.TouchDetected)
  {
    AutoMode = 0;
    AutoModeCountDown = 200;
    /* One or dual touch have been detected          */
    /* Only take into account the first touch so far */

    /* Get X and Y position of the first touch post calibrated */
    x = TS_State.TouchX;
    y = TS_State.TouchY;
    
    for(buttonIndex = 0; buttonIndex < buttonsNumber; buttonIndex++)
    {
      if ((x > pButton[buttonIndex].buttonParams.Xpos) &&
        (x < (pButton[buttonIndex].buttonParams.Xpos + pButton[buttonIndex].buttonParams.Width)))
      {
        if ((y > pButton[buttonIndex].buttonParams.Ypos) &&
            (y < (pButton[buttonIndex].buttonParams.Ypos + pButton[buttonIndex].buttonParams.Height)))
        {
          waveButtonIndex = pButton[buttonIndex].ButtonIndex;
        }
      }
    }
  }
  else
  {
    if (AutoModeCountDown > 0)
    {
     AutoModeCountDown --;
    }
     else
    {
      AutoMode = 1;
      AutoChangeDelay++;
      if (AutoChangeDelay%30 > 25)
      {
        waveButtonIndex = (SG_WaveButtonIndex_e)((rand()) % ((uint32_t)SG_MAX_WAVE_BUTTON_INDEX));
        AutoChangeDelay = 0;
      }
    }
    
  }
  
  return waveButtonIndex;
}

/**
  * @brief  Get up down buttons position
  * @param  None
  * @retval None
  */
SG_UpDownButtonIndex_e SG_GetUpDownButtonIndex(ButtonWidgetTypeDef *pButton, uint32_t buttonsNumber)
{
  uint16_t x, y;
  uint8_t buttonIndex;
  SG_UpDownButtonIndex_e upDownButtonIndex = SG_MAX_UPDOWN_BUTTON_INDEX;
  
  /* Check the touch screen coordinates */
  BSP_TS_GetState(0, &TS_State);
  
  if(TS_State.TouchDetected)
  {
    AutoMode = 0;
    AutoModeCountDown = 200;
    /* One or dual touch have been detected          */
    /* Only take into account the first touch so far */

    /* Get X and Y position of the first touch post calibrated */
    x = TS_State.TouchX;
    y = TS_State.TouchY;
    
    for(buttonIndex = 0; buttonIndex < buttonsNumber; buttonIndex++)
    {
      if ((x > pButton[buttonIndex].buttonParams.Xpos) &&
        (x < (pButton[buttonIndex].buttonParams.Xpos + pButton[buttonIndex].buttonParams.Width)))
      {
        if ((y > pButton[buttonIndex].buttonParams.Ypos) &&
            (y < (pButton[buttonIndex].buttonParams.Ypos + pButton[buttonIndex].buttonParams.Height)))
        {
          upDownButtonIndex = (SG_UpDownButtonIndex_e)pButton[buttonIndex].ButtonIndex;
        }
      }
    }
  }
  else
  {
    if (AutoModeCountDown > 0)
    {
     AutoModeCountDown --;
    }
    else
    {

      AutoMode = 1;
      AutoChangeDelay++;
      if (AutoChangeDelay%30 > 25)
      {
        upDownButtonIndex = (SG_UpDownButtonIndex_e) ((rand()) % (SG_MAX_UPDOWN_BUTTON_INDEX));
        AutoChangeDelay = 0;
      }
    }
      
    
  }
  
  return upDownButtonIndex;
}


/* Private typedef -----------------------------------------------------------*/
typedef  void (*pFunction)(void);

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
pFunction JumpToApplication;
/**
  * @brief  Update Generic Buttons
  * @param  None
  * @retval None
  */
void SG_UpdateGenericButtons(ButtonWidgetTypeDef *pButton)
{
  uint16_t x, y;
  
  /* Check the touch screen coordinates */
  BSP_TS_GetState(0, &TS_State);
  
  if(TS_State.TouchDetected)
  {
    /* One or dual touch have been detected          */
    /* Only take into account the first touch so far */
    
    /* Get X and Y position of the first touch post calibrated */
    x = TS_State.TouchX;
    y = TS_State.TouchY;
    
    if ((x > pButton[0].buttonParams.Xpos) &&
        (x < (pButton[0].buttonParams.Xpos + pButton[0].buttonParams.Width)))
    {
      if ((y > pButton[0].buttonParams.Ypos) &&
          (y < (pButton[0].buttonParams.Ypos + pButton[0].buttonParams.Height)))
      {
        
        HAL_NVIC_SystemReset();
      }
    }
  }
  
  
  
}

/**
  * @}
  */

/**
  * @}
  */

