/**
  ******************************************************************************
  * @file    BSP/CM7/Src/sram.c
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
#define BUFFER_SIZE         ((uint32_t)0x100)
#define WRITE_READ_ADDR     ((uint32_t)0x0800)
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Buffer location should aligned to cache line size (32 bytes) */
ALIGN_32BYTES (uint32_t sram_aRxBuffer[BUFFER_SIZE]);
ALIGN_32BYTES (uint32_t sram_aTxBuffer[BUFFER_SIZE]);
uint8_t ubSramWrite = 0, ubSramRead = 0, ubSramInit = 0;
/* DMA transfer complete flag */
__IO uint32_t uwMDMA_SRAM_Transfer_Complete = 0;
/* Private function prototypes -----------------------------------------------*/
static void SRAM_SetHint(void);
static void SRAM_DMA_SetHint(void);
static void Fill_Buffer(uint32_t *pBuffer, uint32_t uwBufferLength, uint32_t uwOffset);
static uint8_t Buffercmp(uint32_t* pBuffer1, uint32_t* pBuffer2, uint16_t BufferLength);
static void SRAM_DeactivateSharedPins(void);

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
  BSP_LCD_DisplayOff();

  /*##-1- Configure the SRAM device ##########################################*/
  /* SRAM device configuration */
  if(BSP_SRAM_Init() != SRAM_OK)
  {
    ubSramInit++;
  }

  /*##-2- SRAM memory read/write access ######################################*/
  /* Fill the buffer to write */
  Fill_Buffer(sram_aTxBuffer, BUFFER_SIZE, 0xA255C20F);
  /* Clear the buffer to read */
  Fill_Buffer(sram_aRxBuffer, BUFFER_SIZE, 0x0000);

  /* Write data to the SRAM memory */
  if(BSP_SRAM_WriteData(SRAM_DEVICE_ADDR + WRITE_READ_ADDR, (uint16_t *)sram_aTxBuffer, BUFFER_SIZE * 2) != SRAM_OK)
  {
    ubSramWrite++;
  }

  /* Read back data from the SRAM memory */
  if(BSP_SRAM_ReadData(SRAM_DEVICE_ADDR + WRITE_READ_ADDR, (uint16_t *)sram_aRxBuffer, BUFFER_SIZE * 2) != SRAM_OK)
  {
    ubSramRead++;
  }

  /*##-3- Checking data integrity ############################################*/
  /* Enable the LCD */
  BSP_LCD_DisplayOn();

  if(ubSramInit != 0)
  {
    BSP_LCD_DisplayStringAt(20, 100, (uint8_t *)"SRAM Initialization : FAILED.", LEFT_MODE);
    BSP_LCD_DisplayStringAt(20, 115, (uint8_t *)"SRAM Test Aborted.", LEFT_MODE);
  }
  else
  {
    BSP_LCD_DisplayStringAt(20, 100, (uint8_t *)"SRAM Initialization : OK.", LEFT_MODE);
  }
  if(ubSramWrite != 0)
  {
    BSP_LCD_DisplayStringAt(20, 115, (uint8_t *)"SRAM WRITE : FAILED.", LEFT_MODE);
    BSP_LCD_DisplayStringAt(20, 130, (uint8_t *)"SRAM Test Aborted.", LEFT_MODE);
  }
  else
  {
    BSP_LCD_DisplayStringAt(20, 115, (uint8_t *)"SRAM WRITE : OK.", LEFT_MODE);
  }
  if(ubSramRead != 0)
  {
    BSP_LCD_DisplayStringAt(20, 130, (uint8_t *)"SRAM READ : FAILED.", LEFT_MODE);
    BSP_LCD_DisplayStringAt(20, 145, (uint8_t *)"SRAM Test Aborted.", LEFT_MODE);
  }
  else
  {
    BSP_LCD_DisplayStringAt(20, 130, (uint8_t *)"SRAM READ : OK.", LEFT_MODE);
  }

  if(Buffercmp(sram_aRxBuffer, sram_aTxBuffer, BUFFER_SIZE) > 0)
  {
    BSP_LCD_DisplayStringAt(20, 145, (uint8_t *)"SRAM COMPARE : FAILED.", LEFT_MODE);
    BSP_LCD_DisplayStringAt(20, 160, (uint8_t *)"SRAM Test Aborted.", LEFT_MODE);
  }
  else
  {
    BSP_LCD_DisplayStringAt(20, 145, (uint8_t *)"SRAM Test : OK.", LEFT_MODE);
  }

  while (1)
  {
    if(CheckForUserInput() > 0)
    {
      SRAM_DeactivateSharedPins();
      return;
    }
  }
}

