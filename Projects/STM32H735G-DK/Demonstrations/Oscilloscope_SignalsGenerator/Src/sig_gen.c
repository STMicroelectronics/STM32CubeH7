/**
  ******************************************************************************
  * @file    Demonstration/Oscilloscope_SignalsGenerator/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to use STM32H7 to create a digital oscilloscope
    able to display and detect signals with frequency up to 2 MHz.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include "sig_gen.h"
#include "buttons.h"
#include "signals.h"
#include "background.h"
#include <stdio.h>
#include <string.h>

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "cmsis_os.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/ 
#define mainCHECK_TASK_PRIORITY			( configMAX_PRIORITIES - 2 )


/** @addtogroup STM32H7xx_HAL_Demonstration
  * @{
  */

/** @addtogroup Oscilloscope_SignalsGenerator
  * @{
  */


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

static void  SG_WaveButtonCallback( void *pData);
static void SG_StartGeneratorProcess(void);
static void SG_InitGeneratorMenu(void);
/* Private variable ------------------------------------------------------------*/
SG_BackgroundWidgetTypeDef BackgroundImage[1] = 
{
  {
    {
      BACKGROUND_IMAGE_WIDTH,  /* Width */
      BACKGROUND_IMAGE_HEIGHT, /* Height */
      BACKGROUND_IMAGE_XPOSITION,  /* Xpos */ 
      BACKGROUND_IMAGE_YPOSITION,  /* Ypos */
      DMA2D_INPUT_RGB888,   /* ColorMode */
      NULL,                   /* BackgroundImageCallback */
    },
    (uint32_t *)background,
  }
};

SG_SwitchButtonWidgetTypeDef WaveButtonsTab[SG_TOTAL_WAVE_BUTTON_NBR] = 
{
  /************* Wave OFF Button  ***********/
  { 
    {
      SG_WAVE_BUTTON_WIDTH,  /* Width */
      SG_WAVE_BUTTON_HEIGHT, /* Height */
      SG_WAVE_BUTTON_XPOSITION,  /* Xpos */ 
      0,  /* Ypos */
      DMA2D_INPUT_ARGB8888,   /* ColorMode */
      SG_WaveButtonCallback  /* ButtonCallback */
    },
    (uint32_t *)(button_rectangle_dc_on), /* On_FrameBuffer */
    (uint32_t *)(button_rectangle_dc_off),  /* Off_FrameBuffer */
    BUTTON_STATE_OFF,                                  /* State */
    0,  /* LastPressTime_ticks */
    0,  /* isActive */
    SG_DC_WAVE_BUTTON_INDEX,
  },
  /************* Square Wave Button  ***********/
  { 
    {
      SG_WAVE_BUTTON_WIDTH,  /* Width */
      SG_WAVE_BUTTON_HEIGHT, /* Height */
      SG_WAVE_BUTTON_XPOSITION,  /* Xpos */ 
      0,  /* Ypos */
      DMA2D_INPUT_ARGB8888,   /* ColorMode */
      SG_WaveButtonCallback  /* ButtonCallback */
    },
    (uint32_t *)(button_rectangle_square_on), /* On_FrameBuffer */
    (uint32_t *)(button_rectangle_square_off),  /* Off_FrameBuffer */
    BUTTON_STATE_OFF,                                  /* State */
    0,  /* LastPressTime_ticks */
    0,  /* isActive */
    SG_SQUARE_WAVE_BUTTON_INDEX,
  },
  /************* Sine Wave Button  ***********/
  { 
    {
      SG_WAVE_BUTTON_WIDTH,  /* Width */
      SG_WAVE_BUTTON_HEIGHT, /* Height */
      SG_WAVE_BUTTON_XPOSITION,  /* Xpos */ 
      0,  /* Ypos */
      DMA2D_INPUT_ARGB8888,   /* ColorMode */
      SG_WaveButtonCallback  /* ButtonCallback */
    },
    (uint32_t *)(button_rectangle_sine_on), /* On_FrameBuffer */
    (uint32_t *)(button_rectangle_sine_off),  /* Off_FrameBuffer */
    BUTTON_STATE_OFF,                                  /* State */
    0,  /* LastPressTime_ticks */
    0,  /* isActive */
    SG_SINE_WAVE_BUTTON_INDEX,
  },
  /************* Triangle Wave Button  ***********/
  { 
    {
      SG_WAVE_BUTTON_WIDTH,  /* Width */
      SG_WAVE_BUTTON_HEIGHT, /* Height */
      SG_WAVE_BUTTON_XPOSITION,  /* Xpos */ 
      0,  /* Ypos */
      DMA2D_INPUT_ARGB8888,   /* ColorMode */
      SG_WaveButtonCallback  /* ButtonCallback */
    },
    (uint32_t *)(button_rectangle_triangle_on), /* On_FrameBuffer */
    (uint32_t *)(button_rectangle_triangle_off),  /* Off_FrameBuffer */
    BUTTON_STATE_OFF,                                  /* State */
    0,  /* LastPressTime_ticks */
    0,  /* isActive */
    SG_TRIANGLE_WAVE_BUTTON_INDEX,
  },
  /************* Noise Wave Button  ***********/
  { 
    {
      SG_WAVE_BUTTON_WIDTH,  /* Width */
      SG_WAVE_BUTTON_HEIGHT, /* Height */
      SG_WAVE_BUTTON_XPOSITION,  /* Xpos */ 
      0,  /* Ypos */
      DMA2D_INPUT_ARGB8888,   /* ColorMode */
      SG_WaveButtonCallback  /* ButtonCallback */
    },
    (uint32_t *)(button_rectangle_noise_on), /* On_FrameBuffer */
    (uint32_t *)(button_rectangle_noise_off),  /* Off_FrameBuffer */
    BUTTON_STATE_OFF,                                  /* State */
    0,  /* LastPressTime_ticks */
    0,  /* isActive */
    SG_NOISE_WAVE_BUTTON_INDEX,
  },
  /************* Escalator Wave Button  ***********/
  { 
    {
      SG_WAVE_BUTTON_WIDTH,  /* Width */
      SG_WAVE_BUTTON_HEIGHT, /* Height */
      SG_WAVE_BUTTON_XPOSITION,  /* Xpos */ 
      0,  /* Ypos */
      DMA2D_INPUT_ARGB8888,   /* ColorMode */
      SG_WaveButtonCallback  /* ButtonCallback */
    },
    (uint32_t *)(button_rectangle_escalator_on), /* On_FrameBuffer */
    (uint32_t *)(button_rectangle_escalator_off),  /* Off_FrameBuffer */
    BUTTON_STATE_OFF,                                  /* State */
    0,  /* LastPressTime_ticks */
    0,  /* isActive */
    SG_ESCALATOR_WAVE_BUTTON_INDEX,
  }  
};

