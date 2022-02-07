/**
  ******************************************************************************
  * @file    MenuLauncher.c
  * @author  MCD Application Team
  * @brief   This file provides the kernel bsp functions
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

#include "DIALOG.h"

#if defined ( WIN32 )
#include "main.h"
#else
#include <stddef.h>
#include "bsp.h"
#include "splash.h"
#endif
#include "MenuLauncher.h"

#if !defined ( WIN32 )
extern osMessageQId AutoDemoEvent;
#endif

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
/* Windows */
#define ID_WINDOW_0                     (GUI_ID_USER + 0x00)
/* Bitmaps */
#define ID_IMAGE_0                      (GUI_ID_USER + 0x10)
#define ID_IMAGE_1                      (GUI_ID_USER + 0x11)
#define ID_IMAGE_2                      (GUI_ID_USER + 0x12)
#define ID_IMAGE_3                      (GUI_ID_USER + 0x13)
#define ID_IMAGE_4                      (GUI_ID_USER + 0x14)
#define ID_IMAGE_5                      (GUI_ID_USER + 0x15)
/* Texts */
#define ID_TEXT_0                       (GUI_ID_USER + 0x20)
#define ID_TEXT_1                       (GUI_ID_USER + 0x21)
#define ID_TEXT_2                       (GUI_ID_USER + 0x22)
#define ID_TEXT_3                       (GUI_ID_USER + 0x23)
#define ID_TEXT_4                       (GUI_ID_USER + 0x24)
#define ID_TEXT_BOARD_ID                ID_TEXT_0
#define ID_TEXT_BOARD_VAL               ID_TEXT_1
#define ID_TEXT_CPU_ID                  ID_TEXT_2
#define ID_TEXT_CPU_VAL                 ID_TEXT_3
#define ID_TEXT_CPU_FREQ_ID             ID_TEXT_4
#define ID_TEXT_CPU_FREQ_VAL            (ID_TEXT_4 + 1)
#define ID_TEXT_FW_VER_ID               (ID_TEXT_4 + 2)
#define ID_TEXT_FW_VER_VAL              (ID_TEXT_4 + 3)
/* Buttons */
#define ID_BUTTON_DEMO_1                (GUI_ID_USER + 0x30)
#define ID_BUTTON_DEMO_2                (GUI_ID_USER + 0x31)
#define ID_BUTTON_DEMO_3                (GUI_ID_USER + 0x32)
#define ID_BUTTON_DEMO_4                (GUI_ID_USER + 0x33)
#define ID_BUTTON_INFO                  (GUI_ID_USER + 0x34)
#define ID_BUTTON_TOP                   (GUI_ID_USER + 0x35)
#define ID_BUTTON_BOTTOM                (GUI_ID_USER + 0x36)
/* Widgets */
#define ID_PROGBAR_0                    (GUI_ID_USER + 0x02)

// USER START (Optionally insert additional defines)
#define DEFAULT_BUTTON_FONT             &GUI_FontLucidaSans20
#define DIALOG_MESSAGE_FONT_1           &GUI_FontLucidaSans20
#define DIALOG_MESSAGE_FONT_2           &GUI_FontLucidaSans16
#define DIALOG_INFO_FONT_1              &GUI_FontLucidaSans16
#define DIALOG_INFO_FONT_2              &GUI_FontLucidaSans14
#define PROGRESS_BAR_FONT_1             &GUI_FontLucidaSans16
#define PROGRESS_BAR_FONT_2             &GUI_FontLucidaSans16
#define SPLASH_SCREEN_FONT_1            &GUI_FontLucidaSans20

/*
 * STemWin Demonstration context
 */
#define STEW_FIRMWARE_FILE_NAME         "STEW_firmware.bin"
#define STEW_FIRMWARE_VERSION           0xC1010000
#define STEW_FIRMWARE_ADDRESS           0x90040000
#define STEW_FIRMWARE_OFFSET            0x1000
#define STEW_FIRMWARE_MAX_SIZE          (6 * 128 * 1024)
#define STEW_RESOURCES_FILE_NAME        "STEW_resources.bin"
#define STEW_RESOURCES_VERSION          0xD1010000
#define STEW_RESOURCES_ADDRESS          0x92000000
#define STEW_RESOURCES_OFFSET           0x0
#define STEW_RESOURCES_MAX_SIZE         (32 * 1024 * 1024)

/*
 * TouchGFX Demonstration context
 */
#define TGFX_FIRMWARE_FILE_NAME         "TGFX_firmware.bin"
#define TGFX_FIRMWARE_VERSION           0xC2010000
#define TGFX_FIRMWARE_ADDRESS           0x90100000
#define TGFX_FIRMWARE_OFFSET            0x1000
#define TGFX_FIRMWARE_MAX_SIZE          (8 * 128 * 1024)
#define TGFX_RESOURCES_FILE_NAME        "TGFX_resources.bin"
#define TGFX_RESOURCES_VERSION          0xD2010000
#define TGFX_RESOURCES_ADDRESS          0x94000000
#define TGFX_RESOURCES_OFFSET           0x0
#define TGFX_RESOURCES_MAX_SIZE         (64 * 1024 * 1024)

#define GUI_STCOLOR_LIGHTBLUE           0x00DCA939
#define GUI_STCOLOR_DARKBLUE            0x00522000
#define GUI_PROGBAR_LIGHTBLUE           0x01DDA949
#define GUI_PROGBAR_DARKBLUE            0x00421000


#define OCTOSPI_PROG_SECTOR_DURATION_MS 7 /* Assuming one secor erease then programming is taking 7 ms */

#define ML_TaskPRIORITY                 osPriorityNormal /* osPriorityIdle osPriorityBelowNormal osPriorityNormal */
#define ML_TaskSTACK_SIZE               (configMINIMAL_STACK_SIZE * 4)

/*********************************************************************
*
*       Macros
*
**********************************************************************
*/
/* #define ML_FORCE_SUB_DEMO_UPGRADE   */
/* #define ML_FORCE_SUB_DEMO_DOWNGRADE */

#define GET_DEMO_VERSION_STR(tag)  "V%lu.%lu.%lu"
#define GET_DEMO_VERSION_P(tag)    ((tag  &0x00FF0000) >> 16), ((tag  &0x0000FF00) >> 8), (tag  &0x000000FF)

#if defined ( ML_FORCE_SUB_DEMO_UPGRADE )
  #define SET_SUB_DEMO_TAG(TAG)         (0xFF000000 & TAG) /* Keep only ID and set Minor version */