/**
  * @brief  SRAM Demo
  * @param  None
  * @retval None
  */
void SRAM_DMA_demo (void)
{
  SRAMTest = 1;
  SRAM_DMA_SetHint();

  /* -1- SDRAM device configuration */
  if (BSP_SRAM_Init() != SRAM_OK)
  {
    BSP_LCD_DisplayStringAt(20, 115, (uint8_t *)"SRAM Initialization : FAILED.", LEFT_MODE);
    BSP_LCD_DisplayStringAt(20, 130, (uint8_t *)"SRAM Test Aborted.", LEFT_MODE);
  }
  else
  {
    BSP_LCD_DisplayStringAt(20, 100, (uint8_t *)"SRAM Initialization : OK.", LEFT_MODE);
  }

  /*##-2- SRAM memory read/write access ######################################*/
  /* Fill the buffer to write */
  Fill_Buffer(sram_aTxBuffer, BUFFER_SIZE, 0xAC44A50F);
  /* Clear the buffer to read */
  Fill_Buffer(sram_aRxBuffer, BUFFER_SIZE, 0x0000);

  /* -3- Write data to the SDRAM memory using DMA */
  uwMDMA_SRAM_Transfer_Complete = 0;
  if(BSP_SRAM_WriteData_DMA(SRAM_DEVICE_ADDR + WRITE_READ_ADDR, (uint16_t *)sram_aTxBuffer, BUFFER_SIZE * 2) != SRAM_OK)
  {
    BSP_LCD_DisplayStringAt(20, 115, (uint8_t *)"SRAM WRITE dma : FAILED.", LEFT_MODE);
    BSP_LCD_DisplayStringAt(20, 130, (uint8_t *)"SRAM Test Aborted.", LEFT_MODE);
  }
  else
  {
    BSP_LCD_DisplayStringAt(20, 115, (uint8_t *)"SRAM WRITE DMA : REQUESTED", LEFT_MODE);
  }

  /* Wait until DMA transfer is complete */
  while (!uwMDMA_SRAM_Transfer_Complete)
  {}
  BSP_LCD_DisplayStringAt(20, 130, (uint8_t *)"SRAM WRITE DMA : COMPLETED", LEFT_MODE);

  /* -4- Read back data from the SDRAM memory using DMA */
  uwMDMA_SRAM_Transfer_Complete = 0;
  if(BSP_SRAM_ReadData_DMA(SRAM_DEVICE_ADDR + WRITE_READ_ADDR, (uint16_t *)sram_aRxBuffer, BUFFER_SIZE * 2) != SRAM_OK)
  {
    BSP_LCD_DisplayStringAt(20, 145, (uint8_t *)"SRAM  READ DMA : FAILED", LEFT_MODE);
    BSP_LCD_DisplayStringAt(20, 160, (uint8_t *)"SRAM  Test Aborted", LEFT_MODE);
  }
  else
  {
    BSP_LCD_DisplayStringAt(20, 145, (uint8_t *)"SRAM READ  DMA : REQUESTED", LEFT_MODE);
  }

  /* Wait until DMA transfer is complete */
  while (!uwMDMA_SRAM_Transfer_Complete)
  {}
  BSP_LCD_DisplayStringAt(20, 160, (uint8_t *)"SRAM READ  DMA : COMPLETED", LEFT_MODE);

  /* Invalidate Data Cache to get the updated content of the SRAM*/
  SCB_InvalidateDCache_by_Addr((uint32_t *)sram_aRxBuffer, BUFFER_SIZE*4);

  /* -5- Compare both buffers */
  if(Buffercmp(sram_aRxBuffer, sram_aTxBuffer, BUFFER_SIZE) > 0)
  {
    BSP_LCD_DisplayStringAt(20, 175, (uint8_t *)"SRAM COMPARE : FAILED.", LEFT_MODE);
    BSP_LCD_DisplayStringAt(20, 190, (uint8_t *)"SRAM Test Aborted.", LEFT_MODE);
  }
  else
  {
    BSP_LCD_DisplayStringAt(20, 175, (uint8_t *)"SRAM TEST      : SUCCESSFULL", LEFT_MODE);
  }

  while (1)
  {
    if (CheckForUserInput() > 0)
    {
      SRAMTest = 0;
      SRAM_DeactivateSharedPins();
      return;
    }
  }
}

