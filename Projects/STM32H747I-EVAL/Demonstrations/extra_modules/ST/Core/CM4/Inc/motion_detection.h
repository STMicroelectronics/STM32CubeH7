/**
  ******************************************************************************
  * @file    Demonstrations/extra_modules/ST/Core/CM4/Inc/motion_detection.h
  * @author  MCD Application Team
  * @brief   Header for motion_detection.c module
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MOTION_DETECTION_H
#define __MOTION_DETECTION_H

#include <cm_ipc.h>

/* Includes ------------------------------------------------------------------*/


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* LCD Frame Buffer address */

//#define LCD_FRAME_BUFFER                  0xD0800000 /* LCD Frame buffer of size 800x480 in ARGB8888 */

#define ABS(X)                 ((X) > 0 ? (X) : -(X))

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void MotionSimpleDetect(uint32_t xsize, uint32_t ysize, uint32_t* motion);
void MotionDetect(struct ipc_msg *msg);
void StoreMVs(uint32_t xsize, uint32_t ysize,uint32_t BuffIdx);
void MotionCompute(uint32_t xsize, uint32_t ysize);
void CalcMVs(uint32_t x0_Offset, uint32_t x1_Offset,int32_t *MVx,int32_t *MVy);

#endif /* __MOTION_DETECTION_H */

