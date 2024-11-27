/**
  ******************************************************************************
  * @file    Demonstration/STM32H745-Discovery_Demo/CM7/Src/main_oscillo.c
  * @author  MCD Application Team
  * @brief   This example describes how to use STM32H7 to create a digital oscilloscope
  *          able to display and detect signals with frequency up to 2 MHz.
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

#include <stm32h7xx_hal.h>
#include "main_oscillo_cm7.h"
#include "image_oscilo.h"
#include "signals_demo.h"
#include "graphics.h"
#include <string.h>
/** @addtogroup STM32H7xx_HAL_Demonstration
  * @{
  */

/** @addtogroup Oscilloscope_SignalsGenerator
  * @{
  */


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define PWR_WAKEUP_FLAGS  (PWR_WAKEUP_FLAG1 | PWR_WAKEUP_FLAG2 | PWR_WAKEUP_FLAG3 | \
                           PWR_WAKEUP_FLAG4 | PWR_WAKEUP_FLAG5 | PWR_WAKEUP_FLAG6)
/* Private functions ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);

#if (USE_VOS0_480MHZ_OVERCLOCK == 1)
static void SystemClock_Config_480MHz(void);
#else
static void SystemClock_Config_400MHz(void);
#endif

static void MPU_Config(void);
static void CPU_CACHE_Enable(void);
static void Error_Handler(void);
static void  SG_WaveButtonCallback( void *pData);
static void SG_StartGeneratorProcess(void);
static void SG_InitGeneratorMenu(void);
/* Private variable ------------------------------------------------------------*/
BackgroundWidgetTypeDef BackgroundImage[1] = 
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
    (uint32_t*)background/*NULL*/,
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
    (uint32_t*)(button_rectangle_dc_on), /* On_FrameBuffer */
    (uint32_t*)(button_rectangle_dc_off),  /* Off_FrameBuffer */
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
    (uint32_t*)(button_rectangle_square_on), /* On_FrameBuffer */
    (uint32_t*)(button_rectangle_square_off),  /* Off_FrameBuffer */
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
    (uint32_t*)(button_rectangle_sine_on), /* On_FrameBuffer */
    (uint32_t*)(button_rectangle_sine_off),  /* Off_FrameBuffer */
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
    (uint32_t*)(button_rectangle_triangle_on), /* On_FrameBuffer */
    (uint32_t*)(button_rectangle_triangle_off),  /* Off_FrameBuffer */
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
    (uint32_t*)(button_rectangle_noise_on), /* On_FrameBuffer */
    (uint32_t*)(button_rectangle_noise_off),  /* Off_FrameBuffer */
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
    (uint32_t*)(button_rectangle_escalator_on), /* On_FrameBuffer */
    (uint32_t*)(button_rectangle_escalator_off),  /* Off_FrameBuffer */
    BUTTON_STATE_OFF,                                  /* State */
    0,  /* LastPressTime_ticks */
    0,  /* isActive */
    SG_ESCALATOR_WAVE_BUTTON_INDEX,
  }  

};


ButtonWidgetTypeDef GenericButtonsTab[SG_TOTAL_GENERIC_BUTTON_NBR] = 
/************* Return Button  ***********/
{
  { 
    {
      46,  /* Width */
      44, /* Height */
      430,  /* Xpos */ 
      0,  /* Ypos */
      DMA2D_INPUT_ARGB8888,   /* ColorMode */
      SG_WaveButtonCallback /* ButtonCallback*/
    },
    (uint32_t*)(SG_button), /* Return button */
    RETURN,
  }  
};

ButtonWidgetTypeDef UpDownFrequencyButtonsTab[SG_TOTAL_UPDOWN_BUTTON_NBR] = 
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
    (uint32_t*)(Button_Down), /* FrameBuffer */
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
    (uint32_t*)(Button_Up), /* FrameBuffer */
    SG_UP_BUTTON_INDEX,
  }  
};