SG_ButtonWidgetTypeDef UpDownFrequencyButtonsTab[SG_TOTAL_UPDOWN_BUTTON_NBR] = 
{
  {
    /************* Down Button  ***********/
    {
        SG_UPDOWN_BUTTON_WIDTH,  /* Width */
        SG_UPDOWN_BUTTON_HEIGHT, /* Height */
        SG_DOWN_FEQUECNY_BUTTON_XPOSITION,  /* Xpos */ 
        SG_DOWN_FEQUECNY_BUTTON_YPOSITION,  /* Ypos */
        DMA2D_INPUT_ARGB8888,   /* ColorMode */
        NULL  /* ButtonCallback */
    },
    (uint32_t *)(Button_Down), /* FrameBuffer */
    SG_DOWN_BUTTON_INDEX,
  },
  {
    /************* Up Button  ***********/
    {
        SG_UPDOWN_BUTTON_WIDTH,  /* Width */
        SG_UPDOWN_BUTTON_HEIGHT, /* Height */
        SG_UP_FEQUECNY_BUTTON_XPOSITION,  /* Xpos */ 
        SG_UP_FEQUECNY_BUTTON_YPOSITION,  /* Ypos */
        DMA2D_INPUT_ARGB8888,   /* ColorMode */
        NULL  /* ButtonCallback */
    },
    (uint32_t *)(Button_Up), /* FrameBuffer */
    SG_UP_BUTTON_INDEX,
  }  
};

