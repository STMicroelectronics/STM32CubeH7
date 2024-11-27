/**
  ******************************************************************************
  * @file    LibJPEG/LibJPEG_Encoding/Src/main.c
  * @author  MCD Application Team
  * @brief   This sample code shows how to use FatFs with uSD card drive.
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

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define ROW_SIZE 1024
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
FATFS SDFatFs;  /* File system object for SD card logical drive */
FIL MyFile, MyFile1;     /* File object */
char SDPath[4]; /* SD card logical drive path */
RGB_typedef *RGB_matrix;  
ALIGN_32BYTES(uint8_t  _aucLine[ROW_SIZE]);
uint32_t  offset = 0;
uint32_t line_counter = 0;
BSP_IO_Init_t init;

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void LCD_Config(void);
static uint8_t Jpeg_CallbackFunction(uint8_t* Row, uint32_t DataLength);
static void CPU_CACHE_Enable(void);
static void MPU_Config(void);
static void Error_Handler(void);
static void Success_Handler(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  MPU_Config();
  CPU_CACHE_Enable();
  
  /* stm32h7xx HAL library initialization:
       - Systick timer is configured by default as source of time base, but user 
         can eventually implement his proper time base source (a general purpose 
         timer for application or other time source), keeping in mind that Time base 
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
         handled in milliseconds basis.
       - Set NVIC Group Priority to 4
       - Low Level Initialization
  */
  HAL_Init();
  
  /* Configure the system clock to 400 MHz */
  SystemClock_Config();
  
  BSP_LED_Init(LED_GREEN);
  BSP_LED_Init(LED_RED);
  /*##-1- LCD Configuration ##################################################*/   
  LCD_Config();

  /*##-2- Link the micro SD disk I/O driver ##################################*/
  if(FATFS_LinkDriver(&SD_Driver, SDPath) == 0)
  {
    /*##-3- Init the SD Card #################################################*/

    BSP_SD_Init(0);
    //BSP_SD_ITConfig();
        
    /*##-4- Register the file system object to the FatFs module ##############*/
    if(f_mount(&SDFatFs, (TCHAR const*)SDPath, 0) != FR_OK)
    {
      Error_Handler();
    }
    /*##-5- Open the JPG image with read access ############################*/
    if(f_open(&MyFile1, JPEG_IMAGE_FILE, FA_CREATE_ALWAYS | FA_WRITE) != FR_OK)
    {
      Error_Handler();
    }

    if(f_open(&MyFile, BMP_IMAGE_FILE, FA_READ) != FR_OK)
    {
      Error_Handler();
    }
 
    jpeg_encode(&MyFile, &MyFile1, IMAGE_WIDTH, IMAGE_HEIGHT, IMAGE_QUALITY, _aucLine);

    f_close(&MyFile1);
    f_close(&MyFile);
    
    if(f_open(&MyFile1, JPEG_IMAGE_FILE, FA_READ) != FR_OK)
    {
      Error_Handler();
    }

    /* Jpeg Decoding for display to LCD */
    jpeg_decode(&MyFile1, IMAGE_WIDTH, _aucLine, Jpeg_CallbackFunction);
            
    /* Close the JPEG file */
    f_close(&MyFile1);   
  }
  
  Success_Handler();
}

/**
  * @brief  LCD Configuration.
  * @Param  None
  * @retval None
  */
static void LCD_Config(void)
{
  /* Initialize the LCD */  
  BSP_LCD_Init(0, LCD_ORIENTATION_LANDSCAPE);
  UTIL_LCD_SetFuncDriver(&LCD_Driver);

  /* Set Foreground Layer */
  UTIL_LCD_SetLayer(0);
  
  /* Clear the LCD Background layer */
  UTIL_LCD_Clear(UTIL_LCD_COLOR_WHITE); 

}

/**
  * @brief  Copy decompressed data to display buffer.
  * @param  Row: Output row buffer
  * @param  DataLength: Row width in output buffer
  * @retval None
  */
