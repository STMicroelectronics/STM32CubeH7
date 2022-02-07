/**
  ******************************************************************************
  * @file    adafruit_802_conf.h
  * @author  MCD Application Team
  * @brief   This file includes the nucleo configuration file that contains definitions for:
  *          - ADC information(GPIOs and configuration) used for Joystick
  *          - SPI information(GPIOs and configuration) used for microSD and LCD available
  *            on Adafruit 1.8" TFT LCD shield (reference ID 802)
  *
  *
  *   There are two ways to use this file:
  *   1.
  *    - Copy stm32_adafruit_conf_template.h to the application and rename it to stm32_adafruit_conf.h
  *    - Fill stm32_adafruit_conf.h with adequate resources definition that can be directly
  *      copied from "Adafruit_Config" folder depending on the used nucleo board.
  *    - Un-comment and update the below lines with correct nucleo files
  *
  *   2.
  *    - Copy the required configuration file from "Adafruit_Config" folder to the
  *      application and rename it to stm32_adafruit_conf.h
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef ADAFRUIT_802_CONF_H
#define ADAFRUIT_802_CONF_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
 #include "stm32h7xx_nucleo_conf.h"
 #include "stm32h7xx_nucleo_errno.h"
 #include "stm32h7xx_nucleo_bus.h"
/** @addtogroup BSP
  * @{
  */

/** @addtogroup ADAFRUIT_802
  * @{
  */

/** @defgroup ADAFRUIT_802_CONFIG Config
  * @{
  */
#define BUS_SPIx_Init           BSP_SPI1_Init
#define BUS_SPIx_Recv           BSP_SPI1_Recv
#define BUS_SPIx_Send           BSP_SPI1_Send
#define BUS_SPIx_SendRecv       BSP_SPI1_SendRecv

/**
  * @brief  ADC Interface pins
  *         used to detect motion of Joystick available on Adafruit 1.8" TFT shield
  */
#if defined (USE_NUCLEO_H745ZI_Q) || defined (USE_NUCLEO_H743ZI2) || defined (USE_NUCLEO_H7A3ZI)
#define ADAFRUIT_802_ADCx                      ADC1
#define ADAFRUIT_802_ADCx_CLK_ENABLE()         __HAL_RCC_ADC12_CLK_ENABLE()
#define ADAFRUIT_802_ADCx_CLK_DISABLE()        __HAL_RCC_ADC12_CLK_DISABLE()
#define ADAFRUIT_802_ADCx_CHANNEL              ADC_CHANNEL_5

#define ADAFRUIT_802_ADCx_GPIO_PORT            GPIOB
#define ADAFRUIT_802_ADCx_GPIO_PIN             GPIO_PIN_1
#define ADAFRUIT_802_ADCx_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOB_CLK_ENABLE()
#define ADAFRUIT_802_ADCx_GPIO_CLK_DISABLE()   __HAL_RCC_GPIOB_CLK_DISABLE()
#else /* USE_NUCLEO_H743ZI */
#define ADAFRUIT_802_ADCx                      ADC3
#define ADAFRUIT_802_ADCx_CLK_ENABLE()         __HAL_RCC_ADC3_CLK_ENABLE()
#define ADAFRUIT_802_ADCx_CLK_DISABLE()        __HAL_RCC_ADC3_CLK_DISABLE()
#define ADAFRUIT_802_ADCx_CHANNEL              ADC_CHANNEL_5

#define ADAFRUIT_802_ADCx_GPIO_PORT            GPIOF
#define ADAFRUIT_802_ADCx_GPIO_PIN             GPIO_PIN_3
#define ADAFRUIT_802_ADCx_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOF_CLK_ENABLE()
#define ADAFRUIT_802_ADCx_GPIO_CLK_DISABLE()   __HAL_RCC_GPIOF_CLK_DISABLE()
#endif

#define ADAFRUIT_802_ADCx_RANK                 ADC_REGULAR_RANK_1
#define ADAFRUIT_802_ADCx_SAMPLETIME           ADC_SAMPLETIME_2CYCLES_5
#define ADAFRUIT_802_ADCx_PRESCALER            ADC_CLOCKPRESCALER_PCLK_DIV4
#define ADAFRUIT_802_ADCx_POLL_TIMEOUT         10U

/**
  * @brief  SD Control Interface pins (shield D4)
  */
