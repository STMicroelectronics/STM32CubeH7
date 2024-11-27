/**
  ******************************************************************************
  * @file    GFXMMU/GFXMMU_Display/Src/main.c
  * @author  MCD Application Team
  * @brief   Main program body
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
#include "gfxmmu_lut.h"
#include "ARGB_8888_480.h"

/** @addtogroup STM32H7xx_HAL_Examples
  * @{
  */

/** @addtogroup
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

uint32_t  framebuffer = 0x24000000 ;
uint32_t *DisplayBuffer;
uint32_t x,y;
uint32_t i = 0;

GFXMMU_HandleTypeDef  GfxmmuHandle;
LTDC_HandleTypeDef   ltdc;

/* Private function prototypes -----------------------------------------------*/
static void MPU_Config(void);
static void SystemClock_Config(void);
static void GFXMMU_Config(void);
static void DisplayOn(uint32_t VirtualBufferAddress, uint32_t LtdcPixFormat);
static void OnError_Handler(uint32_t condition);


/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* Configure the MPU attributes */
  MPU_Config();

  /* STM32H7xx HAL library initialization:
       - Configure the Systick to generate an interrupt each 1 msec
       - Set NVIC Group Priority to 4
       - Low Level Initialization
    */

  HAL_Init();

  /* Configure the system clock to 280 MHz */
  SystemClock_Config();

  /* Initialize used Leds */
  BSP_LED_Init(LED1);

  /*Configure GFXMMU */
  GFXMMU_Config();

  /* Copy the image to the virtual buffer 0 */
  DisplayBuffer = (uint32_t *) GFXMMU_VIRTUAL_BUFFER0_BASE;

  for (y=0; y<GFXMMU_LUT_SIZE; y++)
  {
    for (x=0; x<480; x++)
    {

      DisplayBuffer[(1024 * y) + x +160] = ARGB_8888_480[(480 * y) + x];

    }
  }

  /* Initialize Ltdc and Display the Image  */
  DisplayOn(GFXMMU_VIRTUAL_BUFFER0_BASE, LTDC_PIXEL_FORMAT_ARGB8888) ;

  /* Infinite loop */
  while (1)
  {

  }
}

/**
  *@brief  Initializes the GFXMMU
  *The initialization is done as below:
  * Initialize GFXMMU
  * Initialize LUT
  * @param  None
  * @retval None
  */
static void GFXMMU_Config(void)
{
  /* Initialize GFXMMU */
  GfxmmuHandle.Instance = GFXMMU;
  __HAL_GFXMMU_RESET_HANDLE_STATE(&GfxmmuHandle);

  GfxmmuHandle.Init.BlocksPerLine                     = GFXMMU_256BLOCKS;
  GfxmmuHandle.Init.DefaultValue                      = 0x0;
  GfxmmuHandle.Init.Buffers.Buf0Address               = (uint32_t)framebuffer;
  GfxmmuHandle.Init.Buffers.Buf1Address               = 0;
  GfxmmuHandle.Init.Buffers.Buf2Address               = 0;
  GfxmmuHandle.Init.Buffers.Buf3Address               = 0;
  GfxmmuHandle.Init.CachePrefetch.Activation          = ENABLE;
  GfxmmuHandle.Init.CachePrefetch.CacheLock           = GFXMMU_CACHE_LOCK_ENABLE;
  GfxmmuHandle.Init.CachePrefetch.CacheLockBuffer     = GFXMMU_CACHE_LOCK_BUFFER0; /* NU */
  GfxmmuHandle.Init.CachePrefetch.CacheForce          = GFXMMU_CACHE_FORCE_ENABLE; /* NU */
  GfxmmuHandle.Init.CachePrefetch.OutterBufferability = GFXMMU_OUTTER_BUFFERABILITY_DISABLE;
  GfxmmuHandle.Init.CachePrefetch.OutterCachability   = GFXMMU_OUTTER_CACHABILITY_ENABLE;
  GfxmmuHandle.Init.CachePrefetch.Prefetch            = GFXMMU_PREFETCH_DISABLE;
  GfxmmuHandle.Init.Interrupts.Activation             = DISABLE;
  GfxmmuHandle.Init.Interrupts.UsedInterrupts         = GFXMMU_AHB_MASTER_ERROR_IT; /* NU */

  OnError_Handler(HAL_GFXMMU_Init(&GfxmmuHandle) != HAL_OK);

  /* Initialize LUT */
  OnError_Handler(HAL_GFXMMU_ConfigLut(&GfxmmuHandle, 0, GFXMMU_LUT_SIZE, (uint32_t) gfxmmu_lut_config_argb8888)!= HAL_OK);
}


/**
  *@brief  Display ON.
  *Configure LCD
  * @note  This function Configure the LTDC peripheral :
  *        1) Configure the Pixel Clock for the LCD
  *        2) Configure the LTDC Timing and Polarity
  *        3) Configure the LTDC Layer 1 :
  *           - The frame buffer is located in GFXMMU_VIRTUAL_BUFFER0_BASE
  *           - The Layer size configuration : 480x480 and format ARGB8888(32 bits/pixel)
  * @retval None
  */

