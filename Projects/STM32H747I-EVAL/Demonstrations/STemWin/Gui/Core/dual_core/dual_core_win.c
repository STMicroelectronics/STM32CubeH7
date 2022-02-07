/**
  ******************************************************************************
  * @file    dual_core_win.c
  * @author  MCD Application Team
  * @brief   dual core functions
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

/* Includes ------------------------------------------------------------------*/
#include "DIALOG.h"
#include "main.h"
#include "k_modules_res.h"

#include <stdint.h>
#include <stdbool.h>

#include "DIALOG.h"

#include <stdint.h>
#include <cmsis_os.h>

#include <GUI_AVI_2.h>

#include "video_player_app_2.h"

/* Genereted bitmaps used for the dual core module */
#include "dual_core_res.c"

extern GUI_CONST_STORAGE GUI_BITMAP bmmenu;
extern GUI_CONST_STORAGE GUI_BITMAP bmbkc_16bit;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontRounded22;

extern uint8_t DualCoreEnabled;
extern uint8_t toggle_chromART;
extern uint8_t AVI_VideoBuffer[AVI_VIDEO_BUF_SIZE];
extern uint8_t AVI_AudioBuffer[AUDIO_BUFFER_SIZE + 8];

static WM_HWIN hWinDualCore;
static void Startup(WM_HWIN hWin, U16 xpos, U16 ypos);
static void _cbMediaConnection(WM_MESSAGE * pMsg);
static STORAGE_IdTypeDef storage_id = USB_DISK_UNIT;

K_ModuleItem_Typedef dual_core_board =
{
    MODULE_DUAL_CORE,
    "Dual Core",
    &hWinDualCore,
    0,
    0,
    Startup,
    NULL,
};

/* Windows and Wigets */
#define ID_WINDOW_0       (GUI_ID_USER + 0x00)
/* Images */
#define ID_IMAGE_0        (GUI_ID_USER + 0x10)
#define ID_IMAGE_1        (GUI_ID_USER + 0x11)
#define ID_IMAGE_2        (GUI_ID_USER + 0x12)
#define ID_IMAGE_3        (GUI_ID_USER + 0x13)
#define ID_IMAGE_4        (GUI_ID_USER + 0x14)
#define ID_IMAGE_5        (GUI_ID_USER + 0x15)
/* Textes */
#define ID_TEXT_FRACTAL   (GUI_ID_USER + 0x20)
#define ID_TEXT_CPU_LOAD  (GUI_ID_USER + 0x21)
#define ID_TEXT_FPS_0     (GUI_ID_USER + 0x22)
#define ID_TEXT_FPS_1     (GUI_ID_USER + 0x23)
#define ID_TEXT_FPS_2     (GUI_ID_USER + 0x24)
#define ID_TEXT_FPS_3     (GUI_ID_USER + 0x25)
#define ID_TEXT_FPS_4     (GUI_ID_USER + 0x26)
#define ID_TEXT_FRQ_CM4   (GUI_ID_USER + 0x27)
#define ID_TEXT_FRQ_CM7   (GUI_ID_USER + 0x28)
/* Buttons */
#define ID_BUTTON_MENU    (GUI_ID_USER + 0x30)
#define ID_BUTTON_CM4     (GUI_ID_USER + 0x31)
/* Timers */
#define ID_TIMER_0        (GUI_ID_USER + 0x40)

#define FRACTAL_WIDHT     160
#define FRACTAL_HEIGHT    120
#define VIDEO_WIDHT       240
#define VIDEO_HEIGHT      192
#define VIDEO_BPP         16
#define VIDEO_COUNT       4

#if defined ( __ICCARM__ )
#pragma data_alignment=32
#pragma location="jpeg_decoder_buffers"
#elif defined(__CC_ARM)
__attribute__((section(".jpeg_decoder_buffers"), zero_init)) __attribute__ ((aligned (32)))
#elif defined(__GNUC__)
__attribute__((section(".jpeg_decoder_buffers"))) __attribute__ ((aligned (32)))
#endif
uint32_t clut8[256];

#if defined ( __ICCARM__ )
#pragma data_alignment=32
#pragma location="jpeg_decoder_buffers"
#elif defined(__CC_ARM)
__attribute__((section(".jpeg_decoder_buffers"), zero_init)) __attribute__ ((aligned (32)))
#elif defined(__GNUC__)
__attribute__((section(".jpeg_decoder_buffers"))) __attribute__ ((aligned (32)))
#endif
uint8_t Fractalbuffer[FRACTAL_WIDHT * FRACTAL_HEIGHT];

