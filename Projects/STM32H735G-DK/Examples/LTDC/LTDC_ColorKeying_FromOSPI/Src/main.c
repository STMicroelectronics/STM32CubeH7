/**
  ******************************************************************************
  * @file    LTDC/LTDC_ColorKeying_FromOSPI/Src/main.c 
  * @author  MCD Application Team
  * @brief   This example describes how to enable and use 
  *          the LTDC color keying functionality.
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

/** @addtogroup LTDC_ColorKeying_FromOSPI
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define PRESSED_FIRST      0x00
#define PRESSED_SECOND     0x01
#define ARGB4444_COLOR_KEY  0xFFFFFF

#define LCD_BUFFER_ADDRESS 0x90000000
#define IMAGE_WIDTH 480;
#define IMAGE_HEIGHT 270;

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
LTDC_HandleTypeDef hltdc;
uint8_t ubPressedButton = PRESSED_FIRST;
__IO uint32_t ReloadFlag = 0;

/* Private function prototypes -----------------------------------------------*/
static void MPU_Config(void);
static void OCTOSPI_Config(void);
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
  
  /* Configure the system clock to 520 MHz */
  SystemClock_Config();
  
  /* Configure LED2 */
  BSP_LED_Init(LED2);   

  /*##-1- TAMPER button will be used #########################################*/
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);
  
  /*##-2- OSPI Configuration ##################################################*/
  OCTOSPI_Config();
  
  /*##-3- LCD Configuration ##################################################*/
  LCD_Config();
  
  /*##-4- Configure Color Keying  ############################################*/
  HAL_LTDC_ConfigColorKeying_NoReload(&hltdc, ARGB4444_COLOR_KEY, LTDC_LAYER_1);  
  
  while (1)
  {
    if(BSP_PB_GetState(BUTTON_USER) == GPIO_PIN_SET)
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
        HAL_LTDC_EnableColorKeying_NoReload(&hltdc, LTDC_LAYER_1);
        ubPressedButton = PRESSED_SECOND;  
      }
      else
      {
        /* Disable Color Keying on layer 1 */
        HAL_LTDC_DisableColorKeying_NoReload(&hltdc, LTDC_LAYER_1);
        ubPressedButton = PRESSED_FIRST; 
      }
      
      ReloadFlag = 0;
      HAL_LTDC_Reload(&hltdc,LTDC_RELOAD_VERTICAL_BLANKING);
      
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
  *            SYSCLK(Hz)                     = 520000000 (CPU Clock)
  *            HCLK(Hz)                       = 260000000 (AXI and AHBs Clock)
  *            AHB Prescaler                  = 2
  *            D1 APB3 Prescaler              = 2 (APB3 Clock  130MHz)
  *            D2 APB1 Prescaler              = 2 (APB1 Clock  130MHz)
  *            D2 APB2 Prescaler              = 2 (APB2 Clock  130MHz)
  *            D3 APB4 Prescaler              = 2 (APB4 Clock  130MHz)
  *            HSE Frequency(Hz)              = 25000000
  *            PLL_M                          = 5
  *            PLL_N                          = 104
  *            PLL_P                          = 1
  *            PLL_Q                          = 4
  *            PLL_R                          = 2
  *            VDD(V)                         = 3.3
  *            Flash Latency(WS)              = 3
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
  RCC_OscInitStruct.CSIState = RCC_CSI_OFF;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;

  RCC_OscInitStruct.PLL.PLLM = 5;
  RCC_OscInitStruct.PLL.PLLN = 104;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  RCC_OscInitStruct.PLL.PLLP = 1;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLQ = 4;

  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
  if(ret != HAL_OK)
  {
    Error_Handler();
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
  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3);
  if(ret != HAL_OK)
  {
    Error_Handler();
  }

/*
  Note : The activation of the I/O Compensation Cell is recommended with communication  interfaces
          (GPIO, SPI, FMC, OSPI ...)  when  operating at  high frequencies(please refer to product datasheet)
          The I/O Compensation Cell activation  procedure requires :
        - The activation of the CSI clock
        - The activation of the SYSCFG clock
        - Enabling the I/O Compensation Cell : setting bit[0] of register SYSCFG_CCCSR
*/


  __HAL_RCC_CSI_ENABLE() ;

  __HAL_RCC_SYSCFG_CLK_ENABLE() ;

  HAL_EnableCompensationCell();

}

/**
  * @brief LCD configuration.
  * @note  This function Configure the LTDC peripheral :
  *        1) Configure the Pixel Clock for the LCD
  *        2) Configure the LTDC Timing and Polarity
  *        3) Configure the LTDC Layer 1 :
  *           - The frame buffer is located in FLASH memory
  *           - The Layer size configuration : 800x480 and format ARGB4444 (16 bits/pixel)
  *        4) Configure the Color Keying for Layer 1.    
  * @retval
  *  None
  */
