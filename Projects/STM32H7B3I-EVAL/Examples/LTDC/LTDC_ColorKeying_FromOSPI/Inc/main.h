/**
  ******************************************************************************
  * @file    LTDC/LTDC_ColorKeying_FromOSPI/Inc/main.h 
  * @author  MCD Application Team
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2018 STMicroelectronics.
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
#include "stm32h7b3i_eval.h"
#include "stm32h7b3i_eval_lcd.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Definition for OSPI Pins */
#define OSPI_CS_PIN                 GPIO_PIN_6
#define OSPI_CS_GPIO_PORT           GPIOG
#define OSPI_CLK_PIN                GPIO_PIN_2
#define OSPI_CLK_GPIO_PORT          GPIOB
#define OSPI_D0_PIN                 GPIO_PIN_8
#define OSPI_D0_GPIO_PORT           GPIOF
#define OSPI_D1_PIN                 GPIO_PIN_9
#define OSPI_D1_GPIO_PORT           GPIOF
#define OSPI_D2_PIN                 GPIO_PIN_7
#define OSPI_D2_GPIO_PORT           GPIOF
#define OSPI_D3_PIN                 GPIO_PIN_6
#define OSPI_D3_GPIO_PORT           GPIOF
#define OSPI_D4_PIN                 GPIO_PIN_1
#define OSPI_D4_GPIO_PORT           GPIOC
#define OSPI_D5_PIN                 GPIO_PIN_3
#define OSPI_D5_GPIO_PORT           GPIOH
#define OSPI_D6_PIN                 GPIO_PIN_6
#define OSPI_D6_GPIO_PORT           GPIOD
#define OSPI_D7_PIN                 GPIO_PIN_14
#define OSPI_D7_GPIO_PORT           GPIOG
#define OSPI_DQS_PIN                GPIO_PIN_5
#define OSPI_DQS_GPIO_PORT          GPIOC
/* Exported macro ------------------------------------------------------------*/
/* MX25LM512ABA1G12 Macronix memory */
#define OSPI_FLASH_SIZE             26
/*Flash commands */
#define OCTAL_IO_DTR_READ_CMD       0xEE11
#define OCTAL_PAGE_PROG_CMD         0x12ED
#define OCTAL_WRITE_ENABLE_CMD      0x06F9
#define OCTAL_READ_STATUS_REG_CMD   0x05FA
#define READ_STATUS_REG_CMD         0x05
#define WRITE_CFG_REG_2_CMD         0x72
#define WRITE_ENABLE_CMD            0x06
/* Dummy clocks cycles */
#define DUMMY_CLOCK_CYCLES_READ_OCTAL           20
#define OCTAL_SECTOR_ERASE_CMD      0x21DE
/* Auto-polling values */
#define WRITE_ENABLE_MATCH_VALUE    0x02
#define WRITE_ENABLE_MASK_VALUE     0x02

#define MEMORY_READY_MATCH_VALUE    0x00
#define MEMORY_READY_MASK_VALUE     0x01

/* Definition for OSPI clock resources */
#define OSPI_CLK_ENABLE()           __HAL_RCC_OSPI1_CLK_ENABLE()
#define OSPI_CLK_DISABLE()          __HAL_RCC_OSPI1_CLK_DISABLE()
#define OSPIM_CLK_ENABLE()          __HAL_RCC_OCTOSPIM_CLK_ENABLE()
#define OSPIM_CLK_DISABLE()         __HAL_RCC_OCTOSPIM_CLK_DISABLE()
#define OSPI_CS_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOG_CLK_ENABLE()
#define OSPI_CLK_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOB_CLK_ENABLE()
#define OSPI_D0_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOF_CLK_ENABLE()
#define OSPI_D1_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOF_CLK_ENABLE()
#define OSPI_D2_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOF_CLK_ENABLE()
#define OSPI_D3_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOF_CLK_ENABLE()
#define OSPI_D4_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOC_CLK_ENABLE()
#define OSPI_D5_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOH_CLK_ENABLE()
#define OSPI_D6_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOD_CLK_ENABLE()
#define OSPI_D7_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOG_CLK_ENABLE()
#define OSPI_DQS_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOC_CLK_ENABLE()

#define OSPI_FORCE_RESET()          __HAL_RCC_OSPI1_FORCE_RESET()
#define OSPI_RELEASE_RESET()        __HAL_RCC_OSPI1_RELEASE_RESET()
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

