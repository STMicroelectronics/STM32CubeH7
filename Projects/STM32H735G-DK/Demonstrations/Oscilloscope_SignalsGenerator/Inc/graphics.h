/**
  ******************************************************************************
  * @file    play_pause_button.h
  * @author  MCD Application Team
  * @brief   This file contains play/pause buttons RGB888 images.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PLAY_PAUSE_BUTTON_H
#define __PLAY_PAUSE_BUTTON_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

  
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


typedef struct 
{
  uint16_t Width;
  uint16_t Height;
  uint16_t Xpos;
  uint16_t Ypos;
  uint32_t ColorMode;  
  void  (* ButtonCallback)( void *pData);     /*!<Button Presssed callback         */
} SG_WidgetTypeDef;

typedef struct
{
  SG_WidgetTypeDef buttonParams;  
  uint32_t *On_FrameBuffer;
  uint32_t *Off_FrameBuffer;
  uint32_t State; 
  uint32_t LastPressTime_ticks;  
  uint32_t isActive;  
  SG_WaveButtonIndex_e ButtonIndex;
} SG_SwitchButtonWidgetTypeDef;

typedef struct
{
  SG_WidgetTypeDef buttonParams;  
  uint32_t *FrameBuffer;
  uint32_t ButtonIndex;
} SG_ButtonWidgetTypeDef;

typedef struct
{
  SG_WidgetTypeDef imageParams;  
  uint32_t *FrameBuffer;
} SG_BackgroundWidgetTypeDef;

/* Exported constants --------------------------------------------------------*/

/* Exported Variables --------------------------------------------------------*/

#endif /* __PLAY_PAUSE_BUTTON_H */


