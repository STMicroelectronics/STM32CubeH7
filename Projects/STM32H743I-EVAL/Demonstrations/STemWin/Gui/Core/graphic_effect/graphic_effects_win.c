/**
  ******************************************************************************
  * @file    graphic_effects_win.c
  * @author  MCD Application Team
  * @brief   graphic effects functions
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stddef.h>
#include <string.h>

#include "filtring.h"
/* Genereted bitmaps used for the Graphic effects */
#include "graphic_effects_res.c"

#include "WM.h"

/** @addtogroup GRAPHIC_EFFECTS_MODULE
  * @{
  */

/** @defgroup GRAPHIC_EFFECTS
  * @brief graphic effects routines
  * @{
  */
  
/* Private function prototypes -----------------------------------------------*/
static void Startup(WM_HWIN hWin, uint16_t xpos, uint16_t ypos);

/* External functions --------------------------------------------------------*/

/* External variables --------------------------------------------------------*/
K_ModuleItem_Typedef  graphic_effects_board =
{
  7,
  "Graphic Effects",
  0,
  0,
  Startup,
  NULL,
};

/* Private typedef -----------------------------------------------------------*/
/* Static variables  ---------------------------------------------------------*/
static WM_HWIN HwinBkc;

static WM_HWIN HwinGoMainMenuButton;
static WM_HWIN HwinApplyFilterButton;
static WM_HWIN HwinResetDstButton;

static WM_HWIN HwinEdgeDetectionButton;
static WM_HWIN HwinMotionBlurButton;
static WM_HWIN HwinRaisingButton;
static WM_HWIN HwinSharpeningButton;
static WM_HWIN HwinSmoothingButton;
static WM_HWIN HwinNextButton;
static WM_HWIN HwinPreviousButton;

static unsigned int FilterType  = 0;
static unsigned int SrcImageIdx = 0;
static unsigned int init_module = 0;
static unsigned int init_touch = 0;

/* Background bitmaps array */
static const GUI_BITMAP *aBackgoundBitmaps[] = 
{
  &bmImage1_640x480,
  &bmImage2_640x480,
  &bmImage3_640x480,
  &bmImage4_640x480,
  &bmImage5_640x480,
  &bmImage6_640x480,
  &bmImage7_640x480,
};

static const GUI_BITMAP pBM = {
  640,                          /* xSize*/
  480,                          /* ySize */
  2560,                         /* BytesPerLine */
  32,                           /* BitsPerPixel */
  (unsigned char *)0xD1000000,  /* Pointer to picture data */
  NULL,                         /* Pointer to palette */
  GUI_DRAW_BMPM8888I
};

/* Defines -------------------------------------------------------------------*/
#define ID_TIMER_CPU                    2

#define ID_FEATURES_CPU                 1

#define ID_BUTTON_APPLY_FILTER          (GUI_ID_USER + 0x1)
#define ID_BUTTON_RESET_DST_IMAGE       (GUI_ID_USER + 0x2)
#define ID_BUTTON_EDGE_DETECTION        (GUI_ID_USER + 0x3)
#define ID_BUTTON_MOTION_BLUR           (GUI_ID_USER + 0x4)
#define ID_BUTTON_RAISING               (GUI_ID_USER + 0x5)
#define ID_BUTTON_SHARPENING            (GUI_ID_USER + 0x6)
#define ID_BUTTON_SMOOTHING             (GUI_ID_USER + 0x7)
#define ID_BUTTON_MAIN_MENU             (GUI_ID_USER + 0x8)
#define ID_BUTTON_NEXT                  (GUI_ID_USER + 0x9)
#define ID_BUTTON_PREVIOUS              (GUI_ID_USER + 0xA)
#define ID_IMAGE_GEINFO                 (GUI_ID_USER + 0xB)

#define EDGE_DETECTION_FILTER           0
#define SMOOTHING_FILTER                1
#define SHARPENING_FILTER               2
#define RAISING_FILTER                  3
#define MOTION_BLUR_FILTER              4

/* Functions -----------------------------------------------------------------*/

