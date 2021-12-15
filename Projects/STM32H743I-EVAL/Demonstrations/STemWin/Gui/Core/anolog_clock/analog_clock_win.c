/**
  ******************************************************************************
  * @file    analog_clock_win.c
  * @author  MCD Application Team   
  * @brief   analog clcok functions
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
#include "DIALOG.h"
#include "k_module.h"
#include "k_rtc.h"
#include "LISTWHEEL.h"

/* Weather generated resources */
#include "generated/weather_cloud_rain_res.c"
#include "generated/weather_cloud_res.c"
#include "generated/weather_cloud_sun_res.c"
#include "generated/weather_cloud_thunder_res.c"
#include "generated/weather_cloud_thunder_sun_res.c"
#include "generated/weather_shape_res.c"
#include "generated/weather_sun_res.c"
#include "generated/analog_clock_res.c"
#include "generated/controls/settings_button_res.c"

/** @addtogroup ANALOG_CLOCK_MODULE
  * @{
  */

/** @defgroup ANALOG_CLOCK
  * @brief analog clock routines 
  * @{
  */


/* External variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void Startup(WM_HWIN hWin, uint16_t xpos, uint16_t ypos);
        int GetNumberOfDays(char * aMonth, int aYear);

/* Private typedef -----------------------------------------------------------*/
K_ModuleItem_Typedef  analog_clock_board =
{
  0,
  "Clock and Date",
  0,
  0,
  Startup,
  NULL,
};

/* Wheel window define */
typedef struct 
{
  WM_HWIN hWin;
} WHEEL;

/* Private defines -----------------------------------------------------------*/
#define ID_BUTTON_EXIT          (GUI_ID_USER + 0x00)
#define ID_BUTTON_SETTINGS      (GUI_ID_USER + 0x01)
#define ID_BUTTON_REFRESH       (GUI_ID_USER + 0x02)
#define ID_BUTTON_NEXT          (GUI_ID_USER + 0x03)
#define ID_BUTTON_PREVIOUS      (GUI_ID_USER + 0x04)
#define ID_BUTTON_CLOCK1        (GUI_ID_USER + 0x05)
#define ID_BUTTON_CLOCK2        (GUI_ID_USER + 0x06)
#define ID_BUTTON_CLOCK3        (GUI_ID_USER + 0x07)
#define ID_CLOCK_CHOOSE         (GUI_ID_USER + 0x08)
#define ID_DATE_CHOOSE          (GUI_ID_USER + 0x09)
#define ID_BUTTON_MINUS         (GUI_ID_USER + 0x0A)
#define ID_BUTTON_PLUS          (GUI_ID_USER + 0x0B)
#define ID_BUTTON_HOURS         (GUI_ID_USER + 0x0C)
#define ID_BUTTON_MINUTES       (GUI_ID_USER + 0x0D)

#define  AA_FACTOR  3
#define  PI         3.14

#define TRANS0    0xEE000000
#define TRANS1    0x11000000
#define TRANS2    0x00000000

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern GUI_CONST_STORAGE GUI_BITMAP bmmenu;
static WM_HWIN  hclockmain, hChoose_Clock, hTime_Clock, hDate_Clock;

/* Setting the right hour/minutes */
uint8_t setHours_plus    = 0;
uint8_t setHours_minus   = 0;
uint8_t setMinutes_plus  = 0;
uint8_t setMinutes_minus = 0;
uint8_t clock_index      = 0;
uint32_t settings        = 0;
uint32_t day_listwheel   = 13;
uint32_t month_listwheel = 3;
uint32_t year_listwheel  = 27;  
uint32_t kabissa         = 0;
uint32_t day_nb          = 0; 

/* RTC defines */
RTC_TimeTypeDef Time;
RTC_DateTypeDef Date;

/* Timer declarations */
WM_HTIMER hTimerTime;
WM_HTIMER hTimerTime2;
  
/* Years interval to be used on the Demo [1990 - 2020] */
static char * _apYear[] = 
{
  "1990", "1991", "1992", "1993", "1994", "1995", "1996", "1997", "1998", "1999", "2000", "2001", "2002", "2003", "2004", "2005", "2006", "2007", "2008", "2009", "2010", "2011", "2012", "2013", "2014", "2015", "2016", "2017", "2018", "2019", "2020"
};

/* Months */
static char * _apMonth[] = 
{
  "January", "February", "March", "April", " May ", "June", "July", "August", "September", "October", "November", "December"
};

/* Days by months */
uint16_t _apDayMonth[] = 
{
  31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365
};

/* Days */
static char * _apDay[] = 
{
  "01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30", "31"
};

/* 3 Wheels: days, month and years */
static WHEEL _aWheel[3];

uint8_t strMonth[][12] = {"January",
                           "February",
                           "March",
                           "April",
                           "May",
                           "June",
                           "July",
                           "August",
                           "September",
                           "October",
                           "November",
                           "December"};

/* Needles draws, clock1 */
static const GUI_POINT aPoints_Clock1[4][4] = 
{   
  /* Hour Needle */
  {{ 2, 0 * AA_FACTOR},
   { 2, -17 * AA_FACTOR},
   {-2, -17 * AA_FACTOR},
   {-2, 0 * AA_FACTOR}},  
  
  /* Min Needle */
  {{ 1, 0 * AA_FACTOR},
   { 1, -20 * AA_FACTOR},
   {-1, -20 * AA_FACTOR},
   {-1, 0 * AA_FACTOR}},
   
   /* Sec Needle */
  {{1, 1 * AA_FACTOR},
   {-1, 1 * AA_FACTOR},
   {-1,-88 * AA_FACTOR},
   {1,-88 * AA_FACTOR}},
   
  {{2, -1 * AA_FACTOR},
   {-2, -1 * AA_FACTOR},
   {-2, 20 * AA_FACTOR},
   {2, 20 * AA_FACTOR}},   
};

/* Needles draws, clock2 */
static const GUI_POINT aPoints_Clock2[4][4] = {
   
  /* Hour Needle */
  {{ 1, 0 * AA_FACTOR},
   { 1, -10 * AA_FACTOR},
   {-1, -10 * AA_FACTOR},
   {-1, 0 * AA_FACTOR}}, 
  
  /* Min Needle */
  {{ 0, 0 * AA_FACTOR},
   { 0, -10 * AA_FACTOR},
   {-1, -10 * AA_FACTOR},
   {-1, 0 * AA_FACTOR}},
   
   /* Sec Needle */
  {{1, 1 * AA_FACTOR},
   {-1, 1 * AA_FACTOR},
   {-1,-50 * AA_FACTOR},
   {1,-50 * AA_FACTOR}},
   
  {{2, -1 * AA_FACTOR},
   {-2, -1 * AA_FACTOR},
   {-2, 20 * AA_FACTOR},
   {2, 20 * AA_FACTOR}},   
};