#elif defined ( ML_FORCE_SUB_DEMO_DOWNGRADE )
  #define SET_SUB_DEMO_TAG(TAG)         (0xFFFFFF00 & TAG) /* Keep only ID and set Major version */
#else
  #define SET_SUB_DEMO_TAG(TAG)         TAG
#endif /* ML_FORCE_SUB_DEMO_XXX */

#define GET_SUB_DEMO_ID(TAG)            (0xFF000000 & TAG)
#define GET_SUB_DEMO_VER(TAG)           (0x00FFFFFF & TAG)

#define BITMAP_OF(x)                    ((uint32_t )(x+sizeof(Demo_Header_t)))

#define MODULE_ICON_RES_X               102
#define MODULE_ICON_RES_Y               102
#define MODULE_ICON_RES                 { MODULE_ICON_RES_X, MODULE_ICON_RES_Y }

#define MODULE_LOGO_RES_X               52
#define MODULE_LOGO_RES_Y               52
#define MODULE_LOGO_RES                 { MODULE_LOGO_RES_X, MODULE_LOGO_RES_Y }

#if defined(WIN32)
extern GUI_CONST_STORAGE GUI_BITMAP bmdefault_icon;
extern GUI_CONST_STORAGE GUI_BITMAP bmdefault_logo;
#else
extern GUI_CONST_STORAGE GUI_BITMAP default_icon __attribute__ ((section ("default_icon_section")));
extern GUI_CONST_STORAGE GUI_BITMAP default_logo __attribute__ ((section ("default_logo_section")));
#endif /* WIN32 */

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

typedef struct
{
  uint32_t      address;
  uint32_t      offset;
  uint32_t      max_size;
  uint32_t      tag;
  uint32_t      reserved[4];    /* 32 bytes */
  char          filename[32];   /* 64 bytes */
} DemoInfo_t;

typedef struct
{
  char          demo_name[32];  /*  32 bytes */
  uint32_t      demo_id;
  uint32_t      icon_addr;
  Bitmap_Res_t  icon_res;
  uint32_t      logo_addr;
  Bitmap_Res_t  logo_res;
  uint32_t      reserved01;
  uint32_t      reserved02;
  uint32_t      reserved1;      /*  64 bytes */
  DemoInfo_t    firmware;       /* 128 bytes */
  DemoInfo_t    resources;      /* 192 bytes */
  uint32_t      reserved2[16];  /* 256 bytes */
} DemoContext_t;

#if defined ( __ICCARM__ )
#pragma location="DemoContextSection"
#elif ! defined ( WIN32 )
__attribute__((section(".DemoContextSection")))
#endif
static const DemoContext_t SubDemo[] = {
  {
    "STemWin", ID_BUTTON_DEMO_1, 0x0, MODULE_ICON_RES, 0x0, MODULE_LOGO_RES, 0, 0, 0,
    { STEW_FIRMWARE_ADDRESS  , STEW_FIRMWARE_OFFSET  , STEW_FIRMWARE_MAX_SIZE  , SET_SUB_DEMO_TAG(STEW_FIRMWARE_VERSION)  , { 0 }, STEW_FIRMWARE_FILE_NAME  },
    { STEW_RESOURCES_ADDRESS , STEW_RESOURCES_OFFSET , STEW_RESOURCES_MAX_SIZE , SET_SUB_DEMO_TAG(STEW_RESOURCES_VERSION) , { 0 }, STEW_RESOURCES_FILE_NAME }
  },
  {
    "TouchGFX", ID_BUTTON_DEMO_2, 0x0, MODULE_ICON_RES, 0x0, MODULE_LOGO_RES, 0, 0, 0,
    { TGFX_FIRMWARE_ADDRESS  , TGFX_FIRMWARE_OFFSET  , TGFX_FIRMWARE_MAX_SIZE  , SET_SUB_DEMO_TAG(TGFX_FIRMWARE_VERSION)  , { 0 }, TGFX_FIRMWARE_FILE_NAME  },
    { TGFX_RESOURCES_ADDRESS , TGFX_RESOURCES_OFFSET , TGFX_RESOURCES_MAX_SIZE , SET_SUB_DEMO_TAG(TGFX_RESOURCES_VERSION) , { 0 }, TGFX_RESOURCES_FILE_NAME }
  },
};

static WM_HWIN hWinSplash;   /* SplashScreen window for video splash screen and calibration window */
static WM_HWIN hWinBase;     /* Parent window for buttons and info window */
static WM_HWIN hWinInfo;     /* Info window for buttons and info window   */

#ifdef WIN32
static int test_id = 0;
#endif /* WIN32 */

#ifndef WIN32
/*********************************************************************
*
*       _aSplashScreenCreate
*/
static const GUI_WIDGET_CREATE_INFO _aSplashScreenCreate[] = {
  { WINDOW_CreateIndirect   , "Splash Screen" , ID_WINDOW_0           ,   0 ,   0 , 480 , 272 , (WM_CF_SHOW | WM_CF_STAYONTOP), 0x0 , 0 },
  { IMAGE_CreateIndirect    , "Video Frame"   , ID_IMAGE_0            ,   0 ,   0 , 480 , 272 , (WM_CF_HASTRANS | WM_CF_MEMDEV) , 0x0 , 0 },
};
#endif /* WIN32 */

