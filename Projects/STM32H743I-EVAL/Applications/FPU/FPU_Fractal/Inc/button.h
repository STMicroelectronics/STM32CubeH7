/**
  ******************************************************************************
  * @file    FPU/FPU_Fractal/Inc/button.h
  * @author  MCD Application Team
  * @brief   Header for button.c module.
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