#if defined ( __ICCARM__ )
#pragma data_alignment=32
#pragma location="rgb_video_buffers"
#elif defined(__CC_ARM)
__attribute__((section(".rgb_video_buffers"), zero_init)) __attribute__ ((aligned (32)))
#elif defined(__GNUC__)
__attribute__((section(".rgb_video_buffers"))) __attribute__ ((aligned (32)))
#endif
uint16_t MJPEGDecodeBuffer[VIDEO_COUNT][VIDEO_WIDHT * VIDEO_HEIGHT];

static void CPU2DisplayTask(void const *pvParameters);
static void AVIDecodeTask(void const *pvParameters);
static void MeasurementsTask(void const *pvParameters);

static uint32_t CPU2DrawingFPS = 0;
static int DualCoreOffload = 0;

static osThreadId hCPU2DisplayTask = NULL;
static osThreadId hMeasurementsTask = NULL;

static osSemaphoreId DecoderSemaphore = NULL;
static osSemaphoreId CPU2DisplaySem = NULL;
static osMessageQId CPU2DisplayEvent = 0;

#define MEASUREMENT_THREAD_STACK_SIZE     (4 * configMINIMAL_STACK_SIZE)
#define MEASUREMENT_THREAD_PRIORITY       osPriorityRealtime

#define CPU2_DISPLAY_THREAD_STACK_SIZE    (8 * configMINIMAL_STACK_SIZE)
#define CPU2_DISPLAY_THREAD_PRIORITY_H    osPriorityHigh
#define CPU2_DISPLAY_THREAD_PRIORITY_L    osPriorityNormal

#define DECODER_THREAD_STACK_SIZE         (8 * configMINIMAL_STACK_SIZE)
#define DECODER_THREAD_PRIORITY           osPriorityAboveNormal

struct AVIDecoderContext {
    FIL file;
    osThreadDef_t TaskDef;
    TaskHandle_t hTask;
    GUI_AVI_HANDLE havi;
    GUI_BITMAP renderTarget;
    WM_HWIN hWindow;
    uint32_t fps;
    osMessageQDef_t DisplayEventDef;
    osMessageQId DisplayEvent;
};

#if defined ( __ICCARM__ )
#pragma data_alignment=32
#pragma location="sdio_heap"
#else
__attribute__((section(".sdio_heap"))) __attribute__ ((aligned (32)))
#endif
static struct AVIDecoderContext AVIContext[VIDEO_COUNT];

static LCD_LOGPALETTE Fractalclut8 = {
    256,
    0,
    (LCD_COLOR const*)clut8
};

static GUI_BITMAP pFractalBM = {
    FRACTAL_WIDHT,
    FRACTAL_HEIGHT,
    FRACTAL_WIDHT,
    8,
    NULL, /* static bitmap buffer */
    &Fractalclut8 /* palette */
};

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    { WINDOW_CreateIndirect, "Window", ID_WINDOW_0, 0, 0, 800, 480, 0, 0x0, 0 },
    { TEXT_CreateIndirect, "FractalTitle", ID_TEXT_FRACTAL, 605, 100, 190, 20, (TEXT_CF_HCENTER | TEXT_CF_VCENTER), 0x64, 0 },
    { IMAGE_CreateIndirect, "FractalWindow", ID_IMAGE_0, 620, 130, FRACTAL_WIDHT, FRACTAL_HEIGHT, 0, 0x0, 0 },
    { TEXT_CreateIndirect, "fps0", ID_TEXT_FPS_0, 620, 255, 100, 16, (GUI_TA_LEFT | GUI_TA_TOP), 0x64, 0 },
    { IMAGE_CreateIndirect, "VideoWindow1", ID_IMAGE_1, 20, 30, VIDEO_WIDHT, VIDEO_HEIGHT, 0, 0x0, 4 },
    { TEXT_CreateIndirect, "fps1", ID_TEXT_FPS_1, 22, 205, 100, 16, (GUI_TA_LEFT | GUI_TA_TOP), 0x64, 0 },
    { IMAGE_CreateIndirect, "VideoWindow2", ID_IMAGE_2, 20, 250, VIDEO_WIDHT, VIDEO_HEIGHT, 0, 0x0, 4 },
    { TEXT_CreateIndirect, "fps2", ID_TEXT_FPS_2, 22, 425, 100, 16, (GUI_TA_LEFT | GUI_TA_TOP), 0x64, 0 },
    { IMAGE_CreateIndirect, "VideoWindow3", ID_IMAGE_3, 330, 30, VIDEO_WIDHT, VIDEO_HEIGHT, 0, 0x0, 4 },
    { TEXT_CreateIndirect, "fps3", ID_TEXT_FPS_3, 332, 205, 100, 16, (GUI_TA_LEFT | GUI_TA_TOP), 0x64, 0 },
    { IMAGE_CreateIndirect, "VideoWindow4", ID_IMAGE_4, 330, 250, VIDEO_WIDHT, VIDEO_HEIGHT, 0, 0x0, 4 },
    { TEXT_CreateIndirect, "fps4", ID_TEXT_FPS_4, 332, 425, 100, 16, (GUI_TA_LEFT | GUI_TA_TOP), 0x64, 0 },
    { TEXT_CreateIndirect, "frq_cm4", ID_TEXT_FRQ_CM4, 640, 360, 140, 20, (GUI_TA_LEFT | GUI_TA_TOP), 0x64, 0 },
    { TEXT_CreateIndirect, "frq_cm7", ID_TEXT_FRQ_CM7, 640, 380, 140, 20, (GUI_TA_LEFT | GUI_TA_TOP), 0x64, 0 },
    { TEXT_CreateIndirect, "CPU", ID_TEXT_CPU_LOAD, 590, 450, 200, 20, (TEXT_CF_TOP | TEXT_CF_RIGHT), 0x64, 0 },
    { BUTTON_CreateIndirect, "CM4", ID_BUTTON_CM4, 660, 280, 77, 77, 0, 0x0, 0 },
    { BUTTON_CreateIndirect, "Back", ID_BUTTON_MENU, 740, 5, 55, 70, 0, 0x0, 0 },
};

