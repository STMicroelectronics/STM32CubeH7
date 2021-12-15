/**
  ******************************************************************************
  * @file    OCTOSPI/OCTOSPI_HyperRAM_MemoryMapped/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
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
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"
#include "stm32h735g_discovery.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* S70KL1281 memory */
/* Size of the HyperRAM */
#define OSPI_HYPERRAM_SIZE          24
#define OSPI_HYPERRAM_INCR_SIZE     256

/* Timing of the HyperRAM */
#define OSPI_HYPERRAM_RW_REC_TIME   3
#define OSPI_HYPERRAM_LATENCY       6

/* End address of the OSPI memory */
#define OSPI_HYPERRAM_END_ADDR      (1 << OSPI_HYPERRAM_SIZE)

/* Size of buffers */
#define BUFFERSIZE                  (COUNTOF(aTxBuffer) - 1)
/* Exported macro ------------------------------------------------------------*/
#define COUNTOF(__BUFFER__)         (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))

/* Definition for OCTOSPI2 Pins */
#define OSPI2_CS_PIN                 GPIO_PIN_12
#define OSPI2_CS_GPIO_PORT           GPIOG

#define OSPI2_CLK_PIN                GPIO_PIN_4
#define OSPI2_CLK_GPIO_PORT          GPIOF

#define OSPI2_D0_PIN                 GPIO_PIN_0
#define OSPI2_D0_GPIO_PORT           GPIOF

#define OSPI2_D1_PIN                 GPIO_PIN_1
#define OSPI2_D1_GPIO_PORT           GPIOF

#define OSPI2_D2_PIN                 GPIO_PIN_2
#define OSPI2_D2_GPIO_PORT           GPIOF

#define OSPI2_D3_PIN                 GPIO_PIN_3
#define OSPI2_D3_GPIO_PORT           GPIOF

#define OSPI2_D4_PIN                 GPIO_PIN_0
#define OSPI2_D4_GPIO_PORT           GPIOG

#define OSPI2_D5_PIN                 GPIO_PIN_1
#define OSPI2_D5_GPIO_PORT           GPIOG

#define OSPI2_D6_PIN                 GPIO_PIN_10
#define OSPI2_D6_GPIO_PORT           GPIOG

#define OSPI2_D7_PIN                 GPIO_PIN_11
#define OSPI2_D7_GPIO_PORT           GPIOG

#define OSPI2_DQS_PIN                GPIO_PIN_12
#define OSPI2_DQS_GPIO_PORT          GPIOF

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/* Definition for OCTOSPI2 clock resources */
#define OSPI2_CLK_ENABLE()             __HAL_RCC_OSPI2_CLK_ENABLE()
#define OSPI2_CLK_DISABLE()            __HAL_RCC_OSPI2_CLK_DISABLE()
#define OSPI2_CS_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOG_CLK_ENABLE()
#define OSPI2_CLK_P_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOF_CLK_ENABLE()
#define OSPI2_D0_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOF_CLK_ENABLE()
#define OSPI2_D1_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOF_CLK_ENABLE()
#define OSPI2_D2_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOF_CLK_ENABLE()
#define OSPI2_D3_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOF_CLK_ENABLE()
#define OSPI2_D4_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOG_CLK_ENABLE()
#define OSPI2_D5_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOG_CLK_ENABLE()
#define OSPI2_D6_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOG_CLK_ENABLE()
#define OSPI2_D7_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOG_CLK_ENABLE()
#define OSPI2_DQS_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOF_CLK_ENABLE()

#define OSP2_FORCE_RESET()             __HAL_RCC_OSPI2_FORCE_RESET()
#define OSP2_RELEASE_RESET()           __HAL_RCC_OSPI2_RELEASE_RESET()
#endif /* __MAIN_H */

