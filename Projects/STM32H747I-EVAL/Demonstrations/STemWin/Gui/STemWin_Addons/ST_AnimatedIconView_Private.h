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
File        : ICONVIEW_Private.h
Purpose     : ICONVIEW private header file
--------------------END-OF-HEADER-------------------------------------
*/

#ifndef ST_ANIMATEDICONVIEW_PRIVATE_H
#define ST_ANIMATEDICONVIEW_PRIVATE_H

#include "WM.h"
#include "GUI_ARRAY.h"
#include "ST_AnimatedIconView.h"
#include "ICONVIEW_Private.h"

#if GUI_WINSUPPORT

/*********************************************************************
*
*       Types
*
**********************************************************************
*/

#define ANIMATION_OPEN_MODE     0
#define ANIMATION_CLOSE_MODE    1

typedef struct {
  WIDGET          Widget;
  WM_SCROLL_STATE ScrollStateV;
  WM_SCROLL_STATE ScrollStateH;
  ICONVIEW_PROPS  Props;
  GUI_ARRAY       ItemArray;
  int             xSizeItems;
  int             ySizeItems;
  int             Sel;
  U16             Flags;
  int             Dual;
  int             NumOfPage;
  int             NumOfLine;
  int             CurrentPage;
  int             Next;
  int             Previous;
  int             AnimatedIconIndex;
  int             EnableAnimation; 
  int             AnimationMode; 
  int             AnimationDelay;
  WM_HTIMER       hTimer;
  int             Nbframe;  
  int             FrameIndex;    
  /* Type check in debug version */
  #if GUI_DEBUG_LEVEL >= GUI_DEBUG_LEVEL_CHECK_ALL
    U32 DebugId;
  #endif  
} ST_ANIMATED_ICONVIEW_OBJ;

typedef void tDrawImage    (const void * pData, int xPos, int yPos);
typedef void tDrawText     (ST_ANIMATED_ICONVIEW_OBJ * pObj, GUI_RECT * pRect, const char * s);
typedef void tDrawDualText (ST_ANIMATED_ICONVIEW_OBJ * pObj, GUI_RECT * pRect, const char * s);
typedef void tGetImageSizes(const void * pData, int * xSize, int * ySize);

typedef struct {
  tDrawImage                    * pfDrawImage;
  tDrawText                     * pfDrawText;
  tDrawDualText                 * pfDrawDualText;
  tGetImageSizes                * pfGetImageSizes;
  const GUI_BITMAP              ** pOpenBitmap;
  const GUI_BITMAP              ** pCloseBitmap;
  U32                           UserData;
  char                          acText[1];
} ST_ANIMATED_ICONVIEW_ITEM;

/*********************************************************************
*
*       Function pointer(s)
*
**********************************************************************
*/
extern void (* ICONVIEW__pfDrawStreamedBitmap_ST)(const void * p, int x, int y);

/*********************************************************************
*
*       Macros for internal use
*
**********************************************************************
*/
#if GUI_DEBUG_LEVEL >= GUI_DEBUG_LEVEL_CHECK_ALL
  #define ICONVIEW_INIT_ID(p) (p->DebugId = ICONVIEW_ID)
#else
  #define ICONVIEW_INIT_ID(p)
#endif

#if GUI_DEBUG_LEVEL >= GUI_DEBUG_LEVEL_CHECK_ALL
  ST_ANIMATED_ICONVIEW_OBJ * ST_ANIMATED_ICONVIEW_LockH(ST_ANIMATED_ICONVIEW_Handle h);
  #define ST_ANIMATED_ICONVIEW_LOCK_H(h)   ST_ANIMATED_ICONVIEW_LockH(h)
#else
  #define ST_ANIMATED_ICONVIEW_LOCK_H(h)   (ST_ANIMATED_ICONVIEW_OBJ *)GUI_LOCK_H(h)
#endif

#endif   /* GUI_WINSUPPORT */
#endif   /* ST_ANIMATEDICONVIEW_PRIVATE_H */

/*************************** End of file ****************************/
