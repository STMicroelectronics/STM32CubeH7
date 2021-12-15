/**
  ******************************************************************************
  * @file    s70kl1281_conf.h
  * @author  MCD Application Team
  * @brief   S70KL1281 OctalSPI memory configuration template file.
  *          to s70kl1281_conf.h
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
#ifndef S70KL1281_CONF_H
#define S70KL1281_CONF_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

/** @addtogroup BSP
  * @{
  */
#define CONF_OSPI_RAM_ODS                         S70KL1281_CR0_DS_34   /* S70KL1281 Output Driver Strength */

#define RW_RECOVERY_TIME                          3U /* 40ns @ 60MHz */
#define DEFAULT_INITIAL_LATENCY                   6U
#define OPTIMAL_FIXED_INITIAL_LATENCY             3U
#define OPTIMAL_FIXED_INITIAL_LATENCY_REG_VAL     S70KL1281_CR0_IL_3_CLOCK
#define OPTIMAL_VARIABLE_INITIAL_LATENCY          6U
#define OPTIMAL_VARIABLE_INITIAL_LATENCY_REG_VAL  S70KL1281_CR0_IL_6_CLOCK

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* S70KL1281_CONF_H */

