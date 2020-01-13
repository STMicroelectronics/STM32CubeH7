/**
  ******************************************************************************
  * @file    Examples_MIX/DMA2D/DMA2D_MemToMemWithLCD/Src/main.c
  * @author  MCD Application Team
  * @brief   This example provides a description of how to configure 
  *          DMA2D peripheral in Memory to Memory transfer mode 
  *          and display the result on LCD, in resorting to DMA2D LL APIs.
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
#include "RGB565_240x160.h"

/** @addtogroup STM32H7xx_HAL_Examples
  * @{
  */

/** @addtogroup DMA2D_MemToMemWithLCD
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Blended image Ready flag */
__IO uint32_t   blended_image_ready;
__IO uint32_t   dma2d_error_status;

/* Private function prototypes -----------------------------------------------*/
static void DMA2D_Config(void);
static void SystemClock_Config(void);
static void CPU_CACHE_Enable(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief   Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  uint32_t lcd_width, lcd_height, i, j;

  blended_image_ready = 0;
  dma2d_error_status = 0;
  
  
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
  
  /* Configure the system clock */
  SystemClock_Config(); 
    
  /* Configure LED2 and LED3 */
  BSP_LED_Init(LED2);
  BSP_LED_Init(LED3);

/*##-1- LCD Configuration ##################################################*/
  BSP_LCD_InitEx(0, LCD_ORIENTATION_LANDSCAPE, LCD_PIXEL_FORMAT_RGB565, LCD_DEFAULT_WIDTH, LCD_DEFAULT_HEIGHT);

  /* Get the LCD width and height */
  BSP_LCD_GetXSize(0, &lcd_width);
  BSP_LCD_GetYSize(0, &lcd_height);

  /* Clear the LCD */
  for(i = 0; i < lcd_height; i++)
  {
    for(j = 0; j < lcd_width; j++)
    {
      BSP_LCD_WritePixel(0, j, i, LCD_COLOR_RGB565_WHITE);
    }
  }
  
  /*##-2- DMA2D configuration ################################################*/
  LL_AHB3_GRP1_EnableClock(LL_AHB3_GRP1_PERIPH_DMA2D);
  NVIC_SetPriority(DMA2D_IRQn, 0);  
  NVIC_EnableIRQ(DMA2D_IRQn);
  
  DMA2D_Config();
  
  /*##-3- Start DMA2D transfer ###############################################*/  
  LL_DMA2D_FGND_SetMemAddr(DMA2D, (uint32_t)&RGB565_240x160);      /* Source buffer in format RGB565 and size 240x160                 */
  LL_DMA2D_SetOutputMemAddr(DMA2D, (uint32_t)LCD_LAYER_0_ADDRESS); /* LCD data address                                                */

  LL_DMA2D_ConfigSize(DMA2D, LAYER_SIZE_Y, LAYER_SIZE_X);   /* Configure DMA2D transfer number of lines and number of pixels per line */
  LL_DMA2D_SetLineOffset(DMA2D, lcd_width - LAYER_SIZE_X);  /* Configure DMA2D output line offset to LCD width - image width for display */  
   
  /* Enable the transfer complete, transfer error and configuration error interrupts */
  LL_DMA2D_EnableIT_TC(DMA2D);
  LL_DMA2D_EnableIT_TE(DMA2D);
  LL_DMA2D_EnableIT_CE(DMA2D);  
  
  /* Enable the Peripheral */
  LL_DMA2D_Start(DMA2D);
  
  /*##-4- Wait until DMA2D transfer is over ################################################*/
  while((blended_image_ready == 0) && (dma2d_error_status == 0)) {;}

  if(dma2d_error_status != 0)
  {
    /* Call Error function */
    OnError_Handler(1);	  
  }
  else
  {
    /* Turn LED2 On */
    BSP_LED_On(LED2);  
  }

  
  while (1)
  { 
    ; 
  }
}

/**
  * @brief DMA2D configuration.
  * @note  This function Configure tha DMA2D peripheral :
  *        1) Configure the transfer mode : memory to memory
  *        2) Configure the output color mode as RGB565
  *        3) Configure the transfer from FLASH to SRAM   
  *        4) Configure the data size : 240x160 (pixels)  
  * @retval
  *  None
  */
static void DMA2D_Config(void)
{
  /* Configure the DMA2D Color Mode */  
  LL_DMA2D_SetOutputColorMode(DMA2D, LL_DMA2D_OUTPUT_MODE_RGB565);
  
  /* Foreground Configuration:     */
  /* Set Alpha value to full opaque */
  LL_DMA2D_FGND_SetAlpha(DMA2D, 0xFF);
  /* Foreground layer format is RGB565 (16 bpp) */
  LL_DMA2D_FGND_SetColorMode(DMA2D, LL_DMA2D_INPUT_MODE_RGB565);
}

/**
  * @brief  On Error Handler on condition TRUE.
  * @param  condition : Can be TRUE or FALSE
  * @retval None
  */
void OnError_Handler(uint32_t condition)
{
  if(condition)
  {
    BSP_LED_On(LED3);
    while(1) { ; } /* Blocking on error */
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 280000000 (CPU Clock)
  *            HCLK(Hz)                       = 280000000 (Bus matrix and AHBs Clock)
  *            AHB Prescaler                  = 1
  *            CD APB3 Prescaler              = 2 (APB3 Clock  140MHz)
  *            CD APB1 Prescaler              = 2 (APB1 Clock  140MHz)
  *            CD APB2 Prescaler              = 2 (APB2 Clock  140MHz)
  *            SRD APB4 Prescaler             = 2 (APB4 Clock  140MHz)
  *            HSE Frequency(Hz)              = 24000000
  *            PLL_M                          = 12
  *            PLL_N                          = 280
  *            PLL_P                          = 2
  *            PLL_Q                          = 2
  *            PLL_R                          = 2
  *            VDD(V)                         = 3.3
  *            Flash Latency(WS)              = 6
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  HAL_StatusTypeDef ret = HAL_OK;

  /*!< Supply configuration update enable */
  HAL_PWREx_ConfigSupply(PWR_DIRECT_SMPS_SUPPLY);

  /* The voltage scaling allows optimizing the power consumption when the device is
  clocked below the maximum system frequency, to update the voltage scaling value
  regarding system frequency refer to product datasheet.
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
  RCC_OscInitStruct.CSIState = RCC_CSI_OFF;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;

  RCC_OscInitStruct.PLL.PLLM = 12;
  RCC_OscInitStruct.PLL.PLLN = 280;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;

  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_1;
  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
  if(ret != HAL_OK)
  {
  while(1) {};
  }

  /* Select PLL as system clock source and configure  bus clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_D1PCLK1 | RCC_CLOCKTYPE_PCLK1 | \
    RCC_CLOCKTYPE_PCLK2  | RCC_CLOCKTYPE_D3PCLK1);

  RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider  = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider  = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;
  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_6);
  if(ret != HAL_OK)
  {
  while(1) {};
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

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
