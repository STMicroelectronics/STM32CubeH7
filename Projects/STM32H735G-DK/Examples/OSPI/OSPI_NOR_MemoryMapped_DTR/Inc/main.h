/**
  ******************************************************************************
  * @file    OCTOSPI/OCTOSPI_Flash_MemoryMapped_DTR/Inc/main.h
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

/* MX25LM512ABA1G12 Macronix memory */
/* Size of the flash */
#define OSPI_FLASH_SIZE             26
#define OSPI_PAGE_SIZE              256

/* Flash commands */
#define OCTAL_IO_DTR_READ_CMD       0xEE11
#define OCTAL_PAGE_PROG_CMD         0x12ED
#define OCTAL_READ_STATUS_REG_CMD   0x05FA
#define OCTAL_SECTOR_ERASE_CMD      0x21DE
#define OCTAL_WRITE_ENABLE_CMD      0x06F9
#define READ_STATUS_REG_CMD         0x05
#define WRITE_CFG_REG_2_CMD         0x72
#define WRITE_ENABLE_CMD            0x06

/* Dummy clocks cycles */
#define DUMMY_CLOCK_CYCLES_READ     6
#define DUMMY_CLOCK_CYCLES_READ_REG 4
#define DUMMY_CLOCK_CYCLES_READ_OCTAL           20

/* Auto-polling values */
#define WRITE_ENABLE_MATCH_VALUE    0x02
#define WRITE_ENABLE_MASK_VALUE     0x02

#define MEMORY_READY_MATCH_VALUE    0x00
#define MEMORY_READY_MASK_VALUE     0x01

#define AUTO_POLLING_INTERVAL       0x10

/* Memory registers address */
#define CONFIG_REG2_ADDR1           0x0000000
#define CR2_DTR_OPI_ENABLE          0x02

#define CONFIG_REG2_ADDR3           0x00000300
#define CR2_DUMMY_CYCLES_66MHZ      0x07

/* Memory delay */
#define MEMORY_REG_WRITE_DELAY      40
#define MEMORY_PAGE_PROG_DELAY      2

/* End address of the OSPI memory */
#define OSPI_END_ADDR               (1 << OSPI_FLASH_SIZE)

/* Size of buffers */
#define BUFFERSIZE                  (COUNTOF(aTxBuffer)-1)

/* Exported macro ------------------------------------------------------------*/
#define COUNTOF(__BUFFER__)         (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))

/* Definition for OCTOSPI1 Pins */
#define OSPI1_CS_PIN                 GPIO_PIN_6
#define OSPI1_CS_GPIO_PORT           GPIOG

#define OSPI1_CLK_PIN                GPIO_PIN_10
#define OSPI1_CLK_GPIO_PORT          GPIOF

#define OSPI1_D0_PIN                 GPIO_PIN_11
#define OSPI1_D0_GPIO_PORT           GPIOD

#define OSPI1_D1_PIN                 GPIO_PIN_12
#define OSPI1_D1_GPIO_PORT           GPIOD

#define OSPI1_D2_PIN                 GPIO_PIN_2
#define OSPI1_D2_GPIO_PORT           GPIOE

#define OSPI1_D3_PIN                 GPIO_PIN_13
#define OSPI1_D3_GPIO_PORT           GPIOD

#define OSPI1_D4_PIN                 GPIO_PIN_4
#define OSPI1_D4_GPIO_PORT           GPIOD

#define OSPI1_D5_PIN                 GPIO_PIN_5
#define OSPI1_D5_GPIO_PORT           GPIOD

#define OSPI1_D6_PIN                 GPIO_PIN_9
#define OSPI1_D6_GPIO_PORT           GPIOG

#define OSPI1_D7_PIN                 GPIO_PIN_7
#define OSPI1_D7_GPIO_PORT           GPIOD

#define OSPI1_DQS_PIN                GPIO_PIN_2
#define OSPI1_DQS_GPIO_PORT          GPIOB

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/* Definition for OCTOSPI1 clock resources */
#define OSPI1_CLK_ENABLE()             __HAL_RCC_OSPI1_CLK_ENABLE()
#define OSPI1_CLK_DISABLE()            __HAL_RCC_OSPI1_CLK_DISABLE()
#define OSPI1_CS_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOG_CLK_ENABLE()
#define OSPI1_CLK_P_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOF_CLK_ENABLE()
#define OSPI1_D0_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOD_CLK_ENABLE()
#define OSPI1_D1_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOD_CLK_ENABLE()
#define OSPI1_D2_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOE_CLK_ENABLE()
#define OSPI1_D3_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOD_CLK_ENABLE()
#define OSPI1_D4_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOD_CLK_ENABLE()
#define OSPI1_D5_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOD_CLK_ENABLE()
#define OSPI1_D6_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOG_CLK_ENABLE()
#define OSPI1_D7_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOD_CLK_ENABLE()
#define OSPI1_DQS_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOB_CLK_ENABLE()

#define OSPI1_FORCE_RESET()             __HAL_RCC_OSPI1_FORCE_RESET()
#define OSPI1_RELEASE_RESET()           __HAL_RCC_OSPI1_RELEASE_RESET()
#endif /* __MAIN_H */

