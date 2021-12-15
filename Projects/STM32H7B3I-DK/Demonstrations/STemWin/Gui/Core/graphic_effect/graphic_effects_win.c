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

#include "k_modules_res.h"
#include "filtring.h"
/* Genereted bitmaps used for the Graphic effects */
#include "fatfs_storage.h"

#include "WM.h"

/** @addtogroup GRAPHIC_EFFECTS_MODULE
  * @{
  */

/** @defgroup GRAPHIC_EFFECTS
  * @brief graphic effects routines
  * @{
  */
  
/* Private function prototypes -----------------------------------------------*/
static WM_HWIN hWinInfo;
static WM_HWIN hWinGraphicEffects;
static void Startup(WM_HWIN hWin, uint16_t xpos, uint16_t ypos);
static void _cbMediaConnection(WM_MESSAGE * pMsg);

/* External functions --------------------------------------------------------*/
extern GUI_CONST_STORAGE GUI_BITMAP bmApplyFilter;
extern GUI_CONST_STORAGE GUI_BITMAP bmApplyFilterPressed;
extern GUI_CONST_STORAGE GUI_BITMAP bmCpuLoadBackground;
extern GUI_CONST_STORAGE GUI_BITMAP bmEdgeDetection;
extern GUI_CONST_STORAGE GUI_BITMAP bmEdgeDetectionGray;
extern GUI_CONST_STORAGE GUI_BITMAP bmMotionBlur;
extern GUI_CONST_STORAGE GUI_BITMAP bmMotionBlurGray;
extern GUI_CONST_STORAGE GUI_BITMAP bmNextButton;
extern GUI_CONST_STORAGE GUI_BITMAP bmNextGrayButton;
extern GUI_CONST_STORAGE GUI_BITMAP bmNextPressedButton;
extern GUI_CONST_STORAGE GUI_BITMAP bmPreviousButton;
extern GUI_CONST_STORAGE GUI_BITMAP bmPreviousGrayButton;
extern GUI_CONST_STORAGE GUI_BITMAP bmPreviousPressedButton;
extern GUI_CONST_STORAGE GUI_BITMAP bmRaising;
extern GUI_CONST_STORAGE GUI_BITMAP bmRaisingGray;
extern GUI_CONST_STORAGE GUI_BITMAP bmReset;
extern GUI_CONST_STORAGE GUI_BITMAP bmResetGray;
extern GUI_CONST_STORAGE GUI_BITMAP bmResetPressed;
extern GUI_CONST_STORAGE GUI_BITMAP bmSharpening;
extern GUI_CONST_STORAGE GUI_BITMAP bmSharpeningGray;
extern GUI_CONST_STORAGE GUI_BITMAP bmSmoothing;
extern GUI_CONST_STORAGE GUI_BITMAP bmSmoothingGray;
extern GUI_CONST_STORAGE GUI_BITMAP bmRocket1;
extern GUI_CONST_STORAGE GUI_BITMAP bmRocket2;
extern GUI_CONST_STORAGE GUI_BITMAP bmRocket3;
extern GUI_CONST_STORAGE GUI_BITMAP bmge_information;

/* External variables --------------------------------------------------------*/
K_ModuleItem_Typedef  graphic_effects_board =
{
  MODULE_GRAPHIC_EFFECTS,
  "Graphic Effects",
  &hWinGraphicEffects,
  0,
  0,
  Startup,
  NULL,
};

FILELIST_FileTypeDef  BMPList = { 0 };