static void ToggleDualCoreOffload(void)
{
  WM_HWIN hItem;

  if(DualCoreOffload < 0)
    return;

  osSemaphoreWait(CPU2DisplaySem, osWaitForever);
  DualCoreOffload = 1 - DualCoreOffload;
  CPU2DrawingFPS = 0;

  if(hWinDualCore)
  {
    hItem = WM_GetDialogItem(hWinDualCore, ID_TEXT_FRQ_CM4);
    if(DualCoreOffload)
    {
      TEXT_SetText(hItem, "CM4: 200Mhz");
    }
    else
    {
      TEXT_SetText(hItem, "CM4: OFF");
    }
  }
  if(DualCoreOffload)
  {
    osThreadSetPriority(hCPU2DisplayTask, CPU2_DISPLAY_THREAD_PRIORITY_H);
  }
  else
  {
    osThreadSetPriority(hCPU2DisplayTask, CPU2_DISPLAY_THREAD_PRIORITY_L);
  }
  osSemaphoreRelease(CPU2DisplaySem);
}

static void ShutdownThreads(void)
{
    struct ipc_msg msg;

    if(hMeasurementsTask)
    {
        if(osThreadTerminate(hMeasurementsTask) == osOK)
            hMeasurementsTask = NULL;
    }

    osSemaphoreWait(CPU2DisplaySem, osWaitForever);
    if(hCPU2DisplayTask)
    {
        if(osThreadTerminate(hCPU2DisplayTask) == osOK)
            hCPU2DisplayTask = NULL;
    }
    if(CPU2DisplayEvent)
    {
      osMessageDelete(CPU2DisplayEvent);
    }
    osSemaphoreRelease(CPU2DisplaySem);

    osSemaphoreWait(DecoderSemaphore, osWaitForever);
    /* Free AVI decoders contexts */
    for (int i = 0; i < VIDEO_COUNT; i++) {
        if(AVIContext[i].hTask)
        {
            if(osThreadTerminate(AVIContext[i].hTask) == osOK)
                AVIContext[i].hTask = NULL;
        }
        if (AVIContext[i].havi)
        {
            GUI_AVI_Delete2(AVIContext[i].havi);
            AVIContext[i].havi = 0;
        }
        if(AVIContext[i].DisplayEvent)
        {
          osMessageDelete(AVIContext[i].DisplayEvent);
          AVIContext[i].DisplayEvent = 0;
        }
        f_close(&AVIContext[i].file);
        memset(&AVIContext[i].file, 0, sizeof(AVIContext[i].file));
    }
    osSemaphoreRelease(DecoderSemaphore);

    if (DecoderSemaphore) {
        osSemaphoreDelete(DecoderSemaphore);
        DecoderSemaphore = NULL;
    }

    if (CPU2DisplaySem) {
        osSemaphoreDelete(CPU2DisplaySem);
        CPU2DisplaySem = NULL;
    }

    /* Unselect CM4 firmware */
    msg.m.command = IPC_CTRL_SELECT_FIRMWARE;
    msg.arg1 = 0;
    ipc_sendmsg(&msg, sizeof(msg));
}

