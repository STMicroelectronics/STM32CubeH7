/**
  ******************************************************************************
  * @file    Display/LTDC_PicturesFromSDCard/Src/main.c
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

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
FATFS SD_FatFs;  /* File system object for SD card logical drive */
char SD_Path[4]; /* SD card logical drive path */
char* pDirectoryFiles[MAX_BMP_FILES];
uint8_t  ubNumberOfFiles = 0;
uint32_t uwBmplen = 0;

/* Internal Buffer defined in SDRAM memory */
uint8_t *uwInternalBuffer;

/* Private function prototypes -----------------------------------------------*/
static void MPU_Config(void);
static void SystemClock_Config(void);
static void Error_Handler(void);
static void CPU_CACHE_Enable(void);
static void LCD_Config(void);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  uint32_t counter = 0, transparency = 0;
  uint8_t str[30];
  uwInternalBuffer = (uint8_t *)INTERNAL_BUFFER_START_ADDRESS;

  /* Configure the MPU attributes */
  MPU_Config();

  /* Enable the CPU Cache */
  CPU_CACHE_Enable();

  /* STM32H7xx HAL library initialization:
       - Configure the Systick to generate an interrupt each 1 msec
       - Set NVIC Group Priority to 4
       - Low Level Initialization
     */
  HAL_Init();

  /* Configure the system clock to 280 MHz */
  SystemClock_Config();

  /* LED Init */
  BSP_LED_Init(LED3);

  /*##-1- Configure LCD ######################################################*/
  LCD_Config();

  /*##-2- Link the SD Card disk I/O driver ###################################*/
  if(FATFS_LinkDriver(&SD_Driver, SD_Path) == 0)
  {
    /*##-3- Initialize the Directory Files pointers (heap) ###################*/
    for (counter = 0; counter < MAX_BMP_FILES; counter++)
    {
      pDirectoryFiles[counter] = malloc(MAX_BMP_FILE_NAME);
      if(pDirectoryFiles[counter] == NULL)
      {
        UTIL_LCD_DisplayStringAtLine(8, (uint8_t*)"  Cannot allocate memory ");
        while(1) { ; }
      }
    }

    /* Get the BMP file names on root directory */
    ubNumberOfFiles = Storage_GetDirectoryBitmapFiles("/Media", pDirectoryFiles);

    if (ubNumberOfFiles == 0)
    {
      for (counter = 0; counter < MAX_BMP_FILES; counter++)
      {
        free(pDirectoryFiles[counter]);
      }
      UTIL_LCD_DisplayStringAtLine(8, (uint8_t*)"  No Bitmap files...      ");
      while(1) { ; }
    }
  }
  else
  {
    /* FatFs Initialization Error */
    Error_Handler();
  }

  /* Main infinite loop */
  while(1)
  {
    counter = 0;

    while ((counter) < ubNumberOfFiles)
    {
      /* Step1 : Display on Foreground layer -------------------------------*/
      /* Format the string */
      sprintf ((char*)str, "Media/%-11.11s", pDirectoryFiles[counter]);

      if (Storage_CheckBitmapFile((const char*)str, &uwBmplen) == 0)
      {
        /* Format the string */
        sprintf ((char*)str, "Media/%-11.11s", pDirectoryFiles[counter]);

        /* Set LCD foreground Layer */
        BSP_LCD_SetActiveLayer(0, LTDC_ACTIVE_LAYER_FOREGROUND);
        UTIL_LCD_SetLayer(LTDC_ACTIVE_LAYER_FOREGROUND);

        /* Open a file and copy its content to an internal buffer */
        Storage_OpenReadFile(uwInternalBuffer, (const char*)str);

        /* Write bmp file on LCD frame buffer */
        BSP_LCD_DrawBitmap(0, 0, 0, uwInternalBuffer);

        /* Configure the transparency for background layer : Increase the transparency */
        for (transparency = 0; transparency < 255; (transparency++))
        {
          BSP_LCD_SetTransparency(0, LTDC_ACTIVE_LAYER_FOREGROUND, transparency);

          /* Insert a delay of display */
          HAL_Delay(10);
        }

        /* Configure the transparency for foreground layer : decrease the transparency */
        for (transparency = 255; transparency > 0; transparency--)
        {
          BSP_LCD_SetTransparency(0, LTDC_ACTIVE_LAYER_FOREGROUND, transparency);

          /* Insert a delay of display */
          HAL_Delay(10);
        }

        HAL_Delay(1000);

        /* Clear the Foreground Layer */
        UTIL_LCD_Clear(UTIL_LCD_COLOR_BLACK);

        /* Jump to the next image */
        counter++;

        /* Step2 : Display on Background layer -----------------------------*/
        /* Format the string */
        sprintf ((char*)str, "Media/%-11.11s", pDirectoryFiles[counter]);

        if ((Storage_CheckBitmapFile((const char*)str, &uwBmplen) == 0) || (counter < (ubNumberOfFiles)))
        {
          /* Connect the Output Buffer to LCD Background Layer  */
          BSP_LCD_SetActiveLayer(0, LTDC_ACTIVE_LAYER_BACKGROUND);
          UTIL_LCD_SetLayer(LTDC_ACTIVE_LAYER_BACKGROUND);

          /* Format the string */
          sprintf ((char*)str, "Media/%-11.11s", pDirectoryFiles[counter]);

          /* Open a file and copy its content to an internal buffer */
          Storage_OpenReadFile(uwInternalBuffer, (const char*)str);

          /* Write bmp file on LCD frame buffer */
          BSP_LCD_DrawBitmap(0, 0, 0, uwInternalBuffer);

          /* Configure the transparency for background layer : decrease the transparency */
          for (transparency = 0; transparency < 255; (transparency++))
          {
            BSP_LCD_SetTransparency(0, LTDC_ACTIVE_LAYER_BACKGROUND, transparency);

            /* Insert a delay of display */
            HAL_Delay(10);
          }

          HAL_Delay(1000);

          /* Step3 : -------------------------------------------------------*/
          /* Configure the transparency for background layer : Increase the transparency */
          for (transparency = 255; transparency > 0; transparency--)
          {
            BSP_LCD_SetTransparency(0, LTDC_ACTIVE_LAYER_BACKGROUND, transparency);

            /* Insert a delay of display */
            HAL_Delay(10);
          }

          HAL_Delay(1000);

          /* Clear the Background Layer */
          UTIL_LCD_Clear(UTIL_LCD_COLOR_BLACK);

          counter++;
        }
        else if (Storage_CheckBitmapFile((const char*)str, &uwBmplen) == 0)
        {
          UTIL_LCD_DisplayStringAtLine(7, (uint8_t *) str);
          UTIL_LCD_DisplayStringAtLine(8, (uint8_t*)"    File type not supported. ");
          while(1) { ; }
        }
      }
    }
  }
}

