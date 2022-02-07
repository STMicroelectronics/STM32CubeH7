/**
  ******************************************************************************
  * @file    BSP/Src/octospi.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use the OSPI Driver
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
#define BUFFER_SIZE         ((uint32_t) 0x1000)
#define OPI_START_ADDRESS   7*MX25LM51245G_SECTOR_64K
#define TEST_VALUE_START    0x43
#define Xpos_START          20
#define Ypos_START          100
#define CHAR_LINE           24
/* Private macro -------------------------------------------------------------*/
#define NEXT_LINE  {Xpos = Xpos_START; Ypos += CHAR_LINE;}

/* Private variables ---------------------------------------------------------*/
uint8_t qspi_aTxBuffer[BUFFER_SIZE];
uint8_t qspi_aRxBuffer[BUFFER_SIZE];
char str1[50];
static BSP_OSPI_NOR_Info_t pOSPI_Info;
static uint16_t Xpos = 20, Ypos = 100;
BSP_OSPI_NOR_Init_t Flash;
uint8_t *QSPI_MMP_pointer;

BSP_OSPI_NOR_Interface_t InterfaceMode[2] =
{
  BSP_OSPI_NOR_SPI_MODE,
  BSP_OSPI_NOR_OPI_MODE
};

char* InterfaceModeText[2] =
{
  "OSPI_SPI_MODE",
  "OSPI_OPI_MODE"
};

/* Private function prototypes -----------------------------------------------*/
static void OSPI_SetHint(void);
static void OSPI_DTR_Mode(void);
static void OSPI_STR_Mode(void);
static void OSPI_STR_MM_Mode(void);
static void OSPI_DTR_MM_Mode(void);
static void    Fill_Buffer(uint8_t *pBuffer, uint32_t uwBufferLength, uint32_t uwOffset);
static uint8_t RMABuffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint32_t BufferLength, uint32_t *ErrorAd);

/* Private functions ---------------------------------------------------------*/
/**
* @brief  OSPI Demo
* @param  None
* @retval None
*/
void OSPI_demo (void)
{
  OSPI_SetHint();
  UTIL_LCD_SetFont(&Font20);

  /* Fill the buffer for write operation *************************************/
  Fill_Buffer(qspi_aTxBuffer, BUFFER_SIZE, TEST_VALUE_START);

  /*##-2- Read & check the OSPI info #######################################*/
  /* Initialize the structure */
  pOSPI_Info.FlashSize          = (uint32_t)0x00;
  pOSPI_Info.EraseSectorSize    = (uint32_t)0x00;
  pOSPI_Info.EraseSectorsNumber = (uint32_t)0x00;
  pOSPI_Info.ProgPageSize       = (uint32_t)0x00;
  pOSPI_Info.ProgPagesNumber    = (uint32_t)0x00;

  /* Read the OSPI memory info */
  if(BSP_OSPI_NOR_GetInfo(0, &pOSPI_Info) != BSP_ERROR_NONE)
  {
    UTIL_LCD_DisplayStringAt(Xpos, Ypos, (uint8_t*)"TEST Get INFO : FAILED", LEFT_MODE);
  }
  /* Test the correctness */
  else if((pOSPI_Info.FlashSize != 0x4000000) || (pOSPI_Info.EraseSectorSize != 0x10000)  ||
          (pOSPI_Info.ProgPageSize != 0x100)  || (pOSPI_Info.EraseSectorsNumber != 0x400) ||
          (pOSPI_Info.ProgPagesNumber != 262144))
  {
    UTIL_LCD_DisplayStringAt(Xpos, Ypos, (uint8_t*)"OSPI GET INFO : FAILED.", LEFT_MODE);
    UTIL_LCD_DisplayStringAt(Xpos, Ypos + 20, (uint8_t*)"OSPI Test Aborted.", LEFT_MODE);
  }
  else
  {
    OSPI_STR_Mode();
    OSPI_STR_MM_Mode();
    OSPI_DTR_Mode();
    OSPI_DTR_MM_Mode();
  }
  ButtonState = 0;
  while(1)
  {
    if(CheckForUserInput() > 0)
    {
      ButtonState = 0;
      return;
    }
  }
}


