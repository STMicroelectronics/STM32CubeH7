/**
  ******************************************************************************
  * @file    FPU/FPU_Fractal/Src/main.c
  * @author  MCD Application Team
  * @brief   Main program body
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
#include "button.h"

/** @addtogroup STM32H7xx_HAL_Applications
  * @{
  */

/** @addtogroup FPU_Fractal
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define SCREEN_SENSTIVITY 5
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

extern LTDC_HandleTypeDef hltdc_eval;
DMA2D_HandleTypeDef    DMA2D_Handle;

extern LTDC_HandleTypeDef  hltdc_eval;
TS_Init_t *hTS;
/* 32-bytes Alignment is needed for cache maintenance purpose */
ALIGN_32BYTES(uint32_t L8_CLUT[ITERATION]);
ALIGN_32BYTES(uint32_t  buffer[LCD_X_SIZE * LCD_Y_SIZE / 4]);

uint8_t  text[50];

uint8_t SizeIndex = 0;
uint16_t SizeTable[][6]={{600, 252}, {500, 220},{400, 200},{300, 180},{200, 150},{160, 120}};

uint16_t XSize = 600;
uint16_t YSize = 252;

uint32_t x_size, y_size;

uint32_t  CurrentZoom = 100;
uint32_t  DirectionZoom = 0;

TS_State_t TS_State;
__IO uint8_t TouchdOn = 0;
__IO uint8_t TouchReleased = 0;
__IO uint8_t isplaying = 1;
__IO uint8_t playOneFrame = 0;
__IO  uint32_t score_fpu = 0;
__IO  uint32_t tickstart = 0;

#if (USE_VOS0_480MHZ_OVERCLOCK == 1)
__IO  uint32_t SystemClock_MHz = 480;
__IO  uint32_t SystemClock_changed = 0;
#endif /* (USE_VOS0_480MHZ_OVERCLOCK == 1) */


/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void SystemClock_Config_400MHz(void);
#if (USE_VOS0_480MHZ_OVERCLOCK == 1)
static void SystemClock_Config_480MHz(void);
static void FMC_SDRAM_Clock_Config(void);
static void EXTI15_10_IRQHandler_Config(void);
static void SystemClockChange_Handler(void);
#endif
static void InitCLUT(uint32_t * clut);
static void Generate_Julia_fpu(uint16_t size_x,
                       uint16_t size_y,
                       uint16_t offset_x,
                       uint16_t offset_y,
                       uint16_t zoom,
                       uint8_t * buffer);

static void CPU_CACHE_Enable(void);
static void MPU_Config(void);

static void DMA2D_Init(uint32_t ImageWidth, uint32_t ImageHeight);
static void DMA2D_CopyBuffer(uint32_t *pSrc, uint32_t *pDst, uint16_t ImageWidth, uint16_t ImageHeight);
static void DMA2D_CopyButton(uint32_t Button, uint32_t *pDst);
static void LCD_BriefDisplay(void);
static void Touch_Handler(void);
static void print_Size(void);
void Error_Handler(void);


/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* Configure the MPU attributes as Write Through for SDRAM*/
  MPU_Config();

  /* Enable the CPU Cache */
  CPU_CACHE_Enable();

  /* STM32H7xx HAL library initialization:
       - Systick timer is configured by default as source of time base, but user
         can eventually implement his proper time base source (a general purpose
         timer for application or other time source), keeping in mind that Time base
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and
         handled in milliseconds basis.
       - Set NVIC Group Priority to 4
       - Low Level Initialization: global MSP (MCU Support Package) initialization
   */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

#if (USE_VOS0_480MHZ_OVERCLOCK == 1)
  /* By default the FMC source clock is the AXI bus clock
     The maximum FMC allowed frequency is 200MHz
     when switching the system clock to 480MHz , the AXI bus clock is 240MHz:
       - In this case the FMC source clock is switched to PLL2 (DIVR) with 200MHz frequency
  */
  FMC_SDRAM_Clock_Config();
