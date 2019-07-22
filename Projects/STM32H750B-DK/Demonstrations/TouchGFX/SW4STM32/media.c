/*
;******************** (C) COPYRIGHT 2019 STMicroelectronics ********************
;* File Name          : media.c
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

/* Includes ------------------------------------------------------------------*/
#include "string.h"
#if defined (__GNUC__)

asm(".section demo_icon_section,\"a\";"
		                     ".incbin \"../../../Core/Src/assets/touchgfx_icon.bmp\";"
);
asm(".section demo_logo_section,\"a\";"
		                     ".incbin \"../../../Core/Src/assets/touchgfx_logo.bmp\";"
);
asm(".section washer_video_section,\"a\";"
		                     ".incbin \"../../../Gui/assets/videos/washer.avi\";"
);
asm(".section receipe_video_section,\"a\";"
		                     ".incbin \"../../../Gui/assets/videos/receipe.avi\";"
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