GUI_POINT aPointsDest_Clock1[4][4];
GUI_POINT aPointsDest_Clock2[4][4];
GUI_POINT aPointsDest_Clock3[4][4];

/* Private functions ---------------------------------------------------------*/
static int _CreateListWheel(int x, int y, int xSize, int ySize, int Id, char ** apText, int NumItems, int TextAlign, WM_HWIN hParent, WHEEL * pWheel); 

/**
  * @brief  Draw Needle
  * @param  index: Needle index  
  * @param  x0:    x position
  * @param  y0:    y position      
  * @retval None
  */
static void DrawNeedle(uint32_t index, uint16_t x0, uint16_t y0) 
{
  /* draw Needles */
  if(index >= 2)
  {
    GUI_SetColor(0xFF1A468C);
    GUI_AA_FillPolygon(aPointsDest_Clock1[index], 4, AA_FACTOR * x0, AA_FACTOR * y0);
  }
  else if(index == 1)
  {
    GUI_SetColor(0xFF1A468C);
    GUI_AA_FillPolygon(aPointsDest_Clock1[index], 4, AA_FACTOR * x0, AA_FACTOR * y0);
  }
  else
  {   
    GUI_SetColor(0xFF1A468C);
    GUI_AA_FillPolygon(aPointsDest_Clock1[index], 4, AA_FACTOR * x0, AA_FACTOR * y0);
  }  
}

/**
  * @brief  Get Nbre of days depending of the user choice
  * @param  aMonth: Month chossen by the user
  * @param  aYear : Year chosen by the user
  * @retval int
  */
int GetNumberOfDays(char * aMonth, int aYear)
{
  int numberOfDays = 0;

  if ((strcmp(aMonth, "January") == 0)  ||
		  (strcmp(aMonth, "March") == 0)   ||
		  (strcmp(aMonth, "May") == 0)   ||
		  (strcmp(aMonth, "July") == 0)    ||
		  (strcmp(aMonth, "August") == 0)  ||
		  (strcmp(aMonth, "October") == 0) ||
		  (strcmp(aMonth, "December") == 0))
  {
    numberOfDays = 31;
  }
  else if ((strcmp(aMonth, "April") == 0)     ||
		  (strcmp(aMonth, "June") == 0)      ||
		  (strcmp(aMonth, "September") == 0) ||
		  (strcmp(aMonth, "November") == 0))
  {
    numberOfDays = 30;
  }
  else if ((strcmp(aMonth, "February") == 0))
  {
    numberOfDays = 28;

    if (aYear % 400 == 0)
    {
      numberOfDays = numberOfDays + 1;
    }
    else if (aYear % 4 == 0)
    {
      numberOfDays = numberOfDays + 1;
    }
  }

  return numberOfDays;
}

/**
  * @brief  Draw Needle
  * @param  index: Needle index  
  * @param  x0:    x position
  * @param  y0:    y position      
  * @retval None
  */
static void DrawNeedle2(uint32_t index, uint16_t x0, uint16_t y0) 
{
  
  /* draw Needles */
  if(index >= 2)
  {
    GUI_SetColor(0xFF6E6E6F);
    GUI_AA_FillPolygon(aPointsDest_Clock2[index], 4, AA_FACTOR * x0, AA_FACTOR * y0);
  }
  else if(index == 1)
  {
    GUI_SetColor(0xFF606060);    
    GUI_AA_FillPolygon(aPointsDest_Clock2[index], 4, AA_FACTOR * x0, AA_FACTOR * y0);
  }
  else
  {
    GUI_SetColor(0xFF555555);
    GUI_AA_FillPolygon(aPointsDest_Clock2[index], 4, AA_FACTOR * x0, AA_FACTOR * y0);
  }  
}

/**
  * @brief  Update clock
  * @param  x0:   x position
  * @param  y0:   y position
  * @param  hour: updated hour
  * @param  min:  updated minute
  * @param  sec:  updated second      
  * @retval None
  */
static void GUI_UpdateClock (uint16_t x0, uint16_t y0, uint8_t hour, uint8_t min, uint8_t sec)
{ 
  GUI_AA_EnableHiRes();
  GUI_AA_SetFactor(AA_FACTOR);
  
  /* Clock1 */
  GUI_MagnifyPolygon(aPointsDest_Clock1[0], aPoints_Clock1[0], 4, 4);
  GUI_RotatePolygon(aPointsDest_Clock1[0], aPointsDest_Clock1[0], 4, - 2 * PI * (float)((float)hour + (float)min /60) / 12);    
  DrawNeedle(0, x0, y0);
  
  GUI_MagnifyPolygon(aPointsDest_Clock1[1], aPoints_Clock1[1], 4, 4);
  GUI_RotatePolygon(aPointsDest_Clock1[1], aPointsDest_Clock1[1], 4, - 2 * PI * (float)((float)min + (float)sec / 60) / 60);
  DrawNeedle(1, x0, y0);  
  
  GUI_MagnifyPolygon(aPointsDest_Clock1[2], aPoints_Clock1[2], 4, 1);
  GUI_RotatePolygon(aPointsDest_Clock1[2], aPointsDest_Clock1[2], 4, - 2 * PI * sec / 60);    
  DrawNeedle(2, x0, y0); 
  
  GUI_MagnifyPolygon(aPointsDest_Clock1[3], aPoints_Clock1[3], 4, 1);
  GUI_RotatePolygon(aPointsDest_Clock1[3], aPointsDest_Clock1[3], 4, - 2 * PI * sec / 60);    
  DrawNeedle(3, x0, y0);

  GUI_AA_DisableHiRes();
}

/**
  * @brief  Update clock
  * @param  x0:   x position
  * @param  y0:   y position
  * @param  hour: updated hour
  * @param  min:  updated minute
  * @param  sec:  updated second      
  * @retval None
  */
