/**
  ******************************************************************************
  * @file    DMA2D/DMA2D_MemToMemWithPFCandRedBlueSwap/Src/main.c
  * @author  MCD Application Team
  * @brief   This example provides a description of how to configure
  *          DMA2D peripheral in Memory to Memory with Pixel Format Conversion 
  *          and Red/Blue swap transfer mode.
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
#include "RGB565_320x240.h"

/** @addtogroup STM32H7xx_HAL_Examples
  * @{
  */

/** @addtogroup DMA2D_MemToMemWithPFCandRedBlueSwap
  * @{
  */
  
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
DMA2D_HandleTypeDef Dma2dHandle;

/* image Ready flag */
static __IO uint32_t   image_ready = 0;
static uint32_t   offset_address_image_in_lcd_buffer =  0;
static uint32_t RedBlueSwap_Config;
static uint32_t LCD_X_Size = 0, LCD_Y_Size = 0; 

/* Private function prototypes -----------------------------------------------*/
static void MPU_Config(void);
static void DMA2D_Config(uint32_t RedBlueSwapConfig);
static void TransferError(DMA2D_HandleTypeDef* dma2dHandle);
static void TransferComplete(DMA2D_HandleTypeDef* dma2dHandle);
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
  HAL_StatusTypeDef hal_status = HAL_OK;
  uint8_t  lcd_status = BSP_ERROR_NONE;

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

  /* Configure the system clock to 280 MHz */
  SystemClock_Config();

  /* Configure LED1, LED2 and LED3 */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);
  BSP_LED_Init(LED3);

  /*##-1- Initialize the LCD #################################################*/
  /* LTDC, DSI initialization and LCD screen initialization */
  lcd_status = BSP_LCD_Init(0,LCD_ORIENTATION_LANDSCAPE);
  BSP_LCD_SetLayerAddress(0,0, LCD_FRAME_BUFFER);

  OnError_Handler(lcd_status != BSP_ERROR_NONE);

  /* Get the LCD width and height */
  BSP_LCD_GetXSize(0,&LCD_X_Size);
  BSP_LCD_GetYSize(0,&LCD_Y_Size);

  UTIL_LCD_SetFuncDriver(&LCD_Driver);

  offset_address_image_in_lcd_buffer =  ((((LCD_Y_Size - IMAGE_SIZE_Y) / 2) * LCD_X_Size)
                                                     + ((LCD_X_Size - IMAGE_SIZE_X) / 2))
                                                     * ARGB8888_BYTE_PER_PIXEL;  
  
  /* Prepare using DMA2D the LCD frame buffer for display : LCD buffer clear and user message priniting*/
  UTIL_LCD_Clear(UTIL_LCD_COLOR_GRAY);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_SetFont(&Font16);

  /* Print example description */
  UTIL_LCD_DisplayStringAt(0, 440, (uint8_t *)"DMA2D_MemToMemWithPFCandRedBlueSwap example", CENTER_MODE);

  HAL_Delay(100);

  RedBlueSwap_Config = DMA2D_RB_REGULAR;
  
  while (1)
  {
    /* Turn LED1 off */
    BSP_LED_Off(LED1);
    
    if(RedBlueSwap_Config == DMA2D_RB_REGULAR)
    {   
      UTIL_LCD_DisplayStringAt(0, 40, (uint8_t *)"Red and Blue Swap : OFF", CENTER_MODE);
    }
    else
    {   
      UTIL_LCD_DisplayStringAt(0, 40, (uint8_t *)"Red and Blue Swap : ON ", CENTER_MODE);
    }   
    
    /*##-2- DMA2D configuration ###############################################*/
    DMA2D_Config(RedBlueSwap_Config);

    /* Reset the image ready flag*/
    image_ready = 0;
        
    /*##-3- Start DMA2D transfer to copy image in center of LCD frame buffer ##*/
    hal_status =  HAL_DMA2D_Start_IT(&Dma2dHandle,
                         (uint32_t)&RGB565_320x240,                                       /* Source buffer in format RGB565 and size 320x240      */
                         (uint32_t)LCD_FRAME_BUFFER + offset_address_image_in_lcd_buffer, /* Destination buffer in format ARGB8888*/
                         IMAGE_SIZE_X,                                                    /* width in pixels  */
                         IMAGE_SIZE_Y);                                                   /* height in pixels */
    
    OnError_Handler(hal_status != HAL_OK);

    /*##-4- Wait until image is ready to be displayed #########################*/
    while(image_ready == 0) {;}
    
    /* Update the Red&Blue swap configuration*/
    RedBlueSwap_Config = (RedBlueSwap_Config == DMA2D_RB_REGULAR) ? DMA2D_RB_SWAP : DMA2D_RB_REGULAR;
    
    /* wait */
    HAL_Delay(2000);
  
  }
}

