;********************************************************************************
;* File Name          : Media.s
;* @author  MCD Application Team
;* Description        : Coretx-M7 binary code include .s file
;******************************************************************************
;* @attention
;*
;* Copyright (c) 2019 STMicroelectronics.
;* All rights reserved.
;*
;* This software is licensed under terms that can be found in the LICENSE file
;* in the root directory of this software component.
;* If no LICENSE file comes with this software, it is provided AS-IS.
;*
;*******************************************************************************
  	  	EXPORT demo_icon		
	AREA demo_icon_section, DATA, READONLY
demo_icon
	INCBIN ..\Core\Src\assets\touchgfx_icon.bmp
		
	EXPORT demo_logo		
	AREA demo_logo_section, DATA, READONLY
demo_logo
	INCBIN ..\Core\Src\assets\touchgfx_logo.bmp
		
	END
