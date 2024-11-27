/**
  ******************************************************************************
  * @file    MDMA/MDMA_LinkedList/Src/main.c
  * @author  MCD Application Team
  * @brief   This example shows how to use the MDMA to start a transfer in linked list mode
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

/** @addtogroup STM32H7xx_HAL_Examples
  * @{
  */

/** @defroup MDMA_LinkedList
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
#define BUFFER_SIZE0 32
#define BUFFER_SIZE1 16
#define BUFFER_SIZE2 8

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
MDMA_HandleTypeDef     MDMA_Handle;

static const uint32_t SRC_Const_Buffer0[BUFFER_SIZE0] =
{
  0x01020304, 0x05060708, 0x090A0B0C, 0x0D0E0F10,
  0x11121314, 0x15161718, 0x191A1B1C, 0x1D1E1F20,
  0x21222324, 0x25262728, 0x292A2B2C, 0x2D2E2F30,
  0x31323334, 0x35363738, 0x393A3B3C, 0x3D3E3F40,
  0x41424344, 0x45464748, 0x494A4B4C, 0x4D4E4F50,
  0x51525354, 0x55565758, 0x595A5B5C, 0x5D5E5F60,
  0x61626364, 0x65666768, 0x696A6B6C, 0x6D6E6F70,
  0x71727374, 0x75767778, 0x797A7B7C, 0x7D7E7F80
};

static const uint32_t SRC_Const_Buffer1[BUFFER_SIZE1] =
{
  0x0A0B0C0D, 0x1A1B1C1D, 0x2A2B2C2D, 0x2A2B2C2D,
  0x3A3B3C3D, 0x4A4B4C4D, 0x5A5B5C5D, 0x6A6B6C6D,
  0x7A7B7C7D, 0x8A8B8C8D, 0x9A9B9C9D, 0xAAABACAD,
  0xBABBBCBD, 0xCACBCCCD, 0xDADBDCDD, 0xEAEBECED
};

static const uint32_t SRC_Const_Buffer2[BUFFER_SIZE2] =
{
  0x00010203, 0x04050607, 0x08090A0B, 0x0C0D0E0F,
  0x00112233, 0x44556677, 0x8899AABB, 0xCCDDEEFF
};

/******** MDMA Destination 0 Buffer definition *******/
/*Buffer location and size should aligned to cache line size (32 bytes) */
#if defined ( __ICCARM__ )
#pragma location = 0x24004000
uint32_t DESTBuffer0_D1_AXISRAM[BUFFER_SIZE0];
#elif defined ( __CC_ARM )
ALIGN_32BYTES(__attribute__((section (".RAM_D1"))) uint32_t DESTBuffer0_D1_AXISRAM[BUFFER_SIZE0]);
#elif defined ( __GNUC__ )
ALIGN_32BYTES(uint32_t __attribute__((section (".RAM_D1"))) DESTBuffer0_D1_AXISRAM[BUFFER_SIZE0]);
#endif

/*****************************************************/


/******** MDMA Destination 1 Buffer definition *******/
/*Buffer location and size should aligned to cache line size (32 bytes) */
#if defined ( __ICCARM__ )
#pragma location = 0x30000000
uint32_t DESTBuffer1_D2_SRAM1[BUFFER_SIZE0];
#elif defined ( __CC_ARM )
ALIGN_32BYTES(__attribute__((section (".RAM_D2"))) uint32_t DESTBuffer1_D2_SRAM1[BUFFER_SIZE0]);
#elif defined ( __GNUC__ )
ALIGN_32BYTES(uint32_t __attribute__((section (".RAM_D2"))) DESTBuffer1_D2_SRAM1[BUFFER_SIZE0]);
#endif
/*****************************************************/


/******** MDMA Destination 2 Buffer definition *******/
/*Buffer location and size should aligned to cache line size (32 bytes) */
#if defined ( __ICCARM__ )
#pragma location = 0x38000000
uint32_t DESTBuffer2_D3_AHBSRAM[BUFFER_SIZE0];
#elif defined ( __CC_ARM )
ALIGN_32BYTES(uint32_t __attribute__((section (".RAM_D3"))) DESTBuffer2_D3_AHBSRAM[BUFFER_SIZE0]);
#elif defined ( __GNUC__ )
ALIGN_32BYTES(uint32_t __attribute__((section (".RAM_D3"))) DESTBuffer2_D3_AHBSRAM[BUFFER_SIZE0]);
#endif
/*****************************************************/