#if defined (__GNUC__)
#if defined (DEBUG)
/**
   Decreasing the SD clock when using GCC compiler in debug mode
  */
HAL_StatusTypeDef MX_SDMMC1_SD_Init(SD_HandleTypeDef *hsd)
{
  HAL_StatusTypeDef ret = HAL_OK;
  /* uSD device interface configuration */
  hsd->Instance                 = SDMMC1;
  hsd->Init.ClockEdge           = SDMMC_CLOCK_EDGE_RISING;
  hsd->Init.ClockPowerSave      = SDMMC_CLOCK_POWER_SAVE_DISABLE;
  hsd->Init.BusWide             = SDMMC_BUS_WIDE_1B;
  hsd->Init.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_DISABLE;
  hsd->Init.ClockDiv            = SDMMC_NSpeed_CLK_DIV*4;
  hsd->Init.TranceiverPresent   = SDMMC_TRANSCEIVER_NOT_PRESENT;

  /* HAL SD initialization */
  if(HAL_SD_Init(hsd) != HAL_OK)
  {
    ret = HAL_ERROR;
  }

  return ret;
}
#endif
#endif

/**
  * @brief  LCD Layers configuration
  * @retval None
  */
static void LCD_Config(void)
{
  int32_t lcd_status = BSP_ERROR_NONE;
  BSP_LCD_LayerConfig_t config;

  /* Initialize LCD BACKGROUND layer: activated by default */
  BSP_LCD_Init(0, LCD_ORIENTATION_LANDSCAPE);
  while(lcd_status != BSP_ERROR_NONE);
  UTIL_LCD_SetFuncDriver(&LCD_Driver);

  /* Clear the Background Layer */
  UTIL_LCD_Clear(UTIL_LCD_COLOR_BLACK);

  /* Initialize LCD FOREGROUND layer */
  config.Address = LCD_BG_LAYER_ADDRESS;
  config.PixelFormat = LTDC_PIXEL_FORMAT_ARGB8888;
  config.X0          = 0;
  config.X1          = LCD_DEFAULT_WIDTH;
  config.Y0          = 0;
  config.Y1          = LCD_DEFAULT_HEIGHT;

  BSP_LCD_ConfigLayer(0, LTDC_ACTIVE_LAYER_FOREGROUND, &config);

  /* Select the LCD Foreground Layer */
  BSP_LCD_SetActiveLayer(0, LTDC_ACTIVE_LAYER_FOREGROUND);
  UTIL_LCD_SetLayer(LTDC_ACTIVE_LAYER_FOREGROUND);

  /* Clear the Foreground Layer */
  UTIL_LCD_Clear(UTIL_LCD_COLOR_BLACK);

  /* Configure the transparency for foreground and background :
  Increase the transparency */
  BSP_LCD_SetTransparency(0, LTDC_ACTIVE_LAYER_BACKGROUND, 0);
  BSP_LCD_SetTransparency(0, LTDC_ACTIVE_LAYER_FOREGROUND, 100);

  /* Set Font */
  UTIL_LCD_SetFont(&Font16);

  /* Set the Text and Back Color */
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_RED);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLACK);
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
    Error_Handler();
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
  while(1)
  {
    BSP_LED_Toggle(LED3);
    HAL_Delay(100);
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

