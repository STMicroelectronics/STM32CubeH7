;******************** (C) COPYRIGHT 2019 STMicroelectronics ********************
;* File Name          : Media.s
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
;******************************************************************************
  	EXPORT demo_icon		
	AREA demo_icon_section, DATA, READONLY
demo_icon
	INCBIN ..\Core\Src\assets\touchgfx_icon.bmp
		
	EXPORT demo_logo		
	AREA demo_logo_section, DATA, READONLY
demo_logo
	INCBIN ..\Core\Src\assets\touchgfx_logo.bmp

	EXPORT washer_video		
	AREA washer_video_section, DATA, READONLY
washer_video
	INCBIN ..\Gui\assets\videos\washer.avi

	EXPORT receipe_video		
	AREA receipe_video_section, DATA, READONLY
receipe_video
	INCBIN ..\Gui\assets\videos\receipe.avi
		
	END