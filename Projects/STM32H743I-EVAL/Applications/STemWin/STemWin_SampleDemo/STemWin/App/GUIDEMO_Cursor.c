/*********************************************************************
*          Portions COPYRIGHT 2017 STMicroelectronics                *
*          Portions SEGGER Microcontroller GmbH & Co. KG             *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2017  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.40 - Graphical user interface for embedded applications **
All  Intellectual Property rights  in the Software belongs to  SEGGER.
emWin is protected by  international copyright laws.  Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with the following terms:

The  software has  been licensed  to STMicroelectronics International
N.V. a Dutch company with a Swiss branch and its headquarters in Plan-
les-Ouates, Geneva, 39 Chemin du Champ des Filles, Switzerland for the
purposes of creating libraries for ARM Cortex-M-based 32-bit microcon_
troller products commercialized by Licensee only, sublicensed and dis_
tributed under the terms and conditions of the End User License Agreement
supplied by STMicroelectronics International N.V.
Full source code is available at: www.segger.com

We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : GUIDEMO_Cursor.c
Purpose     : shows Cursor-API
----------------------------------------------------------------------
*/


#include "GUIDEMO.h"

#if (SHOW_GUIDEMO_CURSOR && GUI_SUPPORT_CURSOR && GUI_SUPPORT_TOUCH)

/*********************************************************************
*
*       defines
*
**********************************************************************
*/

#define countof(Obj) (sizeof(Obj)/sizeof(Obj[0]))

#define NUM_CURSORS   3
#define XMAX         28

/*********************************************************************
*
*       types
*
**********************************************************************
*/

typedef struct {
  const GUI_CURSOR* pCursor;
  char              Size;
} CURSOR_INFO;

typedef struct {
  const CURSOR_INFO aCursor[NUM_CURSORS];
  const char*       pType;
} CURSORTYPE_INFO;

/*********************************************************************
*
*       static data
*
**********************************************************************
*/

static const CURSORTYPE_INFO _CursorArrow = {
  {{&GUI_CursorArrowS, 'S'},
  #if (NUM_CURSORS == 3)
    {&GUI_CursorArrowM, 'M'},
    {&GUI_CursorArrowL, 'L'}},
    "arrow cursors"
  #else
   {&GUI_CursorArrowM, 'M'}},
   "arrow\ncursors"
  #endif
};


static const CURSORTYPE_INFO _CursorArrowI = {
{{ &GUI_CursorArrowSI, 'S'},
  #if (NUM_CURSORS == 3)
    { &GUI_CursorArrowMI, 'M'},
    {&GUI_CursorArrowLI, 'L'}},
    "inverted arrow cursors"
  #else
     { &GUI_CursorArrowMI, 'M'}},
     "inverted\narrow cursors"
  #endif
};

static const CURSORTYPE_INFO _CursorCross = {
  {{&GUI_CursorCrossS, 'S'},
  #if (NUM_CURSORS == 3)
    {&GUI_CursorCrossM, 'M'},
    {&GUI_CursorCrossL, 'L'}},
    "cross cursors"
  #else
    {&GUI_CursorCrossM, 'M'}},
    "cross\ncursors"
  #endif
};

static const CURSORTYPE_INFO _CursorCrossI = {
  {{&GUI_CursorCrossSI, 'S'},
  #if (NUM_CURSORS == 3)
   {&GUI_CursorCrossMI, 'M'},
   {&GUI_CursorCrossLI, 'L'}},
    "inverted cross cursors"
  #else
    {&GUI_CursorCrossMI, 'M'}},
    "inverted\ncross cursors"
  #endif
};

/*********************************************************************
*
*       static code
*
**********************************************************************
*/
/*********************************************************************
*
*       _ShowCursorType
*/
static void _ShowCursorType(const CURSORTYPE_INFO* pCursorType, int x0, int y0) {
  const GUI_CURSOR * pCursor;
  char Char;
  int i, x, y;
  int yMax = 0;
  int yHot = 0;

  //
  // Calculate height and width of biggest cursor
  //
  for (i = 0; i < NUM_CURSORS; i++) {
    pCursor = pCursorType->aCursor[i].pCursor;
    if (pCursor->pBitmap->YSize > yMax) {
      yMax = pCursor->pBitmap->YSize;
      yHot = pCursor->yHot;
    }
  }
  GUI_SetFont(&GUI_FontRounded16);
  #if (NUM_CURSORS != 3)
    GUI_SetLBorder(x0);
  #endif
  GUI_DispStringAt(pCursorType->pType, x0, y0);
  y0 += GUI_GetFontDistY() + 1;
  GUI_SetFont(&GUI_Font13B_ASCII);
  for (i = 0; i < NUM_CURSORS; i++) {
    pCursor = pCursorType->aCursor[i].pCursor;
    Char    = pCursorType->aCursor[i].Size;
    y = y0 + yHot - pCursor->yHot;
    x = ((pCursor->pBitmap->XSize - GUI_GetCharDistX(Char)) >> 1);
    GUI_DrawBitmap(pCursor->pBitmap, x0 + XMAX * i + 5,     y);
    GUI_DispCharAt(Char,             x0 + XMAX * i + 5 + x, y0 + yMax + 2);
  }
}

/*********************************************************************
*
*       _DispCursor
*/
static void _DispCursor(void) {
  int y0, dx, dy, xSize;

  xSize = LCD_GetXSize();
  GUI_SetTextMode(GUI_TM_TRANS);
  GUI_SetFont(&GUI_FontRounded22);
  GUI_DispStringHCenterAt("Available cursors", xSize >> 1, 12);
  //
  // Display the cursors
  //
  y0 = 68;
  dx = (int)(xSize / 2.5);
  dy = 75;
  _ShowCursorType(&_CursorArrow,    5, y0);
  _ShowCursorType(&_CursorCross,    5, y0 + dy);
  _ShowCursorType(&_CursorArrowI,  dx, y0);
  _ShowCursorType(&_CursorCrossI,  dx, y0 + dy);
}

/*********************************************************************
*
*       public code
*
**********************************************************************
*/
/*********************************************************************
*
*       GUIDEMO_Cursor
*/
void GUIDEMO_Cursor(void) {
  GUIDEMO_ShowIntro("Cursor",
                    "STemWin supports\n"
                    "software cursor");
  GUIDEMO_DrawBk(1);
  GUI_CURSOR_Hide();
  _DispCursor();
  GUIDEMO_Wait(4000);
  GUI_CURSOR_Show();
  GUI_CURSOR_SetPosition(0,0);
  GUI_CURSOR_Select(&GUI_CursorArrowM);
}

#else

void GUIDEMO_Cursor(void) {}

#endif

/*************************** End of file ****************************/
