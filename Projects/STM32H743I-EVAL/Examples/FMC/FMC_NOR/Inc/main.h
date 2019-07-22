/**
  ******************************************************************************
  * @file    FMC/FMC_NOR/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
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
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"
#include "stm32h743i_eval.h"

/* Exported types ------------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

/* Exported constants --------------------------------------------------------*/
#define NOR_BANK_ADDR                 ((uint32_t)0x60000000)
#define PROGRAM_TIMEOUT               ((uint32_t)0x00004400)  /* NOR program timeout     */
#define ERASE_TIMEOUT                 ((uint32_t)0x00A00000)  /* NOR erase timeout     */

/* #define NOR_MEMORY_WIDTH            FMC_NORSRAM_MEM_BUS_WIDTH_8  */
#define NOR_MEMORY_WIDTH               FMC_NORSRAM_MEM_BUS_WIDTH_16

/* #define NOR_CONTINUOUS_CLOCK    FMC_CONTINUOUS_CLOCK_SYNC_ONLY */
#define NOR_CONTINUOUS_CLOCK    FMC_CONTINUOUS_CLOCK_SYNC_ASYNC

/* NOR Ready/Busy signal GPIO definitions */
#define NOR_READY_BUSY_PIN    GPIO_PIN_6 
#define NOR_READY_BUSY_GPIO   GPIOC
#define NOR_READY_STATE       GPIO_PIN_SET
#define NOR_BUSY_STATE        GPIO_PIN_RESET 

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
