/**
  ******************************************************************************
  * @file    BSP/CM7/Src/touchscreen.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use the touchscreen driver.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32H7xx_HAL_Examples
  * @{
  */

/** @addtogroup BSP
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define  CIRCLE_RADIUS        40
#define  LINE_LENGHT          30
/* Private macro -------------------------------------------------------------*/
#define  CIRCLE_XPOS(i)       ((i * 800) / 5)
#define  CIRCLE_YPOS(i)       (480 - CIRCLE_RADIUS - 60)
#if (USE_TS_MULTI_TOUCH == 1)
static uint32_t touchscreen_color_idx = 0;
#endif /* USE_TS_MULTI_TOUCH */

/* Private Structures and Enumerations ------------------------------------------------------------*/
/* Possible allowed indexes of touchscreen demo */
typedef enum
{
  TOUCHSCREEN_DEMO_1 = 1,
  TOUCHSCREEN_DEMO_2 = 2,
  TOUCHSCREEN_DEMO_3 = 3,
  TOUCHSCREEN_DEMO_MAX = TOUCHSCREEN_DEMO_3,

} TouchScreenDemoTypeDef;

/* Table for touchscreen event information display on LCD : table indexed on enum @ref TS_TouchEventTypeDef information */
char * ts_event_string_tab[4] = { "None",
"Press down",
"Lift up",
"Contact"};

/* Table for touchscreen gesture Id information display on LCD : table indexed on enum @ref TS_GestureIdTypeDef information */
char * ts_gesture_id_string_tab[7] = { "No Gesture",
"Move Up",
"Move Right",
"Move Down",
"Move Left",
"Zoom In",
"Zoom Out"};

/* Global variables ---------------------------------------------------------*/
uint32_t TouchScreenDemo = 0;
uint16_t x_new_pos = 0, x_previous_pos = 0;
uint16_t y_new_pos = 0, y_previous_pos = 0;
TS_State_t  TS_State = {0};
TS_MultiTouch_State_t  TS_MTState = {0};
uint32_t colors[24] = {GUI_COLOR_BLUE, GUI_COLOR_GREEN, GUI_COLOR_RED, GUI_COLOR_CYAN, GUI_COLOR_MAGENTA, GUI_COLOR_YELLOW,
GUI_COLOR_LIGHTBLUE, GUI_COLOR_LIGHTGREEN, GUI_COLOR_LIGHTRED, GUI_COLOR_WHITE, GUI_COLOR_LIGHTMAGENTA,
GUI_COLOR_LIGHTYELLOW, GUI_COLOR_DARKBLUE, GUI_COLOR_DARKGREEN, GUI_COLOR_DARKRED, GUI_COLOR_DARKCYAN,
GUI_COLOR_DARKMAGENTA, GUI_COLOR_DARKYELLOW, GUI_COLOR_LIGHTGRAY, GUI_COLOR_GRAY, GUI_COLOR_DARKGRAY,
GUI_COLOR_BLACK, GUI_COLOR_BROWN, GUI_COLOR_ORANGE };

/* Private variables ---------------------------------------------------------*/
/* Static variable holding the current touch color index : used to change color at each touch */
TS_Init_t *hTS;
TS_Gesture_Config_t GestureConf;
extern __IO uint32_t ButtonState;
/* Private function prototypes -----------------------------------------------*/
static void     Touchscreen_SetHint_Demo(TouchScreenDemoTypeDef demoIndex);
#if (USE_TS_MULTI_TOUCH == 1)
static uint32_t Touchscreen_Handle_NewTouch(void);
static void TS_Update(void);
#endif /* USE_TS_MULTI_TOUCH */

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Touchscreen Demo1 : test touchscreen calibration and single touch in polling mode
  * @param  None
  * @retval None
  */
