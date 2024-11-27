/**
  ******************************************************************************
  * @file    MDMA/MDMA_RepeatBlock_ZoomOut/Src/main.c
  * @author  MCD Application Team
  * @brief   This example shows how to use the MDMA to start a Repeat_BLock transfer
  *          using the STM32H7xx HAL API.
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
#include "RGB565_480x272.h"

#define IMAGE_WIDTH  (uint32_t) (480)
#define IMAGE_HEIGHT (uint32_t) (272)

#define RGB565_BYTES_PER_PIXEL (uint32_t) (2U)

/** @addtogroup STM32H7xx_HAL_Examples
  * @{
  */

/** @defroup MDMA_RepeatBlock_ZoomOut
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define COLOR_GRAY 0xFF808080U
/* Private macro -------------------------------------------------------------*/
#define ARGB8888_TO_RGB565(ARGB_COLOR) (((ARGB_COLOR) & 0xf80000) >> 8) | (((ARGB_COLOR) & 0xfC00) >> 5) | (((ARGB_COLOR) & 0xf8) >> 3);  
/* Private variables ---------------------------------------------------------*/
MDMA_HandleTypeDef     MDMA_Handle;

__IO uint32_t TransferErrorDetected = 0;
__IO uint32_t TransferCompleteDetected = 0;

/* RGB565 Default Color used to clear the LCD */
uint16_t LCD_Fill_Color = ARGB8888_TO_RGB565(COLOR_GRAY) 

/*LCD Width and Height*/
uint32_t LCD_X_Size = 640, LCD_Y_Size = 480;

/* LCD Offset calculatd versus the image width */
uint32_t LCD_Offset = 0;

/* Private function prototypes -----------------------------------------------*/
static void MPU_Config(void);
static void SystemClock_Config(void);
static void CPU_CACHE_Enable(void);
static void Error_Handler(void);
static void MDMA_TransferCompleteCallback(MDMA_HandleTypeDef *hmdma);
static void MDMA_TransferErrorCallback(MDMA_HandleTypeDef *hmdma);
static void MDMA_Config(uint32_t DecimationFactor);
static void LCD_Config(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  uint32_t DecimationFactor = 1;
  uint32_t xPos = 0;
  uint32_t yPos = 0;
  uint32_t lcd_image_position = 0;  
  uint32_t hal_status = HAL_OK;
  
  /* Configure the MPU attributes */
  MPU_Config();

  /* Enable the CPU Cache */
  CPU_CACHE_Enable();

  /* Initialize LCD Offset to zero */  
  LCD_Offset = 0;
  
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
  
  /* -1- Initialize LED1 and LED3 */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED3);
  
  BSP_SDRAM_Init(0);
  /*##-1- Initialize the LCD #################################################*/
  /* Initialize the LCD */
  LCD_Config();
  
  /*Initialize the MDMA transfer to clear the LCD frame buffer with default color */
  MDMA_Config(0);
    
  /* Start MDMA transfer in Repeat Block to clear the LCD frame buffer with default color*/
  hal_status = HAL_MDMA_Start_IT(&MDMA_Handle, (uint32_t)&LCD_Fill_Color  , 
                                               (uint32_t)LCD_FRAME_BUFFER , 
                                               (LCD_X_Size * RGB565_BYTES_PER_PIXEL), 
                                               LCD_Y_Size);
  if(hal_status != HAL_OK)
  {
    /* Transfer Error */
    Error_Handler();
  }
  
  /* wait for end of the MDMA transfer */
  while((TransferCompleteDetected == 0) && (TransferErrorDetected == 0)) {}
  if(TransferErrorDetected)
  {
    /* Transfer Error */
    Error_Handler();        
  }    

  /* Wait For User inputs */
  while (1)
  {
    TransferCompleteDetected = 0;
    TransferErrorDetected = 0;
    
    /* Calc the LCD_Offset according to the new decimated image width */
    LCD_Offset = (LCD_X_Size - (IMAGE_WIDTH/DecimationFactor)) * RGB565_BYTES_PER_PIXEL;

    /* Calc the image position according to the new decimated image width and height */
    xPos = (LCD_X_Size - (IMAGE_WIDTH/DecimationFactor))/2;
    yPos = (LCD_Y_Size - (IMAGE_HEIGHT/DecimationFactor))/2;    
    lcd_image_position = ((yPos * LCD_X_Size) + xPos) * RGB565_BYTES_PER_PIXEL;
    
    BSP_LED_Off(LED1);
    
    /*Initialize the MDMA transfer to decimate and copy the original image to the LCD frame buffer  */
    MDMA_Config(DecimationFactor);
    
    /* Start the MDMA transfer to decimate and copy the original image to the LCD frame buffer  */
    hal_status = HAL_MDMA_Start_IT(&MDMA_Handle, (uint32_t)&RGB565_480x272,
                                    (uint32_t )(LCD_FRAME_BUFFER + lcd_image_position)  , 
                                    (IMAGE_WIDTH * RGB565_BYTES_PER_PIXEL)/DecimationFactor, 
                                    (IMAGE_HEIGHT/DecimationFactor));
    
    if(hal_status != HAL_OK)
    {
      /* Transfer Error */
      Error_Handler();
    }
    
     /* wait for end of the MDMA transfer */
    while((TransferCompleteDetected == 0) && (TransferErrorDetected == 0)) {}
    if(TransferErrorDetected)
    {
      /* Transfer Error */
      Error_Handler();        
    }

    /* Turn LED1 On*/
    BSP_LED_On(LED1);  
    /* Wait for 2 Sec */
    HAL_Delay(2000); 

    TransferCompleteDetected = 0;
    TransferErrorDetected = 0;    
    
    /*Initialize the MDMA transfer to clear the previous decimated image on LCD frame buffer with default color */
    MDMA_Config(0);
    
    /* Start MDMA transfer in Repeat Block to clear the previous decimated image on  LCD frame buffer with default color*/
    hal_status = HAL_MDMA_Start_IT(&MDMA_Handle, (uint32_t)&LCD_Fill_Color, 
                                                 (uint32_t)(LCD_FRAME_BUFFER + lcd_image_position)  , 
                                                 (IMAGE_WIDTH * RGB565_BYTES_PER_PIXEL)/DecimationFactor, 
                                                 (IMAGE_HEIGHT/DecimationFactor));
    
    if(hal_status != HAL_OK)
    {
      /* Transfer Error */
      Error_Handler();
    }
    
    /* wait for end of the MDMA transfer */
    while((TransferCompleteDetected == 0) && (TransferErrorDetected == 0)) {}
    if(TransferErrorDetected)
    {
      /* Transfer Error */
      Error_Handler();        
    }  
    
    /* Next decimation factor */
    DecimationFactor *= 2;
    if(DecimationFactor > 4) DecimationFactor = 1;    
  }  
}

