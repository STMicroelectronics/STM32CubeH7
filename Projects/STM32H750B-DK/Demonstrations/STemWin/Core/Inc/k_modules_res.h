/**
  ******************************************************************************
  * @file    k_modules_res.h
  * @author  MCD Application Team
  * @brief   Header for k_modules_res.c file
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
#ifndef __K_MODULE_RES_H
#define __K_MODULE_RES_H

#ifdef __cplusplus
 extern "C" {
#endif
 
/* Includes ------------------------------------------------------------------*/   
#include <stdlib.h>
#include "GUI.h"

#ifndef GUI_CONST_STORAGE
  #define GUI_CONST_STORAGE const
#endif

#ifndef GUI_FLASH
  #define GUI_FLASH
#endif    

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
extern GUI_CONST_STORAGE GUI_FONT GUI_FontArial18;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontArialRoundedBold18;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontArialRoundedBold20;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontArialRoundedBold24;

extern GUI_CONST_STORAGE GUI_BITMAP bmbkc_16bit;
extern GUI_CONST_STORAGE GUI_BITMAP bmmenu;
extern GUI_CONST_STORAGE GUI_BITMAP bminfo;
extern GUI_CONST_STORAGE GUI_BITMAP bmaudio_player_clicked;
extern GUI_CONST_STORAGE GUI_BITMAP bmclock_date_clicked;
extern GUI_CONST_STORAGE GUI_BITMAP bmgraphic_effects_clicked;
extern GUI_CONST_STORAGE GUI_BITMAP bmvideo_game_clicked;
extern GUI_CONST_STORAGE GUI_BITMAP bmvideo_player_clicked;
extern GUI_CONST_STORAGE GUI_BITMAP bmaudio_player_idle;
extern GUI_CONST_STORAGE GUI_BITMAP bmclock_date_idle;
extern GUI_CONST_STORAGE GUI_BITMAP bmgraphic_effects_idle;
extern GUI_CONST_STORAGE GUI_BITMAP bmvideo_game_idle;
extern GUI_CONST_STORAGE GUI_BITMAP bmvideo_player_idle;

/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /*__K_MODULE_RES_H */

