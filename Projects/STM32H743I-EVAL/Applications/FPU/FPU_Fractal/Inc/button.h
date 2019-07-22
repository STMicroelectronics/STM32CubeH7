/**
  ******************************************************************************
  * @file    FPU/FPU_Fractal/Inc/button.h
  * @author  MCD Application Team
  * @brief   Header for button.c module.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
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
#ifndef __BUTTON_H
#define __BUTTON_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

#define PLAY_BUTTON       0x01
#define PAUSE_BUTTON      0x02  
#define ZOOM_IN_BUTTON    0x03
#define ZOOM_OUT_BUTTON   0x04
#define ZOOM_IN_OFF_BUTTON    0x05
#define ZOOM_OUT_OFF_BUTTON   0x06   
/*****************Play/Pause Params******************/ 

  
#define PLAY_PAUSE_BUTTON_HEIGHT 64
#define PLAY_PAUSE_BUTTON_WIDTH  64


#define PLAY_PAUSE_BUTTON_IMAGE_SIZE ((PLAY_PAUSE_BUTTON_HEIGHT * PLAY_PAUSE_BUTTON_WIDTH * ARGB8888_BYTES_PER_PIXEL)/4)
  
#define PLAY_PAUSE_BUTTON_XPOS  ((LCD_X_SIZE/2) - (PLAY_PAUSE_BUTTON_WIDTH/2))
#define PLAY_PAUSE_BUTTON_YPOS  (LCD_Y_SIZE - PLAY_PAUSE_BUTTON_HEIGHT - (PLAY_PAUSE_BUTTON_HEIGHT/2)) 

/*****************Zoom In/Out Params******************/ 

  
#define ZOOM_BUTTON_HEIGHT 64
#define ZOOM_BUTTON_WIDTH  64
  
#define ZOOM_IMAGE_SIZE  ((ZOOM_BUTTON_HEIGHT * ZOOM_BUTTON_WIDTH * ARGB8888_BYTES_PER_PIXEL)/4)

#define ZOOM_IN_BUTTON_XPOS  ((LCD_X_SIZE/2) + (3 * (ZOOM_BUTTON_WIDTH/2))) - 40
#define ZOOM_IN_BUTTON_YPOS  (LCD_Y_SIZE - ZOOM_BUTTON_HEIGHT - (ZOOM_BUTTON_HEIGHT/2)) 
  
#define ZOOM_OUT_BUTTON_XPOS  ((LCD_X_SIZE/2) - (5 * (ZOOM_BUTTON_WIDTH/2))) + 40
#define ZOOM_OUT_BUTTON_YPOS  (LCD_Y_SIZE - ZOOM_BUTTON_HEIGHT - (ZOOM_BUTTON_HEIGHT/2))   
  
  
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/* Exported Variables --------------------------------------------------------*/
extern const uint32_t Play_Button[PLAY_PAUSE_BUTTON_IMAGE_SIZE];
extern const uint32_t Pause_Button[PLAY_PAUSE_BUTTON_IMAGE_SIZE];
extern const uint32_t ZOOM_IN_Button[ZOOM_IMAGE_SIZE];
extern const uint32_t ZOOM_Out_Button[ZOOM_IMAGE_SIZE];
extern const uint32_t ZOOM_IN_Off_Button[ZOOM_IMAGE_SIZE];
extern const uint32_t ZOOM_Out_Off_Button[ZOOM_IMAGE_SIZE];

#ifdef __cplusplus
}
#endif

#endif /* __BUTTON_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