/**
  * @brief  MDMA Liked List transfer configuration
  * @note   This function configure the DMA for a Repeat BLock transfer.
  *
  * @retval None
  */
static void MDMA_Config(uint32_t DecimationFactor)
{
  
  /*##-1- Select the MDMA instance to be used for the transfer : MDMA_Channel0 #*/
  MDMA_Handle.Instance = MDMA_INSTANCE;
  
  HAL_MDMA_DeInit(&MDMA_Handle);
  
  /*## Enable peripherals and GPIO Clocks #*/
  __HAL_RCC_MDMA_CLK_ENABLE();

  /*## Configure NVIC for MDMA transfer complete/error interrupts #*/
  /* Set Interrupt Group Priority */
  HAL_NVIC_SetPriority(MDMA_IRQn, 0xF, 0xF);
  
  /* Enable the MDMA channel global Interrupt */
  HAL_NVIC_EnableIRQ(MDMA_IRQn);  
  
  /* Set the MDMA parameters : For repeated block transfer*/ 
  MDMA_Handle.Init.Request              = MDMA_REQUEST_SW;
  MDMA_Handle.Init.TransferTriggerMode  = MDMA_REPEAT_BLOCK_TRANSFER;  
  MDMA_Handle.Init.Priority             = MDMA_PRIORITY_HIGH;
  MDMA_Handle.Init.Endianness           = MDMA_LITTLE_ENDIANNESS_PRESERVE;

  /* Destination  Increment is half word , 2 bytes that correspond to an RGB565 pixel 16 bpp */    
  MDMA_Handle.Init.DestinationInc       = MDMA_DEST_INC_HALFWORD;
  
  /* Source and Destination data size are half word , 2 bytes that correspond to an RGB565 pixel 16 bpp */    
  MDMA_Handle.Init.SourceDataSize       = MDMA_SRC_DATASIZE_HALFWORD;
  MDMA_Handle.Init.DestDataSize         = MDMA_DEST_DATASIZE_HALFWORD;
  MDMA_Handle.Init.DataAlignment        = MDMA_DATAALIGN_PACKENABLE;                            
  MDMA_Handle.Init.SourceBurst          = MDMA_SOURCE_BURST_SINGLE;
  MDMA_Handle.Init.DestBurst            = MDMA_DEST_BURST_SINGLE;
  MDMA_Handle.Init.BufferTransferLength = 128;  
  
  /* Destination Block address offset is set in order to place the image in the center of the LCD frame buffer
     the destination blocks are not contiguous. The LCD_Offset is = to LCD X size - image width */      
  MDMA_Handle.Init.DestBlockAddressOffset    = LCD_Offset;  
      
  if(DecimationFactor == 0)
  {
    /* This case is used to clear the LCD with the default gray color */
 
    /* Source Increment is disabled : the source address always corresponds 
       to the address of the LCD_Fill_Color variable that holds the default color value*/    
    MDMA_Handle.Init.SourceInc            = MDMA_SRC_INC_DISABLE;

    MDMA_Handle.Init.SourceBlockAddressOffset  = 0;  
  }
  else
  {
    /*
      In case of a decimation by 2 , the source block address offset is set to :
          Image_width * bytes_per_pixels = Image_width * 2. 
      This allows  to jump one line over 2 from the source image allowing to decimate vertically the original image by 2.
      
      In case of a decimation by 4 , the source block address offset is set to :
          3 * Image_width * bytes_per_pixels = 3 * Image_width * 2. 
      This allows  to jump Three lines from the source image allowing to decimate vertically the original image by 4.  
    
      If no decimation (DecimationFactor = 1) , the he source block address offset is set to zero.
    */
    MDMA_Handle.Init.SourceBlockAddressOffset  = (IMAGE_WIDTH * RGB565_BYTES_PER_PIXEL * (DecimationFactor - 1));    
  }
  
  if(DecimationFactor == 1)
  {
    /*No decimation by 2, the source increment is set  half word Same as the source data size*/
    MDMA_Handle.Init.SourceInc            = MDMA_SRC_INC_HALFWORD;    
  }
  else if(DecimationFactor == 2)
  {
    /*In case of a decimation by 2, the source increment is set to WORD (2 half word) allowing to skip one pixel over two.*/
    MDMA_Handle.Init.SourceInc            = MDMA_SRC_INC_WORD;
  }
  else  if(DecimationFactor == 4)
  {
    /*In case of a decimation by 4, the source increment is set to DOUBLE WORD (4 half word) allowing to skip 3 pixels over 4.*/
    MDMA_Handle.Init.SourceInc            = MDMA_SRC_INC_DOUBLEWORD;
  }

  /*##-4- Initialize the MDMA channel ##########################################*/
  if (HAL_MDMA_Init(&MDMA_Handle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  /*##-5- Select Callbacks functions called after Transfer complete and Transfer error */
  HAL_MDMA_RegisterCallback(&MDMA_Handle, HAL_MDMA_XFER_CPLT_CB_ID, MDMA_TransferCompleteCallback);
  HAL_MDMA_RegisterCallback(&MDMA_Handle, HAL_MDMA_XFER_ERROR_CB_ID, MDMA_TransferErrorCallback);
}

/**
  * @brief  MDMA conversion complete callback
  * @note   This function is executed when the transfer complete interrupt
  *         is generated
  * @retval None
  */
static void MDMA_TransferCompleteCallback(MDMA_HandleTypeDef *hmdma)
{
  TransferCompleteDetected = 1;
}

/**
  * @brief  MDMA transfer error callback
  * @note   This function is executed when the transfer error interrupt
  *         is generated during MDMA transfer
  * @retval None
  */
static void MDMA_TransferErrorCallback(MDMA_HandleTypeDef *hmdma)
{
  TransferErrorDetected = 1;
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
  * @brief LCD configuration.
  * @note  This function Configure the LTDC peripheral :
  *        1) Configure the Pixel Clock for the LCD
  *        2) Configure the LTDC Timing and Polarity
  *        3) Configure the LTDC Layer 2 :
  *           - ARGB4444 as pixel format  
  *           - The frame buffer is located at external SDRAM
  *           - The Layer size configuration : 640x480  
  * @retval
  *  None
  */