static void GUI_UpdateClock2 (uint16_t x0, uint16_t y0, uint8_t hour, uint8_t min, uint8_t sec)
{ 
  GUI_AA_EnableHiRes();
  GUI_AA_SetFactor(AA_FACTOR);
  
  /* Clock2 */
  GUI_MagnifyPolygon(aPointsDest_Clock2[0], aPoints_Clock2[0], 4, 4);
  GUI_RotatePolygon(aPointsDest_Clock2[0], aPointsDest_Clock2[0], 4, - 2 * PI * (float)((float)hour + (float)min /60) / 12);    
  DrawNeedle2(0, x0, y0);
  
  GUI_MagnifyPolygon(aPointsDest_Clock2[1], aPoints_Clock2[1], 4, 4);
  GUI_RotatePolygon(aPointsDest_Clock2[1], aPointsDest_Clock2[1], 4, - 2 * PI * (float)((float)min + (float)sec / 60) / 60);
  DrawNeedle2(1, x0, y0);  
  
  GUI_MagnifyPolygon(aPointsDest_Clock2[2], aPoints_Clock2[2], 4, 1);
  GUI_RotatePolygon(aPointsDest_Clock2[2], aPointsDest_Clock2[2], 4, - 2 * PI * sec / 60);    
  DrawNeedle2(2, x0, y0); 
  
  GUI_MagnifyPolygon(aPointsDest_Clock2[3], aPoints_Clock2[3], 4, 1);
  GUI_RotatePolygon(aPointsDest_Clock2[3], aPointsDest_Clock2[3], 4, - 2 * PI * sec / 60);    
  DrawNeedle2(3, x0, y0); 

  GUI_AA_DisableHiRes();
}

/**
* @brief  Paints exit button
* @param  hObj: button handle
* @retval None
*/
static void _OnPaint_exit(BUTTON_Handle hObj)
{
  GUI_DrawBitmap(&bmmenu, 13, 17);
}

/**
* @brief  callback for exit button
* @param  pMsg: pointer to data structure of type WM_MESSAGE
* @retval None
*/
static void _cbButton_exit(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
  case WM_PAINT:
    _OnPaint_exit(pMsg->hWin);
    break;
  default:
    /* The original callback */
    BUTTON_Callback(pMsg);
    break;
  }
}

/**
* @brief  Paints settings button
* @param  hObj: button handle
* @retval None
*/
static void _OnPaint_settings(BUTTON_Handle hObj)
{
  GUI_DrawBitmap(&bmsettings_button_res, 0, 0);
}

/**
* @brief  callback for settings button
* @param  pMsg: pointer to data structure of type WM_MESSAGE
* @retval None
*/
static void _cbButton_settings(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
  case WM_PAINT:
    _OnPaint_settings(pMsg->hWin);
    break;
  default:
    /* The original callback */
    BUTTON_Callback(pMsg);
    break;
  }
}

/**
* @brief  Paints next button
* @param  hObj: button handle
* @retval None
*/
static void _OnPaint_next(BUTTON_Handle hObj)
{
  GUI_DrawBitmap(&bmclocknext, 0, 0);
}

/**
* @brief  callback for next button
* @param  pMsg: pointer to data structure of type WM_MESSAGE
* @retval None
*/
static void _cbButton_next(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
  case WM_PAINT:
    _OnPaint_next(pMsg->hWin);
    break;
  default:
    /* The original callback */
    BUTTON_Callback(pMsg);
    break;
  }
}

/**
* @brief  Paints previous button
* @param  hObj: button handle
* @retval None
*/
static void _OnPaint_previous(BUTTON_Handle hObj)
{
  GUI_DrawBitmap(&bmclockprevious, 26, 10);
}

/**
* @brief  callback for previous button
* @param  pMsg: pointer to data structure of type WM_MESSAGE
* @retval None
*/
static void _cbButton_previous(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
  case WM_PAINT:
    _OnPaint_previous(pMsg->hWin);
    break;
  default:
    /* The original callback */
    BUTTON_Callback(pMsg);
    break;
  }
}

/**
* @brief  Paints back button
* @param  hObj: button handle
* @retval None
*/
static void _OnPaint_backclock(BUTTON_Handle hObj)
{
}

/**
* @brief  callback for previous button
* @param  pMsg: pointer to data structure of type WM_MESSAGE
* @retval None
*/
static void _cbButton_backclock(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
  case WM_PAINT:
    _OnPaint_backclock(pMsg->hWin);
    break;
  default:
    /* The original callback */
    BUTTON_Callback(pMsg);
    break;
  }
}

/**
* @brief  Paints minus button
* @param  hObj: button handle
* @retval None
*/
static void _OnPaint_minus(BUTTON_Handle hObj)
{
  GUI_DrawBitmap(&bmminus, 15, 15);
}

/**
* @brief  callback for minus button
* @param  pMsg: pointer to data structure of type WM_MESSAGE
* @retval None
*/
static void _cbButton_minus(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
  case WM_PAINT:
    _OnPaint_minus(pMsg->hWin);
    break;
  default:
    /* The original callback */
    BUTTON_Callback(pMsg);
    break;
  }
}

/**
* @brief  Paints plus button
* @param  hObj: button handle
* @retval None
*/
static void _OnPaint_plus(BUTTON_Handle hObj)
{
  GUI_DrawBitmap(&bmplus, 15, 15);
}

/**
* @brief  callback for plus button
* @param  pMsg: pointer to data structure of type WM_MESSAGE
* @retval None
*/
static void _cbButton_plus(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
  case WM_PAINT:
    _OnPaint_plus(pMsg->hWin);
    break;
  default:
    /* The original callback */
    BUTTON_Callback(pMsg);
    break;
  }
}

/**
* @brief  Paints refresh button
* @param  hObj: button handle
* @retval None
*/
static void _OnPaint_refresh(BUTTON_Handle hObj)
{  
  if(setHours_plus == 1)
  {
    if(Time.Hours < 11)
    {
      Time.Hours++;
    }
    else
    {
      Time.Hours = 1;
    }
  }
  
  if(setMinutes_minus == 1)
  {
    if(Time.Minutes > 0)
    {
      Time.Minutes--;
    }
    else
    {
      Time.Hours--;
      Time.Minutes = 59;
    }
  }
  
  if(setMinutes_plus == 1)
  {
    if(Time.Minutes < 59)
    {
      Time.Minutes++;
    }
    else
    {
      Time.Hours++;
      Time.Minutes = 0;
    }
  }
  
  if(setHours_minus == 1)
  {
    if(Time.Hours > 0)
    {
      Time.Hours--;
    }
    else
    {
      Time.Hours = 11;
    }
  }
}

