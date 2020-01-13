/**
  ******************************************************************************
  * @file    DMA2D/DMA2D_RegToMemWithLCD/Src/main.c 
  * @author  MCD Application Team
  * @brief   This example provides a description of how to configure DMA2D peripheral 
  *          in Register to Memory transfer mode and display the result on LCD.
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

/** @addtogroup DMA2D_RegToMemWithLCD
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
DMA2D_HandleTypeDef Dma2dHandle;
/* DMA2D Transfer Complete flag */
__IO uint32_t   dma2d_TransferComplete_Flag = 0;
uint32_t LCD_X_Size, LCD_Y_Size;

/* Private function prototypes -----------------------------------------------*/

static void DMA2D_Config(void);
static void TransferError(DMA2D_HandleTypeDef* Dma2dHandle);
static void TransferComplete(DMA2D_HandleTypeDef* Dma2dHandle);
static void SystemClock_Config(void);
static void OnError_Handler(uint32_t condition);
static void CPU_CACHE_Enable(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief   Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  dma2d_TransferComplete_Flag = 0;
  HAL_StatusTypeDef hal_status = HAL_OK;
  int32_t  lcd_status = BSP_ERROR_NONE;
  
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
  
  lcd_status = BSP_LCD_Init(0, LCD_ORIENTATION_LANDSCAPE);
  OnError_Handler(lcd_status != BSP_ERROR_NONE);

  /* Get the LCD width and height */
  BSP_LCD_GetXSize(0, &LCD_X_Size);
  BSP_LCD_GetYSize(0, &LCD_Y_Size);
  
  /* Clear the LCD */
  BSP_LCD_FillRect(0, 0, 0, LCD_X_Size, LCD_Y_Size, 0xFFFFFFFF); 
    
  /*##-2- DMA2D configuration ################################################*/
  DMA2D_Config();  
    
  /*##-3- Start DMA2D transfer ###############################################*/ 
  /* Color value is entered in ARGB8888 format: 0xFFFFFF00 will yield a combination
     of Red and Green, that is yellow.
     0x00FF0000 would mean only Red, 0x0000FF00 only Green, 0x000000FF only Blue , 0xFF000000 is the transparency */  
  hal_status = HAL_DMA2D_Start_IT(&Dma2dHandle, 
                        0xFFFFFF00,                /* Color value in Register to Memory DMA2D mode */
                        (uint32_t)LCD_FRAME_BUFFER,  /* DMA2D output buffer */
                        QVGA_RES_X,              /* width of buffer in pixels */
                        QVGA_RES_Y);             /* height of buffer in lines */
  OnError_Handler(hal_status != HAL_OK);
  
  /*##-4- Wait until DMA2D transfer is over ################################################*/
  while(dma2d_TransferComplete_Flag == 0) {;}
    
  
  while (1) 
  { 
    ; 
  } 
}


/**
  * @brief DMA2D configuration.
  * @note  This function Configure tha DMA2D peripheral :
  *        1) Configure the transfer mode : register to memory
  *        2) Configure the color to be used to fill user defined area.
  * @retval
  *  None
  */

static void DMA2D_Config(void)
{  
  HAL_StatusTypeDef hal_status = HAL_OK;
  
  /* Register to memory mode with ARGB4444 as colorMode */  
  Dma2dHandle.Init.Mode          = DMA2D_R2M;             /* Mode Register to Memory */
  Dma2dHandle.Init.ColorMode     = DMA2D_OUTPUT_ARGB8888; /* DMA2D Output color mode is ARGB8888 (32 bpp) */
  Dma2dHandle.Init.OutputOffset  = LCD_X_Size - QVGA_RES_X;
  Dma2dHandle.Init.RedBlueSwap   = DMA2D_RB_REGULAR;      /* No R&B swap for the output image */
  Dma2dHandle.Init.AlphaInverted = DMA2D_REGULAR_ALPHA;   /* No alpha inversion for the output image */

  Dma2dHandle.XferCpltCallback  = TransferComplete;
  Dma2dHandle.XferErrorCallback = TransferError;
  Dma2dHandle.Instance          = DMA2D;    
  
  /* DMA2D Initialization */
  hal_status = HAL_DMA2D_Init(&Dma2dHandle);
  OnError_Handler(hal_status != HAL_OK);
}

/**
  * @brief  On Error Handler on condition TRUE.
  * @param  condition : Can be TRUE or FALSE
  * @retval None
  */
static void OnError_Handler(uint32_t condition)
{
  if(condition)
  {
    BSP_LED_On(LED3);
    while(1) { ; } /* Blocking on error */
  }
}

/**
  * @brief  DMA2D Transfer completed callback
  * @param  hdma2d: DMA2D handle. 
  * @note   This example shows a simple way to report end of DMA2D transfer, and 
  *         you can add your own implementation. 
  * @retval None
  */
static void TransferComplete(DMA2D_HandleTypeDef *hdma2d)
{
  /* Turn LED2 on */
  BSP_LED_On(LED2);
  /* The Blended image is now ready for display */
  dma2d_TransferComplete_Flag = 1;    
}

/**
  * @brief  DMA2D error callbacks
  * @param  hdma2d: DMA2D handle
  * @note   This example shows a simple way to report DMA2D transfer error, and you can
  *         add your own implementation.
  * @retval None
  */
static void TransferError(DMA2D_HandleTypeDef *hdma2d)
{
  /* Turn LED3 on */
  BSP_LED_On(LED3);
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

#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(char* file, uint32_t line)
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
