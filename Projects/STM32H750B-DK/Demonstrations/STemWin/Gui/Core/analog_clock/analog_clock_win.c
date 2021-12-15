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
#include "k_modules_res.h"

/* Clock window generated resources */
extern GUI_CONST_STORAGE GUI_BITMAP bmbackground_1;
extern GUI_CONST_STORAGE GUI_BITMAP bmbackground_2;
extern GUI_CONST_STORAGE GUI_BITMAP bmbackground_3;
extern GUI_CONST_STORAGE GUI_BITMAP bmclockprevious;
extern GUI_CONST_STORAGE GUI_BITMAP bmplus;
extern GUI_CONST_STORAGE GUI_BITMAP bmminus;
extern GUI_CONST_STORAGE GUI_BITMAP bmhours_pressed;
extern GUI_CONST_STORAGE GUI_BITMAP bmminutes;
extern GUI_CONST_STORAGE GUI_BITMAP bmminutes_pressed;
extern GUI_CONST_STORAGE GUI_BITMAP bmhours;

extern GUI_CONST_STORAGE GUI_BITMAP bmnext_arrow;
extern GUI_CONST_STORAGE GUI_BITMAP bmfiltre;

/* Alarm generated resources */
extern GUI_CONST_STORAGE GUI_BITMAP bmClock1_anim_1;
extern GUI_CONST_STORAGE GUI_BITMAP bmClock1_anim_2;
extern GUI_CONST_STORAGE GUI_BITMAP bmClock1_anim_3;
extern GUI_CONST_STORAGE GUI_BITMAP bmClock2_anim_1;
extern GUI_CONST_STORAGE GUI_BITMAP bmClock2_anim_2;
extern GUI_CONST_STORAGE GUI_BITMAP bmClock2_anim_3;
extern GUI_CONST_STORAGE GUI_BITMAP bmClock3_anim_1;
extern GUI_CONST_STORAGE GUI_BITMAP bmClock3_anim_2;
extern GUI_CONST_STORAGE GUI_BITMAP bmClock3_anim_3;
extern GUI_CONST_STORAGE GUI_BITMAP bmLABEL_head;
extern GUI_CONST_STORAGE GUI_BITMAP bmshap_alarm;

/* Clocks generated resources */
extern GUI_CONST_STORAGE GUI_BITMAP bmClock1;
extern GUI_CONST_STORAGE GUI_BITMAP bmClock1_small;
extern GUI_CONST_STORAGE GUI_BITMAP bmClock2;
extern GUI_CONST_STORAGE GUI_BITMAP bmClock2_small;
extern GUI_CONST_STORAGE GUI_BITMAP bmClock3;
extern GUI_CONST_STORAGE GUI_BITMAP bmClock3_small;

/* Controls generated resources */
extern GUI_CONST_STORAGE GUI_BITMAP bmalarm_icon;
extern GUI_CONST_STORAGE GUI_BITMAP bmsettings_button;

/* Weather generated resources */
extern GUI_CONST_STORAGE GUI_BITMAP bmweather_cloud_rain_res;
extern GUI_CONST_STORAGE GUI_BITMAP bmweather_cloud_res;
extern GUI_CONST_STORAGE GUI_BITMAP bmweather_cloud_sun_res;
extern GUI_CONST_STORAGE GUI_BITMAP bmweather_cloud_thunder_res;
extern GUI_CONST_STORAGE GUI_BITMAP bmweather_cloud_thunder_sun_res;
extern GUI_CONST_STORAGE GUI_BITMAP bmweather_shape_res;
extern GUI_CONST_STORAGE GUI_BITMAP bmweather_sun_res;

/** @addtogroup ANALOG_CLOCK_MODULE
  * @{
  */

/** @defgroup ANALOG_CLOCK
  * @brief analog clock routines 
  * @{
  */


