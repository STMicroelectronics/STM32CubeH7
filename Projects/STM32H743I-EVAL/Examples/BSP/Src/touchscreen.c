/**
  ******************************************************************************
  * @file    BSP/Src/touchscreen.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use the touchscreen driver.
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
#include "main.h"

/** @addtogroup STM32H7xx_HAL_Examples
  * @{
  */

/** @addtogroup BSP
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
typedef enum
{
  TOUCHSCREEN_DEMO_1 = 1,
  TOUCHSCREEN_DEMO_2 = 2,
  TOUCHSCREEN_DEMO_MAX = TOUCHSCREEN_DEMO_2,
} TouchScreenDemoTypeDef;
/* Private define ------------------------------------------------------------*/
#define  CIRCLE_RADIUS        40
#define  LINE_LENGHT          30
/* Private macro -------------------------------------------------------------*/
#define  CIRCLE_XPOS(i)       ((i * 640) / 5)
#define  CIRCLE_YPOS(i)       (480 - CIRCLE_RADIUS - 60)
/* Private variables ---------------------------------------------------------*/
uint16_t x_new_pos = 0, x_previous_pos = 0;
uint16_t y_new_pos = 0, y_previous_pos = 0;
TS_State_t  TS_State = {0};
/* Table for touchscreen event information display on LCD : table indexed on enum @ref TS_TouchEventTypeDef information */
char * ts_event_string_tab[4] = { "None",
                                  "Press down",
                                  "Lift up",
                                  "Contact"};
 uint32_t colors[24] = {UTIL_LCD_COLOR_BLUE, UTIL_LCD_COLOR_GREEN, UTIL_LCD_COLOR_RED, UTIL_LCD_COLOR_CYAN, UTIL_LCD_COLOR_MAGENTA, UTIL_LCD_COLOR_YELLOW,
                        UTIL_LCD_COLOR_LIGHTBLUE, UTIL_LCD_COLOR_LIGHTGREEN, UTIL_LCD_COLOR_LIGHTRED, UTIL_LCD_COLOR_WHITE, UTIL_LCD_COLOR_LIGHTMAGENTA,
                        UTIL_LCD_COLOR_LIGHTYELLOW, UTIL_LCD_COLOR_DARKBLUE, UTIL_LCD_COLOR_DARKGREEN, UTIL_LCD_COLOR_DARKRED, UTIL_LCD_COLOR_DARKCYAN,
                        UTIL_LCD_COLOR_DARKMAGENTA, UTIL_LCD_COLOR_DARKYELLOW, UTIL_LCD_COLOR_LIGHTGRAY, UTIL_LCD_COLOR_GRAY, UTIL_LCD_COLOR_DARKGRAY,
                        UTIL_LCD_COLOR_BLACK, UTIL_LCD_COLOR_BROWN, UTIL_LCD_COLOR_ORANGE };
static uint32_t touchscreen_color_idx = 0;
TS_Init_t  hTS;
/* Private function prototypes -----------------------------------------------*/
static void      Touchscreen_SetHint_Demo(TouchScreenDemoTypeDef demoIndex);
static void      TS_Update(void);

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

  hTS.Width = x_size;
  hTS.Height = y_size;
  hTS.Orientation = TS_SWAP_NONE;
  hTS.Accuracy = 5;

  /* Touchscreen initialization */
  ts_status = BSP_TS_Init(0, &hTS);

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
              UTIL_LCD_FillCircle(CIRCLE_XPOS(1), CIRCLE_YPOS(1), CIRCLE_RADIUS, UTIL_LCD_COLOR_BLUE);
              state = 1;
            }
          }
          if ((x1 > (CIRCLE_XPOS(2) - CIRCLE_RADIUS)) &&
              (x1 < (CIRCLE_XPOS(2) + CIRCLE_RADIUS)))
          {
            if ((state & 2) == 0)
            {
              Touchscreen_DrawBackground_Circles(state);
              UTIL_LCD_FillCircle(CIRCLE_XPOS(2), CIRCLE_YPOS(2), CIRCLE_RADIUS, UTIL_LCD_COLOR_RED);
              state = 2;
            }
          }

          if ((x1 > (CIRCLE_XPOS(3) - CIRCLE_RADIUS)) &&
              (x1 < (CIRCLE_XPOS(3) + CIRCLE_RADIUS)))
          {
            if ((state & 4) == 0)
            {
              Touchscreen_DrawBackground_Circles(state);
              UTIL_LCD_FillCircle(CIRCLE_XPOS(3), CIRCLE_YPOS(3), CIRCLE_RADIUS, UTIL_LCD_COLOR_YELLOW);
              state = 4;
            }
          }

          if ((x1 > (CIRCLE_XPOS(4) - CIRCLE_RADIUS)) &&
              (x1 < (CIRCLE_XPOS(4) + CIRCLE_RADIUS)))
          {
            if ((state & 8) == 0)
            {
              Touchscreen_DrawBackground_Circles(state);
              UTIL_LCD_FillCircle(CIRCLE_XPOS(4), CIRCLE_YPOS(3), CIRCLE_RADIUS, UTIL_LCD_COLOR_GREEN);
              state = 8;
            }
          }
        }

      } /* of if(TS_State.TouchDetected) */

      if(CheckForUserInput() > 0)
      {
        BSP_TS_DeInit(0);

        ButtonState = 0;
        return;
      }

      HAL_Delay(20);
    }
  } /* of if(status == BSP_ERROR_NONE) */
}

