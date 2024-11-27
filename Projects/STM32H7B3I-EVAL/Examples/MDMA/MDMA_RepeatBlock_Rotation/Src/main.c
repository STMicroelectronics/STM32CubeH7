/**
  ******************************************************************************
  * @file    MDMA/MDMA_RepeatBlock_Rotation/Src/main.c
  * @author  MCD Application Team
  * @brief   This example shows how to use the MDMA to start a Repeat Block transfer
  *          using the STM32H7xx HAL API.
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
#include "RGB565_480x272.h"

/** @addtogroup STM32H7xx_HAL_Examples
  * @{
  */

/** @defroup MDMA_RepeatBlock_Rotation
  * @{
  */

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define COLOR_GRAY 0xFF808080U

/* Private macro -------------------------------------------------------------*/
#define ARGB8888_TO_RGB565(ARGB_COLOR) (((ARGB_COLOR) & 0xf80000) >> 8) | (((ARGB_COLOR) & 0xfC00) >> 5) | (((ARGB_COLOR) & 0xf8) >> 3);
/* Private variables ---------------------------------------------------------*/
MDMA_HandleTypeDef     MDMA_Handle;

/*MDMA Transfer Complete and Error Flags*/
__IO uint32_t TransferErrorDetected = 0;
__IO uint32_t TransferCompleteDetected = 0;

/*LCD Width and Height*/
uint32_t LCD_X_Size = 800, LCD_Y_Size = 480;


/* RGB565 Default Color used to clear the LCD */
uint16_t LCD_Fill_Color = ARGB8888_TO_RGB565(COLOR_GRAY)

/* LCD Offset calculatd versus the image width */
int32_t LCD_Offset = 0;

/* Private function prototypes -----------------------------------------------*/
static void MPU_Config(void);
static void SystemClock_Config(void);
static void Error_Handler(void);
static void MDMA_TransferCompleteCallback(MDMA_HandleTypeDef *hmdma);
static void MDMA_TransferErrorCallback(MDMA_HandleTypeDef *hmdma);
static void MDMA_Config(uint32_t Config);
static void LCD_Config(void);
static void CPU_CACHE_Enable(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{

  uint32_t config = 0, mdma_Destination_address = 0;
  uint32_t xPos = (LCD_X_Size - IMAGE_WIDTH)/2;
  uint32_t yPos = (LCD_Y_Size - IMAGE_HEIGHT)/2;
  uint32_t lcd_image_position = ((yPos * LCD_X_Size) + xPos) * RGB565_BYTES_PER_PIXEL;
  uint32_t hal_status = HAL_OK;

  
  /* Configure the MPU attributes */
  MPU_Config();

  /* Enable the CPU Cache */
  CPU_CACHE_Enable();

  /* LCD Offset calculated versus the image width */
  LCD_Offset = (LCD_X_Size - IMAGE_WIDTH) * RGB565_BYTES_PER_PIXEL;

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

/* Initialize the LED1 LED2  */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);

  BSP_SDRAM_Init(0);
  /*##-1- Initialize the LCD #################################################*/
  /* Initialize the LCD */
  LCD_Config();


  /* Set the MDMA parameters : For Repeat block transfer to clear the LCD frame buffer */
  MDMA_Config(config);

  /* Start MDMA transfer in Repeat Block to clear the LCD frame buffer with default color*/
  hal_status = HAL_MDMA_Start_IT(&MDMA_Handle, (uint32_t)&LCD_Fill_Color  ,
                                               (uint32_t)LCD_FRAME_BUFFER ,
                                               LCD_X_Size * RGB565_BYTES_PER_PIXEL,
                                               LCD_Y_Size);

  if(hal_status != HAL_OK)
  {
    /* Transfer Error */
    Error_Handler();
  }

  while((TransferCompleteDetected == 0) && (TransferErrorDetected == 0))

  {
  }

  if(TransferErrorDetected)
  {
    /* Transfer Error */
    Error_Handler();
  }
  config = 1;

  while (1)
  {
    TransferCompleteDetected = 0;
    TransferErrorDetected = 0;

    BSP_LED_Off(LED1);

    MDMA_Config(config);

    if(config == 1) /* copy the source image to the LCD frame buffer at the center position */
    {
      mdma_Destination_address = LCD_FRAME_BUFFER + lcd_image_position;
    }
    else if(config == 2) /* copy the source image to the LCD frame buffer at the center position with mirror effect */
    {
      mdma_Destination_address = (uint32_t)(LCD_FRAME_BUFFER + lcd_image_position + ((IMAGE_WIDTH - 1) * RGB565_BYTES_PER_PIXEL));
    }
    else if(config == 3) /* copy the source image to the LCD frame buffer at the center position with 180° rotation effect */
    {
      mdma_Destination_address = (uint32_t)(LCD_FRAME_BUFFER + lcd_image_position + (((IMAGE_WIDTH - 1) + ( LCD_X_Size * (IMAGE_HEIGHT - 1))) * RGB565_BYTES_PER_PIXEL));
    }
    else /*config == 4 , copy the source image to the LCD frame buffer at the center position with 180° rotation and mirror effect */
    {
      mdma_Destination_address = (uint32_t)(LCD_FRAME_BUFFER + lcd_image_position + ((( LCD_X_Size * (IMAGE_HEIGHT - 1))) * RGB565_BYTES_PER_PIXEL)) ;
    }

    /* Start MDMA transfer  */
    hal_status = HAL_MDMA_Start_IT(&MDMA_Handle, (uint32_t)&RGB565_480x272 ,
                                                 mdma_Destination_address  ,
                                                 IMAGE_WIDTH * RGB565_BYTES_PER_PIXEL,
                                                 IMAGE_HEIGHT);

    if(hal_status != HAL_OK)
    {
      /* Transfer Error */
      Error_Handler();
    }
    else
    {
      while((TransferCompleteDetected == 0) && (TransferErrorDetected == 0)) {}
      if(TransferErrorDetected)
      {
        /* Transfer Error */
        Error_Handler();
      }
    }

    BSP_LED_On(LED1);

    HAL_Delay(2000);

    /* Next config*/
    config++;
    if(config > 4)
    {
      config = 1;
    }
  }

}

