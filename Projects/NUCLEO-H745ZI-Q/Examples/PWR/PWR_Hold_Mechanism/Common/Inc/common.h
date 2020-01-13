/**
  ******************************************************************************
  * @file    PWR/PWR_Hold_Mechanism/Common/Inc/common.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2018 STMicroelectronics.
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