/* Defines -------------------------------------------------------------------*/
/* Windows */
#define ID_WINDOW_0                     (GUI_ID_USER + 0x00)
/* Images */
#define ID_IMAGE_GEINFO                 (GUI_ID_USER + 0x10)
/* Textes */
#define ID_TEXT_FEATURES_CPU            (GUI_ID_USER + 0x20)
#define ID_TEXT_FILENAME                (GUI_ID_USER + 0x21)
/* Buttons */
#define ID_BUTTON_APPLY_FILTER          (GUI_ID_USER + 0x31)
#define ID_BUTTON_RESET_DST_IMAGE       (GUI_ID_USER + 0x32)
#define ID_BUTTON_EDGE_DETECTION        (GUI_ID_USER + 0x33)
#define ID_BUTTON_MOTION_BLUR           (GUI_ID_USER + 0x34)
#define ID_BUTTON_RAISING               (GUI_ID_USER + 0x35)
#define ID_BUTTON_SHARPENING            (GUI_ID_USER + 0x36)
#define ID_BUTTON_SMOOTHING             (GUI_ID_USER + 0x37)
#define ID_BUTTON_MAIN_MENU             (GUI_ID_USER + 0x38)
#define ID_BUTTON_NEXT                  (GUI_ID_USER + 0x39)
#define ID_BUTTON_PREVIOUS              (GUI_ID_USER + 0x3A)
/* Timers */
#define ID_TIMER_CONNECTION             (GUI_ID_USER + 0x40)
#define ID_TIMER_CPU                    (GUI_ID_USER + 0x41)

#define EDGE_DETECTION_FILTER           0
#define SMOOTHING_FILTER                1
#define SHARPENING_FILTER               2
#define RAISING_FILTER                  3
#define MOTION_BLUR_FILTER              4

#define FRAME_BUFFER_WIDTH              480
#define FRAME_BUFFER_HEIGHT             272
#define ARGB8888_BYTES_PER_PIXEL        (uint32_t) (4U)

#define GFX_VIDEO_BUFFER_SIZE           ((uint32_t)(FRAME_BUFFER_WIDTH * FRAME_BUFFER_HEIGHT * ARGB8888_BYTES_PER_PIXEL)) /* 1.5 Mb */
#define GFX_VIDEO_BUFFER_NBR            8

/* Private typedef -----------------------------------------------------------*/
typedef struct gfx_buffer_unit_s
{
  uint8_t       header[54];
  uint8_t       pixels[GFX_VIDEO_BUFFER_SIZE]; /* 1.5 Mb */
  uint8_t       padding[10];
} gfx_buffer_unit_t;

typedef struct gfx_buffers_s
{
  uint8_t               padding[10];
  gfx_buffer_unit_t     buffers[GFX_VIDEO_BUFFER_NBR];
} gfx_buffers_t;

/* Static variables  ---------------------------------------------------------*/
static const GUI_WIDGET_CREATE_INFO _aDialogInfo[] = {
  { WINDOW_CreateIndirect, "Info Window", ID_WINDOW_0, 0, 0, 480, 272, (WM_CF_SHOW | WM_CF_STAYONTOP), 0x64, 0 },
};

static const GUI_WIDGET_CREATE_INFO _aDialogGraphicEffects[] = {
  { WINDOW_CreateIndirect, "GraphicEffects Window", ID_WINDOW_0, 0, 0, 480, 272, WM_CF_SHOW, 0x64, 0 },
};

#if defined ( __GNUC__ )
__attribute__((section(".gfx_buffers"))) __attribute__ ((aligned (32)))
#elif defined ( __ICCARM__ )
#pragma data_alignment=32
#pragma location="gfx_buffers"
#elif defined( __CC_ARM )
__attribute__((section(".gfx_buffers"), zero_init)) __attribute__ ((aligned (32)))
#endif
static gfx_buffers_t GFX_Buffers;

static unsigned int FilterType  = 0;
static unsigned int SrcImageIdx = 0;
static unsigned int init_module = 0;
static unsigned int init_touch = 0;

static const GUI_BITMAP pBM = {
  480,                                                  /* xSize                        */
  272,                                                  /* ySize                        */
  1920,                                                 /* BytesPerLine                 */
  32,                                                   /* BitsPerPixel                 */
  (unsigned char *)&(GFX_Buffers.buffers[4].pixels),    /* Pointer to picture data      */
  NULL,                                                 /* Pointer to palette           */
  GUI_DRAW_BMPM888
};

