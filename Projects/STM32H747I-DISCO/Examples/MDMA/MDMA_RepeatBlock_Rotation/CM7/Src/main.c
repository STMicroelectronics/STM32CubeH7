/**
  ******************************************************************************
  * @file    MDMA/MDMA_RepeatBlock_Rotation/CM7/Src/main.c
  * @author  MCD Application Team
  * @brief   This example shows how to use the MDMA to start a Repeat Block transfer
  *          using the STM32H7xx HAL API.
  *          This is the main program for Cortex-M7.  
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
DMA2D_HandleTypeDef Dma2dHandle;

/*MDMA Transfer Complete and Error Flags*/
__IO uint32_t TransferErrorDetected = 0;
__IO uint32_t TransferCompleteDetected = 0;

/*LCD Width and Height*/
uint32_t LCD_X_Size = 800, LCD_Y_Size = 480;

/* RGB565 Default Color used to clear the MDMA destination frame buffer (drawing buffer) */
uint16_t LCD_Fill_Color = ARGB8888_TO_RGB565(COLOR_GRAY) 

/* LCD Offset calculatd versus the image width */
int32_t LCD_Offset = 0;

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void CPU_CACHE_Enable(void);
static void MPU_Config(void);
static void Error_Handler(void);
static void MDMA_TransferCompleteCallback(MDMA_HandleTypeDef *hmdma);
static void MDMA_TransferErrorCallback(MDMA_HandleTypeDef *hmdma);
static void MDMA_Config(uint32_t Config);
static void DMA2D_Config(void);

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

  /* LCD Offset calculatd versus the image width */  
  LCD_Offset = (LCD_X_Size - IMAGE_WIDTH) * RGB565_BYTES_PER_PIXEL;
  
  /* System Init, System clock, voltage scaling and L1-Cache configuration are done by CPU1 (Cortex-M7) 
     in the meantime Domain D2 is put in STOP mode(Cortex-M4 in deep-sleep)
  */
  
  /* Configure the MPU attributes as Write Through for SDRAM */
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
	 
  /* -1- Initialize LED4 and LED3*/
  BSP_LED_Init(LED4);
  BSP_LED_Init(LED3);
  
  BSP_SDRAM_Init(0);
  /*##-1- Initialize the LCD #################################################*/
  /* Initialize the LCD */
  BSP_LCD_Init(0, LCD_ORIENTATION_LANDSCAPE);
  UTIL_LCD_SetFuncDriver(&LCD_Driver);
  UTIL_LCD_SetLayer(0);  
  UTIL_LCD_Clear(COLOR_GRAY);
  UTIL_LCD_SetFont(&Font24);
  
  BSP_LCD_GetXSize(0, &LCD_X_Size);
  BSP_LCD_GetYSize(0, &LCD_Y_Size);
 
  /* Set the MDMA parameters : For Repeat block transfer to clear the MDMA destination frame buffer (drawing buffer) */ 
  MDMA_Config(0);
  
  /* Start MDMA transfer in Repeat Block to clear the MDMA destination frame buffer (drawing buffer) with default color*/
  hal_status = HAL_MDMA_Start_IT(&MDMA_Handle, (uint32_t)&LCD_Fill_Color  , 
                                               (uint32_t)MDMA_DEST_FRAME_BUFFER , 
                                               LCD_X_Size * RGB565_BYTES_PER_PIXEL, 
                                               LCD_Y_Size);
  
  if(hal_status != HAL_OK)  
  {
    /* Transfer Error */
    Error_Handler();
  }

  while((TransferCompleteDetected == 0) && (TransferErrorDetected == 0)) {}
  if(TransferErrorDetected)
  {
    /* Transfer Error */
    Error_Handler();        
  }  
  
  /*Initialize and configure the DMA2D to copy the MDMA destination frame buffer (drawing buffer) to the LCD frame buffer (display buffer) */
  DMA2D_Config();
  
  config = 1;  
  while (1)
  {
    TransferCompleteDetected = 0;
    TransferErrorDetected = 0;
    
    BSP_LED_Off(LED4); 
    
    MDMA_Config(config);
    
    if(config == 1) /* copy the source image to the MDMA destination frame buffer (drawing buffer) at the center position */
    {      
      mdma_Destination_address = MDMA_DEST_FRAME_BUFFER + lcd_image_position;
    }    
    else if(config == 2) /* copy the source image to the MDMA destination frame buffer (drawing buffer) at the center position with mirror effect */
    {
      mdma_Destination_address = (uint32_t)(MDMA_DEST_FRAME_BUFFER + lcd_image_position + ((IMAGE_WIDTH - 1) * RGB565_BYTES_PER_PIXEL));
    }
    else if(config == 3) /* copy the source image to the MDMA destination frame buffer (drawing buffer) at the center position with 180° rotation effect */
    {
      mdma_Destination_address = (uint32_t)(MDMA_DEST_FRAME_BUFFER + lcd_image_position + (((IMAGE_WIDTH - 1) + ( LCD_X_Size * (IMAGE_HEIGHT - 1))) * RGB565_BYTES_PER_PIXEL));
    }     
    else /*config == 4 , copy the source image to the MDMA destination frame buffer (drawing buffer) at the center position with 180° rotation and mirror effect */
    {
      mdma_Destination_address = (uint32_t)(MDMA_DEST_FRAME_BUFFER + lcd_image_position + ((( LCD_X_Size * (IMAGE_HEIGHT - 1))) * RGB565_BYTES_PER_PIXEL)) ;
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

    /*Start DMA2D transfer to copy the MDMA destination frame buffer (drawing buffer) to the LCD frame buffer (display buffer) */
    HAL_DMA2D_Start(&Dma2dHandle,
                         (uint32_t)MDMA_DEST_FRAME_BUFFER, /* Source buffer      */
                         (uint32_t)LCD_FRAME_BUFFER,    /* Destination buffer in format ARGB8888*/
                         LCD_X_Size,  /* width in pixels  */
                         LCD_Y_Size); /* height in pixels */
    
    HAL_DMA2D_PollForTransfer(&Dma2dHandle, 100);    
    
    BSP_LED_On(LED4);
    
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
  * @brief  MDMA Repeat blocks ransfer configuration
  * @note   This function configure the DMA for a Repeat BLock transfer.
  * @retval None
  */
static void MDMA_Config(uint32_t Config)
{
  
  /*##-1- Select the MDMA instance to be used for the transfer : MDMA_Channel0 #*/
  MDMA_Handle.Instance = MDMA_INSTANCE;
  
  HAL_MDMA_DeInit(&MDMA_Handle);
  
  /* Enable peripherals and GPIO Clocks */
  __HAL_RCC_MDMA_CLK_ENABLE();

  /* Configure NVIC for MDMA transfer complete/error interrupts */
  /* Set Interrupt Group Priority */
  HAL_NVIC_SetPriority(MDMA_IRQn, 0xF, 0xF);
  
  /* Enable the MDMA channel global Interrupt */
  HAL_NVIC_EnableIRQ(MDMA_IRQn);  
  
  /*##-2- Select the MDMA functional Parameters ###############################*/
  
  /* Set the MDMA parameters : For repeated block transfer*/  
  if(Config == 0)
  {
    /* config in Repeat Block to clear the MDMA destination frame buffer (drawing buffer) with default color 
       Each block corresponds to a line of the MDMA destination frame buffer (drawing buffer).
       The number of blocks corresponds to the number of lines in the MDMA destination frame buffer 
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
      Config to copy the source image to the MDMA destination frame buffer (drawing buffer) at the center position.
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
  
   /* Destination Block address offset is set in order to place the image in the center of the MDMA destination frame buffer (drawing buffer)
      the destination blocks are not contiguous. The LCD_Offset is = to LCD X size - image width */    
    MDMA_Handle.Init.DestBlockAddressOffset = LCD_Offset;    
  }    
  else if(Config == 2)
  {
    /* 
      Config to copy the source image to the MDMA destination frame buffer (drawing buffer) at the center position with mirror effect.
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
 
   /* Destination Block address offset is set in order to place the image in the center of the MDMA destination frame buffer (drawing buffer)
      the destination blocks are not contiguous. The LCD_Offset is = to LCD X size - image width
      Knowing that the destination address is set to the last pixel address in the first line 
      of the destination image, the destination block offset takes also into account that 
      the destination address is decrementing*/       
    MDMA_Handle.Init.DestBlockAddressOffset    = (int32_t)(LCD_Offset + (2 * (RGB565_BYTES_PER_PIXEL * IMAGE_WIDTH)));
  }
  else if(Config == 3)
  {
    /* Config to copy the source image to the MDMA destination frame buffer (drawing buffer) at the center position with 180° rotation effect 
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
 
    /* Destination Block address offset is set in order to place the image in the center of the MDMA destination frame buffer (drawing buffer)
      the destination blocks are not contiguous. The LCD_Offset is = to LCD X size - image width
      Knowing that the destination address is set to  the last pixel address of 
      the destination image, the destination block offset takes also into account that the destination address is decrementing*/    
    MDMA_Handle.Init.DestBlockAddressOffset    = (int32_t)((-1) * LCD_Offset);    
  }
  else if(Config == 4)
  {
    /* 
      Config copy the source image to the MDMA destination frame buffer (drawing buffer) at the center position with 180° rotation and mirror effect
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
 
    /* Destination Block address offset is set in order to place the image in the center of the MDMA destination frame buffer (drawing buffer)
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
  * @brief DMA2D configuration for RGB565 to ARGB8888 color conversion.
  * @param  None
  * @note  This function Configure the DMA2D peripheral :
  *        1) Configure the transfer mode : memory to memory with PFC
  *        2) Configure the output color mode as ARGB8888
  *        3) Configure the input and output OutputOffset to zero
  *        4) Configure the input color mode as RGB565
  * @retval
  *  None
  */
static void DMA2D_Config(void)
{
  /* Configure the DMA2D Mode, Color Mode and output offset */
  Dma2dHandle.Init.Mode          = DMA2D_M2M_PFC;         /* DMA2D Mode memory to memory  with PFC*/
  Dma2dHandle.Init.ColorMode     = DMA2D_OUTPUT_ARGB8888; /* Output color mode is ARGB8888 : 32 bpp */
  Dma2dHandle.Init.AlphaInverted = DMA2D_REGULAR_ALPHA;   /* No Output Alpha Inversion*/  
  Dma2dHandle.Init.RedBlueSwap   = DMA2D_RB_REGULAR;      /* No Output Red & Blue swap */    

  /* Output offset in pixels == nb of pixels to be added at end of line to come to the  */
  /* first pixel of the next line : on the output side of the DMA2D computation         */
  Dma2dHandle.Init.OutputOffset = 0;  

  /* DMA2D Callbacks Configuration */
  Dma2dHandle.XferCpltCallback  = NULL;
  Dma2dHandle.XferErrorCallback = NULL;

  /* Foreground Configuration : Layer 1 */
  Dma2dHandle.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
  Dma2dHandle.LayerCfg[1].InputAlpha = 0xFF;                   /* Fully opaque */
  Dma2dHandle.LayerCfg[1].InputColorMode = DMA2D_INPUT_RGB565; /* Foreground layer format is RGB565 : 16 bpp */
  Dma2dHandle.LayerCfg[1].InputOffset = 0x0;                   /* No offset in input */
  Dma2dHandle.LayerCfg[1].RedBlueSwap = DMA2D_RB_REGULAR;      /* No Output Red & Blue swap */
  Dma2dHandle.LayerCfg[1].AlphaInverted = DMA2D_REGULAR_ALPHA; /* No Alpha inversion */
  
  Dma2dHandle.Instance = DMA2D;

  /* DMA2D Initialization */
  if(HAL_DMA2D_DeInit(&Dma2dHandle) != HAL_OK)
  {
    Error_Handler();
  }
  
  if(HAL_DMA2D_Init(&Dma2dHandle) != HAL_OK)
  {
    Error_Handler();
  }

  /* DMA2D foreground layer configuration */  
  if(HAL_DMA2D_ConfigLayer(&Dma2dHandle, 1) != HAL_OK)
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
  BSP_LED_On(LED3);
  while(1)
  {
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
  * @brief  Configure the MPU attributes as Write Through for External SDRAM.
  * @note   The Base Address is 0xD0000000 .
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