#endif /* (USE_VOS0_480MHZ_OVERCLOCK == 1) */

  /* Configure LED1 and LED3 */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED3);

  InitCLUT(L8_CLUT);
  /* Clean Data Cache to update the content of the SRAM to be used by the DAMA2D */
  SCB_CleanDCache_by_Addr( (uint32_t*)L8_CLUT, sizeof(L8_CLUT));

 /*##-1- LCD Configuration ##################################################*/
  /* Initialize the LCD   */
  BSP_LCD_Init(0, LCD_ORIENTATION_LANDSCAPE);
  UTIL_LCD_SetFuncDriver(&LCD_Driver);
  UTIL_LCD_SetLayer(0);
  LCD_BriefDisplay();

  BSP_LCD_GetXSize(0, &x_size);
  BSP_LCD_GetYSize(0, &y_size);
  /*##-2- Initialize the Touch Screen ##################################################*/
  hTS->Width = x_size;
  hTS->Height = y_size;
  hTS->Orientation = TS_SWAP_NONE;
  hTS->Accuracy = 0;
  BSP_TS_Init(0, hTS);
  BSP_TS_EnableIT(0);
  TS_State.TouchDetected = 0;

#if (USE_VOS0_480MHZ_OVERCLOCK == 1)
  /* Configure the Tamper push-button in EXTI Mode */
  BSP_PB_Init(BUTTON_TAMPER, BUTTON_MODE_EXTI);
#endif /*USE_VOS0_480MHZ_OVERCLOCK*/

  /* Init xsize and ysize used by fractal algo */
  XSize = SizeTable[SizeIndex][0];
  YSize = SizeTable[SizeIndex][1];

  /*print fractal image size*/
  print_Size();

  /*Copy Pause/Zoom in Off/Zoom out buttons*/
  DMA2D_CopyButton(PAUSE_BUTTON,(uint32_t *)LCD_FRAME_BUFFER);
  DMA2D_CopyButton(ZOOM_IN_OFF_BUTTON,(uint32_t *)LCD_FRAME_BUFFER);
  DMA2D_CopyButton(ZOOM_OUT_BUTTON,(uint32_t *)LCD_FRAME_BUFFER);

  DMA2D_Init(XSize, YSize);

  while(1)
  {
    if((isplaying != 0) || (playOneFrame != 0))
    {
      playOneFrame = 0;

      BSP_LED_On(LED1);

      tickstart = HAL_GetTick();
      /* Start generating the fractal */
      Generate_Julia_fpu(XSize,
                         YSize,
                         XSize / 2,
                         YSize / 2,
                         CurrentZoom,
                         (uint8_t *)buffer);

      /* Get elapsed time */
      score_fpu = (uint32_t)(HAL_GetTick() - tickstart);
      /* Clean Data Cache to update the content of the SRAM to be used by the DAMA2D */
      SCB_CleanDCache_by_Addr( (uint32_t*)buffer, sizeof(buffer));

      BSP_LED_Off(LED1);

      sprintf((char*)text, "%lu ms",score_fpu);
      UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
      UTIL_LCD_DisplayStringAt(440 + 32, 370 + 68 , (uint8_t *)"         ", LEFT_MODE);
      UTIL_LCD_DisplayStringAt(440 + 32, 370 + 68 , (uint8_t *)text, LEFT_MODE);

      sprintf((char*)text, "Zoom : %lu",CurrentZoom);
      UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
      UTIL_LCD_DisplayStringAt(440 + 8 , 370 + 8 , (uint8_t *)"           ", LEFT_MODE);
      UTIL_LCD_DisplayStringAt(440 + 8 , 370 + 8 , (uint8_t *)text, LEFT_MODE);
#if (USE_VOS0_480MHZ_OVERCLOCK == 1)
      sprintf((char*)text,"System Clock = %lu MHz",SystemClock_MHz);
      UTIL_LCD_DisplayStringAt(2, 50, (uint8_t *)text, CENTER_MODE);
#endif /* (USE_VOS0_480MHZ_OVERCLOCK == 1) */


      if(DirectionZoom == 0)
      {
        if (CurrentZoom < 1000)
        {
          CurrentZoom += 20;
        }
        else
        {
          DirectionZoom = 1;
        }
      }

      if(DirectionZoom == 1)
      {
        if (CurrentZoom > 100)
        {
          CurrentZoom -= 20;
        }
        else
        {
          DirectionZoom = 0;
        }
      }

      /* Copy Result image to the display frame buffer*/
      DMA2D_CopyBuffer((uint32_t *)buffer, (uint32_t *)LCD_FRAME_BUFFER, XSize, YSize);
    }

    Touch_Handler();
#if (USE_VOS0_480MHZ_OVERCLOCK == 1)
    SystemClockChange_Handler();
#endif /* (USE_VOS0_480MHZ_OVERCLOCK == 1) */

  }
}
/**
  * @brief  BSP TS Callback.
  * @param  Instance  TS instance. Could be only 0.
  * @retval None.
  */
