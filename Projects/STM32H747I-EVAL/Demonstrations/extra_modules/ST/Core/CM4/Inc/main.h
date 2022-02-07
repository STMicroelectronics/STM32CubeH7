/**
  ******************************************************************************
  * @file    Demonstrations/extra_modules/ST/Core/CM4/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module for Cortex-M4
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
#include "stm32h747i_eval.h"
#include <cmsis_os.h>

/* utilities and standard includes components */   
#include <stdio.h>

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Error_Handler(void);
void BSP_ErrorHandler(void);
#ifdef configUSE_MALLOC_FAILED_HOOK
void vApplicationMallocFailedHook(TaskHandle_t xTask, char *pcTaskName);
#endif /* configUSE_MALLOC_FAILED_HOOK */
#ifdef configCHECK_FOR_STACK_OVERFLOW
void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName );
#endif /* configCHECK_FOR_STACK_OVERFLOW */
#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line);
#endif /* USE_FULL_ASSERT */

#endif /* __MAIN_H */