void Touchscreen_demo1(void)
{
  uint16_t x1, y1;
  uint8_t state = 0;
  uint32_t ts_status = BSP_ERROR_NONE;
  uint32_t x_size, y_size;

  BSP_LCD_GetXSize(0, &x_size);
  BSP_LCD_GetYSize(0, &y_size);
  ButtonState = 0;

  hTS->Width = x_size;
  hTS->Height = y_size;
  hTS->Orientation = TS_SWAP_XY | TS_SWAP_Y;
  hTS->Accuracy = 0;

  /* Touchscreen initialization */
  ts_status = BSP_TS_Init(0, hTS);

  if(ts_status == BSP_ERROR_NONE)
  {
    /* Display touch screen demo description */
    Touchscreen_SetHint_Demo(TOUCHSCREEN_DEMO_1);
    Touchscreen_DrawBackground_Circles(state);

    while (1)
    {
      /* Check in polling mode in touch screen the touch status and coordinates */
      /* of touches if touch occurred                                           */
      ts_status = BSP_TS_GetState(0, &TS_State);
      if(TS_State.TouchDetected)
      {
        /* One or dual touch have been detected          */
        /* Only take into account the first touch so far */

        /* Get X and Y position of the first touch post calibrated */
        x1 = TS_State.TouchX;
        y1 = TS_State.TouchY;

        if ((y1 > (CIRCLE_YPOS(1) - CIRCLE_RADIUS)) &&
            (y1 < (CIRCLE_YPOS(1) + CIRCLE_RADIUS)))
        {
          if ((x1 > (CIRCLE_XPOS(1) - CIRCLE_RADIUS)) &&
              (x1 < (CIRCLE_XPOS(1) + CIRCLE_RADIUS)))
          {
            if ((state & 1) == 0)
            {
              Touchscreen_DrawBackground_Circles(state);
              GUI_FillCircle(CIRCLE_XPOS(1), CIRCLE_YPOS(1), CIRCLE_RADIUS, GUI_COLOR_BLUE);
              state = 1;
            }
          }
          if ((x1 > (CIRCLE_XPOS(2) - CIRCLE_RADIUS)) &&
              (x1 < (CIRCLE_XPOS(2) + CIRCLE_RADIUS)))
          {
            if ((state & 2) == 0)
            {
              Touchscreen_DrawBackground_Circles(state);
              GUI_FillCircle(CIRCLE_XPOS(2), CIRCLE_YPOS(2), CIRCLE_RADIUS, GUI_COLOR_RED);
              state = 2;
            }
          }

          if ((x1 > (CIRCLE_XPOS(3) - CIRCLE_RADIUS)) &&
              (x1 < (CIRCLE_XPOS(3) + CIRCLE_RADIUS)))
          {
            if ((state & 4) == 0)
            {
              Touchscreen_DrawBackground_Circles(state);
              GUI_FillCircle(CIRCLE_XPOS(3), CIRCLE_YPOS(3), CIRCLE_RADIUS, GUI_COLOR_YELLOW);
              state = 4;
            }
          }

          if ((x1 > (CIRCLE_XPOS(4) - CIRCLE_RADIUS)) &&
              (x1 < (CIRCLE_XPOS(4) + CIRCLE_RADIUS)))
          {
            if ((state & 8) == 0)
            {
              Touchscreen_DrawBackground_Circles(state);
              GUI_FillCircle(CIRCLE_XPOS(4), CIRCLE_YPOS(3), CIRCLE_RADIUS, GUI_COLOR_GREEN);
              state = 8;
            }
          }
        }

      }
      if(CheckForUserInput() > 0)
      {

        BSP_TS_DeInit(0);

        ButtonState = 0;
        return;
      }

      HAL_Delay(20);
    }
  }
}

#if (USE_TS_MULTI_TOUCH == 1)
/**
  * @brief  Touchscreen Demo2 : test touchscreen single and dual touch in polling mode
  * @param  None
  * @retval None
  */
void Touchscreen_demo2(void)
{
  uint32_t ts_status = BSP_ERROR_NONE;
  uint32_t x_size, y_size;

  BSP_LCD_GetXSize(0, &x_size);
  BSP_LCD_GetYSize(0, &y_size);

  hTS->Width = x_size;
  hTS->Height = y_size;
  hTS->Orientation = TS_SWAP_XY | TS_SWAP_Y;
  hTS->Accuracy = 5;

  GestureConf.Radian = 0x0A;
  GestureConf.OffsetLeftRight = 0x19;
  GestureConf.OffsetUpDown = 0x19;
  GestureConf.DistanceLeftRight = 0x19;
  GestureConf.DistanceUpDown = 0x19;
  GestureConf.DistanceZoom = 0x32;

  /* Touchscreen initialization */
  ts_status = BSP_TS_Init(0, hTS);
  ts_status = BSP_TS_GestureConfig(0, &GestureConf);

  if(ts_status == BSP_ERROR_NONE)
  {
    /* Display touch screen demo2 description */
    Touchscreen_SetHint_Demo(TOUCHSCREEN_DEMO_2);
  } /* of if(status == BSP_ERROR_NONE) */

  while (1)
  {
    Touchscreen_Handle_NewTouch();

    if(CheckForUserInput() > 0)
    {
      ButtonState = 0;

      return;
    }
    HAL_Delay(100);
  }
}