static const GUI_BITMAP pBM2 = {
  480,                                                          /* xSize                        */
  272,                                                          /* ySize                        */
  1920,                                                         /* BytesPerLine                 */
  32,                                                           /* BitsPerPixel                 */
  (unsigned char *)&(GFX_Buffers.buffers[3].pixels),            /* Pointer to picture data      */
  NULL,                                                         /* Pointer to palette           */
  GUI_DRAW_BMPM8888I
};

static char dialog_msg[512] = { 0 };

/* Default storage ID is MSD */
static STORAGE_IdTypeDef storage_id = MSD_DISK_UNIT;

/* Functions -----------------------------------------------------------------*/

/**
  * @brief  Paints apply filter button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_ApplyFilterButton(BUTTON_Handle hObj)
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
    GUI_DrawBitmap(&bmEdgeDetection, 0, 20);
  }
  else
  {
    if (FilterType == EDGE_DETECTION_FILTER)
    {
      GUI_DrawBitmap(&bmEdgeDetection, 0, 20);
    }
    else
    {
      GUI_DrawBitmap(&bmEdgeDetectionGray, 0, 20);
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
    GUI_DrawBitmap(&bmMotionBlur, 0, 20);
  }
  else
  {
    if (FilterType == MOTION_BLUR_FILTER)
    {
      GUI_DrawBitmap(&bmMotionBlur, 0, 20);
    }
    else
    {
      GUI_DrawBitmap(&bmMotionBlurGray, 0, 20);
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
    GUI_DrawBitmap(&bmRaising, 0, 20);
  }
  else
  {
    if (FilterType == RAISING_FILTER)
    {
      GUI_DrawBitmap(&bmRaising, 0, 20);
    }
    else
    {
      GUI_DrawBitmap(&bmRaisingGray, 0, 20);
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
    GUI_DrawBitmap(&bmSharpening, 0, 20);
  }
  else
  {
    if (FilterType == SHARPENING_FILTER)
    {
      GUI_DrawBitmap(&bmSharpening, 0, 20);
    }
    else
    {
      GUI_DrawBitmap(&bmSharpeningGray, 0, 20);
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
    GUI_DrawBitmap(&bmSmoothing, 0, 20);
  }
  else
  {
    if (FilterType == SMOOTHING_FILTER)
    {
      GUI_DrawBitmap(&bmSmoothing, 0, 20);
    }
    else
    {
      GUI_DrawBitmap(&bmSmoothingGray, 0, 20);
    }
  }
}

/**
  * @brief  Paints main menu button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_MainMenuButton(BUTTON_Handle hObj)
{
  GUI_DrawBitmap(&bmmenu, 10, 10);
}

/**
  * @brief  Paints next button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_NextButton(BUTTON_Handle hObj)
{
  if (SrcImageIdx == (BMPList.ptr - 1))
  {
      GUI_DrawBitmap(&bmNextGrayButton, 30, 30);
  }
  else
  {
    if ((WIDGET_GetState(hObj) & BUTTON_STATE_PRESSED))
    {
        GUI_DrawBitmap(&bmNextPressedButton, 30, 30);
    }
    else
    {
        GUI_DrawBitmap(&bmNextButton, 30, 30);
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
      GUI_DrawBitmap(&bmPreviousGrayButton, 0, 30);
  }
  else
  {
    if ((WIDGET_GetState(hObj) & BUTTON_STATE_PRESSED))
    {
        GUI_DrawBitmap(&bmPreviousPressedButton, 0, 30);
    }
    else
    {
        GUI_DrawBitmap(&bmPreviousButton, 0, 30);
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
  int id = 0;

  switch (pMsg->MsgId) 
  {
    case WM_PAINT:
      id = WM_GetId(pMsg->hWin);
      switch(id)
      {
        case ID_BUTTON_MAIN_MENU:
          _OnPaint_MainMenuButton(pMsg->hWin);
          break;
        case ID_BUTTON_APPLY_FILTER:
          _OnPaint_ApplyFilterButton(pMsg->hWin);
          break;
        case ID_BUTTON_RESET_DST_IMAGE:
          _OnPaint_ResetDstButton(pMsg->hWin);
          break;
        case ID_BUTTON_EDGE_DETECTION:
          _OnPaint_EdgeDetectionButton(pMsg->hWin);
          break;
        case ID_BUTTON_MOTION_BLUR:
          _OnPaint_MotionBlurButton(pMsg->hWin);
          break;
        case ID_BUTTON_RAISING:
          _OnPaint_RaisingButton(pMsg->hWin);
          break;
        case ID_BUTTON_SHARPENING:
          _OnPaint_SharpeningButton(pMsg->hWin);
          break;
        case ID_BUTTON_SMOOTHING:
          _OnPaint_SmoothingButton(pMsg->hWin);
          break;
        case ID_BUTTON_NEXT:
          _OnPaint_NextButton(pMsg->hWin);
          break;
        case ID_BUTTON_PREVIOUS:
          _OnPaint_PreviousButton(pMsg->hWin);
          break;
        default:
          break;
      }
      break;

    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  Callback function of the media connection status
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbMediaConnection(WM_MESSAGE * pMsg) 
{
  static WM_HTIMER hStatusTimer;  
  static uint8_t   prev_msd_status = 0;
  
  switch (pMsg->MsgId) 
  {
    case WM_CREATE:
      prev_msd_status = Storage_GetStatus(storage_id);
      hStatusTimer = WM_CreateTimer(pMsg->hWin, ID_TIMER_CONNECTION, 100, 0);
      break;
    case WM_TIMER:
      if(prev_msd_status != Storage_GetStatus(storage_id))
      {
        prev_msd_status = Storage_GetStatus(storage_id);
        if(prev_msd_status == 0)
        {
          GUI_EndDialog(hWinGraphicEffects, 0);
        }
      }
      if(hStatusTimer != 0)
      {
        WM_RestartTimer(pMsg->Data.v, 100);
      }
      else
      {
        WM_DeleteTimer(hStatusTimer);
        hStatusTimer = 0;
      }
      break;
    case WM_DELETE:
      if(hStatusTimer != 0)
      {
        WM_DeleteTimer(hStatusTimer);
        hStatusTimer = 0;
      }
      break;
    default:
      WM_DefaultProc(pMsg);
      break;
  }
}

/**
  * @brief  callback for background window
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbWinGraphicEffects(WM_MESSAGE * pMsg)
{
  int id;
  char tmp[64];
  WM_HWIN hItem;

  switch (pMsg->MsgId)
  {
    case WM_INIT_DIALOG:
      IMAGE_CreateEx(0,  0, 480, 272, pMsg->hWin, WM_CF_SHOW, 0, ID_IMAGE_GEINFO);
      
      WM_CreateWindowAsChild(479, 271, 1, 1, pMsg->hWin, WM_CF_SHOW, _cbMediaConnection , 0);
      
      hItem = BUTTON_CreateEx(440, 5, 35, 30, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_MAIN_MENU);
      WM_SetCallback(hItem, _cbButtons);
    
      hItem = BUTTON_CreateEx(4,4, bmReset.XSize, bmReset.YSize, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_RESET_DST_IMAGE);
      WM_SetCallback(hItem, _cbButtons);

      hItem = BUTTON_CreateEx(4,  bmReset.YSize + 15, bmApplyFilter.XSize, bmApplyFilter.YSize, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_APPLY_FILTER);
      WM_SetCallback(hItem, _cbButtons);

      hItem = BUTTON_CreateEx(50, 272 - bmEdgeDetection.YSize - 24, bmEdgeDetection.XSize, bmEdgeDetection.YSize + 20, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_EDGE_DETECTION);
      WM_SetCallback(hItem, _cbButtons);

      hItem = BUTTON_CreateEx(50 + 20 + bmEdgeDetection.XSize, 272 - bmMotionBlur.YSize - 24, bmMotionBlur.XSize, bmMotionBlur.YSize + 20, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_MOTION_BLUR);
      WM_SetCallback(hItem, _cbButtons);

      hItem = BUTTON_CreateEx(50 + 20 + 20 + bmEdgeDetection.XSize * 2, 272 - bmRaising.YSize - 24, bmRaising.XSize, bmRaising.YSize + 20, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_RAISING);
      WM_SetCallback(hItem, _cbButtons);

      hItem = BUTTON_CreateEx(50 + 20 + 20 + 20 + bmEdgeDetection.XSize * 3, 272 - bmSharpening.YSize - 24, bmSharpening.XSize, bmSharpening.YSize + 20, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_SHARPENING);
      WM_SetCallback(hItem, _cbButtons);

      hItem = BUTTON_CreateEx(50 + 20 + 20 + 20 + 20 + bmEdgeDetection.XSize * 4, 272 - bmSmoothing.YSize - 24, bmSmoothing.XSize, bmSmoothing.YSize + 20, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_SMOOTHING);
      WM_SetCallback(hItem, _cbButtons);

      hItem = BUTTON_CreateEx(2, 272 - bmPreviousButton.YSize - 4 - 30, bmPreviousButton.XSize + 30, bmPreviousButton.YSize + 30, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_PREVIOUS);
      WM_SetCallback(hItem, _cbButtons);

      hItem = BUTTON_CreateEx(480 - 2 - bmNextButton.XSize - 30, 272 - bmNextButton.YSize - 4 - 30, bmNextButton.XSize + 30, bmNextButton.YSize + 30, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_NEXT);
      WM_SetCallback(hItem, _cbButtons);

      hItem = TEXT_CreateEx(180, 10, 120, 20, pMsg->hWin, WM_CF_SHOW, (TEXT_CF_HCENTER | TEXT_CF_VCENTER), ID_TEXT_FEATURES_CPU, "CPU load: 0%");
      TEXT_SetFont(hItem, GUI_FONT_16_ASCII);
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
      GUI_DrawBitmap(&pBM, 0, 0);
      GUI_DrawBitmap(&bmCpuLoadBackground, 180, 4);
      break;

    case WM_TIMER:
      id = WM_GetTimerId(pMsg->Data.v);
      if (id == ID_TIMER_CPU)
      {
        WM_RestartTimer(pMsg->Data.v, 1000);

        /* Get CPU usage */
        sprintf((char *)tmp , "CPU load : %d%%", osGetCPUUsage());

        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_FEATURES_CPU);
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
        if ((pState.x - 20) < 0)
        {
          pState.x = 20;
        }
        /* Check top border */
        if ((pState.y - 20) < 0)
        {
          pState.y = 20;
        }
        /* Check right border */
        if ((pState.x + 20) > 480)
        {
          pState.x = 480 - 20;
        }
        /* Check bottom border */
        if ((pState.y + 20) > 272)
        {
          pState.y = 272 - 20;
        }
        
        GUI_RECT filterRect = { pState.x - 20, pState.y - 20, pState.x + 20, pState.y + 20 };
        FilterBitmap(&pBM2, (unsigned long)&(GFX_Buffers.buffers[4].pixels), filterRect, FilterType);
        
        /* Update the window rectangle to display the filtred content */
        WM_InvalidateRect(pMsg->hWin, &filterRect);
      }
      else
      {
        init_touch = 1;
      }   
      
    }
    break;
    
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
        else
        {
          id = WM_GetId(pMsg->hWinSrc);
          switch(id)
          {
            case ID_BUTTON_EDGE_DETECTION:
            {
              FilterType = EDGE_DETECTION_FILTER;

              GUI_RECT filterRect = { 0, 235, 480, 272 };
              WM_InvalidateRect(pMsg->hWin, &filterRect);
            }
            break;
            case ID_BUTTON_MOTION_BLUR:
            {
              FilterType = MOTION_BLUR_FILTER;

              GUI_RECT filterRect = { 0, 235, 480, 272 };
              WM_InvalidateRect(pMsg->hWin, &filterRect);
            }
            break;
            case ID_BUTTON_RAISING:
            {
              FilterType = RAISING_FILTER;

              GUI_RECT filterRect = { 0, 235, 480, 272 };
              WM_InvalidateRect(pMsg->hWin, &filterRect);
            }
            break;
            case ID_BUTTON_SHARPENING:
            {
              FilterType = SHARPENING_FILTER;

              GUI_RECT filterRect = { 0, 235, 480, 272 };
              WM_InvalidateRect(pMsg->hWin, &filterRect);
            }
            break;
            case ID_BUTTON_SMOOTHING:
            {
              FilterType = SMOOTHING_FILTER;
              
              GUI_RECT filterRect = { 0, 235, 480, 272 };
              WM_InvalidateRect(pMsg->hWin, &filterRect);
            }
            break;
            case ID_BUTTON_NEXT:
            {
              if (SrcImageIdx < (BMPList.ptr - 1))
              {
                SrcImageIdx++;

                LCD_DrawBitmap_Effect(storage_id, 0, 480, 272, (uint8_t *)&(GFX_Buffers.buffers[SrcImageIdx + 5].pixels), (uint32_t *)&(GFX_Buffers.buffers[3].pixels));
                LCD_DrawBitmap_Effect(storage_id, 0, 480, 272, (uint8_t *)&(GFX_Buffers.buffers[3].pixels), (uint32_t *)&(GFX_Buffers.buffers[4].pixels));

                GUI_RECT filterRect = { 0, 0, 480, 272 };
                WM_InvalidateRect(pMsg->hWin, &filterRect);

                if(BMPList.ptr > 3)
                {
                  if ((SrcImageIdx + 1) < BMPList.ptr)
                  {
                    Storage_OpenReadFile(storage_id, (uint8_t *)&(GFX_Buffers.buffers[3].header), (char const *)BMPList.file[SrcImageIdx + 1].name);
                  }
                  if ((SrcImageIdx) >= 1)
                  {
                    Storage_OpenReadFile(storage_id, (uint8_t *)&(GFX_Buffers.buffers[4].header), (char const *)BMPList.file[SrcImageIdx - 1].name);
                  }
                }
              }
            }
            break;
            case ID_BUTTON_PREVIOUS:
            {
              if (SrcImageIdx > 0)
              {
                SrcImageIdx--;

                LCD_DrawBitmap_Effect(storage_id, 0, 480, 272, (uint8_t *)&(GFX_Buffers.buffers[SrcImageIdx + 5].pixels), (uint32_t *)&(GFX_Buffers.buffers[3].pixels));
                LCD_DrawBitmap_Effect(storage_id, 0, 480, 272, (uint8_t *)&(GFX_Buffers.buffers[3].pixels), (uint32_t *)&(GFX_Buffers.buffers[4].pixels));

                GUI_RECT filterRect = { 0, 0, 480, 272 };
                WM_InvalidateRect(pMsg->hWin, &filterRect);

                if(BMPList.ptr > 3)
                {
                  if ((SrcImageIdx + 1) < BMPList.ptr)
                  {
                    Storage_OpenReadFile(storage_id, (uint8_t *)&(GFX_Buffers.buffers[3].header), (char const *)BMPList.file[SrcImageIdx + 1].name);
                  }
                  if ((SrcImageIdx) > 0)
                  {
                    Storage_OpenReadFile(storage_id, (uint8_t *)&(GFX_Buffers.buffers[4].header), (char const *)BMPList.file[SrcImageIdx - 1].name);
                  }
                }
              }
            }
            break;
            case ID_BUTTON_MAIN_MENU:
            {
              GUI_EndDialog(pMsg->hWin,0 );
            }
            break;
            default:
            break;
          }
        }
        break;
        case WM_NOTIFICATION_RELEASED:

          id = WM_GetId(pMsg->hWinSrc);
          switch(id)
          {
            case ID_BUTTON_APPLY_FILTER:
            {
              GUI_RECT filterRect = { 0, 0, 480, 272 };
              FilterBitmap(&pBM2, (unsigned long)&(GFX_Buffers.buffers[4].pixels), filterRect, FilterType);
              WM_InvalidateRect(pMsg->hWin, &filterRect);
            }
            break;
            case ID_BUTTON_RESET_DST_IMAGE:
            {
              LCD_DrawBitmap_Effect(storage_id, 0, 480, 272, (uint8_t *)&(GFX_Buffers.buffers[3].pixels), (uint8_t *)&(GFX_Buffers.buffers[4].pixels));
              GUI_RECT filterRect = { 0, 0, 480, 272 };
              WM_InvalidateRect(pMsg->hWin, &filterRect);
            }
            break;
          }
            default:
            break;
      }
      break;

    case WM_DELETE:
      hWinGraphicEffects = 0;
      break;

    default:
      WM_DefaultProc(pMsg);
      break;
  }
}

