/*
;******************** (C) COPYRIGHT 2019 STMicroelectronics ********************
;* File Name          : Media.c
;* @author  MCD Application Team
;* Description        : Coretx-M7 binary code include .s file
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

/* Includes ------------------------------------------------------------------*/#include "string.h"
#if defined (__GNUC__)
asm(".section splash_st_section,\"a\";"
		                     ".incbin \"../../../Gui/App/assets/splash_st.avi\";"
);
asm(".section default_icon_section,\"a\";"
		                     ".incbin \"../../../Gui/App/assets/default_icon.bmp\";"
);
asm(".section default_logo_section,\"a\";"
		                     ".incbin \"../../../Gui/App/assets/default_logo.bmp\";"
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