SG_ButtonWidgetTypeDef UpDownAmplitudeButtonsTab[SG_TOTAL_UPDOWN_BUTTON_NBR] = 
{
  {
    /************* Down Button  ***********/
    {
        SG_UPDOWN_BUTTON_WIDTH,  /* Width */
        SG_UPDOWN_BUTTON_HEIGHT, /* Height */
        SG_DOWN_AMPLITUDE_BUTTON_XPOSITION,  /* Xpos */ 
        SG_DOWN_AMPLITUDE_BUTTON_YPOSITION,  /* Ypos */
        DMA2D_INPUT_ARGB8888,   /* ColorMode */
        NULL  /* ButtonCallback */
    },
    (uint32_t *)(Button_Down), /* FrameBuffer */
    SG_DOWN_BUTTON_INDEX,
  },
  {
    /************* Up Button  ***********/
    {
        SG_UPDOWN_BUTTON_WIDTH,  /* Width */
        SG_UPDOWN_BUTTON_HEIGHT, /* Height */
        SG_UP_AMPLITUDE_BUTTON_XPOSITION,  /* Xpos */ 
        SG_UP_AMPLITUDE_BUTTON_YPOSITION,  /* Ypos */
        DMA2D_INPUT_ARGB8888,   /* ColorMode */
        NULL  /* ButtonCallback */
    },
    (uint32_t *)(Button_Up), /* FrameBuffer */
    SG_UP_BUTTON_INDEX,
  }  
};


SG_WaveButtonIndex_e activeButtonIndex = SG_MAX_WAVE_BUTTON_INDEX;
uint32_t waveFrequency = DAC_MIN_FREQUENCY;
uint32_t waveAmplitude = DAC_MAX_AMPLITUDE;
uint8_t  frequencyName[16];
uint8_t  amplitudeName[16];

extern uint32_t AutoMode ;
static uint32_t count = 0;

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  SigGenTask Thread.
  * @param  None
  * @retval None
  */
void SigGenThread(void const *argument)
{  
  
  /* Initialize LED1 */
  BSP_LED_Init(LED1);
  BSP_LED_On(LED1);

  SG_InitGeneratorMenu();
  
  /* Infinite loop */
  while (1)
  {   
    SG_StartGeneratorProcess();
    BSP_LED_Toggle(LED1);
  }  
}

/**
  * @brief  Init the main generator menu
  * @param  None
  * @retval None
  */
static void SG_InitGeneratorMenu(void)
{
  uint8_t  frequencyName[16];
  
  /* Init the Display */  
  SG_InitDisplay();  
  
  /* Init the touch screen */
  SG_InitTouchScreen();

  /* Display background image */
  SG_DisplayBackgroundImage(BackgroundImage); 
  
  /* Display Demo title */
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_GREEN);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_GRAY);
  UTIL_LCD_SetFont(&Font24);
  UTIL_LCD_DisplayStringAt(0, 2,(uint8_t*)  "SIGNALS GENERATOR", CENTER_MODE);
  
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_SetBackColor(0xFFCCCFF2);
  UTIL_LCD_SetFont(&Font16);


  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);  
  UTIL_LCD_FillRect(6, 220 , 466, 47,UTIL_LCD_COLOR_WHITE);
  
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_DARKMAGENTA);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);

  UTIL_LCD_DrawRect(3, 217 , 472, 53,UTIL_LCD_COLOR_DARKMAGENTA);
  UTIL_LCD_DrawRect(4, 218 , 470, 51,UTIL_LCD_COLOR_DARKMAGENTA);
  
  UTIL_LCD_SetFont(&Font20);
  UTIL_LCD_DisplayStringAt(20, 221, (uint8_t*) "           STM32H735           ", LEFT_MODE);
  UTIL_LCD_SetFont(&Font16);
  UTIL_LCD_DisplayStringAt(20, 251, (uint8_t*) " High Performance STM32 Microcontroller ", LEFT_MODE);


  
  /* Display wave buttons */
  SG_DisplayWaveButtons(WaveButtonsTab, SG_TOTAL_WAVE_BUTTON_NBR);
  
  /* Display frequency */
  sprintf((char*)frequencyName , "%3d.000 KHZ", 0);
  SG_DisplayFrequencyIcon(SG_FREQ_RECT_XPOSITION, SG_FREQ_RECT_YPOSITION, SG_FREQ_RECT_WIDHT, SG_FREQ_RECT_HEIGHT, frequencyName);
  
  /* Display frequency up down buttons */
  SG_DisplayUpDownButtons(UpDownFrequencyButtonsTab, SG_TOTAL_UPDOWN_BUTTON_NBR); 
  
  /* Display amplitude */
  sprintf((char*)amplitudeName , "%2.3f V", 3.3);
  SG_DisplayAmplitudeIcon(SG_AMPL_RECT_XPOSITION, SG_AMPL_RECT_YPOSITION, SG_AMPL_RECT_WIDHT, SG_AMPL_RECT_HEIGHT, amplitudeName);
  
  /* Display amplitude up down buttons */
  SG_DisplayUpDownButtons(UpDownAmplitudeButtonsTab, SG_TOTAL_UPDOWN_BUTTON_NBR); 
  

}