ButtonWidgetTypeDef UpDownAmplitudeButtonsTab[SG_TOTAL_UPDOWN_BUTTON_NBR] = 
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
    (uint32_t*)(Button_Down), /* FrameBuffer */
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
    (uint32_t*)Button_Up, /* FrameBuffer */
    SG_UP_BUTTON_INDEX,
  }  
};


static SG_WaveButtonIndex_e activeButtonIndex = SG_MAX_WAVE_BUTTON_INDEX;
static uint32_t waveFrequency = DAC_MIN_FREQUENCY;
static uint32_t waveAmplitude = DAC_MAX_AMPLITUDE;
static uint8_t  frequencyName[16];
static uint8_t  amplitudeName[10];
extern uint32_t AutoMode ;
static uint32_t count = 0;
#if defined (__ICCARM__) /* IAR Compiler */
extern const char * cm4_code; /* section containing CM4 Code */
#else /* GNU & ARM Compilers */
extern const uint32_t cm4_code; /* section containing CM4 Code */
extern const uint32_t cm4_code_end;
#endif /* __ICCARM__ */

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{  
  
  BSP_QSPI_Init_t qspi_init;

  MPU_Config();
  
  /* Enable the CPU Cache */
  CPU_CACHE_Enable();
  
  /* STM32H7xx HAL library initialization:
       - Configure the Systick to generate an interrupt each 1 msec
       - Set NVIC Group Priority to 4
       - Low Level Initialization
   */
  HAL_Init();

  /*  Configure the system clock to 400 MHz */
  SystemClock_Config();  
  
  /* Initialize LED1 */
  BSP_LED_Init(LED1);
  BSP_LED_On(LED1);


  if(__HAL_PWR_GET_FLAG(PWR_FLAG_SB_D1) == RESET)
  {
     /* When system initialization is finished, Cortex-M7 will release (wakeup) Cortex-M4  by means of 
       HSEM notification. Cortex-M4 release could be also ensured by any Domain D2 wakeup source (SEV,EXTI..).
    */

    /* HW semaphore Clock enable */
    __HAL_RCC_HSEM_CLK_ENABLE();
  
    __HAL_RCC_C1_D2SRAM1_CLK_ENABLE(); 
    /* Copy CM4 code from Flash to D2_SRAM memory */
  #if defined (__ICCARM__) /* IAR Compiler */
    memcpy((void *)D2_AXISRAM_BASE,  &cm4_code, 0x20000);
  #else /* GNU & ARM Compilers */
    memcpy((void *)D2_AXISRAM_BASE, &cm4_code, (uint32_t)&cm4_code_end - (uint32_t)&cm4_code);
  #endif /* __ICCARM__ */

    HAL_SYSCFG_CM4BootAddConfig(SYSCFG_BOOT_ADDR0, D2_AXISRAM_BASE); /*0x1000000*/


   /* Enable CPU2 (Cortex-M4) boot regardless of option byte values */
    HAL_RCCEx_EnableBootCore(RCC_BOOT_C2);
    
    /* Configure the boot address to be used after reset */
    HAL_Delay(100);
    HAL_SYSCFG_CM4BootAddConfig(SYSCFG_BOOT_ADDR0, FLASH_BANK2_BASE); /* 0x08100000*/
  }

  qspi_init.InterfaceMode=MT25TL01G_QPI_MODE;
  qspi_init.TransferRate= MT25TL01G_DTR_TRANSFER ;
  qspi_init.DualFlashMode= MT25TL01G_DUALFLASH_ENABLE;

  BSP_QSPI_Init(0, &qspi_init);
  BSP_QSPI_EnableMemoryMappedMode(0);
  
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
  uint8_t  frequencyName[10];
  /* Init the Display */  
  SG_InitDisplay();
  
  /* Init the touch screen */
  SG_InitTouchScreen();

  /* Display background image */
  SG_DisplayBackgroundImage(BackgroundImage,1); 
  
  /* Display Demo title */
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_GREEN);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_GRAY);
  UTIL_LCD_SetFont(&Font24);
  UTIL_LCD_DisplayStringAt(0, 2,(uint8_t *)"SIGNALS GENERATOR", CENTER_MODE);
  
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_SetBackColor(0xFFCCCFF2);
  UTIL_LCD_SetFont(&Font16);
  UTIL_LCD_DisplayStringAt(0, 210,(uint8_t *)" Press User Button to enter STANDBY mode ", CENTER_MODE);


  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);  
  UTIL_LCD_FillRect(6, 233 , 466, 34, UTIL_LCD_COLOR_WHITE);
  
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_DARKMAGENTA);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);

  UTIL_LCD_DrawRect(3, 230 , 472, 40, UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_DrawRect(4, 231 , 470, 38, UTIL_LCD_COLOR_WHITE);
  
  UTIL_LCD_SetFont(&Font20);
  UTIL_LCD_DisplayStringAt(20, 234,(uint8_t *) "           STM32H745           ", LEFT_MODE);
  UTIL_LCD_SetFont(&Font16);
  UTIL_LCD_DisplayStringAt(20, 251,(uint8_t *)"    Dual core STM32 Microcontroller    ", LEFT_MODE);


  
  /* Display wave buttons */
  SG_DisplayWaveButtons(WaveButtonsTab, SG_TOTAL_WAVE_BUTTON_NBR);
  SG_DisplayGenericButtons(GenericButtonsTab, (uint32_t)SG_TOTAL_GENERIC_BUTTON_NBR) ;
  /* Display frequency */
  sprintf((char*)frequencyName , "%u.000 KHZ", 0);
  SG_DisplayFrequencyIcon(SG_FREQ_RECT_XPOSITION, SG_FREQ_RECT_YPOSITION, SG_FREQ_RECT_WIDHT, SG_FREQ_RECT_HEIGHT, frequencyName);
  
  /* Display frequency up down buttons */
  SG_DisplayUpDownButtons(UpDownFrequencyButtonsTab, SG_TOTAL_UPDOWN_BUTTON_NBR); 
  
  /* Display amplitude */
  sprintf((char*)amplitudeName , "%2.3f V", 3.3);
  SG_DisplayAmplitudeIcon(SG_AMPL_RECT_XPOSITION, SG_AMPL_RECT_YPOSITION, SG_AMPL_RECT_WIDHT, SG_AMPL_RECT_HEIGHT, amplitudeName);
  
  /* Display amplitude up down buttons */
  SG_DisplayUpDownButtons(UpDownAmplitudeButtonsTab, SG_TOTAL_UPDOWN_BUTTON_NBR); 
  
  /* Configure the Wakeup push-button in GPIO Mode */
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);
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
  HAL_Delay(50);
  
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
              
      sprintf((char*)frequencyName , "%lu.000 KHZ", (waveFrequency/1000));
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
  HAL_Delay(50);
  
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
  
  
  HAL_Delay(50);
  
  if(WaveButtonIndex != SG_MAX_WAVE_BUTTON_INDEX)
  {
    
    SG_SwitchWaveButtonState(WaveButtonsTab, WaveButtonIndex, SG_TOTAL_WAVE_BUTTON_NBR);
    if(WaveButtonsTab[WaveButtonIndex].State == BUTTON_STATE_ON)
    {
      activeButtonIndex = WaveButtonIndex;
      
      if((activeButtonIndex != SG_DC_WAVE_BUTTON_INDEX) && (activeButtonIndex != SG_NOISE_WAVE_BUTTON_INDEX))
      {
        sprintf((char*)frequencyName , "%lu.000KHZ", (waveFrequency/1000));
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
      LCD_CopyImage((uint32_t*)sig_dc,(uint32_t *)LCD_FRAME_BUFFER, 360,70, 112, 80, DMA2D_INPUT_ARGB8888, 0);
      LCD_CopyImage((uint32_t*)sig_dc,(uint32_t *)LCD_FRAME_BUFFER, 361,70, 112, 80, DMA2D_INPUT_ARGB8888, 0);
      sprintf(status_type , "Type:    DC    ");
      sprintf(status_freq , "Freq: %lu HZ", (waveFrequency));
    }
    if(activeButtonIndex == SG_SQUARE_WAVE_BUTTON_INDEX)
    {
      LCD_CopyImage((uint32_t*)sig_sqr,(uint32_t *)LCD_FRAME_BUFFER, 360,70, 112, 80, DMA2D_INPUT_ARGB8888, 0);
      LCD_CopyImage((uint32_t*)sig_sqr,(uint32_t *)LCD_FRAME_BUFFER, 361,70, 112, 80, DMA2D_INPUT_ARGB8888, 0);
      sprintf(status_type , "Type:   Square ");
      sprintf(status_freq , "Freq: %lu HZ", (waveFrequency));
    }
    if(activeButtonIndex == SG_SINE_WAVE_BUTTON_INDEX)
    {
      LCD_CopyImage((uint32_t*)sig_sine,(uint32_t *)LCD_FRAME_BUFFER, 360,70, 112, 80, DMA2D_INPUT_ARGB8888, 0);
      LCD_CopyImage((uint32_t*)sig_sine,(uint32_t *)LCD_FRAME_BUFFER, 361,70, 112, 80, DMA2D_INPUT_ARGB8888, 0);
      sprintf(status_type , "Type:   Sinus  ");
      sprintf(status_freq , "Freq: %lu HZ", (waveFrequency));
    }
    if(activeButtonIndex == SG_TRIANGLE_WAVE_BUTTON_INDEX)
    {
      LCD_CopyImage((uint32_t*)sig_tri,(uint32_t *)LCD_FRAME_BUFFER, 360,70, 112, 80, DMA2D_INPUT_ARGB8888, 0);
      LCD_CopyImage((uint32_t*)sig_tri,(uint32_t *)LCD_FRAME_BUFFER, 361,70, 112, 80, DMA2D_INPUT_ARGB8888, 0);
      sprintf(status_type , "Type: Triangle ");
      sprintf(status_freq , "Freq: %lu HZ", (waveFrequency));
    }
    if(activeButtonIndex == SG_ESCALATOR_WAVE_BUTTON_INDEX)
    {
      LCD_CopyImage((uint32_t*)sig_esca,(uint32_t *)LCD_FRAME_BUFFER, 360,70, 112, 80, DMA2D_INPUT_ARGB8888, 0);
      LCD_CopyImage((uint32_t*)sig_esca,(uint32_t *)LCD_FRAME_BUFFER, 361,70, 112, 80, DMA2D_INPUT_ARGB8888, 0);
      sprintf(status_type , "Type: Escalator");
      sprintf(status_freq , "Freq: %lu HZ", (waveFrequency));
    }
    if(activeButtonIndex == SG_NOISE_WAVE_BUTTON_INDEX)
    {
      LCD_CopyImage((uint32_t*)sig_noise,(uint32_t *)LCD_FRAME_BUFFER, 360,70, 112, 80, DMA2D_INPUT_ARGB8888, 0);
      LCD_CopyImage((uint32_t*)sig_noise,(uint32_t *)LCD_FRAME_BUFFER, 361,70, 112, 80, DMA2D_INPUT_ARGB8888, 0);
      sprintf(status_type , "Type:   Noise  ");
      sprintf(status_freq , "Freq: ------ HZ");
    }
    
    UTIL_LCD_SetFont(&Font12);
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_GREEN);
    UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLACK);  
    UTIL_LCD_DisplayStringAt(364, 156, (uint8_t*)status_type, LEFT_MODE);  
    UTIL_LCD_DisplayStringAt(364, 172, (uint8_t*)status_freq, LEFT_MODE);
    sprintf((char*)status_msg ,"Ampl: %lu mV", (waveAmplitude));
    UTIL_LCD_DisplayStringAt(364, 188, (uint8_t*)status_msg, LEFT_MODE);   
    
  }
  else
  {
    
    LCD_CopyImage((uint32_t*)sig_none,(uint32_t *)LCD_FRAME_BUFFER, 360,70, 112, 80, DMA2D_INPUT_ARGB8888, 0);
    
    UTIL_LCD_SetFont(&Font12);
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_GREEN);
    UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLACK);
    UTIL_LCD_DisplayStringAt(364, 156,(uint8_t *) "Type:   N.A    ", LEFT_MODE);
    UTIL_LCD_DisplayStringAt(364, 172,(uint8_t *) "Freq: ------ HZ", LEFT_MODE);
    UTIL_LCD_DisplayStringAt(364, 188,(uint8_t *) "Ampl: ------ mV", LEFT_MODE);
  }
  
  
  
  
  
}


