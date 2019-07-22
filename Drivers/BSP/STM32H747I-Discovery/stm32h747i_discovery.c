/**
  ******************************************************************************
  * @file    stm32h747i_discovery.c
  * @author  MCD Application Team
  * @brief   This file provides a set of firmware functions to manage LEDs,
  *          push-buttons, external SDRAM, external QSPI Flash,
  *          available on STM32H747I-Discovery board (MB1248) from 
  *          STMicroelectronics.
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
#include "stm32h747i_discovery.h"

/** @defgroup BSP BSP
  * @{
  */

/** @defgroup STM32H747I_DISCOVERY STM32H747I_DISCOVERY
  * @{
  */

/** @defgroup STM32H747I_DISCOVERY_LOW_LEVEL STM32H747I_DISCOVERY_LOW_LEVEL
  * @{
  */

/** @defgroup STM32H747I_DISCOVERY_LOW_LEVEL_Private_Defines Private Defines
  * @{
  */
/**
 * @brief STM32H747I Discovery BSP Driver version number V1.0.0
   */
#define __STM32H747I_DISCOVERY_BSP_VERSION_MAIN   (0x01) /*!< [31:24] main version */
#define __STM32H747I_DISCOVERY_BSP_VERSION_SUB1   (0x00) /*!< [23:16] sub1 version */
#define __STM32H747I_DISCOVERY_BSP_VERSION_SUB2   (0x00) /*!< [15:8]  sub2 version */
#define __STM32H747I_DISCOVERY_BSP_VERSION_RC     (0x00) /*!< [7:0]  release candidate */
#define __STM32H747I_DISCOVERY_BSP_VERSION        ((__STM32H747I_DISCOVERY_BSP_VERSION_MAIN << 24)\
                                                  |(__STM32H747I_DISCOVERY_BSP_VERSION_SUB1 << 16)\
                                                  |(__STM32H747I_DISCOVERY_BSP_VERSION_SUB2 << 8 )\
                                                  |(__STM32H747I_DISCOVERY_BSP_VERSION_RC))
/**
  * @}
  */


/** @defgroup STM32H747I_DISCOVERY_LOW_LEVEL_Private_Variables Private Variables
  * @{
  */

GPIO_TypeDef* GPIO_PORT[LEDn] = {LED1_GPIO_PORT,
                                 LED2_GPIO_PORT,
                                 LED3_GPIO_PORT,
                                 LED4_GPIO_PORT};

const uint32_t GPIO_PIN[LEDn] = {LED1_PIN,
                                 LED2_PIN,
                                 LED3_PIN,
                                 LED4_PIN};

GPIO_TypeDef* BUTTON_PORT[BUTTONn] = {WAKEUP_BUTTON_GPIO_PORT };

const uint16_t BUTTON_PIN[BUTTONn] = {WAKEUP_BUTTON_PIN };

const uint16_t BUTTON_IRQn[BUTTONn] = {WAKEUP_BUTTON_EXTI_IRQn };

GPIO_TypeDef* JOY_PORT[JOYn] =   {SEL_JOY_GPIO_PORT,
                                  DOWN_JOY_GPIO_PORT,
                                  LEFT_JOY_GPIO_PORT,
                                  RIGHT_JOY_GPIO_PORT,
                                  UP_JOY_GPIO_PORT};

const uint16_t JOY_PIN[JOYn] =   {SEL_JOY_PIN,
                                  DOWN_JOY_PIN,
                                  LEFT_JOY_PIN,
                                  RIGHT_JOY_PIN,
                                  UP_JOY_PIN};

const uint8_t JOY_IRQn[JOYn] =   {SEL_JOY_EXTI_IRQn,
                                  DOWN_JOY_EXTI_IRQn,
                                  LEFT_JOY_EXTI_IRQn,
                                  RIGHT_JOY_EXTI_IRQn,
                                  UP_JOY_EXTI_IRQn};

static I2C_HandleTypeDef heval_I2c = {0};

#if defined(BSP_USE_CMSIS_OS)
static osSemaphoreId BspI2cSemaphore = 0;
#endif

/**
  * @}
  */

/** @defgroup STM32H747I_DISCOVERY_LOW_LEVEL_Private_FunctionPrototypes Private FunctionPrototypes
  * @{
  */
