/**
  ******************************************************************************
  * @file    MDMA/MDMA_DMA2D_Triggering/CM7/Src/main.c
  * @author  MCD Application Team
  * @brief   This sample code shows how to use MDMA HAL API to start a transfer
  *          using the DMA2D Transfer Complete flag as the MDMA transfer trigger
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
#include "image_320x240_argb8888.h"
#include "life_augmented_argb8888.h"

/** @addtogroup STM32H7xx_HAL_Examples
  * @{
  */

/** @addtogroup MDMA_DMA2D_Triggering
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
MDMA_HandleTypeDef     MDMA_Handle;
DMA2D_HandleTypeDef    DMA2D_Handle;

__IO uint32_t MDMA_TransferErrorDetected = 0;
__IO uint32_t MDMA_RepeatBlockTransferCompleteDetected = 0;

static uint32_t ImageIndex = 0;
static uint32_t LCD_X_Size = 0, LCD_Y_Size = 0;

uint32_t dma2d_destination = 0;
uint32_t mdma_destination  = 0;
uint32_t dma2d_source      = 0; 
  
/* LCD Offset calculatd versus the image width */
uint32_t LCD_Offset = 0;

static const uint32_t * Images[] = 
{
  image_320x240_argb8888,
  life_augmented_argb8888,  
};

/*
According to the Ref Manual :  
  The channel configuration (LAR address) must be in the AXI address space.
  LAR value must be aligned at a Double Word address, i.e. LAR[2:0] = 0x0

The LAR address register represents the next Linked list Node address.
As consequence the Linked list transfer nodes must be 64 bits aligned and 
must be in the AXI address space.
*/
/* 32-bytes Alignment is needed for cache maintenance purpose */
ALIGN_32BYTES(MDMA_LinkNodeTypeDef Xfer_Node);

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void CPU_CACHE_Enable(void);
static void MPU_Config(void);
static void Error_Handler(void);
static void MDMA_RepeatBlockTransferCompleteCallback(MDMA_HandleTypeDef *hmdma);
static void MDMA_TransferErrorCallback(MDMA_HandleTypeDef *hmdma);
static void DMA2D_Config(uint16_t xSize);
static void MDMA_Config(void);

/* Private functions ---------------------------------------------------------*/


/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  uint32_t hal_status = HAL_OK;
  uint32_t xPos = 0;
  uint32_t yPos = 0;
  uint32_t mdma_lcd_image_position = 0;

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
  
  /* Configure leds */
  BSP_LED_Init(LED4);
  BSP_LED_Init(LED3);

   /* Initialize the LCD   */
  BSP_LCD_Init(0, LCD_ORIENTATION_LANDSCAPE);
  
  UTIL_LCD_SetFuncDriver(&LCD_Driver);   
  UTIL_LCD_SetLayer(0);   
  UTIL_LCD_Clear(UTIL_LCD_COLOR_WHITE);  
  
  /* Get the LCD Width and Height */
  BSP_LCD_GetXSize(0, &LCD_X_Size);  
  BSP_LCD_GetYSize(0, &LCD_Y_Size); 

  /* Calculate the MDMA and DMA2D destination transfer addresses */  
  xPos = (LCD_X_Size - IMAGE_WIDTH)/2;
  yPos = LCD_Y_Size/2;
  mdma_lcd_image_position = ((yPos * LCD_X_Size) + xPos) * ARGB8888_BYTES_PER_PIXEL;  
  
  dma2d_destination = (uint32_t)LCD_FRAME_BUFFER + (((LCD_X_Size - IMAGE_WIDTH)/2) * ARGB8888_BYTES_PER_PIXEL);
  
  mdma_destination = (uint32_t)(LCD_FRAME_BUFFER + mdma_lcd_image_position + ((( LCD_X_Size * (IMAGE_HEIGHT - 1))) * ARGB8888_BYTES_PER_PIXEL)) ;
  
  /* LCD Offset calculatd versus the image width */  
  LCD_Offset = (LCD_X_Size - IMAGE_WIDTH) * ARGB8888_BYTES_PER_PIXEL;
  
  /*##-1- Config and Start the MDMA transfer process #########################*/ 
  
  /* 
  Config copy the source image to the LCD frame buffer at the center position 
  with 180° rotation and mirror effect.
  Each block corresponds to a line of the source/destination image.
  The number of blocks corresponds to the number of lines of the image.

  The MDMA Request is set to DMA2D Transfer complete flag and the MDMA trigger Mode
  is set to Repeat block mode. As consequence each DMA2D transfer complete event
  (DMA2D has end copying an image to the Top-center of the LCD frame buffer ), 
  the MDMA will start a repeat block transfer which correspond of copying the DMA2D 
  image with 180° and rotation effect to the bottom-center of the LCD frame buffer. 
  */  
  MDMA_Config();
  
  hal_status = HAL_MDMA_Start_IT(&MDMA_Handle, (uint32_t)dma2d_destination,            /* MDMA source address set to DMA2D destination address */
                                               (uint32_t)mdma_destination,             /* MDMA destination address */
                                               IMAGE_WIDTH * ARGB8888_BYTES_PER_PIXEL, /* MDMA Block length in bytes */ 
                                               IMAGE_HEIGHT);                          /* MDMA Block count (number of blocks) */ 
  if(hal_status != HAL_OK)   
  {
    /* Transfer Error */
    Error_Handler();
  }


  /*##-2- Config and Start the DMA2D transfer process ########################*/ 
  
  /* Initialize the DMA2D to copy an image to the top-center of the LCD frame buffer */
  DMA2D_Config(IMAGE_WIDTH);
  
  /* Infinite loop */
  while (1)
  {
    /* Get the DMA2D source image*/
    dma2d_source = (uint32_t)Images[ImageIndex ++]; 
    
    /* Start the DMA2D transfer */
    hal_status = HAL_DMA2D_Start(&DMA2D_Handle, dma2d_source, dma2d_destination, IMAGE_WIDTH, IMAGE_HEIGHT);
    
    if(hal_status != HAL_OK)   
    {
       /* Transfer Error */
        Error_Handler();
    }                  
    
    /* Polling For DMA2D transfer complete */  
    hal_status = HAL_DMA2D_PollForTransfer(&DMA2D_Handle, 100);
    if(hal_status != HAL_OK)   
    {
       /* Transfer Error */
        Error_Handler();
    }    
    
    if(ImageIndex >= 2)
    {
      ImageIndex = 0;
    }
    
    /* Wait some time before switching to next image */
    HAL_Delay(2000); 
    
    BSP_LED_Off(LED4);
  }
}