/**
  * @brief  Enter D1 domain to Standby mode when the user press Wakeup button
  * @param  None
  * @retval None
  */
static void SG_EnterGeneratorLowPowerMode(void)
{
  GPIO_InitTypeDef   GPIO_InitStructure;
  
  if(BSP_PB_GetState(BUTTON_USER) != RESET)
  {
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_RED);
    UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_LIGHTGRAY);
    UTIL_LCD_SetFont(&Font12);
    UTIL_LCD_DisplayStringAt(0, 212,(uint8_t *) "   The D1 Domain will Enter STANDBY mode after 3 seconds ...   ", CENTER_MODE);
    
    /* -2- Configure EXTI15_10 (connected to PC.13 pin) to interrupt CPU1 and CPU2 */
    /* Enable GPIOC clock */
    __HAL_RCC_GPIOC_CLK_ENABLE();
    
    /* Configure PC.13 pin as the EXTI input event line in interrupt mode for both CPU1 and CPU2*/
    GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING;    /* current CPU (CM7) config in IT rising */
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    GPIO_InitStructure.Pin = GPIO_PIN_13;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    /* Enable and set EXTI lines 15 to 10 Interrupt to the lowest priority */
    HAL_NVIC_SetPriority(EXTI15_10_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
    
    /* Configure the CPU (CM7) EXTI line for IT*/
    HAL_EXTI_D1_EventInputConfig(EXTI_LINE13 , EXTI_MODE_IT,  ENABLE);
    
    /* Clear system Standby flag */
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB); 
    
    /* Insert 1 seconds delay */
    HAL_Delay(3000);
    
    BSP_LCD_SetBrightness(0, 0);
    HAL_Delay(10);
    BSP_LCD_DisplayOff(0);
    SG_StopWaveGeneration();
    __HAL_RCC_DAC12_CLK_DISABLE();
    BSP_LED_Off(LED1);
    /* Enter D1 domain to DStandby mode */
    HAL_PWREx_EnterSTANDBYMode(PWR_D1_DOMAIN);
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
  
  /* Enter D1 domain to Standby mode when the user press Wakeup button */
  SG_EnterGeneratorLowPowerMode();

  /* Check Return Button */
  SG_UpdateGenericButtons(GenericButtonsTab);

  
  count++;
  
  if (AutoMode==1)
  {      
    if (count %2 ==0)
    {
      UTIL_LCD_SetFont(&Font12);
      UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_RED); 
      sprintf((char*)msg , "!!!AUTO!!!");
      UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
      UTIL_LCD_DisplayStringAt(400,240, msg, LEFT_MODE);
    }
    else
    {
      UTIL_LCD_SetFont(&Font12);
      UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE); 
      UTIL_LCD_FillRect(400, 240, 70, 15, UTIL_LCD_COLOR_WHITE);  
    }
    
  } 
  else
  {
    UTIL_LCD_SetFont(&Font12);
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE); 
    UTIL_LCD_FillRect(400, 240, 70, 15, UTIL_LCD_COLOR_WHITE);  
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
  * @brief  System Clock Configuration
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  HAL_StatusTypeDef ret = HAL_OK;
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;
  
  /* The voltage scaling allows optimizing the power consumption when the device is 
     clocked below the maximum system frequency, to update the voltage scaling value 
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}
  
#if (USE_VOS0_480MHZ_OVERCLOCK == 1)
  
  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);  
  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  SystemClock_Config_480MHz();
#else
  SystemClock_Config_400MHz();
#endif /* (USE_VOS0_480MHZ_OVERCLOCK == 1) */
  
