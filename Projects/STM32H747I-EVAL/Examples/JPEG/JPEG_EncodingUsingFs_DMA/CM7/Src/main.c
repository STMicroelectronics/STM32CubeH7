/**
  ******************************************************************************
  * @file    JPEG/JPEG_EncodingUsingFs_DMA/CM7/Src/main.c
  * @author  MCD Application Team
  * @brief   This sample code shows how to use the HW JPEG to Encode a JPEG file with DMA method.
  *          This is the main program for Cortex-M7   
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
#include "encode_dma.h"

/** @addtogroup STM32H7xx_HAL_Examples
  * @{
  */

/** @addtogroup JPEG_EncodingUsingFs_DMA
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
FATFS SDFatFs;  /* File system object for SD card logical drive */
char SDPath[4]; /* SD card logical drive path */
FIL JPEG_File;  /* File object */
FIL BMP_File;   /* File object */

JPEG_HandleTypeDef    JPEG_Handle;

static uint32_t LCD_X_Size = 0;

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void LCD_BriefDisplay(void);
static void LCD_FileErrorDisplay(void);
static void LCD_SizesErrorDisplay(void);
static void SD_Initialize(void);
static void MPU_Config(void);
static void CPU_CACHE_Enable(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  uint32_t JpegEncodeProcessing_End = 0;

  /* System Init, System clock, voltage scaling and L1-Cache configuration are done by CPU1 (Cortex-M7) 
     in the meantime Domain D2 is put in STOP mode(Cortex-M4 in deep-sleep)
  */  

  /* Configure the MPU attributes as Write Through for SDRAM*/
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

  /* When system initialization is finished, Cortex-M7 could wakeup (when needed) the Cortex-M4  by means of 
     HSEM notification or by any D2 wakeup source (SEV,EXTI..)   */
	 
  /* Initialize the LED3 (Red LED , set to On when error) */
  BSP_LED_Init(LED3);

  /* Initialize the LED4 (GREEN LED , set to On when Test OK) */
  BSP_LED_Init(LED4);
  
  /*##-1- JPEG Initialization ################################################*/   
  /* Init The JPEG Color Look Up Tables used for YCbCr to RGB conversion   */ 
  JPEG_InitColorTables();

   /* Init the HAL JPEG driver */
  JPEG_Handle.Instance = JPEG;
  HAL_JPEG_Init(&JPEG_Handle);  
  
  /*##-2- LCD Configuration ##################################################*/  
  /* Initialize the LCD   */
  BSP_LCD_Init(0, LCD_ORIENTATION_LANDSCAPE); 
  UTIL_LCD_SetFuncDriver(&LCD_Driver);
  UTIL_LCD_SetLayer(0); 
  /* Get the LCD Width */
  BSP_LCD_GetXSize(0, &LCD_X_Size);
  /* Display example brief   */
  LCD_BriefDisplay();
    
  /*##-3- Link the micro SD disk I/O driver ##################################*/
  if(FATFS_LinkDriver(&SD_Driver, SDPath) == 0)
  {
    /*##-4- Init the SD Card #################################################*/
    SD_Initialize();
    
    if(BSP_SD_IsDetected(0))
    {
      /*##-5- Register the file system object to the FatFs module ##############*/
      if(f_mount(&SDFatFs, (TCHAR const*)SDPath, 0) == FR_OK)
      {
        /*##-6- Open the BMP file with read access #############################*/
        if(f_open(&BMP_File, "image.bmp", FA_READ) == FR_OK)
        {
          /*##-7- Create the JPEG file with write access ########################*/
          if(f_open(&JPEG_File, "image.jpg", FA_CREATE_ALWAYS | FA_WRITE) == FR_OK)
          {
            /*##-8- JPEG Encoding with DMA (Not Blocking ) Method ################*/
            JPEG_Encode_DMA(&JPEG_Handle, &BMP_File, &JPEG_File);
            
            /*##-9- Wait till end of JPEG encoding and perform Input/Output Processing in BackGround  #*/
            do
            {
              JPEG_EncodeInputHandler(&JPEG_Handle);
              JpegEncodeProcessing_End = JPEG_EncodeOutputHandler(&JPEG_Handle);
              
            }while(JpegEncodeProcessing_End == 0);
            
            /*##-10- Close the JPEG file #######################################*/
            f_close(&JPEG_File);
            
            UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
            UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLACK);
            UTIL_LCD_SetFont(&Font16);
            UTIL_LCD_DisplayStringAt(0, LINE(20), (uint8_t *)"JPEG file encoded and saved on the uSD", CENTER_MODE);
          }
          /*##-11- Close the BMP file ##########################################*/
          f_close(&BMP_File);  
        }
        else /* Can't Open BMP file*/
        {
          LCD_FileErrorDisplay();
          Error_Handler();
        }
      }
      else /* FatFs mount failed */
      {
        Error_Handler();
      }      
    }
    else /* uSD not connected */
    {
      LCD_FileErrorDisplay();
      Error_Handler();
    }    
  }

  /* Test accomplished successfully */
  BSP_LED_On(LED4);

  /* Infinite loop */
  while (1)
  {
  }
}