static void     I2Cx_MspInit(void);
static void     I2Cx_Init(void);
static void     I2Cx_Write(uint8_t Addr, uint8_t Reg, uint8_t Value);
static uint8_t  I2Cx_Read(uint8_t Addr, uint8_t Reg);
static HAL_StatusTypeDef I2Cx_ReadMultiple(uint8_t Addr, uint16_t Reg, uint16_t MemAddSize, uint8_t *Buffer, uint16_t Length);
static HAL_StatusTypeDef I2Cx_WriteMultiple(uint8_t Addr, uint16_t Reg, uint16_t MemAddSize, uint8_t *Buffer, uint16_t Length);
static HAL_StatusTypeDef I2Cx_IsDeviceReady(uint16_t DevAddress, uint32_t Trials);
static void     I2Cx_Error(uint8_t Addr);

/* HDMI IO functions */
void            HDMI_IO_Init(void);
void            HDMI_IO_Delay(uint32_t Delay);
void            HDMI_IO_Write(uint8_t Addr, uint8_t Reg, uint8_t Value);
uint8_t         HDMI_IO_Read(uint8_t Addr, uint8_t Reg);

/* AUDIO IO functions */
void            AUDIO_IO_Init(void);
void            AUDIO_IO_DeInit(void);
void            AUDIO_IO_Write(uint8_t Addr, uint16_t Reg, uint16_t Value);
uint16_t        AUDIO_IO_Read(uint8_t Addr, uint16_t Reg);
void            AUDIO_IO_Delay(uint32_t Delay);

/* CAMERA IO functions */
void            CAMERA_IO_Init(void);
void            CAMERA_Delay(uint32_t Delay);
void            CAMERA_IO_Write(uint8_t Addr, uint16_t Reg, uint16_t Value);
uint16_t        CAMERA_IO_Read(uint8_t Addr, uint16_t Reg);

/* I2C EEPROM IO function */
void                EEPROM_IO_Init(void);
HAL_StatusTypeDef   EEPROM_IO_WriteData(uint16_t DevAddress, uint16_t MemAddress, uint8_t* pBuffer, uint32_t BufferSize);
HAL_StatusTypeDef   EEPROM_IO_ReadData(uint16_t DevAddress, uint16_t MemAddress, uint8_t* pBuffer, uint32_t BufferSize);
HAL_StatusTypeDef   EEPROM_IO_IsDeviceReady(uint16_t DevAddress, uint32_t Trials);

/* TouchScreen (TS) IO functions */
void     TS_IO_Init(void);
void     TS_IO_Write(uint8_t Addr, uint8_t Reg, uint8_t Value);
uint8_t  TS_IO_Read(uint8_t Addr, uint8_t Reg);
uint16_t TS_IO_ReadMultiple(uint8_t Addr, uint8_t Reg, uint8_t *Buffer, uint16_t Length);
void     TS_IO_WriteMultiple(uint8_t Addr, uint8_t Reg, uint8_t *Buffer, uint16_t Length);
void     TS_IO_Delay(uint32_t Delay);

/* LCD Display IO functions */
void OTM8009A_IO_Delay(uint32_t Delay);
/**
  * @}
  */

/** @defgroup STM32H747I_DISCOVERY_BSP_Exported_Functions BSP Exported Functions
  * @{
  */

/**
  * @brief  BSP Error Notification
  * @note   Defined as a weak function to be overwritten by the application.
  * @retval None
  */
__weak void BSP_ErrorNotify(void)
{
}

  /**
  * @brief  This method returns the STM32H747I Discovery BSP Driver revision
  * @retval version: 0xXYZR (8bits for each decimal, R for RC)
  */
uint32_t BSP_GetVersion(void)
{
  return __STM32H747I_DISCOVERY_BSP_VERSION;
}

/**
  * @brief  Configures LED GPIO.
  * @param  Led: LED to be configured.
  *          This parameter can be one of the following values:
  *            @arg  LED1
  *            @arg  LED2
  *            @arg  LED3
  *            @arg  LED4
  * @retval None
  */
void BSP_LED_Init(Led_TypeDef Led)
{
  GPIO_InitTypeDef  GPIO_InitStruct;
  
  /* Enable the GPIO_LED clock */
  LEDx_GPIO_CLK_ENABLE();
  
  /* Configure the GPIO_LED pin */
  GPIO_InitStruct.Pin = GPIO_PIN[Led];
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  
  HAL_GPIO_Init(GPIO_PORT[Led], &GPIO_InitStruct);
  
  /* By default, turn off LED */
  HAL_GPIO_WritePin(GPIO_PORT[Led], GPIO_PIN[Led], GPIO_PIN_SET);
}


/**
  * @brief  DeInit LEDs.
  * @param  Led: LED to be configured.
  *          This parameter can be one of the following values:
  *            @arg  LED1
  *            @arg  LED2
  *            @arg  LED3
  *            @arg  LED4
  * @note Led DeInit does not disable the GPIO clock
  * @retval None
  */
