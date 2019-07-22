/**
  ******************************************************************************
  * @file    JPEG/JPEG_DecodingUsingFs_Interrupt/Inc/decode_interrupt.h
  * @author  MCD Application Team
  * @brief   Header for decode_interrupt.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
