/**
  ******************************************************************************
  * @file    BSP/CM7/Src/qspi.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use the QSPI Driver
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
#define BUFFER_SIZE         ((uint32_t)0x0200)
#define WRITE_READ_ADDR     ((uint32_t)0x0050)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t qspi_aTxBuffer[BUFFER_SIZE];
uint8_t qspi_aRxBuffer[BUFFER_SIZE];

/* Private function prototypes -----------------------------------------------*/
static void     QSPI_SetHint(void);
static void     Fill_Buffer (uint8_t *pBuffer, uint32_t uwBufferLength, uint32_t uwOffset);
static uint8_t  Buffercmp   (uint8_t* pBuffer1, uint8_t* pBuffer2, uint32_t BufferLength);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  QSPI Demo
  * @param  None
  * @retval None
  */
void QSPI_demo (void)
{
  /* QSPI info structure */
  BSP_QSPI_Info_t pQSPI_Info;
  uint8_t status;

  QSPI_SetHint();

  /*##-1- Configure the QSPI device ##########################################*/
  /* QSPI device configuration */
  BSP_QSPI_Init_t init ;
  init.InterfaceMode=MT25TL01G_QPI_MODE;
  init.TransferRate= MT25TL01G_DTR_TRANSFER ;
  init.DualFlashMode= MT25TL01G_DUALFLASH_ENABLE;
  status = BSP_QSPI_Init(0,&init);

  if (status != BSP_ERROR_NONE)
  {
    UTIL_LCD_DisplayStringAt(20, 100, (uint8_t*)"QSPI Initialization : FAILED.", LEFT_MODE);
    UTIL_LCD_DisplayStringAt(20, 115, (uint8_t*)"QSPI Test Aborted.", LEFT_MODE);
    UTIL_LCD_DisplayStringAt(20, 145, (uint8_t*)"Check the hardware configuration :", LEFT_MODE);
    UTIL_LCD_DisplayStringAt(20, 160, (uint8_t*)"  refer to the UM of the board", LEFT_MODE);
    UTIL_LCD_DisplayStringAt(20, 175, (uint8_t*)"  for the hardware modifications", LEFT_MODE);
    UTIL_LCD_DisplayStringAt(20, 190, (uint8_t*)"  to connect the QSPI memory", LEFT_MODE);
  }

  else
  {
    UTIL_LCD_DisplayStringAt(20, 100, (uint8_t*)"QSPI Initialization : OK.", LEFT_MODE);

    /*##-2- Read & check the QSPI info #######################################*/
    /* Initialize the structure */
    pQSPI_Info.FlashSize          = (uint32_t)0x00;
    pQSPI_Info.EraseSectorSize    = (uint32_t)0x00;
    pQSPI_Info.EraseSectorsNumber = (uint32_t)0x00;
    pQSPI_Info.ProgPageSize       = (uint32_t)0x00;
    pQSPI_Info.ProgPagesNumber    = (uint32_t)0x00;

    /* Read the QSPI memory info */
    BSP_QSPI_GetInfo(0,&pQSPI_Info);

    /* Test the correctness */
    if((pQSPI_Info.FlashSize != 0x8000000) || (pQSPI_Info.EraseSectorSize != 0x2000)  ||
       (pQSPI_Info.ProgPageSize != 0x100)  || (pQSPI_Info.EraseSectorsNumber != 0x4000) ||
         (pQSPI_Info.ProgPagesNumber != 0x80000))
    {
      UTIL_LCD_DisplayStringAt(20, 115, (uint8_t*)"QSPI GET INFO : FAILED.", LEFT_MODE);
      UTIL_LCD_DisplayStringAt(20, 130, (uint8_t*)"QSPI Test Aborted.", LEFT_MODE);
    }
    else
    {
      UTIL_LCD_DisplayStringAt(20, 115, (uint8_t*)"QSPI GET INFO : OK.   ", LEFT_MODE);

      /*##-3- Erase QSPI memory ################################################*/
      if(BSP_QSPI_EraseBlock(0,WRITE_READ_ADDR,BSP_QSPI_ERASE_8K) != BSP_ERROR_NONE)
      {
        UTIL_LCD_DisplayStringAt(20, 130, (uint8_t*)"QSPI ERASE : FAILED.", LEFT_MODE);
        UTIL_LCD_DisplayStringAt(20, 145, (uint8_t*)"QSPI Test Aborted.", LEFT_MODE);
      }
      else
      {
        UTIL_LCD_DisplayStringAt(20, 130, (uint8_t*)"QSPI ERASE : OK.   ", LEFT_MODE);

        /*##-4- QSPI memory read/write access  #################################*/
        /* Fill the buffer to write */
        Fill_Buffer(qspi_aTxBuffer, BUFFER_SIZE, 0xD20F);

        /* Write data to the QSPI memory */
        if(BSP_QSPI_Write(0,qspi_aTxBuffer, WRITE_READ_ADDR, BUFFER_SIZE) != BSP_ERROR_NONE)
        {
          UTIL_LCD_DisplayStringAt(20, 145, (uint8_t*)"QSPI WRITE : FAILED.", LEFT_MODE);
          UTIL_LCD_DisplayStringAt(20, 160, (uint8_t*)"QSPI Test Aborted.", LEFT_MODE);
        }
        else
        {
          UTIL_LCD_DisplayStringAt(20, 145, (uint8_t*)"QSPI WRITE : OK.     ", LEFT_MODE);

          /* Read back data from the QSPI memory */
          if(BSP_QSPI_Read(0,qspi_aRxBuffer, WRITE_READ_ADDR, BUFFER_SIZE) != BSP_ERROR_NONE)
          {
            UTIL_LCD_DisplayStringAt(20, 160, (uint8_t*)"QSPI READ : FAILED.", LEFT_MODE);
            UTIL_LCD_DisplayStringAt(20, 175, (uint8_t*)"QSPI Test Aborted.", LEFT_MODE);
          }
          else
          {
            UTIL_LCD_DisplayStringAt(20, 160, (uint8_t*)"QSPI READ :  OK.    ", LEFT_MODE);

            /*##-5- Checking data integrity ############################################*/
            if(Buffercmp(qspi_aRxBuffer, qspi_aTxBuffer, BUFFER_SIZE) > 0)
            {
              UTIL_LCD_DisplayStringAt(20, 175, (uint8_t*)"QSPI COMPARE : FAILED.", LEFT_MODE);
              UTIL_LCD_DisplayStringAt(20, 190, (uint8_t*)"QSPI Test Aborted.", LEFT_MODE);
            }
            else
            {
              UTIL_LCD_DisplayStringAt(20, 175, (uint8_t*)"QSPI COMPARE : OK.     ", LEFT_MODE);
              /*##-6-Memory Mapped Mode ###############################################*/
             if(BSP_QSPI_EnableMemoryMappedMode(0)!=BSP_ERROR_NONE)
             {
               UTIL_LCD_DisplayStringAt(20, 190, (uint8_t*)"QSPI Memory Mapped Mode : FAILED.     ", LEFT_MODE);
               UTIL_LCD_DisplayStringAt(20, 190, (uint8_t*)"QSPI Test Aborted.", LEFT_MODE);
             }
             else
             {
               UTIL_LCD_DisplayStringAt(20, 190, (uint8_t*)"QSPI Memory Mapped Mode : OK.     ", LEFT_MODE);
               UTIL_LCD_DisplayStringAt(20, 175, (uint8_t*)"QSPI Test : OK.     ", LEFT_MODE);
             }
            }
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
  * @brief  Display QSPI Demo Hint
  * @param  None
  * @retval None
  */
static void QSPI_SetHint(void)
{
  uint32_t x_size;
  uint32_t y_size;
  BSP_LCD_GetXSize(0, &x_size);
  BSP_LCD_GetYSize(0, &y_size);
  /* Clear the LCD */
  UTIL_LCD_Clear(UTIL_LCD_COLOR_WHITE);

  /* Set LCD Demo description */
  UTIL_LCD_FillRect(0, 0, x_size, 80,UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_SetFont(&Font24);
  UTIL_LCD_DisplayStringAt(0, 0, (uint8_t*)"QSPI", CENTER_MODE);
  UTIL_LCD_SetFont(&Font12);
  UTIL_LCD_DisplayStringAt(0, 30, (uint8_t*)"This example shows how to write", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, 45, (uint8_t*)"and read data on QSPI memory", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, 60, (uint8_t*)"(Hardware modifications needed)", CENTER_MODE);

  /* Set the LCD Text Color */
  UTIL_LCD_DrawRect(10, 90, x_size - 20,y_size- 100,UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_DrawRect(11, 91, x_size - 22, y_size- 102,UTIL_LCD_COLOR_BLUE);

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
  * @retval 1: pBuffer identical to pBuffer1
  *         0: pBuffer differs from pBuffer1
  */
static uint8_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint32_t BufferLength)
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