static uint8_t Jpeg_CallbackFunction(uint8_t* Row, uint32_t DataLength)
{
#ifdef USE_DMA2D  
  static DMA2D_HandleTypeDef hdma2d_eval;

  offset = (LCD_FB_START_ADDRESS + (IMAGE_WIDTH * line_counter * 4));  
  /* Configure the DMA2D Mode, Color Mode and output offset */
  hdma2d_eval.Init.Mode         = DMA2D_M2M_PFC;
  hdma2d_eval.Init.ColorMode    = DMA2D_OUTPUT_ARGB8888;
  hdma2d_eval.Init.OutputOffset = 0;     
  
  /* Foreground Configuration */
  hdma2d_eval.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
  hdma2d_eval.LayerCfg[1].InputAlpha = 0xFF;
  hdma2d_eval.LayerCfg[1].InputColorMode = DMA2D_INPUT_RGB888;
  hdma2d_eval.LayerCfg[1].InputOffset = 0;
  
  hdma2d_eval.Instance = DMA2D; 
  
  /* DMA2D Initialization */
  if(HAL_DMA2D_Init(&hdma2d_eval) == HAL_OK) 
  {
    if(HAL_DMA2D_ConfigLayer(&hdma2d_eval, 1) == HAL_OK) 
    {
      /*
       * As the decode is done by the CPU, a cache clean
       * is needed to ensure that the DMA2D will copy the correct data
       */
      SCB_CleanDCache_by_Addr((uint32_t*)Row, ROW_SIZE);
      if (HAL_DMA2D_Start(&hdma2d_eval, (uint32_t)Row, (uint32_t)offset, IMAGE_WIDTH, 1) == HAL_OK)
      {
        /* Polling For DMA transfer */  
        HAL_DMA2D_PollForTransfer(&hdma2d_eval, 10);
      }
    }
  }
#else /* DMA2D not used */
  RGB_matrix =  (RGB_typedef*)Row;
  uint32_t  ARGB32Buffer[IMAGE_WIDTH];
  uint32_t counter = 0;
  
  for(counter = 0; counter < IMAGE_WIDTH; counter++)
  {
    ARGB32Buffer[counter]  = (uint32_t)
      (
       ((RGB_matrix[counter].B)|
        (RGB_matrix[counter].G << 8)|
        (RGB_matrix[counter].R  << 16 ) | 0xFF000000)
       );
    
    *(__IO uint32_t *)(LCD_FB_START_ADDRESS + (counter*4) + (IMAGE_WIDTH * line_counter  * 4)) = ARGB32Buffer[counter];    
  }
#endif
  
#ifdef SWAP_RB 
  uint32_t pixel = 0, width_counter, result = 0, result1 = 0;
  
  for(width_counter = 0; width_counter < IMAGE_WIDTH; width_counter++)
  {
    pixel = *(__IO uint32_t *)(LCD_FB_START_ADDRESS + (width_counter*4) + (IMAGE_WIDTH * line_counter * 4));     
    result1 = (((pixel & 0x00FF0000) >> 16) | ((pixel & 0x000000FF) << 16));
    pixel = pixel & 0xFF00FF00;
    result = (result1 | pixel);
    *(__IO uint32_t *)(LCD_FB_START_ADDRESS + (width_counter*4) + (IMAGE_WIDTH *  line_counter  * 4)) = result;    
  }
#endif

  line_counter++;
  return 0;
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
  /* PLL_VCO Input = HSE_VALUE/PLL_M = 5 Mhz */
  /* PLL_VCO Output = PLL_VCO Input * PLL_N = 800 Mhz */
  /* SDMMC Kernel Clock = PLL1_VCO Output/PLL_Q = 800/4 = 200 Mhz */
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

  /*activate CSI clock mondatory for I/O Compensation Cell*/  
  __HAL_RCC_CSI_ENABLE() ;
    
  /* Enable SYSCFG clock mondatory for I/O Compensation Cell */
  __HAL_RCC_SYSCFG_CLK_ENABLE() ;
  
  /* Enables the I/O Compensation Cell */    
  HAL_EnableCompensationCell();  
}

/**
  * @brief  Configure the MPU attributes as Write Through for External SDRAM.
  * @note   The Base Address is 0xD0000000.
  *         The Configured Region Size is 32MB same as SDRAM size.
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

#ifdef USE_FULL_ASSERT

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
#endif /* USE_FULL_ASSERT */ 

static void Error_Handler(void)
{
  while(1)
  {
    BSP_LED_Toggle(LED_RED);
    HAL_Delay(250);
  }
}


static void Success_Handler(void)
{
  while(1)
  {
    BSP_LED_Toggle(LED_GREEN);
    HAL_Delay(250);
  }
}