/**
  * @brief DMA2D configuration.
  * @param  RedBlueSwapConfig can be DMA2D_RB_REGULAR or DMA2D_RB_SWAP
  * @note  This function Configure the DMA2D peripheral :
  *        1) Configure the transfer mode : memory to memory with PFC
  *        2) Configure the output color mode as ARGB8888
  *        3) Configure the output OutputOffset to LCD width - image width
  *        4) Configure the input color mode as RGB565
  *        5) Configure the Red and Blue swap to RedBlueSwapConfig
  * @retval
  *  None
  */
static void DMA2D_Config(uint32_t RedBlueSwapConfig)
{
  HAL_StatusTypeDef hal_status = HAL_OK;

  /* Configure the DMA2D Mode, Color Mode and output offset */
  Dma2dHandle.Init.Mode          = DMA2D_M2M_PFC;           /* DMA2D Mode memory to memory  with PFC*/
  Dma2dHandle.Init.ColorMode     = DMA2D_OUTPUT_ARGB8888;   /* Output color mode is ARGB8888 : 32 bpp */
  Dma2dHandle.Init.AlphaInverted = DMA2D_REGULAR_ALPHA;     /* No Output Alpha Inversion*/  
  Dma2dHandle.Init.RedBlueSwap   = DMA2D_RB_REGULAR;        /* No Output Red & Blue swap */    

  /* Output offset in pixels == nb of pixels to be added at end of line to come to the  */
  /* first pixel of the next line : on the output side of the DMA2D computation         */
  Dma2dHandle.Init.OutputOffset = (LCD_X_Size - IMAGE_SIZE_X);  

  /* DMA2D Callbacks Configuration */
  Dma2dHandle.XferCpltCallback  = TransferComplete;
  Dma2dHandle.XferErrorCallback = TransferError;

  /* Foreground Configuration : Layer 1 */
  Dma2dHandle.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
  Dma2dHandle.LayerCfg[1].InputAlpha = 0xFF;                   /* Fully opaque */
  Dma2dHandle.LayerCfg[1].InputColorMode = DMA2D_INPUT_RGB565; /* Foreground layer format is RGB565 : 16 bpp */
  Dma2dHandle.LayerCfg[1].InputOffset = 0x0;                   /* No offset in input */
  Dma2dHandle.LayerCfg[1].RedBlueSwap = RedBlueSwapConfig;     /*Foreground layer Red and Blue swap config */
  Dma2dHandle.LayerCfg[1].AlphaInverted = DMA2D_REGULAR_ALPHA; /* No Alpha inversion */
  
  Dma2dHandle.Instance = DMA2D;

  /* DMA2D Initialization */
  hal_status = HAL_DMA2D_DeInit(&Dma2dHandle);
  OnError_Handler(hal_status != HAL_OK);
  
  hal_status = HAL_DMA2D_Init(&Dma2dHandle);
  OnError_Handler(hal_status != HAL_OK);

  /* DMA2D foreground layer configuration */  
  hal_status = HAL_DMA2D_ConfigLayer(&Dma2dHandle, 1);
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
    BSP_LED_On(LED2);
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
  /* Turn LED1 on */
  BSP_LED_On(LED1);
  /* The image is now ready for display */
  image_ready = 1;
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
  /* Turn LED3 On */
  BSP_LED_On(LED3);
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

  /* Configure the MPU attributes as WT for SDRAM */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress = SDRAM_DEVICE_ADDR;
  MPU_InitStruct.Size = MPU_REGION_SIZE_32MB;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;
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