/**
  * @brief  Initialize the MDMA For repeat block transfer in linked list circular
    with trigger set to DMA2D Transfer complete flag and trigger mode set to 
    repeat block transfer.
  * @param  None 
  * @retval None
  */
static void MDMA_Config(void)
{
  uint32_t hal_status = HAL_OK;  
  MDMA_LinkNodeConfTypeDef mdmaLinkNodeConfig;    
    
  /*##-1- Enable the MDMA clock ###############################################*/  
  __HAL_RCC_MDMA_CLK_ENABLE();  
  
  /*##-2- Select the MDMA instance to be used : MDMA_Channel0 #################*/
  MDMA_Handle.Instance = MDMA_Channel0;  
  
  /*##-3- Configure the MDMA for block transfer in HW request mode ############*/  
  /* 
  Config the MDMA to copy the source image to the LCD frame buffer at the center 
  position with 180° rotation and mirror effect.
  Each block corresponds to a line of the source/destination image.
  The number of blocks corresponds to the number of lines of the image.
  
  The MDMA Request is set to DMA2D Transfer complete flag and the MDMA trigger Mode
  is set to Repeat block mode. As consequence each DMA2D transfer complete event
  (DMA2D has end copying an image to the Top-center of the LCD frame buffer ), 
  the MDMA will start a repeat block transfer which correspond of copying the DMA2D 
  image with 180° and rotation effect to the bottom-center of the LCD frame buffer.
  */
  
  MDMA_Handle.Init.Request              = MDMA_REQUEST_DMA2D_TC;
  MDMA_Handle.Init.TransferTriggerMode  = MDMA_REPEAT_BLOCK_TRANSFER;  
  MDMA_Handle.Init.Priority             = MDMA_PRIORITY_HIGH;
  MDMA_Handle.Init.Endianness           = MDMA_LITTLE_ENDIANNESS_PRESERVE;
  MDMA_Handle.Init.DataAlignment        = MDMA_DATAALIGN_PACKENABLE;                            
  MDMA_Handle.Init.SourceBurst          = MDMA_SOURCE_BURST_SINGLE;
  MDMA_Handle.Init.DestBurst            = MDMA_DEST_BURST_SINGLE;
  MDMA_Handle.Init.BufferTransferLength = 128;
  
  /* Source and Destination data size are word , 4 bytes that correspond to an ARGB8888 pixel 32 bpp */     
  MDMA_Handle.Init.SourceDataSize       = MDMA_SRC_DATASIZE_WORD;
  MDMA_Handle.Init.DestDataSize         = MDMA_DEST_DATASIZE_WORD;
  
  /* Source and Destination  Increment is word , 4 bytes that correspond to an ARGB8888 pixel 32 bpp */      
  MDMA_Handle.Init.SourceInc            = MDMA_SRC_INC_WORD;
  MDMA_Handle.Init.DestinationInc       = MDMA_DEST_INC_WORD;

  /*Source Block address offset is set to LCD Offset as the source image is in the center of the LCD */    
  MDMA_Handle.Init.SourceBlockAddressOffset  = (int32_t)LCD_Offset;
  
  /* Destination Block address offset is set in order to place the image in the center of the LCD frame buffer
  the destination blocks are not contiguous. The LCD_Offset is = to LCD X size - image width
  Knowing that the destination address is set to the first pixel address of the last line 
  in the destination image, the destination block offset allows to decrement the destination address 
  after each block in order to start the next block at previous line */      
  MDMA_Handle.Init.DestBlockAddressOffset    = ((-2) * ((int32_t)(ARGB8888_BYTES_PER_PIXEL * IMAGE_WIDTH))) - ((int32_t)LCD_Offset);   
  
  /*##-4- Initialize the MDMA channel ##########################################*/  
  hal_status = HAL_MDMA_Init(&MDMA_Handle);
  
  if(hal_status != HAL_OK)  
  {
    /* Initialization Error */
    Error_Handler();
  }

  HAL_MDMA_ConfigPostRequestMask(&MDMA_Handle, 0, 0);
  
  /*##-5- Create Linked list Node ############################################*/ 
  
  /* 
     Create and add a linked list node that have the same parameters as the 
     initial transfer.
     The purpose of the linked list node is to create a circular list. the MDMA 
     transfer will then loop infinitely. Each DMA2D Transfer complete flag will trig
     a Node transfer ( a repeat block transfer) that correspond to a new image transfer
  */
  mdmaLinkNodeConfig.Init.Request              = MDMA_Handle.Init.Request;
  mdmaLinkNodeConfig.Init.TransferTriggerMode  = MDMA_Handle.Init.TransferTriggerMode;
  mdmaLinkNodeConfig.Init.Priority             = MDMA_Handle.Init.Priority;          
  mdmaLinkNodeConfig.Init.Endianness           = MDMA_Handle.Init.Endianness;          
  
  mdmaLinkNodeConfig.Init.SourceInc            = MDMA_Handle.Init.SourceInc;     
  mdmaLinkNodeConfig.Init.DestinationInc       = MDMA_Handle.Init.DestinationInc;
  
  mdmaLinkNodeConfig.Init.SourceDataSize       = MDMA_Handle.Init.SourceDataSize;      
  mdmaLinkNodeConfig.Init.DestDataSize         = MDMA_Handle.Init.DestDataSize;        
  mdmaLinkNodeConfig.Init.DataAlignment        = MDMA_Handle.Init.DataAlignment;                                  
  mdmaLinkNodeConfig.Init.SourceBurst          = MDMA_Handle.Init.SourceBurst;         
  mdmaLinkNodeConfig.Init.DestBurst            = MDMA_Handle.Init.DestBurst;           
  mdmaLinkNodeConfig.Init.BufferTransferLength = MDMA_Handle.Init.BufferTransferLength;
  
  mdmaLinkNodeConfig.Init.SourceBlockAddressOffset  = MDMA_Handle.Init.SourceBlockAddressOffset;
  mdmaLinkNodeConfig.Init.DestBlockAddressOffset    = MDMA_Handle.Init.DestBlockAddressOffset;
  
  mdmaLinkNodeConfig.PostRequestMaskAddress = 0;
  mdmaLinkNodeConfig.PostRequestMaskData = 0;  
  
  /* Create Node*/  
  mdmaLinkNodeConfig.SrcAddress      = dma2d_destination;
  mdmaLinkNodeConfig.DstAddress      = mdma_destination;
  mdmaLinkNodeConfig.BlockDataLength = IMAGE_WIDTH * ARGB8888_BYTES_PER_PIXEL;
  mdmaLinkNodeConfig.BlockCount      = IMAGE_HEIGHT;

  HAL_MDMA_LinkedList_CreateNode(&Xfer_Node, &mdmaLinkNodeConfig);
  
  /*Add created Node to the linkedlist */
  HAL_MDMA_LinkedList_AddNode(&MDMA_Handle, &Xfer_Node, 0);
  
  /* Make the linked list circular */
  HAL_MDMA_LinkedList_EnableCircularMode(&MDMA_Handle);
  
  /* 
    As the MDMA Node descriptor "Xfer_Node" is located in the D1 AXI-SRAM which 
    is cacheable, it is necessary to clean the data cache after creating the node 
    in order to make sure that the MDMA will load up to date data from the linked list node
  */
  SCB_CleanDCache_by_Addr((uint32_t *)&(Xfer_Node), sizeof(MDMA_LinkNodeTypeDef)); 
  
  /*##-6- Select Callbacks functions called after MDMA Repeat block Transfer complete and Transfer error */
  HAL_MDMA_RegisterCallback(&MDMA_Handle, HAL_MDMA_XFER_REPBLOCKCPLT_CB_ID, MDMA_RepeatBlockTransferCompleteCallback);
  HAL_MDMA_RegisterCallback(&MDMA_Handle, HAL_MDMA_XFER_ERROR_CB_ID, MDMA_TransferErrorCallback);
  
  /*##-7- Configure NVIC for MDMA transfer complete/error interrupts ##########*/
  /* Set Interrupt Group Priority */
  HAL_NVIC_SetPriority(MDMA_IRQn, 0, 0);
  
  /* Enable the MDMA channel global Interrupt */
  HAL_NVIC_EnableIRQ(MDMA_IRQn);  
  
 
}