/**
  * @brief  Set the output generated wave frequency
  * @param  None
  * @retval None
  */
static void SG_SetWaveFrequency(void)
{
  SG_UpDownButtonIndex_e UpDownButtonIndex = SG_MAX_UPDOWN_BUTTON_INDEX;
  
  /* Get button up down status */
  UpDownButtonIndex = SG_GetUpDownButtonIndex(UpDownFrequencyButtonsTab, SG_TOTAL_UPDOWN_BUTTON_NBR);
  osDelay(50);
  
  if(UpDownButtonIndex != SG_MAX_UPDOWN_BUTTON_INDEX)
  {       
    if((activeButtonIndex != SG_DC_WAVE_BUTTON_INDEX) && (activeButtonIndex != SG_NOISE_WAVE_BUTTON_INDEX))
    {
      if(UpDownButtonIndex == SG_UP_BUTTON_INDEX)
      {
        if(waveFrequency >= DAC_MAX_FREQUENCY)
        {
          waveFrequency = DAC_MAX_FREQUENCY;
        }
        else
        {
          waveFrequency += DAC_OFFSET_FREQUENCY;
        }
      }
      else
      {
        if(waveFrequency <= DAC_MIN_FREQUENCY)
        {
          waveFrequency = DAC_MIN_FREQUENCY;
        }
        else
        {
          waveFrequency -= DAC_OFFSET_FREQUENCY;
        }
      }
              
      sprintf((char*)frequencyName , "%3ld.000 KHZ", (waveFrequency/1000));
    }
    else
    {
      waveFrequency = DAC_MIN_FREQUENCY;
      sprintf((char*)frequencyName , "N.A");
    }
    SG_GenerateWave(activeButtonIndex, waveFrequency, waveAmplitude);
    SG_DisplayFrequencyIcon(SG_FREQ_RECT_XPOSITION, SG_FREQ_RECT_YPOSITION, SG_FREQ_RECT_WIDHT, SG_FREQ_RECT_HEIGHT, frequencyName);
    SG_DisplayUpDownButtons(UpDownFrequencyButtonsTab, SG_TOTAL_UPDOWN_BUTTON_NBR); 
    
    
  }
}

/**
  * @brief  Set the output generated wave Amplitude
  * @param  None
  * @retval None
  */
static void SG_SetWaveAmplitude(void)
{
  SG_UpDownButtonIndex_e UpDownButtonIndex = SG_MAX_UPDOWN_BUTTON_INDEX;
  
  /* Get button up down status */
  UpDownButtonIndex = SG_GetUpDownButtonIndex(UpDownAmplitudeButtonsTab, SG_TOTAL_UPDOWN_BUTTON_NBR);

  osDelay(50);

  if(UpDownButtonIndex != SG_MAX_UPDOWN_BUTTON_INDEX)
  {       
    if(UpDownButtonIndex == SG_UP_BUTTON_INDEX)
    {
      if(waveAmplitude >= DAC_MAX_AMPLITUDE)
      {
        waveAmplitude = DAC_MAX_AMPLITUDE;
      }
      else
      {
        waveAmplitude += DAC_OFFSET_AMPLITUDE;
      }
    }
    else
    {
      if(waveAmplitude <= DAC_MIN_AMPLITUDE)
      {
        waveAmplitude = DAC_MIN_AMPLITUDE;
      }
      else
      {
        waveAmplitude -= DAC_OFFSET_AMPLITUDE;
      }
    }
            
    sprintf((char*)amplitudeName , "%2.3f V", (float)waveAmplitude/1000);
    SG_GenerateWave(activeButtonIndex, waveFrequency, waveAmplitude);
    SG_DisplayAmplitudeIcon(SG_AMPL_RECT_XPOSITION, SG_AMPL_RECT_YPOSITION, SG_AMPL_RECT_WIDHT, SG_AMPL_RECT_HEIGHT, amplitudeName);
    SG_DisplayUpDownButtons(UpDownAmplitudeButtonsTab, SG_TOTAL_UPDOWN_BUTTON_NBR); 
  }
}

/**
  * @brief  Set the output generated wave format
  * @param  None
  * @retval None
  */
