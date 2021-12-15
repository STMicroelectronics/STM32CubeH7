/**
  ******************************************************************************
  * @file    BSP/Src/sdram.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use the SDRAM Driver
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
#define BUFFER_SIZE            ((uint32_t)0x200)
#define WRITE_READ_ADDR        ((uint32_t)0x0000)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
ALIGN_32BYTES (uint32_t sdram_aTxBuffer[BUFFER_SIZE]);
ALIGN_32BYTES (uint32_t sdram_aRxBuffer[BUFFER_SIZE]);
/* Private function prototypes -----------------------------------------------*/
static void SDRAM_SetHint(void);
static void SDRAM_DMA_SetHint(void);
static void Fill_Buffer(uint32_t *pBuffer, uint32_t uwBufferLenght, uint32_t uwOffset);
static uint8_t Buffercmp(uint32_t* pBuffer1, uint32_t* pBuffer2, uint16_t BufferLength);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  SDRAM Demo
  * @param  None
  * @retval None
  */
void SDRAM_demo (void)
{
  SDRAM_SetHint();

  /* SDRAM device configuration */
  if(BSP_SDRAM_Init(0) != BSP_ERROR_NONE)
  {
    UTIL_LCD_DisplayStringAt(20, 115, (uint8_t *)"SDRAM Initialization : FAILED.", LEFT_MODE);
    UTIL_LCD_DisplayStringAt(20, 130, (uint8_t *)"SDRAM Test Aborted.", LEFT_MODE);
  }
  else
  {
    UTIL_LCD_DisplayStringAt(20, 100, (uint8_t *)"SDRAM Initialization : OK.", LEFT_MODE);
  }
  /* Fill the buffer to write */
  Fill_Buffer(sdram_aTxBuffer, BUFFER_SIZE, 0x250F);

  /* Write data to the SDRAM memory */
  if(HAL_SDRAM_Write_32b(&hsdram[0], (uint32_t *)(SDRAM_WRITE_READ_ADDR + WRITE_READ_ADDR), (uint32_t*)sdram_aTxBuffer, BUFFER_SIZE) != BSP_ERROR_NONE)
  {
    UTIL_LCD_DisplayStringAt(20, 115, (uint8_t *)"SDRAM WRITE : FAILED.", LEFT_MODE);
    UTIL_LCD_DisplayStringAt(20, 130, (uint8_t *)"SDRAM Test Aborted.", LEFT_MODE);
  }
  else
  {
    UTIL_LCD_DisplayStringAt(20, 115, (uint8_t *)"SDRAM WRITE : OK.", LEFT_MODE);
  }

  /* Read back data from the SDRAM memory */
  if(HAL_SDRAM_Read_32b(&hsdram[0], (uint32_t *)(SDRAM_WRITE_READ_ADDR + WRITE_READ_ADDR), (uint32_t*)sdram_aRxBuffer, BUFFER_SIZE) != BSP_ERROR_NONE)
  {
    UTIL_LCD_DisplayStringAt(20, 130, (uint8_t *)"SDRAM READ : FAILED.", LEFT_MODE);
    UTIL_LCD_DisplayStringAt(20, 145, (uint8_t *)"SDRAM Test Aborted.", LEFT_MODE);
  }
  else
  {
    UTIL_LCD_DisplayStringAt(20, 130, (uint8_t *)"SDRAM READ : OK.", LEFT_MODE);
  }

  if(Buffercmp(sdram_aTxBuffer, sdram_aRxBuffer, BUFFER_SIZE) > 0)
  {
    UTIL_LCD_DisplayStringAt(20, 145, (uint8_t *)"SDRAM COMPARE : FAILED.", LEFT_MODE);
    UTIL_LCD_DisplayStringAt(20, 160, (uint8_t *)"SDRAM Test Aborted.", LEFT_MODE);
  }
  else
  {
    UTIL_LCD_DisplayStringAt(20, 145, (uint8_t *)"SDRAM Test : OK.", LEFT_MODE);
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
  * @brief  SDRAM DMA Demo
  * @param  None
  * @retval None
  */
void SDRAM_DMA_demo (void)
{

  SDRAM_DMA_SetHint();

  /* SDRAM device configuration */
  if(BSP_SDRAM_Init(0) != BSP_ERROR_NONE)
  {
    UTIL_LCD_DisplayStringAt(20, 115, (uint8_t *)"SDRAM Initialization : FAILED.", LEFT_MODE);
    UTIL_LCD_DisplayStringAt(20, 130, (uint8_t *)"SDRAM Test Aborted.", LEFT_MODE);
  }
  else
  {
    UTIL_LCD_DisplayStringAt(20, 100, (uint8_t *)"SDRAM Initialization : OK.", LEFT_MODE);
  }
  /* Fill the buffer to write */
  Fill_Buffer(sdram_aTxBuffer, BUFFER_SIZE, 0xA244250F);

  /* Clean Data Cache to update the content of the SRAM */
  SCB_CleanDCache_by_Addr((uint32_t*)sdram_aTxBuffer, BUFFER_SIZE*4);

  /* Write data to the SDRAM memory */
  if(HAL_SDRAM_Write_DMA(&hsdram[0], (uint32_t *)(SDRAM_WRITE_READ_ADDR + WRITE_READ_ADDR), sdram_aTxBuffer, BUFFER_SIZE) != BSP_ERROR_NONE)
  {
    UTIL_LCD_DisplayStringAt(20, 115, (uint8_t *)"SDRAM WRITE : FAILED.", LEFT_MODE);
    UTIL_LCD_DisplayStringAt(20, 130, (uint8_t *)"SDRAM Test Aborted.", LEFT_MODE);
  }
  else
  {
    UTIL_LCD_DisplayStringAt(20, 115, (uint8_t *)"SDRAM WRITE : OK.", LEFT_MODE);
  }

  /* Read back data from the SDRAM memory */
  if(HAL_SDRAM_Read_DMA(&hsdram[0], (uint32_t *)(SDRAM_WRITE_READ_ADDR + WRITE_READ_ADDR), sdram_aRxBuffer, BUFFER_SIZE) != BSP_ERROR_NONE)
  {
    UTIL_LCD_DisplayStringAt(20, 130, (uint8_t *)"SDRAM READ : FAILED.", LEFT_MODE);
    UTIL_LCD_DisplayStringAt(20, 145, (uint8_t *)"SDRAM Test Aborted.", LEFT_MODE);
  }
  else
  {
    UTIL_LCD_DisplayStringAt(20, 130, (uint8_t *)"SDRAM READ : OK.", LEFT_MODE);
  }

  /* Invalidate Data Cache to get the updated content of the SRAM*/
  SCB_InvalidateDCache_by_Addr((uint32_t *)sdram_aRxBuffer, BUFFER_SIZE*4);

  if(Buffercmp(sdram_aTxBuffer, sdram_aRxBuffer, BUFFER_SIZE) > 0)
  {
    UTIL_LCD_DisplayStringAt(20, 145, (uint8_t *)"SDRAM COMPARE : FAILED.", LEFT_MODE);
    UTIL_LCD_DisplayStringAt(20, 160, (uint8_t *)"SDRAM Test Aborted.", LEFT_MODE);
    BSP_LED_On(LED2);
  }
  else
  {
    UTIL_LCD_DisplayStringAt(20, 145, (uint8_t *)"SDRAM Test : OK.", LEFT_MODE);
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
  * @brief  Display SDRAM Demo Hint
  * @param  None
  * @retval None
  */
static void SDRAM_SetHint(void)
{
  uint32_t x_size, y_size;

  BSP_LCD_GetXSize(0, &x_size);
  BSP_LCD_GetYSize(0, &y_size);

  /* Clear the LCD */
  UTIL_LCD_Clear(UTIL_LCD_COLOR_WHITE);

  /* Set LCD Demo description */
  UTIL_LCD_FillRect(0, 0, x_size, 80, UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_SetFont(&Font24);
  UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"SDRAM", CENTER_MODE);
  UTIL_LCD_SetFont(&Font12);
  UTIL_LCD_DisplayStringAt(0, 30, (uint8_t *)"This example shows how to write", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, 45, (uint8_t *)"and read data on SDRAM", CENTER_MODE);

   /* Set the LCD Text Color */
  UTIL_LCD_DrawRect(10, 90, x_size - 20, y_size- 100, UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_DrawRect(11, 91, x_size - 22, y_size- 102, UTIL_LCD_COLOR_BLUE);

  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLACK);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
 }

/**
  * @brief  Display SDRAM DMA Demo Hint
  * @param  None
  * @retval None
  */
static void SDRAM_DMA_SetHint(void)
{
  uint32_t x_size, y_size;

  BSP_LCD_GetXSize(0, &x_size);
  BSP_LCD_GetYSize(0, &y_size);

  /* Clear the LCD */
  UTIL_LCD_Clear(UTIL_LCD_COLOR_WHITE);

  /* Set LCD Demo description */
  UTIL_LCD_FillRect(0, 0, x_size, 80, UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_SetFont(&Font24);
  UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"SDRAM DMA", CENTER_MODE);
  UTIL_LCD_SetFont(&Font12);
  UTIL_LCD_DisplayStringAt(0, 30, (uint8_t *)"This example shows how to write", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, 45, (uint8_t *)"and read data on SDRAM in DMA mode", CENTER_MODE);

   /* Set the LCD Text Color */
  UTIL_LCD_DrawRect(10, 90, x_size - 20, y_size- 100, UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_DrawRect(11, 91, x_size - 22, y_size- 102, UTIL_LCD_COLOR_BLUE);

  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLACK);
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