/**
  * @brief  MDMA Repeat blocks transfer configuration
  * @note   This function configure the DMA for a Repeat BLock transfer.
  * @retval None
  */
static void MDMA_Config(uint32_t Config)
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

  /*##-2- Select the MDMA functional Parameters ###############################*/

  /* Set the MDMA parameters : For repeated block transfer*/
  if(Config == 0)
  {
    /* config in Repeat Block to clear the LCD frame buffer with default color
       Each block corresponds to a line of the LCD frame buffer.
       The number of blocks corresponds to the number of lines in the LCD frame buffer
    */
    MDMA_Handle.Init.Request              = MDMA_REQUEST_SW;
    MDMA_Handle.Init.TransferTriggerMode  = MDMA_REPEAT_BLOCK_TRANSFER;
    MDMA_Handle.Init.Priority             = MDMA_PRIORITY_HIGH;
    MDMA_Handle.Init.Endianness           = MDMA_LITTLE_ENDIANNESS_PRESERVE;
    MDMA_Handle.Init.DataAlignment        = MDMA_DATAALIGN_PACKENABLE;
    MDMA_Handle.Init.SourceBurst          = MDMA_SOURCE_BURST_SINGLE;
    MDMA_Handle.Init.DestBurst            = MDMA_DEST_BURST_SINGLE;
    MDMA_Handle.Init.BufferTransferLength = 128;

    /*Source Block address offset is null, the source blocks are contiguous*/
    MDMA_Handle.Init.SourceBlockAddressOffset  = 0;

    /* Source and Destination data size are half word , 2 bytes that correspond to an RGB565 pixel 16 bpp */
    MDMA_Handle.Init.SourceDataSize       = MDMA_SRC_DATASIZE_HALFWORD;
    MDMA_Handle.Init.DestDataSize         = MDMA_DEST_DATASIZE_HALFWORD;

    /* Source Increment is disabled : the source address always corresponds
       to the address of the LCD_Fill_Color variable that holds the default color value*/
    MDMA_Handle.Init.SourceInc            = MDMA_SRC_INC_DISABLE;
    /* Destination  Increment is half word , 2 bytes that correspond to an RGB565 pixel 16 bpp */
    MDMA_Handle.Init.DestinationInc       = MDMA_DEST_INC_HALFWORD;

    /*Destination Block address offset is null, the destination blocks are contiguous*/
    MDMA_Handle.Init.DestBlockAddressOffset    = 0;
  }
  else if(Config == 1)
  {
    /*
      Config to copy the source image to the LCD frame buffer at the center position.
      Each block corresponds to a line of the source/destination image.
      The number of blocks corresponds to the number of lines of the image
    */
    MDMA_Handle.Init.Request              = MDMA_REQUEST_SW;
    MDMA_Handle.Init.TransferTriggerMode  = MDMA_REPEAT_BLOCK_TRANSFER;
    MDMA_Handle.Init.Priority             = MDMA_PRIORITY_HIGH;
    MDMA_Handle.Init.Endianness           = MDMA_LITTLE_ENDIANNESS_PRESERVE;
    MDMA_Handle.Init.DataAlignment        = MDMA_DATAALIGN_PACKENABLE;
    MDMA_Handle.Init.SourceBurst          = MDMA_SOURCE_BURST_SINGLE;
    MDMA_Handle.Init.DestBurst            = MDMA_DEST_BURST_SINGLE;
    MDMA_Handle.Init.BufferTransferLength = 128;

   /*Source Block address offset is null, the source blocks are contiguous*/
    MDMA_Handle.Init.SourceBlockAddressOffset  = 0;

    /* Source and Destination data size are half word , 2 bytes that correspond to an RGB565 pixel 16 bpp */
    MDMA_Handle.Init.SourceDataSize       = MDMA_SRC_DATASIZE_HALFWORD;
    MDMA_Handle.Init.DestDataSize         = MDMA_DEST_DATASIZE_HALFWORD;

    /* Source and Destination  Increment is half word , 2 bytes that correspond to an RGB565 pixel 16 bpp */
    MDMA_Handle.Init.SourceInc            = MDMA_SRC_INC_HALFWORD;
    MDMA_Handle.Init.DestinationInc       = MDMA_DEST_INC_HALFWORD;

   /* Destination Block address offset is set in order to place the image in the center of the LCD frame buffer
      the destination blocks are not contiguous. The LCD_Offset is = to LCD X size - image width */
    MDMA_Handle.Init.DestBlockAddressOffset = LCD_Offset;
  }
  else if(Config == 2)
  {
    /*
      Config to copy the source image to the LCD frame buffer at the center position with mirror effect.
      Each block corresponds to a line of the source/destination image.
      The number of blocks corresponds to the number of lines of the image
    */
    MDMA_Handle.Init.Request              = MDMA_REQUEST_SW;
    MDMA_Handle.Init.TransferTriggerMode  = MDMA_REPEAT_BLOCK_TRANSFER;
    MDMA_Handle.Init.Priority             = MDMA_PRIORITY_HIGH;
    MDMA_Handle.Init.Endianness           = MDMA_LITTLE_ENDIANNESS_PRESERVE;
    MDMA_Handle.Init.DataAlignment        = MDMA_DATAALIGN_PACKENABLE;
    MDMA_Handle.Init.SourceBurst          = MDMA_SOURCE_BURST_SINGLE;
    MDMA_Handle.Init.DestBurst            = MDMA_DEST_BURST_SINGLE;
    MDMA_Handle.Init.BufferTransferLength = 128;

    /*Source Block address offset is null, the source blocks are contiguous*/
    MDMA_Handle.Init.SourceBlockAddressOffset  = 0;

    /* Source and Destination data size are half word , 2 bytes that correspond to an RGB565 pixel 16 bpp */
    MDMA_Handle.Init.SourceDataSize       = MDMA_SRC_DATASIZE_HALFWORD;
    MDMA_Handle.Init.DestDataSize         = MDMA_DEST_DATASIZE_HALFWORD;

    /* Source Increment is half word , 2 bytes that correspond to an RGB565 pixel 16 bpp
       Knowing that the source address is set to the source image start address */
    MDMA_Handle.Init.SourceInc            = MDMA_SRC_INC_HALFWORD;
    /* Destination  Decrement by half word , 2 bytes that correspond to an RGB565 pixel 16 bpp
       Knowing that the destination address is set to the last pixel address in the first line
       of the destination image */
    MDMA_Handle.Init.DestinationInc       = MDMA_DEST_DEC_HALFWORD;

   /* Destination Block address offset is set in order to place the image in the center of the LCD frame buffer
      the destination blocks are not contiguous. The LCD_Offset is = to LCD X size - image width
      Knowing that the destination address is set to the last pixel address in the first line
      of the destination image, the destination block offset takes also into account that
      the destination address is decrementing*/
    MDMA_Handle.Init.DestBlockAddressOffset    = (int32_t)(LCD_Offset + (2 * (RGB565_BYTES_PER_PIXEL * IMAGE_WIDTH)));
  }
  else if(Config == 3)
  {
    /* Config to copy the source image to the LCD frame buffer at the center position with 180° rotation effect
       Each block corresponds to a line of the source/destination image.
      The number of blocks corresponds to the number of lines of the image
    */
    MDMA_Handle.Init.Request              = MDMA_REQUEST_SW;
    MDMA_Handle.Init.TransferTriggerMode  = MDMA_REPEAT_BLOCK_TRANSFER;
    MDMA_Handle.Init.Priority             = MDMA_PRIORITY_HIGH;
    MDMA_Handle.Init.Endianness           = MDMA_LITTLE_ENDIANNESS_PRESERVE;
    MDMA_Handle.Init.DataAlignment        = MDMA_DATAALIGN_PACKENABLE;
    MDMA_Handle.Init.SourceBurst          = MDMA_SOURCE_BURST_SINGLE;
    MDMA_Handle.Init.DestBurst            = MDMA_DEST_BURST_SINGLE;
    MDMA_Handle.Init.BufferTransferLength = 128;

    /*Source Block address offset is null, the source blocks are contiguous*/
    MDMA_Handle.Init.SourceBlockAddressOffset  = 0;

     /* Source and Destination data size are half word , 2 bytes that correspond to an RGB565 pixel 16 bpp */
    MDMA_Handle.Init.SourceDataSize       = MDMA_SRC_DATASIZE_HALFWORD;
    MDMA_Handle.Init.DestDataSize         = MDMA_DEST_DATASIZE_HALFWORD;

    /* Source Increment is half word , 2 bytes that correspond to an RGB565 pixel 16 bpp
       Knowing that the source address is set to the source image start address */
    MDMA_Handle.Init.SourceInc            = MDMA_SRC_INC_HALFWORD;
    /* Destination  Decrement by half word , 2 bytes that correspond to an RGB565 pixel 16 bpp
       Knowing that the destination address is set to the last pixel of the destination image */
    MDMA_Handle.Init.DestinationInc       = MDMA_DEST_DEC_HALFWORD;

    /* Destination Block address offset is set in order to place the image in the center of the LCD frame buffer
      the destination blocks are not contiguous. The LCD_Offset is = to LCD X size - image width
      Knowing that the destination address is set to  the last pixel address of
      the destination image, the destination block offset takes also into account that the destination address is decrementing*/
    MDMA_Handle.Init.DestBlockAddressOffset    = (int32_t)((-1) * LCD_Offset);
  }
  else if(Config == 4)
  {
    /*
      Config copy the source image to the LCD frame buffer at the center position with 180° rotation and mirror effect
      Each block corresponds to a line of the source/destination image.
      The number of blocks corresponds to the number of lines of the image
    */
    MDMA_Handle.Init.Request              = MDMA_REQUEST_SW;
    MDMA_Handle.Init.TransferTriggerMode  = MDMA_REPEAT_BLOCK_TRANSFER;
    MDMA_Handle.Init.Priority             = MDMA_PRIORITY_HIGH;
    MDMA_Handle.Init.Endianness           = MDMA_LITTLE_ENDIANNESS_PRESERVE;
    MDMA_Handle.Init.DataAlignment        = MDMA_DATAALIGN_PACKENABLE;
    MDMA_Handle.Init.SourceBurst          = MDMA_SOURCE_BURST_SINGLE;
    MDMA_Handle.Init.DestBurst            = MDMA_DEST_BURST_SINGLE;
    MDMA_Handle.Init.BufferTransferLength = 128;

    /*Source Block address offset is null, the source blocks are contiguous*/
    MDMA_Handle.Init.SourceBlockAddressOffset  = 0;

    /* Source and Destination data size are half word , 2 bytes that correspond to an RGB565 pixel 16 bpp */
    MDMA_Handle.Init.SourceDataSize       = MDMA_SRC_DATASIZE_HALFWORD;
    MDMA_Handle.Init.DestDataSize         = MDMA_DEST_DATASIZE_HALFWORD;

    /* Source and Destination  Increment is half word , 2 bytes that correspond to an RGB565 pixel 16 bpp */
    MDMA_Handle.Init.SourceInc            = MDMA_SRC_INC_HALFWORD;
    MDMA_Handle.Init.DestinationInc       = MDMA_DEST_INC_HALFWORD;

    /* Destination Block address offset is set in order to place the image in the center of the LCD frame buffer
      the destination blocks are not contiguous. The LCD_Offset is = to LCD X size - image width
      Knowing that the destination address is set to the first pixel address of the last line
      in the destination image, the destination block offset allows to decrement the destination address
      after each block in order to start the next block at previous line */
    MDMA_Handle.Init.DestBlockAddressOffset    = ((-2) * ((int32_t)(RGB565_BYTES_PER_PIXEL * IMAGE_WIDTH))) - ((int32_t)LCD_Offset);
  }

  /*##-3- Initialize the MDMA channel ##########################################*/
  if (HAL_MDMA_Init(&MDMA_Handle) != HAL_OK)
  {
    /* in case of Initialization Error */
    Error_Handler();
  }

  /*##-4- Select Callbacks functions called after Transfer complete and Transfer error */
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
  /* Turn LED2 on */
  BSP_LED_On(LED2);
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
  *           - The Layer size configuration : 800x480
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
  /* RK070ER9427 typical PCLK is 33.3 MHz so the PLL3R is configured to provide this clock */
  /* RK070ER9427 LCD clock configuration */
  /* PLL3_VCO Input = HSE_VALUE/PLL3M = 2 Mhz */
  /* PLL3_VCO Output = PLL3_VCO Input * PLL3N = 400 Mhz */
  /* PLLLCDCLK = PLL3_VCO Output/PLL3R =  33.3 Mhz */
  /* LTDC clock frequency = PLLLCDCLK = 33.3 MHz */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
  PeriphClkInitStruct.PLL3.PLL3M = 12;
  PeriphClkInitStruct.PLL3.PLL3N = 200;
  PeriphClkInitStruct.PLL3.PLL3FRACN = 0;
  PeriphClkInitStruct.PLL3.PLL3P = 2;
  PeriphClkInitStruct.PLL3.PLL3Q = 2;
  PeriphClkInitStruct.PLL3.PLL3R = 12;
  PeriphClkInitStruct.PLL3.PLL3RGE = RCC_PLL3VCIRANGE_0;
  PeriphClkInitStruct.PLL3.PLL3VCOSEL = RCC_PLL3VCOMEDIUM;
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
  /* The LCD RK070ER9427 800x480is selected */
  /* Timing configuration */
  hltdc_F.Init.HorizontalSync = (RK070ER9427_HSYNC - 1);
  hltdc_F.Init.VerticalSync = (RK070ER9427_VSYNC - 1);
  hltdc_F.Init.AccumulatedHBP = (RK070ER9427_HSYNC + RK070ER9427_HBP - 1);
  hltdc_F.Init.AccumulatedVBP = (RK070ER9427_VSYNC + RK070ER9427_VBP - 1);
  hltdc_F.Init.AccumulatedActiveH = (RK070ER9427_HEIGHT + RK070ER9427_VSYNC + RK070ER9427_VBP - 1);
  hltdc_F.Init.AccumulatedActiveW = (RK070ER9427_WIDTH + RK070ER9427_HSYNC + RK070ER9427_HBP - 1);
  hltdc_F.Init.TotalHeigh = (RK070ER9427_HEIGHT + RK070ER9427_VSYNC + RK070ER9427_VBP + RK070ER9427_VFP - 1);
  hltdc_F.Init.TotalWidth = (RK070ER9427_WIDTH + RK070ER9427_HSYNC + RK070ER9427_HBP + RK070ER9427_HFP - 1);

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

