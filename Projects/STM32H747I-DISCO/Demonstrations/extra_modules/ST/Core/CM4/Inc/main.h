/**
  ******************************************************************************
  * @file    main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module for Cortex-M4
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"
#include "stm32h747i_discovery.h"
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