__IO uint32_t TransferErrorDetected = 0;
__IO uint32_t TransferCompleteDetected = 0;


/******** Linked list nodes 1 to 2 definition *******/

/*
According to the Ref Manual :
  The channel configuration (LAR address) must be in the AXI address space.
  LAR value must be aligned at a Double Word address, i.e. LAR[2:0] = 0x0

The LAR address register represents the next Linked list Node address.
As consequence the Liked list transfer nodes must be 64 bits aligned and must be in the AXI address space.
*/
/* 32-bytes Alignment is needed for cache maintenance purpose */
ALIGN_32BYTES(MDMA_LinkNodeTypeDef Xfer_Node1);
ALIGN_32BYTES(MDMA_LinkNodeTypeDef Xfer_Node2);
/*****************************************************/

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void CPU_CACHE_Enable(void);
static void Error_Handler(void);
static void MDMA_TransferCompleteCallback(MDMA_HandleTypeDef *hmdma);
static void MDMA_TransferErrorCallback(MDMA_HandleTypeDef *hmdma);
static void MDMA_Config(void);
static uint16_t Buffercmp(uint8_t *pBuffer1, uint8_t *pBuffer2, uint16_t BufferLength);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{

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

  /* -1- Initialize LEDs */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);

  /*##-2- Configure and start the MDMA Linked list transfer #####################*/
  MDMA_Config();

  /*##-3- Wait for the end of the transfer of error #############################*/
  while((TransferCompleteDetected == 0) && (TransferErrorDetected == 0))
  {
  }

  /*##-4- if transfer completed compare source and destinations buffers for all nodes ##*/
  if(TransferCompleteDetected != 0)
  {
    TransferCompleteDetected = 0;  /* Reset Transfer Complete Detect Flag */

    /* Turn LED1 on: Transfer complete */
    BSP_LED_On(LED1);

    if(Buffercmp((uint8_t *)SRC_Const_Buffer0, (uint8_t *)DESTBuffer0_D1_AXISRAM, (BUFFER_SIZE0 * 4)) != 0)
    {
      TransferErrorDetected = 1;   /* Set the Transfer Error Detect Flag */
    }

    if(Buffercmp((uint8_t *)SRC_Const_Buffer1, (uint8_t *)DESTBuffer1_D2_SRAM1, (BUFFER_SIZE1 * 4)) != 0)
    {
      TransferErrorDetected = 1;    /* Set the Transfer Error Detect Flag */
    }

    if(Buffercmp((uint8_t *)SRC_Const_Buffer2, (uint8_t *)DESTBuffer2_D3_AHBSRAM, (BUFFER_SIZE2 * 4)) != 0)
    {
      TransferErrorDetected = 1;    /* Set the Transfer Error Detect Flag */
    }
  }

  /*##-4- if transfer error detcetd  Turn LED2 (Red LED) On  ##*/
  if (TransferErrorDetected != 0)
  {
    /* Turn LED2 on To indicate a transfer Error*/
    BSP_LED_On(LED2);
    TransferErrorDetected = 0;     /* Reset Transfer Error Detect Flag */
  }


  /* Infinite loop */
  while (1)
  {
  }
}

/**
  * @brief  MDMA Liked List transfer configuration
  * @note   This function configure the DMA for a linked list transfer.
  *         The linked list contains 3 nodes.
  *         Node 0 transfer parameters are configured within the Handle init parameters.
  *         Next nodes are filled using HAL function "HAL_MDMA_SetLinkNodeParams"
  *         after setting all nodes parameters (node 0 using HAL_MDMA_Init and
  *         next nodes uing HAL_MDMA_SetLinkNodeParams) start the transfer in interrupt mode
  *         using function "HAL_MDMA_Start_IT". Note that the source and destination addresses given to
  *         the function HAL_MDMA_Start_IT must correspond to the node 0 source and destination addresses,
  *         same for the transfer data length.
  *         Note that using the MDMA the transfer data length is always expressed in bytes whatever
  *         is the source and data size (byte, half word, word or double word)
  *
  * @retval None
  */