/* Select PLL as system clock source and configure  bus clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_D1PCLK1 | RCC_CLOCKTYPE_PCLK1 | \
                                 RCC_CLOCKTYPE_PCLK2  | RCC_CLOCKTYPE_D3PCLK1);

  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;  
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2; 
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2; 
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2; 
  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4);
  if(ret != HAL_OK)
  {
    Error_Handler();
  }

#if (USE_VOS0_480MHZ_OVERCLOCK == 1)
  /* By default the FMC source clock is the AXI bus clock
     The maximum FMC allowed frequency is 200MHz
     when switching the system clock to 480MHz , the AXI bus clock is 240MHz:
       - In this case the FMC source clock is switched to PLL2 (DIVR) with 200MHz frequency    
  */

  /* PLL2_VCO Input = HSE_VALUE/PLL2_M = 5 Mhz */
  /* PLL2_VCO Output = PLL2_VCO Input * PLL_N = 800 Mhz */
  /* FMC Kernel Clock = PLL2_VCO Output/PLL_R = 800/4 = 200 Mhz */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_FMC;
  PeriphClkInitStruct.FmcClockSelection = RCC_FMCCLKSOURCE_PLL2;
  PeriphClkInitStruct.PLL2.PLL2RGE = RCC_PLL1VCIRANGE_2;
  PeriphClkInitStruct.PLL2.PLL2M = 5;
  PeriphClkInitStruct.PLL2.PLL2N = 160;
  PeriphClkInitStruct.PLL2.PLL2FRACN = 0;
  PeriphClkInitStruct.PLL2.PLL2P = 2;
  PeriphClkInitStruct.PLL2.PLL2R = 4;
  PeriphClkInitStruct.PLL2.PLL2Q = 4;
  PeriphClkInitStruct.PLL2.PLL2VCOSEL = RCC_PLL2VCOWIDE;
  ret = HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
  if(ret != HAL_OK)
  {
    Error_Handler();
  }