static void LCD_Config(void)
{
  static LTDC_HandleTypeDef        hltdc_F;
  static LTDC_LayerCfgTypeDef      pLayerCfg;
  HAL_StatusTypeDef hal_status = HAL_OK;
  RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;

  /*## LTDC Clock Configuration ###########################################*/  
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
 
  /* LTDC Initialization -------------------------------------------------------*/
  /* DeInit */
  hltdc_F.Instance = LTDC;
  HAL_LTDC_DeInit(&hltdc_F);
  
  /* Polarity configuration */
  /* Initialize the horizontal synchronization polarity as active low */
  hltdc_F.Init.HSPolarity = LTDC_HSPOLARITY_AL;
  /* Initialize the vertical synchronization polarity as active low */ 
  hltdc_F.Init.VSPolarity = LTDC_VSPOLARITY_AL; 
  /* Initialize the data enable polarity as active low */ 
  hltdc_F.Init.DEPolarity = LTDC_DEPOLARITY_AL; 
  /* Initialize the pixel clock polarity as input pixel clock */  
  hltdc_F.Init.PCPolarity = LTDC_PCPOLARITY_IPC;
  /* Timing configuration */
  /* The LCD AMPIRE 640x480 is selected */
  /* Timing configuration */
  hltdc_F.Init.HorizontalSync = (AMPIRE640480_HSYNC - 1);
  hltdc_F.Init.VerticalSync = (AMPIRE640480_VSYNC - 1);
  hltdc_F.Init.AccumulatedHBP = (AMPIRE640480_HSYNC + AMPIRE640480_HBP - 1);
  hltdc_F.Init.AccumulatedVBP = (AMPIRE640480_VSYNC + AMPIRE640480_VBP - 1);  
  hltdc_F.Init.AccumulatedActiveH = (AMPIRE640480_HEIGHT + AMPIRE640480_VSYNC + AMPIRE640480_VBP - 1);
  hltdc_F.Init.AccumulatedActiveW = (AMPIRE640480_WIDTH + AMPIRE640480_HSYNC + AMPIRE640480_HBP - 1);
  hltdc_F.Init.TotalHeigh = (AMPIRE640480_HEIGHT + AMPIRE640480_VSYNC + AMPIRE640480_VBP + AMPIRE640480_VFP - 1);
  hltdc_F.Init.TotalWidth = (AMPIRE640480_WIDTH + AMPIRE640480_HSYNC + AMPIRE640480_HBP + AMPIRE640480_HFP - 1); 
  
  /* Configure R,G,B component values for LCD background color */
  hltdc_F.Init.Backcolor.Blue = 0;
  hltdc_F.Init.Backcolor.Green = 0;
  hltdc_F.Init.Backcolor.Red = 0;

  hltdc_F.Instance = LTDC;

/* Layer1 Configuration ------------------------------------------------------*/
  
  /* Windowing configuration */ 
  /*
     WindowX0 = Horizontal start
     WindowX1 = Horizontal stop      
     WindowY0 = Vertical start 
     WindowY1 = Vertical stop      
  */
  pLayerCfg.WindowX0 = 0;
  pLayerCfg.WindowX1 = LCD_X_Size;
  pLayerCfg.WindowY0 = 0;
  pLayerCfg.WindowY1 = LCD_Y_Size;
  
  /* Pixel Format configuration*/ 
  pLayerCfg.PixelFormat = LTDC_PIXEL_FORMAT_RGB565;
  
  /* Start Address configuration : frame buffer is located at FLASH memory */
  pLayerCfg.FBStartAdress = (uint32_t)LCD_FRAME_BUFFER;
  
  /* Alpha constant (255 totally opaque) */
  pLayerCfg.Alpha = 255;
  
  /* Default Color configuration (configure A,R,G,B component values) */
  pLayerCfg.Alpha0 = 0;
  pLayerCfg.Backcolor.Blue = 0;
  pLayerCfg.Backcolor.Green = 0;
  pLayerCfg.Backcolor.Red = 0;
  
  /* Configure blending factors */
  pLayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_CA;
  pLayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_CA;
  
  /* Configure the number of lines and number of pixels per line */
  pLayerCfg.ImageWidth  = LCD_X_Size;
  pLayerCfg.ImageHeight = LCD_Y_Size;

  /* Configure the LTDC */  
  hal_status  = HAL_LTDC_Init(&hltdc_F); 
  if(hal_status != HAL_OK)
  {
    Error_Handler();
  }
  
  /* Configure the Layer 1 */
  hal_status  = HAL_LTDC_ConfigLayer(&hltdc_F, &pLayerCfg, 1); 
  if(hal_status != HAL_OK)
  {
    Error_Handler();
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
 
  /*activate CSI clock mondatory for I/O Compensation Cell*/  
  __HAL_RCC_CSI_ENABLE() ;
    
  /* Enable SYSCFG clock mondatory for I/O Compensation Cell */
  __HAL_RCC_SYSCFG_CLK_ENABLE() ;
  
  /* Enables the I/O Compensation Cell */    
  HAL_EnableCompensationCell();  
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

