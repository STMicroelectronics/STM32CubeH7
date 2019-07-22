/**
  ******************************************************************************
  * @file    play_pause_button.h
  * @author  MCD Application Team
  * @brief   This file contains play/pause buttons RGB888 images.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2018 STMicroelectronics.
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
#ifndef __GRAPHICS_H
#define __GRAPHICS_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"
  
/* Exported types ------------------------------------------------------------*/
typedef enum {
  SG_DC_WAVE_BUTTON_INDEX,
  SG_SQUARE_WAVE_BUTTON_INDEX,
  SG_SINE_WAVE_BUTTON_INDEX,
  SG_TRIANGLE_WAVE_BUTTON_INDEX,
  SG_NOISE_WAVE_BUTTON_INDEX,
  SG_ESCALATOR_WAVE_BUTTON_INDEX,
  SG_MAX_WAVE_BUTTON_INDEX
} SG_WaveButtonIndex_e;

typedef enum {
  SG_UP_BUTTON_INDEX,
  SG_DOWN_BUTTON_INDEX,
  SG_MAX_UPDOWN_BUTTON_INDEX
} SG_UpDownButtonIndex_e;

typedef enum {
  RETURN,
  MAX_GENERIC_BUTTON_INDEX
} GenericButtonIndex_e;

typedef struct 
{
  uint16_t Width;
  uint16_t Height;
  uint16_t Xpos;
  uint16_t Ypos;
  uint32_t ColorMode;  
  void  (* ButtonCallback)( void *pData);     /*!<Button Presssed callback         */
} WidgetTypeDef;

typedef struct
{
  WidgetTypeDef buttonParams;  
  uint32_t *On_FrameBuffer;
  uint32_t *Off_FrameBuffer;
  uint32_t State; 
  uint32_t LastPressTime_ticks;  
  uint32_t isActive;  
  SG_WaveButtonIndex_e ButtonIndex;
} SG_SwitchButtonWidgetTypeDef;

typedef struct
{
  WidgetTypeDef buttonParams;  
  uint32_t *FrameBuffer;
  uint32_t ButtonIndex;
} ButtonWidgetTypeDef;

typedef struct
{
  WidgetTypeDef imageParams;  
  uint32_t *FrameBuffer;
} BackgroundWidgetTypeDef;

/* Exported constants --------------------------------------------------------*/

/* Exported Variables --------------------------------------------------------*/

#endif /* __GRAPHICS_H */

/*************************** End of file ****************************/