static void MDMA_Config(void)
{
  MDMA_LinkNodeConfTypeDef mdmaLinkNodeConfig;

  /*##-1- Enable the MDMA clock ###############################################*/
  __HAL_RCC_MDMA_CLK_ENABLE();


  /*##-2- Select the MDMA instance to be used for the transfer : MDMA_Channel0 #*/
  MDMA_Handle.Instance = MDMA_INSTANCE;

  /*##-3- Initialize the MDMA channel for Node 0###############################*/

  /* Set the parameters to be configured for transfer Node0*/
  MDMA_Handle.Init.Request              = MDMA_REQUEST_SW;
  MDMA_Handle.Init.TransferTriggerMode  = MDMA_FULL_TRANSFER;
  MDMA_Handle.Init.Priority             = MDMA_PRIORITY_HIGH;
  MDMA_Handle.Init.Endianness           = MDMA_LITTLE_ENDIANNESS_PRESERVE;
  MDMA_Handle.Init.SourceInc            = MDMA_SRC_INC_WORD;
  MDMA_Handle.Init.DestinationInc       = MDMA_DEST_INC_BYTE;
  MDMA_Handle.Init.SourceDataSize       = MDMA_SRC_DATASIZE_WORD;
  MDMA_Handle.Init.DestDataSize         = MDMA_DEST_DATASIZE_BYTE;
  MDMA_Handle.Init.DataAlignment        = MDMA_DATAALIGN_PACKENABLE;
  MDMA_Handle.Init.SourceBurst          = MDMA_SOURCE_BURST_SINGLE;
  MDMA_Handle.Init.DestBurst            = MDMA_DEST_BURST_SINGLE;
  MDMA_Handle.Init.BufferTransferLength = 4;
  MDMA_Handle.Init.SourceBlockAddressOffset  = 0;
  MDMA_Handle.Init.DestBlockAddressOffset    = 0;


  if (HAL_MDMA_Init(&MDMA_Handle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /*##-4 Add linkedklist node 1 and  2##########################################*/
  /* Set the parameters to be configured for transfer Node1*/
  mdmaLinkNodeConfig.Init.Request              = MDMA_REQUEST_SW;
  mdmaLinkNodeConfig.Init.TransferTriggerMode  = MDMA_FULL_TRANSFER;
  mdmaLinkNodeConfig.Init.Priority             = MDMA_PRIORITY_HIGH;
  mdmaLinkNodeConfig.Init.Endianness           = MDMA_LITTLE_ENDIANNESS_PRESERVE;
  mdmaLinkNodeConfig.Init.SourceInc            = MDMA_SRC_INC_BYTE;
  mdmaLinkNodeConfig.Init.DestinationInc       = MDMA_DEST_INC_BYTE;
  mdmaLinkNodeConfig.Init.SourceDataSize       = MDMA_SRC_DATASIZE_BYTE;
  mdmaLinkNodeConfig.Init.DestDataSize         = MDMA_DEST_DATASIZE_BYTE;
  mdmaLinkNodeConfig.Init.DataAlignment        = MDMA_DATAALIGN_PACKENABLE;
  mdmaLinkNodeConfig.Init.SourceBurst          = MDMA_SOURCE_BURST_SINGLE;
  mdmaLinkNodeConfig.Init.DestBurst            = MDMA_DEST_BURST_SINGLE;
  mdmaLinkNodeConfig.Init.BufferTransferLength = 2;
  mdmaLinkNodeConfig.Init.SourceBlockAddressOffset  = 0;
  mdmaLinkNodeConfig.Init.DestBlockAddressOffset    = 0;

  mdmaLinkNodeConfig.SrcAddress      = (uint32_t)SRC_Const_Buffer1;
  mdmaLinkNodeConfig.DstAddress      = (uint32_t)DESTBuffer1_D2_SRAM1;
  mdmaLinkNodeConfig.BlockDataLength = (BUFFER_SIZE1*4);
  mdmaLinkNodeConfig.BlockCount      = 1;

  HAL_MDMA_LinkedList_CreateNode(&Xfer_Node1, &mdmaLinkNodeConfig);

  /* Set the parameters to be configured for transfer Node2*/
  mdmaLinkNodeConfig.Init.Request              = MDMA_REQUEST_SW;
  mdmaLinkNodeConfig.Init.TransferTriggerMode  = MDMA_FULL_TRANSFER;
  mdmaLinkNodeConfig.Init.Priority             = MDMA_PRIORITY_HIGH;
  mdmaLinkNodeConfig.Init.Endianness           = MDMA_LITTLE_ENDIANNESS_PRESERVE;
  mdmaLinkNodeConfig.Init.SourceInc            = MDMA_SRC_INC_BYTE;
  mdmaLinkNodeConfig.Init.DestinationInc       = MDMA_DEST_INC_WORD;
  mdmaLinkNodeConfig.Init.SourceDataSize       = MDMA_SRC_DATASIZE_BYTE;
  mdmaLinkNodeConfig.Init.DestDataSize         = MDMA_DEST_DATASIZE_WORD;
  mdmaLinkNodeConfig.Init.DataAlignment        = MDMA_DATAALIGN_PACKENABLE;
  mdmaLinkNodeConfig.Init.SourceBurst          = MDMA_SOURCE_BURST_SINGLE;
  mdmaLinkNodeConfig.Init.DestBurst            = MDMA_DEST_BURST_SINGLE;
  mdmaLinkNodeConfig.Init.BufferTransferLength = 4;
  mdmaLinkNodeConfig.Init.SourceBlockAddressOffset  = 0;
  mdmaLinkNodeConfig.Init.DestBlockAddressOffset    = 0;

  mdmaLinkNodeConfig.SrcAddress      = (uint32_t)SRC_Const_Buffer2;
  mdmaLinkNodeConfig.DstAddress      = (uint32_t)DESTBuffer2_D3_AHBSRAM;
  mdmaLinkNodeConfig.BlockDataLength = (BUFFER_SIZE2*4);
  mdmaLinkNodeConfig.BlockCount      = 1;

  HAL_MDMA_LinkedList_CreateNode(&Xfer_Node2, &mdmaLinkNodeConfig);


  HAL_MDMA_LinkedList_AddNode(&MDMA_Handle, &Xfer_Node1, 0);
  HAL_MDMA_LinkedList_AddNode(&MDMA_Handle, &Xfer_Node2, 0);

  /*##-5- Select Callbacks functions called after Transfer complete and Transfer error */
  HAL_MDMA_RegisterCallback(&MDMA_Handle, HAL_MDMA_XFER_CPLT_CB_ID, MDMA_TransferCompleteCallback);
  HAL_MDMA_RegisterCallback(&MDMA_Handle, HAL_MDMA_XFER_ERROR_CB_ID, MDMA_TransferErrorCallback);

  /*##-6- Configure NVIC for MDMA transfer complete/error interrupts ##########*/
  /* Set Interrupt Group Priority */
  HAL_NVIC_SetPriority(MDMA_IRQn, 0, 0);

  /* Enable the MDMA channel global Interrupt */
  HAL_NVIC_EnableIRQ(MDMA_IRQn);

  /*
    As the MDMA Nodes descriptor are located in the SRAM which
    is cacheable, it is necessary to clean the data cache after creating the node
    in order to make sure that the MDMA will load up to date data from the linked list node
  */
  SCB_CleanDCache_by_Addr( (uint32_t*)&Xfer_Node1, sizeof(MDMA_LinkNodeTypeDef));
  SCB_CleanDCache_by_Addr( (uint32_t*)&Xfer_Node2, sizeof(MDMA_LinkNodeTypeDef));

  /*##-7- Start the MDMA transfer using the interrupt mode ####################*/
  /* Configure the source, destination and buffer size MDMA fields and Start MDMA channel transfer of the Node 0 */
  if (HAL_MDMA_Start_IT(&MDMA_Handle, (uint32_t)&SRC_Const_Buffer0, (uint32_t)&DESTBuffer0_D1_AXISRAM, (BUFFER_SIZE0 * 4), 1) != HAL_OK)
  {
    /* Transfer Error */
    Error_Handler();
  }
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
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval 0  : pBuffer1 identical to pBuffer2
  *         >0 : pBuffer1 differs from pBuffer2
  */
static uint16_t Buffercmp(uint8_t *pBuffer1, uint8_t *pBuffer2, uint16_t BufferLength)
{
  /* Invalidate Data cache to get the updated SRAM content */
  SCB_InvalidateDCache_by_Addr((uint32_t *)pBuffer2,BufferLength);

  while (BufferLength--)
  {
    if ((*pBuffer1) != *pBuffer2)
    {
      return BufferLength;
    }
    pBuffer1++;
    pBuffer2++;
  }

  return 0;
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

  /* Enable D2 domain SRAM1 Clock (0x10000000 AXI)*/
  __HAL_RCC_D2SRAM1_CLK_ENABLE();

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

