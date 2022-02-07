;********************************************************************************
;* File Name          : cm7_code.s
;* @author  MCD Application Team
;* Description        : Coretx-M7 binary code include .s file
;*                      allowing to include the Coretx-M7 binary into the
;*                      Coretx-M4 MDK-ARM project
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
        AREA    CM7_CODE_AREA, DATA, READONLY
        EXPORT  cm7_code
        EXPORT  cm7_code_end

; Includes the binary file for CM7 code
cm7_code
        INCBIN STM32H745I_DISCO_CM7\Exe\STM32H745I_DISCO_CM7.bin

cm7_code_end

        END