static void OSPI_STR_Mode(void)
{
  uint32_t Offset;
  uint32_t block_size = MX25LM51245G_SECTOR_64K;
  uint32_t j = 0, temp_value = 4*block_size;
  static uint32_t i = 0;
  uint32_t error_code = 0;

  BSP_OSPI_NOR_DeInit(0);

  /* OSPI device configuration */
  Flash.InterfaceMode = InterfaceMode[j];
  Flash.TransferRate  = BSP_OSPI_NOR_STR_TRANSFER;
  BSP_OSPI_NOR_Init(0, &Flash);

  /* Fill the buffer for write operation *************************************/
  Fill_Buffer(qspi_aTxBuffer, BUFFER_SIZE, TEST_VALUE_START);

  for(j = 0; j < 2; j++)
  {
    if(BSP_OSPI_NOR_ConfigFlash(0, Flash.InterfaceMode, Flash.TransferRate) != BSP_ERROR_NONE)
    {
      UTIL_LCD_DisplayStringAt(Xpos, Ypos, (uint8_t*)"Config flash Failed.", LEFT_MODE);
    }

    BSP_OSPI_NOR_GetStatus(0);

    for(i = 0; i < temp_value/block_size; i++)
    {
      if(BSP_OSPI_NOR_Erase_Block(0, OPI_START_ADDRESS + i*block_size, MX25LM51245G_ERASE_64K) != BSP_ERROR_NONE)
      {
        UTIL_LCD_DisplayStringAt(Xpos, Ypos, (uint8_t*)"Erase Failed.", LEFT_MODE);
      }
      while(BSP_OSPI_NOR_GetStatus(0) == BSP_ERROR_BUSY);
    }

    for(i = 0; i < (temp_value/BUFFER_SIZE); i++)
    {
      if(BSP_OSPI_NOR_Write(0, qspi_aTxBuffer, OPI_START_ADDRESS + i*BUFFER_SIZE, BUFFER_SIZE) != BSP_ERROR_NONE)
      {
        UTIL_LCD_DisplayStringAt(Xpos, Ypos, (uint8_t*)"WRITE : FAILED.", LEFT_MODE);
        NEXT_LINE
        UTIL_LCD_DisplayStringAt(Xpos, Ypos, (uint8_t*)"Test Aborted.", LEFT_MODE);
      }

      if(BSP_OSPI_NOR_Read(0, qspi_aRxBuffer, OPI_START_ADDRESS + i*BUFFER_SIZE, BUFFER_SIZE) != BSP_ERROR_NONE)
      {
        UTIL_LCD_DisplayStringAt(Xpos, Ypos, (uint8_t*)"Read : FAILED.", LEFT_MODE);
        NEXT_LINE
          UTIL_LCD_DisplayStringAt(Xpos, Ypos, (uint8_t*)"Test Aborted.", LEFT_MODE);
      }

      if(RMABuffercmp(qspi_aRxBuffer, qspi_aTxBuffer, (uint32_t)BUFFER_SIZE, &Offset) != 0)
      {
        error_code++;
      }
    }

    if(error_code == 0)
    {
      UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLACK);
      UTIL_LCD_DisplayStringAt(20, 100, (uint8_t*)"STR COMPARE:", LEFT_MODE);
      UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_GREEN);
      UTIL_LCD_DisplayStringAt(20, j*20 + 120, (uint8_t*)InterfaceModeText[j], LEFT_MODE);
    }
    else
    {
      UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_RED);
      UTIL_LCD_DisplayStringAt(20, j*20 + 120, (uint8_t*)InterfaceModeText[j], LEFT_MODE);
      error_code = 0;
    }
  }
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLACK);
}

