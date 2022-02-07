/**
  ******************************************************************************
  * @file    PWR/PWR_STOP_RTC/Common/Inc/common.h
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

/* Uncomment this line to set the CM7 core as wakeup target */
/* #define WAKEUP_TARGET_CM7 */

/* Uncomment this line to set the CM4 core as wakeup target */
/* #define WAKEUP_TARGET_CM4 */

/* Uncomment this line to set the CM7 and CM4 cores as wakeup targets */
 #define WAKEUP_TARGET_ALL_CPU 

/* Exported types ------------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __COMMON_H */


