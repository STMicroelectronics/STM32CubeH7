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
