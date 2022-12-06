/**
  ******************************************************************************
  * @file    cm4_code.c
  * @author  MCD Application Team
  * @brief   SFU FLASH Interface STM32L4xx module
  *          This file provides set of firmware functions to manage SFU FLASH
  *          Interface for STM32L4xx.
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

/* Includes ------------------------------------------------------------------*/
#include "string.h"

#if defined (__GNUC__)
asm(".section .cm4_code,\"a\";"
                             ".incbin \"../../CM4/Debug/OSCILLO_SG_CM4.bin\";"
);
#endif 

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

