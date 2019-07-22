/**
  ******************************************************************************
  * @file    BSP/Src/mmc.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use the MMC Driver
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
#define BLOCK_START_ADDR         0     /* Block start address      */
#define NUM_OF_BLOCKS            5     /* Total number of blocks   */
#define BUFFER_WORDS_SIZE        ((MMC_BLOCKSIZE * NUM_OF_BLOCKS) >> 2) /* Total data size in bytes */
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t aTxBuffer[BUFFER_WORDS_SIZE];
uint32_t aRxBuffer[BUFFER_WORDS_SIZE];
__IO uint32_t MMCWriteStatus = 0, MMCReadStatus = 0;
/* Private function prototypes -----------------------------------------------*/
static void MMC_SetHint(void);
static void Fill_Buffer(uint32_t *pBuffer, uint32_t uwBufferLenght, uint32_t uwOffset);
static uint8_t Buffercmp(uint32_t* pBuffer1, uint32_t* pBuffer2, uint16_t BufferLength);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  MMC Demo
  * @param  None
  * @retval None
  */
void MMC_demo (void)
{
  uint8_t MMC_state = MMC_OK;

  MMC_SetHint();

  MMC_state = BSP_MMC_Init();

  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);

  if(MMC_state != MMC_OK)
  {
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    BSP_LCD_DisplayStringAt(20, 100, (uint8_t *)"MMC INITIALIZATION : FAIL.", LEFT_MODE);
    BSP_LCD_DisplayStringAt(20, 115, (uint8_t *)"MMC Test Aborted.", LEFT_MODE);
  }
  else
  {
    BSP_LCD_DisplayStringAt(20, 100, (uint8_t *)"MMC INITIALIZATION : OK.", LEFT_MODE);

    MMC_state = BSP_MMC_Erase(BLOCK_START_ADDR, (MMC_BLOCKSIZE * NUM_OF_BLOCKS));
    while(BSP_MMC_GetCardState() != MMC_TRANSFER_OK)
    {
    }

    if(MMC_state != MMC_OK)
    {
      BSP_LCD_SetTextColor(LCD_COLOR_RED);
      BSP_LCD_DisplayStringAt(20, 115, (uint8_t *)"MMC ERASE : FAILED.", LEFT_MODE);
      BSP_LCD_DisplayStringAt(20, 130, (uint8_t *)"MMC Test Aborted.", LEFT_MODE);
    }
    else
    {
      BSP_LCD_DisplayStringAt(20, 115, (uint8_t *)"MMC ERASE : OK.", LEFT_MODE);

      /* Fill the buffer to write */
      Fill_Buffer(aTxBuffer, BUFFER_WORDS_SIZE, 0x22FF);
      MMC_state = BSP_MMC_WriteBlocks((uint32_t *)aTxBuffer, BLOCK_START_ADDR, NUM_OF_BLOCKS, MMC_DATATIMEOUT);
      
      /* Wait until MMC cards are ready to use for new operation */
      while(BSP_MMC_GetCardState() != MMC_TRANSFER_OK)
      {
      }
      if(MMC_state != MMC_OK)
      {
        BSP_LCD_SetTextColor(LCD_COLOR_RED);
        BSP_LCD_DisplayStringAt(20, 130, (uint8_t *)"MMC WRITE : FAILED.", LEFT_MODE);
        BSP_LCD_DisplayStringAt(20, 145, (uint8_t *)"MMC Test Aborted.", LEFT_MODE);
      }
      else
      {
        BSP_LCD_DisplayStringAt(20, 130, (uint8_t *)"MMC WRITE : OK.", LEFT_MODE);
        MMC_state = BSP_MMC_ReadBlocks((uint32_t *)aRxBuffer, BLOCK_START_ADDR, NUM_OF_BLOCKS, MMC_DATATIMEOUT);

        /* Wait until MMC cards are ready to use for new operation */
        while(BSP_MMC_GetCardState() != MMC_TRANSFER_OK)
        {
        }
        if(MMC_state != MMC_OK)
        {
          BSP_LCD_SetTextColor(LCD_COLOR_RED);
          BSP_LCD_DisplayStringAt(20, 145, (uint8_t *)"MMC READ : FAILED.", LEFT_MODE);
          BSP_LCD_DisplayStringAt(20, 160, (uint8_t *)"MMC Test Aborted.", LEFT_MODE);
        }
        else
        {
          BSP_LCD_DisplayStringAt(20, 145, (uint8_t *)"MMC READ : OK.", LEFT_MODE);
          if(Buffercmp(aTxBuffer, aRxBuffer, BUFFER_WORDS_SIZE) > 0)
          {
            BSP_LCD_SetTextColor(LCD_COLOR_RED);
            BSP_LCD_DisplayStringAt(20, 160, (uint8_t *)"MMC COMPARE : FAILED.", LEFT_MODE);
            BSP_LCD_DisplayStringAt(20, 175, (uint8_t *)"MMC Test Aborted.", LEFT_MODE);
          }
          else
          {
            BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
            BSP_LCD_DisplayStringAt(20, 160, (uint8_t *)"MMC TEST : OK.", LEFT_MODE);
          }
        }
      }
    }
  }

  while (1)
  {
    
    if(CheckForUserInput() > 0)
    {
      return;
    }
  }
}


/**
  * @brief Tx Transfer completed callbacks
  * @param None
  * @retval None
  */
void BSP_MMC_WriteCpltCallback()
{
  MMCWriteStatus = 1;

}

/**
  * @brief Rx Transfer completed callbacks
  * @param None
  * @retval None
  */
void BSP_MMC_ReadCpltCallback()
{
  MMCReadStatus = 1;
}

/**
  * @brief  Display MMC Demo Hint
  * @param  None
  * @retval None
  */
static void MMC_SetHint(void)
{
  /* Clear the LCD */
  BSP_LCD_Clear(LCD_COLOR_WHITE);

  /* Set LCD Demo description */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), 80);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
  BSP_LCD_SetFont(&Font24);
  BSP_LCD_DisplayStringAt(0, 0, (uint8_t *)"MMC", CENTER_MODE);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, 30, (uint8_t *)"This example shows how to write", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 45, (uint8_t *)"and read data on the microMMC and also", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 60, (uint8_t *)"how to detect the presence of the card", CENTER_MODE);

   /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_DrawRect(10, 90, BSP_LCD_GetXSize() - 20, BSP_LCD_GetYSize()- 100);
  BSP_LCD_DrawRect(11, 91, BSP_LCD_GetXSize() - 22, BSP_LCD_GetYSize()- 102);

  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
 }

/**
  * @brief  Fills buffer with user predefined data.
  * @param  pBuffer: pointer on the buffer to fill
  * @param  uwBufferLenght: size of the buffer to fill
  * @param  uwOffset: first value to fill on the buffer
  * @retval None
  */
static void Fill_Buffer(uint32_t *pBuffer, uint32_t uwBufferLenght, uint32_t uwOffset)
{
  uint32_t tmpIndex = 0;

  /* Put in global buffer different values */
  for (tmpIndex = 0; tmpIndex < uwBufferLenght; tmpIndex++ )
  {
    pBuffer[tmpIndex] = tmpIndex + uwOffset;
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
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
