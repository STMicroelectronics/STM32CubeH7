/**
  ******************************************************************************
  * @file    mt48lc4m32b2_conf.h
  * @author  MCD Application Team
  * @brief   This file contains some configurations required for the
  *          mt48lc4m32b2 SDRAM memory.
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
#ifndef mt48lc4m32b2_CONF_H
#define mt48lc4m32b2_CONF_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup Components
  * @{
  */

/** @addtogroup mt48lc4m32b2
  * @{
  */

/** @addtogroup mt48lc4m32b2_Exported_Constants
  * @{
  */
#define REFRESH_COUNT                    ((uint32_t)0x0603)   /* SDRAM refresh counter (100Mhz SD clock) */

#define MT48LC4M32B2_TIMEOUT             ((uint32_t)0xFFFF)

#ifdef __cplusplus
}
#endif

#endif /* mt48lc4m32b2_CONF_H */
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