void BSP_LED_DeInit(Led_TypeDef Led)
{
    /* Turn off LED */
    HAL_GPIO_WritePin(GPIO_PORT[Led], GPIO_PIN[Led], GPIO_PIN_RESET);
    /* Configure the GPIO_LED pin */
    HAL_GPIO_DeInit(GPIO_PORT[Led], GPIO_PIN[Led]);
}

/**
  * @brief  Turns selected LED On.
  * @param  Led: LED to be set on
  *          This parameter can be one of the following values:
  *            @arg  LED1
  *            @arg  LED2
  *            @arg  LED3
  *            @arg  LED4
  * @retval None
  */
void BSP_LED_On(Led_TypeDef Led)
{
  HAL_GPIO_WritePin(GPIO_PORT[Led], GPIO_PIN[Led], GPIO_PIN_RESET);
}

/**
  * @brief  Turns selected LED Off.
  * @param  Led: LED to be set off
  *          This parameter can be one of the following values:
  *            @arg  LED1
  *            @arg  LED2
  *            @arg  LED3
  *            @arg  LED4
  * @retval None
  */
void BSP_LED_Off(Led_TypeDef Led)
{
  HAL_GPIO_WritePin(GPIO_PORT[Led], GPIO_PIN[Led], GPIO_PIN_SET);
}

/**
  * @brief  Toggles the selected LED.
  * @param  Led: LED to be toggled
  *          This parameter can be one of the following values:
  *            @arg  LED1
  *            @arg  LED2
  *            @arg  LED3
  *            @arg  LED4
  * @retval None
  */
void BSP_LED_Toggle(Led_TypeDef Led)
{
  HAL_GPIO_TogglePin(GPIO_PORT[Led], GPIO_PIN[Led]);
}

/**
  * @brief  Configures button GPIO and EXTI Line.
  * @param  Button: Button to be configured
  *          This parameter can be one of the following values:
  *            @arg  BUTTON_WAKEUP: Wakeup Push Button
  *            @arg  BUTTON_USER: User Push Button
  * @param  Button_Mode: Button mode
  *          This parameter can be one of the following values:
  *            @arg  BUTTON_MODE_GPIO: Button will be used as simple IO
  *            @arg  BUTTON_MODE_EXTI: Button will be connected to EXTI line
  *                                    with interrupt generation capability
  * @retval None
  */
void BSP_PB_Init(Button_TypeDef Button, ButtonMode_TypeDef Button_Mode)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  /* Enable the BUTTON clock */
  BUTTON_GPIO_CLK_ENABLE();
  
  if(Button_Mode == BUTTON_MODE_GPIO)
  {
    /* Configure Button pin as input */
    GPIO_InitStruct.Pin = BUTTON_PIN[Button];
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(BUTTON_PORT[Button], &GPIO_InitStruct);
  }
  
  if(Button_Mode == BUTTON_MODE_EXTI)
  {
    /* Configure Button pin as input with External interrupt */
    GPIO_InitStruct.Pin = BUTTON_PIN[Button];
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    
    HAL_GPIO_Init(BUTTON_PORT[Button], &GPIO_InitStruct);
    
    /* Enable and set Button EXTI Interrupt to the lowest priority */
    HAL_NVIC_SetPriority((IRQn_Type)(BUTTON_IRQn[Button]), 0x0F, 0x00);
    HAL_NVIC_EnableIRQ((IRQn_Type)(BUTTON_IRQn[Button]));
  }
}

/**
  * @brief  Push Button DeInit.
  * @param  Button: Button to be configured
  *          This parameter can be one of the following values:
  *            @arg  BUTTON_WAKEUP: Wakeup Push Button
  *            @arg  BUTTON_USER: User Push Button
  * @note PB DeInit does not disable the GPIO clock
  * @retval None
  */
void BSP_PB_DeInit(Button_TypeDef Button)
{
    GPIO_InitTypeDef gpio_init_structure;

    gpio_init_structure.Pin = BUTTON_PIN[Button];
    HAL_NVIC_DisableIRQ((IRQn_Type)(BUTTON_IRQn[Button]));
    HAL_GPIO_DeInit(BUTTON_PORT[Button], gpio_init_structure.Pin);
}


/**
  * @brief  Returns the selected button state.
  * @param  Button: Button to be checked
  *          This parameter can be one of the following values:
  *            @arg  BUTTON_WAKEUP: Wakeup Push Button
  *            @arg  BUTTON_USER: User Push Button
  * @retval The Button GPIO pin value
  */
