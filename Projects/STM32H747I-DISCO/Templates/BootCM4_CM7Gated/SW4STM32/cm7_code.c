/**
;********************************************************************************
;* File Name          : cm7_code.c
;* @author  MCD Application Team
;* Description        : Coretx-M7 binary code include .c file
;*                      allowing to include the Coretx-M7 binary into the
;*                      Coretx-M4 MDK-ARM project
;******************************************************************************
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

/* Includes ------------------------------------------------------------------*/
#include "string.h"

#if defined (__GNUC__)
asm(".section .cm7_code,\"a\";"
                             ".incbin \"../Debug_CM7/STM32H747I_DISCO_CM7.bin\";"
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

