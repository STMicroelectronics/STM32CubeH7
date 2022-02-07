/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    : USB_Host/HID_Standalone/USB_Host/App/mouse.c
  * @author  : MCD Application Team
  * @brief   : This file implements the USB Host
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "mouse.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Mouse defines */
 /* Set the mouse bounds to -999,-999 999,999 */
#define MOUSE_WINDOW_X               0
#define MOUSE_WINDOW_Y               0
#define MOUSE_WINDOW_HEIGHT          500
#define MOUSE_WINDOW_WIDTH           500

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern HID_MOUSE_Info_TypeDef mouse_info;

/* Private function prototypes -----------------------------------------------*/
static void HID_MOUSE_ProcessData(HID_MOUSE_Info_TypeDef *data);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main routine for Mouse application
  * @param  phost: Host handle
  * @retval None
  */
void HID_MOUSE_App(USBH_HandleTypeDef *phost)
{
  HID_MOUSE_Info_TypeDef *m_pinfo;

  m_pinfo = USBH_HID_GetMouseInfo(phost);

  if(m_pinfo != NULL)
  {
    /* Handle Mouse data position */
    HID_MOUSE_ProcessData(&mouse_info);

    if(m_pinfo->buttons[0])
    {
      USBH_UsrLog("Left Button Pressed");
    }
    if(m_pinfo->buttons[1])
    {
      USBH_UsrLog("Right Button Pressed");
    }
    if(m_pinfo->buttons[2])
    {
      USBH_UsrLog("Middle Button Pressed");
    }
  }
}

/**
  * @brief  Processes Mouse data.
  * @param  data: Mouse data to be displayed
  * @retval None
  */
static void HID_MOUSE_ProcessData(HID_MOUSE_Info_TypeDef *data)
{
  if((data->x != 0) || (data->y != 0))
  {
    USBH_UsrLog("Mouse : X = %3d, Y = %3d", data->x, data->y);
  }

}