void BSP_TS_Callback(uint32_t Instance)
{
  TS_Update();
}

static void TS_Update(void)
{
  uint16_t i, j;

  BSP_TS_GetState(0, &TS_State);
  if(TS_State.TouchDetected)
  {
    /* One or dual touch have been detected          */
    /* Only take into account the first touch so far */

    /* Get X and Y position of the first touch post calibrated */
    x_new_pos = TS_State.TouchX;
    y_new_pos = TS_State.TouchY;

    for(i = 0; i < hTS->Width/40; i++)
    {
      for(j = 0; j < hTS->Height/40; j++)
      {
        if(((x_new_pos > 40*i) && (x_new_pos < 40*(i+1))) && ((y_new_pos > 40*j) && (y_new_pos < 40*(j+1))))
        {
          GUI_FillRect(x_previous_pos, y_previous_pos,40,40, GUI_COLOR_WHITE);
          GUI_DrawRect(x_previous_pos, y_previous_pos,40,40, GUI_COLOR_BLACK);

          GUI_FillRect(40*i, 40*j,40,40, colors[(touchscreen_color_idx++ % 24)]);

          x_previous_pos = 40*i;
          y_previous_pos = 40*j;
          break;
        }
      }
    }
  }
}

/**
  * @brief  Touchscreen_Handle_NewTouch: factorization of touch management
  * @param  None
  * @retval BSP_ERROR_NONE
  */