uint32_t BSP_PB_GetState(Button_TypeDef Button)
{
  return HAL_GPIO_ReadPin(BUTTON_PORT[Button], BUTTON_PIN[Button]);
}

/**
  * @brief  Configures all joystick's buttons in GPIO or EXTI modes.
  * @param  Joy_Mode: Joystick mode.
  *    This parameter can be one of the following values:
  *     @arg  JOY_MODE_GPIO: Joystick pins will be used as simple IOs
  *     @arg  JOY_MODE_EXTI: Joystick pins will be connected to EXTI line
  *                                 with interrupt generation capability
  * @retval HAL_OK: if all initializations are OK. Other value if error.
  */
uint8_t BSP_JOY_Init(JOYMode_TypeDef Joy_Mode)
{
  JOYState_TypeDef joykey;
  GPIO_InitTypeDef GPIO_InitStruct;

  /* Initialized the Joystick. */
  for(joykey = JOY_SEL; joykey < (JOY_SEL + JOYn) ; joykey++)
  {
    /* Enable the JOY clock */
    JOYx_GPIO_CLK_ENABLE(joykey);

    GPIO_InitStruct.Pin = JOY_PIN[joykey];
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

    if (Joy_Mode == JOY_MODE_GPIO)
    {
      /* Configure Joy pin as input */
      GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
      HAL_GPIO_Init(JOY_PORT[joykey], &GPIO_InitStruct);
    }
    else if (Joy_Mode == JOY_MODE_EXTI)
    {
      /* Configure Joy pin as input with External interrupt */
      GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
      HAL_GPIO_Init(JOY_PORT[joykey], &GPIO_InitStruct);

      /* Enable and set Joy EXTI Interrupt to the lowest priority */
      HAL_NVIC_SetPriority((IRQn_Type)(JOY_IRQn[joykey]), 0x0F, 0x00);
      HAL_NVIC_EnableIRQ((IRQn_Type)(JOY_IRQn[joykey]));
    }
  }

  return HAL_OK;
}

/**
  * @brief  Un-configures all GPIOs used as joystick's buttons.
  * @retval None.
  */
void BSP_JOY_DeInit(void)
{
  JOYState_TypeDef joykey;

  /* Initialized the Joystick. */
  for(joykey = JOY_SEL; joykey < (JOY_SEL + JOYn) ; joykey++)
  {
    /* Enable the JOY clock */
    JOYx_GPIO_CLK_ENABLE(joykey);

    HAL_GPIO_DeInit(JOY_PORT[joykey], JOY_PIN[joykey]);
  }
}

/**
* @brief  Returns the current joystick status.
* @retval Code of the joystick key pressed
*          This code can be one of the following values:
*            @arg  JOY_NONE
*            @arg  JOY_SEL
*            @arg  JOY_DOWN
*            @arg  JOY_LEFT
*            @arg  JOY_RIGHT
*            @arg  JOY_UP
*/
JOYState_TypeDef BSP_JOY_GetState(void)
{
  JOYState_TypeDef joykey;

  for (joykey = JOY_SEL; joykey < (JOY_SEL + JOYn) ; joykey++)
  {
    if (HAL_GPIO_ReadPin(JOY_PORT[joykey], JOY_PIN[joykey]) == GPIO_PIN_RESET)
    {
      /* Return Code Joystick key pressed */
      return joykey;
    }
  }

  /* No Joystick key pressed */
  return JOY_NONE;
}

/**
  * @}
  */

/** @defgroup STM32H747I_DISCOVERY_LOW_LEVEL_Private_Functions Private Functions
  * @{
  */
/*******************************************************************************
                            BUS OPERATIONS
*******************************************************************************/

/******************************* I2C Routines *********************************/
/**
  * @brief  Initializes I2C MSP.
  * @retval None
  */