/**
* @brief  callback for refresh button
* @param  pMsg: pointer to data structure of type WM_MESSAGE
* @retval None
*/
static void _cbButton_refresh(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
  case WM_PAINT:
    _OnPaint_refresh(pMsg->hWin);
    break;
  default:
    /* The original callback */
    BUTTON_Callback(pMsg);
    break;
  }
}

/**
* @brief  Paints hours button
* @param  hObj: button handle
* @retval None
*/
static void _OnPaint_hours(BUTTON_Handle hObj)
{
  if(settings == 2)
  {
    GUI_DrawBitmap(&bmhours_pressed, 0, 0);
  }
  else
  { 
    GUI_DrawBitmap(&bmhours, 0, 0);
  }
}

/**
* @brief  Paints minutes button
* @param  hObj: button handle
* @retval None
*/
static void _OnPaint_minutes(BUTTON_Handle hObj)
{
  if(settings == 1)
  {
    GUI_DrawBitmap(&bmminutes_pressed, 0, 0);
  }
  else
  { 
    GUI_DrawBitmap(&bmminutes, 0, 0);
  }
}

/**
* @brief  callback for hours button
* @param  pMsg: pointer to data structure of type WM_MESSAGE
* @retval None
*/
static void _cbButton_hours(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
  case WM_PAINT:
    _OnPaint_hours(pMsg->hWin);
    break;
  default:
    /* The original callback */
    BUTTON_Callback(pMsg);
    break;
  }
}

/**
* @brief  callback for minutes button
* @param  pMsg: pointer to data structure of type WM_MESSAGE
* @retval None
*/
static void _cbButton_minutes(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
  case WM_PAINT:
    _OnPaint_minutes(pMsg->hWin);
    break;
  default:
    /* The original callback */
    BUTTON_Callback(pMsg);
    break;
  }
}

