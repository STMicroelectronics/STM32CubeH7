/**
;********************************************************************************
;* File Name          : cm4_code.c
;* @author  MCD Application Team
;* Description        : Coretx-M4 binary code include .c file
;*                      allowing to include the Coretx-M4 binary into the
;*                      Coretx-M7 MDK-ARM project
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

#if defined (__GNUC__)&& defined (DEBUG)
asm(".section .cm4_code,\"a\";"
                             ".incbin \"../../CM4/Debug/BootCM7_CM4Gated_RAM_CM4.bin\";"
);
#elif defined (__GNUC__)
asm(".section .cm4_code,\"a\";"
                             ".incbin \"../../CM4/Release/BootCM7_CM4Gated_RAM_CM4.bin\";"
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

