/**
  ******************************************************************************
  * @file    stm32h747i_eval_sd.h
  * @author  MCD Application Team
  * @brief   This file contains the common defines and functions prototypes for
  *          the stm32h747i_eval_sd.c driver.
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
#ifndef __STM32H747I_EVAL_SD_H
#define __STM32H747I_EVAL_SD_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"
#include "stm32h747i_eval_io.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32H747I_EVAL
  * @{
  */

/** @defgroup STM32H747I_EVAL_SD STM32H747I_EVAL_SD
  * @{
  */

/** @defgroup STM32H747I_EVAL_SD_Exported_Types Exported Types
  * @{
  */

/**
  * @brief SD Card information structure
  */
#define BSP_SD_CardInfo HAL_SD_CardInfoTypeDef
/**
  * @}
  */

/** @defgroup STM32H747I_EVAL_SD_Exported_Constants Exported Constants
  * @{
  */
/**
  * @brief  SD status structure definition
  */
#define   MSD_OK                        ((uint8_t)0x00)
#define   MSD_ERROR                     ((uint8_t)0x01)
#define   MSD_ERROR_SD_NOT_PRESENT      ((uint8_t)0x02)

/**
  * @brief  SD transfer state definition
  */
#define   SD_TRANSFER_OK                ((uint8_t)0x00)
#define   SD_TRANSFER_BUSY              ((uint8_t)0x01)

#define SD_PRESENT               ((uint8_t)0x01)
#define SD_NOT_PRESENT           ((uint8_t)0x00)

#define SD_DATATIMEOUT           ((uint32_t)100000000)


#define SD_DetectIRQHandler()             HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_8)

#ifndef USE_SD_TRANSCEIVER
#define USE_SD_TRANSCEIVER 1
#endif /* USE_SD_TRANSCEIVER */
/**
  * @}
  */

/** @defgroup STM32H747I_EVAL_SD_Exported_Macro Exported Macro
  * @{
  */
/**
  * @}
  */

/** @defgroup STM32H747I_EVAL_SD_Exported_Functions Exported Functions
  * @{
  */
uint8_t BSP_SD_Init(void);
uint8_t BSP_SD_DeInit(void);
uint8_t BSP_SD_ITConfig(void);

uint8_t BSP_SD_ReadBlocks(uint32_t *pData, uint32_t ReadAddr, uint32_t NumOfBlocks, uint32_t Timeout);
uint8_t BSP_SD_WriteBlocks(uint32_t *pData, uint32_t WriteAddr, uint32_t NumOfBlocks,  uint32_t Timeout);
uint8_t BSP_SD_ReadBlocks_DMA(uint32_t *pData, uint32_t ReadAddr, uint32_t NumOfBlocks);
uint8_t BSP_SD_WriteBlocks_DMA(uint32_t *pData, uint32_t WriteAddr, uint32_t NumOfBlocks);
uint8_t BSP_SD_Erase(uint32_t StartAddr, uint32_t EndAddr);
uint8_t BSP_SD_GetCardState(void);
void    BSP_SD_GetCardInfo(BSP_SD_CardInfo *CardInfo);
uint8_t BSP_SD_IsDetected(void);

/* These functions can be modified in case the current settings (e.g. DMA stream)
   need to be changed for specific application needs */
void    BSP_SD_MspInit(SD_HandleTypeDef *hsd, void *Params);
void    BSP_SD_MspDeInit(SD_HandleTypeDef *hsd, void *Params);
void    BSP_SD_AbortCallback(void);
void    BSP_SD_WriteCpltCallback(void);
void    BSP_SD_ReadCpltCallback(void);
void    BSP_SD_ErrorCallback(void);
void    BSP_SD_DriveTransciver_1_8V_Callback(FlagStatus status);

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

#endif /* __STM32H747I_EVAL_SD_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