static void I2Cx_MspInit(void)
{
  GPIO_InitTypeDef  gpio_init_structure;
  RCC_PeriphCLKInitTypeDef  RCC_PeriphClkInit;
  
  /* Configure the I2C clock source */
  RCC_PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C123;
  RCC_PeriphClkInit.I2c123ClockSelection = RCC_I2C123CLKSOURCE_D2PCLK1;
  HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInit);
  
  /* set STOPWUCK in RCC_CFGR */
  __HAL_RCC_WAKEUPSTOP_CLK_CONFIG(RCC_STOP_WAKEUPCLOCK_HSI);
  
  /*** Configure the GPIOs ***/  
  /* Enable GPIO clock */
  DISCOVERY_I2Cx_SCL_SDA_GPIO_CLK_ENABLE();
  
  /* Configure I2C Tx as alternate function */
  gpio_init_structure.Pin = DISCOVERY_I2Cx_SCL_PIN;
  gpio_init_structure.Mode = GPIO_MODE_AF_OD;
  gpio_init_structure.Pull = GPIO_NOPULL;
  gpio_init_structure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  gpio_init_structure.Alternate = DISCOVERY_I2Cx_SCL_SDA_AF;
  HAL_GPIO_Init(DISCOVERY_I2Cx_SCL_SDA_GPIO_PORT, &gpio_init_structure);
  
  /* Configure I2C Rx as alternate function */
  gpio_init_structure.Pin = DISCOVERY_I2Cx_SDA_PIN;
  HAL_GPIO_Init(DISCOVERY_I2Cx_SCL_SDA_GPIO_PORT, &gpio_init_structure);
  
  /*** Configure the I2C peripheral ***/ 
  /* Enable I2C clock */
  DISCOVERY_I2Cx_CLK_ENABLE();
  
  /* Force the I2C peripheral clock reset */  
  DISCOVERY_I2Cx_FORCE_RESET(); 
  
  /* Release the I2C peripheral clock reset */  
  DISCOVERY_I2Cx_RELEASE_RESET(); 
  
  /* Enable and set I2Cx Interrupt to a lower priority */
  HAL_NVIC_SetPriority(DISCOVERY_I2Cx_EV_IRQn, 0x0F, 0);
  HAL_NVIC_EnableIRQ(DISCOVERY_I2Cx_EV_IRQn);
  
  /* Enable and set I2Cx Interrupt to a lower priority */
  HAL_NVIC_SetPriority(DISCOVERY_I2Cx_ER_IRQn, 0x0F, 0);
  HAL_NVIC_EnableIRQ(DISCOVERY_I2Cx_ER_IRQn);
}

/**
  * @brief  Initializes I2C HAL.
  * @retval None
  */
static void I2Cx_Init(void)
{
  if(HAL_I2C_GetState(&heval_I2c) == HAL_I2C_STATE_RESET)
  {
#if defined(BSP_USE_CMSIS_OS)
    if(BspI2cSemaphore == NULL)
    {
      /* Create semaphore to prevent multiple I2C access */
      osSemaphoreDef(BSP_I2C_SEM);
      BspI2cSemaphore = osSemaphoreCreate(osSemaphore(BSP_I2C_SEM), 1);
    }
#endif

    heval_I2c.Instance              = DISCOVERY_I2Cx;
    heval_I2c.Init.Timing           = DISCOVERY_I2Cx_TIMING;
    heval_I2c.Init.OwnAddress1      = 0x72;
    heval_I2c.Init.AddressingMode   = I2C_ADDRESSINGMODE_7BIT;
    heval_I2c.Init.DualAddressMode  = I2C_DUALADDRESS_ENABLE;
    heval_I2c.Init.OwnAddress2      = 0;
    heval_I2c.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
    heval_I2c.Init.GeneralCallMode  = I2C_GENERALCALL_ENABLE;
    heval_I2c.Init.NoStretchMode    = I2C_NOSTRETCH_DISABLE;
    
    /* Init the I2C */
    I2Cx_MspInit();
    HAL_I2C_Init(&heval_I2c);    
  }
}

/**
  * @brief  Writes a single data.
  * @param  Addr: I2C address
  * @param  Reg: Register address 
  * @param  Value: Data to be written
  * @retval None
  */
static void I2Cx_Write(uint8_t Addr, uint8_t Reg, uint8_t Value)
{
  HAL_StatusTypeDef status = HAL_OK;

#if defined(BSP_USE_CMSIS_OS)
  /* Get semaphore to prevent multiple I2C access */
  osSemaphoreWait(BspI2cSemaphore, osWaitForever);
#endif

  status = HAL_I2C_Mem_Write(&heval_I2c, Addr, (uint16_t)Reg, I2C_MEMADD_SIZE_8BIT, &Value, 1, 100); 

#if defined(BSP_USE_CMSIS_OS)
  /* Release semaphore to prevent multiple I2C access */
  osSemaphoreRelease(BspI2cSemaphore);
#endif

  /* Check the communication status */
  if(status != HAL_OK)
  {
    /* Execute user timeout callback */
    I2Cx_Error(Addr);
  }
}

/**
  * @brief  Reads a single data.
  * @param  Addr: I2C address
  * @param  Reg: Register address
  * @retval Read data
  */
