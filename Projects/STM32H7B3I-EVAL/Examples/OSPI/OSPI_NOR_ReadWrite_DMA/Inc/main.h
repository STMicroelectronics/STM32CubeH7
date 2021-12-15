/**
  ******************************************************************************
  * @file    OSPI/OSPI_NOR_ReadWrite_DMA/Inc/main.h
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

/* MX25LM512ABA1G12 Macronix memory */
/* Size of the flash */
#define OSPI_FLASH_SIZE             26
#define OSPI_PAGE_SIZE              256

/* Flash commands */
#define OCTAL_IO_READ_CMD           0xEC13
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
#define CR2_STR_OPI_ENABLE          0x01

#define CONFIG_REG2_ADDR3           0x00000300
#define CR2_DUMMY_CYCLES_66MHZ      0x07

/* Memory delay */
#define MEMORY_REG_WRITE_DELAY      40
#define MEMORY_PAGE_PROG_DELAY      2

/* End address of the OSPI memory */
#define OSPI_END_ADDR               (1 << OSPI_FLASH_SIZE)

/* Size of buffers */
#define BUFFER_LENGTH                  (COUNTOF(TX_Buffer) - 1)

/* Exported macro ------------------------------------------------------------*/
#define COUNTOF(__BUFFER__)         (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))

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