/**
  * @brief  callback for Fractal Frame
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbFractalFrame(WM_MESSAGE * pMsg)
{
  switch (pMsg->MsgId)
  {
    case WM_POST_PAINT:
      if(CPU2DisplayEvent)
      {
        osMessagePut (CPU2DisplayEvent, 0, 0);
      }
      break;
    default:
      /* The original callback */
      IMAGE_Callback(pMsg);
      break;
  }
}

/**
  * @brief  Paints CM4 button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_cm4(BUTTON_Handle hObj)
{
  if(DualCoreOffload == -1)
  {
    GUI_DrawBitmap(&bmcm4_na, 6, 6);
  }
  else if (DualCoreOffload == 1)
  {
    GUI_DrawBitmap(&bmcm4_on, 6, 6);
  }
  else
  {
    GUI_DrawBitmap(&bmcm4_off, 6, 6);
  }
}

/**
  * @brief  callback for cm4 button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_cm4(WM_MESSAGE * pMsg)
{
  switch (pMsg->MsgId)
  {
    case WM_PAINT:
      _OnPaint_cm4(pMsg->hWin);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  callback for Video Frame
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbVideoFrame(WM_MESSAGE * pMsg)
{
  int i = -1;
  switch (pMsg->MsgId)
  {
    case WM_POST_PAINT:
      IMAGE_GetUserData(pMsg->hWin, &i, sizeof(i));
      if(AVIContext[i].DisplayEvent)
      {
        osMessagePut (AVIContext[i].DisplayEvent, i, 0);
      }
      break;
    default:
      /* The original callback */
      IMAGE_Callback(pMsg);
      break;
  }
}

/**
  * @brief  Paints menu button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_menu(BUTTON_Handle hObj)
{
  GUI_DrawBitmap(&bmmenu, 15, 30);
}

/**
  * @brief  callback for menu button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_menu(WM_MESSAGE * pMsg)
{
  switch (pMsg->MsgId)
  {
    case WM_PAINT:
      _OnPaint_menu(pMsg->hWin);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

static void DrawImageBorder(IMAGE_Handle hObj, uint32_t c, uint32_t w, uint32_t r)
{
  GUI_RECT Rect;

  WM_GetWindowRectEx(hObj, &Rect);
  GUI_SetColor(c);
  GUI_SetPenSize(w);
  for(int i = 0; i < w; i++)
  {
    Rect.x0--;
    Rect.y0--;
    Rect.x1++;
    Rect.y1++;
    GUI_DrawRoundedRect(Rect.x0, Rect.y0, Rect.x1, Rect.y1, r);
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
      prev_msd_status = k_StorageGetStatus(storage_id);
      hStatusTimer = WM_CreateTimer(pMsg->hWin, ID_TIMER_0, 100, 0);
      break;
    case WM_TIMER:
      if(prev_msd_status != k_StorageGetStatus(storage_id))
      {
        prev_msd_status = k_StorageGetStatus(storage_id);
        if(prev_msd_status == 0)
        {
          GUI_EndDialog(hWinDualCore, 0);
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

/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg) {
    WM_HWIN hItem;
    int     NCode;
    int     Id;

    switch (pMsg->MsgId) {
    case WM_INIT_DIALOG:
        WM_CreateWindowAsChild(799, 479, 1, 1, pMsg->hWin, WM_CF_SHOW, _cbMediaConnection , 0);

        WINDOW_SetBkColor(pMsg->hWin, GUI_TRANSPARENT);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_FRACTAL);
        TEXT_SetTextColor(hItem, GUI_WHITE);
        TEXT_SetText(hItem, "Fractal Computation");
        TEXT_SetFont(hItem, &GUI_FontRounded22);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_0);
        IMAGE_SetBitmap(hItem, &pFractalBM);
        WM_SetCallback(hItem, _cbFractalFrame);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_FPS_0);
        TEXT_SetTextColor(hItem, GUI_WHITE);
        TEXT_SetText(hItem, "-- fps");
        TEXT_SetFont(hItem, GUI_FONT_13B_1);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_FPS_1);
        TEXT_SetTextColor(hItem, GUI_WHITE);
        TEXT_SetText(hItem, "-- fps");
        TEXT_SetFont(hItem, GUI_FONT_13B_1);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_FPS_2);
        TEXT_SetTextColor(hItem, GUI_WHITE);
        TEXT_SetText(hItem, "-- fps");
        TEXT_SetFont(hItem, GUI_FONT_13B_1);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_FPS_3);
        TEXT_SetTextColor(hItem, GUI_WHITE);
        TEXT_SetText(hItem, "-- fps");
        TEXT_SetFont(hItem, GUI_FONT_13B_1);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_FPS_4);
        TEXT_SetTextColor(hItem, GUI_WHITE);
        TEXT_SetText(hItem, "-- fps");
        TEXT_SetFont(hItem, GUI_FONT_13B_1);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_FRQ_CM4);
        TEXT_SetTextColor(hItem, GUI_WHITE);
        TEXT_SetText(hItem, "CM4: 200Mhz");
        TEXT_SetFont(hItem, &GUI_FontRounded22);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_FRQ_CM7);
        TEXT_SetTextColor(hItem, GUI_WHITE);
        TEXT_SetText(hItem, "CM7: 400Mhz");
        TEXT_SetFont(hItem, &GUI_FontRounded22);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_CPU_LOAD);
        TEXT_SetTextColor(hItem, GUI_WHITE);
        TEXT_SetText(hItem, "CM7 CPU load: 0%");
        TEXT_SetFont(hItem, &GUI_FontRounded22);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_MENU);
        WM_SetCallback(hItem, _cbButton_menu);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_CM4);
        WM_SetCallback(hItem, _cbButton_cm4);

        break;

    case WM_PAINT:
        GUI_DrawBitmap(&bmbkc_16bit, 0, 0);
        for(Id = ID_IMAGE_0; Id < ID_IMAGE_5; Id++)
        {
            hItem = WM_GetDialogItem(pMsg->hWin, Id);
            DrawImageBorder(hItem, GUI_WHITE, 3, 5);
        }
    break;

    case WM_NOTIFY_PARENT:
        Id    = WM_GetId(pMsg->hWinSrc);
        NCode = pMsg->Data.v;
        switch(Id) {
        case ID_BUTTON_MENU: /* Window close button */
            switch(NCode) {
            case WM_NOTIFICATION_CLICKED:
                break;
            case WM_NOTIFICATION_RELEASED:
                  GUI_EndDialog(pMsg->hWin, 0);
                break;
            }
            break;
        case ID_BUTTON_CM4:
            switch(NCode) {
            case WM_NOTIFICATION_CLICKED:
                break;
            case WM_NOTIFICATION_RELEASED:
                ToggleDualCoreOffload();
                break;
            case WM_NOTIFICATION_VALUE_CHANGED:
                break;
            }
            break;
        }
        break;
    case WM_DELETE:
        ShutdownThreads();
        break;
    default:
        WM_DefaultProc(pMsg);
        break;
    }
}