/**
  * @brief  Paints apply filter button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_ApplayFilterButton(BUTTON_Handle hObj)
{
  if ((WIDGET_GetState(hObj) & BUTTON_STATE_PRESSED))
  {
    GUI_DrawBitmap(&bmApplyFilterPressed, 0, 0);
  }
  else
  {
    GUI_DrawBitmap(&bmApplyFilter, 0, 0);
  }
}

/**
  * @brief  Paints reset destination image button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_ResetDstButton(BUTTON_Handle hObj)
{
  if ((WIDGET_GetState(hObj) & BUTTON_STATE_PRESSED))
  {
    GUI_DrawBitmap(&bmResetPressed, 0, 0);
  }
  else
  {
    GUI_DrawBitmap(&bmReset, 0, 0);
  }
}

/**
  * @brief  Paints edge detection button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_EdgeDetectionButton(BUTTON_Handle hObj)
{
  if ((WIDGET_GetState(hObj) & BUTTON_STATE_PRESSED))
  {
    GUI_DrawBitmap(&bmEdgeDetection, 0, 0);
  }
  else
  {
    if (FilterType == EDGE_DETECTION_FILTER)
    {
      GUI_DrawBitmap(&bmEdgeDetection, 0, 0);
    }
    else
    {
      GUI_DrawBitmap(&bmEdgeDetectionGray, 0, 0);
    }
  }
}

/**
  * @brief  Paints motion blur button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_MotionBlurButton(BUTTON_Handle hObj)
{
  if ((WIDGET_GetState(hObj) & BUTTON_STATE_PRESSED))
  {
    GUI_DrawBitmap(&bmMotionBlur, 0, 0);
  }
  else
  {
    if (FilterType == MOTION_BLUR_FILTER)
    {
      GUI_DrawBitmap(&bmMotionBlur, 0, 0);
    }
    else
    {
      GUI_DrawBitmap(&bmMotionBlurGray, 0, 0);
    }
  }
}

/**
  * @brief  Paints raising button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_RaisingButton(BUTTON_Handle hObj)
{
  if ((WIDGET_GetState(hObj) & BUTTON_STATE_PRESSED))
  {
    GUI_DrawBitmap(&bmRaising, 0, 0);
  }
  else
  {
    if (FilterType == RAISING_FILTER)
    {
      GUI_DrawBitmap(&bmRaising, 0, 0);
    }
    else
    {
      GUI_DrawBitmap(&bmRaisingGray, 0, 0);
    }
  }
}

/**
  * @brief  Paints sharpening button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_SharpeningButton(BUTTON_Handle hObj)
{
  if ((WIDGET_GetState(hObj) & BUTTON_STATE_PRESSED))
  {
    GUI_DrawBitmap(&bmSharpening, 0, 0);
  }
  else
  {
    if (FilterType == SHARPENING_FILTER)
    {
      GUI_DrawBitmap(&bmSharpening, 0, 0);
    }
    else
    {
      GUI_DrawBitmap(&bmSharpeningGray, 0, 0);
    }
  }
}

/**
  * @brief  Paints smoothing button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_SmoothingButton(BUTTON_Handle hObj)
{
  if ((WIDGET_GetState(hObj) & BUTTON_STATE_PRESSED))
  {
    GUI_DrawBitmap(&bmSmoothing, 0, 0);
  }
  else
  {
    if (FilterType == SMOOTHING_FILTER)
    {
      GUI_DrawBitmap(&bmSmoothing, 0, 0);
    }
    else
    {
      GUI_DrawBitmap(&bmSmoothingGray, 0, 0);
    }
  }
}

/**
  * @brief  Paints main menu button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_MainMainMenuButton(BUTTON_Handle hObj)
{
  if ((WIDGET_GetState(hObj) & BUTTON_STATE_PRESSED))
  {
    GUI_DrawBitmap(&bmMainMenuButton, 0, 0);
  }
  else
  {
    GUI_DrawBitmap(&bmMainMenuButton, 0, 0);
  }
}

/**
  * @brief  Paints next button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_NextButton(BUTTON_Handle hObj)
{
  if (SrcImageIdx >= 6)
  {
      GUI_DrawBitmap(&bmNextGrayButton, 0, 0);
  }
  else
  {
    if ((WIDGET_GetState(hObj) & BUTTON_STATE_PRESSED))
    {
        GUI_DrawBitmap(&bmNextPressedButton, 0, 0);
    }
    else
    {
        GUI_DrawBitmap(&bmNextButton, 0, 0);
    }
  }
}

/**
  * @brief  Paints previous button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_PreviousButton(BUTTON_Handle hObj)
{
  if (SrcImageIdx == 0)
  {
      GUI_DrawBitmap(&bmPreviousGrayButton, 0, 0);
  }
  else
  {
    if ((WIDGET_GetState(hObj) & BUTTON_STATE_PRESSED))
    {
        GUI_DrawBitmap(&bmPreviousPressedButton, 0, 0);
    }
    else
    {
        GUI_DrawBitmap(&bmPreviousButton, 0, 0);
    }
  }
}

/**
  * @brief  callback for buttons
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButtons(WM_MESSAGE * pMsg) 
{
  switch (pMsg->MsgId) 
  {
    case WM_PAINT:
      if (pMsg->hWin == HwinGoMainMenuButton)
      {
        _OnPaint_MainMainMenuButton(pMsg->hWin);
      }

      if (pMsg->hWin == HwinApplyFilterButton)
      {
        _OnPaint_ApplayFilterButton(pMsg->hWin);
      }

      if (pMsg->hWin == HwinResetDstButton)
      {
        _OnPaint_ResetDstButton(pMsg->hWin);
      }

      if (pMsg->hWin == HwinEdgeDetectionButton)
      {
        _OnPaint_EdgeDetectionButton(pMsg->hWin);
      }

      if (pMsg->hWin == HwinMotionBlurButton)
      {
        _OnPaint_MotionBlurButton(pMsg->hWin);
      }

      if (pMsg->hWin == HwinRaisingButton)
      {
        _OnPaint_RaisingButton(pMsg->hWin);
      }

      if (pMsg->hWin == HwinSharpeningButton)
      {
        _OnPaint_SharpeningButton(pMsg->hWin);
      }

      if (pMsg->hWin == HwinSmoothingButton)
      {
        _OnPaint_SmoothingButton(pMsg->hWin);
      }

      if (pMsg->hWin == HwinNextButton)
      {
        _OnPaint_NextButton(pMsg->hWin);
      }

      if (pMsg->hWin == HwinPreviousButton)
      {
        _OnPaint_PreviousButton(pMsg->hWin);
      }

      break;

    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  callback for background window
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbWinBkc(WM_MESSAGE * pMsg)
{
  switch (pMsg->MsgId)
  {
    case WM_CREATE:
      CopyBitmap(&bmImage1_640x480, (unsigned long)0xD1000000);

      break;

    case WM_PAINT:
       GUI_DrawBitmap(&pBM, 0, 0);

      break;

    case WM_TIMER:
      break;

    case WM_DELETE:
      break;

    default:
      WM_DefaultProc(pMsg);
      break;
  }
}

/**
  * @brief  callback for buttons window
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbWinButtons(WM_MESSAGE * pMsg)
{
  int id;
  char tmp[64];
  WM_HWIN hItem;

  switch (pMsg->MsgId)
  {
    case WM_CREATE:
      IMAGE_CreateEx(0,  0, 640, 480, pMsg->hWin, WM_CF_SHOW, 0, ID_IMAGE_GEINFO);
      
      HwinGoMainMenuButton = BUTTON_CreateEx((640 - bmMainMenuButton.XSize - 2), 2, bmMainMenuButton.XSize, bmMainMenuButton.YSize, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_MAIN_MENU);
      WM_SetCallback(HwinGoMainMenuButton, _cbButtons); 
    
      HwinResetDstButton = BUTTON_CreateEx(4,4, bmReset.XSize, bmReset.YSize, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_RESET_DST_IMAGE);
      WM_SetCallback(HwinResetDstButton, _cbButtons); 

      HwinApplyFilterButton = BUTTON_CreateEx(4,  bmReset.YSize + 8, bmApplyFilter.XSize, bmApplyFilter.YSize, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_APPLY_FILTER);
      WM_SetCallback(HwinApplyFilterButton, _cbButtons); 

      HwinEdgeDetectionButton = BUTTON_CreateEx(50, 480 - bmEdgeDetection.YSize - 4, bmEdgeDetection.XSize, bmEdgeDetection.YSize, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_EDGE_DETECTION);
      WM_SetCallback(HwinEdgeDetectionButton, _cbButtons);

      HwinMotionBlurButton = BUTTON_CreateEx(50 + 10 + bmEdgeDetection.XSize, 480 - bmMotionBlur.YSize - 4, bmMotionBlur.XSize, bmMotionBlur.YSize, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_MOTION_BLUR);
      WM_SetCallback(HwinMotionBlurButton, _cbButtons);

      HwinRaisingButton = BUTTON_CreateEx(50 + 10 + 10 + bmEdgeDetection.XSize * 2, 480 - bmRaising.YSize - 4, bmRaising.XSize, bmRaising.YSize, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_RAISING);
      WM_SetCallback(HwinRaisingButton, _cbButtons);

      HwinSharpeningButton = BUTTON_CreateEx(50 + 10 + 10 + 10 + bmEdgeDetection.XSize * 3, 480 - bmSharpening.YSize - 4, bmSharpening.XSize, bmSharpening.YSize, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_SHARPENING);
      WM_SetCallback(HwinSharpeningButton, _cbButtons);

      HwinSmoothingButton = BUTTON_CreateEx(50 + 10 + 10 + 10 + 10 + bmEdgeDetection.XSize * 4, 480 - bmSmoothing.YSize - 4, bmSmoothing.XSize, bmSmoothing.YSize, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_SMOOTHING);
      WM_SetCallback(HwinSmoothingButton, _cbButtons);

      HwinPreviousButton = BUTTON_CreateEx(2, 480 - bmPreviousButton.YSize - 4, bmPreviousButton.XSize, bmPreviousButton.YSize, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_PREVIOUS);
      WM_SetCallback(HwinPreviousButton, _cbButtons);

      HwinNextButton = BUTTON_CreateEx(640 - 2 - bmPreviousButton.XSize, 480 - bmNextButton.YSize - 4, bmNextButton.XSize, bmNextButton.YSize, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_NEXT);
      WM_SetCallback(HwinNextButton, _cbButtons);

      hItem = TEXT_CreateEx(288, 12, 150, 20, pMsg->hWin, WM_CF_SHOW, TEXT_CF_LEFT, ID_FEATURES_CPU, "CPU load: 0%");
      TEXT_SetFont(hItem, GUI_FONT_16B_1);
      TEXT_SetTextColor(hItem, GUI_WHITE);
 
      WM_CreateTimer(pMsg->hWin, ID_TIMER_CPU, 2000, 0);

      break;

    case WM_PAINT:
      if(init_module == 0)
      {
        hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_GEINFO);
        IMAGE_SetBitmap(hItem, &bmge_information);  
        init_module = 1;
      }    

      GUI_DrawBitmap(&bmCpuLoadBackground, 274, 4);
      break;

    case WM_TIMER:
      id = WM_GetTimerId(pMsg->Data.v);

      if (id == ID_TIMER_CPU)
      {
        WM_RestartTimer(pMsg->Data.v, 1000);

        /* Get CPU usage */
        sprintf((char *)tmp , "CPU load : %d%%", osGetCPUUsage());

        hItem = WM_GetDialogItem(pMsg->hWin, ID_FEATURES_CPU);
        TEXT_SetText(hItem, tmp);
      }

      break;

      case WM_TOUCH:
      {
        GUI_PID_STATE pState;
        GUI_PID_GetState(&pState);

        if(init_touch == 1)
        {
        /* Check left border */
        if ((pState.x - 35) < 0)
        {
          pState.x = 35;
        }
        /* Check top border */
        if ((pState.y - 35) < 0)
        {
          pState.y = 35;
        }
        /* Check right border */
        if ((pState.x + 35) > 640)
        {
          pState.x = 640 - 35;
        }
        /* Check bottom border */
        if ((pState.y + 35) > 480)
        {
          pState.y = 480 - 35;
        }

        GUI_RECT filterRect = { pState.x - 35, pState.y - 35, pState.x + 35, pState.y + 35 };
        FilterBitmap(aBackgoundBitmaps[SrcImageIdx], (unsigned long)0xD1000000, filterRect, FilterType);

        /* Update the window rectangle to display the filtred content */
        WM_InvalidateRect(HwinBkc, &filterRect);

        /* Do the background work ... Update windows etc.) */
        GUI_Exec1();
        }
        else
        {
          init_touch = 1;
        }
          
        break;
      }

    case WM_NOTIFY_PARENT:
      switch (pMsg->Data.v)
      {
        case WM_NOTIFICATION_CLICKED:
        if(init_module == 1)
        {
          hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_GEINFO);
          WM_DeleteWindow(hItem); 
          init_module = 2;
        }    
          break;
          
        case WM_NOTIFICATION_RELEASED:
          
        
          if (pMsg->hWinSrc == HwinGoMainMenuButton)
          {
            WM_DeleteWindow(HwinBkc);
          }

          if (pMsg->hWinSrc == HwinEdgeDetectionButton)
          {
            FilterType = EDGE_DETECTION_FILTER;

            GUI_RECT filterRect = { 0, 430, 640, 480 };
            WM_InvalidateRect(HwinBkc, &filterRect);
            GUI_Exec1();
          }
          else if (pMsg->hWinSrc == HwinMotionBlurButton)
          {
            FilterType = MOTION_BLUR_FILTER;

            GUI_RECT filterRect = { 0, 430, 640, 480 };
            WM_InvalidateRect(HwinBkc, &filterRect);
            GUI_Exec1();
          }
          else if (pMsg->hWinSrc == HwinRaisingButton)
          {
            FilterType = RAISING_FILTER;

            GUI_RECT filterRect = { 0, 430, 640, 480 };
            WM_InvalidateRect(HwinBkc, &filterRect);
            GUI_Exec1();
          }
          else if (pMsg->hWinSrc == HwinSharpeningButton)
          {
            FilterType = SHARPENING_FILTER;

            GUI_RECT filterRect = { 0, 430, 640, 480 };
            WM_InvalidateRect(HwinBkc, &filterRect);
            GUI_Exec1();
          }
          else if (pMsg->hWinSrc == HwinSmoothingButton)
          {
            FilterType = SMOOTHING_FILTER;
            
            GUI_RECT filterRect = { 0, 430, 640, 480 };
            WM_InvalidateRect(HwinBkc, &filterRect);
            GUI_Exec1();
          }

          if (pMsg->hWinSrc == HwinNextButton)
          {
            if (SrcImageIdx < GUI_COUNTOF(aBackgoundBitmaps) - 1)
            {
              SrcImageIdx++;
              CopyBitmap(aBackgoundBitmaps[SrcImageIdx], (unsigned long)0xD1000000);

              GUI_RECT filterRect = { 0, 0, 640, 480 };
              WM_InvalidateRect(HwinBkc, &filterRect);
              GUI_Exec1();
            }
          }

          if (pMsg->hWinSrc == HwinPreviousButton)
          {
            if (SrcImageIdx >= 1)
            {
              SrcImageIdx--;
              CopyBitmap(aBackgoundBitmaps[SrcImageIdx], (unsigned long)0xD1000000);

              GUI_RECT filterRect = { 0, 0, 640, 480 };
              WM_InvalidateRect(HwinBkc, &filterRect);
              GUI_Exec1();
            }
          }

          if (pMsg->hWinSrc == HwinApplyFilterButton)
          {
            GUI_RECT filterRect = { 0, 0, 640, 480 };
            FilterBitmap(aBackgoundBitmaps[SrcImageIdx], (unsigned long)0xD1000000, filterRect, FilterType);

            WM_InvalidateRect(HwinBkc, &filterRect);
            GUI_Exec1();
          }

          if (pMsg->hWinSrc == HwinResetDstButton)
          {
            CopyBitmap(aBackgoundBitmaps[SrcImageIdx], (unsigned long)0xD1000000);

            GUI_RECT filterRect = { 0, 0, 640, 480 };
            WM_InvalidateRect(HwinBkc, &filterRect);
            GUI_Exec1();
          }

          break;
      }

    default:
      WM_DefaultProc(pMsg);
      break;
  }
}

static void Startup(WM_HWIN hWin, uint16_t xpos, uint16_t ypos)
{
  GUI_SetFont(&GUI_FontComic24B_ASCII);
  GUI_SetTextMode(GUI_TM_TRANS);

  init_module = 0;
  SrcImageIdx = 0;
  init_touch = 0;

  /* Create the background window */
  HwinBkc = WM_CreateWindowAsChild(0, 0, 640, 480, hWin, WM_CF_HASTRANS | WM_CF_SHOW, _cbWinBkc, 0);

  /* Create the buttons window */
  WM_CreateWindowAsChild(0, 0, 640, 480, HwinBkc, WM_CF_HASTRANS | WM_CF_SHOW, _cbWinButtons, 0);
}

/**
  * @}
  */

/**
  * @}
  */
  