void BSP_TS_Callback(uint32_t Instance)
{
    /* Get the IT status register value */
    BSP_TS_GetState(0, &TS_State);

    if(TS_State.TouchDetected)
    {
      if(TouchdOn < 4)
      {
        TouchdOn++;
      }
      else /*TouchReleased */
      {
        TouchdOn = 0;
        TouchReleased = 1;
      }
    }
}
static void Touch_Handler(void)
{
  if(TouchReleased != 0)
  {
    TouchReleased = 0;

    /*************************Pause/Play buttons *********************/
    if((TS_State.TouchX + SCREEN_SENSTIVITY >= PLAY_PAUSE_BUTTON_XPOS) && \
      (TS_State.TouchX <= (PLAY_PAUSE_BUTTON_XPOS + PLAY_PAUSE_BUTTON_WIDTH + SCREEN_SENSTIVITY)) && \
        (TS_State.TouchY + SCREEN_SENSTIVITY >= PLAY_PAUSE_BUTTON_YPOS) && \
          (TS_State.TouchY <= (PLAY_PAUSE_BUTTON_YPOS + PLAY_PAUSE_BUTTON_HEIGHT + SCREEN_SENSTIVITY)))
    {
      isplaying = 1 - isplaying;

      if(isplaying == 0)
      {
        DMA2D_CopyButton(PLAY_BUTTON,(uint32_t *)LCD_FRAME_BUFFER);
      }
      else
      {
        DMA2D_CopyButton(PAUSE_BUTTON,(uint32_t *)LCD_FRAME_BUFFER);
      }

      DMA2D_Init(XSize, YSize);
    }
    /*************************Zoom In button *********************/
    else if((TS_State.TouchX + SCREEN_SENSTIVITY >= ZOOM_IN_BUTTON_XPOS) && \
      (TS_State.TouchX <= (ZOOM_IN_BUTTON_XPOS + ZOOM_BUTTON_WIDTH + SCREEN_SENSTIVITY)) && \
        (TS_State.TouchY + SCREEN_SENSTIVITY >= ZOOM_IN_BUTTON_YPOS) && \
          (TS_State.TouchY <= (ZOOM_IN_BUTTON_YPOS + ZOOM_BUTTON_HEIGHT + SCREEN_SENSTIVITY)))
    {
      if (SizeIndex > 0)
      {
        SizeIndex--;
        XSize = SizeTable[SizeIndex][0];
        YSize = SizeTable[SizeIndex][1];

        if(SizeIndex == 0)
        {
          /*zoom in limit reached */
          DMA2D_CopyButton(ZOOM_IN_OFF_BUTTON,(uint32_t *)LCD_FRAME_BUFFER);
        }
        else if(SizeIndex == 4)
        {
          /* zoom out limit unreached display zoom out button */
          DMA2D_CopyButton(ZOOM_OUT_BUTTON,(uint32_t *)LCD_FRAME_BUFFER);
        }

        /*Clear Fractal Display area */
        UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLACK);
        UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLACK);
        UTIL_LCD_FillRect( 2, 112 + 1, 640 - 4, 254,UTIL_LCD_COLOR_BLACK);

        UTIL_LCD_SetBackColor(0xFF0080FF);
        UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);

        print_Size();
        CurrentZoom = 100;

        playOneFrame = 1;
        DMA2D_Init(XSize, YSize);
      }
    }
    /*************************Zoom Out button *********************/
    else if((TS_State.TouchX + SCREEN_SENSTIVITY >= ZOOM_OUT_BUTTON_XPOS) && \
      (TS_State.TouchX <= (ZOOM_OUT_BUTTON_XPOS + ZOOM_BUTTON_WIDTH + SCREEN_SENSTIVITY)) && \
        (TS_State.TouchY + SCREEN_SENSTIVITY >= ZOOM_OUT_BUTTON_YPOS) && \
          (TS_State.TouchY <= (ZOOM_OUT_BUTTON_YPOS + ZOOM_BUTTON_HEIGHT + SCREEN_SENSTIVITY)))
    {
      if (SizeIndex < 5)
      {
        SizeIndex++;
        XSize = SizeTable[SizeIndex][0];
        YSize = SizeTable[SizeIndex][1];

        if(SizeIndex == 5)
        {
          /* zoom out limit reached */
          DMA2D_CopyButton(ZOOM_OUT_OFF_BUTTON,(uint32_t *)LCD_FRAME_BUFFER);
        }
        else if (SizeIndex == 1)
        {
          /*zoom in limit unreached  Display zoom in button */
          DMA2D_CopyButton(ZOOM_IN_BUTTON,(uint32_t *)LCD_FRAME_BUFFER);
        }

        /*Clear Fractal Display area */
        UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLACK);
        UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLACK);
        UTIL_LCD_FillRect( 2, 112 + 1, 640 - 4, 254,UTIL_LCD_COLOR_BLACK);

        UTIL_LCD_SetBackColor(0xFF0080FF);
        UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);

        print_Size();
        CurrentZoom = 100;

        playOneFrame = 1;
        DMA2D_Init(XSize, YSize);
      }
    }
  }
}

