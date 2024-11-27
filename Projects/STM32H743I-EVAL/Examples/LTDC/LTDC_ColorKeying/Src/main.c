/**
  ******************************************************************************
  * @file    LTDC/LTDC_ColorKeying/Src/main.c 
  * @author  MCD Application Team
  * @brief   This example describes how to enable and use 
  *          the LTDC color keying functionality.
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
#include "ARGB4444_480x272.h"

/** @addtogroup STM32H7xx_HAL_Examples
  * @{
  */

/** @addtogroup LTDC_ColorKeying
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define PRESSED_FIRST      0x00
#define PRESSED_SECOND     0x01
#define ARGB4444_COLOR_KEY  0xFFFF
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
LTDC_HandleTypeDef LtdcHandle;
uint8_t ubPressedButton = PRESSED_FIRST;

__IO uint32_t ReloadFlag = 0;

/* Private function prototypes -----------------------------------------------*/
static void MPU_Config(void);
static void LCD_Config(void); 
static void SystemClock_Config(void);
static void Error_Handler(void);
static void CPU_CACHE_Enable(void);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief   Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  static uint8_t buttonState = 0;  
  RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;

  /* Configure the MPU attributes */
  MPU_Config();

  /* Enable the CPU Cache */
  CPU_CACHE_Enable();
  
  /* STM32H7xx HAL library initialization:
       - Systick timer is configured by default as source of time base, but user 
         can eventually implement his proper time base source (a general purpose 
         timer for example or other time source), keeping in mind that Time base 
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
         handled in milliseconds basis.
       - Set NVIC Group Priority to 4
       - Low Level Initialization
     */
  HAL_Init();
  
  /* Configure the system clock to 400 MHz */
  SystemClock_Config();

  /* AMPIRE640480 typical PCLK is 25 MHz so the PLL3R is configured to provide this clock */ 
  /* AMPIRE640480 LCD clock configuration */
  /* PLL3_VCO Input = HSE_VALUE/PLL3M = 5 Mhz */
  /* PLL3_VCO Output = PLL3_VCO Input * PLL3N = 800 Mhz */
  /* PLLLCDCLK = PLL3_VCO Output/PLL3R = 800/32 = 25Mhz */
  /* LTDC clock frequency = PLLLCDCLK = 25 Mhz */    
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
  PeriphClkInitStruct.PLL3.PLL3M = 5;    
  PeriphClkInitStruct.PLL3.PLL3N = 160;
  PeriphClkInitStruct.PLL3.PLL3FRACN = 0;
  PeriphClkInitStruct.PLL3.PLL3P = 2;
  PeriphClkInitStruct.PLL3.PLL3Q = 2;
  PeriphClkInitStruct.PLL3.PLL3R = 32;
  PeriphClkInitStruct.PLL3.PLL3VCOSEL = RCC_PLL3VCOWIDE;
  PeriphClkInitStruct.PLL3.PLL3RGE = RCC_PLL3VCIRANGE_2;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);

  
  
  /* Configure LED3 */
  BSP_LED_Init(LED3);   
  
  /*##-1- TAMPER button will be used #########################################*/
  BSP_PB_Init(BUTTON_TAMPER, BUTTON_MODE_GPIO);
  
  /*##-2- LCD Configuration ##################################################*/
  LCD_Config();
  
  /*##-3- Configure Color Keying  ############################################*/
  HAL_LTDC_ConfigColorKeying_NoReload(&LtdcHandle, ARGB4444_COLOR_KEY, LTDC_LAYER_1);  

  while (1)
  {

    if(BSP_PB_GetState(BUTTON_TAMPER) == GPIO_PIN_SET)
    {
      /* user button is pressed     */
      buttonState = 1;
    }
    else if(buttonState == 1)
    {
      /* user button is released     */
      buttonState = 0;
    
      if(ubPressedButton == PRESSED_FIRST)
      {
        /* Enable Color Keying on layer 1 */
        HAL_LTDC_EnableColorKeying_NoReload(&LtdcHandle, LTDC_LAYER_1);
        ubPressedButton = PRESSED_SECOND;  
      }
      else
      {
        /* Disable Color Keying on layer 1 */
        HAL_LTDC_DisableColorKeying_NoReload(&LtdcHandle, LTDC_LAYER_1);
        ubPressedButton = PRESSED_FIRST; 
      }
      
      ReloadFlag = 0;
      HAL_LTDC_Reload(&LtdcHandle,LTDC_RELOAD_VERTICAL_BLANKING);
      
      while(ReloadFlag == 0)
      {
        /* wait till reload takes effect (in the next vertical blanking period) */
      }    
    }
  }
}

/**
  * @brief  System Clock Configuration
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
  *            VDD(V)                         = 3.3
  *            Flash Latency(WS)              = 4
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  HAL_StatusTypeDef ret = HAL_OK;

  /* The voltage scaling allows optimizing the power consumption when the device is
     clocked below the maximum system frequency, to update the voltage scaling value
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}
 
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
    while(1) { ; }
  }
  
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
    while(1) { ; }
  }
}

/**
  * @brief LCD configuration.
  * @note  This function Configure the LTDC peripheral :
  *        1) Configure the Pixel Clock for the LCD
  *        2) Configure the LTDC Timing and Polarity
  *        3) Configure the LTDC Layer 1 :
  *           - The frame buffer is located in FLASH memory
  *           - The Layer size configuration : 480x272 and format ARGB4444 (16 bits/pixel)
  *        4) Configure the Color Keying for Layer 1.    
  * @retval
  *  None
  */
