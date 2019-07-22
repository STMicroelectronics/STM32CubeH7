/**
  ******************************************************************************
  * @file    USB_Host/MSC_Standalone/CM7/Src/menu.c
  * @author  MCD Application Team
  * @brief   This file implements Menu Functions
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------ */
#include "main.h"

/* Private typedef ----------------------------------------------------------- */
/* Private define ------------------------------------------------------------ */
/* Private macro ------------------------------------------------------------- */
/* Private variables --------------------------------------------------------- */
MSC_DEMO_StateMachine msc_demo;
JOYState_TypeDef JoyState = JOY_NONE;
uint8_t prev_select = 0;
uint8_t joy_select = 0;
uint8_t *MSC_main_menu[] = {
  (uint8_t *)
    "      1 - File Operations                                                   ",
  (uint8_t *)
    "      2 - Explorer Disk                                                     ",
  (uint8_t *)
    "      3 - Re-Enumerate                                                      ",
};

/* Private function prototypes ----------------------------------------------- */

/* Private functions --------------------------------------------------------- */

/**
* @brief  Demo state machine.
* @param  None
* @retval None
*/
void Menu_Init(void)
{
  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  USBH_UsrLog("Connect your MSC Device");
  msc_demo.state = MSC_DEMO_WAIT;
}

/**
* @brief  Manages MSC Menu Process.
* @param  None
* @retval None
*/
void MSC_MenuProcess(void)
{
  switch(msc_demo.state)
  {
  case MSC_DEMO_WAIT:
    msc_demo.state = MSC_DEMO_FILE_OPERATIONS;
    break;

  case MSC_DEMO_FILE_OPERATIONS:
    /* Read and Write File Here */
    if(Appli_state == APPLICATION_READY)
    {
      LCD_DbgLog("\n\n*** Files operations ***\n");
      msc_demo.state = MSC_DEMO_EXPLORER;
      if(MSC_File_Operations() != 0)
      {
        msc_demo.state = MSC_DEMO_IDLE;
        return;
      }
    }
    break;

  case MSC_DEMO_EXPLORER:
    /* Display disk content */
    if(Appli_state == APPLICATION_READY)
    {
      LCD_DbgLog("\n\n*** Explore Disk ***\n");
      Explore_Disk("0:/", 1);
      msc_demo.state = MSC_DEMO_IDLE;
    }
    break;

  default:
    break;
  }
  if(Appli_state == APPLICATION_DISCONNECT)
  {
    Appli_state = APPLICATION_IDLE;
    USBH_ErrLog("USB device disconnected !!! \n");
    msc_demo.state = MSC_DEMO_WAIT;
  }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