/* External variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static WM_HWIN hWinClockAndDate;
static void Startup(WM_HWIN hWin, uint16_t xpos, uint16_t ypos);
static int GetNumberOfDays(const char * aMonth, int aYear);

/* Private typedef -----------------------------------------------------------*/
K_ModuleItem_Typedef  analog_clock_board =
{
  MODULE_ANALOG_CLOCK,
  "Clock and Date",
  &hWinClockAndDate,
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
#define ID_BUTTON_NEXT          (GUI_ID_USER + 0x02)
#define ID_BUTTON_PREVIOUS      (GUI_ID_USER + 0x03)
#define ID_BUTTON_CLOCK1        (GUI_ID_USER + 0x04)
#define ID_BUTTON_CLOCK2        (GUI_ID_USER + 0x05)
#define ID_BUTTON_CLOCK3        (GUI_ID_USER + 0x06)
#define ID_CLOCK_CHOOSE         (GUI_ID_USER + 0x07)
#define ID_DATE_CHOOSE          (GUI_ID_USER + 0x08)
#define ID_BUTTON_MINUS         (GUI_ID_USER + 0x09)
#define ID_BUTTON_PLUS          (GUI_ID_USER + 0x0A)
#define ID_BUTTON_HOURS         (GUI_ID_USER + 0x0B)
#define ID_BUTTON_MINUTES       (GUI_ID_USER + 0x0C)

#define ID_BUTTON_ALARM         (GUI_ID_USER + 0x0E)
#define ID_BUTTON_PLUS_ALARM    (GUI_ID_USER + 0x0F)

#define GUI_ID_LISTWHEEL4       (GUI_ID_USER + 0x10)
#define GUI_ID_LISTWHEEL5       (GUI_ID_USER + 0x11)
#define ID_TWO_POINTS           (GUI_ID_USER + 0x12)

#define ID_BUTTON_SUNDAY       (GUI_ID_USER + 0x13)
#define ID_BUTTON_MONDAY       (GUI_ID_USER + 0x14)
#define ID_BUTTON_TUESDAY      (GUI_ID_USER + 0x15)
#define ID_BUTTON_WEDNESDAY    (GUI_ID_USER + 0x16)
#define ID_BUTTON_THIRSDAY     (GUI_ID_USER + 0x17)
#define ID_BUTTON_FRIDAY       (GUI_ID_USER + 0x18)
#define ID_BUTTON_SATURDAY     (GUI_ID_USER + 0x19)

#define GUI_ID_ALARM_LIST      (GUI_ID_USER + 0x50)

#define GUI_ID_BUTTON_ADD_ALARM    (GUI_ID_USER + 0x30)
#define GUI_ID_BUTTON_ALARM_LOCK   (GUI_ID_USER + 0x31)

#define GUI_ID_BUTTON_ALARM1       (GUI_ID_USER + 0x33)
#define GUI_ID_BUTTON_ALARM2       (GUI_ID_USER + 0x34)
#define GUI_ID_BUTTON_ALARM3       (GUI_ID_USER + 0x35)
#define GUI_ID_BUTTON_ALARM4       (GUI_ID_USER + 0x36)
#define GUI_ID_BUTTON_ALARM5       (GUI_ID_USER + 0x37)

#define GUI_ID_BUTTON_ALARM_LOCK1  (GUI_ID_USER + 0x38)
#define GUI_ID_BUTTON_ALARM_LOCK2  (GUI_ID_USER + 0x3A)
#define GUI_ID_BUTTON_ALARM_LOCK3  (GUI_ID_USER + 0x3B)
#define GUI_ID_BUTTON_ALARM_LOCK4  (GUI_ID_USER + 0x3C)
#define GUI_ID_BUTTON_ALARM_LOCK5  (GUI_ID_USER + 0x3D)

#define GUI_ID_BUTTON_ALARM_REPEAT (GUI_ID_USER + 0x3E)

#define GUI_ID_BUTTON_ALARM_REP1   (GUI_ID_USER + 0x20)
#define GUI_ID_BUTTON_ALARM_REP2   (GUI_ID_USER + 0x21)
#define GUI_ID_BUTTON_ALARM_REP3   (GUI_ID_USER + 0x22)
#define GUI_ID_BUTTON_ALARM_REP4   (GUI_ID_USER + 0x23)
#define GUI_ID_BUTTON_ALARM_REP5   (GUI_ID_USER + 0x24)


#define ID_CLOCK_WAKEUP            (GUI_ID_USER + 0x25)
#define GUI_ID_BUTTON_ALARM_SNOOZE (GUI_ID_USER + 0x26)
#define ID_CLOCK_ALARM1            (GUI_ID_USER + 0x27)
#define ID_CLOCK_STOP              (GUI_ID_USER + 0x28)

#define  AA_FACTOR  3
#define  PI         3.14

#define TRANS0    0xEE000000
#define TRANS1    0x11000000
#define TRANS2    0x00000000

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static WM_HWIN  hChoose_Clock, hTime_Clock, hDate_Clock, hAlarm_Clock;

/* Setting the right hour/minutes */
static int      setHours          = 0;
static int      setMinutes        = 0;
static uint8_t  clock_index       = 0;
static uint8_t  settings          = 0;
static uint8_t  new_settings      = 0;
static uint32_t day_listwheel     = 13;
static uint32_t month_listwheel   = 3;
static uint32_t year_listwheel    = 27;



/* RTC defines */
static RTC_TimeTypeDef Time;
static RTC_DateTypeDef Date;

uint32_t hour_listwheel = 0;
uint32_t minute_listwheel = 0;

/* Timer declarations */
static WM_HTIMER hTimerAlarm;
static WM_HTIMER hTimerRTCSync;
static WM_HTIMER hTimerSetTime;

/* All needed information to create an alarm */
typedef struct 
{  
  int   buttonId;
  int   validButtonId;
  int   repeatButtonId;
  int   IsValid;
  int   repeat[8];
  int   added;
  int   hour;
  int   min;
  char  alarmItems[10];
} AlarmParams;

/* Alarm structure to set on the RTC HW */
typedef struct 
{  
  int   IndexAlarmOnRTC; /* Will be "0" to "4" as 5 alarms can be set, If IndexForRepeat = (-1): so no alarm to set*/
  int   Alarm_A_IsSet;
  int   Alarm_B_IsSet;  
} AlarmOnRTC_t;

/* Years interval to be used on the Demo [1990 - 2020] */
static const char * _apYear[] = {
  "2000", "2001", "2002", "2003", "2004", "2005", "2006", "2007", "2008", "2009", "2010", "2011", "2012", "2013", "2014", "2015", "2016", "2017", "2018", "2019", "2020", "2021", "2022", "2023", "2024", "2025", "2026", "2027", "2028", "2029", "2030"
};

/* Months */
static const char * _apMonth[] = {
  "January", "February", "March", "April", " May ", "June", "July", "August", "September", "October", "November", "December"
};

/* Days */
static const char * _apDay[] = {
  "01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30", "31"
};

static const char * _apDay_30[] = {
  "01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30"
};

static const char * _apDay_29[] = {
  "01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29"
};

static const char * _apDay_28[] = {
  "01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28"
};

/* Days by months */
static const uint16_t _apDayMonth[] = {
  31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365
};

/* Days */
static const char * _aphours[] = {
  "00", "01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23"
};

/* Days */
static const char * _apminutes[] = {
  "00", "01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30", "31", "32", "33", "34", "35", "36", "37", "38", "39", "40", "41", "42", "43", "44", "45", "46", "47", "48", "49", "50", "51", "52", "53", "54", "55", "56", "57", "58", "59" 
};

/* 3 Wheels: days, month and years */
static WHEEL _aWheel[8];
static WM_HWIN  hAlarmAlert;

/* Alarm index: 5 SW alarms are possibles */
static int alarmIndex = 0;
static AlarmParams AlarmTable[1];
static AlarmOnRTC_t AlarmOnRTC;

/* Needles draws, clock1 */
static const GUI_POINT aPoints_Clock1[4][4] = {
  /* Hour Needle */
  {{ 2, 0 * AA_FACTOR},
   { 2, -10 * AA_FACTOR},
   {-2, -10 * AA_FACTOR},
   {-2, 0 * AA_FACTOR}},  
  
  /* Min Needle */
  {{ 1, 0 * AA_FACTOR},
   { 1, -10 * AA_FACTOR},
   {-1, -10 * AA_FACTOR},
   {-1, 0 * AA_FACTOR}},
   
   /* Sec Needle */
  {{1, 1 * AA_FACTOR},
   {-1, 1 * AA_FACTOR},
   {-1,-58 * AA_FACTOR},
   {1,-58 * AA_FACTOR}},
   
  {{2, -1 * AA_FACTOR},
   {-2, -1 * AA_FACTOR},
   {-2, 10 * AA_FACTOR},
   {2, 10 * AA_FACTOR}},   
};

/* Needles draws, clock2 */
static const GUI_POINT aPoints_Clock2[4][4] = {
   
  /* Hour Needle */
  {{ 1, 0 * AA_FACTOR},
   { 1, -7 * AA_FACTOR},
   {-1, -7 * AA_FACTOR},
   {-1, 0 * AA_FACTOR}}, 
  
  /* Min Needle */
  {{ 0, 0 * AA_FACTOR},
   { 0, -7 * AA_FACTOR},
   {-1, -7 * AA_FACTOR},
   {-1, 0 * AA_FACTOR}},
   
   /* Sec Needle */
  {{1, 1 * AA_FACTOR},
   {-1, 1 * AA_FACTOR},
   {-1,-30 * AA_FACTOR},
   {1,-30 * AA_FACTOR}},
   
  {{2, -1 * AA_FACTOR},
   {-2, -1 * AA_FACTOR},
   {-2, 10 * AA_FACTOR},
   {2, 10 * AA_FACTOR}},   
};

GUI_POINT aPointsDest_Clock1[4][4];
GUI_POINT aPointsDest_Clock2[4][4];
GUI_POINT aPointsDest_Clock3[4][4];

/* Alarm Animation */ 
static GUI_CONST_STORAGE GUI_BITMAP *AlarmAnim1[] = {
  &bmClock1_anim_1,
  &bmClock1_anim_2,
  &bmClock1_anim_3,
};

static GUI_CONST_STORAGE GUI_BITMAP *AlarmAnim2[] = {
  &bmClock2_anim_1,
  &bmClock2_anim_2,
  &bmClock2_anim_3,
};

static GUI_CONST_STORAGE GUI_BITMAP *AlarmAnim3[] = 
{
  &bmClock3_anim_1,
  &bmClock3_anim_2,
  &bmClock3_anim_3,
};

static const GUI_BITMAP * background_clock[] = {
  &bmbackground_1,   &bmbackground_2, &bmbackground_3   
};

static const GUI_BITMAP * bmClock_choose[] = {
  &bmClock1,   &bmClock2,   &bmClock3
};

/* Private functions ---------------------------------------------------------*/

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
static int GetNumberOfDays(const char * aMonth, int aYear)
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
  * @brief  callback for alarm alert callback
  * @param  None
  * @retval None
  */
static void AlarmAlertCallback(void)
{
  WM_ShowWindow(hAlarmAlert);
}

/**
* @brief  Paints exit button
* @param  hObj: button handle
* @retval None
*/
static void _OnPaint_exit(BUTTON_Handle hObj)
{
  GUI_DrawBitmap(&bmmenu, 10, 10);
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
* @brief  Paints hold button
* @param  hObj: button handle
* @retval None
*/
static void _OnPaint_hold(BUTTON_Handle hObj)
{
  GUI_DrawBitmap(&bmfiltre, 0, 0);
}

/**
* @brief  callback for hold button
* @param  pMsg: pointer to data structure of type WM_MESSAGE
* @retval None
*/
static void _cbButton_hold(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
  case WM_PAINT:
    _OnPaint_hold(pMsg->hWin);
    break;
  default:
    /* The original callback */
    BUTTON_Callback(pMsg);
    break;
  }
}

/**
* @brief  Paints plus alarm button
* @param  hObj: button handle
* @retval None
*/
static void _OnPaint_plus_alarm(BUTTON_Handle hObj)
{
//  GUI_DrawBitmap(&bmplus_alarm, 0, 0);
  GUI_DrawBitmap(&bmplus, 0, 0);
}

/**
* @brief  callback for plus alarm button
* @param  pMsg: pointer to data structure of type WM_MESSAGE
* @retval None
*/
static void _cbButton_plus_alarm(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
  case WM_PAINT:
    _OnPaint_plus_alarm(pMsg->hWin);
    break;
  default:
    /* The original callback */
    BUTTON_Callback(pMsg);
    break;
  }
}