static uint32_t Touchscreen_Handle_NewTouch(void)
{
  uint32_t GestureId = GESTURE_ID_NO_GESTURE;
#define TS_MULTITOUCH_FOOTPRINT_CIRCLE_RADIUS 15
#define TOUCH_INFO_STRING_SIZE                70
  uint16_t x1 = 0;
  uint16_t y1 = 0;
  uint16_t x2 = 0;
  uint16_t y2 = 0;
  uint32_t drawTouch1 = 0; /* activate/deactivate draw of footprint of touch 1 */
  uint32_t drawTouch2 = 0; /* activate/deactivate draw of footprint of touch 2 */
  uint32_t colors[24] = {GUI_COLOR_BLUE, GUI_COLOR_GREEN, GUI_COLOR_RED, GUI_COLOR_CYAN, GUI_COLOR_MAGENTA, GUI_COLOR_YELLOW,
  GUI_COLOR_LIGHTBLUE, GUI_COLOR_LIGHTGREEN, GUI_COLOR_LIGHTRED, GUI_COLOR_LIGHTCYAN, GUI_COLOR_LIGHTMAGENTA,
  GUI_COLOR_LIGHTYELLOW, GUI_COLOR_DARKBLUE, GUI_COLOR_DARKGREEN, GUI_COLOR_DARKRED, GUI_COLOR_DARKCYAN,
  GUI_COLOR_DARKMAGENTA, GUI_COLOR_DARKYELLOW, GUI_COLOR_LIGHTGRAY, GUI_COLOR_GRAY, GUI_COLOR_DARKGRAY,
  GUI_COLOR_BLACK, GUI_COLOR_BROWN, GUI_COLOR_ORANGE };
  uint32_t ts_status = BSP_ERROR_NONE;
  uint8_t lcd_string[TOUCH_INFO_STRING_SIZE] = "";
  uint32_t x_size, y_size;

  BSP_LCD_GetXSize(0, &x_size);
  BSP_LCD_GetYSize(0, &y_size);

  /* Check in polling mode in touch screen the touch status and coordinates */
  /* of touches if touch occurred                                           */
  ts_status = BSP_TS_Get_MultiTouchState(0, &TS_MTState);
  if(TS_MTState.TouchDetected)
  {
    /* One or dual touch have been detected  */

    /* Erase previous information on touchscreen play pad area */
    GUI_FillRect(0, 80, x_size, y_size - 160, GUI_COLOR_WHITE);

    /* Re-Draw touch screen play area on LCD */
    GUI_DrawRect(10, 90, x_size - 20, y_size - 180, GUI_COLOR_BLUE);
    GUI_DrawRect(11, 91, x_size - 22, y_size - 182, GUI_COLOR_BLUE);

    /* Erase previous information on bottom text bar */
    GUI_FillRect(0, y_size - 80, x_size, 80, GUI_COLOR_BLUE);

    /* Desactivate drawing footprint of touch 1 and touch 2 until validated against boundaries of touch pad values */
    drawTouch1 = drawTouch2 = 0;

    /* Get X and Y position of the first touch post calibrated */
    x1 = TS_MTState.TouchX[0];
    y1 = TS_MTState.TouchY[0];

    if((y1 > (90 + TS_MULTITOUCH_FOOTPRINT_CIRCLE_RADIUS)) &&
       (y1 < (y_size - 90 - TS_MULTITOUCH_FOOTPRINT_CIRCLE_RADIUS)))
    {
      drawTouch1 = 1;
    }

    /* If valid touch 1 position : inside the reserved area for the use case : draw the touch */
    if(drawTouch1 == 1)
    {
      /* Draw circle of first touch : turn on colors[] table */
      GUI_FillCircle(x1, y1, TS_MULTITOUCH_FOOTPRINT_CIRCLE_RADIUS, colors[(touchscreen_color_idx++ % 24)]);

      GUI_SetTextColor(GUI_COLOR_WHITE);
      GUI_SetFont(&Font16);
      GUI_DisplayStringAt(0, y_size - 70, (uint8_t *)"TOUCH INFO : ", CENTER_MODE);

      GUI_SetFont(&Font12);
      sprintf((char*)lcd_string, "x1 = %u, y1 = %u, Event = %s, Weight = %lu",
              x1,
              y1,
              ts_event_string_tab[TS_MTState.TouchEvent[0]],
              TS_MTState.TouchWeight[0]);
      GUI_DisplayStringAt(0, y_size - 45, lcd_string, CENTER_MODE);
    } /* of if(drawTouch1 == 1) */

    if(TS_MTState.TouchDetected > 1)
    {
      /* Get X and Y position of the second touch post calibrated */
      x2 = TS_MTState.TouchX[1];
      y2 = TS_MTState.TouchY[1];

      if((y2 > (90 + TS_MULTITOUCH_FOOTPRINT_CIRCLE_RADIUS)) &&
         (y2 < (y_size - 90 - TS_MULTITOUCH_FOOTPRINT_CIRCLE_RADIUS)))
      {
        drawTouch2 = 1;
      }

      /* If valid touch 2 position : inside the reserved area for the use case : draw the touch */
      if(drawTouch2 == 1)
      {
        sprintf((char*)lcd_string, "x2 = %u, y2 = %u, Event = %s, Weight = %lu",
                x2,
                y2,
                ts_event_string_tab[TS_MTState.TouchEvent[1]],
                TS_MTState.TouchWeight[1]);
        GUI_DisplayStringAt(0, y_size - 35, lcd_string, CENTER_MODE);

        /* Draw circle of second touch : turn on color[] table */
        GUI_FillCircle(x2, y2, TS_MULTITOUCH_FOOTPRINT_CIRCLE_RADIUS, colors[(touchscreen_color_idx++ % 24)]);
      } /* of if(drawTouch2 == 1) */

    } /* of if(TS_MTState.TouchDetected > 1) */

    if((drawTouch1 == 1) || (drawTouch2 == 1))
    {
      /* Get updated gesture Id in global variable 'TS_State' */
      ts_status = BSP_TS_GetGestureId(0, &GestureId);

      sprintf((char*)lcd_string, "Gesture Id = %s", ts_gesture_id_string_tab[GestureId]);
    }
    else
    {
      /* Invalid touch position */
      GUI_SetTextColor(GUI_COLOR_WHITE);
      GUI_DisplayStringAt(0, y_size - 70, (uint8_t *)"Invalid touch position : use drawn touch area : ", CENTER_MODE);
    }
  } /* of if(TS_MTState.TouchDetected) */

  return(ts_status);
}

#endif /* USE_TS_MULTI_TOUCH == 1 */

/**
  * @brief  Display TS Demo Hint for all touchscreen demos depending on passed
  *         demoIndex in parameter.
  * @param  demoIndex : parameter of type @ref TouchScreenDemoTypeDef
  * @retval None
  */
