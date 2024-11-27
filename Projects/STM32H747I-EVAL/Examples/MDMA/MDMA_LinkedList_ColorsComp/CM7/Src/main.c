/**
  ******************************************************************************
  * @file    MDMA/MDMA_LinkedList_ColorsComp/CM7/Src/main.c
  * @author  MCD Application Team
  * @brief   This example shows how to use the MDMA in linked list mode to decimate by 2 
  *          and decompose colors components of an ARGB8888 image.
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

/** @addtogroup STM32H7xx_HAL_Examples
  * @{
  */

/** @defroup MDMA_LinkedList_ColorsComp
  * @{
  */

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
MDMA_HandleTypeDef          MDMA_Handle;

__IO uint32_t TransferErrorDetected = 0;
__IO uint32_t TransferCompleteDetected = 0;

uint32_t LCD_X_Size = 0, LCD_Y_Size = 0;

/* 32-bytes Alignment is needed for cache maintenance purpose */
ALIGN_32BYTES(MDMA_LinkNodeTypeDef Xfer_Node0_1);
ALIGN_32BYTES(MDMA_LinkNodeTypeDef Xfer_Node0_2);
ALIGN_32BYTES(MDMA_LinkNodeTypeDef Xfer_Node0_3);
ALIGN_32BYTES(MDMA_LinkNodeTypeDef Xfer_Node1);
ALIGN_32BYTES(MDMA_LinkNodeTypeDef Xfer_Node2);
ALIGN_32BYTES(MDMA_LinkNodeTypeDef Xfer_Node3);

const uint32_t RGBReset_Value = 0xFF000000;

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void CPU_CACHE_Enable(void);
static void MPU_Config(void);
static void Error_Handler(void);
static void MDMA_TransferCompleteCallback(MDMA_HandleTypeDef *hmdma);
static void MDMA_TransferErrorCallback(MDMA_HandleTypeDef *hmdma);
static void MDMA_Config(void);
static void DisplayColor(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  uint32_t destination_node0 = 0;
  uint32_t hal_status = HAL_OK;

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
  UTIL_LCD_Clear(UTIL_LCD_COLOR_WHITE); 
  UTIL_LCD_SetFont(&Font24);
  
  BSP_LCD_GetXSize(0, &LCD_X_Size);
  BSP_LCD_GetYSize(0, &LCD_Y_Size);
  
  /* Display colors description */
  DisplayColor();
  
  /*##-2- Config the MDMA for Linked list : each node represent a color component : Red, Green, Blue #*/  
  MDMA_Config();

  /*Reset the MDMA Transfer complete a error status*/
  TransferCompleteDetected = 0;
  TransferErrorDetected = 0;
  
  /*##-3- Start the MDMA transfer with source and destination address set for node 0 to copy original image #*/  
  destination_node0 = (uint32_t)LCD_FRAME_BUFFER + ((16 * LCD_X_Size) + ((LCD_X_Size - IMAGE_WIDTH)/2)) * ARGB8888_BYTES_PER_PIXEL;
                                            
  /* note the MDMA is configured in linked list mode */
  hal_status = HAL_MDMA_Start_IT(&MDMA_Handle, ((uint32_t)&image_320x240_argb8888),
                                               destination_node0 , 
                                               IMAGE_WIDTH * ARGB8888_BYTES_PER_PIXEL, 
                                               IMAGE_HEIGHT);
  
  if(hal_status != HAL_OK)
  {
    /* Transfer Error */
    Error_Handler();
  }
  else
  {        
    /* Wait for MDMA transfer complete or error*/
    while((TransferCompleteDetected == 0) && (TransferErrorDetected == 0)) {}
    if(TransferErrorDetected)
    {
      /* Transfer Error */
      Error_Handler();        
    }
    else
    {
      /* Turn LED4 on: Transfer correct */
      BSP_LED_On(LED4);
      /*Display transformed Images one image for each color Component*/
    }
  }     
  
  /* Infinite loop */
  while (1);    
}

/**
  * @brief  MDMA Linked List transfer configuration
  * @note   This function configure the DMA for a Repeat BLock transfer.
  * @param  None
  * @retval None
  */
