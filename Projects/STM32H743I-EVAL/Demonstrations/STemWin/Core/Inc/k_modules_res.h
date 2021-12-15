/**
  ******************************************************************************
  * @file    k_modules_res.h
  * @author  MCD Application Team
  * @brief   Header for k_modules_res.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __K_MODULE_RES_H
#define __K_MODULE_RES_H

#ifdef __cplusplus
 extern "C" {
#endif
 
/* Includes ------------------------------------------------------------------*/   
#include <stdlib.h>
#include "GUI.h"

#ifndef GUI_CONST_STORAGE
  #define GUI_CONST_STORAGE const
#endif

#ifndef GUI_FLASH
  #define GUI_FLASH
#endif    

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /*__K_MODULE_RES_H */