static void InitClut8Table()
{
    uint32_t red = 0, green = 0, blue = 0;
    uint32_t i = 0x00;

    for (i = 0; i < 256; i++) {
        red = (i * 8) % 256;
        green = (i * 6) % 256;
        blue = (i * 4) % 256;

        red = red << 16;
        green = green << 8;

        clut8[i] = 0xFF000000 | (red + green + blue);
    }
}

#define ITERATION       ((uint32_t)256)
#define REAL_CONSTANT   (0.285f)
#define IMG_CONSTANT    (0.01f)

static void render_julia(uint16_t offset_x, uint16_t offset_y, uint16_t zoom)
{
    float tmp1, tmp2;
    float num_real, num_img;
    float radius;
    uint8_t i;
    uint16_t x, y;

    for (y = 0; y < FRACTAL_HEIGHT; y++) {
        for (x = 0; x < FRACTAL_WIDHT; x++) {
            num_real = y - offset_y;
            num_real = num_real / zoom;
            num_img = x - offset_x;
            num_img = num_img / zoom;
            i = 0;
            radius = 0;

            while ((i < ITERATION - 1) && (radius < 4)) {
                tmp1 = num_real * num_real;
                tmp2 = num_img * num_img;
                num_img = 2 * num_real * num_img + IMG_CONSTANT;
                num_real = tmp1 - tmp2 + REAL_CONSTANT;
                radius = tmp1 + tmp2;
                i++;
            }

            Fractalbuffer[x + y * FRACTAL_WIDHT] = i;
        }
    }

    /* Invalidate Data Cache */
    SCB_CleanDCache_by_Addr((uint32_t *)Fractalbuffer, sizeof(Fractalbuffer));

    /* Address of the local buffer (in AXI SRAM) where the julia fractal will be rendered */
    pFractalBM.pData = (const U8*)Fractalbuffer;
}

static void render_frame(uint16_t offset_x, uint16_t offset_y, uint16_t zoom)
{
    struct ipc_msg msg;
    size_t len;

    /* Render a 160x120 julia fractal on the CM4 core */
    msg.m.command = IPC_COMMAND_RENDER_FRAME;
    msg.arg1 = offset_x;
    msg.arg2 = offset_y;
    msg.arg3 = zoom;

    len = ipc_sendmsg(&msg, sizeof(msg));
    configASSERT(len == sizeof(msg));
}

