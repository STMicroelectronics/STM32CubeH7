/**
  ******************************************************************************
  * @file    is42s16800j_conf.h
  * @author  MCD Application Team
  * @brief   This file contains some configurations required for the
  *          IS42S16800J SDRAM memory.
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
#ifndef IS42S16800J_CONF_H
#define IS42S16800J_CONF_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"
   
/** @addtogroup BSP
  * @{
  */
#define REFRESH_COUNT                    ((uint32_t)0x0603)   /* SDRAM refresh counter (100Mhz SD clock) */
   
#define IS42S16800J_TIMEOUT             ((uint32_t)0xFFFF)

#ifdef __cplusplus
}
#endif

#endif /* IS42S32800J_CONF_H */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

