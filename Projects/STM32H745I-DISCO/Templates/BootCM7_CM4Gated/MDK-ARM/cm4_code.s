;******************** (C) COPYRIGHT 2019 STMicroelectronics ********************
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
;* This software component is licensed by ST under BSD 3-Clause license,
;* the "License"; You may not use this file except in compliance with the
;* License. You may obtain a copy of the License at:
;*                        opensource.org/licenses/BSD-3-Clause
;*
;******************************************************************************
        AREA    CM4_CODE_AREA, DATA, READONLY
        EXPORT  cm4_code
        EXPORT  cm4_code_end

; Includes the binary file for CM4 code
cm4_code
        INCBIN STM32H745I_DISCO_CM4\Exe\STM32H745I_DISCO_CM4.bin

cm4_code_end

        END