static int wait_completion(uint32_t timeout)
{
    struct ipc_msg msg = { 0, };
    size_t len;
    uint32_t StartTime = osKernelSysTick();

    do {
        len = ipc_recvmsg(&msg, sizeof(msg), timeout);
        if(len != sizeof(msg))
            return 1;
        if(osKernelSysTick() > (StartTime + timeout))
            return 2;
    } while (msg.m.state != IPC_COMMAND_FRAME_ACK);

    /* Address of the remote buffer (in SRAM1) where the julia fractal has been rendered */
    pFractalBM.pData = (const U8*)msg.arg1;
    return 0;
}

static void Startup(WM_HWIN hWin, U16 xpos, U16 ypos)
{
    toggle_chromART = 1;
    DualCoreOffload = 0;
    char filename[64] = { 0 };
    char taskname[16] = { 0 };

    if(Storage_GetStatus(storage_id) == 0)
    {
      storage_id = MSD_DISK_UNIT;
      if(Storage_GetStatus(storage_id) == 0)
        return;
    }

    /* Initialize CLUT8 table */
    InitClut8Table();

    /* Address of the local buffer (in AXI SRAM) where the julia fractal will be rendered */
    pFractalBM.pData = (const U8*)Fractalbuffer;

    /* Create main window */
    hWinDualCore = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, hWin, xpos, ypos);

    osSemaphoreDef(CPU2Display_SEM);
    CPU2DisplaySem = osSemaphoreCreate(osSemaphore(CPU2Display_SEM), 1);

    /* Semaphore for serializing access to the HWJPEG decoder IP */
    osSemaphoreDef(DECODERS_SEM);
    DecoderSemaphore = osSemaphoreCreate(osSemaphore(DECODERS_SEM), 1);
    configASSERT(DecoderSemaphore);

    /* Clear MJPEG decoding buffers */
    memset(MJPEGDecodeBuffer, 0, sizeof(MJPEGDecodeBuffer));

    /* Create CPU2DisplayEvent */
    osMessageQDef(osCPU2Displayqueue, 8, uint8_t);
    CPU2DisplayEvent = osMessageCreate (osMessageQ(osCPU2Displayqueue), NULL);
    configASSERT(CPU2DisplayEvent);

    /* CPU2DisplayTask is for CM7/CM4 IPC */
    osThreadDef(CPU2Display_Thread, CPU2DisplayTask, CPU2_DISPLAY_THREAD_PRIORITY_L, 0, CPU2_DISPLAY_THREAD_STACK_SIZE);
    hCPU2DisplayTask = osThreadCreate (osThread(CPU2Display_Thread), NULL);
    configASSERT(hCPU2DisplayTask);

    for (int i = 0; i < VIDEO_COUNT; i++) {
        snprintf(filename, sizeof(filename), "%sVideo/video%d_%d_%d.avi", Storage_GetDrive(storage_id), i, VIDEO_WIDHT, VIDEO_HEIGHT);
        snprintf(taskname, sizeof(taskname), "Video%d", i);
        FRESULT ret = f_open(&AVIContext[i].file, filename,
                             FA_READ | FA_OPEN_EXISTING);
        if (ret == FR_OK) {
            AVIContext[i].hWindow = WM_GetDialogItem(hWinDualCore, ID_IMAGE_1 + i);

            AVIContext[i].renderTarget.XSize = VIDEO_WIDHT;
            AVIContext[i].renderTarget.YSize = VIDEO_HEIGHT;
            AVIContext[i].renderTarget.BitsPerPixel = VIDEO_BPP;
            AVIContext[i].renderTarget.BytesPerLine = VIDEO_WIDHT * (VIDEO_BPP / 8);
            AVIContext[i].renderTarget.pData = (const U8*)MJPEGDecodeBuffer[i];
            AVIContext[i].renderTarget.pPal = NULL;
            AVIContext[i].renderTarget.pMethods = GUI_DRAW_BMPM565;
            AVIContext[i].fps = 0;

            /* Associate renderTarget bitmap to the IMAGE widget */
            IMAGE_SetBitmap(AVIContext[i].hWindow, &AVIContext[i].renderTarget);
            IMAGE_SetUserData(AVIContext[i].hWindow, &i, sizeof(i));
            WM_SetCallback(AVIContext[i].hWindow, _cbVideoFrame);

            /* Create Display Event */
            AVIContext[i].DisplayEventDef.queue_sz = 2;
            AVIContext[i].DisplayEventDef.item_sz = sizeof(uint8_t);
            AVIContext[i].DisplayEvent = osMessageCreate (&AVIContext[i].DisplayEventDef, NULL);
            configASSERT(AVIContext[i].DisplayEvent);

            /* Create one decoding thread per MJPEG bitstream */
            AVIContext[i].TaskDef.name      = taskname;
            AVIContext[i].TaskDef.pthread   = AVIDecodeTask;
            AVIContext[i].TaskDef.tpriority = DECODER_THREAD_PRIORITY;
            AVIContext[i].TaskDef.instances = 0;
            AVIContext[i].TaskDef.stacksize = DECODER_THREAD_STACK_SIZE;

            AVIContext[i].hTask = osThreadCreate (&AVIContext[i].TaskDef, &AVIContext[i]);
            configASSERT(AVIContext[i].hTask);
        }
    }

    /* Offload Fractal task to the CPU2 */
    ToggleDualCoreOffload();

    /* Measurement Task */
    osThreadDef(Measurement_Thread, MeasurementsTask, MEASUREMENT_THREAD_PRIORITY, 0, MEASUREMENT_THREAD_STACK_SIZE);
    hMeasurementsTask = osThreadCreate (osThread(Measurement_Thread), NULL);
    configASSERT(hMeasurementsTask);
}