/**
  * @brief  callback for clocks window
  * @param  pMsg: pointer to a data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbTimeClock(WM_MESSAGE * pMsg) 
{
  int Id, NCode;
  WM_HWIN  hItem;
  uint8_t min, hour;
  static uint8_t TempStr[50];
  GUI_RECT filterRect = {180, 100, 380, 380};
   
  switch (pMsg->MsgId) 
  {
    case WM_CREATE:
      hItem = BUTTON_CreateEx(580, 17, 50, 50, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_EXIT);
      WM_SetCallback(hItem, _cbButton_exit);
      
      hItem = BUTTON_CreateEx(538, 379, 102, 102, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_NEXT);
      WM_SetCallback(hItem, _cbButton_next);  
      
      hItem = BUTTON_CreateEx(25, 25, 50, 50, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_PREVIOUS);
      WM_SetCallback(hItem, _cbButton_previous);
      
      hItem = BUTTON_CreateEx(90, 195, 85, 85, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_MINUS);
      WM_SetCallback(hItem, _cbButton_minus); 
      
      hItem = BUTTON_CreateEx(465, 195, 85, 85, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_PLUS);
      WM_SetCallback(hItem, _cbButton_plus);
      
      hItem = BUTTON_CreateEx(255, 400, 50, 50, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_HOURS);
      WM_SetCallback(hItem, _cbButton_hours); 
      
      hItem = BUTTON_CreateEx(335, 400, 50, 50, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_MINUTES);
      WM_SetCallback(hItem, _cbButton_minutes); 
      
      hItem = BUTTON_CreateEx(639, 479, 1, 1, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_REFRESH);
      WM_SetCallback(hItem, _cbButton_refresh); 
      break;
    case WM_TIMER:
      if((setHours_plus != 0) || (setHours_minus != 0) || (setMinutes_plus != 0)  || (setMinutes_minus != 0) )
      {
        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_REFRESH);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem); 
      }
      
      /* Update the window rectangle to display the filtred content */
      WM_InvalidateRect(pMsg->hWin, &filterRect);    
      WM_RestartTimer(hTimerTime2, 100);
      break; 
      
    case WM_DELETE:
      if(hTimerTime2 != 0)
      {
        WM_DeleteTimer(hTimerTime2);
        hTimerTime2 = 0;
      }
      break;
    
    case WM_PAINT:
      GUI_DrawBitmap(background_clock[clock_index], 0, 0);
      
      hItem = TEXT_CreateEx(255, 30, 250, 28, pMsg->hWin, WM_CF_SHOW, 0, ID_CLOCK_CHOOSE, "SET TIME");
      TEXT_SetFont(hItem, GUI_FONT_32_1);
      TEXT_SetTextColor(hItem, GUI_WHITE);
      TEXT_SetText     (hItem, "SET TIME");
      
      if(clock_index == 0)
      {
        GUI_DrawBitmap(&bmClock, 180, 100);
        GUI_UpdateClock (320, 240, Time.Hours, Time.Minutes, Time.Seconds);
        GUI_SetColor(0xFF1A468C);
        GUI_AA_FillCircle(320, 240, 8);
      }
      else if(clock_index == 1)
      {
        GUI_DrawBitmap(&bmClock2_280, 180, 100);
        GUI_UpdateClock2 (320, 240, Time.Hours, Time.Minutes, Time.Seconds); 
        GUI_SetColor(0xFF555555);
        GUI_AA_FillCircle(320, 240, 5);
      }
      else
      {
        GUI_DrawBitmap(&bmClock3_280, 180, 100); 
        
        hour  =  Time.Hours;
        min   =  Time.Minutes;
        
        GUI_SetTextMode(GUI_TM_TRANS);
        GUI_SetFont(&GUI_Font32_ASCII);
        GUI_SetColor(GUI_WHITE);
        if(hour == 12) 
        {
          hour = 0;
        }
        sprintf((char *)TempStr, "%02d %02d", hour , min);
        GUI_DispStringAt((char *)TempStr, 280, 225);
      }
      break;
    case WM_NOTIFY_PARENT:
      Id    = WM_GetId(pMsg->hWinSrc);    /* Id of widget */
      NCode = pMsg->Data.v;               /* Notification code */
      switch (NCode) 
      {
        case WM_NOTIFICATION_CLICKED:
          if(Id == ID_BUTTON_EXIT)
          {  
            k_SetTime(&Time);
            k_SetDate(&Date);
            WM_HideWindow(pMsg->hWin);
            if(hTimerTime2 != 0)
            {
              WM_DeleteTimer(hTimerTime2);
              hTimerTime2 = 0;
            }
            hTimerTime = WM_CreateTimer(hclockmain, 0, 1000, 0);
          }
          if(Id == ID_BUTTON_HOURS)
          {
            settings = 2;
          }
          if(Id == ID_BUTTON_MINUTES)
          {      
            settings = 1;
          }
          if(Id == ID_BUTTON_NEXT)
          {
            k_SetTime(&Time);  
            k_SetDate(&Date);
            if(hTimerTime2 != 0)
            {
              WM_DeleteTimer(hTimerTime2); 
              hTimerTime2 = 0;
            }
            WM_HideWindow(pMsg->hWin);
            WM_ShowWindow(hDate_Clock);
          }
          if(Id == ID_BUTTON_PREVIOUS)
          {
            k_SetTime(&Time);  
            k_SetDate(&Date);
            if(hTimerTime2 != 0)
            {
              WM_DeleteTimer(hTimerTime2); 
              hTimerTime2 = 0;
            }
            WM_HideWindow(pMsg->hWin);
            WM_ShowWindow(hChoose_Clock);
          } 
          if(Id == ID_BUTTON_PLUS)
          {      
            if (settings == 2)
            {     
              setHours_plus = 1;
              if(Time.Hours < 11)
              {
                Time.Hours++;
              }
              else
              {
                Time.Hours = 0;
              }
            }
            else if (settings == 1)
            { 
              setMinutes_plus = 1;
              if(Time.Minutes < 59)
              {
                Time.Minutes++;
              }
              else
              {
                if(Time.Hours < 11)
                {
                  Time.Hours++;
                }
                else
                {
                  Time.Hours = 0;
                }
                Time.Minutes = 0;
              }
            } 
          }
          if(Id == ID_BUTTON_MINUS)
          {
            if (settings == 2)
            { 
              setHours_minus = 1;
              if(Time.Hours > 0)
              {
                Time.Hours--;
              }
              else
              {
                Time.Hours = 11;
              }
            }
            if (settings == 1)
            { 
              setMinutes_minus = 1;
              if(Time.Minutes > 0)
              {
                Time.Minutes--;
              }
              else
              {
                if(Time.Hours == 0)
                {
                  Time.Hours = 11;
                }
                else
                {
                  Time.Hours--;
                }
                Time.Minutes = 59;
              }
            } 
          }
          break;
      case WM_NOTIFICATION_RELEASED:
        if(Id == ID_BUTTON_PLUS)
        {
          if (settings == 2)
          {
            setHours_plus = 0;
          }
          else if (settings == 1)
          {
            setMinutes_plus = 0;
          }
        }
        if(Id == ID_BUTTON_MINUS)
        {
          if (settings == 2)
          {  
            setHours_minus = 0;           
          }
          if (settings == 1)
          { 
            setMinutes_minus = 0;
          }        
        } 
        break;
      }       
      break;
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/**
  * @brief  callback for clock choice window
  * @param  pMsg: pointer to a data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbChooseClock(WM_MESSAGE * pMsg) 
{
  int Id, NCode;
  WM_HWIN  hItem;
  uint8_t min, hour;
  static uint8_t TempStr[50];
    
  switch (pMsg->MsgId) 
  {
    case WM_CREATE:
      hItem = BUTTON_CreateEx(580, 17, 50, 50, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_EXIT);
      WM_SetCallback(hItem, _cbButton_exit);
      
      hItem = BUTTON_CreateEx(538, 379, 102, 102, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_NEXT);
      WM_SetCallback(hItem, _cbButton_next);  
      
      hItem = BUTTON_CreateEx(180, 100, 280, 280, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_CLOCK1);
      WM_SetCallback(hItem, _cbButton_backclock); 
      
      hItem = BUTTON_CreateEx(440, 140, 200, 200, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_CLOCK2);
      WM_SetCallback(hItem, _cbButton_backclock); 
      
      hItem = BUTTON_CreateEx(0, 140, 200, 200, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_CLOCK3);
      WM_SetCallback(hItem, _cbButton_backclock);
      break;
      
  case WM_PAINT:
    GUI_DrawBitmap(background_clock[clock_index], 0, 0);
    hItem = TEXT_CreateEx(200, 30, 250, 28, pMsg->hWin, WM_CF_SHOW, 0, ID_CLOCK_CHOOSE, "CHOOSE CLOCK");
    TEXT_SetFont(hItem, GUI_FONT_32_1);
    TEXT_SetTextColor(hItem, GUI_WHITE);     
    
    if(clock_index == 0)
    {
      GUI_DrawBitmap(&bmClock3, 0, 140);
      GUI_DrawBitmap(&bmClock, 180, 100);
      GUI_DrawBitmap(&bmClock2, 440, 140);
      
      hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_CLOCK1);
      WM_SetWindowPos(hItem, 180, 100, 280, 280);
      
      hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_CLOCK2);
      WM_SetWindowPos(hItem, 440, 140, 200, 200);
      
      hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_CLOCK3);
      WM_SetWindowPos(hItem, 0, 140, 200, 200);
      
      k_GetTime(&Time); 
      k_GetDate(&Date);
      
      GUI_UpdateClock (320, 240, Time.Hours, Time.Minutes, Time.Seconds);
      GUI_UpdateClock2 (540, 240, Time.Hours, Time.Minutes, Time.Seconds);      
    }
    else if(clock_index == 1)
    {
      GUI_DrawBitmap(&bmClock1_200, 0, 140);
      GUI_DrawBitmap(&bmClock2_280, 180, 100);
      GUI_DrawBitmap(&bmClock3, 440, 140);
      
      hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_CLOCK1);
      WM_SetWindowPos(hItem, 0, 140, 200, 200);
      
      hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_CLOCK2);
      WM_SetWindowPos(hItem, 180, 100, 280, 280);
      
      hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_CLOCK3);
      WM_SetWindowPos(hItem, 440, 140, 200, 200);
      
      k_GetTime(&Time); 
      k_GetDate(&Date);
      
      GUI_UpdateClock (100, 240, Time.Hours, Time.Minutes, Time.Seconds);
      GUI_UpdateClock2 (320, 240, Time.Hours, Time.Minutes, Time.Seconds);
    }
    else
    {
      GUI_DrawBitmap(&bmClock2, 0, 140);
      GUI_DrawBitmap(&bmClock3_280, 180, 100);
      GUI_DrawBitmap(&bmClock1_200, 440, 140);
      
      hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_CLOCK1);
      WM_SetWindowPos(hItem, 440, 140, 200, 200);
      
      hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_CLOCK2);
      WM_SetWindowPos(hItem, 0, 140, 200, 200);
      
      hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_CLOCK3);
      WM_SetWindowPos(hItem, 180, 100, 280, 280);
      
      k_GetTime(&Time);
      k_GetDate(&Date);
      
      GUI_UpdateClock (540, 240, Time.Hours, Time.Minutes, Time.Seconds);
      GUI_UpdateClock2 (100, 240, Time.Hours, Time.Minutes, Time.Seconds);
    }
    hour  =  Time.Hours;
    min    =  Time.Minutes;
    
    GUI_SetTextMode(GUI_TM_TRANS);    
    GUI_SetFont(&GUI_Font32_ASCII);
    GUI_SetColor(GUI_WHITE);
    if(hour == 12) 
    {
      hour = 0;
    }
    sprintf((char *)TempStr, "%02d %02d", hour , min);    
    
    if(clock_index == 0)
    {
      GUI_DispStringAt((char *)TempStr, 60, 225);
      GUI_SetColor(0x1A468C);
      GUI_AA_FillCircle(320, 240, 8);
      GUI_SetColor(0x555555);
      GUI_AA_FillCircle(540, 240, 5); 
    }
    else if(clock_index == 1)
    {
      GUI_DispStringAt((char *)TempStr, 500, 225);
      GUI_SetColor(0x1A468C);
      GUI_AA_FillCircle(100, 240, 8);
      GUI_SetColor(0x555555);
      GUI_AA_FillCircle(320, 240, 5); 
    }
    else
    {
      GUI_DispStringAt((char *)TempStr, 280, 225);
      GUI_SetColor(0x1A468C);
      GUI_AA_FillCircle(540, 240, 8);
      GUI_SetColor(0x555555);
      GUI_AA_FillCircle(100, 240, 5); 
    }    
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);    /* Id of widget */
    NCode = pMsg->Data.v;               /* Notification code */
    
    switch (NCode)
    {
      case WM_NOTIFICATION_CLICKED:
        if(Id == ID_BUTTON_EXIT)
        {
          WM_HideWindow(pMsg->hWin);
          hTimerTime = WM_CreateTimer(hclockmain, 0, 1000, 0);
        }
        if(Id == ID_BUTTON_NEXT)
        {
          hTimerTime2 = WM_CreateTimer(hTime_Clock, 0, 100, 0);
          WM_HideWindow(pMsg->hWin);
          WM_ShowWindow(hTime_Clock);
        }
        if(Id == ID_BUTTON_CLOCK1)
        {
          clock_index = 0;
          WM_InvalidateWindow(pMsg->hWin);
        }
        if(Id == ID_BUTTON_CLOCK2)
        {
          clock_index = 1;
          WM_InvalidateWindow(pMsg->hWin);
        }
        if(Id == ID_BUTTON_CLOCK3)
        {
          clock_index = 2;
          WM_InvalidateWindow(pMsg->hWin);
        }
        break;
    }   
    break;
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/**
  * @brief  callback for wheel draw
  * @param  pDrawItemInfo: pointer to the draw infos needed
  * @retval None
  */
