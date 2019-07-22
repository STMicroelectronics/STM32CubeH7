/**
  ******************************************************************************
  * @file    Demonstrations/extra_modules/ST/Core/CM4/Inc/motion_detection.h
  * @author  MCD Application Team
  * @brief   Header for motion_detection.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