static uint8_t I2Cx_Read(uint8_t Addr, uint8_t Reg)
{
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t Value = 0;

#if defined(BSP_USE_CMSIS_OS)
  /* Get semaphore to prevent multiple I2C access */
  osSemaphoreWait(BspI2cSemaphore, osWaitForever);
#endif

  status = HAL_I2C_Mem_Read(&heval_I2c, Addr, Reg, I2C_MEMADD_SIZE_8BIT, &Value, 1, 1000);

#if defined(BSP_USE_CMSIS_OS)
  /* Release semaphore to prevent multiple I2C access */
  osSemaphoreRelease(BspI2cSemaphore);
#endif

  /* Check the communication status */
  if(status != HAL_OK)
  {
    /* Execute user timeout callback */
    I2Cx_Error(Addr);
  }
  return Value;
}

/**
  * @brief  Reads multiple data.
  * @param  Addr: I2C address
  * @param  Reg: Reg address
  * @param  MemAddress: memory address
  * @param  Buffer: Pointer to data buffer
  * @param  Length: Length of the data
  * @retval HAL status
  */
static HAL_StatusTypeDef I2Cx_ReadMultiple(uint8_t Addr, uint16_t Reg, uint16_t MemAddress, uint8_t *Buffer, uint16_t Length)
{
  HAL_StatusTypeDef status = HAL_OK;

#if defined(BSP_USE_CMSIS_OS)
  /* Get semaphore to prevent multiple I2C access */
  osSemaphoreWait(BspI2cSemaphore, osWaitForever);
#endif

  status = HAL_I2C_Mem_Read(&heval_I2c, Addr, (uint16_t)Reg, MemAddress, Buffer, Length, 1000);

#if defined(BSP_USE_CMSIS_OS)
  /* Release semaphore to prevent multiple I2C access */
  osSemaphoreRelease(BspI2cSemaphore);
#endif

  /* Check the communication status */
  if(status != HAL_OK)
  {
    /* I2C error occurred */
    I2Cx_Error(Addr);
  }
  return status;
}

/**
  * @brief  Writes a value in a register of the device through BUS in using DMA mode.
  * @param  Addr: Device address on BUS Bus.  
  * @param  Reg: The target register address to write
  * @param  MemAddress: memory address  
  * @param  Buffer: The target register value to be written 
  * @param  Length: buffer size to be written
  * @retval HAL status
  */
static HAL_StatusTypeDef I2Cx_WriteMultiple(uint8_t Addr, uint16_t Reg, uint16_t MemAddress, uint8_t *Buffer, uint16_t Length)
{
  HAL_StatusTypeDef status = HAL_OK;

#if defined(BSP_USE_CMSIS_OS)
  /* Get semaphore to prevent multiple I2C access */
  osSemaphoreWait(BspI2cSemaphore, osWaitForever);
#endif

  status = HAL_I2C_Mem_Write(&heval_I2c, Addr, (uint16_t)Reg, MemAddress, Buffer, Length, 1000);

#if defined(BSP_USE_CMSIS_OS)
  /* Release semaphore to prevent multiple I2C access */
  osSemaphoreRelease(BspI2cSemaphore);
#endif

  /* Check the communication status */
  if(status != HAL_OK)
  {
    /* Re-Initiaize the I2C Bus */
    I2Cx_Error(Addr);
  }
  return status;
}

/**
  * @brief  Checks if target device is ready for communication. 
  * @note   This function is used with Memory devices
  * @param  DevAddress: Target device address
  * @param  Trials: Number of trials
  * @retval HAL status
  */
static HAL_StatusTypeDef I2Cx_IsDeviceReady(uint16_t DevAddress, uint32_t Trials)
{
  HAL_StatusTypeDef status = HAL_OK;

#if defined(BSP_USE_CMSIS_OS)
  /* Get semaphore to prevent multiple I2C access */
  osSemaphoreWait(BspI2cSemaphore, osWaitForever);
#endif

  status = HAL_I2C_IsDeviceReady(&heval_I2c, DevAddress, Trials, 1000);

#if defined(BSP_USE_CMSIS_OS)
  /* Release semaphore to prevent multiple I2C access */
  osSemaphoreRelease(BspI2cSemaphore);
#endif

  return status;
}

/**
  * @brief  Manages error callback by re-initializing I2C.
  * @param  Addr: I2C Address
  * @retval None
  */
static void I2Cx_Error(uint8_t Addr)
{
  BSP_ErrorNotify();

  /* De-initialize the I2C comunication bus */
  HAL_I2C_DeInit(&heval_I2c);

  /* Re-Initialize the I2C communication bus */
  I2Cx_Init();

#if defined(BSP_USE_CMSIS_OS)
  /* Release semaphore to prevent multiple I2C access */
  osSemaphoreRelease(BspI2cSemaphore);
#endif
}