static void LCD_Config()
{
  LTDC_LayerCfgTypeDef      pLayerCfg;
  
  /* LTDC Initialization -------------------------------------------------------*/
  
  /* Polarity configuration */
  /* Initialize the horizontal synchronization polarity as active low */
  hltdc.Init.HSPolarity = LTDC_HSPOLARITY_AL;
  /* Initialize the vertical synchronization polarity as active low */ 
  hltdc.Init.VSPolarity = LTDC_VSPOLARITY_AL; 
  /* Initialize the data enable polarity as active low */ 
  hltdc.Init.DEPolarity = LTDC_DEPOLARITY_AL; 
  /* Initialize the pixel clock polarity as input pixel clock */  
  hltdc.Init.PCPolarity = LTDC_PCPOLARITY_IPC;
  
  /* Timing configuration */
    /* The LCD  480x272 is selected */
    /* Timing configuration */
  hltdc.Init.HorizontalSync     = RK043FN48H_HSYNC - 1;
  hltdc.Init.AccumulatedHBP     = (RK043FN48H_HSYNC + (RK043FN48H_HBP-11) - 1);
  hltdc.Init.AccumulatedActiveW = RK043FN48H_HSYNC + RK043FN48H_WIDTH + RK043FN48H_HBP - 1;
  hltdc.Init.TotalWidth         = RK043FN48H_HSYNC + RK043FN48H_WIDTH + (RK043FN48H_HBP-11) + RK043FN48H_HFP - 1;
  hltdc.Init.VerticalSync       = RK043FN48H_VSYNC - 1;
  hltdc.Init.AccumulatedVBP     = RK043FN48H_VSYNC + RK043FN48H_VBP - 1;
  hltdc.Init.AccumulatedActiveH = RK043FN48H_VSYNC + RK043FN48H_HEIGHT + RK043FN48H_VBP - 1;
  hltdc.Init.TotalHeigh         = RK043FN48H_VSYNC + RK043FN48H_HEIGHT + RK043FN48H_VBP + RK043FN48H_VFP - 1;

  
  /* Configure R,G,B component values for LCD background color : all black */
  hltdc.Init.Backcolor.Blue = 0;
  hltdc.Init.Backcolor.Green = 0;
  hltdc.Init.Backcolor.Red = 0;

  hltdc.Instance = LTDC;
  
  /* Layer1 Configuration ------------------------------------------------------*/
  
  /* Windowing configuration */ 
  pLayerCfg.WindowX0 = 0;
  pLayerCfg.WindowX1 = IMAGE_WIDTH;
  pLayerCfg.WindowY0 = 0;
  pLayerCfg.WindowY1 = IMAGE_HEIGHT;
  
  /* Pixel Format configuration */ 
  pLayerCfg.PixelFormat = LTDC_PIXEL_FORMAT_ARGB4444;
  
  /* Start Address configuration : Frame buffer is located at OSPI memory */
  pLayerCfg.FBStartAdress = LCD_BUFFER_ADDRESS;  
  
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
  pLayerCfg.ImageWidth  = IMAGE_WIDTH;
  pLayerCfg.ImageHeight = IMAGE_HEIGHT;

  /* Configure the LTDC */  
  if(HAL_LTDC_Init(&hltdc) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler(); 
  }
  
  /* Configure the Layer 1 */
  if(HAL_LTDC_ConfigLayer(&hltdc, &pLayerCfg, LTDC_LAYER_1) != HAL_OK)
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
  * @brief OSPI configuration.
  * @note  This function Configure the OSPI peripheral :
  *        1) Initialize OctoSPI 
  *        2) Configure the memory in octal mode
  *        3) Memory-mapped mode configuration  
  * @retval
  *  None
  */
static void OCTOSPI_Config(void)
{
  BSP_OSPI_NOR_Init_t ospi_flash ;
  
  ospi_flash.InterfaceMode = BSP_OSPI_NOR_OPI_MODE;
  ospi_flash.TransferRate  = BSP_OSPI_NOR_DTR_TRANSFER;
  
  if(BSP_OSPI_NOR_Init(0,&ospi_flash) != BSP_ERROR_NONE)
  {
    Error_Handler();
  }
  
  if(BSP_OSPI_NOR_EnableMemoryMappedMode(0) != BSP_ERROR_NONE)
  {
    Error_Handler();
  }
}


/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* Turn LED3 on */
  BSP_LED_On(LED2);
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

  /* Configure the MPU attributes as WT for OctoSPI RAM */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress = 0x90000000 ; 
  MPU_InitStruct.Size = MPU_REGION_SIZE_16MB;
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