#if (USE_VOS0_480MHZ_OVERCLOCK == 1)
static void SystemClockChange_Handler(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  if(SystemClock_changed != 0)
  {
    /*switch off the display during the system clock change */
    BSP_LCD_DisplayOff();

    /* Select HSE  as system clock source to allow modification of the PLL configuration */
    RCC_ClkInitStruct.ClockType       = RCC_CLOCKTYPE_SYSCLK;
    RCC_ClkInitStruct.SYSCLKSource    = RCC_SYSCLKSOURCE_HSE;
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
    {
      /* Initialization Error */
      Error_Handler();
    }


    if(SystemClock_MHz == 400)
    {
      __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);
      while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

      SystemClock_Config_480MHz();
      SystemClock_MHz = 480;
    }
    else
    {
      SystemClock_Config_400MHz();

      __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
      while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

      SystemClock_MHz = 400;
    }

    /* PLL1  as system clock source to allow modification of the PLL configuration */
    RCC_ClkInitStruct.ClockType       = RCC_CLOCKTYPE_SYSCLK;
    RCC_ClkInitStruct.SYSCLKSource    = RCC_SYSCLKSOURCE_PLLCLK;
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
    {
      /* Initialization Error */
      Error_Handler();
    }

    CurrentZoom = 100;
    playOneFrame = 1;

    SystemClock_changed = 0;

    /*switch off the display during the system clock change */
    BSP_LCD_DisplayOn();

  }
}
#endif /* (USE_VOS0_480MHZ_OVERCLOCK == 1) */

/**
  * @brief  Button Callback
  * @param  Button Specifies the pin connected EXTI line
  * @retval None
  */
#if (USE_VOS0_480MHZ_OVERCLOCK == 1)
void BSP_PB_Callback(Button_TypeDef Button)
{
  if(Button == BUTTON_TAMPER)
  {
    SystemClock_changed = 1;
  }
}
#endif

static void print_Size(void)
{
  sprintf((char*)text, "%u x %u",XSize,YSize);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_DisplayStringAt(64, 370 + 24 , (uint8_t *)"Size", LEFT_MODE);
  UTIL_LCD_DisplayStringAt(24, 370 + 48 , (uint8_t *)text, LEFT_MODE);

}

static void InitCLUT(uint32_t * clut)
{
  uint32_t  red = 0, green = 0, blue = 0;
  uint32_t  i = 0x00;

  /* Color map generation */
  for (i = 0; i < ITERATION; i++)
  {
    /* Generate red, green and blue values */
    red = (i * 8 * 256 / ITERATION) % 256;
    green = (i * 6 * 256 / ITERATION) % 256;
    blue = (i * 4 * 256 / ITERATION) % 256;

    red = red << 16;
    green = green << 8;

    /* Store the 32-bit value */
    clut[i] = 0xFF000000 | (red + green + blue);
  }
}