static void SG_SetWaveFormat(void)
{ 
  SG_WaveButtonIndex_e WaveButtonIndex = SG_MAX_WAVE_BUTTON_INDEX;
  char  status_msg[20]; 
  char  status_type[20];  
  char  status_freq[20];  
  WaveButtonIndex = SG_GetWaveButtonIndex(WaveButtonsTab, SG_TOTAL_WAVE_BUTTON_NBR);
  
  osDelay(50);  

  if(WaveButtonIndex != SG_MAX_WAVE_BUTTON_INDEX)
  {
    
    SG_SwitchWaveButtonState(WaveButtonsTab, WaveButtonIndex, SG_TOTAL_WAVE_BUTTON_NBR);
    if(WaveButtonsTab[WaveButtonIndex].State == BUTTON_STATE_ON)
    {
      activeButtonIndex = WaveButtonIndex;
      
      if((activeButtonIndex != SG_DC_WAVE_BUTTON_INDEX) && (activeButtonIndex != SG_NOISE_WAVE_BUTTON_INDEX))
      {
        sprintf((char*)frequencyName , "%ld.000 KHZ", (waveFrequency/1000));
      }
      else
      {
        waveFrequency = DAC_MIN_FREQUENCY;
        sprintf((char*)frequencyName , "    N.A");
      }
      SG_GenerateWave(WaveButtonIndex, waveFrequency, waveAmplitude);
      
      SG_DisplayFrequencyIcon(SG_FREQ_RECT_XPOSITION, SG_FREQ_RECT_YPOSITION, SG_FREQ_RECT_WIDHT, SG_FREQ_RECT_HEIGHT, frequencyName);
      SG_DisplayUpDownButtons(UpDownFrequencyButtonsTab, SG_TOTAL_UPDOWN_BUTTON_NBR);
      SG_DisplayUpDownButtons(UpDownAmplitudeButtonsTab, SG_TOTAL_UPDOWN_BUTTON_NBR);
      
      
    }
    else
    {
      SG_StopWaveGeneration();
      activeButtonIndex = SG_MAX_WAVE_BUTTON_INDEX;
      
    }
  }
  
  if(WaveButtonsTab[activeButtonIndex].State == BUTTON_STATE_ON)
  {
    
    if(activeButtonIndex == SG_DC_WAVE_BUTTON_INDEX)
    {
      LCD_CopyImage((uint32_t*)sig_dc,(uint32_t *)LCD_LAYER_0_ADDRESS, 360,70, 112, 80, DMA2D_INPUT_ARGB8888, 0);
      LCD_CopyImage((uint32_t*)sig_dc,(uint32_t *)LCD_LAYER_0_ADDRESS, 361,70, 112, 80, DMA2D_INPUT_ARGB8888, 0);
      sprintf(status_type , "Type:    DC    ");
      sprintf(status_freq , "Freq: %6ld HZ", (waveFrequency));
    }
    if(activeButtonIndex == SG_SQUARE_WAVE_BUTTON_INDEX)
    {
      LCD_CopyImage((uint32_t*)sig_sqr,(uint32_t *)LCD_LAYER_0_ADDRESS, 360,70, 112, 80, DMA2D_INPUT_ARGB8888, 0);
      LCD_CopyImage((uint32_t*)sig_sqr,(uint32_t *)LCD_LAYER_0_ADDRESS, 361,70, 112, 80, DMA2D_INPUT_ARGB8888, 0);
      sprintf(status_type , "Type:   Square ");
      sprintf(status_freq , "Freq: %6ld HZ", (waveFrequency));
    }
    if(activeButtonIndex == SG_SINE_WAVE_BUTTON_INDEX)
    {
      LCD_CopyImage((uint32_t*)sig_sine,(uint32_t *)LCD_LAYER_0_ADDRESS, 360,70, 112, 80, DMA2D_INPUT_ARGB8888, 0);
      LCD_CopyImage((uint32_t*)sig_sine,(uint32_t *)LCD_LAYER_0_ADDRESS, 361,70, 112, 80, DMA2D_INPUT_ARGB8888, 0);
      sprintf(status_type , "Type:   Sinus  ");
      sprintf(status_freq , "Freq: %6ld HZ", (waveFrequency));
    }
    if(activeButtonIndex == SG_TRIANGLE_WAVE_BUTTON_INDEX)
    {
      LCD_CopyImage((uint32_t*)sig_tri,(uint32_t *)LCD_LAYER_0_ADDRESS, 360,70, 112, 80, DMA2D_INPUT_ARGB8888, 0);
      LCD_CopyImage((uint32_t*)sig_tri,(uint32_t *)LCD_LAYER_0_ADDRESS, 361,70, 112, 80, DMA2D_INPUT_ARGB8888, 0);
      sprintf(status_type , "Type: Triangle ");
      sprintf(status_freq , "Freq: %6ld HZ", (waveFrequency));
    }
    if(activeButtonIndex == SG_ESCALATOR_WAVE_BUTTON_INDEX)
    {
      LCD_CopyImage((uint32_t*)sig_esca,(uint32_t *)LCD_LAYER_0_ADDRESS, 360,70, 112, 80, DMA2D_INPUT_ARGB8888, 0);
      LCD_CopyImage((uint32_t*)sig_esca,(uint32_t *)LCD_LAYER_0_ADDRESS, 361,70, 112, 80, DMA2D_INPUT_ARGB8888, 0);
      sprintf(status_type , "Type: Escalator");
      sprintf(status_freq , "Freq: %6ld HZ", (waveFrequency));
    }
    if(activeButtonIndex == SG_NOISE_WAVE_BUTTON_INDEX)
    {
      LCD_CopyImage((uint32_t*)sig_noise,(uint32_t *)LCD_LAYER_0_ADDRESS, 360,70, 112, 80, DMA2D_INPUT_ARGB8888, 0);
      LCD_CopyImage((uint32_t*)sig_noise,(uint32_t *)LCD_LAYER_0_ADDRESS, 361,70, 112, 80, DMA2D_INPUT_ARGB8888, 0);
      sprintf(status_type , "Type:   Noise  ");
      sprintf(status_freq , "Freq: ------ HZ");
    }
    
    UTIL_LCD_SetFont(&Font12);
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_GREEN);
    UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLACK);  
    UTIL_LCD_DisplayStringAt(364, 156, (uint8_t*)status_type, LEFT_MODE);  
    UTIL_LCD_DisplayStringAt(364, 172, (uint8_t*)status_freq, LEFT_MODE);
    sprintf((char*)status_msg , "Ampl: %6ld mV", (waveAmplitude));
    UTIL_LCD_DisplayStringAt(364, 188, (uint8_t*)status_msg, LEFT_MODE);   
    
  }
  else
  {
    
    LCD_CopyImage((uint32_t*)sig_none,(uint32_t *)LCD_LAYER_0_ADDRESS, 360,70, 112, 80, DMA2D_INPUT_ARGB8888, 0);
    
    UTIL_LCD_SetFont(&Font12);
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_GREEN);
    UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLACK);
    UTIL_LCD_DisplayStringAt(364, 156,(uint8_t*) "Type:   N.A    ", LEFT_MODE);
    UTIL_LCD_DisplayStringAt(364, 172, (uint8_t*)"Freq: ------ HZ", LEFT_MODE);
    UTIL_LCD_DisplayStringAt(364, 188, (uint8_t*)"Ampl: ------ mV", LEFT_MODE);
  }
  
  
  
  
  
}