/**
* @brief  Paints alarm button
* @param  hObj: button handle
* @retval None
*/
static void _OnPaint_alarm(BUTTON_Handle hObj)
{
  GUI_DrawBitmap(&bmalarm_icon, 0, 0);
}

/**
* @brief  callback for alarm button
* @param  pMsg: pointer to data structure of type WM_MESSAGE
* @retval None
*/
static void _cbButton_alarm(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
  case WM_PAINT:
    _OnPaint_alarm(pMsg->hWin);
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
  GUI_DrawBitmap(&bmsettings_button, 0, 0);
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
  GUI_DrawBitmap(&bmnext_arrow, 0, 0);
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
  GUI_DrawBitmap(&bmclockprevious, 10, 10);
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
  GUI_DrawBitmap(&bmminus, 5, 5);
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
  GUI_DrawBitmap(&bmplus, 5, 5);
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
* @brief  refresh clocks variables
* @param  None
* @retval None
*/
static void UpdateClockData(void)
{
  /* Check if any update is needed for the minute's value */
  if(setMinutes == -1)
  {
    if(Time.Minutes > 0)
    {
      Time.Minutes--;
    }
    else
    {
      setHours = -2; /* Internal request to update Hours */
      Time.Minutes = 59;
    }
  }
  else if(setMinutes == 1)
  {
    if(Time.Minutes < 59)
    {
      Time.Minutes++;
    }
    else
    {
      setHours = 2; /* Internal request to update Hours */
      Time.Minutes = 0;
    }
  }

  /* Check if any update is needed for the Hour's value */
  if(setHours < 0)
  {
    if(Time.Hours > 0)
    {
      Time.Hours--;
    }
    else
    {
      Time.Hours = 23;
    }
    if(setHours == -2)
    {
      setHours = 0; /* Reset internal request */
    }
  }
  else if(setHours > 0)
  {
    if(Time.Hours < 23)
    {
      Time.Hours++;
    }
    else
    {
      Time.Hours = 0;
    }
    if(setHours == 2)
    {
      setHours = 0; /* Reset internal request */
    }
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
  uint8_t hours;
  static uint8_t TempStr[50];
   
  switch (pMsg->MsgId) 
  {
    case WM_CREATE:
      hItem = BUTTON_CreateEx(440, 5, 35, 30, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_EXIT);
      WM_SetCallback(hItem, _cbButton_exit);
      
      hItem = BUTTON_CreateEx(420, 215, 58, 57, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_NEXT);
      WM_SetCallback(hItem, _cbButton_next);  
      
      hItem = BUTTON_CreateEx(5, 5, 100, 100, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_PREVIOUS);
      WM_SetCallback(hItem, _cbButton_previous);
      
      hItem = BUTTON_CreateEx(80, 120, 85, 85, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_MINUS);
      WM_SetCallback(hItem, _cbButton_minus); 
      
      hItem = BUTTON_CreateEx(335, 120, 85, 85, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_PLUS);
      WM_SetCallback(hItem, _cbButton_plus);
      
      hItem = BUTTON_CreateEx(170, 215, 50, 50, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_HOURS);
      WM_SetCallback(hItem, _cbButton_hours); 

      hItem = BUTTON_CreateEx(260, 215, 50, 50, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_MINUTES);
      WM_SetCallback(hItem, _cbButton_minutes);

      hItem = TEXT_CreateEx(200, 20, 250, 28, pMsg->hWin, WM_CF_SHOW, 0, ID_CLOCK_CHOOSE, "SET TIME");
      TEXT_SetFont(hItem, &GUI_FontArialRoundedBold24);
      TEXT_SetTextColor(hItem, GUI_WHITE);
      
      break;

    case WM_TIMER:
      if(setHours || setMinutes)
      {
        UpdateClockData();
        /* Refresh the whole screen */
        WM_InvalidateWindow(pMsg->hWin);
        if(new_settings < 5)
        {
          new_settings++;
        }
      }
      if(new_settings == 5) /* long press */
      {
        if(setMinutes)
        {
          WM_RestartTimer(hTimerSetTime, 25);
        }
        else
        {
          WM_RestartTimer(hTimerSetTime, 100);
        }
      }
      else
      {
        WM_RestartTimer(hTimerSetTime, 200);
      }
      break; 
      
    case WM_DELETE:
      if(hTimerSetTime != 0)
      {
        WM_DeleteTimer(hTimerSetTime);
        hTimerSetTime = 0;
      }
      break;
    
    case WM_PAINT:
      hours  =  Time.Hours;

      GUI_DrawBitmap(background_clock[clock_index], 0, 0);

      if(clock_index == 0)
      {
        if(hours > 11)
        {
          hours -= 12;
        }
        GUI_DrawBitmap(&bmClock1, 165, 66);
        GUI_UpdateClock (240, 139, hours, Time.Minutes, Time.Seconds);
        GUI_SetColor(0xFF1A468C);
        GUI_AA_FillCircle(240, 139, 5);
      }
      else if(clock_index == 1)
      {
        if(hours > 11)
        {
          hours -= 12;
        }
        GUI_DrawBitmap(&bmClock2, 165, 66);
        GUI_UpdateClock2 (240, 139, hours, Time.Minutes, Time.Seconds);
        GUI_SetColor(0xFF555555);
        GUI_AA_FillCircle(240, 139, 4);
      }
      else
      {
        GUI_DrawBitmap(&bmClock3, 165, 66);
        GUI_SetTextMode(GUI_TM_TRANS);
        GUI_SetFont(&GUI_FontArialRoundedBold24);
        GUI_SetColor(GUI_WHITE);

        sprintf((char *)TempStr, "%02d %02d", hours , Time.Minutes);
        GUI_DispStringAt((char *)TempStr, 220, 130);
      }
      break;

    case WM_NOTIFY_PARENT:
      Id    = WM_GetId(pMsg->hWinSrc);    /* Id of widget */
      NCode = pMsg->Data.v;               /* Notification code */
      switch (NCode) 
      {
        case WM_NOTIFICATION_GOT_FOCUS:
          if(new_settings)
          {
            k_SetTime(&Time);
            new_settings = 0;
          }
          break;
        case WM_NOTIFICATION_CLICKED:
          if(Id == ID_BUTTON_EXIT)
          {
            WM_HideWindow(pMsg->hWin);
            if(hTimerSetTime != 0)
            {
              WM_DeleteTimer(hTimerSetTime);
              hTimerSetTime = 0;
            }
            hTimerRTCSync = WM_CreateTimer(hWinClockAndDate, 0, 10, 0);
          }
          if(Id == ID_BUTTON_HOURS)
          {
            settings = 2;
            WM_InvalidateWindow(pMsg->hWinSrc);
            hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_MINUTES);
            WM_InvalidateWindow(hItem);
          }
          if(Id == ID_BUTTON_MINUTES)
          {      
            settings = 1;
            WM_InvalidateWindow(pMsg->hWinSrc);
            hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_HOURS);
            WM_InvalidateWindow(hItem);
          }
          if(Id == ID_BUTTON_NEXT)
          {
            if(hTimerSetTime != 0)
            {
              WM_DeleteTimer(hTimerSetTime);
              hTimerSetTime = 0;
            }
            WM_HideWindow(pMsg->hWin);
            WM_ShowWindow(hDate_Clock);
          }
          if(Id == ID_BUTTON_PREVIOUS)
          {
            if(hTimerSetTime != 0)
            {
              WM_DeleteTimer(hTimerSetTime);
              hTimerSetTime = 0;
            }
            WM_HideWindow(pMsg->hWin);
            WM_ShowWindow(hChoose_Clock);
          } 
          if(Id == ID_BUTTON_PLUS)
          {
            new_settings = 1;
            if (settings == 1)
            {
              setMinutes  = 1;
              setHours    = 0;
            }
            else if (settings == 2)
            { 
              setMinutes  = 0;
              setHours    = 1;
            }
          }
          if(Id == ID_BUTTON_MINUS)
          {
            new_settings = 1;
            if (settings == 1)
            {
              setMinutes  = -1;
              setHours    = 0;
            }
            else if (settings == 2)
            { 
              setMinutes  = 0;
              setHours    = -1;
            }
          }
          break;
      case WM_NOTIFICATION_RELEASED:
      case WM_NOTIFICATION_MOVED_OUT:
        if( (Id == ID_BUTTON_PLUS) || (Id == ID_BUTTON_MINUS) )
        {
          setHours = 0;
          setMinutes = 0;
          if(new_settings == 5) /* Reset long press state */
          {
            new_settings = 1;
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
      hItem = BUTTON_CreateEx(440, 5, 35, 30, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_EXIT);
      WM_SetCallback(hItem, _cbButton_exit);
      
      hItem = BUTTON_CreateEx(420, 215, 58, 57, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_NEXT);
      WM_SetCallback(hItem, _cbButton_next);  
      
      hItem = BUTTON_CreateEx(165, 66, 150, 150, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_CLOCK1);
      WM_SetCallback(hItem, _cbButton_backclock); 
      
      hItem = BUTTON_CreateEx(345, 78, 115, 115, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_CLOCK2);
      WM_SetCallback(hItem, _cbButton_backclock); 
      
      hItem = BUTTON_CreateEx(30, 78, 115, 115, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_CLOCK3);
      WM_SetCallback(hItem, _cbButton_backclock);

      hItem = TEXT_CreateEx(170, 20, 250, 28, pMsg->hWin, WM_CF_SHOW, 0, ID_CLOCK_CHOOSE, "CHOOSE CLOCK");
      TEXT_SetFont(hItem, &GUI_FontArialRoundedBold24);
      TEXT_SetTextColor(hItem, GUI_WHITE);
      break;
      
  case WM_PAINT:
    GUI_DrawBitmap(background_clock[clock_index], 0, 0);

    if(clock_index == 0)
    {
      GUI_DrawBitmap(&bmClock1, 165, 66);
      GUI_DrawBitmap(&bmClock2_small, 345, 78);
      GUI_DrawBitmap(&bmClock3_small, 30, 78);
      
      hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_CLOCK1);
      WM_SetWindowPos(hItem, 165, 66, 150, 150);
      
      hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_CLOCK2);
      WM_SetWindowPos(hItem, 345, 78, 115, 115);
      
      hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_CLOCK3);
      WM_SetWindowPos(hItem, 30, 78, 115, 115);

      GUI_UpdateClock (240, 139, Time.Hours, Time.Minutes, Time.Seconds);
      GUI_UpdateClock2 (403, 136, Time.Hours, Time.Minutes, Time.Seconds);
    }
    else if(clock_index == 1)
    {
      GUI_DrawBitmap(&bmClock1_small, 30, 78);
      GUI_DrawBitmap(&bmClock2, 165, 66);
      GUI_DrawBitmap(&bmClock3_small, 345, 78);
      
      hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_CLOCK1);
      WM_SetWindowPos(hItem, 30, 78, 115, 115);
      
      hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_CLOCK2);
      WM_SetWindowPos(hItem, 165, 66, 150, 150);
      
      hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_CLOCK3);
      WM_SetWindowPos(hItem, 345, 78, 115, 115);

      GUI_UpdateClock (87, 136, Time.Hours, Time.Minutes, Time.Seconds);
      GUI_UpdateClock2 (240, 139, Time.Hours, Time.Minutes, Time.Seconds);
    }
    else
    {
      GUI_DrawBitmap(&bmClock2_small, 30, 78);
      GUI_DrawBitmap(&bmClock1_small, 345, 78);
      GUI_DrawBitmap(&bmClock3, 165, 66);
      
      hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_CLOCK1);
      WM_SetWindowPos(hItem, 345, 140, 115, 115);
      
      hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_CLOCK2);
      WM_SetWindowPos(hItem, 30, 140, 115, 115);
      
      hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_CLOCK3);
      WM_SetWindowPos(hItem, 165, 66, 150, 150);

      GUI_UpdateClock (403, 139, Time.Hours, Time.Minutes, Time.Seconds);
      GUI_UpdateClock2 (87, 136, Time.Hours, Time.Minutes, Time.Seconds);
    }
    hour  =  Time.Hours;
    min   =  Time.Minutes;
    
    GUI_SetTextMode(GUI_TM_TRANS);    
    GUI_SetFont(&GUI_Font16B_1);
    GUI_SetColor(GUI_WHITE);
    if(hour == 12) 
    {
      hour = 0;
    }
    sprintf((char *)TempStr, "%02d %02d", hour , min);    
    
    if(clock_index == 0)
    {
      GUI_DispStringAt((char *)TempStr, 72, 125);
      GUI_SetColor(0xFF1A468C);
      GUI_AA_FillCircle(240, 139, 5);
      GUI_SetColor(0xFF555555);
      GUI_AA_FillCircle(403, 136, 4);
    }
    else if(clock_index == 1)
    {
      GUI_DispStringAt((char *)TempStr, 385, 125);
      GUI_SetColor(0xFF1A468C);
      GUI_AA_FillCircle(87, 136, 5);
      GUI_SetColor(0xFF555555);
      GUI_AA_FillCircle(240, 139, 5);
    }
    else
    {
      GUI_DispStringAt((char *)TempStr, 225, 130);
      GUI_SetColor(0xFF1A468C);
      GUI_AA_FillCircle(403, 136, 5);
      GUI_SetColor(0xFF555555);
      GUI_AA_FillCircle(87, 136, 4);
    }    
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);    /* Id of widget */
    NCode = pMsg->Data.v;               /* Notification code */
    
    switch (NCode)
    {
      case WM_NOTIFICATION_RELEASED:
        switch(Id)
        {
          case ID_BUTTON_EXIT :
            WM_HideWindow(pMsg->hWin);
            hTimerRTCSync = WM_CreateTimer(hWinClockAndDate, 0, 10, 0);
          break;
          case ID_BUTTON_NEXT :
            hTimerSetTime = WM_CreateTimer(hTime_Clock, 0, 100, 0);
            WM_HideWindow(pMsg->hWin);
            WM_ShowWindow(hTime_Clock);
          break;
          case ID_BUTTON_CLOCK1 :
          case ID_BUTTON_CLOCK2 :
          case ID_BUTTON_CLOCK3 :
            clock_index = (Id - ID_BUTTON_CLOCK1);
            WM_InvalidateWindow(pMsg->hWin);
          break;
          default :
            /* Nothing to do */
          break;
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
  * @brief  Function for a personilised liswheel creation
  * @param  x: x position
  * @param  y: y position
  * @param  xSize: x size
  * @param  ySize: y size
  * @param  Id: wheel id
  * @param  pText: text table used 
  * @param  NumItems: number of items of the table
  * @param  TextAlign: text align
  * @param  hParent: parent handle
  * @param  pWheel: wheel to use
  * @retval None
  */
static void _CreateListWheel_Alarm(int x, int y, int xSize, int ySize, int Id, const char ** apText, int NumItems, int TextAlign, WM_HWIN hParent, WHEEL * pWheel) {
  WM_HWIN hWin;
  int     i, LineHeight = 33;
  
  /* Create LISTWHEEL object */
  hWin          = LISTWHEEL_CreateUser(x, y, xSize, ySize, hParent, WM_CF_SHOW | WM_CF_HASTRANS, 0, Id, NULL, sizeof(void *));
  LISTWHEEL_SetFont(hWin, &GUI_FontArialRoundedBold20);
  LISTWHEEL_SetTextAlign(hWin, TextAlign);
  LISTWHEEL_SetSnapPosition(hWin, LineHeight);
  LISTWHEEL_SetUserData(hWin, &pWheel, sizeof(pWheel));
  LISTWHEEL_SetLineHeight(hWin, LineHeight); 

  LISTWHEEL_SetTextColor(hWin, LISTWHEEL_CI_SEL, GUI_WHITE);
  LISTWHEEL_SetTextColor(hWin, LISTWHEEL_CI_UNSEL, GUI_LIGHTBLUE);
  LISTWHEEL_SetBkColor(hWin, LISTWHEEL_CI_UNSEL, GUI_TRANSPARENT);
  LISTWHEEL_SetBkColor(hWin, LISTWHEEL_CI_SEL, GUI_TRANSPARENT);
  
  LISTWHEEL_SetDeceleration(hWin, 10000);
  
  for (i = 0; i < NumItems; i++)
  {
    LISTWHEEL_AddString(hWin, *(apText + i));
  }
  pWheel->hWin = hWin;
  return;
}

/**
  * @brief  Function for a personilised liswheel creation
  * @param  x: x position
  * @param  y: y position
  * @param  xSize: x size
  * @param  ySize: y size
  * @param  Id: wheel id
  * @param  pText: text table used 
  * @param  NumItems: number of items of the table
  * @param  TextAlign: text align
  * @param  hParent: parent handle
  * @param  pWheel: wheel to use
  * @retval None
  */
static void _CreateListWheel(int x, int y, int xSize, int ySize, int Id, const char ** apText, int NumItems, int TextAlign, int Show_hide, WM_HWIN hParent, WHEEL * pWheel) {
  WM_HWIN hWin;
  int     i, LineHeight = 33;
  
  /* Create LISTWHEEL object */
  hWin          = LISTWHEEL_CreateUser(x, y, xSize, ySize, hParent, Show_hide | WM_CF_HASTRANS, 0, Id, NULL, sizeof(void *));
  LISTWHEEL_SetFont(hWin, &GUI_FontArialRoundedBold20);
  LISTWHEEL_SetTextAlign(hWin, TextAlign);
  LISTWHEEL_SetSnapPosition(hWin, LineHeight);
  LISTWHEEL_SetUserData(hWin, &pWheel, sizeof(pWheel));
  LISTWHEEL_SetLineHeight(hWin, LineHeight); 

  LISTWHEEL_SetTextColor(hWin, LISTWHEEL_CI_SEL, GUI_WHITE);
  LISTWHEEL_SetTextColor(hWin, LISTWHEEL_CI_UNSEL, GUI_LIGHTBLUE);
  LISTWHEEL_SetBkColor(hWin, LISTWHEEL_CI_UNSEL, GUI_TRANSPARENT);
  LISTWHEEL_SetBkColor(hWin, LISTWHEEL_CI_SEL, GUI_TRANSPARENT);
                         
  for (i = 0; i < NumItems; i++)
  {
    LISTWHEEL_AddString(hWin, *(apText + i));
  }
  pWheel->hWin = hWin;
  return;
}

/**
  * @brief  Program the RTC with the right alarm
  * @param  none 
  * @retval int
  */
static void SetAlarmOnRTC(void) 
{
  RTC_AlarmTypeDef alarm;
  
    /* Set the First alarm */
    alarm.Alarm                 = RTC_ALARM_A;
    alarm.AlarmDateWeekDay      = RTC_WEEKDAY_MONDAY | RTC_WEEKDAY_TUESDAY | RTC_WEEKDAY_WEDNESDAY | RTC_WEEKDAY_THURSDAY | RTC_WEEKDAY_FRIDAY | RTC_WEEKDAY_SUNDAY | RTC_WEEKDAY_SATURDAY;
    alarm.AlarmDateWeekDaySel   = RTC_ALARMDATEWEEKDAYSEL_WEEKDAY;
    alarm.AlarmMask             = RTC_ALARMMASK_DATEWEEKDAY;
    alarm.AlarmSubSecondMask    = RTC_ALARMSUBSECONDMASK_NONE;

    alarm.AlarmTime.TimeFormat = RTC_HOURFORMAT12_AM;
    alarm.AlarmTime.Hours      = AlarmTable[AlarmOnRTC.IndexAlarmOnRTC].hour;
    alarm.AlarmTime.Minutes     = AlarmTable[AlarmOnRTC.IndexAlarmOnRTC].min;
    alarm.AlarmTime.Seconds     = 0;
    alarm.AlarmTime.SubSeconds  = 0;

    k_SetAlarm(&alarm);
    k_SetAlarmCallback(AlarmAlertCallback);
    AlarmOnRTC.Alarm_A_IsSet = 1;
  
  return;
}

/**
  * @brief  Get the right alarm to set
  * @param  none 
  * @retval int
  */
static int IndexOfAlarmToSet(void) 
{
  int i;  
  
  /* First of all get the Time and the Date */
  k_GetTime(&Time); 
  
  /* We need to find the alarm with the closest hour, then the closest minutes */
  for(i=0; i < alarmIndex; i++)
  {    
    if(AlarmTable[i].IsValid)
    {
      if((strstr(AlarmTable[i].alarmItems, "AM")) != NULL)
      {
        if((AlarmTable[i].hour + 1) >= Time.Hours)
        {
          if(AlarmTable[i].min > Time.Minutes)
          {
            return i;
          }
        }
      }
      else
      {
        if((AlarmTable[i].hour + 1) >= Time.Hours)
        {
          if(AlarmTable[i].min >= Time.Minutes)
          {
            return i;
          }
        }
      }
    } 
  }
  return 0;
}

/**
  * @brief  callback for alarm setting window
  * @param  pMsg: pointer to a data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbAlarmClock(WM_MESSAGE * pMsg) 
{
  int Id, NCode, i;
  WM_HWIN  hItem;
  static char TempStr[20];

  
  switch (pMsg->MsgId) 
  {
    case WM_CREATE:

      hItem = BUTTON_CreateEx(440, 5, 35, 30, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_EXIT);
      WM_SetCallback(hItem, _cbButton_exit);

      hItem = BUTTON_CreateEx(212, 160, 120, 120, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_PLUS_ALARM);
      WM_SetCallback(hItem, _cbButton_plus_alarm);
      
      _CreateListWheel_Alarm(65, 70,  40, 60, GUI_ID_LISTWHEEL3, _aphours,   GUI_COUNTOF(_aphours),   GUI_TA_VCENTER | GUI_TA_HCENTER, pMsg->hWin, &_aWheel[3]);
      _CreateListWheel_Alarm(110, 70, 40, 60, GUI_ID_LISTWHEEL4, _apminutes, GUI_COUNTOF(_apminutes), GUI_TA_VCENTER | GUI_TA_HCENTER, pMsg->hWin, &_aWheel[4]);

      /* Update the Alarm Time wheel */
      LISTWHEEL_SetSel(_aWheel[3].hWin, hour_listwheel);
      LISTWHEEL_MoveToPos(_aWheel[3].hWin, hour_listwheel);
      LISTWHEEL_SetSel(_aWheel[4].hWin, minute_listwheel);
      LISTWHEEL_MoveToPos(_aWheel[4].hWin, minute_listwheel);
          
      hItem = TEXT_CreateEx(360, 105, 250, 35, pMsg->hWin, WM_CF_SHOW, 0, ID_CLOCK_ALARM1, "00 : 00");
      TEXT_SetFont(hItem, &GUI_FontArialRoundedBold20);
      TEXT_SetTextColor(hItem, GUI_BLUE);
      hItem = TEXT_CreateEx(190, 20, 250, 28, pMsg->hWin, WM_CF_SHOW, 0, ID_DATE_CHOOSE, "SET ALARM");
      TEXT_SetFont(hItem, &GUI_FontArialRoundedBold24);
      TEXT_SetTextColor(hItem, GUI_WHITE);
      
      hItem = TEXT_CreateEx(95, 105, 30, 30, pMsg->hWin, WM_CF_SHOW, 0, ID_TWO_POINTS, " : ");
      TEXT_SetFont(hItem, &GUI_FontArialRoundedBold24);
      TEXT_SetTextColor(hItem, GUI_WHITE);
      break;

    case WM_PAINT:
      GUI_DrawBitmap(background_clock[clock_index], 0, 0);
      GUI_DrawBitmap(&bmshap_alarm, 60, 100);
      GUI_DrawBitmap(&bmshap_alarm, 340, 100);

      hItem = WM_GetDialogItem(pMsg->hWin, ID_CLOCK_ALARM1);
      if(clock_index == 0)
      {
        TEXT_SetTextColor(hItem, GUI_BLUE);
      }
      else
      {
        TEXT_SetTextColor(hItem, GUI_WHITE);
      }

      break;
    case WM_NOTIFY_PARENT:
      Id    = WM_GetId(pMsg->hWinSrc);    /* Id of widget */
      NCode = pMsg->Data.v;               /* Notification code */
      switch (NCode) 
      {
        case WM_NOTIFICATION_RELEASED:
        case WM_NOTIFICATION_MOVED_OUT:
          if(Id == ID_BUTTON_EXIT)
          { 
            WM_HideWindow(pMsg->hWin);
          }   

          hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_PLUS_ALARM);
          WM_InvalidateWindow(hItem);
          WM_Update(hItem); 
          
          
          hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_NEXT);
          WM_InvalidateWindow(hItem);
          WM_Update(hItem);           
          
          if(Id == ID_BUTTON_PLUS_ALARM)
          {
            /* Set the needed alarm before leaving */              
            sprintf((char *)AlarmTable[alarmIndex].alarmItems, "%s:%s", _aphours[hour_listwheel], _apminutes[minute_listwheel]);
            
            AlarmTable[alarmIndex].hour = hour_listwheel;
            AlarmTable[alarmIndex].min = minute_listwheel;
            AlarmTable[alarmIndex].added = 1;
            AlarmTable[alarmIndex].buttonId = (GUI_ID_BUTTON_ALARM1 + alarmIndex);
            AlarmTable[alarmIndex].validButtonId = (GUI_ID_BUTTON_ALARM_LOCK1 + alarmIndex);
            AlarmTable[alarmIndex].repeatButtonId = (GUI_ID_BUTTON_ALARM_REP1 + alarmIndex);
            AlarmTable[alarmIndex].IsValid = 1;
            
            for(i = 0; i < 7; i++)
            {
              AlarmTable[alarmIndex].repeat[i] = 0;
            }
            if(alarmIndex >= 1)
            {

            }
            
            AlarmOnRTC.IndexAlarmOnRTC = IndexOfAlarmToSet();
            
            /* Now Program the right alarm on the RTC HW */              
            SetAlarmOnRTC();
            
            hItem = WM_GetDialogItem(pMsg->hWin, ID_CLOCK_ALARM1);
            sprintf((char *)TempStr, "%02lu : %02lu", hour_listwheel , minute_listwheel);
            TEXT_SetText     (hItem, TempStr); 
          }      
          
          break;
          
      case WM_NOTIFICATION_SEL_CHANGED:
        hour_listwheel = LISTWHEEL_GetItemFromPos(_aWheel[3].hWin, (LISTWHEEL_GetSnapPosition(_aWheel[3].hWin)));
        LISTWHEEL_SetSel(_aWheel[3].hWin, hour_listwheel);
        
        minute_listwheel = LISTWHEEL_GetItemFromPos(_aWheel[4].hWin, (LISTWHEEL_GetSnapPosition(_aWheel[4].hWin)));
        LISTWHEEL_SetSel(_aWheel[4].hWin, minute_listwheel);
        
//        k_SetDate(&Date);
        break;          
      }       
      break;
  default:
    WM_DefaultProc(pMsg);
    break;
  }
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
  static uint32_t month = 0;

  switch (pMsg->MsgId) 
  {
    case WM_CREATE:
      hItem = BUTTON_CreateEx(440, 5, 35, 30, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_EXIT);
      WM_SetCallback(hItem, _cbButton_exit);
      
      hItem = BUTTON_CreateEx(5, 5, 100, 100, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_PREVIOUS);
      WM_SetCallback(hItem, _cbButton_previous);

      hItem = TEXT_CreateEx(200, 20, 200, 28, pMsg->hWin, WM_CF_SHOW, 0, ID_DATE_CHOOSE, "SET DATE");
      TEXT_SetFont(hItem, &GUI_FontArialRoundedBold24);
      TEXT_SetTextColor(hItem, GUI_WHITE);

      _CreateListWheel(60, 80,  70, 100, GUI_ID_LISTWHEEL0, _apDay,   GUI_COUNTOF(_apDay),   GUI_TA_VCENTER | GUI_TA_HCENTER, WM_CF_SHOW, pMsg->hWin, &_aWheel[0]);
      _CreateListWheel(150, 80, 150, 100, GUI_ID_LISTWHEEL1, _apMonth, GUI_COUNTOF(_apMonth), GUI_TA_VCENTER | GUI_TA_HCENTER, WM_CF_SHOW, pMsg->hWin, &_aWheel[1]);
      _CreateListWheel(360, 80,  70, 100, GUI_ID_LISTWHEEL2, _apYear,  GUI_COUNTOF(_apYear),  GUI_TA_VCENTER | GUI_TA_HCENTER, WM_CF_SHOW, pMsg->hWin, &_aWheel[2]); 

      _CreateListWheel(60, 80,  70, 100, GUI_ID_LISTWHEEL0, _apDay_30,   GUI_COUNTOF(_apDay_30),   GUI_TA_VCENTER | GUI_TA_HCENTER, WM_CF_HIDE, pMsg->hWin, &_aWheel[5]);
      _CreateListWheel(60, 80,  70, 100, GUI_ID_LISTWHEEL0, _apDay_29,   GUI_COUNTOF(_apDay_29),   GUI_TA_VCENTER | GUI_TA_HCENTER, WM_CF_HIDE, pMsg->hWin, &_aWheel[6]);
      _CreateListWheel(60, 80,  70, 100, GUI_ID_LISTWHEEL0, _apDay_28,   GUI_COUNTOF(_apDay_28),   GUI_TA_VCENTER | GUI_TA_HCENTER, WM_CF_HIDE, pMsg->hWin, &_aWheel[7]);

      /* Update the Date variables */
      day_listwheel   = Date.Date - 1;
      month_listwheel = Date.Month - 1;
      year_listwheel  = Date.Year;

      /* Update the Day wheel */
      LISTWHEEL_SetSel(_aWheel[0].hWin, day_listwheel);
      LISTWHEEL_MoveToPos(_aWheel[0].hWin, day_listwheel);
      LISTWHEEL_SetSel(_aWheel[5].hWin, day_listwheel);
      LISTWHEEL_MoveToPos(_aWheel[5].hWin, day_listwheel);
      LISTWHEEL_SetSel(_aWheel[6].hWin, day_listwheel);
      LISTWHEEL_MoveToPos(_aWheel[6].hWin, day_listwheel);
      LISTWHEEL_SetSel(_aWheel[7].hWin, day_listwheel);
      LISTWHEEL_MoveToPos(_aWheel[7].hWin, day_listwheel);

      /* Update the Month wheel */
      LISTWHEEL_SetSel(_aWheel[1].hWin, month_listwheel);
      LISTWHEEL_MoveToPos(_aWheel[1].hWin, month_listwheel);

      /* Update the Year wheel */
      LISTWHEEL_SetSel(_aWheel[2].hWin, year_listwheel);
      LISTWHEEL_MoveToPos(_aWheel[2].hWin, year_listwheel);
      break;

    case WM_PAINT:
      GUI_DrawBitmap(background_clock[clock_index], 0, 0);
      GUI_DrawBitmap(&bmfiltre, 65, 113);
      break;

    case WM_NOTIFY_PARENT:
      Id    = WM_GetId(pMsg->hWinSrc);    /* Id of widget */
      NCode = pMsg->Data.v;               /* Notification code */
      switch (NCode) 
      {
        case WM_NOTIFICATION_GOT_FOCUS:
          if(new_settings)
          {
            k_SetDate(&Date);
            new_settings = 0;
          }
          break;
        case WM_NOTIFICATION_CLICKED:
          if(Id == ID_BUTTON_EXIT)
          {
            WM_HideWindow(pMsg->hWin);
            hTimerRTCSync = WM_CreateTimer(hWinClockAndDate, 0, 10, 0);
          }
          if(Id == ID_BUTTON_PREVIOUS)
          {
            hTimerSetTime = WM_CreateTimer(hTime_Clock, 0, 100, 0);
            WM_HideWindow(pMsg->hWin);
            WM_ShowWindow(hTime_Clock);
          }
          break;
        case WM_NOTIFICATION_SEL_CHANGED:
          new_settings = 1;

          day_listwheel = LISTWHEEL_GetItemFromPos(_aWheel[month].hWin, (LISTWHEEL_GetSnapPosition(_aWheel[month].hWin)));
          LISTWHEEL_SetSel(_aWheel[month].hWin, day_listwheel);
          
          month_listwheel = LISTWHEEL_GetItemFromPos(_aWheel[1].hWin, (LISTWHEEL_GetSnapPosition(_aWheel[1].hWin)));
          LISTWHEEL_SetSel(_aWheel[1].hWin, month_listwheel);
          
          year_listwheel = LISTWHEEL_GetItemFromPos(_aWheel[2].hWin, (LISTWHEEL_GetSnapPosition(_aWheel[2].hWin)));
          LISTWHEEL_SetSel(_aWheel[2].hWin, year_listwheel);

          if((month_listwheel == 3) ||  (month_listwheel == 5) || (month_listwheel == 8) || (month_listwheel == 10)) 
          {
            month = 5;
            WM_HideWindow(_aWheel[0].hWin);
            WM_HideWindow(_aWheel[6].hWin);
            WM_HideWindow(_aWheel[7].hWin);
            WM_ShowWindow(_aWheel[5].hWin);

            if(day_listwheel == 30)
            {
              day_listwheel = 29;
            }
            LISTWHEEL_SetSel(_aWheel[month].hWin, day_listwheel);
            LISTWHEEL_MoveToPos(_aWheel[5].hWin, day_listwheel);
          }
          else if(month_listwheel == 1)
          {
            if((((year_listwheel + 2)%4) != 0))
            {
              month = 7;
            if((day_listwheel == 30)  || (day_listwheel == 29) || (day_listwheel == 28))
            {
              day_listwheel = 27;
            }
              WM_HideWindow(_aWheel[0].hWin);
              WM_HideWindow(_aWheel[5].hWin);
              WM_HideWindow(_aWheel[6].hWin);
              WM_ShowWindow(_aWheel[7].hWin);
              LISTWHEEL_SetSel(_aWheel[month].hWin, day_listwheel);
              LISTWHEEL_MoveToPos(_aWheel[7].hWin, day_listwheel);
            }
            else
            {
            if((day_listwheel == 30) || (day_listwheel == 29))
            {
              day_listwheel = 28;
            }
              month = 6;
              WM_HideWindow(_aWheel[0].hWin);
              WM_HideWindow(_aWheel[5].hWin);
              WM_HideWindow(_aWheel[7].hWin);
              WM_ShowWindow(_aWheel[6].hWin);
              LISTWHEEL_MoveToPos(_aWheel[6].hWin, day_listwheel);
              LISTWHEEL_SetSel(_aWheel[month].hWin, day_listwheel);
            }
          }
          else
          {
            month = 0;
            WM_HideWindow(_aWheel[5].hWin);
            WM_HideWindow(_aWheel[6].hWin);
            WM_HideWindow(_aWheel[7].hWin);
            WM_ShowWindow(_aWheel[0].hWin);
            LISTWHEEL_MoveToPos(_aWheel[0].hWin, day_listwheel);
            LISTWHEEL_SetSel(_aWheel[month].hWin, day_listwheel);
          }

          Date.Date = day_listwheel + 1;
          Date.Month = month_listwheel + 1;
          Date.Year = year_listwheel;
          break;
      }
      break;
    default:
      WM_DefaultProc(pMsg);
      break;
  }
}

