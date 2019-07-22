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
  	EXPORT splash_st		
	AREA splash_st_section, DATA, READONLY
splash_st
	INCBIN ..\Gui\App\assets\splash_st.avi
		
	EXPORT default_icon		
	AREA default_icon_section, DATA, READONLY
default_icon
	INCBIN ..\Gui\App\assets\default_icon.bmp

	EXPORT default_logo		
	AREA default_logo_section, DATA, READONLY
default_logo
	INCBIN ..\Gui\App\assets\default_logo.bmp
		
	END