static void Touchscreen_SetHint_Demo(TouchScreenDemoTypeDef demoIndex)
{
  uint32_t x_size, y_size;

  BSP_LCD_GetXSize(0, &x_size);
  BSP_LCD_GetYSize(0, &y_size);

  if(demoIndex <= TOUCHSCREEN_DEMO_MAX)
  {
    /* Clear the LCD */
    GUI_Clear(GUI_COLOR_WHITE);

    /* Set Touchscreen Demo1 description */
    GUI_FillRect(0, 0, x_size, 80, GUI_COLOR_BLUE);
    GUI_SetTextColor(GUI_COLOR_WHITE);
    GUI_SetBackColor(GUI_COLOR_BLUE);
    GUI_SetFont(&Font24);

    if(demoIndex == TOUCHSCREEN_DEMO_1)
    {
      GUI_DisplayStringAt(0, 0, (uint8_t *)"Touchscreen basic polling", CENTER_MODE);
      GUI_SetFont(&Font12);
      GUI_DisplayStringAt(0, 30, (uint8_t *)"Please use the Touchscreen to", CENTER_MODE);
      GUI_DisplayStringAt(0, 45, (uint8_t *)"activate the colored circle", CENTER_MODE);
      GUI_DisplayStringAt(0, 60, (uint8_t *)"inside the rectangle. Then press User button", CENTER_MODE);
    }
    else if (demoIndex == TOUCHSCREEN_DEMO_2)
    {
      GUI_DisplayStringAt(0, 0, (uint8_t *)"Touchscreen dual touch polling", CENTER_MODE);
      GUI_SetFont(&Font12);
      GUI_DisplayStringAt(0, 30, (uint8_t *)"Please press the Touchscreen to", CENTER_MODE);
      GUI_DisplayStringAt(0, 45, (uint8_t *)"activate single and", CENTER_MODE);
      GUI_DisplayStringAt(0, 60, (uint8_t *)"dual touch", CENTER_MODE);

    }
    else /* demoIndex == TOUCHSCREEN_DEMO_3 */
    {
      GUI_DisplayStringAt(0, 0, (uint8_t *)"Touchscreen dual touch interrupt", CENTER_MODE);
      GUI_SetFont(&Font12);
      GUI_DisplayStringAt(0, 30, (uint8_t *)"Please press the Touchscreen to", CENTER_MODE);
      GUI_DisplayStringAt(0, 45, (uint8_t *)"activate single and", CENTER_MODE);
      GUI_DisplayStringAt(0, 60, (uint8_t *)"dual touch", CENTER_MODE);
    }

    GUI_DrawRect(10, 90, x_size - 20, y_size - 100, GUI_COLOR_BLUE);
    GUI_DrawRect(11, 91, x_size - 22, y_size - 102, GUI_COLOR_BLUE);

  } /* of if(demoIndex <= TOUCHSCREEN_DEMO_MAX) */
}

/**
  * @brief  Draw Touchscreen Background
  * @param  state : touch zone state
  * @retval None
  */