static void Generate_Julia_fpu(uint16_t size_x, uint16_t size_y, uint16_t offset_x, uint16_t offset_y, uint16_t zoom, uint8_t * buffer)
{
  float       tmp1, tmp2;
  float       num_real, num_img;
  float       rayon;

  uint8_t       i;
  uint16_t      x, y;

  for (y = 0; y < size_y; y++)
  {
    for (x = 0; x < size_x; x++)
    {
      num_real = y - offset_y;
      num_real = num_real / zoom;
      num_img = x - offset_x;
      num_img = num_img / zoom;
      i = 0;
      rayon = 0;
      while ((i < ITERATION - 1) && (rayon < 4))
      {
        tmp1 = num_real * num_real;
        tmp2 = num_img * num_img;
        num_img = 2 * num_real * num_img + IMG_CONSTANT;
        num_real = tmp1 - tmp2 + REAL_CONSTANT;
        rayon = tmp1 + tmp2;
        i++;
      }
      /* Store the value in the buffer */
      buffer[x+y*size_x] = i;
    }
  }
}

static void LCD_BriefDisplay(void)
{
#if (USE_VOS0_480MHZ_OVERCLOCK == 1)
  char message[64];
#endif

  UTIL_LCD_SetFont(&Font24);
  UTIL_LCD_Clear(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_FillRect(2, 2, 640 - 4, 112 - 2,0xFF0080FF);
  UTIL_LCD_SetBackColor(0xFF0080FF);
  /*Title*/
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_DisplayStringAt(2, 24, (uint8_t *)"STM32H7xx Fractal Benchmark", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(2, 72, (uint8_t *)SCORE_FPU_MODE, CENTER_MODE);
#if (USE_VOS0_480MHZ_OVERCLOCK == 1)
  sprintf((char*)message,"System Clock = %lu MHz",SystemClock_MHz);
  UTIL_LCD_DisplayStringAt(2, 50, (uint8_t *)message, CENTER_MODE);
  UTIL_LCD_SetFont(&Font12);
  UTIL_LCD_DisplayStringAt(2, 94, (uint8_t *)"Press Tamper button to switch the System Clock 400MHz/480MHz", CENTER_MODE);
  UTIL_LCD_SetFont(&Font24);

#else
  UTIL_LCD_DisplayStringAt(2, 50, (uint8_t *)"System Clock = 400MHz", CENTER_MODE);
#endif

  /*Fractal Display area */
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLACK);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLACK);
  UTIL_LCD_FillRect(2, 112 + 1, 640 - 4, 254,UTIL_LCD_COLOR_BLACK);



  /*image Size*/
  UTIL_LCD_SetBackColor(0xFF0080FF);
  UTIL_LCD_SetTextColor(0xFF0080FF);
  UTIL_LCD_FillRect(2, 370, 180 - 2, 112 - 4,0xFF0080FF);

  /*Button area */
  UTIL_LCD_SetBackColor(0xFF0080FF);
  UTIL_LCD_SetTextColor(0xFF0080FF);
  UTIL_LCD_FillRect(182, 370, 260 - 2, 112 - 4,0xFF0080FF);

  /*Calculation Time*/
  UTIL_LCD_SetBackColor(0xFF0080FF);
  UTIL_LCD_SetTextColor(0xFF0080FF);
  UTIL_LCD_FillRect(442, 370, 200 - 4, 112 - 4,0xFF0080FF);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_DisplayStringAt(440 + 32, 370 + 48 , (uint8_t *)"Duration:", LEFT_MODE);

}

/**
  * @brief  Initialize the DMA2D in memory to memory with PFC.
  * @param  ImageWidth: image width
  * @param  ImageHeight: image Height
  * @retval None
  */
static void DMA2D_Init(uint32_t ImageWidth, uint32_t ImageHeight)
{
  DMA2D_CLUTCfgTypeDef CLUTCfg;

  /* Init DMA2D */
  /*##-1- Configure the DMA2D Mode, Color Mode and output offset #############*/
  DMA2D_Handle.Init.Mode         = DMA2D_M2M_PFC;
  DMA2D_Handle.Init.ColorMode    = DMA2D_OUTPUT_ARGB8888;
  DMA2D_Handle.Init.OutputOffset = LCD_X_SIZE - ImageWidth;

  /*##-2- DMA2D Callbacks Configuration ######################################*/
  DMA2D_Handle.XferCpltCallback  = NULL;

  /*##-3- Foreground Configuration ###########################################*/
  DMA2D_Handle.LayerCfg[1].AlphaMode = DMA2D_REPLACE_ALPHA;
  DMA2D_Handle.LayerCfg[1].InputAlpha = 0xFF;

  DMA2D_Handle.LayerCfg[1].InputColorMode = DMA2D_INPUT_L8;
  DMA2D_Handle.LayerCfg[1].InputOffset = 0;

  DMA2D_Handle.Instance          = DMA2D;

  /*##-4- DMA2D Initialization     ###########################################*/
  HAL_DMA2D_Init(&DMA2D_Handle);
  HAL_DMA2D_ConfigLayer(&DMA2D_Handle, 1);

  /* Load DMA2D Foreground CLUT */
  CLUTCfg.CLUTColorMode = DMA2D_CCM_ARGB8888;
  CLUTCfg.pCLUT = (uint32_t *)L8_CLUT;
  CLUTCfg.Size = 255;

  HAL_DMA2D_CLUTLoad(&DMA2D_Handle, CLUTCfg, 1);
  HAL_DMA2D_PollForTransfer(&DMA2D_Handle, 100);
}

