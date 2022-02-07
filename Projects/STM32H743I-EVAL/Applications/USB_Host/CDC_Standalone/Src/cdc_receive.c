/**
  ******************************************************************************
  * @file    USB_Host/CDC_Standalone/Src/cdc_receive.c 
  * @author  MCD Application Team
  * @brief   CDC Receive state machine
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
#include "main.h"

/* Private define ------------------------------------------------------------*/
#define RX_BUFF_SIZE   0x400  /* Max Received data 1KB */

/* Private typedef -----------------------------------------------------------*/
uint8_t *DEMO_RECEIVE_menu[] = 
{
  (uint8_t *)"      1 - Start receiving data                                              ",
  (uint8_t *)"      2 - Return                                                            ",
  (uint8_t *)"                                                                            ",
};

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint16_t xPos, yLinePos;
 
/* Private function prototypes -----------------------------------------------*/
uint8_t CDC_RX_Buffer[RX_BUFF_SIZE];

/* Private functions ---------------------------------------------------------*/
static void DumpReceivedData(void);
static void ReturnFromReceiveMenu(void);

/**
  * @brief  Handles CDC Receive Menu. 
  * @param  None
  * @retval None
  */
void CDC_Handle_Receive_Menu(void)
{
  switch(CdcDemo.Receive_state)
  {
  case CDC_RECEIVE_IDLE:
    CdcDemo.Receive_state = CDC_RECEIVE_WAIT;
    CDC_SelectItem(DEMO_RECEIVE_menu, 0);
    CdcDemo.select = 0; 
    USBH_CDC_Stop(&hUSBHost);
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_GREEN); 
    UTIL_LCD_DisplayStringAtLine(14, (uint8_t *)"                                          ");
    UTIL_LCD_DisplayStringAtLine(15, (uint8_t *)"Use [Buttons Left/Right] to scroll up/down");
    break;
    
  case CDC_RECEIVE_WAIT:
    if(CdcDemo.select != PrevSelect)
    {
      PrevSelect = CdcDemo.select;
      CDC_SelectItem(DEMO_RECEIVE_menu, CdcDemo.select & 0x7F);
      /* Handle select item */
      if(CdcDemo.select & 0x80)
      {
        switch(CdcDemo.select & 0x7F)
        {
        case 0: 
          UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE); 
          /* Start Reception */
          LCD_ClearTextZone();
          UTIL_LCD_DisplayStringAtLine(3, (uint8_t *)"Receiving data ...");
          xPos = 0;
          yLinePos = 4;
          memset(CDC_RX_Buffer, 0, RX_BUFF_SIZE); 
          USBH_CDC_Receive(&hUSBHost, CDC_RX_Buffer, RX_BUFF_SIZE);
          CdcDemo.Receive_state = CDC_RECEIVE_WAIT;
          break;

        case 1: 
          UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
          USBH_CDC_Stop(&hUSBHost);
          ReturnFromReceiveMenu();
          break;
          
        default:
          break;
        }
      }
    }   
    break;
    
  default:
    break;
  }
  CdcDemo.select &= 0x7F;  
}

/**
  * @brief  Returns from Receive Menu 
  * @param  None
  * @retval None
  */
static void ReturnFromReceiveMenu(void)
{
  CdcDemo.state = CDC_DEMO_IDLE;
  CdcDemo.select = 0; 
  
  /* Restore main menu */
  LCD_ClearTextZone();
  UTIL_LCD_TRACE_UpdateDisplay();
  Menu_Init();  
}

/**
  * @brief  CDC data receive callback.
  * @param  phost: Host handle
  * @retval None
  */
void USBH_CDC_ReceiveCallback(USBH_HandleTypeDef *phost)
{
  DumpReceivedData();
  USBH_CDC_Receive(&hUSBHost, CDC_RX_Buffer, RX_BUFF_SIZE);
}

/**
  * @brief  Displays received data
  * @param  data: Keyboard data to be displayed
  * @retval None
  */
static void DumpReceivedData(void)
{
  uint16_t size;
  uint8_t *ptr = CDC_RX_Buffer;
  
  uint32_t Xsize = 0;
  
  size = USBH_CDC_GetLastReceivedDataSize(&hUSBHost);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_YELLOW);
  while(size--)
  {
    if((*ptr != '\n') && (*ptr != '\r'))
    { 
      if(*ptr == '\t')
      {
         UTIL_LCD_DisplayChar(xPos, LINE(yLinePos), ' ');
      }
      else
      {
        UTIL_LCD_DisplayChar(xPos, LINE(yLinePos), *ptr);
      }
      xPos += 7;
    }
    else if(*ptr == '\n')
    {
      yLinePos++;
      xPos = 0;
    }
    
    ptr++;
    
    BSP_LCD_GetXSize(0, &Xsize);
    
    if(xPos > (Xsize - 7))
    {
      xPos = 0;
      yLinePos++;
    }
    
    if(yLinePos > 13)
    {
      UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_GREEN); 
      UTIL_LCD_DisplayStringAtLine(15, (uint8_t *)"Use [User Tamper] to see more data");
      /* TAmper Button in polling */
      while(BSP_PB_GetState(BUTTON_TAMPER) != RESET)
      {
        /* Wait for User Input */
      }
      
      LCD_ClearTextZone();
      UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
      UTIL_LCD_DisplayStringAtLine(3, (uint8_t *)"Receiving data ...");
      UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_YELLOW);
      xPos = 0;
      yLinePos = 4;
    }
  }
}