static void LCD_Config(void)
{
  LTDC_LayerCfgTypeDef      pLayerCfg;
  
  /* LTDC Initialization -------------------------------------------------------*/
  
  /* Polarity configuration */
  /* Initialize the horizontal synchronization polarity as active low */
  LtdcHandle.Init.HSPolarity = LTDC_HSPOLARITY_AL;
  /* Initialize the vertical synchronization polarity as active low */ 
  LtdcHandle.Init.VSPolarity = LTDC_VSPOLARITY_AL; 
  /* Initialize the data enable polarity as active low */ 
  LtdcHandle.Init.DEPolarity = LTDC_DEPOLARITY_AL; 
  /* Initialize the pixel clock polarity as input pixel clock */  
  LtdcHandle.Init.PCPolarity = LTDC_PCPOLARITY_IPC;
  
  /* The LCD AMPIRE 640x480 is selected */
  /* Timing configuration */
  LtdcHandle.Init.HorizontalSync = (AMPIRE640480_HSYNC - 1);
  LtdcHandle.Init.VerticalSync = (AMPIRE640480_VSYNC - 1);
  LtdcHandle.Init.AccumulatedHBP = (AMPIRE640480_HSYNC + AMPIRE640480_HBP - 1);
  LtdcHandle.Init.AccumulatedVBP = (AMPIRE640480_VSYNC + AMPIRE640480_VBP - 1);  
  LtdcHandle.Init.AccumulatedActiveH = (AMPIRE640480_HEIGHT + AMPIRE640480_VSYNC + AMPIRE640480_VBP - 1);
  LtdcHandle.Init.AccumulatedActiveW = (AMPIRE640480_WIDTH + AMPIRE640480_HSYNC + AMPIRE640480_HBP - 1);
  LtdcHandle.Init.TotalHeigh = (AMPIRE640480_HEIGHT + AMPIRE640480_VSYNC + AMPIRE640480_VBP + AMPIRE640480_VFP - 1);
  LtdcHandle.Init.TotalWidth = (AMPIRE640480_WIDTH + AMPIRE640480_HSYNC + AMPIRE640480_HBP + AMPIRE640480_HFP - 1); 
  
  /* Configure R,G,B component values for LCD background color : all black */
  LtdcHandle.Init.Backcolor.Blue = 0;
  LtdcHandle.Init.Backcolor.Green = 0;
  LtdcHandle.Init.Backcolor.Red = 0;

  LtdcHandle.Instance = LTDC;
  
  /* Layer1 Configuration ------------------------------------------------------*/
  
  /* Windowing configuration */ 
  /* In this case all the active display area is used to display a picture then :
     Horizontal start = horizontal synchronization + Horizontal back porch = 43 
     Vertical start   = vertical synchronization + vertical back porch     = 12
     Horizontal stop = Horizontal start + window width -1 = 43 + 480 -1 
     Vertical stop   = Vertical start + window height -1  = 12 + 272 -1      */
  pLayerCfg.WindowX0 = 0;
  pLayerCfg.WindowX1 = 480;
  pLayerCfg.WindowY0 = 0;
  pLayerCfg.WindowY1 = 272;
  
  /* Pixel Format configuration */ 
  pLayerCfg.PixelFormat = LTDC_PIXEL_FORMAT_ARGB4444;
  
  /* Start Address configuration : frame buffer is located in FLASH memory */
  pLayerCfg.FBStartAdress = (uint32_t)&ARGB4444_480x272;
  
  /* Alpha constant (255 totally opaque) */
  pLayerCfg.Alpha = 255;
  
  /* Default Color configuration (configure A,R,G,B component values) */
  /* default color == all transparent pixel                           */
  pLayerCfg.Alpha0 = 0; /* completely transparent */
  pLayerCfg.Backcolor.Blue = 0;
  pLayerCfg.Backcolor.Green = 0;
  pLayerCfg.Backcolor.Red = 0;
  
  /* Configure blending factors */
  pLayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_CA;
  pLayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_CA;
  
  /* Configure the number of lines and number of pixels per line */
  pLayerCfg.ImageWidth  = 480;
  pLayerCfg.ImageHeight = 272;

  /* Configure the LTDC */  
  if(HAL_LTDC_Init(&LtdcHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler(); 
  }
  
  /* Configure the Layer 1 */
  if(HAL_LTDC_ConfigLayer(&LtdcHandle, &pLayerCfg, LTDC_LAYER_1) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler(); 
  }    
}  

/**
  * @brief  Reload Event callback.
  * @param  hltdc: pointer to a LTDC_HandleTypeDef structure that contains
  *                the configuration information for the LTDC.
  * @retval None
  */
void HAL_LTDC_ReloadEventCallback(LTDC_HandleTypeDef *hltdc)
{
  ReloadFlag = 1;
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
    /* Turn LED3 on */
    BSP_LED_On(LED3);
    while(1)
    {
    }
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
  * @brief  Configure the MPU attributes
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

  /* Enable the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
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