/**
  * @brief  Copy the Decoded image to the display Frame buffer.
  * @param  pSrc: Pointer to source buffer
  * @param  pDst: Pointer to destination buffer
  * @param  ImageWidth: image width
  * @param  ImageHeight: image Height
  * @retval None
  */
static void DMA2D_CopyBuffer(uint32_t *pSrc, uint32_t *pDst, uint16_t ImageWidth, uint16_t ImageHeight)
{

  uint32_t xPos, yPos, destination;

  /*##-1- calculate the destination transfer address  ############*/
  xPos = (LCD_X_SIZE - ImageWidth)/2;
  yPos = ((LCD_Y_SIZE - ImageHeight)/2);

  destination = (uint32_t)pDst + (yPos * LCD_X_SIZE + xPos) * 4;


  HAL_DMA2D_Start(&DMA2D_Handle, (uint32_t)pSrc, destination, ImageWidth, ImageHeight);
  HAL_DMA2D_PollForTransfer(&DMA2D_Handle, 100);

}

static void DMA2D_CopyButton(uint32_t Button, uint32_t *pDst)
{
  uint32_t xPos = 0, yPos =0, destination, buttonWidth = PLAY_PAUSE_BUTTON_WIDTH;
  uint32_t *pSrc = (uint32_t *)Play_Button;

  if(PLAY_BUTTON == Button)
  {
    xPos = PLAY_PAUSE_BUTTON_XPOS;
    yPos = PLAY_PAUSE_BUTTON_YPOS;

    buttonWidth = PLAY_PAUSE_BUTTON_WIDTH;

    pSrc = (uint32_t *)Play_Button;
  }
  else if(PAUSE_BUTTON == Button)
  {
    xPos = PLAY_PAUSE_BUTTON_XPOS;
    yPos = PLAY_PAUSE_BUTTON_YPOS;

    buttonWidth = PLAY_PAUSE_BUTTON_WIDTH;

    pSrc = (uint32_t *)Pause_Button;
  }
  else if(ZOOM_IN_BUTTON == Button)
  {
    xPos = ZOOM_IN_BUTTON_XPOS;
    yPos = ZOOM_IN_BUTTON_YPOS;

    buttonWidth = ZOOM_BUTTON_WIDTH;

    pSrc = (uint32_t *)ZOOM_IN_Button;
  }
  else if(ZOOM_OUT_BUTTON == Button)
  {
    xPos = ZOOM_OUT_BUTTON_XPOS;
    yPos = ZOOM_OUT_BUTTON_YPOS;

    buttonWidth = ZOOM_BUTTON_WIDTH;

    pSrc = (uint32_t *)ZOOM_Out_Button;
  }
  else if(ZOOM_IN_OFF_BUTTON == Button)
  {
    xPos = ZOOM_IN_BUTTON_XPOS;
    yPos = ZOOM_IN_BUTTON_YPOS;

    buttonWidth = ZOOM_BUTTON_WIDTH;

    pSrc = (uint32_t *)ZOOM_IN_Off_Button;
  }
  else if(ZOOM_OUT_OFF_BUTTON == Button)
  {
    xPos = ZOOM_OUT_BUTTON_XPOS;
    yPos = ZOOM_OUT_BUTTON_YPOS;

    buttonWidth = ZOOM_BUTTON_WIDTH;

    pSrc = (uint32_t *)ZOOM_Out_Off_Button;
  }

  /* Init DMA2D */
  /*##-1- Configure the DMA2D Mode, Color Mode and output offset #############*/
  DMA2D_Handle.Init.Mode         = DMA2D_M2M_PFC;
  DMA2D_Handle.Init.ColorMode    = DMA2D_OUTPUT_ARGB8888;
  DMA2D_Handle.Init.OutputOffset = LCD_X_SIZE - buttonWidth;

  /*##-2- DMA2D Callbacks Configuration ######################################*/
  DMA2D_Handle.XferCpltCallback  = NULL;

  /*##-3- Foreground Configuration ###########################################*/
  DMA2D_Handle.LayerCfg[1].AlphaMode = DMA2D_REPLACE_ALPHA;
  DMA2D_Handle.LayerCfg[1].InputAlpha = 0xFF;
  DMA2D_Handle.LayerCfg[1].InputColorMode = DMA2D_INPUT_ARGB8888;
  DMA2D_Handle.LayerCfg[1].InputOffset = 0;

  DMA2D_Handle.Instance          = DMA2D;

  /*##-4- DMA2D Initialization     ###########################################*/
  HAL_DMA2D_Init(&DMA2D_Handle);
  HAL_DMA2D_ConfigLayer(&DMA2D_Handle, 1);



  destination = (uint32_t)pDst + (yPos * LCD_X_SIZE + xPos) * 4;

  HAL_DMA2D_Start(&DMA2D_Handle, (uint32_t)pSrc, destination, PLAY_PAUSE_BUTTON_WIDTH, PLAY_PAUSE_BUTTON_HEIGHT);
  HAL_DMA2D_PollForTransfer(&DMA2D_Handle, 100);

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

/**
  * @brief  System Clock Configuration to 400MHz
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 400000000 (CPU Clock)
  *            HCLK(Hz)                       = 200000000 (AXI and AHBs Clock)
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

#if (USE_VOS0_480MHZ_OVERCLOCK == 1)

/**
  * @brief  System Clock Configuration to 480MHz
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 480000000 (CPU Clock)
  *            HCLK(Hz)                       = 240000000 (AXI and AHBs Clock)
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

/**
  * @brief  Configure the FMC/SDRAM source clock to PLL2 at 200MHz.
  * @param  None
  * @retval None
  */
static void FMC_SDRAM_Clock_Config(void)
{
  RCC_PeriphCLKInitTypeDef  RCC_PeriphCLKInitStruct;
  HAL_StatusTypeDef ret = HAL_OK;

  /* PLL2_VCO Input = HSE_VALUE/PLL2_M = 5 Mhz */
  /* PLL2_VCO Output = PLL2_VCO Input * PLL_N = 800 Mhz */
  /* FMC Kernel Clock = PLL2_VCO Output/PLL_R = 800/4 = 200 Mhz */
  RCC_PeriphCLKInitStruct.PeriphClockSelection = RCC_PERIPHCLK_FMC;
  RCC_PeriphCLKInitStruct.FmcClockSelection = RCC_FMCCLKSOURCE_PLL2;
  RCC_PeriphCLKInitStruct.PLL2.PLL2RGE = RCC_PLL1VCIRANGE_2;
  RCC_PeriphCLKInitStruct.PLL2.PLL2M = 5;
  RCC_PeriphCLKInitStruct.PLL2.PLL2N = 160;
  RCC_PeriphCLKInitStruct.PLL2.PLL2FRACN = 0;
  RCC_PeriphCLKInitStruct.PLL2.PLL2P = 2;
  RCC_PeriphCLKInitStruct.PLL2.PLL2R = 4;
  RCC_PeriphCLKInitStruct.PLL2.PLL2Q = 4;
  RCC_PeriphCLKInitStruct.PLL2.PLL2VCOSEL = RCC_PLL2VCOWIDE;
  ret = HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphCLKInitStruct);
  if(ret != HAL_OK)
  {
    Error_Handler();
  }
}
#endif /* USE_VOS0_480MHZ_OVERCLOCK */


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
  * @brief  Configure the MPU attributes as Write Through for External SDRAM.
  * @note   The Base Address is SDRAM_DEVICE_ADDR .
  *         The Configured Region Size is 32MB because same as SDRAM size.
  * @param  None
  * @retval None
  */
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

  /* Configure the MPU attributes as WT for SDRAM */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress = SDRAM_DEVICE_ADDR;
  MPU_InitStruct.Size = MPU_REGION_SIZE_32MB;
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
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  BSP_LED_On(LED3);
  while(1);
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/**
  * @}
  */

