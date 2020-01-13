/**
  ******************************************************************************
  * @file    cpu_utils.h
  * @author  MCD Application Team
  * @brief   Header for cpu_utils module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2014 STMicroelectronics.
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
#ifndef _CPU_UTILS_H__
#define _CPU_UTILS_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define CALCULATION_PERIOD    1000

/* Exported functions ------------------------------------------------------- */
uint16_t osGetCPUUsage (void);

#ifdef __cplusplus
}
#endif

#endif /* _CPU_UTILS_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
