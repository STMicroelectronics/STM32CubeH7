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
typedef enum
{
  SD1_DETECTED,
  SD1_NOT_DETECTED,
  SD2_DETECTED,
  SD2_NOT_DETECTED
}SD_Detect;

SD_Detect SdDetection;

/* Private define ------------------------------------------------------------*/
#define BLOCK_START_ADDR         0     /* Block start address      */
#define NUM_OF_BLOCKS            5     /* Total number of blocks   */
#define BUFFER_WORDS_SIZE        ((BLOCKSIZE * NUM_OF_BLOCKS) >> 2) /* Total data size in bytes */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#if defined ( __ICCARM__ )
#pragma location = 0x24000000
#elif defined ( __CC_ARM )
__attribute__((section (".RAM_D1")))
#elif defined ( __GNUC__ )
__attribute__((section (".RAM_D1")))
#endif
uint32_t aTxBuffer[BUFFER_WORDS_SIZE];
#if defined ( __ICCARM__ )
#pragma location = 0x24001000
#elif defined ( __CC_ARM )
__attribute__((section (".RAM_D1")))
#elif defined ( __GNUC__ )
__attribute__((section (".RAM_D1")))
#endif
uint32_t aRxBuffer[BUFFER_WORDS_SIZE];
#if defined ( __ICCARM__ )
#pragma location = 0x24002000
#elif defined ( __CC_ARM )
__attribute__((section (".RAM_D1")))
#elif defined ( __GNUC__ )
__attribute__((section (".RAM_D1")))
#endif
uint32_t aRxBuffer2[BUFFER_WORDS_SIZE];
HAL_SD_CardInfoTypeDef CardInfo;
HAL_SD_CardInfoTypeDef CardInfo2;

extern SD_HandleTypeDef hSdHandle[SD_INSTANCES_NBR];

__IO uint32_t SD1WriteStatus = 0, SD1ReadStatus = 0, SD2WriteStatus = 0, SD2ReadStatus = 0;

/* Private function prototypes -----------------------------------------------*/
static void SD_SetHint(uint32_t Mode);
static void Fill_Buffer(uint32_t *pBuffer, uint32_t uwBufferLenght, uint32_t uwOffset);
static uint8_t Buffercmp(uint32_t* pBuffer1, uint32_t* pBuffer2, uint16_t BufferLength);
static void SD_demo(uint32_t Mode);
static uint8_t Wait_SDCARD_Ready(uint32_t Instance);
static void SD_ClockConfig(uint32_t Mode);

/* Private functions ---------------------------------------------------------*/

void SD_DMA_demo(void)
{
  SD_demo(SD_DMA_MODE);
}

void SD_IT_demo(void)
{
  SD_demo(SD_IT_MODE);
}

void SD_POLLING_demo(void)
{
  SD_demo(SD_POLLING_MODE);
}

/**
  * @brief  SD Demo
  * @param  Mode SD_DMA_MODE, SD_IT_MODE or SD_POLLING_MODE
  * @retval None
  */
