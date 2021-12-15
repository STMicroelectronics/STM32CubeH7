/**
  ******************************************************************************
  * @file    BSP/Src/sd.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use the SD Driver
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
#define SD_BLOCKSIZE      ((uint32_t)512U)  /*!< Block size is 512 bytes */
#define BLOCK_START_ADDR  0 /* Block start address      */
#define NUM_OF_BLOCKS     2 /* Total number of blocks   */
#define BUFFER_WORDS_SIZE ((SD_BLOCKSIZE * NUM_OF_BLOCKS) >> 2) /* Total data size in bytes */
#define SD_OK             0

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t aTxBuffer[BUFFER_WORDS_SIZE];
uint32_t aRxBuffer[BUFFER_WORDS_SIZE];

/* Private function prototypes -----------------------------------------------*/
static void SD_SetHint(void);
static void Fill_Buffer(uint32_t *pBuffer, uint32_t uwBufferLenght, uint32_t uwOffset);
static uint8_t Buffercmp(uint32_t* pBuffer1, uint32_t* pBuffer2, uint16_t BufferLength);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  SD Demo
  * @retval None
  */
void SD_demo (void)
{ 
  int32_t SD_state = BSP_ERROR_NONE;
  
  SD_SetHint();
  SD_state = ADAFRUIT_802_SD_Init(0);
  ButtonState = 0;
  
  if(SD_state != BSP_ERROR_NONE)
  {
    UTIL_LCD_DisplayStringAt(5, 90, (uint8_t *)"SD Initialization", LEFT_MODE);
    UTIL_LCD_DisplayStringAt(5, 105, (uint8_t *)"     FAILED", LEFT_MODE);
    UTIL_LCD_DisplayStringAt(5, 120, (uint8_t *)"SD Test Aborted.", LEFT_MODE);
  }
  else
  {
    UTIL_LCD_DisplayStringAt(5, 75, (uint8_t *)"SD Init : OK.", LEFT_MODE);
    
    SD_state = ADAFRUIT_802_SD_Erase(0, BLOCK_START_ADDR, BLOCK_START_ADDR + NUM_OF_BLOCKS - 1);
        /* Wait until SD card is ready to use for new operation */
    while(ADAFRUIT_802_SD_GetCardState(0) != SD_TRANSFER_OK);
    
    if(SD_state != BSP_ERROR_NONE)
    {
      UTIL_LCD_DisplayStringAt(5, 90, (uint8_t *)"SD ERASE : FAILED.", LEFT_MODE);
      UTIL_LCD_DisplayStringAt(5, 105, (uint8_t *)"SD Test Aborted.", LEFT_MODE);
    }
    else
    {
      UTIL_LCD_DisplayStringAt(5, 90, (uint8_t *)"SD ERASE : OK.", LEFT_MODE);
      
      /* Fill the buffer to write */
      Fill_Buffer(aTxBuffer, BUFFER_WORDS_SIZE, 0x22FF);
      SD_state = ADAFRUIT_802_SD_WriteBlocks(0, aTxBuffer, BLOCK_START_ADDR, NUM_OF_BLOCKS);
      
      if(SD_state != BSP_ERROR_NONE)
      {
        UTIL_LCD_DisplayStringAt(5, 105, (uint8_t *)"SD WRITE : FAILED.", LEFT_MODE);
        UTIL_LCD_DisplayStringAt(5, 120, (uint8_t *)"SD Test Aborted.", LEFT_MODE);
      }
      else
      {
        UTIL_LCD_DisplayStringAt(5, 105, (uint8_t *)"SD WRITE : OK.", LEFT_MODE);
        SD_state = ADAFRUIT_802_SD_ReadBlocks(0, aRxBuffer, BLOCK_START_ADDR, NUM_OF_BLOCKS);
        if(SD_state != BSP_ERROR_NONE)
        {
          UTIL_LCD_DisplayStringAt(5, 120, (uint8_t *)"SD READ : FAILED.", LEFT_MODE);
          UTIL_LCD_DisplayStringAt(5, 135, (uint8_t *)"SD Test Aborted.", LEFT_MODE);
        }
        else
        {
          UTIL_LCD_DisplayStringAt(5, 120, (uint8_t *)"SD READ : OK.", LEFT_MODE);
          if(Buffercmp(aTxBuffer, aRxBuffer, BUFFER_WORDS_SIZE) > 0)
          {
            UTIL_LCD_DisplayStringAt(5, 135, (uint8_t *)"SD COMPARE : FAILED.", LEFT_MODE);
            UTIL_LCD_DisplayStringAt(5, 150, (uint8_t *)"SD Test Aborted.", LEFT_MODE);
          }
          else
          {
            UTIL_LCD_DisplayStringAt(5, 135, (uint8_t *)"SD Test : OK.", LEFT_MODE);
            UTIL_LCD_DisplayStringAt(5, 150, (uint8_t *)"SD can be removed.", LEFT_MODE);
          }
        }
      }
    }
  }
  
  while (1)
  {
    if(CheckForUserInput() > 0)
    {
      /* ADAFRUIT_802_SD_DeInit(); */
      return;
    }
  }
}

/**
  * @brief  Display SD Demo Hint
  * @param  None
  * @retval None
  */
static void SD_SetHint(void)
{
  uint32_t x_size;
  
  ADAFRUIT_802_LCD_GetXSize(0, &x_size);
  
  /* Clear the LCD */ 
  UTIL_LCD_Clear(UTIL_LCD_COLOR_WHITE);
  
  /* Set SD Demo description */
  UTIL_LCD_FillRect(0, 0, x_size, 55, UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLUE); 
  UTIL_LCD_SetFont(&Font24);

  UTIL_LCD_SetFont(&Font12);
  UTIL_LCD_DisplayStringAt(0, 10, (uint8_t *)"SD Demo", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, 25, (uint8_t *)"Shows how to write", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, 40, (uint8_t *)"& read data on SD", CENTER_MODE);

  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE); 
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