void Touchscreen_DrawBackground_Circles(uint8_t state)
{
  uint16_t x, y;
  switch (state)
  {
  case 0:
    GUI_FillCircle(CIRCLE_XPOS(1), CIRCLE_YPOS(1), CIRCLE_RADIUS, GUI_COLOR_BLUE);

    GUI_FillCircle(CIRCLE_XPOS(2), CIRCLE_YPOS(2), CIRCLE_RADIUS, GUI_COLOR_RED);

    GUI_FillCircle(CIRCLE_XPOS(3), CIRCLE_YPOS(3), CIRCLE_RADIUS, GUI_COLOR_YELLOW);


    GUI_FillCircle(CIRCLE_XPOS(4), CIRCLE_YPOS(3), CIRCLE_RADIUS, GUI_COLOR_GREEN);

    GUI_FillCircle(CIRCLE_XPOS(1), CIRCLE_YPOS(1), CIRCLE_RADIUS - 2, GUI_COLOR_WHITE);
    GUI_FillCircle(CIRCLE_XPOS(2), CIRCLE_YPOS(2), CIRCLE_RADIUS - 2, GUI_COLOR_WHITE);
    GUI_FillCircle(CIRCLE_XPOS(3), CIRCLE_YPOS(3), CIRCLE_RADIUS - 2, GUI_COLOR_WHITE);
    GUI_FillCircle(CIRCLE_XPOS(4), CIRCLE_YPOS(3), CIRCLE_RADIUS - 2, GUI_COLOR_WHITE);
    break;

  case 1:
    GUI_FillCircle(CIRCLE_XPOS(1), CIRCLE_YPOS(1), CIRCLE_RADIUS, GUI_COLOR_BLUE);
    GUI_FillCircle(CIRCLE_XPOS(1), CIRCLE_YPOS(1), CIRCLE_RADIUS - 2, GUI_COLOR_WHITE);
    break;

  case 2:
    GUI_FillCircle(CIRCLE_XPOS(2), CIRCLE_YPOS(2), CIRCLE_RADIUS, GUI_COLOR_RED);
    GUI_FillCircle(CIRCLE_XPOS(2), CIRCLE_YPOS(2), CIRCLE_RADIUS - 2, GUI_COLOR_WHITE);
    break;

  case 4:
    GUI_FillCircle(CIRCLE_XPOS(3), CIRCLE_YPOS(3), CIRCLE_RADIUS, GUI_COLOR_YELLOW);
    GUI_FillCircle(CIRCLE_XPOS(3), CIRCLE_YPOS(3), CIRCLE_RADIUS - 2, GUI_COLOR_WHITE);
    break;

  case 8:
    GUI_FillCircle(CIRCLE_XPOS(4), CIRCLE_YPOS(4), CIRCLE_RADIUS, GUI_COLOR_GREEN);
    GUI_FillCircle(CIRCLE_XPOS(4), CIRCLE_YPOS(4), CIRCLE_RADIUS - 2, GUI_COLOR_WHITE);
    break;

  case 16:
    GUI_FillCircle(CIRCLE_XPOS(1), CIRCLE_YPOS(1), CIRCLE_RADIUS, GUI_COLOR_BLUE);
    GUI_FillCircle(CIRCLE_XPOS(2), CIRCLE_YPOS(2), CIRCLE_RADIUS, GUI_COLOR_BLUE);
    GUI_FillCircle(CIRCLE_XPOS(3), CIRCLE_YPOS(3), CIRCLE_RADIUS, GUI_COLOR_BLUE);
    GUI_FillCircle(CIRCLE_XPOS(4), CIRCLE_YPOS(3), CIRCLE_RADIUS, GUI_COLOR_BLUE);

    GUI_FillCircle(CIRCLE_XPOS(1), CIRCLE_YPOS(1), CIRCLE_RADIUS - 2, GUI_COLOR_WHITE);
    GUI_FillCircle(CIRCLE_XPOS(2), CIRCLE_YPOS(2), CIRCLE_RADIUS - 2, GUI_COLOR_WHITE);
    GUI_FillCircle(CIRCLE_XPOS(3), CIRCLE_YPOS(3), CIRCLE_RADIUS - 2, GUI_COLOR_WHITE);
    GUI_FillCircle(CIRCLE_XPOS(4), CIRCLE_YPOS(3), CIRCLE_RADIUS - 2, GUI_COLOR_WHITE);

    GUI_DrawHLine(CIRCLE_XPOS(1)-LINE_LENGHT, CIRCLE_YPOS(1), 2*LINE_LENGHT, GUI_COLOR_BLUE);
    GUI_DrawHLine(CIRCLE_XPOS(2)-LINE_LENGHT, CIRCLE_YPOS(2), 2*LINE_LENGHT, GUI_COLOR_BLUE);
    GUI_DrawVLine(CIRCLE_XPOS(2), CIRCLE_YPOS(2)-LINE_LENGHT, 2*LINE_LENGHT, GUI_COLOR_BLUE);
    GUI_DrawHLine(CIRCLE_XPOS(3)-LINE_LENGHT, CIRCLE_YPOS(3), 2*LINE_LENGHT, GUI_COLOR_BLUE);
    GUI_DrawHLine(CIRCLE_XPOS(4)-LINE_LENGHT, CIRCLE_YPOS(4), 2*LINE_LENGHT, GUI_COLOR_BLUE);
    GUI_DrawVLine(CIRCLE_XPOS(4), CIRCLE_YPOS(4)-LINE_LENGHT, 2*LINE_LENGHT, GUI_COLOR_BLUE);

    GUI_SetTextColor(GUI_COLOR_BLUE);
    GUI_SetBackColor(GUI_COLOR_WHITE);
    GUI_SetFont(&Font24);
    x = CIRCLE_XPOS(1);
    y = CIRCLE_YPOS(1) - CIRCLE_RADIUS - GUI_GetFont()->Height;
    GUI_DisplayStringAt(x, y, (uint8_t *)"Volume", CENTER_MODE);
    x = CIRCLE_XPOS(4);
    y = CIRCLE_YPOS(4) - CIRCLE_RADIUS - GUI_GetFont()->Height;
    GUI_DisplayStringAt(x, y, (uint8_t *)"Frequency", CENTER_MODE);

    break;

  case 32:
    GUI_FillCircle(CIRCLE_XPOS(1), CIRCLE_YPOS(1), CIRCLE_RADIUS, GUI_COLOR_BLACK);
    GUI_FillCircle(CIRCLE_XPOS(2), CIRCLE_YPOS(2), CIRCLE_RADIUS, GUI_COLOR_BLACK);

    GUI_FillCircle(CIRCLE_XPOS(1), CIRCLE_YPOS(1), CIRCLE_RADIUS - 2, GUI_COLOR_WHITE);
    GUI_FillCircle(CIRCLE_XPOS(2), CIRCLE_YPOS(2), CIRCLE_RADIUS - 2, GUI_COLOR_WHITE);


    GUI_SetTextColor(GUI_COLOR_BLACK);
    GUI_SetBackColor(GUI_COLOR_WHITE);
    GUI_SetFont(&Font20);
    x = CIRCLE_XPOS(1) - 10;
    y = CIRCLE_YPOS(1) - (GUI_GetFont()->Height)/2;
    GUI_DisplayStringAt(x, y, (uint8_t *)"Up", LEFT_MODE);
    x = CIRCLE_XPOS(2) - 10;
    y = CIRCLE_YPOS(3)  - (GUI_GetFont()->Height)/2;
    GUI_DisplayStringAt(x, y, (uint8_t *)"Dw", LEFT_MODE);
    GUI_SetFont(&Font12);

    break;
  }
}

