/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2015  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.28 - Graphical user interface for embedded applications **
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
File        : ST_AnimatedIconView.h
Purpose     : ICONVIEW include
--------------------END-OF-HEADER-------------------------------------
*/

#ifndef ST_ANIMATEDICONVIEW_H
#define ST_ANIMATEDICONVIEW_H

#include "WM.h"
#include "DIALOG_Intern.h"      /* Req. for Create indirect data structure */
#include "WIDGET.h"

#if GUI_WINSUPPORT

#if defined(__cplusplus)
  extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

#define SINGLE_TEXT                 0    
#define DUAL_TEXT                   1  
#define MAX_LENGTH                  20

/*********************************************************************
*
*       Types
*
**********************************************************************
*/
typedef WM_HMEM ST_ANIMATED_ICONVIEW_Handle;

/*********************************************************************
*
*       Public functions
*
**********************************************************************
*/
ST_ANIMATED_ICONVIEW_Handle ST_AnimatedIconView_CreateEx      (int x0, int y0, int xSize, int ySize, WM_HWIN hParent, int WinFlags, int ExFlags, int Id, int xSizeItems, int ySizeItems, int delay, int FrameNbr);
ST_ANIMATED_ICONVIEW_Handle ST_AnimatedIconView_CreateUser    (int x0, int y0, int xSize, int ySize, WM_HWIN hParent, int WinFlags, int ExFlags, int Id, int xSizeItems, int ySizeItems, int NumExtraBytes, int delay, int FrameNbr);
ST_ANIMATED_ICONVIEW_Handle ST_AnimatedIconView_CreateIndirect(const GUI_WIDGET_CREATE_INFO * pCreateInfo, WM_HWIN hWinParent, int x0, int y0, WM_CALLBACK * cb, int delay, int FrameNbr);

void ST_AnimatedIconView_DeleteItem              (ST_ANIMATED_ICONVIEW_Handle hObj, unsigned Index);
U32  ST_AnimatedIconView_GetItemUserData         (ST_ANIMATED_ICONVIEW_Handle hObj, int Index);
int  ST_AnimatedIconView_GetNumItems             (ST_ANIMATED_ICONVIEW_Handle hObj);
int  ST_AnimatedIconView_GetItemText             (ST_ANIMATED_ICONVIEW_Handle hObj, int Index, char * pBuffer, int MaxSize);
int  ST_AnimatedIconView_GetSel                  (ST_ANIMATED_ICONVIEW_Handle hObj);
int  ST_AnimatedIconView_GetUserData             (ST_ANIMATED_ICONVIEW_Handle hObj, void * pDest, int NumBytes);
void ST_AnimatedIconView_SetBkColor              (ST_ANIMATED_ICONVIEW_Handle hObj, int Index, GUI_COLOR Color);
void ST_AnimatedIconView_SetFont                 (ST_ANIMATED_ICONVIEW_Handle hObj, const GUI_FONT * pFont);
void ST_AnimatedIconView_SetDualFont             (ST_ANIMATED_ICONVIEW_Handle hObj, const GUI_FONT * pFont1, const GUI_FONT * pFont2);
void ST_AnimatedIconView_SetFrame                (ST_ANIMATED_ICONVIEW_Handle hObj, int Coord, int Value);
void ST_AnimatedIconView_SetItemText             (ST_ANIMATED_ICONVIEW_Handle hObj, int Index, const char * pText);
void ST_AnimatedIconView_SetDualItemText         (ST_ANIMATED_ICONVIEW_Handle hObj, int Index, const char * pText, char * pText1);
void ST_AnimatedIconView_SetItemUserData         (ST_ANIMATED_ICONVIEW_Handle hObj, int Index, U32 UserData);
void ST_AnimatedIconView_SetSel                  (ST_ANIMATED_ICONVIEW_Handle hObj, int Sel);
void ST_AnimatedIconView_SetSpace                (ST_ANIMATED_ICONVIEW_Handle hObj, int Coord, int Value);
void ST_AnimatedIconView_SetIconAlign            (ST_ANIMATED_ICONVIEW_Handle hObj, int IconAlign);
void ST_AnimatedIconView_SetTextAlign            (ST_ANIMATED_ICONVIEW_Handle hObj, int TextAlign);
void ST_AnimatedIconView_SetTextColor            (ST_ANIMATED_ICONVIEW_Handle hObj, int Index, GUI_COLOR Color);
void ST_AnimatedIconView_SetDualTextColor        (ST_ANIMATED_ICONVIEW_Handle hObj, int Index, GUI_COLOR Color, GUI_COLOR Color1);
int  ST_AnimatedIconView_SetUserData             (ST_ANIMATED_ICONVIEW_Handle hObj, const void * pSrc, int NumBytes);
void ICONVIEW_SetWrapMode_ST             (ST_ANIMATED_ICONVIEW_Handle hObj, GUI_WRAPMODE WrapMode);

int  ST_AnimatedIconView_SetIcon(ST_ANIMATED_ICONVIEW_Handle hObj, int Index, const GUI_BITMAP ** pOpenBitmap, const GUI_BITMAP ** pCloseBitmap, int size, int index);
int  ST_AnimatedIconView_AddIcon(ST_ANIMATED_ICONVIEW_Handle hObj, const GUI_BITMAP ** pOpenBitmap, const GUI_BITMAP ** pCloseBitmap, const char * pText);
void ST_AnimatedIconView_SetNextButton(ST_ANIMATED_ICONVIEW_Handle hObj);

void ICONVIEW_Callback_ST(WM_MESSAGE * pMsg);

#if defined(__cplusplus)
  }
#endif

#endif  // GUI_WINSUPPORT
#endif  // ST_ANIMATEDICONVIEW_H

/*************************** End of file ****************************/
