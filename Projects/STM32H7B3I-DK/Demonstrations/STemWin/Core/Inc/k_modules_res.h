/**
  ******************************************************************************
  * @file    k_modules_res.h
  * @author  MCD Application Team
  * @brief   Header for k_modules_res.c file
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
//extern GUI_CONST_STORAGE GUI_BITMAP bmgraphic_effects_clicked;
//extern GUI_CONST_STORAGE GUI_BITMAP bmvideo_game_clicked;
extern GUI_CONST_STORAGE GUI_BITMAP bmgraphic_effects_idle;
extern GUI_CONST_STORAGE GUI_BITMAP bmvideo_game_idle;

/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /*__K_MODULE_RES_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