/**
  * @brief  Initialize the DMA2D for Mem TO Mem copy with output offset set to
    place the image in the center of the LCD frame buffer.
  * @param  xSize: Image width  
  * @retval None
  */
static void DMA2D_Config(uint16_t xSize)
{   
  uint32_t hal_status = HAL_OK;
  
  /*##-1- Configure the DMA2D Mode, Color Mode and output offset #############*/ 
  DMA2D_Handle.Init.Mode          = DMA2D_M2M;
  DMA2D_Handle.Init.ColorMode     = DMA2D_OUTPUT_ARGB8888;
  DMA2D_Handle.Init.OutputOffset  = LCD_X_Size - xSize; 
  DMA2D_Handle.Init.AlphaInverted = DMA2D_REGULAR_ALPHA;  /* No Output Alpha Inversion*/  
  DMA2D_Handle.Init.RedBlueSwap   = DMA2D_RB_REGULAR;     /* No Output Red & Blue swap */  
  
  /*##-2- DMA2D Callbacks Configuration ######################################*/
  DMA2D_Handle.XferCpltCallback  = NULL;
  
  /*##-3- Foreground Configuration ###########################################*/
  DMA2D_Handle.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
  DMA2D_Handle.LayerCfg[1].InputAlpha = 0xFF;
  DMA2D_Handle.LayerCfg[1].InputColorMode = DMA2D_INPUT_ARGB8888;
  DMA2D_Handle.LayerCfg[1].InputOffset = 0;
  DMA2D_Handle.LayerCfg[1].RedBlueSwap = DMA2D_RB_REGULAR; /* No ForeGround Red/Blue swap */
  DMA2D_Handle.LayerCfg[1].AlphaInverted = DMA2D_REGULAR_ALPHA; /* No ForeGround Alpha inversion */  

  DMA2D_Handle.Instance          = DMA2D; 
   
  /* DMA2D Initialization */
  hal_status = HAL_DMA2D_Init(&DMA2D_Handle);
  
  if(hal_status != HAL_OK)
  {
    Error_Handler();
  } 

  hal_status = HAL_DMA2D_ConfigLayer(&DMA2D_Handle, 1);
  
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
  * @brief  MDMA Repeat block Transfer complete callback
  * @note   This function is executed when the transfer complete interrupt
  *         is generated
  * @retval None
  */
static void MDMA_RepeatBlockTransferCompleteCallback(MDMA_HandleTypeDef *hmdma)
{
  /* Turn LED4 on: MDMA Transfer is correct */
  BSP_LED_On(LED4);
  MDMA_RepeatBlockTransferCompleteDetected = 1;
}

/**
  * @brief  MDMA transfer error callback
  * @note   This function is executed when the transfer error interrupt
  *         is generated during MDMA transfer
  * @retval None
  */
static void MDMA_TransferErrorCallback(MDMA_HandleTypeDef *hmdma)
{
  MDMA_TransferErrorDetected = 1;

  /* Turn LED3 on: MDMA Transfer error */
  BSP_LED_On(LED3);  
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