/**
  * @brief  Add entire folder to play list.
  * @param  Foldername: pointer to folder name.
  * @retval None
  */
static void _AddEntireFolder(char *Foldername, int update)
{
  FRESULT res;
  FILINFO fno;
  DIR dir;
  char *fn;
  static char tmp[FILEMGR_FILE_NAME_SIZE];
  char folder[256] = { 0 };

  if(Storage_GetStatus(storage_id) == 0)
  {
    return;
  }
  snprintf(folder, sizeof(folder), "%s%s", Storage_GetDrive(storage_id), Foldername);

  res = f_opendir(&dir, folder);
  
  if (res == FR_OK)
  {
    
    while (1)
    {
      res = f_readdir(&dir, &fno);
      
      if (res != FR_OK || fno.fname[0] == 0)
      {
        break;
      }
      if (fno.fname[0] == '.')
      {
        continue;
      }

      fn = fno.fname;
      
      if (BMPList.ptr < FILEMGR_LIST_DEPDTH)
      {
        if ((fno.fattrib & AM_DIR) == 0)
        {
          if(((strstr(fn, ".bmp")) || (strstr(fn, ".BMP"))))
          {
            
            strcpy(tmp, folder);
            strcat(tmp, "/");
            strcat(tmp, fn);
            strncpy((char *)BMPList.file[BMPList.ptr].name, (char *)tmp, FILEMGR_FILE_NAME_SIZE);
            BMPList.ptr++;
          }
        }
      }   
    }
  }
  f_closedir(&dir);
}