/**
  * @brief  TouchScreen get touch position
  * @param  None
  * @retval None
  */
uint8_t TouchScreen_GetTouchPosition(void)
{
  uint16_t x1, y1;
  uint8_t circleNr = 0;

  /* Check in polling mode in touch screen the touch status and coordinates */
  /* of touches if touch occurred                                           */
  BSP_TS_GetState(0, &TS_State);
  if(TS_State.TouchDetected)
  {
    /* One or dual touch have been detected          */
    /* Only take into account the first touch so far */

    /* Get X and Y position of the first */
    x1 = TS_State.TouchX;
    y1 = TS_State.TouchY;

    if ((y1 > (CIRCLE_YPOS(1) - CIRCLE_RADIUS)) &&
        (y1 < (CIRCLE_YPOS(1) + CIRCLE_RADIUS)))
    {
      if ((x1 > (CIRCLE_XPOS(1) - CIRCLE_RADIUS)) &&
          (x1 < (CIRCLE_XPOS(1) + CIRCLE_RADIUS)))
      {
        circleNr = 1;
      }
      if ((x1 > (CIRCLE_XPOS(2) - CIRCLE_RADIUS)) &&
          (x1 < (CIRCLE_XPOS(2) + CIRCLE_RADIUS)))
      {
        circleNr = 2;
      }

      if ((x1 > (CIRCLE_XPOS(3) - CIRCLE_RADIUS)) &&
          (x1 < (CIRCLE_XPOS(3) + CIRCLE_RADIUS)))
      {
        circleNr = 3;
      }

      if ((x1 > (CIRCLE_XPOS(4) - CIRCLE_RADIUS)) &&
          (x1 < (CIRCLE_XPOS(4) + CIRCLE_RADIUS)))
      {
        circleNr = 4;
      }
    }
    else
    {
      if (((y1 < 220) && (y1 > 140)) &&
          ((x1 < 160) && (x1 > 100)))
      {
        circleNr = 0xFE;   /* top part of the screen */
      }
      else
      {
        circleNr = 0xFF;
      }
    }
  } /* of if(TS_State.TouchDetected) */
  return circleNr;
}
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
