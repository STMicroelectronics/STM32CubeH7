/**
  ******************************************************************************
  * @file    stm32h743i_eval_sram.h
  * @author  MCD Application Team
  * @brief   This file contains the common defines and functions prototypes for
  *          the stm32h743i_eval_sram.c driver.
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
#ifndef __STM32H743I_EVAL_SRAM_H
#define __STM32H743I_EVAL_SRAM_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32H743I_EVAL
  * @{
  */

/** @defgroup STM32H743I_EVAL_SRAM SRAM
  * @{
  */

/** @defgroup STM32H743I_EVAL_SRAM_Exported_Types SRAM Exported Types
  * @{
  */
/**
  * @}
  */

/** @defgroup STM32H743I_EVAL_SRAM_Exported_Constants SRAM Exported Constants
  * @{
  */

/**
  * @brief  SRAM status structure definition
  */
#define   SRAM_OK         ((uint8_t)0x00)
#define   SRAM_ERROR      ((uint8_t)0x01)

#define SRAM_DEVICE_ADDR  ((uint32_t)0x68000000)
#define SRAM_DEVICE_SIZE  ((uint32_t)0x200000)  /* SRAM device size in Bytes (2MB)*/

/* #define SRAM_MEMORY_WIDTH    FMC_NORSRAM_MEM_BUS_WIDTH_8*/
#define SRAM_MEMORY_WIDTH    FMC_NORSRAM_MEM_BUS_WIDTH_16

#define SRAM_BURSTACCESS     FMC_BURST_ACCESS_MODE_DISABLE
/* #define SRAM_BURSTACCESS     FMC_BURST_ACCESS_MODE_ENABLE*/

#define SRAM_WRITEBURST      FMC_WRITE_BURST_DISABLE
/* #define SRAM_WRITEBURST     FMC_WRITE_BURST_ENABLE */

#define CONTINUOUSCLOCK_FEATURE    FMC_CONTINUOUS_CLOCK_SYNC_ONLY
/* #define CONTINUOUSCLOCK_FEATURE     FMC_CONTINUOUS_CLOCK_SYNC_ASYNC */

/* DMA definitions for SRAM DMA transfer */
#define __SRAM_MDMAx_CLK_ENABLE            __HAL_RCC_MDMA_CLK_ENABLE
#define __SRAM_MDMAx_CLK_DISABLE           __HAL_RCC_MDMA_CLK_DISABLE
#define SRAM_MDMAx_CHANNEL                 MDMA_Channel1
#define SRAM_MDMAx_IRQn                    MDMA_IRQn

/**
  * @}
  */

/** @defgroup STM32H743I_EVAL_SRAM_Exported_Macro SRAM Exported Macro
  * @{
  */
/**
  * @}
  */

/** @defgroup STM32H743I_EVAL_SRAM_Exported_Functions SRAM Exported Functions
  * @{
  */
uint8_t BSP_SRAM_Init(void);
uint8_t BSP_SRAM_DeInit(void);
uint8_t BSP_SRAM_ReadData(uint32_t uwStartAddress, uint16_t *pData, uint32_t uwDataSize);
uint8_t BSP_SRAM_ReadData_DMA(uint32_t uwStartAddress, uint16_t *pData, uint32_t uwDataSize);
uint8_t BSP_SRAM_WriteData(uint32_t uwStartAddress, uint16_t *pData, uint32_t uwDataSize);
uint8_t BSP_SRAM_WriteData_DMA(uint32_t uwStartAddress, uint16_t *pData, uint32_t uwDataSize);

/* These functions can be modified in case the current settings (e.g. DMA stream)
   need to be changed for specific application needs */
void    BSP_SRAM_MspInit(SRAM_HandleTypeDef  *hsram, void *Params);
void    BSP_SRAM_MspDeInit(SRAM_HandleTypeDef  *hsram, void *Params);

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

#endif /* __STM32H743I_EVAL_SRAM_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
