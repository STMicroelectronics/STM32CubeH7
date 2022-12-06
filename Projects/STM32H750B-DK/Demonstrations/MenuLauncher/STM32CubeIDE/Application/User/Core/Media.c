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
asm(".section splash_st_section,\"a\";"
		                     ".incbin \"../../Gui/App/assets/splash_st.avi\";"
);
asm(".section default_icon_section,\"a\";"
		                     ".incbin \"../../Gui/App/assets/default_icon.bmp\";"
);
asm(".section default_logo_section,\"a\";"
		                     ".incbin \"../../Gui/App/assets/default_logo.bmp\";"
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