static int _OwnerDraw(const WIDGET_ITEM_DRAW_INFO * pDrawItemInfo) 
{
  WM_HWIN   hWin;
  WHEEL   * pWheel;
  int       xSizeWin;
  int       ySizeWin;

  hWin = pDrawItemInfo->hWin;
  
  /* Process messages */
  switch (pDrawItemInfo->Cmd) 
  {
    case WIDGET_ITEM_GET_XSIZE:
      /* Return x-size of item */
      return LISTWHEEL_OwnerDraw(pDrawItemInfo);
    case WIDGET_ITEM_GET_YSIZE:
      /* Return y-size of item */
      return LISTWHEEL_OwnerDraw(pDrawItemInfo);
    case WIDGET_ITEM_DRAW:
      /* Draw item */
      return LISTWHEEL_OwnerDraw(pDrawItemInfo);
    case WIDGET_ITEM_DRAW_BACKGROUND:
      xSizeWin = WM_GetWindowSizeX(hWin);
      ySizeWin = WM_GetWindowSizeY(hWin);
      
      /* Draw background */
      GUI_SetColor(GUI_WHITE);
      break;
    case WIDGET_ITEM_DRAW_OVERLAY:
      LISTWHEEL_GetUserData(hWin, &pWheel, sizeof(pWheel));
      xSizeWin = WM_GetWindowSizeX(hWin);
      ySizeWin = WM_GetWindowSizeY(hWin);
      
      GUI_EnableAlpha(1);
      
      GUI_SetColor(GUI_DARKBLUE);
      GUI_DrawVLine(0, 0, ySizeWin - 1);
      GUI_SetColor(GUI_LIGHTBLUE);
      GUI_FillRect(1, 0, 3, ySizeWin - 1);
      
      GUI_SetColor(GUI_DARKBLUE);
      GUI_DrawVLine(3, 0, ySizeWin - 1);
      GUI_SetColor(GUI_LIGHTBLUE);
      GUI_FillRect(0, 0, 2, ySizeWin - 1);
      
      GUI_DrawGradientV(0,                0,                   xSizeWin - 1, (ySizeWin * 2) / 10 - 1,               TRANS0,     TRANS1);
      GUI_DrawGradientV(0, (ySizeWin * 2) / 10,                   xSizeWin - 1, (ySizeWin / 2) - (50 / 2) - 1, TRANS1,     TRANS2);
      
      /* Gray gradients at bottom */
      GUI_DrawGradientV(0, (ySizeWin / 2) + (50 / 2),     xSizeWin - 1, (ySizeWin / 2) + (50 / 2) + 2, 0x88000000, TRANS2);
      GUI_DrawGradientV(0, (ySizeWin / 2) + (50 / 2) + 3, xSizeWin - 1, ySizeWin - (ySizeWin * 2) / 10 - 1,       TRANS2,     TRANS1);
      GUI_DrawGradientV(0, ySizeWin - (ySizeWin * 2) / 10,           xSizeWin - 1, ySizeWin - 1,                          TRANS1,     TRANS0);
      GUI_DrawGradientV(0, ySizeWin - 1,                          xSizeWin - 1, ySizeWin - 1,                          0x33000000, 0x33000000);
      
      /* Translucent glass effect in the middle */
      GUI_SetColor(0x80000000);
      GUI_DrawHLine((ySizeWin / 2) - (50 / 2),     0, xSizeWin - 1);
      GUI_SetColor(0x44000000);
      GUI_DrawHLine((ySizeWin / 2) + (50 / 2) - 1, 0, xSizeWin - 1);
      GUI_DrawGradientV(1, (ySizeWin / 2) - (50 / 2) + 1, xSizeWin - 2, (ySizeWin / 2) - 1,                    0x88583704, 0x55D6B6F4);
      GUI_DrawGradientV(1, (ySizeWin / 2),                        xSizeWin - 2, (ySizeWin / 2) + (50 / 2) - 2, 0x88583704, 0x55D6B6F4);
      
      GUI_EnableAlpha(0);
      break;
  }
  return 0;
}