#endif

/* Select PLL2 As an independent clock source generator for ADC (PLL2 output = 36 MHz) */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInitStruct.AdcClockSelection = RCC_ADCCLKSOURCE_PLL2;
  PeriphClkInitStruct.PLL2.PLL2M = 5;
  PeriphClkInitStruct.PLL2.PLL2N = 160;
  PeriphClkInitStruct.PLL2.PLL2P = 22;
  PeriphClkInitStruct.PLL2.PLL2Q = 4;
  PeriphClkInitStruct.PLL2.PLL2R = 4;
  PeriphClkInitStruct.PLL2.PLL2RGE = RCC_PLL2VCIRANGE_2;
  PeriphClkInitStruct.PLL2.PLL2VCOSEL = RCC_PLL2VCOWIDE;
  PeriphClkInitStruct.PLL2.PLL2FRACN = 0;
  if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

 /*
  Note : The activation of the I/O Compensation Cell is recommended with communication  interfaces
          (GPIO, SPI, FMC, QSPI ...)  when  operating at  high frequencies(please refer to product datasheet)       
          The I/O Compensation Cell activation  procedure requires :
        - The activation of the CSI clock
        - The activation of the SYSCFG clock
        - Enabling the I/O Compensation Cell : setting bit[0] of register SYSCFG_CCCSR
 */
 
  /*activate CSI clock mondatory for I/O Compensation Cell*/  
  __HAL_RCC_CSI_ENABLE() ;
    
  /* Enable SYSCFG clock mondatory for I/O Compensation Cell */
  __HAL_RCC_SYSCFG_CLK_ENABLE() ;
  
  /* Enables the I/O Compensation Cell */    
  HAL_EnableCompensationCell();  
}

