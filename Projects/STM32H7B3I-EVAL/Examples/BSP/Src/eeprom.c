/**
  ******************************************************************************
  * @file    BSP/Src/eeprom.c
  * @author  MCD Application Team
  * @brief   This example code shows how to manage I2C EEPROM memory
  *          ===================================================================
  *          Notes:
  *           - The I2C EEPROM memory (M24LR64) is available on separate daughter
  *             board ANT7-M24LR-A, which is provided with the STM32H7B3I-EVAL board.
  *             To use this driver with M24LR64, you have to connect
  *             the ANT7-M24LR-A to CN24 connector of STM32H7B3I-EVAL board.
  *          ===================================================================
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
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;
/* Private define ------------------------------------------------------------*/
#define EEPROM_FEATURES_NUM     3

#define EEPROM_WRITE_ADDRESS1   0x0
#define EEPROM_READ_ADDRESS1    0x0
/* Private macro -------------------------------------------------------------*/
#define countof(a) (sizeof(a) / sizeof(*(a)))
/* Private variables ---------------------------------------------------------*/
static uint8_t EepromFeature = 0;

char Tx1Buffer[] = "****STM32H7B3I EEPROM****STM32H7B3I EEPROM****STM32H7B3I EEPROM***STM32H7B3I EEPROM***READ AND WRITE****";
#define BUFFER_SIZE1            8192
#define BUFFER_SIZE  countof(Tx1Buffer)
char RxBuffer[BUFFER_SIZE] = {0};
char Rx1Buffer[BUFFER_SIZE1] = {0};
char Tx2Buffer[BUFFER_SIZE1] = {0};

/* Private function prototypes -----------------------------------------------*/
static void EEPROM_SetHint(void);
static void EEPROM_Show_Feature(uint8_t feature);
static TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);
static void FillBuffer(uint8_t* pBuffer1, uint16_t Index, uint16_t BufferLength);

extern __IO uint8_t NbLoop;

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  EEPROM Demo
  * @param  None
  * @retval None
  */
void EEPROM_demo (void)
{
  EEPROM_SetHint();
  EepromFeature = 0;

  EEPROM_Show_Feature(EepromFeature);

  while (1)
  {
    if(CheckForUserInput() > 0)
    {
      ButtonState = 0;
      if(EepromFeature < EEPROM_FEATURES_NUM)
      {
        EEPROM_Show_Feature(EepromFeature++);
      }
      else
      {
        ButtonState = 0;
        return;
      }
    }
  }
}

/**
  * @brief  Display EEPROM Demo Hint
  * @param  None
  * @retval None
  */
static void EEPROM_SetHint(void)
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
  UTIL_LCD_DisplayStringAt(0, 0, (uint8_t*)"EEPROM", CENTER_MODE);
  UTIL_LCD_SetFont(&Font12);
  UTIL_LCD_DisplayStringAt(0, 30, (uint8_t*)"This example shows the different", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, 45, (uint8_t*)"EEPROM Features, use TAMPER button", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, 60, (uint8_t*)"to start EEPROM data transfer", CENTER_MODE);

   /* Set the LCD Text Color */
  UTIL_LCD_DrawRect(10, 90, x_size - 20, y_size- 100, UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_DrawRect(11, 91, x_size - 22, y_size- 102, UTIL_LCD_COLOR_BLUE);
 }

/**
  * @brief  Show EEPROM Features
  * @param  feature : feature index
  * @retval None
  */
