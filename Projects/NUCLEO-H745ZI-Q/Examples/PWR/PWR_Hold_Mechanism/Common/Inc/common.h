/**
  ******************************************************************************
  * @file    PWR/PWR_Hold_Mechanism/Common/Inc/common.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
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
#ifndef __COMMON_H
#define __COMMON_H

/* Includes ------------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define HSEM_ID_0 (0U) /* HW semaphore 0*/

/* Uncomment this line to set CM7 as master core and CM4 as slave core */ 
/* #define CM4_HOLD_TEST */

/* Uncomment this line to set CM4 as master core and CM7 as slave core */ 
 #define CM7_HOLD_TEST 

/* Exported types ------------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __COMMON_H */