/*********************************************************************
*
*       _aMainDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aMainDialogCreate[] = {
  { WINDOW_CreateIndirect   , "ST Launcher"     , ID_WINDOW_0           ,   0,   0, 480, 554,   (WM_CF_SHOW), 0x0, 0 },
  { IMAGE_CreateIndirect    , "Background"      , ID_IMAGE_0            ,   0,   0, 480, 272,   0, 0x0, 0 },
  { IMAGE_CreateIndirect    , "Reflect"         , ID_IMAGE_1            ,   0,   0, 276, 272,   0, 0x0, 0 },
  { BUTTON_CreateIndirect   , "STemWin"         , ID_BUTTON_DEMO_1      ,  90,  85, 102, 102,   0, 0x0, 4 },
  { BUTTON_CreateIndirect   , "TouchGFX"        , ID_BUTTON_DEMO_2      , 290,  85, 102, 102,   0, 0x0, 4 },
};

/*********************************************************************
*
*       _aInfoDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aInfoDialogCreate[] = {
  { WINDOW_CreateIndirect , "ST Info"             , ID_WINDOW_0           ,   0 ,   0 , 342 , 268 , (WM_CF_SHOW | WM_CF_HASTRANS | WM_CF_MEMDEV | WM_CF_STAYONTOP), 0x0, 0 },
  { IMAGE_CreateIndirect  , "Background"          , ID_IMAGE_0            ,   0 ,  19 , 342 , 262 , 0 , 0x0 , 0 },
  { IMAGE_CreateIndirect  , "Info"                , ID_IMAGE_1            , 151 ,   5 ,  40 ,  40 , 0 , 0x0 , 0 },
  { IMAGE_CreateIndirect  , "MCU Icon"            , ID_IMAGE_2            ,  56 ,  44 ,  46 ,  46 , 0 , 0x0 , 0 },
  { TEXT_CreateIndirect   , "MCU"                 , ID_TEXT_CPU_ID        , 114 ,  53 ,  92 ,  16 , (GUI_TA_LEFT | GUI_TA_VCENTER) , 0x0, 0 },
  { TEXT_CreateIndirect   , "STM32H750"           , ID_TEXT_CPU_VAL       , 124 ,  70 , 142 ,  16 , (GUI_TA_LEFT | GUI_TA_TOP)     , 0x0, 0 },
  { IMAGE_CreateIndirect  , "Board Icon"          , ID_IMAGE_3            ,  56 ,  93 ,  46 ,  46 , 0 , 0x0 , 0 },
  { TEXT_CreateIndirect   , "Board"               , ID_TEXT_BOARD_ID      , 114 , 100 ,  92 ,  16 , (GUI_TA_LEFT | GUI_TA_VCENTER) , 0x0, 0 },
  { TEXT_CreateIndirect   , "STM32H750B-DK"       , ID_TEXT_BOARD_VAL     , 124 , 117 , 142 ,  16 , (GUI_TA_LEFT | GUI_TA_TOP)     , 0x0, 0 },
  { IMAGE_CreateIndirect  , "CPU Speed Icon"      , ID_IMAGE_4            ,  56 , 142 ,  46 ,  46 , 0 , 0x0 , 0 },
  { TEXT_CreateIndirect   , "CPU Speed"           , ID_TEXT_CPU_FREQ_ID   , 114 , 150 ,  92 ,  16 , (GUI_TA_LEFT | GUI_TA_VCENTER) , 0x0, 0 },
  { TEXT_CreateIndirect   , "400 MHz"             , ID_TEXT_CPU_FREQ_VAL  , 124 , 167 , 142 ,  16 , (GUI_TA_LEFT | GUI_TA_TOP)     , 0x0, 0 },
  { IMAGE_CreateIndirect  , "Firm. Ver Icon"      , ID_IMAGE_5            ,  56 , 191 ,  46 ,  46 , 0 , 0x0 , 0 },
  { TEXT_CreateIndirect   , "Firm. Ver"           , ID_TEXT_FW_VER_ID     , 114 , 198 ,  92 ,  16 , (GUI_TA_LEFT | GUI_TA_VCENTER) , 0x0, 0 },
  { TEXT_CreateIndirect   , "V1.4.0"              , ID_TEXT_FW_VER_VAL    , 124 , 215 , 142 ,  16 , (GUI_TA_LEFT | GUI_TA_TOP)     , 0x0, 0 },
  { BUTTON_CreateIndirect , "Sub-Demos Details"   , ID_BUTTON_INFO        , 290 , 117 ,  40 ,  40 , 0, 0x0, 4 },
};

/*********************************************************************
*
*       _aDialogMessageCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogMessageCreate[] = {
  { WINDOW_CreateIndirect   , "Dialog Box"    , ID_WINDOW_0           ,   0 ,   0 , 342 , 242 , (WM_CF_SHOW | WM_CF_HASTRANS | WM_CF_MEMDEV | WM_CF_STAYONTOP), 0x0 , 0 },
  { IMAGE_CreateIndirect    , "Background"    , ID_IMAGE_0            ,   0 ,   0 , 342 , 242 , 0 , 0x0 , 0 },
  { TEXT_CreateIndirect     , "Title"         , ID_TEXT_0             ,  18 ,  21 , 302 ,  20 , (GUI_TA_HCENTER | GUI_TA_TOP)      , 0x0 , 0 },
  { TEXT_CreateIndirect     , "Text"          , ID_TEXT_1             ,  36 ,  50 , 264 , 100 , (GUI_TA_HCENTER | GUI_TA_HCENTER)  , 0x0 , 0 },
  { BUTTON_CreateIndirect   , "OK"            , GUI_ID_OK             , 104 , 160 , 132 ,  68 , 0 , 0x0 , 4 },
};

/*********************************************************************
*
*       _aMoreInfoDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aMoreInfoDialogCreate[] = {
  { WINDOW_CreateIndirect   , "Info Box"                , ID_WINDOW_0       ,   0 ,   0 , 342 , 242 , (WM_CF_SHOW | WM_CF_HASTRANS | WM_CF_MEMDEV | WM_CF_STAYONTOP), 0x0 , 0 },
  { IMAGE_CreateIndirect    , "Background"              , ID_IMAGE_0        ,   0 ,   0 , 342 , 242 , 0 , 0x0 , 0 },
  { TEXT_CreateIndirect     , "Sub-Demo Details"        , ID_TEXT_0         ,  18 ,  21 , 302 ,  20 , (GUI_TA_HCENTER | GUI_TA_TOP)   , 0x0 , 0 },
  { IMAGE_CreateIndirect    , "Icon1"                   , ID_IMAGE_1        ,  40 ,  50 ,  50 ,  50 , 0 , 0x0 , 0 },
  { TEXT_CreateIndirect     , "Sub-Demo Not Installed"  , ID_TEXT_1         , 100 ,  60 , 200 ,  30 , (GUI_TA_LEFT | GUI_TA_VCENTER)  , 0x0 , 0 },
  { IMAGE_CreateIndirect    , "Icon2"                   , ID_IMAGE_2        ,  40 , 110 ,  50 ,  50 , 0 , 0x0 , 0 },
  { TEXT_CreateIndirect     , "Sub-Demo Not Installed"  , ID_TEXT_2         , 100 , 120 , 200 ,  30 , (GUI_TA_LEFT | GUI_TA_VCENTER)  , 0x0 , 0 },
  { BUTTON_CreateIndirect   , "OK"                      , GUI_ID_OK         , 104 , 160 , 132 ,  68 , 0 , 0x0 , 4 },
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

static int GetDemoIndex (uint32_t Button_Id)
{
  int index;

  for(index = 0; index < DEMO_ID_INVALID; index++)
  {
    if (SubDemo[index].demo_id == Button_Id)
      return index;
  }

  return -1;
}

#ifndef WIN32
static uint32_t GetBitmapSize(GUI_CONST_STORAGE GUI_BITMAP *bitmap)
{
  uint8_t *bmp_header = (uint8_t *)bitmap;
  uint32_t *file_size = (uint32_t *)(&bmp_header[2]);
  return *file_size;
}
#endif /* WIN32 */

