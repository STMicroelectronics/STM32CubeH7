/**
  ******************************************************************************
  * @file    OTFDEC_ExecutingAesInstruction/Inc/main.h
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
#include "stdio.h"
#include "macronix.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define PLLM_VALUE                                              25U
#define PLLN_VALUE                                              240U
#define PLLP_VALUE                                              4U
#define PLLQ_VALUE                                              1U
#define PLLR_VALUE                                              1U
#define FLASH_LATENCY_VALUE                                     FLASH_LATENCY_1
#define NB_DUMMY_CYCLE                                          20U
#define CR2_DUMMY_CYCLES                                        CR2_DUMMY_CYCLES_66MHZ

#define OSPI_INSTANCE                                           OCTOSPI1
#define OSPI_NUMBER                                             1U
#define OSPI_CS_PIN                                             GPIO_PIN_6
#define OSPI_CS_GPIO_PORT                                       GPIOG
#define OSPI_CS_AF                                              GPIO_AF10_OCTOSPIM_P1
#define OSPI_CLK_PIN                                            GPIO_PIN_2
#define OSPI_CLK_GPIO_PORT                                      GPIOB
#define OSPI_CLK_AF                                             GPIO_AF9_OCTOSPIM_P1
#define OSPI_D0_PIN                                             GPIO_PIN_8
#define OSPI_D0_GPIO_PORT                                       GPIOF
#define OSPI_D0_AF                                              GPIO_AF10_OCTOSPIM_P1
#define OSPI_D1_PIN                                             GPIO_PIN_9
#define OSPI_D1_GPIO_PORT                                       GPIOF
#define OSPI_D1_AF                                              GPIO_AF10_OCTOSPIM_P1
#define OSPI_D2_PIN                                             GPIO_PIN_7
#define OSPI_D2_GPIO_PORT                                       GPIOF
#define OSPI_D2_AF                                              GPIO_AF10_OCTOSPIM_P1
#define OSPI_D3_PIN                                             GPIO_PIN_6
#define OSPI_D3_GPIO_PORT                                       GPIOF
#define OSPI_D3_AF                                              GPIO_AF10_OCTOSPIM_P1
#define OSPI_D4_PIN                                             GPIO_PIN_1
#define OSPI_D4_GPIO_PORT                                       GPIOC
#define OSPI_D4_AF                                              GPIO_AF10_OCTOSPIM_P1
#define OSPI_D5_PIN                                             GPIO_PIN_3
#define OSPI_D5_GPIO_PORT                                       GPIOH
#define OSPI_D5_AF                                              GPIO_AF9_OCTOSPIM_P1
#define OSPI_D6_PIN                                             GPIO_PIN_6
#define OSPI_D6_GPIO_PORT                                       GPIOD
#define OSPI_D6_AF                                              GPIO_AF10_OCTOSPIM_P1
#define OSPI_D7_PIN                                             GPIO_PIN_14
#define OSPI_D7_GPIO_PORT                                       GPIOG
#define OSPI_D7_AF                                              GPIO_AF9_OCTOSPIM_P1
#define OSPI_DQS_PIN                                            GPIO_PIN_5
#define OSPI_DQS_GPIO_PORT                                      GPIOC
#define OSPI_DQS_AF                                             GPIO_AF10_OCTOSPIM_P1
#define OSPI_IRQn                                               OCTOSPI1_IRQn
#define EXTMEM_ADDRESS                                          0x90000000UL
#define EXTMEM_JUMP_ADDRESS                                     0x90000004UL
#define OTFD_INSTANCE                                           OTFDEC1
#define Region1                                                 0
#define Region2                                                 1
#define Region3                                                 2
#define Region4                                                 3
#define OTFDREGION_ID                                           1U
#define OTFDEC1_REGION1_START_ADD                               0x90000000UL
#define OTFDEC1_REGION1_END_ADD                                 0x9001FFFFUL
#define OTFDEC_REGION1_START_ADD                                OTFDEC1_REGION1_START_ADD
#define OTFDEC_REGION1_END_ADD                                  OTFDEC1_REGION1_END_ADD

#define OSPI_WRITE_ENABLE_CMD                                   OCTAL_WRITE_ENABLE_CMD
#define OSPI_READ_STATUS_REG_CMD                                OCTAL_READ_STATUS_REG_CMD
#define OSPI_READ_CFG_REG_CMD                                   OCTAL_READ_CFG_REG_CMD
#define OSPI_READ_CFG2_REG_CMD                                  OCTAL_READ_CFG_REG_2_CMD
#define OSPI_INSTRUCTION_MODE                                   HAL_OSPI_INSTRUCTION_8_LINES
#define OSPI_INSTRUCTION_SIZE                                   HAL_OSPI_INSTRUCTION_16_BITS
#define OSPI_INSTRUCTION_DTRMODE                                HAL_OSPI_INSTRUCTION_DTR_ENABLE
#define OSPI_ADDRESS_MODE                                       HAL_OSPI_ADDRESS_8_LINES
#define OSPI_ADDRESS_SIZE                                       HAL_OSPI_ADDRESS_32_BITS
#define OSPI_ADDRESS_DTRMODE                                    HAL_OSPI_ADDRESS_DTR_ENABLE
#define OSPI_DATA_MODE                                          HAL_OSPI_DATA_8_LINES
#define OSPI_DATA_DTRMODE                                       HAL_OSPI_DATA_DTR_ENABLE
#define OSPI_DUMMY_CYCLE                                        DUMMY_CLOCK_CYCLES_READ_OCTAL
#define OSPI_SECTOR_ERASE_CMD                                   OCTAL_SECTOR_ERASE_CMD
#define OSPI_SECTOR_ERASE_ADDRESS_MODE                          HAL_OSPI_ADDRESS_8_LINES
#define OSPI_SECTOR_ERASE_ADDRESS_SIZE                          HAL_OSPI_ADDRESS_32_BITS
#define OSPI_PAGE_PROG_CMD                                      OCTAL_PAGE_PROG_CMD
#define OSPI_PAGE_PROG_ADDRESS_MODE                             HAL_OSPI_ADDRESS_8_LINES
#define OSPI_PAGE_PROG_ADDRESS_SIZE                             HAL_OSPI_ADDRESS_32_BITS
#define OSPI_READ_CMD                                           OCTAL_IO_DTR_READ_CMD
#define OSPI_READ_ADDRESS_MODE                                  HAL_OSPI_ADDRESS_8_LINES
#define OSPI_READ_ADDRESS_SIZE                                  HAL_OSPI_ADDRESS_32_BITS
#define OSPI_DQS_MODE                                           HAL_OSPI_DQS_ENABLE
#define CR2_OPI_ENABLE                                          CR2_DTR_OPI_ENABLE

/* User can use this section to tailor USARTx/UARTx instance used and associated
   resources */