static void OSPI_STR_MM_Mode(void)
{
  uint32_t Offset;
  uint32_t block_size = MX25LM51245G_SECTOR_64K;
  uint32_t j = 0, temp_value = 4*block_size;
  static uint32_t i = 0;
  uint32_t error_code = 0;

  BSP_OSPI_NOR_DeInit(0);

  /* OSPI device configuration */
  Flash.InterfaceMode = InterfaceMode[j];
  Flash.TransferRate  = BSP_OSPI_NOR_STR_TRANSFER;
  BSP_OSPI_NOR_Init(0, &Flash);

  /* Fill the buffer for write operation *************************************/
  Fill_Buffer(qspi_aTxBuffer, BUFFER_SIZE, TEST_VALUE_START);

  for(j = 0; j < 2; j++)
  {
    if(BSP_OSPI_NOR_ConfigFlash(0, Flash.InterfaceMode, Flash.TransferRate) != BSP_ERROR_NONE)
    {
      UTIL_LCD_DisplayStringAt(Xpos, Ypos, (uint8_t*)"Config flash Failed.", LEFT_MODE);
    }

    BSP_OSPI_NOR_GetStatus(0);

    for(i = 0; i < temp_value/block_size; i++)
    {
      if(BSP_OSPI_NOR_Erase_Block(0, OPI_START_ADDRESS + i*block_size, MX25LM51245G_ERASE_64K) != BSP_ERROR_NONE)
      {
        UTIL_LCD_DisplayStringAt(Xpos, Ypos, (uint8_t*)"Erase Failed.", LEFT_MODE);
      }
      while(BSP_OSPI_NOR_GetStatus(0) == BSP_ERROR_BUSY);
    }

    for(i = 0; i < (temp_value/BUFFER_SIZE); i++)
    {
      if(BSP_OSPI_NOR_Write(0, qspi_aTxBuffer, OPI_START_ADDRESS + i*BUFFER_SIZE, BUFFER_SIZE) != BSP_ERROR_NONE)
      {
        UTIL_LCD_DisplayStringAt(Xpos, Ypos, (uint8_t*)"WRITE : FAILED.", LEFT_MODE);
        NEXT_LINE
          UTIL_LCD_DisplayStringAt(Xpos, Ypos, (uint8_t*)"Test Aborted.", LEFT_MODE);
      }
    }

    BSP_OSPI_NOR_EnableMemoryMappedMode(0);

    QSPI_MMP_pointer = (uint8_t*)(0x90000000 + OPI_START_ADDRESS);
    for(i = 0; i < (temp_value/BUFFER_SIZE); i++)
    {
      for(int k = 0; k < BUFFER_SIZE; k++)
      {
        qspi_aRxBuffer[k] = *(QSPI_MMP_pointer++);
      }
      if(RMABuffercmp(qspi_aRxBuffer, qspi_aTxBuffer, (uint32_t)BUFFER_SIZE, &Offset) != 0)
      {
        error_code++;
      }
    }

    BSP_OSPI_NOR_DisableMemoryMappedMode(0);

    if(error_code == 0)
    {
      UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLACK);
      UTIL_LCD_DisplayStringAt(20, 160, (uint8_t*)"MM STR COMPARE:", LEFT_MODE);
      UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_GREEN);
      UTIL_LCD_DisplayStringAt(20, j*20 + 180, (uint8_t*)InterfaceModeText[j], LEFT_MODE);
    }
    else
    {
      UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_RED);
      UTIL_LCD_DisplayStringAt(20, j*20 + 180, (uint8_t*)InterfaceModeText[j], LEFT_MODE);
      error_code = 0;
    }
  }
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLACK);
}

static void OSPI_DTR_Mode(void)
{
  uint32_t Offset;
  uint32_t block_size = 65536;
  uint32_t i = 0, temp_value = 4*block_size;
  uint32_t error_code = 0;

  BSP_OSPI_NOR_DeInit(0);

  /* OSPI device configuration */
  Flash.InterfaceMode = BSP_OSPI_NOR_OPI_MODE;
  Flash.TransferRate  = BSP_OSPI_NOR_DTR_TRANSFER;
  BSP_OSPI_NOR_Init(0, &Flash);

  /* Fill the buffer for write operation *************************************/
  Fill_Buffer(qspi_aTxBuffer, BUFFER_SIZE, TEST_VALUE_START);

  for(i = 0; i < temp_value/block_size; i++)
  {
    if(BSP_OSPI_NOR_Erase_Block(0, i*block_size + OPI_START_ADDRESS, MX25LM51245G_ERASE_64K) != BSP_ERROR_NONE)
    {
      UTIL_LCD_DisplayStringAt(Xpos, Ypos, (uint8_t*)"Erase Failed.", LEFT_MODE);
    }
    while(BSP_OSPI_NOR_GetStatus(0) == BSP_ERROR_BUSY);
  }

  for(i = 0; i < (temp_value/BUFFER_SIZE); i++)
  {
    if(BSP_OSPI_NOR_Write(0, qspi_aTxBuffer, i*BUFFER_SIZE + OPI_START_ADDRESS, BUFFER_SIZE) != BSP_ERROR_NONE)
    {
      UTIL_LCD_DisplayStringAt(Xpos, Ypos, (uint8_t*)"WRITE : FAILED.", LEFT_MODE);
      NEXT_LINE
        UTIL_LCD_DisplayStringAt(Xpos, Ypos, (uint8_t*)"Test Aborted.", LEFT_MODE);
    }
    if(BSP_OSPI_NOR_Read(0, qspi_aRxBuffer, i*BUFFER_SIZE + OPI_START_ADDRESS, BUFFER_SIZE) != BSP_ERROR_NONE)
    {
      UTIL_LCD_DisplayStringAt(Xpos, Ypos, (uint8_t*)"Read : FAILED.", LEFT_MODE);
      NEXT_LINE
        UTIL_LCD_DisplayStringAt(Xpos, Ypos, (uint8_t*)"Test Aborted.", LEFT_MODE);
    }
    if(RMABuffercmp(qspi_aRxBuffer, qspi_aTxBuffer, (uint32_t)BUFFER_SIZE, &Offset) != 0)
    {
      error_code++;
    }
  }
  if(error_code == 0)
  {
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLACK);
    UTIL_LCD_DisplayStringAt(20, 100, (uint8_t*)"DTR COMPARE:", RIGHT_MODE);
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_GREEN);
    UTIL_LCD_DisplayStringAt(20, 120, (uint8_t*)InterfaceModeText[1], RIGHT_MODE);
  }
  else
  {
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_RED);
    UTIL_LCD_DisplayStringAt(20, 120, (uint8_t*)InterfaceModeText[1], RIGHT_MODE);
    error_code = 0;
  }

  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLACK);
}

