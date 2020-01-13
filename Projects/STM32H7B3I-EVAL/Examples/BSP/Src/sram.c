/**
  ******************************************************************************
  * @file    BSP/Src/sram.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use the SRAM Driver
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32H7xx_HAL_Examples
  * @{
  */

/** @addtogroup BSP
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define BUFFER_SIZE         ((uint32_t)0x1000)
#define WRITE_READ_ADDR     ((uint32_t)0x0800)
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
ALIGN_32BYTES(uint32_t sram_aTxBuffer[BUFFER_SIZE]);
ALIGN_32BYTES(uint32_t sram_aRxBuffer[BUFFER_SIZE]);
uint32_t ubSramWrite = 0, ubSramRead = 0, ubSramInit = 0;
uint32_t TransferCplt;
/* Private function prototypes -----------------------------------------------*/
static void SRAM_SetHint(void);
static void Fill_Buffer(uint32_t *pBuffer, uint32_t uwBufferLength, uint32_t uwOffset);
static uint8_t Buffercmp(uint32_t* pBuffer1, uint32_t* pBuffer2, uint16_t BufferLength);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  SRAM Demo
  * @param  None
  * @retval None
  */
void SRAM_demo (void)
{
  SRAM_SetHint();

  /* Disable the LCD to avoid the refrech from the SDRAM */
  BSP_LCD_DisplayOff(0);

  /*##-1- Configure the SRAM device ##########################################*/
  /* SRAM device configuration */
  if(BSP_SRAM_Init(0) != BSP_ERROR_NONE)
  {
    ubSramInit++;
  }

  /*##-2- SRAM memory read/write access ######################################*/
  /* Fill the buffer to write */
  Fill_Buffer(sram_aTxBuffer, BUFFER_SIZE, 0xC20FC20F);

  /* Write data to the SRAM memory */
  if(HAL_SRAM_Write_32b(&hsram, (uint32_t *)(SRAM_DEVICE_ADDR + WRITE_READ_ADDR), sram_aTxBuffer, BUFFER_SIZE) != BSP_ERROR_NONE)
  {
    ubSramWrite++;
  }

  /* Read back data from the SRAM memory */
  if(HAL_SRAM_Read_32b(&hsram, (uint32_t *)(SRAM_DEVICE_ADDR + WRITE_READ_ADDR), sram_aRxBuffer, BUFFER_SIZE) != BSP_ERROR_NONE)
  {
    ubSramRead++;
  }

  /*##-3- Checking data integrity ############################################*/
  /* Enable the LCD */
  BSP_LCD_DisplayOn(0);

  if(ubSramInit != 0)
  {
    GUI_DisplayStringAt(20, 100, (uint8_t *)"SRAM Initialization : FAILED.", LEFT_MODE);
    GUI_DisplayStringAt(20, 115, (uint8_t *)"SRAM Test Aborted.", LEFT_MODE);
  }
  else
  {
    GUI_DisplayStringAt(20, 100, (uint8_t *)"SRAM Initialization : OK.", LEFT_MODE);
  }
  if(ubSramWrite != 0)
  {
    GUI_DisplayStringAt(20, 115, (uint8_t *)"SRAM WRITE : FAILED.", LEFT_MODE);
    GUI_DisplayStringAt(20, 130, (uint8_t *)"SRAM Test Aborted.", LEFT_MODE);
  }
  else
  {
    GUI_DisplayStringAt(20, 115, (uint8_t *)"SRAM WRITE : OK.", LEFT_MODE);
  }
  if(ubSramRead != 0)
  {
    GUI_DisplayStringAt(20, 130, (uint8_t *)"SRAM READ : FAILED.", LEFT_MODE);
    GUI_DisplayStringAt(20, 145, (uint8_t *)"SRAM Test Aborted.", LEFT_MODE);
  }
  else
  {
    GUI_DisplayStringAt(20, 130, (uint8_t *)"SRAM READ : OK.", LEFT_MODE);
  }

  if(Buffercmp(sram_aRxBuffer, sram_aTxBuffer, BUFFER_SIZE) > 0)
  {
    GUI_DisplayStringAt(20, 145, (uint8_t *)"SRAM COMPARE : FAILED.", LEFT_MODE);
    GUI_DisplayStringAt(20, 160, (uint8_t *)"SRAM Test Aborted.", LEFT_MODE);
  }
  else
  {
    GUI_DisplayStringAt(20, 145, (uint8_t *)"SRAM Test : OK.", LEFT_MODE);
  }
  ButtonState = 0;
  while (1)
  {
    if(CheckForUserInput() > 0)
    {
      ButtonState = 0;
      return;
    }
  }
}

/**
  * @brief  Display SRAM Demo Hint
  * @param  None
  * @retval None
  */
static void SRAM_SetHint(void)
{
  uint32_t x_size, y_size;

  BSP_LCD_GetXSize(0, &x_size);
  BSP_LCD_GetYSize(0, &y_size);

  /* Clear the LCD */
  GUI_Clear(GUI_COLOR_WHITE);

  /* Set LCD Demo description */
  GUI_FillRect(0, 0, x_size, 80, GUI_COLOR_BLUE);
  GUI_SetTextColor(GUI_COLOR_WHITE);
  GUI_SetBackColor(GUI_COLOR_BLUE);
  GUI_SetFont(&Font24);

  GUI_DisplayStringAt(0, 0, (uint8_t *)"SRAM", CENTER_MODE);
  GUI_SetFont(&Font12);
  GUI_DisplayStringAt(0, 30, (uint8_t *)"This example shows how to write", CENTER_MODE);
  GUI_DisplayStringAt(0, 45, (uint8_t *)"and read data on the SRAM", CENTER_MODE);


  /* Set the LCD Text Color */
  GUI_DrawRect(10, 90, x_size - 20, y_size- 100, GUI_COLOR_BLUE);
  GUI_DrawRect(11, 91, x_size - 22, y_size- 102, GUI_COLOR_BLUE);

  GUI_SetTextColor(GUI_COLOR_BLACK);
  GUI_SetBackColor(GUI_COLOR_WHITE);
}

/**
  * @brief  Fills buffer with user predefined data.
  * @param  pBuffer: pointer on the buffer to fill
  * @param  uwBufferLength: size of the buffer to fill
  * @param  uwOffset: first value to fill on the buffer
  * @retval None
  */
static void Fill_Buffer(uint32_t *pBuffer, uint32_t uwBufferLength, uint32_t uwOffset)
{
  uint32_t tmpindex = 0;

  /* Put in global buffer different values */
  for (tmpindex = 0; tmpindex < uwBufferLength; tmpindex++ )
  {
    pBuffer[tmpindex] = tmpindex + uwOffset;
  }
}

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval 1: pBuffer identical to pBuffer1
  *         0: pBuffer differs from pBuffer1
  */
static uint8_t Buffercmp(uint32_t* pBuffer1, uint32_t* pBuffer2, uint16_t BufferLength)
{
  while (BufferLength--)
  {
    if (*pBuffer1 != *pBuffer2)
    {
      return 1;
    }

    pBuffer1++;
    pBuffer2++;
  }

  return 0;
}

/**
  * @brief  DMA transfer complete callback.
  * @param  hmdma pointer to a SRAM_HandleTypeDef structure that contains
  *                the configuration information for SRAM module.
  * @retval None
  */
void HAL_SRAM_DMA_XferCpltCallback(MDMA_HandleTypeDef *hmdma)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hmdma);

  TransferCplt = 1;
}


/**
  * @}
  */

/**
  * @}
  */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