/**
  * @brief  Paints ok message button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_okMessage(BUTTON_Handle hObj) {
  /* Nothing to do for paint */
}

/**
  * @brief  callback for ok message button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_okMessage(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId)
  {
    case WM_PAINT:
      _OnPaint_okMessage(pMsg->hWin);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  Callback routine of More Info dialog
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbMoreInfoDialog(WM_MESSAGE * pMsg)
{
  int Id, Demo_Id;
  WM_HWIN hItem;
#ifndef WIN32
  GUI_CONST_STORAGE GUI_BITMAP *button_bmp;
  Demo_Header_t *pResDemoHeader = 0;
  Demo_Header_t *pFirmDemoHeader = 0;
  char tmp[64] = { 0 };
#endif /* WIN32 */

  switch (pMsg->MsgId)
  {
    case WM_INIT_DIALOG:
    {
      WM_SetHasTrans(WM_GetClientWindow(pMsg->hWin));
      WINDOW_SetBkColor(pMsg->hWin, GUI_INVALID_COLOR);

      hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_0);
      IMAGE_SetBitmap(hItem, &bmdialog_box);

      hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
      TEXT_SetBkColor(hItem, GUI_INVALID_COLOR);
      TEXT_SetFont(hItem, DIALOG_MESSAGE_FONT_1);
      TEXT_SetTextColor(hItem, GUI_WHITE);
      TEXT_SetWrapMode(hItem, GUI_WRAPMODE_WORD);

      for (Id = 0; Id < 2; Id++)
      {
        Demo_Id = GetDemoIndex(Id+ID_BUTTON_DEMO_1);
        if(Demo_Id >= 0)
        {
#ifndef WIN32
          pFirmDemoHeader = (Demo_Header_t *)(SubDemo[Demo_Id].firmware.address + SubDemo[Demo_Id].firmware.offset);
          pResDemoHeader = (Demo_Header_t *)(SubDemo[Demo_Id].resources.address + SubDemo[Demo_Id].resources.offset);

          if(  (SubDemo[Demo_Id].firmware.tag != 0xFFFFFFFF)
            && (SubDemo[Demo_Id].resources.tag != 0xFFFFFFFF)
            && (pFirmDemoHeader->demo_id != 0xFFFFFFFF)
            && (pResDemoHeader->demo_id != 0xFFFFFFFF)
            && (SubDemo[Demo_Id].logo_res.x == MODULE_LOGO_RES_X)
            && (SubDemo[Demo_Id].logo_res.y == MODULE_LOGO_RES_Y)
            && (pResDemoHeader->logo_addr != 0)
            && (GUI_BMP_GetXSize((GUI_CONST_STORAGE GUI_BITMAP *)(pResDemoHeader->logo_addr)) > 0)
            && (GUI_BMP_GetXSize((GUI_CONST_STORAGE GUI_BITMAP *)(pResDemoHeader->logo_addr)) <= SubDemo[Demo_Id].logo_res.x)
            && (GUI_BMP_GetYSize((GUI_CONST_STORAGE GUI_BITMAP *)(pResDemoHeader->logo_addr)) > 0)
            && (GUI_BMP_GetYSize((GUI_CONST_STORAGE GUI_BITMAP *)(pResDemoHeader->logo_addr)) <= SubDemo[Demo_Id].logo_res.y))
          {
            /* Use Bitmap Icon */
            button_bmp = (GUI_CONST_STORAGE GUI_BITMAP *)(pResDemoHeader->logo_addr);
            /* Update Sub-Demo Details */
            hItem = WM_GetDialogItem(pMsg->hWin, (ID_TEXT_1+Id));
            sprintf((char *)tmp , "%s " GET_DEMO_VERSION_STR(pFirmDemoHeader->demo_id), pFirmDemoHeader->demo_descr, GET_DEMO_VERSION_P(pFirmDemoHeader->demo_id));
            TEXT_SetText(hItem, tmp);
          }
          else
          {
            /* Use Default Bitmap Icon */
            button_bmp = &default_logo;
          }
          /* Set Bitmap Icon */
          hItem = WM_GetDialogItem(pMsg->hWin, (ID_IMAGE_1+Id));
		  IMAGE_SetBMP(hItem, button_bmp, GetBitmapSize(button_bmp));
#else
          /* Set Bitmap Icon */
          hItem = WM_GetDialogItem(pMsg->hWin, (ID_IMAGE_1 + Id));
          IMAGE_SetBitmap(hItem, &bmdefault_logo);
#endif /* WIN32 */
          /* Set Text attributes */
          hItem = WM_GetDialogItem(pMsg->hWin, (ID_TEXT_1+Id));
          TEXT_SetBkColor(hItem, GUI_INVALID_COLOR);
          TEXT_SetFont(hItem, DIALOG_INFO_FONT_2);
          TEXT_SetTextColor(hItem, GUI_WHITE);
          TEXT_SetWrapMode(hItem, GUI_WRAPMODE_WORD);
        }
      }

      hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_OK);
      WM_SetCallback(hItem, _cbButton_okMessage);

      WM_HideWindow(hWinInfo);
    }
    break;

    case WM_NOTIFY_PARENT:
      if (pMsg->Data.v == WM_NOTIFICATION_RELEASED)
      {
        Id = WM_GetId(pMsg->hWinSrc);
        if (Id == GUI_ID_OK)
        {
          GUI_EndDialog(pMsg->hWin, 1);
        }
      }
      break;

    case WM_DELETE :
      WM_ShowWindow(hWinInfo);
      break;

    default:
      WM_DefaultProc(pMsg);
      break;
  }
}

