/**
  ******************************************************************************
  * @file    stm32h747i_discovery.h
  * @author  MCD Application Team
  * @brief   This file contains definitions for STM32H747I-Discovery LEDs,
  *          push-buttons hardware resources.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32H747I_DISCOVERY_H
#define __STM32H747I_DISCOVERY_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"
#if defined(BSP_USE_CMSIS_OS)
#include "cmsis_os.h"
#endif

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32H747I_DISCOVERY
  * @{
  */

/** @addtogroup STM32H747I_DISCOVERY_LOW_LEVEL
  * @{
  */

/** @defgroup STM32H747I_DISCOVERY_LOW_LEVEL_Exported_Types Exported Types
 * @{
 */

/** @brief Led_TypeDef
  *  STM32H747I_DISCOVERY board leds definitions.
  */
typedef enum
{
  LED1 = 0,
  LED_GREEN = LED1,
  LED2 = 1,
  LED_ORANGE = LED2,
  LED3 = 2,
  LED_RED = LED3,
  LED4 = 3,
  LED_BLUE = LED4
} Led_TypeDef;

/** @brief Button_TypeDef
  *  STM32H747I_DISCOVERY board Buttons definitions.
  */
typedef enum
{
  BUTTON_WAKEUP = 0,
} Button_TypeDef;

#define BUTTON_USER BUTTON_WAKEUP

/** @brief ButtonMode_TypeDef
  *  STM32H747I_DISCOVERY board Buttons Modes definitions.
  */
typedef enum
{
  BUTTON_MODE_GPIO = 0,
  BUTTON_MODE_EXTI = 1
} ButtonMode_TypeDef;


typedef enum
{
  PB_SET = 0,
  PB_RESET = !PB_SET
} ButtonValue_TypeDef;

typedef enum
{
  JOY_MODE_GPIO = 0,
  JOY_MODE_EXTI = 1
} JOYMode_TypeDef;

typedef enum
{
  JOY_SEL   = 0,
  JOY_DOWN  = 1,
  JOY_LEFT  = 2,
  JOY_RIGHT = 3,
  JOY_UP    = 4,
  JOY_NONE  = 5
} JOYState_TypeDef;

/** @brief DISCO_Status_TypeDef
  *  STM32H747I_DISCO board Status return possible values.
  */
typedef enum
{
  DISCO_OK    = 0,
  DISCO_ERROR = 1
} DISCO_Status_TypeDef;

/**
  * @}
  */

/** @defgroup STM32H747I_DISCOVERY_LOW_LEVEL_Exported_Constants Exported Constants
  * @{
  */

/**
  * @brief  Define for STM32H747I_DISCOVERY board
  */
#if !defined (USE_STM32H747I_DISCO)
 #define USE_STM32H747I_DISCO
#endif

#define LEDn                             ((uint32_t)4)

#define LED1_GPIO_PORT                   GPIOI
#define LED1_PIN                         GPIO_PIN_12

#define LED2_GPIO_PORT                   GPIOI
#define LED2_PIN                         GPIO_PIN_13

#define LED3_GPIO_PORT                   GPIOI
#define LED3_PIN                         GPIO_PIN_14

#define LED4_GPIO_PORT                   GPIOI
#define LED4_PIN                         GPIO_PIN_15

#define LEDx_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOI_CLK_ENABLE()
#define LEDx_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOI_CLK_DISABLE()

/* Only one User/Wakeup button */
#define BUTTONn                             ((uint8_t)1)

/**
  * @brief Wakeup push-button
  */
#define WAKEUP_BUTTON_PIN                   GPIO_PIN_13
#define WAKEUP_BUTTON_GPIO_PORT             GPIOC
#define WAKEUP_BUTTON_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOC_CLK_ENABLE()
#define WAKEUP_BUTTON_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOC_CLK_DISABLE()
#define WAKEUP_BUTTON_EXTI_IRQn             EXTI15_10_IRQn

/* Define the USER button as an alias of the Wakeup button */
#define USER_BUTTON_PIN                   WAKEUP_BUTTON_PIN
#define USER_BUTTON_GPIO_PORT             WAKEUP_BUTTON_GPIO_PORT
#define USER_BUTTON_GPIO_CLK_ENABLE()     WAKEUP_BUTTON_GPIO_CLK_ENABLE()
#define USER_BUTTON_GPIO_CLK_DISABLE()    WAKEUP_BUTTON_GPIO_CLK_DISABLE()
#define USER_BUTTON_EXTI_IRQn             WAKEUP_BUTTON_EXTI_IRQn

#define BUTTON_GPIO_CLK_ENABLE()            __HAL_RCC_GPIOC_CLK_ENABLE()

#define JOYn                              ((uint8_t)5)

/**
 * @brief Joystick Selection push-button
 */