static void OSPI_DTR_MM_Mode(void)
{
  uint32_t Offset;
  uint32_t block_size = 65536;
  uint32_t i = 0, temp_value = 4*block_size;
  uint32_t error_code = 0;

  BSP_OSPI_NOR_DeInit(0);

  /* OSPI device configuration */
  Flash.InterfaceMode = BSP_OSPI_NOR_OPI_MODE;
  Flash.TransferRate  = BSP_OSPI_NOR_DTR_TRANSFER;
  BSP_OSPI_NOR_Init(0, &Flash);

  /* Fill the buffer for write operation *************************************/
  Fill_Buffer(qspi_aTxBuffer, BUFFER_SIZE, TEST_VALUE_START);

  for(i = 0; i < temp_value/block_size; i++)
  {
    if(BSP_OSPI_NOR_Erase_Block(0, i*block_size + OPI_START_ADDRESS, MX25LM51245G_ERASE_64K) != BSP_ERROR_NONE)
    {
      UTIL_LCD_DisplayStringAt(Xpos, Ypos, (uint8_t*)"Erase Failed.", LEFT_MODE);
    }
    while(BSP_OSPI_NOR_GetStatus(0) == BSP_ERROR_BUSY);
  }

  for(i = 0; i < (temp_value/BUFFER_SIZE); i++)
  {
    if(BSP_OSPI_NOR_Write(0, qspi_aTxBuffer, i*BUFFER_SIZE + OPI_START_ADDRESS, BUFFER_SIZE) != BSP_ERROR_NONE)
    {
      UTIL_LCD_DisplayStringAt(Xpos, Ypos, (uint8_t*)"WRITE : FAILED.", LEFT_MODE);
      NEXT_LINE
        UTIL_LCD_DisplayStringAt(Xpos, Ypos, (uint8_t*)"Test Aborted.", LEFT_MODE);
    }
  }
    BSP_OSPI_NOR_EnableMemoryMappedMode(0);

    QSPI_MMP_pointer = (uint8_t*)(0x90000000 + OPI_START_ADDRESS);
    for(i = 0; i < (temp_value/BUFFER_SIZE); i++)
    {
      for(int k = 0; k < BUFFER_SIZE; k++)
      {
        qspi_aRxBuffer[k] = *(QSPI_MMP_pointer++);
      }
      if(RMABuffercmp(qspi_aRxBuffer, qspi_aTxBuffer, (uint32_t)BUFFER_SIZE, &Offset) != 0)
      {
        error_code++;
      }
    }

   BSP_OSPI_NOR_DisableMemoryMappedMode(0);
  if(error_code == 0)
  {
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLACK);
    UTIL_LCD_DisplayStringAt(20, 160, (uint8_t*)"MM DTR COMPARE:", RIGHT_MODE);
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_GREEN);
    UTIL_LCD_DisplayStringAt(20, 180, (uint8_t*)InterfaceModeText[1], RIGHT_MODE);
  }
  else
  {
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_RED);
    UTIL_LCD_DisplayStringAt(20, 180, (uint8_t*)InterfaceModeText[1], RIGHT_MODE);
    error_code = 0;
  }

  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLACK);
}

/**
* @brief  Display OSPI Demo Hint
* @param  None
* @retval None
*/
static void OSPI_SetHint(void)
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
  UTIL_LCD_DisplayStringAt(0, 0, (uint8_t*)"OSPI", CENTER_MODE);
  UTIL_LCD_SetFont(&Font12);
  UTIL_LCD_DisplayStringAt(0, 30, (uint8_t*)"This example shows how to write", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, 45, (uint8_t*)"and read data on OSPI memory", CENTER_MODE);

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
static void Fill_Buffer(uint8_t *pBuffer, uint32_t uwBufferLenght, uint32_t uwOffset)
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
*         ErrorAd: Difference address
* @retval 0: pBuffer identical to pBuffer1
*         1: pBuffer differs from pBuffer1
*/
static uint8_t RMABuffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint32_t BufferLength, uint32_t *ErrorAd)
{
  (*ErrorAd) = 0;

  while (BufferLength--)
  {
    if (*pBuffer1 != *pBuffer2)
    {
      return 1;
    }

    (*ErrorAd)++;
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
