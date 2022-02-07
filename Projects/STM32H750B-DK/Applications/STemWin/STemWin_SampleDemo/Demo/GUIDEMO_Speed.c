/*********************************************************************
*          Portions COPYRIGHT(c) 2019 STMicroelectronics                *
*          Portions SEGGER Microcontroller GmbH & Co. KG             *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2015  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.24 - Graphical user interface for embedded applications **
All  Intellectual Property rights  in the Software belongs to  SEGGER.
emWin is protected by  international copyright laws.  Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with the following terms:

The  software has  been licensed  to STMicroelectronics International
N.V. a Dutch company with a Swiss branch and its headquarters in Plan-
les-Ouates, Geneva, 39 Chemin du Champ des Filles, Switzerland for the
purposes of creating libraries for ARM Cortex-M-based 32-bit microcon_
troller products commercialized by Licensee only, sublicensed and dis_
tributed under the terms and conditions of the End User License Agree_
ment supplied by STMicroelectronics International N.V.
Full source code is available at: www.segger.com

We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : GUIDEMO_Speed.c
Purpose     : Speed demo
----------------------------------------------------------------------
*/

/**
  ******************************************************************************
  * @file    GUIDEMO_Speed.c
  * @author  MCD Application Team
  * @brief   Speed demo
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

#include <stdlib.h>  /* for rand */

#include "GUIDEMO.h"

#if (SHOW_GUIDEMO_SPEED)

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
static const GUI_COLOR _aColor[8] = {
  0x000000, 
  0x0000FF, 
  0x00FF00, 
  0x00FFFF, 
  0xFF0000, 
  0xFF00FF, 
  0xFFFF00, 
  0xFFFFFF
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
/*********************************************************************
*
*       _GetPixelsPerSecond
*/
static U32 _GetPixelsPerSecond(void) {
  GUI_COLOR Color, BkColor;
  U32 x0, y0, x1, y1, xSize, ySize;
  I32 t, t0;
  U32 Cnt, PixelsPerSecond, PixelCnt;

  //
  // Find an area which is not obstructed by any windows
  //
  xSize   = LCD_GetXSize();
  ySize   = LCD_GetYSize();
  Cnt     = 0;
  x0      = 0;
  x1      = xSize - 1;
  y0      = 65;
  y1      = ySize - 60 - 1;
  Color   = GUI_GetColor();
  BkColor = GUI_GetBkColor();
  GUI_SetColor(BkColor);
  //
  // Repeat fill as often as possible in 100 ms
  //
  t0 = GUIDEMO_GetTime();
  do {
    GUI_FillRect(x0, y0, x1, y1);
    Cnt++;
    t = GUIDEMO_GetTime();    
  } while ((t - (t0 + 100)) <= 0);
  //
  // Compute result
  //
  t -= t0;
  PixelCnt = (x1 - x0 + 1) * (y1 - y0 + 1) * Cnt;
  PixelsPerSecond = PixelCnt / t * 1000;   
  GUI_SetColor(Color);
  return PixelsPerSecond;
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       GUIDEMO_Speed
*/
void GUIDEMO_Speed(void) {
  int      TimeStart, i;
  U32      PixelsPerSecond;
  unsigned aColorIndex[8];
  int      xSize, ySize, vySize;
  GUI_RECT Rect, ClipRect;
  char     cText[40] = { 0 };

  xSize  = LCD_GetXSize();
  ySize  = LCD_GetYSize();
  vySize = LCD_GetVYSize();
#if GUI_SUPPORT_CURSOR
  GUI_CURSOR_Hide();
#endif
  if (vySize > ySize) {
    ClipRect.x0 = 0;
    ClipRect.y0 = 0;
    ClipRect.x1 = xSize;
    ClipRect.y1 = ySize;
    GUI_SetClipRect(&ClipRect);
  }
  GUIDEMO_ShowIntro("High speed",
                    "Multi layer clipping\n"
                    "Highly optimized drivers");
  for (i = 0; i< 8; i++) {
    aColorIndex[i] = GUI_Color2Index(_aColor[i]);
  }  
  TimeStart = GUIDEMO_GetTime();
  for (i = 0; ((GUIDEMO_GetTime() - TimeStart) < 5000) && (GUIDEMO_CheckCancel() == 0); i++) {
    GUI_SetColorIndex(aColorIndex[i&7]);
    //
    // Calculate random positions
    //
    Rect.x0 = rand() % xSize - xSize / 2;
    Rect.y0 = rand() % ySize - ySize / 2;
    Rect.x1 = Rect.x0 + 20 + rand() % xSize;
    Rect.y1 = Rect.y0 + 20 + rand() % ySize;
    GUI_FillRect(Rect.x0, Rect.y0, Rect.x1, Rect.y1);
    //
    // Clip rectangle to visible area and add the number of pixels (for speed computation)
    //
    if (Rect.x1 >= xSize) {
      Rect.x1 = xSize - 1;
    }
    if (Rect.y1 >= ySize) {
      Rect.y1 = ySize - 1;
    }
    if (Rect.x0 < 0 ) {
      Rect.x0 = 0;
    }
    if (Rect.y1 < 0) {
      Rect.y1 = 0;
    }
    GUI_Exec();
    //
    // Allow short breaks so we do not use all available CPU time ...
    //
  }
  GUIDEMO_NotifyStartNext();
  PixelsPerSecond = _GetPixelsPerSecond();
  GUI_SetClipRect(NULL);
  GUIDEMO_DrawBk(0);
  GUI_SetColor(GUI_WHITE);
  GUI_SetTextMode(GUI_TM_TRANS);
  GUI_SetFont(&GUI_FontRounded22);
  GUI_DrawBitmap(&bmSTLogo70x35, 5, 5);
  GUIDEMO_AddStringToString(cText, "Pixels/sec: ");
  GUIDEMO_AddIntToString(cText, PixelsPerSecond);
  GUI_DispStringHCenterAt(cText, xSize >> 1, (ySize - GUI_GetFontSizeY()) >> 1);
  GUIDEMO_Delay(4000);
#if GUI_SUPPORT_CURSOR
  /* Cursor disabled */
  /* GUI_CURSOR_Show(); */
#endif
}

#else

void GUIDEMO_Speed(void) {}

#endif