static void SD_demo(uint32_t Mode)
{
  uint8_t SD1_state = BSP_ERROR_NONE;
  uint32_t x_size, y_size;

  BSP_LCD_GetXSize(0, &x_size);
  BSP_LCD_GetYSize(0, &y_size);

  SdmmcTest = 1;
  ButtonState = 0;
  SD_SetHint(Mode);
  SD_ClockConfig(Mode);
  BSP_SD_DetectITConfig(0);
  /* Check if the SD card is plugged in the slot */
  if(BSP_SD_IsDetected(0) == SD_NOT_PRESENT)
  {
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_RED);
    UTIL_LCD_DisplayStringAt(20, y_size - 30, (uint8_t *)"SD1 Not Connected", LEFT_MODE);
    UTIL_LCD_DisplayStringAt(20, 100, (uint8_t *)"Plug a SD card to CN28 connector", LEFT_MODE);
    UTIL_LCD_DisplayStringAt(20, 115, (uint8_t *)"Or press TAMPER button to quit test", LEFT_MODE);
  }
  while(BSP_SD_IsDetected(0) == SD_NOT_PRESENT)
  {
    if(ButtonState > 0)
    {
      SdmmcTest = 0;
      ButtonState = 0;
      return;
    }
  }

  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_GREEN);
  UTIL_LCD_DisplayStringAt(20, y_size - 30, (uint8_t *)" SD1 Connected    ", LEFT_MODE);
  UTIL_LCD_DisplayStringAt(20, 100, (uint8_t *)"                                ", LEFT_MODE);
  UTIL_LCD_DisplayStringAt(20, 115, (uint8_t *)"                                   ", LEFT_MODE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLACK);
  SD1_state = BSP_SD_Init(0);
  if(SD1_state == BSP_ERROR_NONE)
  {
    UTIL_LCD_DisplayStringAt(20, 100, (uint8_t *)"SD1 INITIALIZATION : OK.", LEFT_MODE);

    SD1_state = BSP_SD_Erase(0, BLOCK_START_ADDR, BLOCK_START_ADDR + NUM_OF_BLOCKS);
    /* Wait until SD card is ready to use for new operation */
    while(BSP_SD_GetCardState(0) != SD_TRANSFER_OK);

    if(SD1_state != BSP_ERROR_NONE)
    {
      UTIL_LCD_DisplayStringAt(20, 115, (uint8_t *)"SD1 ERASE : FAILED.", LEFT_MODE);
      UTIL_LCD_DisplayStringAt(20, 130, (uint8_t *)"SD1 Test Aborted.", LEFT_MODE);
    }
    else
    {
      UTIL_LCD_DisplayStringAt(20, 115, (uint8_t *)"SD1 ERASE : OK.", LEFT_MODE);

      /* Fill the buffer to write */
      Fill_Buffer(aTxBuffer, BUFFER_WORDS_SIZE, 0x22FF);
      SCB_CleanDCache_by_Addr((uint32_t*)aTxBuffer, BUFFER_WORDS_SIZE*4);
      if(Mode == SD_DMA_MODE)
      {
        SD1_state = BSP_SD_WriteBlocks_DMA(0, (uint32_t *)aTxBuffer, BLOCK_START_ADDR, NUM_OF_BLOCKS);
        /* Wait for the write process is completed */
        while((SD1WriteStatus == 0));
        SD1WriteStatus = 0;
      }
      else if(Mode == SD_IT_MODE)
      {
        SD1_state = BSP_SD_WriteBlocks_IT(0, (uint32_t *)aTxBuffer, BLOCK_START_ADDR, NUM_OF_BLOCKS);
        /* Wait for the write process is completed */
        while((SD1WriteStatus == 0));
        SD1WriteStatus = 0;
      }
      else
      {
        SD1_state = BSP_SD_WriteBlocks(0, (uint32_t *)aTxBuffer, BLOCK_START_ADDR, NUM_OF_BLOCKS);
      }

      /* Wait until SD cards are ready to use for new operation */
      while((BSP_SD_GetCardState(0) != SD_TRANSFER_OK));

      if(SD1_state != BSP_ERROR_NONE)
      {
        UTIL_LCD_DisplayStringAt(20, 130, (uint8_t *)"SD1 WRITE : FAILED.", LEFT_MODE);
        UTIL_LCD_DisplayStringAt(20, 145, (uint8_t *)"SD1 Test Aborted.", LEFT_MODE);
      }
      else
      {
        UTIL_LCD_DisplayStringAt(20, 130, (uint8_t *)"SD1 WRITE : OK.", LEFT_MODE);
        SCB_CleanDCache_by_Addr((uint32_t*)aRxBuffer, BUFFER_WORDS_SIZE*4);
        if(Mode == SD_DMA_MODE)
        {
          SD1_state = BSP_SD_ReadBlocks_DMA(0, (uint32_t *)aRxBuffer, BLOCK_START_ADDR, NUM_OF_BLOCKS);

          /* Wait for the read process is completed */
          while(SD1ReadStatus == 0);
          SD1ReadStatus = 0;
        }
        else if(Mode == SD_IT_MODE)
        {
          SD1_state = BSP_SD_ReadBlocks_IT(0, (uint32_t *)aRxBuffer, BLOCK_START_ADDR, NUM_OF_BLOCKS);
          /* Wait for the read process is completed */
          while(SD1ReadStatus == 0);
          SD1ReadStatus = 0;
        }
        else
        {
          SD1_state = BSP_SD_ReadBlocks(0, (uint32_t *)aRxBuffer, BLOCK_START_ADDR, NUM_OF_BLOCKS);
        }
        /* Wait until SD card is ready to use for new operation */
        while(BSP_SD_GetCardState(0) != SD_TRANSFER_OK);

        if(SD1_state != BSP_ERROR_NONE)
        {
          UTIL_LCD_DisplayStringAt(20, 145, (uint8_t *)"SD1 READ : FAILED.", LEFT_MODE);
          UTIL_LCD_DisplayStringAt(20, 160, (uint8_t *)"SD1 Test Aborted.", LEFT_MODE);
        }
        else
        {
          UTIL_LCD_DisplayStringAt(20, 145, (uint8_t *)"SD1 READ : OK.", LEFT_MODE);
          if(Buffercmp(aTxBuffer, aRxBuffer, BUFFER_WORDS_SIZE) > 0)
          {
            UTIL_LCD_DisplayStringAt(20, 160, (uint8_t *)"SD1 COMPARE : FAILED.", LEFT_MODE);
            UTIL_LCD_DisplayStringAt(20, 175, (uint8_t *)"SD1 Test Aborted.", LEFT_MODE);
          }
          else
          {
            UTIL_LCD_DisplayStringAt(20, 160, (uint8_t *)"SD1 COMPARE : OK.", LEFT_MODE);
          }
        }
      }
    }
  }

  while (1)
  {
    if(CheckForUserInput() > 0)
    {
      ButtonState = 0;
      BSP_SD_DeInit(0);
      return;
    }
  }
}

