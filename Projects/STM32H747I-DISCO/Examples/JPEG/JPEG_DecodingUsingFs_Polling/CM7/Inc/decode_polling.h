/**
  ******************************************************************************
  * @file    JPEG/JPEG_DecodingUsingFs_Polling/CM7/Inc/decode_polling.h
  * @author  MCD Application Team
  * @brief   Header for decode_polling.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
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
#ifndef __DECODE_POLLING_H
#define __DECODE_POLLING_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"


/* Exported variables --------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
uint32_t JPEG_DecodePolling(JPEG_HandleTypeDef *hjpeg, FIL *file, uint32_t DestAddress);

#endif /* __DECODE_POLLING_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
