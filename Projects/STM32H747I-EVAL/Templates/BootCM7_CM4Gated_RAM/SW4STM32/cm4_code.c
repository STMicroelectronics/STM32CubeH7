/**
;******************** (C) COPYRIGHT 2019 STMicroelectronics ********************
;* File Name          : cm4_code.c
;* @author  MCD Application Team
;* Description        : Coretx-M4 binary code include .c file
;*                      allowing to include the Coretx-M4 binary into the
;*                      Coretx-M7 MDK-ARM project
;******************************************************************************
;* @attention
;*
;* Copyright (c) 2019 STMicroelectronics.
;* All rights reserved.
;*
;* This software component is licensed by ST under BSD 3-Clause license,
;* the "License"; You may not use this file except in compliance with the
;* License. You may obtain a copy of the License at:
;*                        opensource.org/licenses/BSD-3-Clause
;*
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "string.h"

#if defined (__GNUC__)
asm(".section .cm4_code,\"a\";"
                             ".incbin \"../Debug_CM4/STM32H747I_EVAL_CM4.bin\";"
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