/**
  * @brief  Callback routine of the video main dialog
  * @param  pMsg: pointer to a data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbDialog_info(WM_MESSAGE * pMsg) {
  WM_HWIN  hItem;
  int Id, NCode;
  
  switch (pMsg->MsgId) 
  {
    case WM_INIT_DIALOG:
      hItem = BUTTON_CreateEx(440, 5, 35, 30, pMsg->hWin, WM_CF_HIDE, 0, ID_BUTTON_MAIN_MENU);
      WM_SetCallback(hItem, _cbButtons);
      
      hItem = TEXT_CreateEx(110, 124, 360, 20, pMsg->hWin, WM_CF_SHOW, 0, ID_TEXT_FILENAME, dialog_msg);
      TEXT_SetTextAlign(hItem, (TEXT_CF_LEFT | TEXT_CF_VCENTER));
      TEXT_SetFont(hItem, GUI_FONT_16_ASCII);
      TEXT_SetTextColor(hItem, GUI_WHITE);
      break;
    case WM_PAINT:
      GUI_DrawBitmap(&bmbkc_16bit, 0, 0);
      GUI_DrawBitmap(&bminfo, 60, 121);
      break;

    case WM_NOTIFY_PARENT:
      Id    = WM_GetId(pMsg->hWinSrc);    /* Id of widget */
      NCode = pMsg->Data.v;               /* Notification code */
      switch(Id) 
      {
        case ID_BUTTON_MAIN_MENU:
          switch(NCode) 
          {
            case WM_NOTIFICATION_RELEASED:
              GUI_EndDialog(pMsg->hWin, 0);
              if(hWinGraphicEffects)
              {
                GUI_EndDialog(hWinGraphicEffects, 0);
              }
              break;
          }
          break;
      }
      break;

    case WM_DELETE:
      hWinInfo = 0;
      break;

    default:
      WM_DefaultProc(pMsg);
      break;
  }    
}

