/**
  ******************************************************************************
  * @file    k_bsp.c
  * @author  MCD Application Team
  * @brief   This file provides the kernel bsp functions
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
TS_Init_t hTS;
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
  BSP_QSPI_Init_t init ;
  init.InterfaceMode=MT25TL01G_QPI_MODE;
  init.TransferRate= MT25TL01G_DTR_TRANSFER ;
  init.DualFlashMode= MT25TL01G_DUALFLASH_ENABLE;
  
  BSP_QSPI_Init(0,&init);
  
  BSP_QSPI_EnableMemoryMappedMode(0);  

  /* Initialize the SDRAM */
  BSP_SDRAM_Init(0);
  
  /* Initialize the Touch screen */  
  hTS.Width = 640;
  hTS.Height = 480;
  hTS.Orientation = TS_SWAP_NONE;
  hTS.Accuracy = 5;
  
  BSP_TS_Init(0, &hTS);
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
  TS_State_t  ts;
  uint16_t xDiff, yDiff;
  
  BSP_TS_GetState(0,&ts);

  if((ts.TouchX >= LCD_GetXSize()) ||(ts.TouchY >= LCD_GetYSize()) ) 
  {
    ts.TouchX = 0;
    ts.TouchY = 0;
  }

  xDiff = (TS_State.x > ts.TouchX) ? (TS_State.x - ts.TouchX) : (ts.TouchX - TS_State.x);
  yDiff = (TS_State.y > ts.TouchY) ? (TS_State.y - ts.TouchY) : (ts.TouchY - TS_State.y);
  
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
          TS_State.x = ts.TouchX;
          TS_State.y = ts.TouchY ;
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

