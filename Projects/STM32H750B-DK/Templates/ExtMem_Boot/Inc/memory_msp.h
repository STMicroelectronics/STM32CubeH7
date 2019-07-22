/**
  ******************************************************************************
  * @file    memory_msp.h
  * @author  MCD Application Team
  * @brief   This file contains Header for msp function needed for external
             memory initialization.
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
#ifndef __MEMORY_PRIVATE_H
#define __MEMORY_PRIVATE_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

/** @addtogroup MEMORY_DEFS
  * @{
  */

/* Exported constants --------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void SDRAM_MspInit(void);
void QSPI_MspInit(void);
void QSPI_MspDeInit(void);
void MMC_MspInit(void);
void MMC_MspDeInit(void);

/* Exported macro ------------------------------------------------------------*/

/* Definition for QSPI clock resources */
#define QSPI_CLK_ENABLE()              __HAL_RCC_QSPI_CLK_ENABLE()
#define QSPI_CLK_DISABLE()             __HAL_RCC_QSPI_CLK_DISABLE()
#define QSPI_CLK_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOF_CLK_ENABLE()
#define QSPI_BK1_CS_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOG_CLK_ENABLE()
#define QSPI_BK1_D0_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOD_CLK_ENABLE()
#define QSPI_BK1_D1_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOF_CLK_ENABLE()
#define QSPI_BK1_D2_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOF_CLK_ENABLE()
#define QSPI_BK1_D3_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOF_CLK_ENABLE()
#define QSPI_BK2_CS_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOG_CLK_ENABLE()
#define QSPI_BK2_D0_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOH_CLK_ENABLE()
#define QSPI_BK2_D1_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOH_CLK_ENABLE()
#define QSPI_BK2_D2_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOG_CLK_ENABLE()
#define QSPI_BK2_D3_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOG_CLK_ENABLE()


#define QSPI_FORCE_RESET()         __HAL_RCC_QSPI_FORCE_RESET()
#define QSPI_RELEASE_RESET()       __HAL_RCC_QSPI_RELEASE_RESET()

/* Definition for QSPI Pins */
#define QSPI_CLK_PIN               GPIO_PIN_10
#define QSPI_CLK_GPIO_PORT         GPIOF
/* Bank 1 */
#define QSPI_BK1_CS_PIN            GPIO_PIN_6
#define QSPI_BK1_CS_GPIO_PORT      GPIOG
#define QSPI_BK1_D0_PIN            GPIO_PIN_11
#define QSPI_BK1_D0_GPIO_PORT      GPIOD
#define QSPI_BK1_D1_PIN            GPIO_PIN_9
#define QSPI_BK1_D1_GPIO_PORT      GPIOF
#define QSPI_BK1_D2_PIN            GPIO_PIN_7
#define QSPI_BK1_D2_GPIO_PORT      GPIOF
#define QSPI_BK1_D3_PIN            GPIO_PIN_6
#define QSPI_BK1_D3_GPIO_PORT      GPIOF

/* Bank 2 */
#define QSPI_BK2_CS_PIN            GPIO_PIN_6
#define QSPI_BK2_CS_GPIO_PORT      GPIOG
#define QSPI_BK2_D0_PIN            GPIO_PIN_2
#define QSPI_BK2_D0_GPIO_PORT      GPIOH
#define QSPI_BK2_D1_PIN            GPIO_PIN_3
#define QSPI_BK2_D1_GPIO_PORT      GPIOH
#define QSPI_BK2_D2_PIN            GPIO_PIN_9
#define QSPI_BK2_D2_GPIO_PORT      GPIOG
#define QSPI_BK2_D3_PIN            GPIO_PIN_14
#define QSPI_BK2_D3_GPIO_PORT      GPIOG


#ifdef __cplusplus
}
#endif

#endif /* __MEMORY_PRIVATE_H */
/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
