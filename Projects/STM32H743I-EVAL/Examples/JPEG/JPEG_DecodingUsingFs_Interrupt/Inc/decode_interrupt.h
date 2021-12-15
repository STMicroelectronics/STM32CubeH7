/**
  ******************************************************************************
  * @file    JPEG/JPEG_DecodingUsingFs_Interrupt/Inc/decode_interrupt.h
  * @author  MCD Application Team
  * @brief   Header for decode_interrupt.c module
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DECODE_INTERRUPT_H
#define __DECODE_INTERRUPT_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Exported variables --------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
uint32_t JPEG_Decode_IT(JPEG_HandleTypeDef *hjpeg, FIL *file, uint32_t DestAddress);
uint32_t JPEG_InputHandler(JPEG_HandleTypeDef *hjpeg);

#endif /* __DECODE_INTERRUPT_H */