/**
  * @brief  DMA conversion complete callback
  * @note   This function is executed when the transfer complete interrupt
  *         is generated
  * @retval None
  */
void HAL_SRAM_DMA_XferCpltCallback(MDMA_HandleTypeDef *hmdma)
{
  /* Set transfer complete flag */
  uwMDMA_SRAM_Transfer_Complete = 1;
}

/**
  * @brief  DMA transfer complete error callback.
  * @param  hdma: DMA handle
  * @retval None
  */

void HAL_SRAM_DMA_XferErrorCallback(MDMA_HandleTypeDef *hmdma)
{
  BSP_LCD_SetTextColor(LCD_COLOR_RED);
  BSP_LCD_DisplayStringAt(20, 215, (uint8_t *)"MDMA ERROR", LEFT_MODE);
}

/**
  * @brief  Display SRAM Demo Hint
  * @param  None
  * @retval None
  */
static void SRAM_SetHint(void)
{
  /* Clear the LCD */
  BSP_LCD_Clear(LCD_COLOR_WHITE);

  /* Set LCD Demo description */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), 80);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
  BSP_LCD_SetFont(&Font24);
  BSP_LCD_DisplayStringAt(0, 0, (uint8_t *)"SRAM", CENTER_MODE);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, 30, (uint8_t *)"This example shows how to write", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 45, (uint8_t *)"and read data on the SRAM", CENTER_MODE);

   /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_DrawRect(10, 90, BSP_LCD_GetXSize() - 20, BSP_LCD_GetYSize()- 100);
  BSP_LCD_DrawRect(11, 91, BSP_LCD_GetXSize() - 22, BSP_LCD_GetYSize()- 102);

  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
}

/**
  * @brief  Display SRAM DMA Demo Hint
  * @param  None
  * @retval None
  */
static void SRAM_DMA_SetHint(void)
{
  /* Clear the LCD */
  BSP_LCD_Clear(LCD_COLOR_WHITE);

  /* Set LCD Demo description */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), 80);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
  BSP_LCD_SetFont(&Font24);
  BSP_LCD_DisplayStringAt(0, 0, (uint8_t *)"SRAM MDMA", CENTER_MODE);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, 30, (uint8_t *)"This example shows how to write", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 45, (uint8_t *)"and read data on the SRAM using MDMA mode", CENTER_MODE);

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
  * @param  uwBufferLength: size of the buffer to fill
  * @param  uwOffset: first value to fill on the buffer
  * @retval None
  */
static void Fill_Buffer(uint32_t *pBuffer, uint32_t uwBufferLength, uint32_t uwOffset)
{
  uint32_t tmpIndex = 0;

  /* Put in global buffer different values */
  for (tmpIndex = 0; tmpIndex < uwBufferLength; tmpIndex++ )
  {
    pBuffer[tmpIndex] = tmpIndex + uwOffset;
  }
  /* Clean Data Cache to update the content of the SRAM */
  SCB_CleanDCache_by_Addr((uint32_t*)pBuffer, uwBufferLength*4);
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
  * @brief  Deactivate shared Pins between SRAM and SAI.
  * @param  None
  * @retval None
  */
static void SRAM_DeactivateSharedPins(void)
{
  uint32_t pin;

  /* Shared Pin between SRAM and SAI Audio (GPIOE) */
  pin   = GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5;

  HAL_GPIO_DeInit(GPIOE, pin);
}

/**
  * @}
  */

/**
  * @}
  */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
