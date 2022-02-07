/**
  ******************************************************************************
  * @file    BSP/Src/nor.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use the NOR Driver
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
extern NOR_HandleTypeDef norHandle;
/* Private define ------------------------------------------------------------*/
#define NOR_BUFFER_SIZE     ((uint32_t)1150)
#define NOR_WRITE_READ_ADDR     ((uint32_t)0x0800)
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint16_t nor_aTxBuffer[NOR_BUFFER_SIZE];
uint16_t nor_aRxBuffer[NOR_BUFFER_SIZE];
uint8_t ubIDStatus = 0, ubEraseStatus = 0, ubWriteStatus = 0, ubReadStatus = 0, ubInitStatus = 0;


/* Private function prototypes -----------------------------------------------*/
static void     NOR_SetHint(void);
static void     Fill_Buffer(uint16_t *pBuffer, uint32_t uwBufferLenght, uint32_t uwOffset);
static uint8_t  Buffercmp(uint16_t* pBuffer1, uint16_t* pBuffer2, uint16_t BufferLength);

#if (USE_HAL_NOR_REGISTER_CALLBACKS == 1)
static void TestNOR_MspInit_Cb(NOR_HandleTypeDef *norHandle);
static void TestNOR_MspDeInit_Cb(NOR_HandleTypeDef *norHandle);
BSP_NOR_Cb_t NorCallbacks = {TestNOR_MspInit_Cb, TestNOR_MspDeInit_Cb};
#endif
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  NOR Demo
  * @param  None
  * @retval None
  */