#define ADAFRUIT_802_SD_CS_PIN                               GPIO_PIN_14
#if defined (USE_NUCLEO_H745ZI_Q) || defined (USE_NUCLEO_H743ZI2) || defined (USE_NUCLEO_H7A3ZI)
#define ADAFRUIT_802_SD_CS_GPIO_PORT                         GPIOE
#define ADAFRUIT_802_SD_CS_GPIO_CLK_ENABLE()                 __HAL_RCC_GPIOE_CLK_ENABLE()
#define ADAFRUIT_802_SD_CS_GPIO_CLK_DISABLE()                __HAL_RCC_GPIOE_CLK_DISABLE()
#else /* USE_NUCLEO_H743ZI */
#define ADAFRUIT_802_SD_CS_GPIO_PORT                         GPIOF
#define ADAFRUIT_802_SD_CS_GPIO_CLK_ENABLE()                 __HAL_RCC_GPIOF_CLK_ENABLE()
#define ADAFRUIT_802_SD_CS_GPIO_CLK_DISABLE()                __HAL_RCC_GPIOF_CLK_DISABLE()
#endif
/**
  * @brief  LCD Control Interface pins (shield D10)
  */
#define ADAFRUIT_802_LCD_CS_PIN                               GPIO_PIN_14
#define ADAFRUIT_802_LCD_CS_GPIO_PORT                         GPIOD
#define ADAFRUIT_802_LCD_CS_GPIO_CLK_ENABLE()                 __HAL_RCC_GPIOD_CLK_ENABLE()
#define ADAFRUIT_802_LCD_CS_GPIO_CLK_DISABLE()                __HAL_RCC_GPIOD_CLK_DISABLE()

/**
  * @brief  LCD Data/Command Interface pins (shield D8)
  */
#if defined (USE_NUCLEO_H743ZI2) || defined (USE_NUCLEO_H7A3ZI)
#define ADAFRUIT_802_LCD_DC_PIN                               GPIO_PIN_3
#define ADAFRUIT_802_LCD_DC_GPIO_PORT                         GPIOF
#define ADAFRUIT_802_LCD_DC_GPIO_CLK_ENABLE()                 __HAL_RCC_GPIOF_CLK_ENABLE()
#define ADAFRUIT_802_LCD_DC_GPIO_CLK_DISABLE()                __HAL_RCC_GPIOF_CLK_DISABLE()
#elif defined (USE_NUCLEO_H745ZI_Q)
#define ADAFRUIT_802_LCD_DC_PIN                               GPIO_PIN_9
#define ADAFRUIT_802_LCD_DC_GPIO_PORT                         GPIOG
#define ADAFRUIT_802_LCD_DC_GPIO_CLK_ENABLE()                 __HAL_RCC_GPIOG_CLK_ENABLE()
#define ADAFRUIT_802_LCD_DC_GPIO_CLK_DISABLE()                __HAL_RCC_GPIOG_CLK_DISABLE()
#else /* USE_NUCLEO_H743ZI */
#define ADAFRUIT_802_LCD_DC_PIN                               GPIO_PIN_12
#define ADAFRUIT_802_LCD_DC_GPIO_PORT                         GPIOF
#define ADAFRUIT_802_LCD_DC_GPIO_CLK_ENABLE()                 __HAL_RCC_GPIOF_CLK_ENABLE()
#define ADAFRUIT_802_LCD_DC_GPIO_CLK_DISABLE()                __HAL_RCC_GPIOF_CLK_DISABLE()
#endif
/**
  * @brief  SD Control Lines management
  */
#define ADAFRUIT_802_SD_CS_LOW()       HAL_GPIO_WritePin(ADAFRUIT_802_SD_CS_GPIO_PORT, ADAFRUIT_802_SD_CS_PIN, GPIO_PIN_RESET)
#define ADAFRUIT_802_SD_CS_HIGH()      HAL_GPIO_WritePin(ADAFRUIT_802_SD_CS_GPIO_PORT, ADAFRUIT_802_SD_CS_PIN, GPIO_PIN_SET)

/**
  * @brief  LCD Control Lines management
  */
#define ADAFRUIT_802_LCD_CS_LOW()      HAL_GPIO_WritePin(ADAFRUIT_802_LCD_CS_GPIO_PORT, ADAFRUIT_802_LCD_CS_PIN, GPIO_PIN_RESET)
#define ADAFRUIT_802_LCD_CS_HIGH()     HAL_GPIO_WritePin(ADAFRUIT_802_LCD_CS_GPIO_PORT, ADAFRUIT_802_LCD_CS_PIN, GPIO_PIN_SET)
#define ADAFRUIT_802_LCD_DC_LOW()      HAL_GPIO_WritePin(ADAFRUIT_802_LCD_DC_GPIO_PORT, ADAFRUIT_802_LCD_DC_PIN, GPIO_PIN_RESET)
#define ADAFRUIT_802_LCD_DC_HIGH()     HAL_GPIO_WritePin(ADAFRUIT_802_LCD_DC_GPIO_PORT, ADAFRUIT_802_LCD_DC_PIN, GPIO_PIN_SET)

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

#ifdef __cplusplus
}
#endif

#endif /* ADAFRUIT_802_CONF_H */

