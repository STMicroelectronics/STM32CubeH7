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
extern GUI_CONST_STORAGE GUI_FONT   GUI_FontLucidaSans14;
extern GUI_CONST_STORAGE GUI_FONT   GUI_FontLucidaSans16;
extern GUI_CONST_STORAGE GUI_FONT   GUI_FontLucidaSans20;

#endif  /* Avoid multiple inclusion */
/*************************** End of file ****************************/