void NOR_demo (void)
{
  /* NOR IDs structure */
  static NOR_IDTypeDef pNOR_ID;

  NOR_SetHint();

  /*##-1- Configure the NOR device ###########################################*/
  /* NOR device configuration */
#if (USE_HAL_NOR_REGISTER_CALLBACKS == 1)
  BSP_NOR_RegisterMspCallbacks(0, &NorCallbacks);
#endif
  if(BSP_NOR_Init(0) != BSP_ERROR_NONE)
  {
    ubInitStatus++;
  }

  /*##-2- Read & check the NOR device IDs ####################################*/
  /* Initialize the ID structure */
  pNOR_ID.Manufacturer_Code = (uint16_t)0x00;
  pNOR_ID.Device_Code1 = (uint16_t)0x00;
  pNOR_ID.Device_Code2 = (uint16_t)0x00;
  pNOR_ID.Device_Code3 = (uint16_t)0x00;

  /* Read the NOR memory ID */
  BSP_NOR_ReadID(0, &pNOR_ID);

  /* Test the NOR ID correctness */
  if(pNOR_ID.Manufacturer_Code != (uint16_t)0x0089)
    ubIDStatus++;
  else if(pNOR_ID.Device_Code1 != (uint16_t)0x227E)
    ubIDStatus++;
  else if (pNOR_ID.Device_Code2 != (uint16_t)0x2221)
    ubIDStatus++;
  else if (pNOR_ID.Device_Code3 != (uint16_t)0x2201)
    ubIDStatus++;

  /*##-3- Erase NOR memory ###################################################*/
  /* Return to read mode */
  BSP_NOR_ReturnToReadMode(0);

  if(BSP_NOR_EraseBlock(0, NOR_WRITE_READ_ADDR) != BSP_ERROR_NONE)
  {
    ubEraseStatus++;
  }

  /* Read back data from the NOR memory */
  if(BSP_NOR_ReadData(0, NOR_WRITE_READ_ADDR, nor_aRxBuffer, NOR_BUFFER_SIZE) != BSP_ERROR_NONE)
  {
    ubReadStatus++;
  }
  /*##-4- NOR memory read/write access  ######################################*/
  /* Fill the buffer to write */
  Fill_Buffer(nor_aTxBuffer, NOR_BUFFER_SIZE, 0xD20F);

  /* Write data to the NOR memory */
  if(BSP_NOR_WriteData(0, NOR_WRITE_READ_ADDR, nor_aTxBuffer, NOR_BUFFER_SIZE) != BSP_ERROR_NONE)
  {
    ubWriteStatus++;
  }

  /* Read back data from the NOR memory */
  if(BSP_NOR_ReadData(0, NOR_WRITE_READ_ADDR, nor_aRxBuffer, NOR_BUFFER_SIZE) != BSP_ERROR_NONE)
  {
    ubReadStatus++;
  }

  /*##-5- Checking data integrity ############################################*/
  if(ubInitStatus != 0)
  {
    UTIL_LCD_DisplayStringAt(20, 145, (uint8_t*)"NOR Initialization : FAILED.", LEFT_MODE);
    UTIL_LCD_DisplayStringAt(20, 160, (uint8_t*)"NOR Test Aborted.", LEFT_MODE);
  }
  else
  {
    UTIL_LCD_DisplayStringAt(20, 145, (uint8_t*)"NOR Initialization : OK.", LEFT_MODE);
  }
  if(ubIDStatus != 0)
  {
    UTIL_LCD_DisplayStringAt(20, 160, (uint8_t*)"NOR Read ID : FAILED.", LEFT_MODE);
    UTIL_LCD_DisplayStringAt(20, 175, (uint8_t*)"NOR Test Aborted.", LEFT_MODE);
  }
  else
  {
    UTIL_LCD_DisplayStringAt(20, 160, (uint8_t*)"NOR Read ID : OK.", LEFT_MODE);
  }
  if(ubEraseStatus != 0)
  {
    UTIL_LCD_DisplayStringAt(20, 175, (uint8_t*)"NOR ERASE : FAILED.", LEFT_MODE);
    UTIL_LCD_DisplayStringAt(20, 190, (uint8_t*)"NOR Test Aborted.", LEFT_MODE);
  }
  else
  {
    UTIL_LCD_DisplayStringAt(20, 175, (uint8_t*)"NOR ERASE : OK.   ", LEFT_MODE);
  }
  if(ubWriteStatus != 0)
  {
    UTIL_LCD_DisplayStringAt(20, 190, (uint8_t*)"NOR WRITE : FAILED.", LEFT_MODE);
    UTIL_LCD_DisplayStringAt(20, 205, (uint8_t*)"NOR Test Aborted.", LEFT_MODE);
  }
  else
  {
    UTIL_LCD_DisplayStringAt(20, 190, (uint8_t*)"NOR WRITE : OK.     ", LEFT_MODE);
  }
  if(ubReadStatus != 0)
  {
    UTIL_LCD_DisplayStringAt(20, 205, (uint8_t*)"NOR READ : FAILED.", LEFT_MODE);
    UTIL_LCD_DisplayStringAt(20, 220, (uint8_t*)"NOR Test Aborted.", LEFT_MODE);
  }
  else
  {
    UTIL_LCD_DisplayStringAt(20, 205, (uint8_t*)"NOR READ :  OK.    ", LEFT_MODE);
  }
  if(Buffercmp(nor_aRxBuffer, nor_aTxBuffer, NOR_BUFFER_SIZE) > 0)
  {
    UTIL_LCD_DisplayStringAt(20, 220, (uint8_t*)"NOR COMPARE : FAILED.", LEFT_MODE);
    UTIL_LCD_DisplayStringAt(20, 235, (uint8_t*)"NOR Test Aborted.", LEFT_MODE);
  }
  else
  {
    UTIL_LCD_DisplayStringAt(20, 220, (uint8_t*)"NOR Test : OK.     ", LEFT_MODE);
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
#if (USE_HAL_NOR_REGISTER_CALLBACKS == 1)
static void TestNOR_MspDeInit_Cb(NOR_HandleTypeDef *norHandle)
{

}
/**
  * @brief  Initializes the NOR MSP.
  * @param  norHandle: NOR handle
  * @retval None
  */
static void TestNOR_MspInit_Cb(NOR_HandleTypeDef *norHandle)
{
  GPIO_InitTypeDef gpio_init_structure;

  /* Enable FMC clock */
  __HAL_RCC_FMC_CLK_ENABLE();

  /* Enable GPIOs clock */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();

  /* Common GPIO configuration */
  gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
  gpio_init_structure.Pull      = GPIO_PULLUP;
  gpio_init_structure.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;

    /* GPIOC configuration */
  gpio_init_structure.Alternate = GPIO_AF9_FMC;
  gpio_init_structure.Pin   = GPIO_PIN_6 | GPIO_PIN_7;
  HAL_GPIO_Init(GPIOC, &gpio_init_structure);

  /* GPIOD configuration */
  gpio_init_structure.Alternate = GPIO_AF12_FMC;
  gpio_init_structure.Pin   = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5 |\
                              GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 |\
                              GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
  HAL_GPIO_Init(GPIOD, &gpio_init_structure);

  /* GPIOE configuration */
  gpio_init_structure.Pin   = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 |\
                              GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 |\
                              GPIO_PIN_14 | GPIO_PIN_15;
  HAL_GPIO_Init(GPIOE, &gpio_init_structure);

  /* GPIOF configuration */
  gpio_init_structure.Pin   = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2| GPIO_PIN_3 | GPIO_PIN_4     |\
                              GPIO_PIN_5 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
  HAL_GPIO_Init(GPIOF, &gpio_init_structure);

  /* GPIOG configuration */
  gpio_init_structure.Pin   = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2| GPIO_PIN_3 | GPIO_PIN_4     |\
                              GPIO_PIN_5;
  HAL_GPIO_Init(GPIOG, &gpio_init_structure);
}
#endif

/**
  * @brief  Display NOR Demo Hint
  * @param  None
  * @retval None
  */
static void NOR_SetHint(void)
{
  uint32_t x_size, y_size;

  BSP_LCD_GetXSize(0, &x_size);
  BSP_LCD_GetYSize(0, &y_size);

  /* Clear the LCD */
  UTIL_LCD_Clear(UTIL_LCD_COLOR_WHITE);

  /* Set LCD Demo description */
  UTIL_LCD_FillRect(0, 0, x_size, 120, UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_SetFont(&Font24);
  UTIL_LCD_DisplayStringAt(0, 0, (uint8_t*)"NOR", CENTER_MODE);
  UTIL_LCD_SetFont(&Font12);
  UTIL_LCD_DisplayStringAt(0, 30, (uint8_t*)"This example shows how to write", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, 45, (uint8_t*)"and read data on NOR", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, 75, (uint8_t *)"Press TAMPER button for next menu          ", CENTER_MODE);

   /* Set the LCD Text Color */
  UTIL_LCD_DrawRect(10, 130, x_size - 20, y_size - 130, UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_DrawRect(11, 131, x_size - 22, y_size - 132, UTIL_LCD_COLOR_BLUE);

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
static void Fill_Buffer(uint16_t *pBuffer, uint32_t uwBufferLenght, uint32_t uwOffset)
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
static uint8_t Buffercmp(uint16_t* pBuffer1, uint16_t* pBuffer2, uint16_t BufferLength)
{
  uint32_t i=0;
  for(i=0; i< BufferLength; i++)
  {
    if (pBuffer1[i] != pBuffer2[i])
    {
      return 1;
    }
  }
  return 0;
}

/**
  * @}
  */

/**
  * @}
  */
