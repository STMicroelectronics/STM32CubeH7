/*
;********************************************************************************
;* File Name          : Media.c
;* @author  MCD Application Team
;* Description        : Coretx-M7 binary code include .s file
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

/* Includes ------------------------------------------------------------------*/#include "string.h"
#if defined (__GNUC__)

asm(".section demo_icon_section,\"a\";"
		                     ".incbin \"../../Core/Src/assets/stemwin_icon.bmp\";"
);
asm(".section demo_logo_section,\"a\";"
		                     ".incbin \"../../Core/Src/assets/stemwin_logo.bmp\";"
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