/**
  * @brief  BSP SD Callback.
  * @param  GPIO_Pin: Specifies the pin connected EXTI line
  * @retval None.
  */
void BSP_SD_DetectCallback(uint32_t Instance, uint32_t Status)
{
  uint32_t x_size, y_size;

  BSP_LCD_GetXSize(0, &x_size);
  BSP_LCD_GetYSize(0, &y_size);
  if(Instance == 0)
  {
    /* Check if the SD card is plugged in the slot */
    if(Status == SD_PRESENT)
    {
      UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_GREEN);
      UTIL_LCD_DisplayStringAt(20, y_size - 30, (uint8_t *)" SD1 Connected    ", LEFT_MODE);
    }
    else
    {
      UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_RED);
      UTIL_LCD_DisplayStringAt(20, y_size - 30, (uint8_t *)"SD1 Not Connected", LEFT_MODE);
    }
  }
}

/**
  * @brief  Configure SDMMC clock
  * @param  Mode SD_IT_MODE or SD_POLLING_MODE
  * @retval None
  */
static void SD_ClockConfig(uint32_t Mode)
{
  if(Mode != SD_DMA_MODE)
  {
    RCC_PeriphCLKInitTypeDef RCC_PeriphCLKInitStruct;
    /*  SDMMC Clk: (((24/24) * 200) / 2) / SDMMC_NSpeed_CLK_DIV = 25Mhz */
    RCC_PeriphCLKInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SDMMC;
    RCC_PeriphCLKInitStruct.SdmmcClockSelection = RCC_SDMMCCLKSOURCE_PLL2;
    RCC_PeriphCLKInitStruct.PLL2.PLL2RGE = RCC_PLL1VCIRANGE_2;
    RCC_PeriphCLKInitStruct.PLL2.PLL2M = 24;
    RCC_PeriphCLKInitStruct.PLL2.PLL2N = 200;
    RCC_PeriphCLKInitStruct.PLL2.PLL2FRACN = 0;
    RCC_PeriphCLKInitStruct.PLL2.PLL2P = 0;
    RCC_PeriphCLKInitStruct.PLL2.PLL2R = 2;
    RCC_PeriphCLKInitStruct.PLL2.PLL2Q = 0;
    RCC_PeriphCLKInitStruct.PLL2.PLL2VCOSEL = RCC_PLL2VCOMEDIUM;
    HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphCLKInitStruct);

    __HAL_RCC_SDMMC1_FORCE_RESET();
    __HAL_RCC_SDMMC1_RELEASE_RESET();
  }
}