static void EEPROM_Show_Feature(uint8_t feature)
{
  char text[50];
  __IO TestStatus TransferStatus1 = FAILED;
  __IO uint32_t NumDataRead = 0;
  uint32_t x_size, y_size;

  BSP_LCD_GetXSize(0, &x_size);
  BSP_LCD_GetYSize(0, &y_size);

  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_FillRect(12, 92, x_size- 24, y_size - 104, UTIL_LCD_COLOR_WHITE);

  /* Initialize the EEPROM driver --------------------------------------------*/
  if (BSP_EEPROM_Init(0) != BSP_ERROR_NONE)
  {
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_RED);
    UTIL_LCD_DisplayStringAt(0, 115, (uint8_t*)"Initialization problem", CENTER_MODE);
    UTIL_LCD_DisplayStringAt(0, 130, (uint8_t*)"Check if HW connected or", CENTER_MODE);
    UTIL_LCD_DisplayStringAt(0, 145, (uint8_t*)"HW version not supported", CENTER_MODE);
    return;
  }

  switch (feature)
  {
  case 0:
    /* Read old parameter in EEPROM */
    if(BSP_EEPROM_IsDeviceReady(0) == BSP_ERROR_NONE)
    {
      /* Set the Number of data to be read */
      NumDataRead = BUFFER_SIZE;

      /* Read from EEPROM from EEPROM_READ_ADDRESS1 */
      if (BSP_EEPROM_ReadBuffer(0, (uint8_t*)RxBuffer, EEPROM_READ_ADDRESS1, NumDataRead) != BSP_ERROR_NONE)
      {
        UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_RED);
        UTIL_LCD_DisplayStringAt(0, 115, (uint8_t*)"Init issue at read old data", CENTER_MODE);
        UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLACK);
        UTIL_LCD_DisplayStringAt(0, 145, (uint8_t*)"Press again TAMPER button", CENTER_MODE);
        UTIL_LCD_DisplayStringAt(0, 160, (uint8_t*)"To write new data", CENTER_MODE);
        return;
      }
      UTIL_LCD_DisplayStringAt(0, 115, (uint8_t*)"String read", CENTER_MODE);
      UTIL_LCD_DisplayStringAt(0, 130, (uint8_t*)"in the current EEPROM selected:", CENTER_MODE);
      UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLUE);
      UTIL_LCD_DisplayStringAt(0, 160, (uint8_t*)RxBuffer, CENTER_MODE);
      UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLACK);
      UTIL_LCD_DisplayStringAt(0, 190, (uint8_t*)"Press TAMPER button", CENTER_MODE);
      UTIL_LCD_DisplayStringAt(0, 205, (uint8_t*)"To write new data", CENTER_MODE);
    }
    else
    {
      UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_RED);
      UTIL_LCD_DisplayStringAt(0, 115, (uint8_t*)"Problem to communicate", CENTER_MODE);
      UTIL_LCD_DisplayStringAt(0, 130, (uint8_t*)"with EEPROM", CENTER_MODE);
      UTIL_LCD_DisplayStringAt(0, 145, (uint8_t*)"Press again TAMPER button", CENTER_MODE);
    }

    break;

  case 1:
     /* Write new parameter in EEPROM */
    if(BSP_EEPROM_IsDeviceReady(0) == BSP_ERROR_NONE)
    {
      FillBuffer((uint8_t*)Tx2Buffer, 0x21, BUFFER_SIZE1);

      UTIL_LCD_DisplayStringAt(0, 115, (uint8_t*)"Writing data to memory ...", CENTER_MODE);
      /* First write in the memory followed by a read of the written data ----*/
      /* Write on EEPROM to EEPROM_WRITE_ADDRESS1 */
      if (BSP_EEPROM_WriteBuffer(0, (uint8_t*)Tx2Buffer, EEPROM_WRITE_ADDRESS1, BUFFER_SIZE1) != BSP_ERROR_NONE)
      {
        UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_RED);
        UTIL_LCD_DisplayStringAt(0, 115, (uint8_t*)"Init issue at write", CENTER_MODE);
        return;
      }

      /* Set the Number of data to be read */
      NumDataRead = (uint32_t)BUFFER_SIZE1;

      /* Read from I2C EEPROM from EEPROM_READ_ADDRESS1 */
      if (BSP_EEPROM_ReadBuffer(0, (uint8_t*)Rx1Buffer, EEPROM_READ_ADDRESS1, NumDataRead) != BSP_ERROR_NONE)
      {
        UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_RED);
        UTIL_LCD_DisplayStringAt(0, 115, (uint8_t*)"Init issue at read", CENTER_MODE);
        return;
      }

      /* Check if the data written to the memory is read correctly */
      TransferStatus1 = Buffercmp((uint8_t*)Tx2Buffer, (uint8_t*)Rx1Buffer, BUFFER_SIZE1);
      if(TransferStatus1 != FAILED)
      {
        UTIL_LCD_DisplayStringAt(0, 115, (uint8_t*)"String writes                  ", CENTER_MODE);
        UTIL_LCD_DisplayStringAt(0, 130, (uint8_t*)"in the current EEPROM selected:", CENTER_MODE);
        UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLUE);
        UTIL_LCD_DisplayStringAt(0, 160, (uint8_t*)"READ/WRITE SUCCEEDED", CENTER_MODE);
      }
      else
      {
        UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_RED);
        UTIL_LCD_DisplayStringAt(0, 115, (uint8_t*)"FAILED to write!", CENTER_MODE);
        UTIL_LCD_DisplayStringAt(0, 130, (uint8_t*)"Press TAMPER button to end test", CENTER_MODE);
      }
    }
    else
    {
      UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_RED);
      UTIL_LCD_DisplayStringAt(0, 115, (uint8_t*)"Problem to communicate", CENTER_MODE);
      UTIL_LCD_DisplayStringAt(0, 130, (uint8_t*)"again with EEPROM", CENTER_MODE);
      UTIL_LCD_DisplayStringAt(0, 145, (uint8_t*)"Press TAMPER button to end test", CENTER_MODE);
    }
    break;
  case 2:
    if(BSP_EEPROM_IsDeviceReady(0) == BSP_ERROR_NONE)
    {
      FillBuffer((uint8_t*)Tx2Buffer, 0xAA, EEPROM_PAGESIZE);

      for(uint32_t i = 0; i < 2048; i++)
      {
      /* First write in the memory followed by a read of the written data ----*/
      /* Write on EEPROM to EEPROM_WRITE_ADDRESS1 */
      if (BSP_EEPROM_WritePage(0, (uint8_t*)Tx2Buffer, i) != BSP_ERROR_NONE)
      {
        UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_RED);
        UTIL_LCD_DisplayStringAt(0, 115, (uint8_t*)"Init issue at write page", CENTER_MODE);
        return;
      }

      /* Read from I2C EEPROM from EEPROM_READ_ADDRESS1 */
      if (BSP_EEPROM_ReadPage(0, (uint8_t*)Rx1Buffer, i) != BSP_ERROR_NONE)
      {
        UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_RED);
        UTIL_LCD_DisplayStringAt(0, 115, (uint8_t*)"Init issue at read page", CENTER_MODE);
        return;
      }

      /* Check if the data written to the memory is read correctly */
      TransferStatus1 = Buffercmp((uint8_t*)Tx2Buffer, (uint8_t*)Rx1Buffer, EEPROM_PAGESIZE);
      if(TransferStatus1 != FAILED)
      {
        UTIL_LCD_DisplayStringAt(0, 115, (uint8_t*)"String writes", CENTER_MODE);
        UTIL_LCD_DisplayStringAt(0, 130, (uint8_t*)"in the current EEPROM selected:", CENTER_MODE);
        UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLUE);
        sprintf(text,"READ/WRITE PAGE %lu  SUCCEEDED", i);
        UTIL_LCD_DisplayStringAt(0, 160, (uint8_t*)text, CENTER_MODE);
      }
      else
      {
        UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_RED);
        sprintf(text,"READ/WRITE PAGE %lu  FAILED", i);
        UTIL_LCD_DisplayStringAt(0, 160, (uint8_t*)text, CENTER_MODE);
        UTIL_LCD_DisplayStringAt(0, 130, (uint8_t*)"Press TAMPER button to end test", CENTER_MODE);
      }
      }
    }
    else
    {
      UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_RED);
      UTIL_LCD_DisplayStringAt(0, 115, (uint8_t*)"Problem to communicate", CENTER_MODE);
      UTIL_LCD_DisplayStringAt(0, 130, (uint8_t*)"again with EEPROM", CENTER_MODE);
      UTIL_LCD_DisplayStringAt(0, 145, (uint8_t*)"Press TAMPER button to end test", CENTER_MODE);
    }

    /* Tx1Buffer to the EEPROM */
    if (BSP_EEPROM_WriteBuffer(0, (uint8_t*)Tx1Buffer, EEPROM_WRITE_ADDRESS1, BUFFER_SIZE) != BSP_ERROR_NONE)
    {
      UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_RED);
      UTIL_LCD_DisplayStringAt(0, 115, (uint8_t*)"Issue to write Tx1Buffer", CENTER_MODE);
    }
    break;
  }
}

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval PASSED: pBuffer1 identical to pBuffer2
  *         FAILED: pBuffer1 differs from pBuffer2
  */
static TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
  while(BufferLength--)
  {
    if(*pBuffer1 != *pBuffer2)
    {
      return FAILED;
    }

    pBuffer1++;
    pBuffer2++;
  }

  return PASSED;
}

static void FillBuffer(uint8_t* pBuffer1, uint16_t Index, uint16_t BufferLength)
{
  uint32_t i;
  for(i = 0; i < BufferLength; i++)
  {
    pBuffer1[i] = Index + i;
  }
}

/**
  * @}
  */
/**
  * @}
  */
