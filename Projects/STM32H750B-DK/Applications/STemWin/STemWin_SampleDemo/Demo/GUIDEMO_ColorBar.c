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
File        : GUIDEMO_ColorBar.c
Purpose     : Draws color bars
----------------------------------------------------------------------
*/

/**
  ******************************************************************************
  * @file    GUIDEMO_ColorBar.c
  * @author  MCD Application Team
  * @brief   Draws color bars
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

#include "GUIDEMO.h"

/*********************************************************************
*
*       GUIDEMO_ShowColorBar
*
**********************************************************************
*/

#if (SHOW_GUIDEMO_COLORBAR)

#define GRADIENT_START_X 60
#define Y_START          43
#define Y_STEP           10

#define TIME_PAUSE       500
#define TIME_STEP        500
#define TIME_RUN         (TIME_PAUSE + TIME_STEP) * 6

void GUIDEMO_ColorBar(void) {
  GUI_COLOR ColorStartBlack, ColorStartWhite;
  char      acText[80] = { 0 };
  int       NumColors, BitsPerPixel, xSize, ySize;
  int       Time, TimeStart;
  int       Dir, Index;

  xSize = LCD_GetXSize();
  ySize = LCD_GetYSize();
  GUIDEMO_ShowIntro("Color bar",
                    "STemWin features an integrated\n"
                    "color management which automatically finds\n"
                    "the best available color for any logical color");
  GUIDEMO_DrawBk(1);
  //
  // Heading
  //
	GUI_SetColor(GUI_WHITE);
	GUI_SetFont(&GUI_FontRounded22);
	GUI_DispStringHCenterAt("Color bars", xSize >> 1, 12);
  GUI_SetFont(&GUI_Font16_ASCII);
  //
  // Colors
  //
  GUI_DispStringAt("Red",     1, Y_START);
  GUI_DispStringAt("Green",   1, Y_START + Y_STEP *  2);
  GUI_DispStringAt("Blue",    1, Y_START + Y_STEP *  4);
  GUI_DispStringAt("Grey",    1, Y_START + Y_STEP *  5 + (Y_STEP >> 1));
  GUI_DispStringAt("Yellow",  1, Y_START + Y_STEP *  7);
  GUI_DispStringAt("Cyan",    1, Y_START + Y_STEP *  9);
  GUI_DispStringAt("Magenta", 1, Y_START + Y_STEP * 11);
  //
  // Additional Information
  //
  GUI_SetFont(&GUI_Font8_ASCII);
  //
  // LCD Controller
  //
  #ifdef LCD_CONTROLLER
    GUIDEMO_AddStringToString(acText, "LCD Controller: ");
    GUIDEMO_AddStringToString(acText, LCD_CONTROLLER);
    GUI_DispStringAt         (acText, 12, ySize - 45);
    GUIDEMO_ClearText        (acText);
  #endif
  //
  // BPP and number of colors
  //
  BitsPerPixel = LCD_GetBitsPerPixel();
  GUIDEMO_AddIntToString   (acText, BitsPerPixel);
  GUIDEMO_AddStringToString(acText, " bpp");
  NumColors = LCD_GetDevCap(LCD_DEVCAP_NUMCOLORS);
  if (NumColors) {
    GUIDEMO_AddStringToString(acText, ", ");
    GUIDEMO_AddIntToString   (acText, NumColors);
    GUIDEMO_AddStringToString(acText, " colors");
  }
  GUI_DispStringAt(acText, 12, ySize - 25);
  //
  // Gradients
  //
  TimeStart = GUIDEMO_GetTime();
  while (((GUIDEMO_GetTime() - TimeStart) < TIME_RUN) && (GUIDEMO_CheckCancel() == 0)) {
    Time  = (GUIDEMO_GetTime() - TimeStart) % ((TIME_PAUSE + TIME_STEP) << 1);
    Dir   = Time / (TIME_PAUSE + TIME_STEP);
    Time -= Dir * (TIME_PAUSE + TIME_STEP);
    if (Time > TIME_PAUSE) {
      continue;
    }
    Index = ((Time * 0xFF) / TIME_STEP) ^ (Dir * 0xFF);
    ColorStartBlack = 0x000000 + 0x010101 * Index;
    ColorStartWhite = 0xFFFFFF - ColorStartBlack;
    GUI_DrawGradientH(GRADIENT_START_X, Y_START + Y_STEP *  0, xSize, (Y_START + Y_STEP *  1) - 1, GUI_RED,     ColorStartBlack);
    GUI_DrawGradientH(GRADIENT_START_X, Y_START + Y_STEP *  1, xSize, (Y_START + Y_STEP *  2) - 1, GUI_RED,     ColorStartWhite);
    GUI_DrawGradientH(GRADIENT_START_X, Y_START + Y_STEP *  2, xSize, (Y_START + Y_STEP *  3) - 1, GUI_GREEN,   ColorStartBlack);
    GUI_DrawGradientH(GRADIENT_START_X, Y_START + Y_STEP *  3, xSize, (Y_START + Y_STEP *  4) - 1, GUI_GREEN,   ColorStartWhite);
    GUI_DrawGradientH(GRADIENT_START_X, Y_START + Y_STEP *  4, xSize, (Y_START + Y_STEP *  5) - 1, GUI_BLUE,    ColorStartBlack);
    GUI_DrawGradientH(GRADIENT_START_X, Y_START + Y_STEP *  5, xSize, (Y_START + Y_STEP *  6) - 1, GUI_BLUE,    ColorStartWhite);
    GUI_DrawGradientH(GRADIENT_START_X, Y_START + Y_STEP *  6, xSize, (Y_START + Y_STEP *  7) - 1, GUI_GRAY,    ColorStartBlack);
    GUI_DrawGradientH(GRADIENT_START_X, Y_START + Y_STEP *  7, xSize, (Y_START + Y_STEP *  8) - 1, GUI_YELLOW,  ColorStartWhite);
    GUI_DrawGradientH(GRADIENT_START_X, Y_START + Y_STEP *  8, xSize, (Y_START + Y_STEP *  9) - 1, GUI_YELLOW,  ColorStartBlack);
    GUI_DrawGradientH(GRADIENT_START_X, Y_START + Y_STEP *  9, xSize, (Y_START + Y_STEP * 10) - 1, GUI_CYAN,    ColorStartWhite);
    GUI_DrawGradientH(GRADIENT_START_X, Y_START + Y_STEP * 10, xSize, (Y_START + Y_STEP * 11) - 1, GUI_CYAN,    ColorStartBlack);
    GUI_DrawGradientH(GRADIENT_START_X, Y_START + Y_STEP * 11, xSize, (Y_START + Y_STEP * 12) - 1, GUI_MAGENTA, ColorStartWhite);
    GUI_DrawGradientH(GRADIENT_START_X, Y_START + Y_STEP * 12, xSize, (Y_START + Y_STEP * 13) - 1, GUI_MAGENTA, ColorStartBlack);
    GUI_Exec();
  }
}

#else

void GUIDEMO_ColorBar(void) {}

#endif