#if (USE_VOS0_480MHZ_OVERCLOCK == 1)

/**
  * @brief  System Clock Configuration to 480MHz
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 480000000 (CM7 CPU Clock)
  *            HCLK(Hz)                       = 240000000 (CM4 CPU, AXI and AHBs Clock)
  *            AHB Prescaler                  = 2
  *            D1 APB3 Prescaler              = 2 (APB3 Clock  120MHz)
  *            D2 APB1 Prescaler              = 2 (APB1 Clock  120MHz)
  *            D2 APB2 Prescaler              = 2 (APB2 Clock  120MHz)
  *            D3 APB4 Prescaler              = 2 (APB4 Clock  120MHz)
  *            HSE Frequency(Hz)              = 25000000
  *            PLL_M                          = 5
  *            PLL_N                          = 192
  *            PLL_P                          = 2
  *            PLL_Q                          = 4
  *            PLL_R                          = 2
  * @param  None
  * @retval None
  */
static void SystemClock_Config_480MHz(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct;
  HAL_StatusTypeDef ret = HAL_OK;

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
  RCC_OscInitStruct.CSIState = RCC_CSI_OFF;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;

  RCC_OscInitStruct.PLL.PLLM = 5;
  RCC_OscInitStruct.PLL.PLLN = 192;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLQ = 4;

  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
  if(ret != HAL_OK)
  {
    Error_Handler();
  }
}