/*******************************************************************************
                            LINK OPERATIONS
*******************************************************************************/

/********************************* LINK AUDIO *********************************/

/**
  * @brief  Initializes Audio low level.
  * @retval None
  */
void AUDIO_IO_Init(void) 
{
  I2Cx_Init();
}

/**
  * @brief  De-Initializes Audio low level.
  * @retval None
  */
void AUDIO_IO_DeInit(void)
{
}

/**
  * @brief  Writes a single data.
  * @param  Addr: I2C address
  * @param  Reg: Reg address 
  * @param  Value: Data to be written
  * @retval None
  */
void AUDIO_IO_Write(uint8_t Addr, uint16_t Reg, uint16_t Value)
{
  uint16_t tmp = Value;
  
  Value = ((uint16_t)(tmp >> 8) & 0x00FF);
  
  Value |= ((uint16_t)(tmp << 8)& 0xFF00);
  
  I2Cx_WriteMultiple(Addr, Reg, I2C_MEMADD_SIZE_16BIT,(uint8_t*)&Value, 2);
}

/**
  * @brief  Reads a single data.
  * @param  Addr: I2C address
  * @param  Reg: Reg address 
  * @retval Data to be read
  */
uint16_t AUDIO_IO_Read(uint8_t Addr, uint16_t Reg)
{
  uint16_t read_value = 0, tmp = 0;
  
  I2Cx_ReadMultiple(Addr, Reg, I2C_MEMADD_SIZE_16BIT, (uint8_t*)&read_value, 2); 
  
  tmp = ((uint16_t)(read_value >> 8) & 0x00FF);
  
  tmp |= ((uint16_t)(read_value << 8)& 0xFF00);
  
  read_value = tmp;
  
  return read_value;
}

/**
  * @brief  AUDIO Codec delay
  * @param  Delay: Delay in ms
  * @retval None
  */
void AUDIO_IO_Delay(uint32_t Delay)
{
  HAL_Delay(Delay);
}

/********************************* LINK CAMERA ********************************/

/**
  * @brief  Initializes Camera low level.
  * @retval None
  */
void CAMERA_IO_Init(void) 
{
  I2Cx_Init();
}

/**
  * @brief  Camera writes single data.
  * @param  Addr: I2C address
  * @param  Reg: Register address 
  * @param  Value: Data to be written
  * @retval None
  */
void CAMERA_IO_Write(uint8_t Addr, uint16_t Reg, uint16_t Value)
{
  
  I2Cx_WriteMultiple(Addr, (uint16_t)Reg, I2C_MEMADD_SIZE_8BIT,(uint8_t*)&Value, 1);
  
}

/**
  * @brief  Camera reads single data.
  * @param  Addr: I2C address
  * @param  Reg: Register address 
  * @retval Read data
  */
uint16_t CAMERA_IO_Read(uint8_t Addr, uint16_t Reg)
{
  
  uint16_t Read_Value = 0;
  
  I2Cx_ReadMultiple(Addr, Reg, I2C_MEMADD_SIZE_8BIT, (uint8_t*)&Read_Value, 1);
  
  return Read_Value;
  
}

/**
  * @brief  Camera delay 
  * @param  Delay: Delay in ms
  * @retval None
  */
void CAMERA_Delay(uint32_t Delay)
{
  HAL_Delay(Delay);
}

/******************************** LINK I2C EEPROM *****************************/

/**
  * @brief  Initializes peripherals used by the I2C EEPROM driver.
  * @retval None
  */
void EEPROM_IO_Init(void)
{
  I2Cx_Init();
}

/**
  * @brief  Write data to I2C EEPROM driver in using DMA channel.
  * @param  DevAddress: Target device address
  * @param  MemAddress: Internal memory address
  * @param  pBuffer: Pointer to data buffer
  * @param  BufferSize: Amount of data to be sent
  * @retval HAL status
  */
HAL_StatusTypeDef EEPROM_IO_WriteData(uint16_t DevAddress, uint16_t MemAddress, uint8_t* pBuffer, uint32_t BufferSize)
{
  return (I2Cx_WriteMultiple(DevAddress, MemAddress, I2C_MEMADD_SIZE_16BIT, pBuffer, BufferSize));
}

/**
  * @brief  Read data from I2C EEPROM driver in using DMA channel.
  * @param  DevAddress: Target device address
  * @param  MemAddress: Internal memory address
  * @param  pBuffer: Pointer to data buffer
  * @param  BufferSize: Amount of data to be read
  * @retval HAL status
  */