static void Startup(WM_HWIN hWin, uint16_t xpos, uint16_t ypos)
{
  WM_HWIN  hItem;
  __IO uint8_t bmp_count = 3;

  GUI_SetFont(&GUI_Font24B_1);
  GUI_SetTextMode(GUI_TM_TRANS);

  init_module = 0;
  init_touch = 0;
  SrcImageIdx = 1;

  hWinGraphicEffects = GUI_CreateDialogBox(_aDialogGraphicEffects, GUI_COUNTOF(_aDialogGraphicEffects), _cbWinGraphicEffects, hWin, xpos, ypos);

  if(BMPList.ptr == 0)
  {
    /* Reset bitmap counter */
    bmp_count = 0;

    /* Add bmp files to the list */
    _AddEntireFolder("BMP", 0);

    if(BMPList.ptr >= 3)
    {
      sprintf(dialog_msg, "Loading BMP files from media storage ...");
      hWinInfo = GUI_CreateDialogBox(_aDialogInfo, GUI_COUNTOF(_aDialogInfo), _cbDialog_info, hWinGraphicEffects, 0, 0);
      GUI_Exec1();
      for (bmp_count = 0; bmp_count < 3; bmp_count++)
      {
        if(Storage_OpenReadFile(storage_id, (uint8_t *)&(GFX_Buffers.buffers[bmp_count].header), (char const *)BMPList.file[bmp_count].name))
        {
          sprintf(dialog_msg, "Failed to read file '%s'", BMPList.file[bmp_count].name);
          hItem = WM_GetDialogItem(hWinInfo, ID_TEXT_FILENAME);
          TEXT_SetText(hItem, dialog_msg);
          WM_InvalidateWindow(hItem);
          GUI_Exec1();
          hItem = WM_GetDialogItem(hWinInfo, ID_BUTTON_MAIN_MENU);
          WM_ShowWindow(hItem);
          WM_InvalidateWindow(hItem);
          GUI_Exec1();
          memset(&BMPList, 0, sizeof(BMPList));
          break;
        }
      }
    }
    else
    {
      if(BMPList.ptr == 0)
      {
        sprintf(dialog_msg, "Please add bitmap files in the selected media storage");
      }
      else
      {
        sprintf(dialog_msg, "Minimum of 3 bitmaps are required inside BPM folder");
      }
      hWinInfo = GUI_CreateDialogBox(_aDialogInfo, GUI_COUNTOF(_aDialogInfo), _cbDialog_info, hWinGraphicEffects, 0, 0);
      hItem = WM_GetDialogItem(hWinInfo, ID_BUTTON_MAIN_MENU);
      WM_ShowWindow(hItem);
      WM_InvalidateWindow(hItem);
      GUI_Exec1();
    }

    if(bmp_count == 3)
    {
      if(hWinInfo)
      {
        GUI_EndDialog(hWinInfo, 0);
      }

      /* load inverted bitmaps to new locations */
      LCD_DrawBitmap_Effect(storage_id, 1, 480, 272, (uint8_t *)&(GFX_Buffers.buffers[0].header), (uint32_t *)&(GFX_Buffers.buffers[5].pixels));
      LCD_DrawBitmap_Effect(storage_id, 1, 480, 272, (uint8_t *)&(GFX_Buffers.buffers[1].header), (uint32_t *)&(GFX_Buffers.buffers[6].pixels));
      LCD_DrawBitmap_Effect(storage_id, 1, 480, 272, (uint8_t *)&(GFX_Buffers.buffers[2].header), (uint32_t *)&(GFX_Buffers.buffers[7].pixels));
    }
  }

  if(bmp_count == 3)
  {
    /* Load Second bitmap to Reset bitmap */
    LCD_DrawBitmap_Effect(storage_id, 0, 480, 272, (uint8_t *)&(GFX_Buffers.buffers[6].pixels), (uint32_t *)&(GFX_Buffers.buffers[3].pixels));
    /* Load Second bitmap frame to Active bitmap */
    LCD_DrawBitmap_Effect(storage_id, 0, 480, 272, (uint8_t *)&(GFX_Buffers.buffers[3].pixels), (uint32_t *)&(GFX_Buffers.buffers[4].pixels));
  }
}

/**
  * @}
  */

/**
  * @}
  */
  
