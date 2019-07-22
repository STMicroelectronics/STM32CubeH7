;******************** (C) COPYRIGHT 2019 STMicroelectronics ********************
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
;* This software component is licensed by ST under BSD 3-Clause license,
;* the "License"; You may not use this file except in compliance with the
;* License. You may obtain a copy of the License at:
;*                        opensource.org/licenses/BSD-3-Clause
;*
;******************************************************************************
        AREA    CM7_CODE_AREA, DATA, READONLY
        EXPORT  cm7_code
        EXPORT  cm7_code_end

; Includes the binary file for CM7 code
cm7_code
        INCBIN STM32H745I_DISCO_CM7\Exe\STM32H745I_DISCO_CM7.bin

cm7_code_end

        END