/**
  * @brief  callback for Date setting window
  * @param  pMsg: pointer to a data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbDateClock(WM_MESSAGE * pMsg) 
{
  int Id, NCode;
  WM_HWIN  hItem;
    
  switch (pMsg->MsgId) 
  {
    case WM_CREATE:
      hItem = BUTTON_CreateEx(580, 17, 50, 50, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_EXIT);
      WM_SetCallback(hItem, _cbButton_exit);
      
      hItem = BUTTON_CreateEx(25, 25, 50, 50, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_PREVIOUS);
      WM_SetCallback(hItem, _cbButton_previous);
      
      _CreateListWheel(68, 120,  102, 250, GUI_ID_LISTWHEEL0, _apDay,   GUI_COUNTOF(_apDay),   GUI_TA_VCENTER | GUI_TA_HCENTER, pMsg->hWin, &_aWheel[0]);
      _CreateListWheel(172, 120, 303, 250, GUI_ID_LISTWHEEL1, _apMonth, GUI_COUNTOF(_apMonth), GUI_TA_VCENTER | GUI_TA_HCENTER, pMsg->hWin, &_aWheel[1]);
      _CreateListWheel(476, 120,  102, 250, GUI_ID_LISTWHEEL2, _apYear,  GUI_COUNTOF(_apYear),  GUI_TA_VCENTER | GUI_TA_HCENTER, pMsg->hWin, &_aWheel[2]);   
      break;
    case WM_PAINT:
      GUI_DrawBitmap(background_clock[clock_index], 0, 0);
      hItem = TEXT_CreateEx(255, 30, 250, 28, pMsg->hWin, WM_CF_SHOW, 0, ID_DATE_CHOOSE, "SET DATE");
      TEXT_SetFont(hItem, GUI_FONT_32_1);
      TEXT_SetTextColor(hItem, GUI_WHITE);    
  
      k_SetTime(&Time);  
      k_SetDate(&Date);        
      break;
    case WM_NOTIFY_PARENT:
      Id    = WM_GetId(pMsg->hWinSrc);    /* Id of widget */
      NCode = pMsg->Data.v;               /* Notification code */
      switch (NCode) 
      {
        case WM_NOTIFICATION_CLICKED:
          if(Id == ID_BUTTON_EXIT)
          { 
            WM_HideWindow(pMsg->hWin);
            hTimerTime = WM_CreateTimer(hclockmain, 0, 1000, 0);
          }   
          if(Id == ID_BUTTON_PREVIOUS)
          {
            hTimerTime2 = WM_CreateTimer(hTime_Clock, 0, 100, 0);
            WM_HideWindow(pMsg->hWin);
            WM_ShowWindow(hTime_Clock);
          } 
          break;
        case WM_NOTIFICATION_SEL_CHANGED:

          day_listwheel = LISTWHEEL_GetItemFromPos(_aWheel[0].hWin, (LISTWHEEL_GetSnapPosition(_aWheel[0].hWin)));
          LISTWHEEL_SetSel(_aWheel[0].hWin, day_listwheel);
          
          month_listwheel = LISTWHEEL_GetItemFromPos(_aWheel[1].hWin, (LISTWHEEL_GetSnapPosition(_aWheel[1].hWin)));
          LISTWHEEL_SetSel(_aWheel[1].hWin, month_listwheel);
          
          year_listwheel = LISTWHEEL_GetItemFromPos(_aWheel[2].hWin, (LISTWHEEL_GetSnapPosition(_aWheel[2].hWin)));
          LISTWHEEL_SetSel(_aWheel[2].hWin, year_listwheel);



          if((month_listwheel == 3) ||  (month_listwheel == 5) || (month_listwheel == 8) || (month_listwheel == 10)) 
          {
            if(day_listwheel == 30)
            {
              day_listwheel = 29;
            }
            else
            {
              day_listwheel = day_listwheel;
            }
          }
          else if(month_listwheel == 1)
          {
            if((day_listwheel == 30) || (day_listwheel == 29))
            {
              day_listwheel = 28;
            }
          }
              
          Date.Date = day_listwheel + 1;
          Date.Month = month_listwheel + 1;
          Date.Year = year_listwheel;       
          
          k_SetDate(&Date);
          break;
      }       
      break;
    default:
      WM_DefaultProc(pMsg);
      break;
  }
}

static int _CreateListWheel(int x, int y, int xSize, int ySize, int Id, char ** apText, int NumItems, int TextAlign, WM_HWIN hParent, WHEEL * pWheel) {
  WM_HWIN hWin;
  int     i, LineHeight = 50;
  
  /* Create LISTWHEEL object */
  hWin          = LISTWHEEL_CreateUser(x, y, xSize, ySize, hParent, WM_CF_SHOW, 0, Id, NULL, sizeof(void *));
  LISTWHEEL_SetFont(hWin, &GUI_Font32B_ASCII);
  LISTWHEEL_SetTextAlign(hWin, TextAlign);
  LISTWHEEL_SetSnapPosition(hWin, (ySize - LineHeight) / 2);
  LISTWHEEL_SetOwnerDraw(hWin, _OwnerDraw);
  LISTWHEEL_SetUserData(hWin, &pWheel, sizeof(pWheel));
  LISTWHEEL_SetLineHeight(hWin, LineHeight);
  LISTWHEEL_SetTextColor(hWin, LISTWHEEL_CI_SEL, GUI_DARKBLUE);
  LISTWHEEL_SetTextColor(hWin, LISTWHEEL_CI_UNSEL, GUI_LIGHTBLUE);
  LISTWHEEL_SetBkColor(hWin, LISTWHEEL_CI_UNSEL, GUI_WHITE);
  LISTWHEEL_SetBkColor(hWin, LISTWHEEL_CI_SEL, GUI_WHITE);
                         
  for (i = 0; i < NumItems; i++) {
    LISTWHEEL_AddString(hWin, *(apText + i));
  }
  pWheel->hWin = hWin;
  return 0;
}

/**
  * @brief  callback for Clock Settings
  * @param  pMsg: Pointer to Date structure
  * @retval None
  */