HAL_StatusTypeDef EEPROM_IO_ReadData(uint16_t DevAddress, uint16_t MemAddress, uint8_t* pBuffer, uint32_t BufferSize)
{
  return (I2Cx_ReadMultiple(DevAddress, MemAddress, I2C_MEMADD_SIZE_16BIT, pBuffer, BufferSize));
}

/**
  * @brief  Checks if target device is ready for communication. 
  * @note   This function is used with Memory devices
  * @param  DevAddress: Target device address
  * @param  Trials: Number of trials
  * @retval HAL status
  */
HAL_StatusTypeDef EEPROM_IO_IsDeviceReady(uint16_t DevAddress, uint32_t Trials)
{ 
  return (I2Cx_IsDeviceReady(DevAddress, Trials));
}

/******************************** LINK TS (TouchScreen) *****************************/

/**
  * @brief  Initialize I2C communication
  *         channel from MCU to TouchScreen (TS).
  */
void TS_IO_Init(void)
{
  I2Cx_Init();
}

/**
  * @brief  Writes single data with I2C communication
  *         channel from MCU to TouchScreen.
  * @param  Addr: I2C address
  * @param  Reg: Register address
  * @param  Value: Data to be written
  */
void TS_IO_Write(uint8_t Addr, uint8_t Reg, uint8_t Value)
{
  I2Cx_Write(Addr, Reg, Value);
}

/**
  * @brief  Reads single data with I2C communication
  *         channel from TouchScreen.
  * @param  Addr: I2C address
  * @param  Reg: Register address
  * @retval Read data
  */
uint8_t TS_IO_Read(uint8_t Addr, uint8_t Reg)
{
  return I2Cx_Read(Addr, Reg);
}

/**
  * @brief  Reads multiple data with I2C communication
  *         channel from TouchScreen.
  * @param  Addr: I2C address
  * @param  Reg: Register address
  * @param  Buffer: Pointer to data buffer
  * @param  Length: Length of the data
  * @retval Number of read data
  */
uint16_t TS_IO_ReadMultiple(uint8_t Addr, uint8_t Reg, uint8_t *Buffer, uint16_t Length)
{
 return I2Cx_ReadMultiple(Addr, (uint16_t)Reg, I2C_MEMADD_SIZE_8BIT, Buffer, Length);
}

/**
  * @brief  Writes multiple data with I2C communication
  *         channel from MCU to TouchScreen.
  * @param  Addr: I2C address
  * @param  Reg: Register address
  * @param  Buffer: Pointer to data buffer
  * @param  Length: Length of the data
  * @retval None
  */
void TS_IO_WriteMultiple(uint8_t Addr, uint8_t Reg, uint8_t *Buffer, uint16_t Length)
{
  I2Cx_WriteMultiple(Addr, (uint16_t)Reg, I2C_MEMADD_SIZE_8BIT, Buffer, Length);
}

/**
  * @brief  Delay function used in TouchScreen low level driver.
  * @param  Delay: Delay in ms
  * @retval None
  */
void TS_IO_Delay(uint32_t Delay)
{
  HAL_Delay(Delay);
}

/**************************** LINK OTM8009A (Display driver) ******************/
/**
  * @brief  OTM8009A delay
  * @param  Delay: Delay in ms
  */
void OTM8009A_IO_Delay(uint32_t Delay)
{
  HAL_Delay(Delay);
}

#if defined(USE_LCD_HDMI) 
/**************************** LINK ADV7533 DSI-HDMI (Display driver) **********/
/**
  * @brief  Initializes HDMI IO low level.
  * @retval None
  */
void HDMI_IO_Init(void)
{
  I2Cx_Init();
}

/**
  * @brief  HDMI writes single data.
  * @param  Addr: I2C address
  * @param  Reg: Register address 
  * @param  Value: Data to be written
  * @retval None
  */
void HDMI_IO_Write(uint8_t Addr, uint8_t Reg, uint8_t Value)
{
  I2Cx_Write(Addr, Reg, Value);
}

/**
  * @brief  Reads single data with I2C communication
  *         channel from HDMI bridge.
  * @param  Addr: I2C address
  * @param  Reg: Register address
  * @retval Read data
  */
uint8_t HDMI_IO_Read(uint8_t Addr, uint8_t Reg)
{
  return I2Cx_Read(Addr, Reg);
}

/**
  * @brief  HDMI delay 
  * @param  Delay: Delay in ms
  * @retval None
  */
void HDMI_IO_Delay(uint32_t Delay)
{
  HAL_Delay(Delay);
}
#endif /* USE_LCD_HDMI */

/**
  * @}
  */

/**
  * @}
  */
/**
  * @}
  */

/**
  * @}
  */
  
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