/**
  * @brief  callback for Clock Settings
  * @param  pMsg: Pointer to Date structure
  * @retval None
  */

static void _cbClockAndDate(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int Id, NCode;
  uint32_t leap_year = 0;
  uint32_t day_nb = 0;
  static uint8_t TempStr[150];
  
  hItem = pMsg->hWin;
  switch (pMsg->MsgId) {
    
  case WM_TIMER:
    k_GetTime(&Time);
    k_GetDate(&Date);
    WM_InvalidateWindow(pMsg->hWin);
    WM_RestartTimer(pMsg->Data.v, 1000);
    break; 
    
  case WM_DELETE:
    WM_DeleteTimer(hTimerRTCSync);
    break;
    
  case WM_CREATE:
    hTimerRTCSync = WM_CreateTimer(pMsg->hWin, 0, 10, 0);

    hItem = BUTTON_CreateEx(440, 5, 35, 30, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_EXIT);
    WM_SetCallback(hItem, _cbButton_exit);

    hItem = BUTTON_CreateEx(420, 157, 58, 57, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_ALARM);
    WM_SetCallback(hItem, _cbButton_alarm);   
    
    hItem = BUTTON_CreateEx(420, 215, 58, 57, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_SETTINGS);
    WM_SetCallback(hItem, _cbButton_settings);             
    break;
    
  case WM_PAINT:
    GUI_DrawBitmap(background_clock[clock_index], 0, 0);
    GUI_DrawBitmap(bmClock_choose[clock_index], 165, 66);

    GUI_DrawBitmap(&bmweather_cloud_res, 55, 20);
    GUI_DrawBitmap(&bmweather_cloud_thunder_res, 55, 55); 
    GUI_DrawBitmap(&bmweather_cloud_rain_res, 55, 90);
    GUI_DrawBitmap(&bmweather_cloud_thunder_sun_res, 55, 125);
    GUI_DrawBitmap(&bmweather_cloud_sun_res, 55, 160);
    GUI_DrawBitmap(&bmweather_sun_res, 55, 195);
    GUI_DrawBitmap(&bmweather_cloud_sun_res, 55, 230);
    
    GUI_SetTextMode(GUI_TM_TRANS);
    GUI_SetFont(&GUI_Font16_ASCII);    
    GUI_DispStringAt("MON", 20, 20);
    GUI_DispStringAt("TUE", 20, 55);
    GUI_DispStringAt("WED", 20, 90);
    GUI_DispStringAt("THU", 20, 125);
    GUI_DispStringAt("FRI", 20, 160);
    GUI_DispStringAt("SAT", 20, 195);
    GUI_DispStringAt("SUN", 20, 230);
    
    if(day_listwheel >= GetNumberOfDays(_apMonth[month_listwheel], (year_listwheel + 2)))
    {
      day_listwheel = GetNumberOfDays(_apMonth[month_listwheel], (year_listwheel + 2)) - 1;
    }  
    
    leap_year = ((year_listwheel + 2)/4);

    if(((year_listwheel + 2)%4) == 0)
    {
      if((month_listwheel > 1))
      {
        leap_year = ((year_listwheel + 2)/4);
      }
      else
      {
        leap_year = ((year_listwheel + 2)/4) - 1;
      }
    }
    
    day_nb = (year_listwheel*365) + _apDayMonth[month_listwheel - 1] + day_listwheel + leap_year;
    day_nb = day_nb%7;
    
    GUI_DrawBitmap(&bmweather_shape_res, 10, 8+(35*day_nb));

    GUI_SetFont(&GUI_FontArialRoundedBold20);
    sprintf((char *)TempStr, "%02d:%02d", Time.Hours , Time.Minutes);
    
    if(clock_index == 0)
    {
      GUI_UpdateClock (240, 139, Time.Hours, Time.Minutes, Time.Seconds);
      GUI_SetColor(0xFF1A468C);      
      GUI_AA_FillCircle(240, 139, 5);
      GUI_SetColor(GUI_WHITE);
      GUI_DispStringHCenterAt((char *)TempStr, 240, 40);
    }
    else if(clock_index == 1)
    {
      GUI_UpdateClock2 (240, 139, Time.Hours, Time.Minutes, Time.Seconds);
      GUI_SetColor(0xFF555555);
      GUI_AA_FillCircle(240, 139, 5);
      GUI_SetColor(GUI_WHITE);
      GUI_DispStringHCenterAt((char *)TempStr, 240, 40);
    }
    else
    {
      GUI_DispStringAt((char *)TempStr, 220, 130);
    }    
         
    GUI_SetColor(GUI_WHITE);
    GUI_SetFont(&GUI_FontArialRoundedBold20);
    
    if(Date.Year > GUI_COUNTOF(_apYear))
      Date.Year = 27;
    
    sprintf((char *)TempStr, "%s %s %s", _apDay[Date.Date - 1], _apMonth[Date.Month - 1], _apYear[Date.Year]);
    
    GUI_DispStringHCenterAt((char *)TempStr, 240, 240);
    
    break;
     
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);    /* Id of widget */
    NCode = pMsg->Data.v;               /* Notification code */
    
    switch (NCode) 
    {
      case WM_NOTIFICATION_CLICKED:
        if(Id == ID_BUTTON_SETTINGS)
        {
          WM_DeleteTimer(hTimerRTCSync);
          WM_ShowWindow(hChoose_Clock);
        }
        if(Id == ID_BUTTON_EXIT)
        {
          WM_DeleteWindow(pMsg->hWin);
        }
        if(Id == ID_BUTTON_ALARM)
        {      
          WM_ShowWindow(hAlarm_Clock);
        }        
        break;
      default:
        break;
    }          
    break;    
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/**
  * @brief  callback for alarm alert window
  * @param  pMsg: Pointer to message structure
  * @retval None
  */
static void _cbAlarmAlert(WM_MESSAGE * pMsg) 
{
  static char TempStr[20];
  int Id;
  static int anim = 0;
  WM_HWIN hItem;
  
  switch (pMsg->MsgId)
  {
  case WM_TIMER:
    
    if (anim > 1)
    {
      anim = 0;
    }      
    else
    {
      anim++;
    }
    WM_InvalidateWindow(pMsg->hWin);
    WM_RestartTimer(pMsg->Data.v, 100);
    
    break;

    case WM_DELETE:
      WM_DeleteTimer(hTimerAlarm);
      break;

    case WM_CREATE:
      hItem = BUTTON_CreateEx(440, 5, 35, 30, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_EXIT);
      WM_SetCallback(hItem, _cbButton_exit);
      hTimerAlarm = WM_CreateTimer(pMsg->hWin, 0, 100, 0);

      hItem = TEXT_CreateEx(180, 40, 250, 35, pMsg->hWin, WM_CF_SHOW, 0, ID_CLOCK_WAKEUP, "TIME TO WAKEUP");
      TEXT_SetFont(hItem, &GUI_FontArialRoundedBold24);
      TEXT_SetTextColor(hItem, GUI_WHITE);

      hItem = TEXT_CreateEx(60, 225, 380, 60, pMsg->hWin, WM_CF_SHOW, 0, ID_CLOCK_STOP, "HOLD TO STOP ALARM");
      TEXT_SetFont(hItem, &GUI_FontArialRoundedBold20);
      TEXT_SetTextColor(hItem, GUI_WHITE);

      hItem = BUTTON_CreateEx(50, 220, 380, 60, pMsg->hWin, WM_CF_SHOW, 0, GUI_ID_BUTTON_ALARM_SNOOZE);
      WM_SetCallback(hItem, _cbButton_hold);
           
      break;

    case WM_PAINT:
      GUI_DrawBitmap(background_clock[clock_index], 0, 0);
      GUI_DrawBitmap(&bmLABEL_head, 50, -100);

      if(clock_index == 0)
      {
        GUI_DrawBitmap(AlarmAnim1[anim], 165, 76);
        GUI_UpdateClock (240, 135, Time.Hours, Time.Minutes, Time.Seconds);
      }
      else if(clock_index == 1)
      {
        GUI_DrawBitmap(AlarmAnim2[anim], 165, 76);
        GUI_UpdateClock2 (240, 135, Time.Hours, Time.Minutes, Time.Seconds);
      }
      else
      {
        GUI_DrawBitmap(AlarmAnim3[anim], 165, 76);
        GUI_SetTextMode(GUI_TM_TRANS);
        GUI_SetFont(&GUI_FontArial18);
        GUI_SetColor(GUI_WHITE);

        sprintf((char *)TempStr, "%02d %02d", Time.Hours , Time.Minutes);
        GUI_DispStringAt((char *)TempStr, 200, 130);
      }
      break;
      
    case WM_TOUCH:
      break;

    case WM_NOTIFY_PARENT:
      Id    = WM_GetId(pMsg->hWinSrc);    /* Id of widget */
      switch (pMsg->Data.v) 
      {
        
      case WM_NOTIFICATION_RELEASED:
        if(Id == GUI_ID_BUTTON_ALARM_SNOOZE)
        {
          WM_DeleteWindow(pMsg->hWin);
        }
        if(Id == ID_BUTTON_EXIT)
        {      
          WM_DeleteWindow(pMsg->hWin);
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
  * @brief  Analog clock window Startup
  * @param  hWin: pointer to the parent handle.
  * @param  xpos: X position 
  * @param  ypos: Y position
  * @retval None
  */
static void Startup(WM_HWIN hWin, uint16_t xpos, uint16_t ypos)
{
  settings          = 2;
  new_settings      = 0;
  setHours          = 0;
  setMinutes        = 0;
  clock_index       = 0;
  day_listwheel     = 13;
  month_listwheel   = 3;
  year_listwheel    = 27;

  /* Initialise the alarms table */
  memset((void *)AlarmTable, 0x0, sizeof(AlarmTable));

  /* Get the current Time and Date values */
  k_GetTime(&Time);
  k_GetDate(&Date);

  /* Initialise the alarmOnRTC structure */
  AlarmOnRTC.IndexAlarmOnRTC = (-1);
  AlarmOnRTC.Alarm_A_IsSet = 0;
  AlarmOnRTC.Alarm_B_IsSet = 0; 

  /* Create Main window */
  hWinClockAndDate  = WM_CreateWindowAsChild(xpos, ypos, 480, 272, hWin, WM_CF_SHOW, _cbClockAndDate, 0);

  /* Create different windows to be used */
  hChoose_Clock     = WM_CreateWindowAsChild(0, 0, 480, 272,hWinClockAndDate, WM_CF_HIDE, _cbChooseClock , 0);
  hTime_Clock       = WM_CreateWindowAsChild(0, 0, 480, 272, hWinClockAndDate, WM_CF_HIDE, _cbTimeClock , 0);
  hDate_Clock       = WM_CreateWindowAsChild(0, 0, 480, 272,hWinClockAndDate, WM_CF_HIDE, _cbDateClock , 0);
  hAlarm_Clock      = WM_CreateWindowAsChild(0, 0, 480, 272,hWinClockAndDate, WM_CF_HIDE, _cbAlarmClock , 0);
  hAlarmAlert       = WM_CreateWindowAsChild(0, 0, 480, 272, WM_HBKWIN, WM_CF_HIDE | WM_CF_STAYONTOP, _cbAlarmAlert, 0);
}

/**
  * @}
  */

/**
  * @}
  */