#define SEL_JOY_PIN                       GPIO_PIN_2
#define SEL_JOY_GPIO_PORT                 GPIOK
#define SEL_JOY_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOK_CLK_ENABLE()
#define SEL_JOY_GPIO_CLK_DISABLE()        __HAL_RCC_GPIOK_CLK_DISABLE()
#define SEL_JOY_EXTI_IRQn                 EXTI2_IRQn

/**
* @brief Joystick Down push-button
*/
#define DOWN_JOY_PIN                      GPIO_PIN_3
#define DOWN_JOY_GPIO_PORT                GPIOK
#define DOWN_JOY_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOK_CLK_ENABLE()
#define DOWN_JOY_GPIO_CLK_DISABLE()       __HAL_RCC_GPIOK_CLK_DISABLE()
#define DOWN_JOY_EXTI_IRQn                EXTI3_IRQn

/**
* @brief Joystick Left push-button
*/
#define LEFT_JOY_PIN                      GPIO_PIN_4
#define LEFT_JOY_GPIO_PORT                GPIOK
#define LEFT_JOY_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOK_CLK_ENABLE()
#define LEFT_JOY_GPIO_CLK_DISABLE()       __HAL_RCC_GPIOK_CLK_DISABLE()
#define LEFT_JOY_EXTI_IRQn                EXTI4_IRQn

/**
 * @brief Joystick Right push-button
 */
#define RIGHT_JOY_PIN                     GPIO_PIN_5
#define RIGHT_JOY_GPIO_PORT               GPIOK
#define RIGHT_JOY_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOK_CLK_ENABLE()
#define RIGHT_JOY_GPIO_CLK_DISABLE()      __HAL_RCC_GPIOK_CLK_DISABLE()
#define RIGHT_JOY_EXTI_IRQn               EXTI9_5_IRQn

/**
* @brief Joystick Up push-button
*/
#define UP_JOY_PIN                        GPIO_PIN_6
#define UP_JOY_GPIO_PORT                  GPIOK
#define UP_JOY_GPIO_CLK_ENABLE()          __HAL_RCC_GPIOK_CLK_ENABLE()
#define UP_JOY_GPIO_CLK_DISABLE()         __HAL_RCC_GPIOK_CLK_DISABLE()
#define UP_JOY_EXTI_IRQn                  EXTI9_5_IRQn

#define JOYx_GPIO_CLK_ENABLE(__JOY__)     do { if((__JOY__) == JOY_SEL)   { SEL_JOY_GPIO_CLK_ENABLE();   } else \
                                               if((__JOY__) == JOY_DOWN)  { DOWN_JOY_GPIO_CLK_ENABLE();  } else \
                                               if((__JOY__) == JOY_LEFT)  { LEFT_JOY_GPIO_CLK_ENABLE();  } else \
                                               if((__JOY__) == JOY_RIGHT) { RIGHT_JOY_GPIO_CLK_ENABLE(); } else \
                                               if((__JOY__) == JOY_UP)    { UP_JOY_GPIO_CLK_ENABLE(); }  } while(0)

#define JOYx_GPIO_CLK_DISABLE(__JOY__)    do { if((__JOY__) == JOY_SEL)   { SEL_JOY_GPIO_CLK_DISABLE();   } else \
                                               if((__JOY__) == JOY_DOWN)  { DOWN_JOY_GPIO_CLK_DISABLE();  } else \
                                               if((__JOY__) == JOY_LEFT)  { LEFT_JOY_GPIO_CLK_DISABLE();  } else \
                                               if((__JOY__) == JOY_RIGHT) { RIGHT_JOY_GPIO_CLK_DISABLE(); } else \
                                               if((__JOY__) == JOY_UP)    { UP_JOY_GPIO_CLK_DISABLE(); }  } while(0)

#define JOY_ALL_PINS                      (RIGHT_JOY_PIN | LEFT_JOY_PIN | UP_JOY_PIN | DOWN_JOY_PIN | SEL_JOY_PIN)


/**
  * @brief USB OTG HS Over Current signal
  */
#define OTG_HS_OVER_CURRENT_PIN                  GPIO_PIN_1
#define OTG_HS_OVER_CURRENT_PORT                 GPIOJ
#define OTG_HS_OVER_CURRENT_PORT_CLK_ENABLE()    __HAL_RCC_GPIOJ_CLK_ENABLE()

/**
  * @brief SD-detect signal
  */
#define SD_DETECT_PIN                        ((uint32_t)GPIO_PIN_8)
#define SD_DETECT_GPIO_PORT                  ((GPIO_TypeDef*)GPIOI)
#define SD_DETECT_GPIO_CLK_ENABLE()          __HAL_RCC_GPIOI_CLK_ENABLE()
#define SD_DETECT_GPIO_CLK_DISABLE()         __HAL_RCC_GPIOI_CLK_DISABLE()
#define SD_DETECT_EXTI_IRQn                  EXTI9_5_IRQn