static void MDMA_Config(void)
{
  uint32_t mdma_Destination_address = 0, mdma_Source_address, x = 0, y = 0;   
  MDMA_LinkNodeConfTypeDef mdmaLinkNodeConfig;  
  
  /*##-1- Enable the MDMA clock ###############################################*/
  __HAL_RCC_MDMA_CLK_ENABLE();
  
  /*##-2- Select the MDMA instance to be used for the transfer : MDMA_Channel0 #*/
  MDMA_Handle.Instance = MDMA_INSTANCE;
  
  HAL_MDMA_DeInit(&MDMA_Handle);
  
  /*##-3- Initialize the MDMA channel (with liked list node 0 parameters) ####*/ 
  MDMA_Handle.Init.Request              = MDMA_REQUEST_SW;
  MDMA_Handle.Init.TransferTriggerMode  = MDMA_FULL_TRANSFER;  
  MDMA_Handle.Init.Priority             = MDMA_PRIORITY_HIGH;                          
  MDMA_Handle.Init.Endianness           = MDMA_LITTLE_ENDIANNESS_PRESERVE;

  MDMA_Handle.Init.SourceInc            = MDMA_SRC_INC_WORD;
  MDMA_Handle.Init.DestinationInc       = MDMA_DEST_INC_WORD;  
    
  MDMA_Handle.Init.SourceDataSize       = MDMA_SRC_DATASIZE_WORD;
  MDMA_Handle.Init.DestDataSize         = MDMA_DEST_DATASIZE_WORD;
  MDMA_Handle.Init.DataAlignment        = MDMA_DATAALIGN_PACKENABLE;                            
  MDMA_Handle.Init.SourceBurst          = MDMA_SOURCE_BURST_SINGLE;
  MDMA_Handle.Init.DestBurst            = MDMA_DEST_BURST_SINGLE;
  MDMA_Handle.Init.BufferTransferLength = 64;
      
  MDMA_Handle.Init.SourceBlockAddressOffset  = 0;
  MDMA_Handle.Init.DestBlockAddressOffset    = (LCD_X_Size - IMAGE_WIDTH) * ARGB8888_BYTES_PER_PIXEL;

  if (HAL_MDMA_Init(&MDMA_Handle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  

  /*##-4- Create Linked list Node 0_1, 0_2 and 0_3 to erase Red/Green Blue sub images ####*/       
  
  mdmaLinkNodeConfig.Init.Request              = MDMA_REQUEST_SW;
  mdmaLinkNodeConfig.Init.TransferTriggerMode  = MDMA_FULL_TRANSFER;  
  mdmaLinkNodeConfig.Init.Priority             = MDMA_PRIORITY_HIGH;
  mdmaLinkNodeConfig.Init.Endianness           = MDMA_LITTLE_ENDIANNESS_PRESERVE;
  
  mdmaLinkNodeConfig.Init.SourceInc            = MDMA_SRC_INC_DISABLE;
  mdmaLinkNodeConfig.Init.DestinationInc       = MDMA_DEST_INC_WORD;
  
  mdmaLinkNodeConfig.Init.SourceDataSize       = MDMA_SRC_DATASIZE_WORD;
  mdmaLinkNodeConfig.Init.DestDataSize         = MDMA_DEST_DATASIZE_WORD;
  mdmaLinkNodeConfig.Init.DataAlignment        = MDMA_DATAALIGN_PACKENABLE;                            
  mdmaLinkNodeConfig.Init.SourceBurst          = MDMA_SOURCE_BURST_SINGLE;
  mdmaLinkNodeConfig.Init.DestBurst            = MDMA_DEST_BURST_SINGLE;
  mdmaLinkNodeConfig.Init.BufferTransferLength = 64;
  
  mdmaLinkNodeConfig.Init.SourceBlockAddressOffset  = 0;
  mdmaLinkNodeConfig.Init.DestBlockAddressOffset    = (LCD_X_Size - (IMAGE_WIDTH/2)) * ARGB8888_BYTES_PER_PIXEL;
  
  /* Create Node0_1 : erase Red region */
  mdma_Source_address = (uint32_t)(&RGBReset_Value);  
  x = 80;
  y = (LCD_Y_Size - (IMAGE_HEIGHT/2)) - 16;
  mdma_Destination_address = (uint32_t)LCD_FRAME_BUFFER  + ((y * LCD_X_Size) + x) * ARGB8888_BYTES_PER_PIXEL;
  
  mdmaLinkNodeConfig.SrcAddress      = mdma_Source_address;
  mdmaLinkNodeConfig.DstAddress      = mdma_Destination_address;
  mdmaLinkNodeConfig.BlockDataLength = (IMAGE_WIDTH/2) * ARGB8888_BYTES_PER_PIXEL;
  mdmaLinkNodeConfig.BlockCount      = IMAGE_HEIGHT/2;
  
  HAL_MDMA_LinkedList_CreateNode(&Xfer_Node0_1, &mdmaLinkNodeConfig);

  /* Create Node0_2 : erase Green region*/
  mdma_Source_address = (uint32_t)(&RGBReset_Value);  
  x = 160 + (IMAGE_WIDTH/2);
  y = (LCD_Y_Size - (IMAGE_HEIGHT/2)) - 16;
  mdma_Destination_address = (uint32_t)LCD_FRAME_BUFFER  + ((y * LCD_X_Size) + x) * ARGB8888_BYTES_PER_PIXEL;
  

  mdmaLinkNodeConfig.SrcAddress = mdma_Source_address;
  mdmaLinkNodeConfig.DstAddress = mdma_Destination_address;
  mdmaLinkNodeConfig.BlockDataLength = (IMAGE_WIDTH/2) * ARGB8888_BYTES_PER_PIXEL;
  mdmaLinkNodeConfig.BlockCount      = IMAGE_HEIGHT/2; 
  
  HAL_MDMA_LinkedList_CreateNode(&Xfer_Node0_2, &mdmaLinkNodeConfig);
  
  /* Create Node0_3 : erase Blue region*/
  mdma_Source_address = (uint32_t)(&RGBReset_Value);  
  x = 240 + IMAGE_WIDTH;
  y = (LCD_Y_Size - (IMAGE_HEIGHT/2)) - 16;
  mdma_Destination_address = (uint32_t)LCD_FRAME_BUFFER  + ((y * LCD_X_Size) + x) * ARGB8888_BYTES_PER_PIXEL;
  
  mdmaLinkNodeConfig.SrcAddress      = mdma_Source_address;
  mdmaLinkNodeConfig.DstAddress      = mdma_Destination_address;
  mdmaLinkNodeConfig.BlockDataLength = (IMAGE_WIDTH/2) * ARGB8888_BYTES_PER_PIXEL;
  mdmaLinkNodeConfig.BlockCount      = IMAGE_HEIGHT/2;
  
  HAL_MDMA_LinkedList_CreateNode(&Xfer_Node0_3, &mdmaLinkNodeConfig);

  
  /*##-5- Create Linked list Node 1, Node2 and Node3 to copy Red/Green Blue subimages ###*/ 
  mdmaLinkNodeConfig.Init.Request              = MDMA_REQUEST_SW;
  mdmaLinkNodeConfig.Init.TransferTriggerMode  = MDMA_FULL_TRANSFER;  
  mdmaLinkNodeConfig.Init.Priority             = MDMA_PRIORITY_HIGH;
  mdmaLinkNodeConfig.Init.Endianness           = MDMA_LITTLE_ENDIANNESS_PRESERVE;
  
  mdmaLinkNodeConfig.Init.SourceInc            = MDMA_SRC_INC_DOUBLEWORD;
  mdmaLinkNodeConfig.Init.DestinationInc       = MDMA_DEST_INC_WORD;
  
  mdmaLinkNodeConfig.Init.SourceDataSize       = MDMA_SRC_DATASIZE_BYTE;
  mdmaLinkNodeConfig.Init.DestDataSize         = MDMA_DEST_DATASIZE_BYTE;
  mdmaLinkNodeConfig.Init.DataAlignment        = MDMA_DATAALIGN_PACKENABLE;                            
  mdmaLinkNodeConfig.Init.SourceBurst          = MDMA_SOURCE_BURST_SINGLE;
  mdmaLinkNodeConfig.Init.DestBurst            = MDMA_DEST_BURST_SINGLE;
  mdmaLinkNodeConfig.Init.BufferTransferLength = 64;
  
  mdmaLinkNodeConfig.Init.SourceBlockAddressOffset  = IMAGE_WIDTH * ARGB8888_BYTES_PER_PIXEL;
  mdmaLinkNodeConfig.Init.DestBlockAddressOffset    = (LCD_X_Size - (IMAGE_WIDTH/2)) * ARGB8888_BYTES_PER_PIXEL;
  
  /* Create Node1*/
  /* Set the parameters to be configured for transfer Node1 : Red color component */ 
  /* Set source and destination start address to extract Red component*/  
  
  mdma_Source_address = ((uint32_t)&image_320x240_argb8888) + 2;  
  x = 80;
  y = (LCD_Y_Size - (IMAGE_HEIGHT/2)) - 16;
  mdma_Destination_address = 2+ ((uint32_t)LCD_FRAME_BUFFER  + ((y * LCD_X_Size) + x) * ARGB8888_BYTES_PER_PIXEL);    

  mdmaLinkNodeConfig.SrcAddress      = mdma_Source_address;
  mdmaLinkNodeConfig.DstAddress      = mdma_Destination_address;
  mdmaLinkNodeConfig.BlockDataLength = IMAGE_WIDTH/2;
  mdmaLinkNodeConfig.BlockCount      = IMAGE_HEIGHT/2;  
  
  HAL_MDMA_LinkedList_CreateNode(&Xfer_Node1, &mdmaLinkNodeConfig);

  /* Create Node2*/
  /* Set the parameters to be configured for transfer Node2 : Green color component */ 
  /* Set source and destination start address to extract Green component*/        
  
  mdma_Source_address = ((uint32_t)&image_320x240_argb8888) + 1;  
  x = 160 + (IMAGE_WIDTH/2);
  y = (LCD_Y_Size - (IMAGE_HEIGHT/2)) - 16;
  mdma_Destination_address = 1 + ((uint32_t)LCD_FRAME_BUFFER  + ((y * LCD_X_Size) + x) * ARGB8888_BYTES_PER_PIXEL);  

  mdmaLinkNodeConfig.SrcAddress      = mdma_Source_address;
  mdmaLinkNodeConfig.DstAddress      = mdma_Destination_address;
  mdmaLinkNodeConfig.BlockDataLength = IMAGE_WIDTH/2;
  mdmaLinkNodeConfig.BlockCount      = IMAGE_HEIGHT/2;    
  
  HAL_MDMA_LinkedList_CreateNode(&Xfer_Node2, &mdmaLinkNodeConfig);
  

  /* Create Node3*/
  /* Set the parameters to be configured for transfer Node1 : Blue color component */ 
  /* Set source and destination start address to extract Blue component*/  
  mdma_Source_address = ((uint32_t)&image_320x240_argb8888);
  x = 240 + IMAGE_WIDTH;
  y = (LCD_Y_Size - (IMAGE_HEIGHT/2)) - 16;  
  mdma_Destination_address = (uint32_t)LCD_FRAME_BUFFER  + ((y * LCD_X_Size) + x) * ARGB8888_BYTES_PER_PIXEL;  

  mdmaLinkNodeConfig.SrcAddress      = mdma_Source_address;
  mdmaLinkNodeConfig.DstAddress      = mdma_Destination_address;
  mdmaLinkNodeConfig.BlockDataLength = IMAGE_WIDTH/2;
  mdmaLinkNodeConfig.BlockCount      = IMAGE_HEIGHT/2;      
  
  HAL_MDMA_LinkedList_CreateNode(&Xfer_Node3, &mdmaLinkNodeConfig);  
    
  /*##-5- Add linked list nodes ##############################################*/ 
  HAL_MDMA_LinkedList_AddNode(&MDMA_Handle, &Xfer_Node0_1, 0);
  HAL_MDMA_LinkedList_AddNode(&MDMA_Handle, &Xfer_Node0_2, 0);
  HAL_MDMA_LinkedList_AddNode(&MDMA_Handle, &Xfer_Node0_3, 0);
    
  HAL_MDMA_LinkedList_AddNode(&MDMA_Handle, &Xfer_Node1, 0);
  HAL_MDMA_LinkedList_AddNode(&MDMA_Handle, &Xfer_Node2, 0);
  HAL_MDMA_LinkedList_AddNode(&MDMA_Handle, &Xfer_Node3, 0);
  
  /* 
    As the MDMA Nodes descriptor are located in the SRAM which 
    is cacheable, it is necessary to clean the data cache after creating the node 
    in order to make sure that the MDMA will load up to date data from the linked list node
  */
  SCB_CleanDCache();

  /*##-6- Select Callbacks functions called after Transfer complete and Transfer error */
  HAL_MDMA_RegisterCallback(&MDMA_Handle, HAL_MDMA_XFER_CPLT_CB_ID, MDMA_TransferCompleteCallback);
  HAL_MDMA_RegisterCallback(&MDMA_Handle, HAL_MDMA_XFER_ERROR_CB_ID, MDMA_TransferErrorCallback);

  /*##-7- Configure NVIC for MDMA transfer complete/error interrupts ##########*/
  /* Set Interrupt Group Priority */
  HAL_NVIC_SetPriority(MDMA_IRQn, 0, 0);
  
  /* Enable the MDMA channel global Interrupt */
  HAL_NVIC_EnableIRQ(MDMA_IRQn);
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

static void DisplayColor(void)
{      
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_RED);
  UTIL_LCD_DisplayStringAt(80 , LCD_Y_Size - (IMAGE_HEIGHT/2) - 40 , (uint8_t *)"Red", LEFT_MODE); 
  
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_GREEN);        
  UTIL_LCD_DisplayStringAt(160 + (IMAGE_WIDTH/2) , LCD_Y_Size - (IMAGE_HEIGHT/2) - 40, (uint8_t *)"Green", LEFT_MODE); 
  
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLUE);        
  UTIL_LCD_DisplayStringAt(240 + IMAGE_WIDTH  , LCD_Y_Size - (IMAGE_HEIGHT/2) - 40, (uint8_t *)"Blue", LEFT_MODE);
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

