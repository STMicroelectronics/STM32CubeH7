/**
  ******************************************************************************
  * @file    MenuLauncher.h
  * @author  MCD Application Team
  * @brief   headr file for MenuLauncher.c module 
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

#ifndef MENULAUNCHER_H__
#define MENULAUNCHER_H__

/*********************************************************************
*
*       Includes
*
**********************************************************************
*/
#include <stdlib.h>

#include "GUI.h"

/*********************************************************************
*
*       Exported constants
*
**********************************************************************
*/
#ifndef GUI_CONST_STORAGE
  #define GUI_CONST_STORAGE const
#endif

/*********************************************************************
*
*       Exported variables
*
**********************************************************************
*/
typedef union
{
  struct
  {
    uint32_t     A1 : 15;
    uint32_t     B1 : 16;
    uint32_t     Reserved : 1;
  }b;
  uint32_t d32;
}CALIBRATION_Data1Typedef;

typedef union
{
  struct
  {
    uint32_t      A2 : 15;
    uint32_t      B2 : 16;
    uint32_t      IsCalibrated : 1;
  }b;
  uint32_t d32;

}CALIBRATION_Data2Typedef;

extern GUI_CONST_STORAGE GUI_BITMAP bmSTemWin;
extern GUI_CONST_STORAGE GUI_BITMAP bmEmbeddedWizard;
extern GUI_CONST_STORAGE GUI_BITMAP bmTouchGFX;
extern GUI_CONST_STORAGE GUI_BITMAP bmbackground;
extern GUI_CONST_STORAGE GUI_BITMAP bminfo_screen;
extern GUI_CONST_STORAGE GUI_BITMAP bminfo_icon;
extern GUI_CONST_STORAGE GUI_BITMAP bmmessage_box;
extern GUI_CONST_STORAGE GUI_BITMAP bmdialog_box;
extern GUI_CONST_STORAGE GUI_BITMAP bmpbar_bck;
extern GUI_CONST_STORAGE GUI_BITMAP bmarrow_right;
extern GUI_CONST_STORAGE GUI_BITMAP bmreflect;
extern GUI_CONST_STORAGE GUI_BITMAP bmmcu_icon;
extern GUI_CONST_STORAGE GUI_BITMAP bmboard_icon;
extern GUI_CONST_STORAGE GUI_BITMAP bmcpu_icon;
extern GUI_CONST_STORAGE GUI_BITMAP bmfirmware_icon;
extern GUI_CONST_STORAGE GUI_FONT   GUI_FontLucidaSans18;
extern GUI_CONST_STORAGE GUI_FONT   GUI_FontLucidaSans25;
extern GUI_CONST_STORAGE GUI_FONT   GUI_FontLucidaSans30;

#endif  /* Avoid multiple inclusion */
