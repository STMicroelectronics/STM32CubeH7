;********************************************************************************
;* File Name          : cm4_code.s
;* @author  MCD Application Team
;* Description        : Coretx-M4 binary code include .s file
;*                      allowing to include the Coretx-M4 binary into the
;*                      Coretx-M7 MDK-ARM project
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
        AREA    CM4_CODE_AREA, DATA, READONLY
        EXPORT  cm4_code
        EXPORT  cm4_code_end

; Includes the binary file for CM4 code

cm4_code 
        INCBIN OSCILLO_SG_CM4/Exe/STM32H745I_OSCILLO_SG_CM4.bin   

cm4_code_end

        END


