/**
  ******************************************************************************
  * @file    bsp.h
  * @author  MCD Application Team
  * @brief   Header for bsp.c file
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
#ifndef __BSP_H
#define __BSP_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Exported types ------------------------------------------------------------*/
typedef struct
{
  uint32_t       demo_id;
  unsigned char  demo_descr[50];
} Demo_Header_t;

/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
uint8_t BSP_Config(void);

#ifdef __cplusplus
}
#endif

#endif /*__BSP_H */