/**
  * @brief  Callback routine of Message dialog
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbDialogMessage(WM_MESSAGE * pMsg)
{
  int     Id;
  WM_HWIN hItem;

  switch (pMsg->MsgId)
  {
    case WM_INIT_DIALOG:
    {
      WM_SetHasTrans(WM_GetClientWindow(pMsg->hWin));
      WINDOW_SetBkColor(pMsg->hWin, GUI_INVALID_COLOR);

      hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_0);
      IMAGE_SetBitmap(hItem, &bmdialog_box);

      hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
      TEXT_SetBkColor(hItem, GUI_INVALID_COLOR);
      TEXT_SetFont(hItem, DIALOG_MESSAGE_FONT_1);
      TEXT_SetTextColor(hItem, GUI_WHITE);
      TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
      TEXT_SetWrapMode(hItem, GUI_WRAPMODE_WORD);

      hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
      TEXT_SetBkColor(hItem, GUI_INVALID_COLOR);
      TEXT_SetFont(hItem, DIALOG_MESSAGE_FONT_2);
      TEXT_SetTextColor(hItem, GUI_WHITE);
      TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
      TEXT_SetWrapMode(hItem, GUI_WRAPMODE_WORD);

      hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_OK);
      WM_SetCallback(hItem, _cbButton_okMessage);

      WM_HideWindow(hWinInfo);
    }
    break;

    case WM_NOTIFY_PARENT:
      if (pMsg->Data.v == WM_NOTIFICATION_RELEASED)
      {
        Id = WM_GetId(pMsg->hWinSrc);
        if (Id == GUI_ID_OK)
        {
          GUI_EndDialog(pMsg->hWin, 1);
        }
      }
      break;

    case WM_DELETE :
      WM_ShowWindow(hWinInfo);
      break;

    default:
      WM_DefaultProc(pMsg);
      break;
  }
}

/**
* @brief  Show Message Box
* @param  hWin: pointer to the parent window
* @param  pMsgItem: pointer to the message item data
* @param  text_id: text ID used to get then set the Dialog Box user data
* @retval None
*/
static int _ShowDialogBox(WM_HWIN hWin, const char* pTitle, const char* pText)
{
  WM_HWIN hFrame, hItem;

  /* Create and display the Dialog Box */
  hFrame = GUI_CreateDialogBox(_aDialogMessageCreate,
                              GUI_COUNTOF(_aDialogMessageCreate),
                              _cbDialogMessage,
                              hWin,
                              70, 20);

  hItem = WM_GetDialogItem(hFrame, ID_TEXT_0);
  TEXT_SetText(hItem, pTitle);

  hItem = WM_GetDialogItem(hFrame, ID_TEXT_1);
  TEXT_SetText(hItem, pText);

  /* Show the Dialog Box */
  WM_MakeModal(hFrame);

  return GUI_ExecCreatedDialog(hFrame);
}


/**
* @brief  Show More Info Box
* @param  hWin: pointer to the parent window
* @param  pMsgItem: pointer to the message item data
* @param  text_id: text ID used to get then set the Dialog Box user data
* @retval None
*/
static int _ShowMoreInfoBox(WM_HWIN hWin)
{
  WM_HWIN hFrame;

  /* Create and display the Dialog Box */
  hFrame = GUI_CreateDialogBox(_aMoreInfoDialogCreate,
                              GUI_COUNTOF(_aMoreInfoDialogCreate),
                              _cbMoreInfoDialog,
                              hWin,
                              70, 20);

  /* Show the Dialog Box */
  WM_MakeModal(hFrame);

  return GUI_ExecCreatedDialog(hFrame);
}

#ifdef WIN32
/**
  * @brief  SUBDEMO_Sim
  * @param  None
  * @retval None
  */
static void SUBDEMO_Sim(WM_HWIN hWin, const DemoContext_t *pContext)
{

  WM_HWIN hFrame;
  PROGBAR_Handle hProg;
  int Value = 0;
  char message[256] = { 0 };

  if (test_id == 5)
  {
    test_id = 0;
  }

  if (test_id == 0)
  {
    /* Calculate estimated programming duration */
    uint32_t file_size = 20 * 1024 * 1024; /* 20 MBytes --> 320 * 64 Kbytes sectors */
    uint32_t duration_ms = (file_size/(1024 * 64)) *  OCTOSPI_PROG_SECTOR_DURATION_MS; /* Use appropriate value */

    snprintf(message, sizeof(message), "%s demo resources download\n\n* Expected duration : %d minutes *\nCarry on?", pContext->demo_name, (duration_ms/1000));
    if(!_ShowMessageBox(hWin, "QUESTION", message))
      return;
  }

  if (test_id == 0)
  {
    /* Display error message: No storage media inserted! */
    _ShowDialogBox(hWin, "FAILED", "No storage media inserted!\nPlease insert storage media which includes valid binary files.");

    test_id++;
  }
  else if (test_id == 1)
  {
    /* Display error message: Invalid Firmware binary file */
    snprintf(message, sizeof(message), "Invalid firmware binary file\nfor %s demo", pContext->demo_name);
    _ShowDialogBox(hWin, "FAILED", message);

    test_id++;
  }
  else if (test_id == 2)
  {
    /* Display error message: Invalid Demo Resources file */
    snprintf(message, sizeof(message), "Invalid resources file\nfor %s demo", pContext->demo_name);
    _ShowDialogBox(hWin, "FAILED", message);

    test_id++;
  }
  else if (test_id == 3)
  {
    /* Display error message: Missing Resource binary file */
    snprintf(message, sizeof(message), "Missing resources binary file\nfor %s demo", pContext->demo_name);
    _ShowDialogBox(hWin, "FAILED", message);

    test_id++;
  }
  else if (test_id == 4)
  {
    /* Simulate Firmware binary file programming */
    snprintf(message, sizeof(message), "%s Firmware Programming...", pContext->demo_name);

    /* Create and display the Dialog Box */
    hFrame = _ShowProgressBar(hWin, message);
    hProg = WM_GetDialogItem(hFrame, ID_PROGBAR_0);
	PROGBAR_SetMinMax(hProg, 0, 512);
	WM_SetFocus(hFrame);

    Value = 0;
    while (1)
    {
      GUI_X_Delay(1);
      int val = PROGBAR_GetValue(hProg);
      if (val == 512)
        break;
      Value++;
      PROGBAR_SetValue(hProg, Value);
      GUI_Exec1();
    }

    WM_DeleteWindow(hFrame);

    /* Simulate Resources binary file programming */
    snprintf(message, sizeof(message), "%s Resources Programming...", pContext->demo_name);

    /* Create and display the Dialog Box */
    hFrame = _ShowProgressBar(hWin, message);
    hProg = WM_GetDialogItem(hFrame, ID_PROGBAR_0);
	PROGBAR_SetMinMax(hProg, 0, 128);
    WM_SetFocus(hFrame);

    Value = 0;
    while (1)
    {
      GUI_X_Delay(OCTOSPI_PROG_SECTOR_DURATION_MS);
      int val = PROGBAR_GetValue(hProg);
      if (val == 128)
              break;
      Value++;
      PROGBAR_SetValue(hProg, Value);
      GUI_Exec1();
    }

    WM_DeleteWindow(hFrame);
    GUI_Delay(10);

    /* Display error message: Invalid Demo Resources file */
    snprintf(message, sizeof(message), "Firmware and Resources programming successfully done\nfor %s demo", pContext->demo_name);
    _ShowDialogBox(hWin, "RESTART NOW", message);

    test_id++;
  }
}
#endif /* WIN32 */