static void CPU2DisplayTask(void const *pvParameters)
{
    struct ipc_msg msg;
    int32_t ret = 0;
    uint32_t StartTime, EndTime;
    int32_t delay_ms;
    WM_HWIN hItem;

    /* Set the fractal firmware as current firmware */
    msg.m.command = IPC_CTRL_SELECT_FIRMWARE;
    msg.arg1 = IPC_FRACTAL_FIRMWARE_ID;

    if ((DualCoreEnabled == 0) || (ipc_sendmsg(&msg, sizeof(msg)) == 0))
    {
      DualCoreOffload = -1;
      osThreadSetPriority(hCPU2DisplayTask, CPU2_DISPLAY_THREAD_PRIORITY_L);
      if(hWinDualCore)
      {
        hItem = WM_GetDialogItem(hWinDualCore, ID_TEXT_FRQ_CM4);
        TEXT_SetText(hItem, "CM4: N/A");
      }
    }
    else
    {
      do {
          /* Wait for IPC_CTRL_FIRMWARE_STATUS ACK */
          ret = ipc_recvmsg(&msg, sizeof(msg), 500);
          configASSERT(ret == 0 || msg.m.state == IPC_CTRL_FIRMWARE_STATUS);
      } while (ret == 0);
    }

    int posx = 80;
    int posy = 60;
    int factor = 100;
    int directection = 0;

    while (true)
    {
        ret = osSemaphoreWait(CPU2DisplaySem, osWaitForever);
        configASSERT(ret == osOK);

        StartTime = osKernelSysTick();

        if (DualCoreOffload == 1) {
            /* Render a frame on CM4 and wait for its availability */
            render_frame(posx, posy, factor);
            /* Wait for completion notification */
            if(wait_completion(1000))
               continue;
        } else {
            /* Render a frame on the master CM7 core */
            render_julia(posx, posy, factor);
        }

        if (directection == 0) {
            if (factor < 1000)
                factor += 20;
            else
                directection = 1;
        }

        if (directection == 1) {
            if (factor > 100)
                factor -= 20;
            else {
                directection = 0;
                posx = (rand() % FRACTAL_WIDHT);
                posy = (rand() % FRACTAL_HEIGHT);
            }
        }

        ret = osSemaphoreRelease(CPU2DisplaySem);
        configASSERT(ret == osOK);

        if (hWinDualCore)
        {
          hItem = WM_GetDialogItem(hWinDualCore, ID_IMAGE_0);
          WM_InvalidateWindow(hItem);
        }
        
        if(CPU2DisplayEvent)
        {
          osEvent event = osMessageGet( CPU2DisplayEvent, osWaitForever );
          if( event.status != osEventMessage )
          {
            /* Just ignore this frame */
            continue;
          }
        }

        CPU2DrawingFPS++;
        EndTime = osKernelSysTick();

        if (DualCoreOffload == 1)
          delay_ms = 66 - (EndTime - StartTime); /* target a max 15 FPS */
        else
          delay_ms = 100 - (EndTime - StartTime); /* target a max 10 FPS */

        if (delay_ms > 0)
          osDelay(delay_ms);
    }
}

void prvAVINotify(GUI_HMEM hMem, int Notification, U32 CurrentFrame, void* pUserData)
{
    switch (Notification) {
    case GUI_AVI_NOTIFICATION_START:
        break;
    case GUI_AVI_NOTIFICATION_PREDRAW:
        break;
    case GUI_AVI_NOTIFICATION_POSTDRAW:
        break;
    case GUI_AVI_NOTIFICATION_EOF:
        break;
    }
}