/**
  * @brief  Display SD Demo Hint
  * @param  Mode SD_DMA_MODE, SD_IT_MODE or SD_POLLING_MODE
  * @retval None
  */
static void SD_SetHint(uint32_t Mode)
{
  uint32_t x_size, y_size;

  BSP_LCD_GetXSize(0, &x_size);
  BSP_LCD_GetYSize(0, &y_size);

  /* Clear the LCD */
  UTIL_LCD_Clear(UTIL_LCD_COLOR_WHITE);

  /* Set LCD Demo description */
  UTIL_LCD_FillRect(0, 0, x_size, 87, UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_SetFont(&Font24);
  if(Mode == SD_DMA_MODE)
  {
    UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"SD1: DMA MODE", CENTER_MODE);
  }
  else if(Mode == SD_IT_MODE)
  {
    UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"SD1: IT MODE", CENTER_MODE);
  }
  else
  {
    UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"SD1: POLLING MODE", CENTER_MODE);
  }
  UTIL_LCD_SetFont(&Font12);
  UTIL_LCD_DisplayStringAt(0, 30, (uint8_t *)"This example shows how to write", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, 45, (uint8_t *)"and read data on the microSD and also", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, 60, (uint8_t *)"how to detect the presence of the card", CENTER_MODE);

   /* Set the LCD Text Color */
  UTIL_LCD_DrawRect(10, 92, x_size - 20, y_size- 100, UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_DrawRect(11, 93, x_size - 22, y_size- 102, UTIL_LCD_COLOR_BLUE);

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
uint32_t i = 0;
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
  * @brief Tx Transfer completed callbacks
  * @param hsd: SD handle
  * @retval None
  */
void BSP_SD_WriteCpltCallback(uint32_t Instance)
{
  Wait_SDCARD_Ready(Instance);
  if(Instance == 0)
  {
    SD1WriteStatus = 1;
  }
  else
  {
    SD2WriteStatus = 1;
  }
}

/**
  * @brief Rx Transfer completed callbacks
  * @param hsd: SD handle
  * @retval None
  */
void BSP_SD_ReadCpltCallback(uint32_t Instance)
{
  Wait_SDCARD_Ready(Instance);
  SCB_InvalidateDCache_by_Addr((uint32_t*)aRxBuffer, BUFFER_WORDS_SIZE*4);
  if(Instance == 0)
  {
    SD1ReadStatus = 1;
  }
  else
  {
    SD2ReadStatus = 1;
  }
}

static uint8_t Wait_SDCARD_Ready(uint32_t Instance)
{
  uint32_t loop = 0x00100000U;

  /* Wait for the Erasing process is completed */
  /* Verify that SD card is ready to use after the Erase */
  while(loop > 0)
  {
    loop--;
    if(BSP_SD_GetCardState(Instance) == SD_TRANSFER_OK)
    {
        return HAL_OK;
    }
  }
  return HAL_ERROR;
}

/**
  * @}
  */

/**
  * @}
  */