#ifndef WIN32
void SUBDEMO_StartAutoDemo(const uint8_t demo_id)
{
  DemoContext_t const *pContext = NULL;
  Demo_Header_t const *pFirmDemoHeader = NULL;
  Demo_Header_t const *pResDemoHeader = NULL;
  const uint8_t demos_count  = (sizeof(SubDemo)/sizeof(SubDemo[0]));

  if( demo_id < demos_count )
  {
    pContext = &SubDemo[demo_id];
    if( (pContext->demo_id != 0x0) && (pContext->demo_id != 0xFFFFFFFF)
     && (pContext->firmware.tag != 0x0) && (pContext->firmware.tag != 0xFFFFFFFF)
     && (pContext->firmware.address != 0x0) && (pContext->firmware.address != 0xFFFFFFFF) )
    {
      pFirmDemoHeader = (Demo_Header_t *)(pContext->firmware.address + pContext->firmware.offset);
      pResDemoHeader = (Demo_Header_t *)(pContext->resources.address + pContext->resources.offset);
      if ( (pFirmDemoHeader && ((pFirmDemoHeader->sw_flags & SW_FLAG_AUTO_DEMO) == SW_FLAG_AUTO_DEMO)
        && (pFirmDemoHeader->demo_id == pContext->firmware.tag) )
      &&   ( pResDemoHeader && (pResDemoHeader->demo_id == pContext->resources.tag)
        && ( (pResDemoHeader->icon_addr != 0x0) && (pResDemoHeader->icon_addr != 0xFFFFFFFF)
          && (pResDemoHeader->icon_res.x == pContext->icon_res.x) && (pResDemoHeader->icon_res.y == pContext->icon_res.y) )
        && ( (pResDemoHeader->logo_addr != 0x0) && (pResDemoHeader->logo_addr != 0xFFFFFFFF)
          && (pResDemoHeader->logo_res.x == pContext->logo_res.x) && (pResDemoHeader->logo_res.y == pContext->logo_res.y) ) ) )
      {
        /* Now Jump to the selected demo */
        BSP_JumpToSubDemo(pContext->firmware.address);
      }
    }
  }
}

/**
  * @brief  SUBDEMO_Start
  * @param  None
  * @retval None
  */
static void SUBDEMO_Start(WM_HWIN hWin, const DemoContext_t *pStaticContext)
{
  Demo_Header_t *   pResDemoHeader      = 0;
  Demo_Header_t *   pFirmDemoHeader     = 0;
  char              message[256];

  memset(message, 0, sizeof(message));
  osMessagePut ( AutoDemoEvent, AUTO_DEMO_DISABLE, 0);

  /* Error Management */
  pFirmDemoHeader = (Demo_Header_t *)(pStaticContext->firmware.address + pStaticContext->firmware.offset);
  pResDemoHeader = (Demo_Header_t *)(pStaticContext->resources.address + pStaticContext->resources.offset);

  if( (GET_SUB_DEMO_ID(pFirmDemoHeader->demo_id) == GET_SUB_DEMO_ID(pStaticContext->firmware.tag))
   || (GET_SUB_DEMO_ID(pResDemoHeader->demo_id) == GET_SUB_DEMO_ID(pStaticContext->resources.tag)) )
  {
    /* Now Jump to the programmed demo */
    BSP_JumpToSubDemo(pStaticContext->firmware.address);
  }
  else
  {
    snprintf(message, sizeof(message), "%s Sub-Demo is not installed", pStaticContext->demo_name);
    _ShowDialogBox(hWin, "FAILED", message);
    /* Refresh Main screen */
    WM_InvalidateWindow(hWin);

    osMessagePut ( AutoDemoEvent, AUTO_DEMO_ENABLE, 0);
  }
}
#endif /* WIN32 */

/**
* @brief  Paints no message button
* @param  hObj: button handle
* @retval None
*/
static void _OnPaintDefaultButton(BUTTON_Handle hObj, const char *text)
{
  GUI_RECT                Rect;

  /* Receive the area of the BUTTON widget */
  WM_GetClientRectEx(hObj, &Rect);

  if ((WIDGET_GetState(hObj) & BUTTON_STATE_PRESSED))
  {
    GUI_SetColor(GUI_LIGHTGRAY);
  }
  else
  {
    GUI_SetColor(GUI_WHITE);
  }

  GUI_SetTextMode(GUI_TM_TRANS);
  GUI_SetBkColor(GUI_TRANSPARENT);
  GUI_SetFont(DEFAULT_BUTTON_FONT);
  GUI_DispStringInRect(text, &Rect, (GUI_TA_VCENTER | GUI_TA_HCENTER));
}

/**
  * @brief  Paints buttons
  * @param  hButton: button handle
  * @retval None
  */