#else
/**
  * @brief  System Clock Configuration to 400MHz
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 400000000 (CM7 CPU Clock)
  *            HCLK(Hz)                       = 200000000 (CM4 CPU, AXI and AHBs Clock)
  *            AHB Prescaler                  = 2
  *            D1 APB3 Prescaler              = 2 (APB3 Clock  100MHz)
  *            D2 APB1 Prescaler              = 2 (APB1 Clock  100MHz)
  *            D2 APB2 Prescaler              = 2 (APB2 Clock  100MHz)
  *            D3 APB4 Prescaler              = 2 (APB4 Clock  100MHz)
  *            HSE Frequency(Hz)              = 25000000
  *            PLL_M                          = 5
  *            PLL_N                          = 160
  *            PLL_P                          = 2
  *            PLL_Q                          = 4
  *            PLL_R                          = 2
  * @param  None
  * @retval None
  */
static void SystemClock_Config_400MHz(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct;
  HAL_StatusTypeDef ret = HAL_OK;
  
  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
  RCC_OscInitStruct.CSIState = RCC_CSI_OFF;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;

  RCC_OscInitStruct.PLL.PLLM = 5;
  RCC_OscInitStruct.PLL.PLLN = 160;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLQ = 4;

  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
  if(ret != HAL_OK)
  {
    Error_Handler();
  }
}

#endif /* USE_VOS0_480MHZ_OVERCLOCK */


static void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct;
  
  /* Disable the MPU */
  HAL_MPU_Disable();

  /* Configure the MPU as Strongly ordered for not defined regions */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress = 0x00;
  MPU_InitStruct.Size = MPU_REGION_SIZE_4GB;
  MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x87;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Configure the MPU attributes as WT for SRAM */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress = LCD_FRAME_BUFFER;
  MPU_InitStruct.Size = MPU_REGION_SIZE_512KB;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER1;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Enable the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

/**
  * @brief  CPU L1-Cache enable.
  * @param  None
  * @retval None
  */
static void CPU_CACHE_Enable(void)
{
  /* Enable I-Cache */
  SCB_EnableICache();

  /* Enable D-Cache */
  SCB_EnableDCache();
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* Turn LED REDon */
  BSP_LED_On(LED_RED);
  while(1)
  {
  }
}

/**
  * @}
  */

/**
  * @}
  */

