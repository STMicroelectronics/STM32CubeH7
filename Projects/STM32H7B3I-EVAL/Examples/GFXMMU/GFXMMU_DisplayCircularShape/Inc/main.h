/**
  ******************************************************************************
  * @file    GFXMMU/GFXMMU_DisplayCircularShape/Inc/Inc/main.h
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
#include "stm32h7b3i_eval.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/* Private define ------------------------------------------------------------*/
#define  RK070ER9427_WIDTH          800U          /* LCD PIXEL WIDTH            */
#define  RK070ER9427_HEIGHT         480U          /* LCD PIXEL HEIGHT           */

/**
  * @brief  RK070ER9427 Timing
  */
#define  RK070ER9427_HSYNC          46U    /* Horizontal synchronization */
#define  RK070ER9427_HBP            2U     /* Horizontal back porch      */
#define  RK070ER9427_HFP            210U   /* Horizontal front porch     */
#define  RK070ER9427_VSYNC          23U    /* Vertical synchronization   */
#define  RK070ER9427_VBP            2U     /* Vertical back porch        */
#define  RK070ER9427_VFP            22U    /* Vertical front porch       */
#endif /* __MAIN_H */