static void _OnPaintButton(BUTTON_Handle hButton) {

  int Demo_Id = -1;
  int Id;
  int size_x, size_y;
#ifndef WIN32
  GUI_CONST_STORAGE GUI_BITMAP *button_bmp;
  Demo_Header_t *pResDemoHeader = 0;
  Demo_Header_t *pFirmDemoHeader = 0;
#endif /* WIN32 */

  GUI_SetBkColor(GUI_TRANSPARENT);

  BUTTON_GetUserData(hButton, &Id, sizeof(Id));
  switch (Id)
  {
    case ID_BUTTON_DEMO_1 :
    case ID_BUTTON_DEMO_2 :
    case ID_BUTTON_DEMO_3 :
    case ID_BUTTON_DEMO_4 :
      Demo_Id = GetDemoIndex(Id);
      if(Demo_Id >= 0)
      {
#ifndef WIN32
        pFirmDemoHeader = (Demo_Header_t *)(SubDemo[Demo_Id].firmware.address + SubDemo[Demo_Id].firmware.offset);
        pResDemoHeader = (Demo_Header_t *)(SubDemo[Demo_Id].resources.address + SubDemo[Demo_Id].resources.offset);

        if(  (SubDemo[Demo_Id].firmware.tag != 0xFFFFFFFF)
          && (SubDemo[Demo_Id].resources.tag != 0xFFFFFFFF)
          && (pFirmDemoHeader->demo_id != 0xFFFFFFFF)
          && (pResDemoHeader->demo_id != 0xFFFFFFFF)
          && (SubDemo[Demo_Id].icon_res.x == MODULE_ICON_RES_X)
          && (SubDemo[Demo_Id].icon_res.y == MODULE_ICON_RES_Y)
          && (pResDemoHeader->icon_addr != 0)
          && (GUI_BMP_GetXSize((GUI_CONST_STORAGE GUI_BITMAP *)(pResDemoHeader->icon_addr)) > 0)
          && (GUI_BMP_GetXSize((GUI_CONST_STORAGE GUI_BITMAP *)(pResDemoHeader->icon_addr)) <= SubDemo[Demo_Id].icon_res.x)
          && (GUI_BMP_GetYSize((GUI_CONST_STORAGE GUI_BITMAP *)(pResDemoHeader->icon_addr)) > 0)
          && (GUI_BMP_GetYSize((GUI_CONST_STORAGE GUI_BITMAP *)(pResDemoHeader->icon_addr)) <= SubDemo[Demo_Id].icon_res.y))
        {
          button_bmp = (GUI_CONST_STORAGE GUI_BITMAP *)(pResDemoHeader->icon_addr);
        }
        else
        {
          button_bmp = &default_icon;
        }

        size_x = GUI_BMP_GetXSize(button_bmp);
        size_y = GUI_BMP_GetYSize(button_bmp);

        GUI_BMP_EnableAlpha();
        GUI_BMP_Draw(button_bmp, ((MODULE_ICON_RES_X - size_x) / 2), ((MODULE_ICON_RES_Y - size_y) / 2));
#else
		size_x = MODULE_ICON_RES_X;
		size_y = MODULE_ICON_RES_Y;
		GUI_DrawBitmap(&bmdefault_icon, ((MODULE_ICON_RES_X - size_x) / 2), ((MODULE_ICON_RES_Y - size_y) / 2));
#endif /* WIN32 */
      }
      else
      {
        _OnPaintDefaultButton(hButton, "+");
      }
      break;
    case ID_BUTTON_INFO :
      GUI_DrawBitmap(&bmarrow_right, (WM_GetWindowSizeX(hButton) - (bmarrow_right.XSize))/2 , (WM_GetWindowSizeY(hButton) - (bmarrow_right.YSize))/2);
      break;
    default:
      _OnPaintDefaultButton(hButton, "Button");
      break;
  }
}

/**
  * @brief  callback for buttons
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
    case WM_PAINT:
      _OnPaintButton(pMsg->hWin);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

typedef enum window_position_e
{
  WIN_BOTTOM
, WIN_TOP
, WIN_CENTER
, WIN_RIGHT
, WIN_LEFT
} window_position_t;

/**
* @brief  Callback routine of the Info dialog
* @param  pMsg: pointer to data structure of type WM_MESSAGE
* @retval None
*/
static void _cbInfoDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int          NCode;
  int          Id;
  static window_position_t window_position = WIN_BOTTOM;
  signed long speed = 0;

  GUI_CONST_STORAGE GUI_BITMAP *bitmap_icon[] = {
    &bmmcu_icon, &bmboard_icon
  , &bmcpu_icon, &bmfirmware_icon
  };
  const uint8_t bitmap_cnt  = (sizeof(bitmap_icon)/sizeof(bitmap_icon[0]));
  const uint16_t text_id[] = {
    ID_TEXT_CPU_ID, ID_TEXT_CPU_VAL
  , ID_TEXT_BOARD_ID, ID_TEXT_BOARD_VAL
  , ID_TEXT_CPU_FREQ_ID, ID_TEXT_CPU_FREQ_VAL
  , ID_TEXT_FW_VER_ID, ID_TEXT_FW_VER_VAL
  };
  const uint8_t text_cnt  = (sizeof(text_id)/sizeof(text_id[0]));

#ifndef WIN32
  char tmp[16];
#endif /* WIN32 */

	switch (pMsg->MsgId) {
	case WM_INIT_DIALOG:
    WINDOW_SetBkColor(pMsg->hWin, GUI_INVALID_COLOR);
    hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_0);
    IMAGE_SetBitmap(hItem, &bminfo_screen);
    hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_1);
    IMAGE_SetBitmap(hItem, &bminfo_icon);
    for(Id = 0; Id < bitmap_cnt; Id++)
    {
      hItem = WM_GetDialogItem(pMsg->hWin, (ID_IMAGE_2+Id));
      IMAGE_SetBitmap(hItem, bitmap_icon[Id]);
    }
    for(Id = 0; Id < text_cnt; Id+=2)
    {
      hItem = WM_GetDialogItem(pMsg->hWin, text_id[Id]);
      TEXT_SetBkColor(hItem, GUI_INVALID_COLOR);
      TEXT_SetTextColor(hItem, GUI_WHITE);
      TEXT_SetFont(hItem, DIALOG_INFO_FONT_1);
    }
    for(Id = 1; Id < text_cnt; Id+=2)
    {
      hItem = WM_GetDialogItem(pMsg->hWin, text_id[Id]);
      TEXT_SetBkColor(hItem, GUI_INVALID_COLOR);
      TEXT_SetTextColor(hItem, GUI_WHITE);
      TEXT_SetFont(hItem, DIALOG_INFO_FONT_2);
    }
#ifndef WIN32
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_CPU_FREQ_VAL);
    sprintf((char *)tmp , "%lu MHz",  (HAL_RCCEx_GetD1SysClockFreq()/1000000U));
    TEXT_SetText(hItem, tmp);