static void _cbClockSettings(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int Id, NCode;    
         
  
  uint8_t min, hour;
  static uint8_t TempStr[150]; 
  
  hItem = pMsg->hWin;
  switch (pMsg->MsgId) {
    
  case WM_TIMER:
    WM_InvalidateWindow(pMsg->hWin);
    WM_RestartTimer(pMsg->Data.v, 1000);
    break; 
    
  case WM_DELETE:
    WM_DeleteTimer(hTimerTime);
    break;
    
  case WM_CREATE:  
    
    hTimerTime = WM_CreateTimer(pMsg->hWin, 0, 1000, 0);

    hItem = BUTTON_CreateEx(580, 17, 50, 50, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_EXIT);
    WM_SetCallback(hItem, _cbButton_exit);
    
    hItem = BUTTON_CreateEx(538, 379, 102, 101, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_SETTINGS);
    WM_SetCallback(hItem, _cbButton_settings);             

    break;
    
  case WM_PAINT:
    
    GUI_DrawBitmap(background_clock[clock_index], 0, 0);
    GUI_DrawBitmap(bmClock_choose[clock_index], 180, 100);    
    GUI_DrawBitmap(&bmweather_cloud_res, 55, 50);
    GUI_DrawBitmap(&bmweather_cloud_thunder_res, 55, 110); 
    GUI_DrawBitmap(&bmweather_cloud_rain_res, 55, 170);
    GUI_DrawBitmap(&bmweather_cloud_thunder_sun_res, 55, 230);
    GUI_DrawBitmap(&bmweather_cloud_sun_res, 55, 290);  
    GUI_DrawBitmap(&bmweather_sun_res, 55, 350);
    GUI_DrawBitmap(&bmweather_cloud_sun_res, 55, 410);    
    
    GUI_SetTextMode(GUI_TM_TRANS);
    GUI_SetFont(&GUI_Font16_ASCII);    
    GUI_DispStringAt         ("MON", 15, 55);
    GUI_DispStringAt         ("TUE", 15, 115);
    GUI_DispStringAt         ("WED", 15, 175);
    GUI_DispStringAt         ("THU", 15, 235);
    GUI_DispStringAt         ("FRI", 15, 295);
    GUI_DispStringAt         ("SAT", 15, 355);
    GUI_DispStringAt         ("SUN", 15, 415);
    
    if(day_listwheel >= GetNumberOfDays(_apMonth[month_listwheel], (year_listwheel + 2)))
    {
      day_listwheel = GetNumberOfDays(_apMonth[month_listwheel], (year_listwheel + 2)) - 1;
    }  
    
    kabissa = ((year_listwheel + 2)/4);

    if(((year_listwheel + 2)%4) == 0)
    {
      if((month_listwheel > 1))
      {
        kabissa = ((year_listwheel + 2)/4);
      }
      else
      {
        kabissa = ((year_listwheel + 2)/4) - 1;
      }
    }
    
    day_nb = (year_listwheel*365) + _apDayMonth[month_listwheel - 1] + day_listwheel + kabissa;
    day_nb = day_nb%7;
    
    GUI_DrawBitmap(&bmweather_shape_res, 10, 40+(60*day_nb));

    k_GetTime(&Time); 
    k_GetDate(&Date);  

    hour  =  Time.Hours;
    min    =  Time.Minutes;
    
    
    GUI_SetFont(&GUI_Font32_ASCII);
    if(hour == 12) 
    {
      hour = 0;
    }
    sprintf((char *)TempStr, "%02d:%02d", hour , min);
    
    if(clock_index == 0)
    {
      GUI_UpdateClock (320, 240, Time.Hours, Time.Minutes, Time.Seconds);
      GUI_SetColor(GUI_WHITE);
      GUI_DispStringHCenterAt((char *)TempStr, 320, 50);
      GUI_SetColor(0x1A468C);
      GUI_AA_FillCircle(320, 240, 8); 
    }
    else if(clock_index == 1)
    {
      GUI_UpdateClock2 (320, 240, Time.Hours, Time.Minutes, Time.Seconds);
      GUI_SetColor(GUI_WHITE);
      GUI_DispStringHCenterAt((char *)TempStr, 320, 50);
      GUI_SetColor(0x555555);
      GUI_AA_FillCircle(320, 240, 5); 
    }
    else
    {
      GUI_DispStringAt((char *)TempStr, 280, 225);
    }    
         
    GUI_SetColor(GUI_WHITE);
    GUI_SetFont(&GUI_Font32_ASCII);
    
    if(Date.Year > GUI_COUNTOF(_apYear))
      Date.Year = 27;
    
    sprintf((char *)TempStr, "%s %s %s", _apDay[Date.Date - 1], _apMonth[Date.Month - 1], _apYear[Date.Year]);
    
    GUI_DispStringHCenterAt((char *)TempStr, 320, 420);
    
    break;
    
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);    /* Id of widget */
    NCode = pMsg->Data.v;               /* Notification code */
    
    switch (NCode) 
    {
      case WM_NOTIFICATION_CLICKED:
        if(Id == ID_BUTTON_SETTINGS)
        {
          WM_DeleteTimer(hTimerTime);
          WM_ShowWindow(hChoose_Clock); 
        }
        if(Id == ID_BUTTON_EXIT)
        {      
          WM_DeleteWindow(pMsg->hWin);
        }
        break;
    case WM_NOTIFICATION_RELEASED:
      break;
    }          
    break;    
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/**
  * @brief  Analog clock window Startup
  * @param  hWin: pointer to the parent handle.
  * @param  xpos: X position 
  * @param  ypos: Y position
  * @retval None
  */
static void Startup(WM_HWIN hWin, uint16_t xpos, uint16_t ypos)
{
  settings = 2;
  
  /* Create different windows to be used */
  hclockmain    = WM_CreateWindowAsChild(0, 0, 640, 480, hWin, WM_CF_SHOW, _cbClockSettings, 0);
  hChoose_Clock = WM_CreateWindowAsChild(0, 0, 640, 480,hclockmain, WM_CF_HIDE, _cbChooseClock , 0);
  hTime_Clock   = WM_CreateWindowAsChild(0, 0, 640, 480, hclockmain, WM_CF_HIDE, _cbTimeClock , 0);
  hDate_Clock   = WM_CreateWindowAsChild(0, 0, 640, 480,hclockmain, WM_CF_HIDE, _cbDateClock , 0);
}

/**
  * @}
  */

/**
  * @}
  */