/**
  * @brief  Touchscreen Demo3 : test touchscreen single touch in interrupt mode
  * @param  None
  * @retval None
  */
void Touchscreen_demo2(void)
{
  uint16_t k, l;

  uint32_t x_size, y_size;
  TSInterruptTest=1;
  ButtonState = 0;
  BSP_LCD_GetXSize(0, &x_size);
  BSP_LCD_GetYSize(0, &y_size);

  hTS.Width = x_size;
  hTS.Height = y_size;
  hTS.Orientation = TS_SWAP_NONE;
  hTS.Accuracy = 5;

  /* Touchscreen initialization */
  BSP_TS_Init(0, &hTS);

  BSP_TS_EnableIT(0);

  UTIL_LCD_Clear(UTIL_LCD_COLOR_WHITE);
  for(k = 0; k < hTS.Width/40; k++)
  {
    for(l = 0; l < hTS.Height/40; l++)
    {
      UTIL_LCD_DrawRect(40*k, 40*l,40,40, UTIL_LCD_COLOR_BLACK);
    }
  }
  ButtonState = 0;
  while (1)
  {
    if(CheckForUserInput() > 0)
    {
      ButtonState = 0;
      TSInterruptTest=0;
      BSP_TS_DisableIT(0);
      BSP_TS_DeInit(0);
      return;
    }
  }
}

/**
  * @brief  BSP TS Callback.
  * @param  Instance  TS instance. Could be only 0.
  * @retval None.
  */
void BSP_TS_Callback(uint32_t Instance)
{
  if(TSInterruptTest == 1)
  {
    TS_Update();
  }
  else
  {
    AudioUpdate = 1;
  }
}