/* Definition for USARTx clock resources */
#define USARTx                                                  USART1
#define USARTx_CLK_ENABLE()                                     __HAL_RCC_USART1_CLK_ENABLE()
#define USARTx_RX_GPIO_CLK_ENABLE()                             __HAL_RCC_GPIOB_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE()                             __HAL_RCC_GPIOB_CLK_ENABLE()

#define USARTx_FORCE_RESET()                                    __HAL_RCC_USART1_FORCE_RESET()
#define USARTx_RELEASE_RESET()                                  __HAL_RCC_USART1_RELEASE_RESET()

/* Definition for USARTx Pins */
#define USARTx_TX_PIN                                           GPIO_PIN_14
#define USARTx_TX_GPIO_PORT                                     GPIOB
#define USARTx_TX_AF                                            GPIO_AF4_USART1
#define USARTx_RX_PIN                                           GPIO_PIN_15
#define USARTx_RX_GPIO_PORT                                     GPIOB
#define USARTx_RX_AF                                            GPIO_AF4_USART1

/* Exported macro ------------------------------------------------------------*/
/* Definition for OSPI clock resources */
#define OSPI_CLK_ENABLE()                                       __HAL_RCC_OSPI1_CLK_ENABLE()
#define OSPI_CLK_DISABLE()                                      __HAL_RCC_OSPI1_CLK_DISABLE()
#define OSPI_CS_GPIO_CLK_ENABLE()                               __HAL_RCC_GPIOG_CLK_ENABLE()
#define OSPI_CLK_GPIO_CLK_ENABLE()                              __HAL_RCC_GPIOB_CLK_ENABLE()
#define OSPI_D0_GPIO_CLK_ENABLE()                               __HAL_RCC_GPIOF_CLK_ENABLE()
#define OSPI_D1_GPIO_CLK_ENABLE()                               __HAL_RCC_GPIOF_CLK_ENABLE()
#define OSPI_D2_GPIO_CLK_ENABLE()                               __HAL_RCC_GPIOF_CLK_ENABLE()
#define OSPI_D3_GPIO_CLK_ENABLE()                               __HAL_RCC_GPIOF_CLK_ENABLE()
#define OSPI_D4_GPIO_CLK_ENABLE()                               __HAL_RCC_GPIOC_CLK_ENABLE()
#define OSPI_D5_GPIO_CLK_ENABLE()                               __HAL_RCC_GPIOH_CLK_ENABLE()
#define OSPI_D6_GPIO_CLK_ENABLE()                               __HAL_RCC_GPIOD_CLK_ENABLE()
#define OSPI_D7_GPIO_CLK_ENABLE()                               __HAL_RCC_GPIOG_CLK_ENABLE()
#define OSPI_DQS_GPIO_CLK_ENABLE()                              __HAL_RCC_GPIOC_CLK_ENABLE()

#define OSPI_FORCE_RESET()                                      __HAL_RCC_OSPI2_FORCE_RESET()
#define OSPI_RELEASE_RESET()                                    __HAL_RCC_OSPI2_RELEASE_RESET()

/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

