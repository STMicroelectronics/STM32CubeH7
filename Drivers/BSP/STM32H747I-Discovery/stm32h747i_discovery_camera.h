/**
  ******************************************************************************
  * @file    stm32h747i_discovery.h
  * @author  MCD Application Team
  * @brief   This file contains the common defines and functions prototypes for
  *          the STM32H747I_DISCOVERY_camera.c driver.
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
#ifndef __STM32H747I_DISCOVERY_CAMERA_H
#define __STM32H747I_DISCOVERY_CAMERA_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
/* Include Camera component Driver */
#include "../Components/ov9655/ov9655.h"
#include "stm32h747i_discovery.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32H747I_DISCOVERY
  * @{
  */
    
/** @addtogroup STM32H747I_DISCOVERY_CAMERA
  * @{
  */ 
   
/** @defgroup STM32H747I_DISCOVERY_CAMERA_Exported_Constants Exported Constants
  * @{
  */
  
/** 
  * @brief  Camera State structures definition  
  */
#define CAMERA_OK             0x00
#define CAMERA_ERROR          0x01
#define CAMERA_TIMEOUT        0x02
#define CAMERA_NOT_DETECTED   0x03
#define CAMERA_NOT_SUPPORTED  0x04

/**
  * @brief  Camera Image rotation definition
  *         in frame buffer for LCD Display.
  */
#define CAMERA_NO_ROTATION        0x00
#define CAMERA_ROTATION_90        0x01
#define CAMERA_ROTATION_INVALID   0x02

#define RESOLUTION_R160x120      CAMERA_R160x120      /* QQVGA Resolution     */
#define RESOLUTION_R320x240      CAMERA_R320x240      /* QVGA Resolution      */
#define RESOLUTION_R480x272      CAMERA_R480x272      /* 480x272 Resolution   */
#define RESOLUTION_R640x480      CAMERA_R640x480      /* VGA Resolution       */

#define CAMERA_VGA_RES_X          640
#define CAMERA_VGA_RES_Y          480
#define CAMERA_480x272_RES_X      480
#define CAMERA_480x272_RES_Y      272
#define CAMERA_QVGA_RES_X         320
#define CAMERA_QVGA_RES_Y         240
#define CAMERA_QQVGA_RES_X        160
#define CAMERA_QQVGA_RES_Y        120

#define BSP_CAMERA_IRQHandler      DCMI_IRQHandler
#define BSP_CAMERA_DMA_IRQHandler  DMA2_Stream3_IRQHandler
 
/* DCMI DMA Stream definitions */
#define CAMERA_DCMI_DMAx_CLK_ENABLE         __HAL_RCC_DMA2_CLK_ENABLE
#define CAMERA_DCMI_DMAx_STREAM             DMA2_Stream3
#define CAMERA_DCMI_DMAx_IRQ                DMA2_Stream3_IRQn 

/** 
  * @brief  CAMERA FB_StartAddress  
  */
#define CAMERA_FB_START_ADDRESS       ((uint32_t)0xD0600000)
 
/**
  * @}
  */

/** @addtogroup STM32H747I_DISCOVERY_CAMERA_Exported_Functions
  * @{
  */    
uint8_t BSP_CAMERA_Init(uint32_t Resolution);  
uint8_t BSP_CAMERA_DeInit(void);
void    BSP_CAMERA_ContinuousStart(uint8_t *buff);
void    BSP_CAMERA_SnapshotStart(uint8_t *buff);
void    BSP_CAMERA_Suspend(void);
void    BSP_CAMERA_Resume(void);
uint8_t BSP_CAMERA_Stop(void); 
void    BSP_CAMERA_PwrUp(void);
void    BSP_CAMERA_PwrDown(void);
void    BSP_CAMERA_LineEventCallback(void);
void    BSP_CAMERA_VsyncEventCallback(void);
void    BSP_CAMERA_FrameEventCallback(void);
void    BSP_CAMERA_ErrorCallback(void);

/* Camera features functions prototype */
void    BSP_CAMERA_ContrastBrightnessConfig(uint32_t contrast_level, uint32_t brightness_level);
void    BSP_CAMERA_BlackWhiteConfig(uint32_t Mode);
void    BSP_CAMERA_ColorEffectConfig(uint32_t Effect);
 
/* These functions can be modified in case the current settings (e.g. DMA stream)
   need to be changed for specific application needs */
void BSP_CAMERA_MspInit(DCMI_HandleTypeDef *hdcmi, void *Params);
void BSP_CAMERA_MspDeInit(DCMI_HandleTypeDef *hdcmi, void *Params);
uint8_t  BSP_CAMERA_SetRotation(uint32_t rotation);
uint32_t BSP_CAMERA_GetRotation(void);

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

#endif /* __STM32H747I_DISCOVERY_CAMERA_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