static void DisplayOn(uint32_t VirtualBufferAddress, uint32_t LtdcPixFormat)
{

  LTDC_LayerCfgTypeDef LayerCfg;

  /* Init LTDC  */
  __HAL_RCC_LTDC_FORCE_RESET();
  __HAL_RCC_LTDC_RELEASE_RESET();

  ltdc.Instance = LTDC;

  /* DeInit */
  ltdc.Instance = LTDC;
  HAL_LTDC_DeInit(&ltdc);

  /* The RK070ER9427 800x480 is selected */
  /* Timing configuration */
  ltdc.Init.HorizontalSync     = (RK070ER9427_HSYNC - 1);
  ltdc.Init.VerticalSync       = (RK070ER9427_VSYNC - 1);
  ltdc.Init.AccumulatedHBP     = (RK070ER9427_HSYNC + RK070ER9427_HBP - 1);
  ltdc.Init.AccumulatedVBP     = (RK070ER9427_VSYNC + RK070ER9427_VBP - 1);
  ltdc.Init.AccumulatedActiveH = (RK070ER9427_HEIGHT + RK070ER9427_VSYNC + RK070ER9427_VBP - 1);
  ltdc.Init.AccumulatedActiveW = (RK070ER9427_WIDTH + RK070ER9427_HSYNC + RK070ER9427_HBP - 1);
  ltdc.Init.TotalHeigh         = (RK070ER9427_HEIGHT + RK070ER9427_VSYNC + RK070ER9427_VBP + RK070ER9427_VFP - 1);
  ltdc.Init.TotalWidth         = (RK070ER9427_WIDTH + RK070ER9427_HSYNC + RK070ER9427_HBP + RK070ER9427_HFP - 1);

  /* Configure R,G,B component values for LCD background color : all black */
  ltdc.Init.Backcolor.Blue = 0;
  ltdc.Init.Backcolor.Green = 0;
  ltdc.Init.Backcolor.Red = 0;

  /* Polarity */
  ltdc.Init.HSPolarity = LTDC_HSPOLARITY_AL;
  ltdc.Init.VSPolarity = LTDC_VSPOLARITY_AL;
  ltdc.Init.DEPolarity = LTDC_DEPOLARITY_AL;
  ltdc.Init.PCPolarity = LTDC_PCPOLARITY_IPC;


  /* Configure LTDC Layer 1 (index 0) */
  /* Windowing configuration */
  LayerCfg.WindowX0        = 0;
  LayerCfg.WindowX1        = 799;
  LayerCfg.WindowY0        = 0;
  LayerCfg.WindowY1        = 479;

  /* Pixel Format configuration */
  LayerCfg.PixelFormat     = LtdcPixFormat;
  LayerCfg.Alpha           = 0xFF; /* NU default value */
  LayerCfg.Alpha0          = 0;    /* completely transparent */

  /* Configure blending factors */
  LayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA; /* NU default value */
  LayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA; /* NU default value */

  /* Start Address configuration : frame buffer is located in GFXMMU_VIRTUAL_BUFFER0_BASE */
  LayerCfg.FBStartAdress   = VirtualBufferAddress;

  /* Configure the number of lines and number of pixels per line */
  LayerCfg.ImageWidth      = (LtdcPixFormat == LTDC_PIXEL_FORMAT_ARGB8888) ? 1024 : 2048;
  LayerCfg.ImageHeight     = 480;
  LayerCfg.Backcolor.Red   = 0; /* NU default value */
  LayerCfg.Backcolor.Green = 0; /* NU default value */
  LayerCfg.Backcolor.Blue  = 0; /* NU default value */
  LayerCfg.Backcolor.Reserved = 0xFF;

    /* Initialize the LTDC */
  OnError_Handler(HAL_LTDC_Init(&ltdc) != HAL_OK);
  OnError_Handler(HAL_LTDC_ConfigLayer(&ltdc, &LayerCfg, 0) != HAL_OK) ;
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
    BSP_LED_On(LED1);
    while(1)
    {
      BSP_LED_Toggle(LED1);
      HAL_Delay(100);
    } /* Blocking on error */
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

  OnError_Handler(ret != HAL_OK);

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

  OnError_Handler(ret != HAL_OK);


    /*
  Note : The activation of the I/O Compensation Cell is recommended with communication  interfaces
          (GPIO, SPI, FMC, OSPI ...)  when  operating at  high frequencies(please refer to product datasheet)
          The I/O Compensation Cell activation  procedure requires :
        - The activation of the CSI clock
        - The activation of the SYSCFG clock
        - Enabling the I/O Compensation Cell : setting bit[0] of register SYSCFG_CCCSR

          To do this please uncomment the following code
  */

  /*
  __HAL_RCC_CSI_ENABLE() ;

  __HAL_RCC_SYSCFG_CLK_ENABLE() ;

  HAL_EnableCompensationCell();
  */
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
  MPU_InitStruct.BaseAddress = 0xD0000000;
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