/**
  * @brief  Update display depending on detected touch point
  * @retval None.
  */
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

    for(i = 0; i < hTS.Width/40; i++)
    {
      for(j = 0; j < hTS.Height/40; j++)
      {
        if(((x_new_pos > 40*i) && (x_new_pos < 40*(i+1))) && ((y_new_pos > 40*j) && (y_new_pos < 40*(j+1))))
        {
          UTIL_LCD_FillRect(x_previous_pos, y_previous_pos,40,40, UTIL_LCD_COLOR_WHITE);
          UTIL_LCD_DrawRect(x_previous_pos, y_previous_pos,40,40, UTIL_LCD_COLOR_BLACK);

          UTIL_LCD_FillRect(40*i, 40*j,40,40, colors[(touchscreen_color_idx++ % 24)]);

          x_previous_pos = 40*i;
          y_previous_pos = 40*j;
          break;
        }
      }
    }
  }
}

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
    UTIL_LCD_Clear(UTIL_LCD_COLOR_WHITE);

    /* Set Touchscreen Demo1 description */
    UTIL_LCD_FillRect(0, 0, x_size, 80, UTIL_LCD_COLOR_BLUE);
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
    UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLUE);
    UTIL_LCD_SetFont(&Font24);

    if(demoIndex == TOUCHSCREEN_DEMO_1)
    {
      UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"Touchscreen basic polling", CENTER_MODE);
      UTIL_LCD_SetFont(&Font12);
      UTIL_LCD_DisplayStringAt(0, 30, (uint8_t *)"Please use the Touchscreen to", CENTER_MODE);
      UTIL_LCD_DisplayStringAt(0, 45, (uint8_t *)"activate the colored circle", CENTER_MODE);
      UTIL_LCD_DisplayStringAt(0, 60, (uint8_t *)"inside the rectangle. Then press User button", CENTER_MODE);
    }
    else /* demoIndex == TOUCHSCREEN_DEMO_2 */
    {
      UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"Touchscreen dual touch interrupt", CENTER_MODE);
      UTIL_LCD_SetFont(&Font12);
      UTIL_LCD_DisplayStringAt(0, 30, (uint8_t *)"Please press the Touchscreen to", CENTER_MODE);
      UTIL_LCD_DisplayStringAt(0, 45, (uint8_t *)"activate single and", CENTER_MODE);
      UTIL_LCD_DisplayStringAt(0, 60, (uint8_t *)"dual touch", CENTER_MODE);
    }

    UTIL_LCD_DrawRect(10, 90, x_size - 20, y_size - 100, UTIL_LCD_COLOR_BLUE);
    UTIL_LCD_DrawRect(11, 91, x_size - 22, y_size - 102, UTIL_LCD_COLOR_BLUE);

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
      UTIL_LCD_FillCircle(CIRCLE_XPOS(1), CIRCLE_YPOS(1), CIRCLE_RADIUS, UTIL_LCD_COLOR_BLUE);

      UTIL_LCD_FillCircle(CIRCLE_XPOS(2), CIRCLE_YPOS(2), CIRCLE_RADIUS, UTIL_LCD_COLOR_RED);

      UTIL_LCD_FillCircle(CIRCLE_XPOS(3), CIRCLE_YPOS(3), CIRCLE_RADIUS, UTIL_LCD_COLOR_YELLOW);


      UTIL_LCD_FillCircle(CIRCLE_XPOS(4), CIRCLE_YPOS(3), CIRCLE_RADIUS, UTIL_LCD_COLOR_GREEN);

      UTIL_LCD_FillCircle(CIRCLE_XPOS(1), CIRCLE_YPOS(1), CIRCLE_RADIUS - 2, UTIL_LCD_COLOR_WHITE);
      UTIL_LCD_FillCircle(CIRCLE_XPOS(2), CIRCLE_YPOS(2), CIRCLE_RADIUS - 2, UTIL_LCD_COLOR_WHITE);
      UTIL_LCD_FillCircle(CIRCLE_XPOS(3), CIRCLE_YPOS(3), CIRCLE_RADIUS - 2, UTIL_LCD_COLOR_WHITE);
      UTIL_LCD_FillCircle(CIRCLE_XPOS(4), CIRCLE_YPOS(3), CIRCLE_RADIUS - 2, UTIL_LCD_COLOR_WHITE);
      break;

    case 1:
      UTIL_LCD_FillCircle(CIRCLE_XPOS(1), CIRCLE_YPOS(1), CIRCLE_RADIUS, UTIL_LCD_COLOR_BLUE);
      UTIL_LCD_FillCircle(CIRCLE_XPOS(1), CIRCLE_YPOS(1), CIRCLE_RADIUS - 2, UTIL_LCD_COLOR_WHITE);
      break;

    case 2:
      UTIL_LCD_FillCircle(CIRCLE_XPOS(2), CIRCLE_YPOS(2), CIRCLE_RADIUS, UTIL_LCD_COLOR_RED);
      UTIL_LCD_FillCircle(CIRCLE_XPOS(2), CIRCLE_YPOS(2), CIRCLE_RADIUS - 2, UTIL_LCD_COLOR_WHITE);
      break;

    case 4:
      UTIL_LCD_FillCircle(CIRCLE_XPOS(3), CIRCLE_YPOS(3), CIRCLE_RADIUS, UTIL_LCD_COLOR_YELLOW);
      UTIL_LCD_FillCircle(CIRCLE_XPOS(3), CIRCLE_YPOS(3), CIRCLE_RADIUS - 2, UTIL_LCD_COLOR_WHITE);
      break;

    case 8:
      UTIL_LCD_FillCircle(CIRCLE_XPOS(4), CIRCLE_YPOS(4), CIRCLE_RADIUS, UTIL_LCD_COLOR_GREEN);
      UTIL_LCD_FillCircle(CIRCLE_XPOS(4), CIRCLE_YPOS(4), CIRCLE_RADIUS - 2, UTIL_LCD_COLOR_WHITE);
      break;

    case 16:
      UTIL_LCD_FillCircle(CIRCLE_XPOS(1), CIRCLE_YPOS(1), CIRCLE_RADIUS, UTIL_LCD_COLOR_BLUE);
      UTIL_LCD_FillCircle(CIRCLE_XPOS(2), CIRCLE_YPOS(2), CIRCLE_RADIUS, UTIL_LCD_COLOR_BLUE);
      UTIL_LCD_FillCircle(CIRCLE_XPOS(3), CIRCLE_YPOS(3), CIRCLE_RADIUS, UTIL_LCD_COLOR_BLUE);
      UTIL_LCD_FillCircle(CIRCLE_XPOS(4), CIRCLE_YPOS(3), CIRCLE_RADIUS, UTIL_LCD_COLOR_BLUE);

      UTIL_LCD_FillCircle(CIRCLE_XPOS(1), CIRCLE_YPOS(1), CIRCLE_RADIUS - 2, UTIL_LCD_COLOR_WHITE);
      UTIL_LCD_FillCircle(CIRCLE_XPOS(2), CIRCLE_YPOS(2), CIRCLE_RADIUS - 2, UTIL_LCD_COLOR_WHITE);
      UTIL_LCD_FillCircle(CIRCLE_XPOS(3), CIRCLE_YPOS(3), CIRCLE_RADIUS - 2, UTIL_LCD_COLOR_WHITE);
      UTIL_LCD_FillCircle(CIRCLE_XPOS(4), CIRCLE_YPOS(3), CIRCLE_RADIUS - 2, UTIL_LCD_COLOR_WHITE);

      UTIL_LCD_DrawHLine(CIRCLE_XPOS(1)-LINE_LENGHT, CIRCLE_YPOS(1), 2*LINE_LENGHT, UTIL_LCD_COLOR_BLUE);
      UTIL_LCD_DrawHLine(CIRCLE_XPOS(2)-LINE_LENGHT, CIRCLE_YPOS(2), 2*LINE_LENGHT, UTIL_LCD_COLOR_BLUE);
      UTIL_LCD_DrawVLine(CIRCLE_XPOS(2), CIRCLE_YPOS(2)-LINE_LENGHT, 2*LINE_LENGHT, UTIL_LCD_COLOR_BLUE);
      UTIL_LCD_DrawHLine(CIRCLE_XPOS(3)-LINE_LENGHT, CIRCLE_YPOS(3), 2*LINE_LENGHT, UTIL_LCD_COLOR_BLUE);
      UTIL_LCD_DrawHLine(CIRCLE_XPOS(4)-LINE_LENGHT, CIRCLE_YPOS(4), 2*LINE_LENGHT, UTIL_LCD_COLOR_BLUE);
      UTIL_LCD_DrawVLine(CIRCLE_XPOS(4), CIRCLE_YPOS(4)-LINE_LENGHT, 2*LINE_LENGHT, UTIL_LCD_COLOR_BLUE);

      UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLUE);
      UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
      UTIL_LCD_SetFont(&Font24);
      x = CIRCLE_XPOS(1);
      y = CIRCLE_YPOS(1) - CIRCLE_RADIUS - UTIL_LCD_GetFont()->Height;
      UTIL_LCD_DisplayStringAt(x, y, (uint8_t *)"Volume", CENTER_MODE);
      x = CIRCLE_XPOS(4);
      y = CIRCLE_YPOS(4) - CIRCLE_RADIUS - UTIL_LCD_GetFont()->Height;
      UTIL_LCD_DisplayStringAt(x, y, (uint8_t *)"Frequency", CENTER_MODE);

      break;

    case 32:
      UTIL_LCD_FillCircle(CIRCLE_XPOS(1), CIRCLE_YPOS(1), CIRCLE_RADIUS, UTIL_LCD_COLOR_BLACK);
      UTIL_LCD_FillCircle(CIRCLE_XPOS(2), CIRCLE_YPOS(2), CIRCLE_RADIUS, UTIL_LCD_COLOR_BLACK);

      UTIL_LCD_FillCircle(CIRCLE_XPOS(1), CIRCLE_YPOS(1), CIRCLE_RADIUS - 2, UTIL_LCD_COLOR_WHITE);
      UTIL_LCD_FillCircle(CIRCLE_XPOS(2), CIRCLE_YPOS(2), CIRCLE_RADIUS - 2, UTIL_LCD_COLOR_WHITE);


      UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLACK);
      UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
      UTIL_LCD_SetFont(&Font20);
      x = CIRCLE_XPOS(1) - 10;
      y = CIRCLE_YPOS(1) - (UTIL_LCD_GetFont()->Height)/2;
      UTIL_LCD_DisplayStringAt(x, y, (uint8_t *)"Up", LEFT_MODE);
      x = CIRCLE_XPOS(2) - 10;
      y = CIRCLE_YPOS(3)  - (UTIL_LCD_GetFont()->Height)/2;
      UTIL_LCD_DisplayStringAt(x, y, (uint8_t *)"Dw", LEFT_MODE);
      UTIL_LCD_SetFont(&Font12);

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