/**
  * @brief TS_INT signal from TouchScreen when it is configured in interrupt mode
  * GPIOI13 is used for that purpose on Manta Dragon Discovery board
  */
#define TS_INT_PIN                        ((uint32_t)GPIO_PIN_7)
#define TS_INT_GPIO_PORT                  ((GPIO_TypeDef*)GPIOK)
#define TS_INT_GPIO_CLK_ENABLE()          __HAL_RCC_GPIOK_CLK_ENABLE()
#define TS_INT_GPIO_CLK_DISABLE()         __HAL_RCC_GPIOK_CLK_DISABLE()
#define TS_INT_EXTI_IRQn                  EXTI9_5_IRQn

/**
  * @brief TouchScreen FT6206 Slave I2C address 1
  */
#define TS_I2C_ADDRESS                   ((uint16_t)0x54)

/**
  * @brief TouchScreen FT6336G Slave I2C address 2
  */
#define TS_I2C_ADDRESS_A02               ((uint16_t)0x70)

/**
  * @brief LCD DSI Slave I2C address 1
  */
#define LCD_DSI_ADDRESS                  TS_I2C_ADDRESS

/**
  * @brief LCD DSI Slave I2C address 2
  */
#define LCD_DSI_ADDRESS_A02              TS_I2C_ADDRESS_A02

/**
  * @brief Audio I2C Slave address
  */
#define AUDIO_I2C_ADDRESS                ((uint16_t)0x34)

#define CAMERA_I2C_ADDRESS               ((uint16_t)0x60)

/**
  * @brief User can use this section to tailor I2C4/I2C4 instance used and associated
  * resources (audio codec).
  * Definition for I2C4 clock resources
  */
#define DISCOVERY_I2Cx                             I2C4
#define DISCOVERY_I2Cx_CLK_ENABLE()                __HAL_RCC_I2C4_CLK_ENABLE()
#define DISCOVERY_I2Cx_SCL_SDA_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOD_CLK_ENABLE()

#define DISCOVERY_I2Cx_FORCE_RESET()               __HAL_RCC_I2C4_FORCE_RESET()
#define DISCOVERY_I2Cx_RELEASE_RESET()             __HAL_RCC_I2C4_RELEASE_RESET()

/** @brief Definition for I2C4 Pins
  */
#define DISCOVERY_I2Cx_SCL_PIN                     GPIO_PIN_12 /*!< PD12 */
#define DISCOVERY_I2Cx_SDA_PIN                     GPIO_PIN_13 /*!< PD13 */
#define DISCOVERY_I2Cx_SCL_SDA_AF                  GPIO_AF4_I2C4
#define DISCOVERY_I2Cx_SCL_SDA_GPIO_PORT           GPIOD
/** @brief Definition of I2C4 interrupt requests
  */
#define DISCOVERY_I2Cx_EV_IRQn                     I2C4_EV_IRQn
#define DISCOVERY_I2Cx_ER_IRQn                     I2C4_ER_IRQn

/* I2C TIMING Register define when I2C clock source is SYSCLK */
/* I2C TIMING is calculated from APB1 source clock = 50 MHz */
/* Due to the big MOFSET capacity for adapting the camera level the rising time is very large (>1us) */
/* 0x40912732 takes in account the big rising and aims a clock of 100khz */
#ifndef DISCOVERY_I2Cx_TIMING
#define DISCOVERY_I2Cx_TIMING                      ((uint32_t)0x40912732)
#endif /* DISCOVERY_I2Cx_TIMING */

/**
  * @}
  */


/** @addtogroup STM32H747I_DISCOVERY_LOW_LEVEL_Exported_Functions
  * @{
  */
uint32_t         BSP_GetVersion(void);
void             BSP_LED_Init(Led_TypeDef Led);
void             BSP_LED_DeInit(Led_TypeDef Led);
void             BSP_LED_On(Led_TypeDef Led);
void             BSP_LED_Off(Led_TypeDef Led);
void             BSP_LED_Toggle(Led_TypeDef Led);
void             BSP_PB_Init(Button_TypeDef Button, ButtonMode_TypeDef Button_Mode);
void             BSP_PB_DeInit(Button_TypeDef Button);
uint32_t         BSP_PB_GetState(Button_TypeDef Button);
uint8_t          BSP_JOY_Init(JOYMode_TypeDef Joy_Mode);
void             BSP_JOY_DeInit(void);
JOYState_TypeDef BSP_JOY_GetState(void);
void             BSP_ErrorNotify(void);

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

#endif /* __STM32H747I_DISCOVERY_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