#endif /* WIN32 */

    Id = ID_BUTTON_INFO;
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_INFO);
    BUTTON_SetUserData(hItem, &Id, sizeof(Id));
    WM_DisableWindow(hItem);
    WM_SetCallback(hItem, _cbButton);
    break;

  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    if(NCode == WM_NOTIFICATION_RELEASED)
    {
      switch(Id) {

        case ID_BUTTON_INFO:
          _ShowMoreInfoBox(WM_GetParent(pMsg->hWin));
          break;

        default:
          if( (window_position == WIN_CENTER) || (window_position == WIN_BOTTOM))
          {
            if (window_position == WIN_BOTTOM)
            {
              speed = -1300;
            }
            else
            {
              speed = +1200;
            }

            WM_MOTION_SetMovement(pMsg->hWin, GUI_COORD_Y, speed, 236);

            if (window_position == WIN_BOTTOM)
            {
              window_position = WIN_CENTER;
              hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_INFO);
              WM_EnableWindow(hItem);
#if !defined ( WIN32 )
              osMessagePut ( AutoDemoEvent, AUTO_DEMO_DISABLE, 0);
#endif /* ! WIN32 */
              WM_MakeModal(pMsg->hWin);
            }
            else
            {
              window_position = WIN_BOTTOM;
              hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_INFO);
              WM_DisableWindow(hItem);
#if !defined ( WIN32 )
              osMessagePut ( AutoDemoEvent, AUTO_DEMO_ENABLE, 0);
#endif /* ! WIN32 */
              WM_MakeModal(WM_GetParent(pMsg->hWin));
            }
          }
          break;
      }
    }
    break;

	case WM_DELETE:
		break;

  default:
		WM_DefaultProc(pMsg);
		break;
	}
}

/*********************************************************************
*
*       _cbMainDialog
*/
static void _cbMainDialog(WM_MESSAGE * pMsg) {
  WM_HWIN      hItem;
  int          NCode;
  int          Id;
  int          Demo_Id = -1;

  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    GUI_SetBkColor(GUI_BLACK);
    GUI_Clear();

    hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_0);
    IMAGE_SetBitmap(hItem, &bmbackground);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_1);
    IMAGE_SetBitmap(hItem, &bmreflect);

    Demo_Id = ID_BUTTON_DEMO_1;
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_DEMO_1);
    BUTTON_SetUserData(hItem, &Demo_Id, sizeof(Demo_Id));
    WM_SetCallback(hItem, _cbButton);

    Demo_Id = ID_BUTTON_DEMO_2;
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_DEMO_2);
    BUTTON_SetUserData(hItem, &Demo_Id, sizeof(Demo_Id));
    WM_SetCallback(hItem, _cbButton);

    break;

  case WM_NOTIFY_PARENT:
    Demo_Id = -1;
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    if(NCode == WM_NOTIFICATION_RELEASED)
    {
      switch(Id) {
        case ID_BUTTON_DEMO_1: // Notifications sent by 'STemWin'
        case ID_BUTTON_DEMO_2: // Notifications sent by 'TouchGFX'
          // USER START (Optionally insert code for reacting on notification message)
          Demo_Id = GetDemoIndex(Id);
          break;
      }
    }

    if (Demo_Id >= 0)
    {
#ifdef WIN32
      SUBDEMO_Sim(pMsg->hWin, &SubDemo[Demo_Id]);
#else
      SUBDEMO_Start(pMsg->hWin, &SubDemo[Demo_Id]);
#endif /* WIN32 */
    }
    break;

  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

#ifndef WIN32
/*********************************************************************
*
*       _cbSplashScreen
*/
static void _cbSplashScreen(WM_MESSAGE * pMsg) {
  int           NCode;

  switch (pMsg->MsgId) {
    case WM_NOTIFY_PARENT:
      NCode = pMsg->Data.v;
      if(NCode == WM_NOTIFICATION_RELEASED)
      {
        SplashScreen_Stop();
      }
    break;

    case WM_DELETE :
      hWinSplash = 0;
      break;

    default:
      WM_DefaultProc(pMsg);
    break;
  }
}


/*********************************************************************
*
*       _SplashScreen
*
*  Function description:
*    Show the Splash Screen at startup.
*/
static void _SplashScreen(WM_HWIN hWinBase, int xSize, int ySize)
{
  hWinSplash = GUI_CreateDialogBox(_aSplashScreenCreate, GUI_COUNTOF(_aSplashScreenCreate), _cbSplashScreen, hWinBase, 0, 0);
  if(hWinSplash)
  {
    WM_HWIN hItem = WM_GetDialogItem(hWinSplash, ID_IMAGE_0);
    WM_EnableMemdev(hItem);
    SplashScreen_Start(hItem);
    GUI_EndDialog(hWinSplash, 0);
  }
}
#endif /* WIN32 */

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
void LauncherStartUp(const uint8_t SoftwareReset)
{
  int xSize, ySize;

  BUTTON_SetReactOnLevel();

  /* Get display dimension */
  xSize = LCD_GetXSize();
  ySize = LCD_GetYSize();

  /* Limit desktop window to display size */
  WM_SetSize(WM_HBKWIN, xSize, ySize);

#ifndef WIN32
  if(SoftwareReset == 0)
  {
    osMessagePut ( AutoDemoEvent, AUTO_DEMO_DISABLE, 0);
    _SplashScreen(WM_HBKWIN, xSize, ySize);
    osMessagePut ( AutoDemoEvent, AUTO_DEMO_ENABLE, 0);
  }
#endif /* ! WIN32 */

  /* Create windows */
  hWinBase = GUI_CreateDialogBox(_aMainDialogCreate, GUI_COUNTOF(_aMainDialogCreate), _cbMainDialog, WM_HBKWIN, 0, 0);
  hWinInfo = GUI_CreateDialogBox(_aInfoDialogCreate, GUI_COUNTOF(_aInfoDialogCreate), _cbInfoDialog, hWinBase, 70, 236);
}

#ifdef WIN32
/*
*       MainTask
*/
void MainTask(void)
{
  /* Initialize the GUI */
  GUI_Init();

  WM_MULTIBUF_Enable(1);
  WM_MOTION_Enable(1);

  GUI_DispStringAt("Starting...", 0, 0);

  /* Initialize LCD and LEDs */
  GUI_DispStringAt("Initializing lcd...", 0, 12);

  LauncherStartUp(0);
  GUI_SPY_StartServer();

  /* Keep demo alive */
  while (1) {
    GUI_Delay(20);
  }
}
#endif /* WIN32 */