/**
  * @brief  Start the generator process
  * @param  None
  * @retval None
  */
static void SG_StartGeneratorProcess(void)
{
  uint8_t msg[16];

  /* Update wave format when user set frequency */ 
  SG_SetWaveFormat();
    
  /* Update wave frequency when user set up down buttons */ 
  SG_SetWaveFrequency();
  
  /* Update wave Amplitude when user set up down buttons */ 
  SG_SetWaveAmplitude();

  count++;
  
  if (AutoMode==1)
  {      
    if (count %2 ==0)
    {
      UTIL_LCD_SetFont(&Font12);
      UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_RED); 
      sprintf((char*)msg , "!!!AUTO!!!");
      UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
      UTIL_LCD_DisplayStringAt(400, 227, msg, LEFT_MODE);
    }
    else
    {
      UTIL_LCD_SetFont(&Font12);
      UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE); 
      UTIL_LCD_FillRect(400, 227, 70, 15, UTIL_LCD_COLOR_WHITE);  
    }
    
  } 
  else
  {
    UTIL_LCD_SetFont(&Font12);
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE); 
    UTIL_LCD_FillRect(400, 227, 70, 15, UTIL_LCD_COLOR_WHITE);  
  }  
}

/**
  * @brief  Wave button callback
  * @param  None
  * @retval None
  */
static void  SG_WaveButtonCallback( void *pData)
{
}


/**
  * @}
  */

/**
  * @}
  */