/**
  * @brief  System Clock Configuration
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
  * @brief  Display Example description.
  * @param  None
  * @retval None
  */
static void LCD_BriefDisplay(void)
{
  UTIL_LCD_Clear(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_FillRect(0, 177, LCD_X_Size, 112, UTIL_LCD_COLOR_BLUE);  
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
  
  UTIL_LCD_DisplayStringAt(0, LINE(9), (uint8_t *)"JPEG Encoding from uSD Fatfs with DMA", CENTER_MODE);
  UTIL_LCD_SetFont(&Font16);
  UTIL_LCD_DisplayStringAt(0, LINE(15), (uint8_t *)"This example shows how to Encode (with DMA model)", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, LINE(16), (uint8_t *)"and save the result JPEG file on the uSD", CENTER_MODE);  
}

/**
  * @brief  Display File access error message.
  * @param  None
  * @retval None
  */
static void LCD_FileErrorDisplay(void)
{
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE); 
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_RED);
  UTIL_LCD_SetFont(&Font16);
  UTIL_LCD_DisplayStringAtLine(27, (uint8_t *)"     Unable to open BMP file image.bmp");
  UTIL_LCD_DisplayStringAtLine(28, (uint8_t *)"     Check that a bmp file named image.bmp is stored on the uSD");

}

static void SD_Initialize(void)
{  
  BSP_SD_Init(0);
}

/**
  * @brief  Display error message when Image Sizes do not meet the requirements.
  * @param  None
  * @retval None
  */
static void LCD_SizesErrorDisplay(void)
{
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE); 
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_RED);
  UTIL_LCD_SetFont(&Font16);
  UTIL_LCD_DisplayStringAtLine(27, (uint8_t *)"     Input Sizes of image.bmp do not meet requirements");
  UTIL_LCD_DisplayStringAtLine(28, (uint8_t *)"    Please change the bmp file named image.bmp stored on the uSD1");

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

/**
  * @brief  On Error Handler.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  BSP_LED_On(LED3);
  while(1) { ; } /* Blocking on error */
}

/**
  * @brief  Get the images sizes from BMP header.
  * @param  MyFile: pointer to the source file name
  * @param  pInfo: pointer to the Info structure
  * @retval None
  */
void BMP_GetInfo(FIL * MyFile, JPEG_ConfTypeDef *pInfo)
{
  uint32_t bytesReadfile = 0;
  uint32_t ImageWidth = 0;
  uint32_t ImageHeight = 0 ;
  uint32_t ImageWidthOffset = 18;
  uint32_t ImageHeightOffset = 22;

  /*Open the image with read access */
  f_lseek (MyFile,ImageWidthOffset);
  if(f_read (MyFile, (void *) &ImageWidth , 4, (UINT*)(&bytesReadfile)) == FR_OK)
  {
    pInfo->ImageWidth         = ImageWidth;
  }
  f_lseek (MyFile,ImageHeightOffset);
  if(f_read (MyFile, (void *) &ImageHeight , 4, (UINT*)(&bytesReadfile)) == FR_OK)
  {
    pInfo->ImageHeight        = ImageHeight;
  }

  /* Jpeg Encoding Setting to be set by users */
  pInfo->ChromaSubsampling  = JPEG_CHROMA_SAMPLING;
  pInfo->ColorSpace         = JPEG_COLOR_SPACE;
  pInfo->ImageQuality       = JPEG_IMAGE_QUALITY;
    
  /*Check if Image Sizes meets the requirements */
  if (((pInfo->ImageWidth % 8) != 0 ) || ((pInfo->ImageHeight % 8) != 0 ) || \
      (((pInfo->ImageWidth % 16) != 0 ) && (pInfo->ColorSpace == JPEG_YCBCR_COLORSPACE) && (pInfo->ChromaSubsampling != JPEG_444_SUBSAMPLING)) || \
      (((pInfo->ImageHeight % 16) != 0 ) && (pInfo->ColorSpace == JPEG_YCBCR_COLORSPACE) && (pInfo->ChromaSubsampling == JPEG_420_SUBSAMPLING)))
  {
    LCD_SizesErrorDisplay();
    Error_Handler();
  }
  
}
/**
  * @}
  */
