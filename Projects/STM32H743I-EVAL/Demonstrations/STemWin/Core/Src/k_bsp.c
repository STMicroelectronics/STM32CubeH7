/**
  ******************************************************************************
  * @file    k_bsp.c
  * @author  MCD Application Team
  * @brief   This file provides the kernel bsp functions
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "k_bsp.h"

/** @addtogroup CORE
  * @{
  */

/** @defgroup KERNEL_BSP
  * @brief Kernel BSP routines
  * @{
  */

/* External variables --------------------------------------------------------*/
extern uint8_t SelLayer;
uint8_t detected = 0;
uint8_t Touchdetected = 0;

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Initializes LEDs, SDRAM, touch screen, CRC and SRAM.
  * @param  None 
  * @retval None
  */
void k_BspInit(void)
{
  /* Initialize the NOR */
  BSP_QSPI_Init();
  BSP_QSPI_EnableMemoryMappedMode();  

  /* Initialize the SDRAM */
  BSP_SDRAM_Init();
  
  /* Initialize the Touch screen */
  BSP_TS_Init(640, 480);   
        
  /* Enable CRC to Unlock GUI */
  __HAL_RCC_CRC_CLK_ENABLE();
  /* Enable Back up SRAM */
  /* Enable write access to Backup domain */
  PWR->CR1 |= PWR_CR1_DBP;
  while ((PWR->CR1 & PWR_CR1_DBP) == RESET)
  {
  }
  /*Enable BKPRAM clock*/
  __HAL_RCC_BKPRAM_CLK_ENABLE();
  
}

/**
  * @brief  Read the coordinate of the point touched and assign their
  *         value to the variables u32_TSXCoordinate and u32_TSYCoordinate
  * @param  None
  * @retval None
  */
void k_TouchUpdate(void)
{
  static GUI_PID_STATE TS_State = {0, 0, 0, 0};
  __IO TS_StateTypeDef  ts;
  uint16_t xDiff, yDiff;
  
  BSP_TS_GetState((TS_StateTypeDef *)&ts);

  if((ts.x >= LCD_GetXSize()) ||(ts.y >= LCD_GetYSize()) ) 
  {
    ts.x = 0;
    ts.y = 0;
  }

  xDiff = (TS_State.x > ts.x) ? (TS_State.x - ts.x) : (ts.x - TS_State.x);
  yDiff = (TS_State.y > ts.y) ? (TS_State.y - ts.y) : (ts.y - TS_State.y);
  
  if((ts.TouchDetected == 1) || (ts.TouchDetected == 0))
  {
    if((TS_State.Pressed != ts.TouchDetected) ||
       ((xDiff > 0 )||
        (yDiff > 0)))
    {
      TS_State.Pressed = ts.TouchDetected;
      TS_State.Layer = SelLayer;
      if(ts.TouchDetected)
      {
        detected++;
        if(detected >= 2)
        {
          ts.TouchDetected = 0;
          TS_State.x = ts.x;
          TS_State.y = ts.y ;
          GUI_TOUCH_StoreStateEx(&TS_State);
        }
      }
      else
      {
        GUI_TOUCH_StoreStateEx(&TS_State); 
        TS_State.x = 0;
        TS_State.y = 0;
        if(detected >= 2)
        {
          detected = 0;
        }
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