static int prvAVIGetData(void *p, const U8** ppData, unsigned NumBytesReq, U32 offset)
{
    uint32_t num_bytes;
    FRESULT res;

    if (offset != 0xFFFFFFFF) {
        /* Set file pointer to the required position */
        f_lseek((FIL *)p, offset);
    }

    /* Read data into buffer */
    res = f_read((FIL *)p, (void*)*ppData, NumBytesReq, (UINT *)&num_bytes);

    if (res != FR_OK)
        num_bytes = 0;

    /* Return number of available bytes */
    return num_bytes;  
}

static void AVIDecodeTask(void const *pvParameters)
{
    GUI_AVI_INFO info;
    uint32_t size;
    int32_t ret = 0;
    uint32_t StartTime, EndTime;
    int32_t delay_ms;
    osEvent DisplayEvent;

    struct AVIDecoderContext *context = (struct AVIDecoderContext*)pvParameters;

    /* Synchronized access to the shared video buffer (pavi->pVideoBuffer) */
    osSemaphoreWait(DecoderSemaphore, osWaitForever);

    context->havi = GUI_AVI_CreateEx2(prvAVIGetData, prvAVINotify, true, context);

    GUI_AVI_SetDevFunc2(context->havi, GUI_AVI_JPEG_INIT, (void*)HW_JPEG_Init2);
    GUI_AVI_SetDevFunc2(context->havi, GUI_AVI_JPEG_DEINIT, (void*)HW_JPEG_DeInit2);
    GUI_AVI_SetDevFunc2(context->havi, GUI_AVI_JPEG_DRAW, (void*)HW_JPEG_Draw2);

    GUI_AVI_SelectMode2(context->havi, 1);

    /* Parse AVI container */
    GUI_AVI_SetBuffers2(context->havi, AVI_VideoBuffer, AVI_VIDEO_BUF_SIZE,
                       AVI_AudioBuffer, AUDIO_BUFFER_SIZE);

    size = f_size(&context->file);
    GUI_AVI_Start2(context->havi, (void*)&context->file, size, false);

    GUI_AVI_GetInfo2(context->havi, &info);
    
    osSemaphoreRelease(DecoderSemaphore);

    while (true)
    {
        /* Synchronized access to video buffers and HWJPEG decoder */
        osSemaphoreWait(DecoderSemaphore, osWaitForever);

        StartTime = osKernelSysTick();

        /* h/w accelerated decoding */
        ret = GUI_AVI_DecodeFrame(context->havi,
                                  (uint8_t*)context->renderTarget.pData,
                                  (uint16_t )context->renderTarget.XSize,
                                  (uint16_t )context->renderTarget.YSize,
                                  context->renderTarget.BytesPerLine);

        /* Release semaphore */
        osSemaphoreRelease(DecoderSemaphore);

        if (ret == 0)
        {
          if (context->hWindow)
          {
            WM_InvalidateWindow(context->hWindow);
            if (context->DisplayEvent)
            {
              DisplayEvent = osMessageGet( context->DisplayEvent, osWaitForever );
              if( DisplayEvent.status != osEventMessage )
              {
                /* Skip this frame */
                continue;
              }
            }
          }

          EndTime = osKernelSysTick();
          context->fps++;

          /* Presentation period */
          delay_ms = (info.msPerFrame - (EndTime - StartTime));
          if(delay_ms > 0)
            osDelay(delay_ms); /* Presentation period */
        }
    }
}


static void MeasurementsTask(void const *pvParameters)
{
  uint8_t i;
  char buf[32];
  WM_HWIN hItem;

  while (true)
  {
    if(hWinDualCore)
    {
      sprintf(buf , "%lu fps", CPU2DrawingFPS);
      CPU2DrawingFPS = 0;
      hItem = WM_GetDialogItem(hWinDualCore, ID_TEXT_FPS_0);
      TEXT_SetText(hItem, buf);

      sprintf(buf , "CM7 CPU load : %d%%", osGetCPUUsage());
      hItem = WM_GetDialogItem(hWinDualCore, ID_TEXT_CPU_LOAD);
      TEXT_SetText(hItem, buf);

      for(i = 0; i < VIDEO_COUNT; i++)
      {
        sprintf(buf , "%lu fps", AVIContext[i].fps);
        AVIContext[i].fps = 0;
        hItem = WM_GetDialogItem(hWinDualCore, ID_TEXT_FPS_1+i);
        TEXT_SetText(hItem, buf);
      }
    }
    osDelay(1000);
  }
}
