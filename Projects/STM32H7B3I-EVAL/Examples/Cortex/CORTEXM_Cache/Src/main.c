/**
  ******************************************************************************
  * @file    Cortex/CORTEXM_Cache/Src/main.c
  * @author  MCD Application Team
  * @brief   This example provides a description of how to do Data-Cache
  *          maintenance on a shared memory buffer accessed by 2 masters
  *          (CPU and DMA).
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

/** @addtogroup CORTEXM_Cache
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* DMA Handle declaration */
DMA_HandleTypeDef     DmaHandle;

static const uint32_t aSRC_Const_Buffer1[BUFFER_SIZE] =
{
  0x01020304, 0x05060708, 0x090A0B0C, 0x0D0E0F10,  0x11121314, 0x15161718, 0x191A1B1C, 0x1D1E1F20,
  0x21222324, 0x25262728, 0x292A2B2C, 0x2D2E2F30,  0x31323334, 0x35363738, 0x393A3B3C, 0x3D3E3F40,
  0x41424344, 0x45464748, 0x494A4B4C, 0x4D4E4F50,  0x51525354, 0x55565758, 0x595A5B5C, 0x5D5E5F60,
  0x61626364, 0x65666768, 0x696A6B6C, 0x6D6E6F70,  0x71727374, 0x75767778, 0x797A7B7C, 0x7D7E7F80,
  0x01020304, 0x05060708, 0x090A0B0C, 0x0D0E0F10,  0x11121314, 0x15161718, 0x191A1B1C, 0x1D1E1F20,
  0x21222324, 0x25262728, 0x292A2B2C, 0x2D2E2F30,  0x31323334, 0x35363738, 0x393A3B3C, 0x3D3E3F40,
  0x41424344, 0x45464748, 0x494A4B4C, 0x4D4E4F50,  0x51525354, 0x55565758, 0x595A5B5C, 0x5D5E5F60,
  0x61626364, 0x65666768, 0x696A6B6C, 0x6D6E6F70,  0x71727374, 0x75767778, 0x797A7B7C, 0x7D7E7F80,
  0x01020304, 0x05060708, 0x090A0B0C, 0x0D0E0F10,  0x11121314, 0x15161718, 0x191A1B1C, 0x1D1E1F20,
  0x21222324, 0x25262728, 0x292A2B2C, 0x2D2E2F30,  0x31323334, 0x35363738, 0x393A3B3C, 0x3D3E3F40,
  0x41424344, 0x45464748, 0x494A4B4C, 0x4D4E4F50,  0x51525354, 0x55565758, 0x595A5B5C, 0x5D5E5F60,
  0x61626364, 0x65666768, 0x696A6B6C, 0x6D6E6F70,  0x71727374, 0x75767778, 0x797A7B7C, 0x7D7E7F80,
  0x01020304, 0x05060708, 0x090A0B0C, 0x0D0E0F10,  0x11121314, 0x15161718, 0x191A1B1C, 0x1D1E1F20,
  0x21222324, 0x25262728, 0x292A2B2C, 0x2D2E2F30,  0x31323334, 0x35363738, 0x393A3B3C, 0x3D3E3F40,
  0x41424344, 0x45464748, 0x494A4B4C, 0x4D4E4F50,  0x51525354, 0x55565758, 0x595A5B5C, 0x5D5E5F60,
  0x61626364, 0x65666768, 0x696A6B6C, 0x6D6E6F70,  0x71727374, 0x75767778, 0x797A7B7C, 0x7D7E7F80,
  0x01020304, 0x05060708, 0x090A0B0C, 0x0D0E0F10,  0x11121314, 0x15161718, 0x191A1B1C, 0x1D1E1F20,
  0x21222324, 0x25262728, 0x292A2B2C, 0x2D2E2F30,  0x31323334, 0x35363738, 0x393A3B3C, 0x3D3E3F40,
  0x41424344, 0x45464748, 0x494A4B4C, 0x4D4E4F50,  0x51525354, 0x55565758, 0x595A5B5C, 0x5D5E5F60,
  0x61626364, 0x65666768, 0x696A6B6C, 0x6D6E6F70,  0x71727374, 0x75767778, 0x797A7B7C, 0x7D7E7F80
};

#if defined ( __ICCARM__ )
/* Use ALIGN_32BYTES() to align the destination buffer 1 on 32-byte boundary */
ALIGN_32BYTES(static uint32_t aDST_Buffer1[BUFFER_SIZE] = {0});

/* Use ALIGN_32BYTES() to align the source buffer 2 on 32-byte boundary */
ALIGN_32BYTES(static uint32_t aSRC_Buffer2[BUFFER_SIZE]);

/* Allocate Destination Buffer 2 in non-cacheable memory (Internal SRD SRAM) */
#pragma location = 0x38000000
static uint32_t aDST_NonCacheable_Buffer2[BUFFER_SIZE];

static __IO uint32_t transferErrorDetected = 0;
static __IO uint32_t transferCompleteDetected = 0;
#elif defined ( __CC_ARM )
/* Use ALIGN_32BYTES() to align the destination buffer 1 on 32-byte boundary */
ALIGN_32BYTES(static uint32_t aDST_Buffer1[BUFFER_SIZE])={0};
/* Use ALIGN_32BYTES() to align the source buffer 2 on 32-byte boundary */
ALIGN_32BYTES(static uint32_t aSRC_Buffer2[BUFFER_SIZE]);
/* Allocate Destination Buffer 2 in non-cacheable memory (RAM_D3) */
static uint32_t aDST_NonCacheable_Buffer2[BUFFER_SIZE] __attribute__((section (".RAM_D3"))) ;
static __IO uint32_t transferErrorDetected __attribute__((section (".RAM_D3")))= 0;
static __IO uint32_t transferCompleteDetected __attribute__((section (".RAM_D3")))= 0;
#elif defined ( __GNUC__ )
/* Use ALIGN_32BYTES() to align the destination buffer 1 on 32-byte boundary */
static uint32_t aDST_Buffer1[BUFFER_SIZE]__attribute__ ((aligned (32)))={0};
/* Use ALIGN_32BYTES() to align the source buffer 2 on 32-byte boundary */
static uint32_t aSRC_Buffer2[BUFFER_SIZE]__attribute__ ((aligned (32)));
/* Allocate Destination Buffer 2 in non-cacheable memory (RAM_D3) */
static uint32_t aDST_NonCacheable_Buffer2[BUFFER_SIZE] __attribute__((section (".RAM_D3"))) ;
static __IO uint32_t transferErrorDetected __attribute__((section (".RAM_D3")))= 0;
static __IO uint32_t transferCompleteDetected __attribute__((section (".RAM_D3")))= 0;
#endif

static uint32_t tempValue;
static uint32_t i;

/* Private function prototypes -----------------------------------------------*/
static void DMA_Config(void);
static uint32_t DMA_StartAndCompare(uint32_t* SrcBuffer, uint32_t* DstBuffer, uint32_t BufferLength);
static void TransferComplete(DMA_HandleTypeDef *DmaHandle);
static void TransferError(DMA_HandleTypeDef *DmaHandle);
static void SystemClock_Config(void);
static void Error_Handler(void);
static void CPU_CACHE_Enable(void);

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

  /* Configure the MPU attributes for AXI SRAM */
#if defined(MPU_WRITE_THROUGH)
  MPU_Config_WT();
#elif defined(MPU_WRITE_BACK_WRITE_ALLOCATE)
  MPU_Config_WB_WA();
#elif defined(MPU_WRITE_BACK_NO_WRITE_ALLOCATE)
  MPU_Config_WB_nWA();
#endif

  /* Configure the internal SRD SRAM as non-cacheable memory */
  MPU_Config_nCacheable();

  /* STM32H7xx HAL library initialization:
       - Configure the Systick to generate an interrupt each 1 msec
       - Set NVIC Group Priority to 4
       - Low Level Initialization
     */
  HAL_Init();

  /* Configure the system clock to 280 MHz */
  SystemClock_Config();

  /* Initialize LEDs */
  BSP_LED_Init(LED_GREEN);
  BSP_LED_Init(LED_ORANGE);
  BSP_LED_Init(LED_RED);
  BSP_LED_Init(LED_YELLOW);

  /* Configure the Tamper push-button in GPIO Mode */
  BSP_PB_Init(BUTTON_TAMPER, BUTTON_MODE_GPIO);

  /* Configure the DMA stream for Memory to Memory transfer */
  DMA_Config();

  /*## -1- DMA Transfer 1: aSRC_Const_Buffer1 (FLASH) --> aDST_Buffer1 (AXI-SRAM) #######*/
  /*#####################################################################################*/

  /* Read the Destination Buffer to ensure that it will be put in DCACHE */
  for (i=0; i<BUFFER_SIZE; i++)
  {
    tempValue += aDST_Buffer1[i];
  }

  /* Start the DMA transfer, and compare source and destination buffers */
  if (DMA_StartAndCompare((uint32_t *)aSRC_Const_Buffer1, aDST_Buffer1, BUFFER_SIZE) != 0)
  {
    /* Wrong behaviour: Turn on Yellow LED */
    BSP_LED_On(LED_YELLOW);
  }
  else
  {
    /* Correct behaviour: Turn on Green LED */
    BSP_LED_On(LED_GREEN);
  }

  /*## -2- DMA Transfer 2: aSRC_Buffer2 (AXI-SRAM) --> aDST_NonCacheable_Buffer2 (SRD SRAM) */
  /*#####################################################################################*/

  /* Fill the Source Buffer: it will be compared with the destination buffer */
  for (i=0; i<BUFFER_SIZE; i++)
  {
    aSRC_Buffer2[i] = i;
  }

  /* Start the DMA transfer, and compare source and destination buffers */
  if (DMA_StartAndCompare(aSRC_Buffer2, aDST_NonCacheable_Buffer2, BUFFER_SIZE) != 0)
  {
    /* Wrong behaviour: Turn on Red LED */
    BSP_LED_On(LED_RED);
  }
  else
  {
    /* Correct behaviour: Turn on Orange LED  */
    BSP_LED_On(LED_ORANGE);
  }

  /*## -3- Wait for Tamper Key press / release  #########################################*/
  /*#####################################################################################*/

  /* Wait for Tamper Key press before starting the Cache maintenance */
  while (BSP_PB_GetState(BUTTON_TAMPER) == RESET)
  {
  }

  /* Wait for Tamper Key to be released before starting the Cache maintenance */
  while (BSP_PB_GetState(BUTTON_TAMPER) == SET)
  {
  }

  /* Turn off leds */
  BSP_LED_Off(LED_GREEN);
  BSP_LED_Off(LED_ORANGE);
  BSP_LED_Off(LED_RED);
  BSP_LED_Off(LED_YELLOW);

  /*## -4- Do Cache maintenance for Transfer 1  #########################################*/
  /*#####################################################################################*/

  /*
  CPU Data Cache maintenance:
  It is recommended to invalidate the destination buffer in Data cache after a DMA transfer.
  This guarantees up-to-date data when CPU access to the destination buffer located in the
  AXI-SRAM (which is cacheable).
  */

  /* Invalidate aDST_Buffer1 in D-Cache: (BUFFER_SIZE * 4) bytes */
  SCB_InvalidateDCache_by_Addr(aDST_Buffer1, BUFFER_SIZE * 4);

  /* Compare Source and Destination buffers */
  if(memcmp(aSRC_Const_Buffer1, aDST_Buffer1, BUFFER_SIZE * 4) != 0)
  {
    /* Wrong Comparison: Turn on Red LED */
    BSP_LED_On(LED_RED);
  }
  else
  {
    /* Right comparison: Turn on Green LED */
    BSP_LED_On(LED_GREEN);
  }

  /*## -5- Do Cache maintenance for Transfer 2  #########################################*/
  /*#####################################################################################*/

  /*
  CPU Data Cache maintenance:
  It is recommended to clean the source buffer in CPU Data cache before starting
  the DMA transfer. As the source buffer may be used by the CPU and modified
  locally in DCACHE. The cache clean guarantees up-to-date data when DMA accesses
  to the source buffer located in the AXI-SRAM (which is cacheable).
  */

  /* Clean aSRC_Buffer2 in DCACHE: (BUFFER_SIZE * 4) bytes */
  SCB_CleanDCache_by_Addr(aSRC_Buffer2, BUFFER_SIZE * 4);

  /* Re-Start the DMA transfer, and compare source and destination buffers */
  if (DMA_StartAndCompare(aSRC_Buffer2, aDST_NonCacheable_Buffer2, BUFFER_SIZE) != 0)
  {
    /* Wrong behaviour: Turn on Yellow LED */
    BSP_LED_On(LED_YELLOW);
  }
  else
  {
    /* Correct behaviour: Turn on Orange LED */
    BSP_LED_On(LED_ORANGE);
  }

  /* Infinite loop */
  while (1)
  {
  }
}

/**
  * @brief  Configure the DMA controller
  * @note  This function is used to :
  *        -1- Enable DMA1 clock
  *        -2- Select the DMA functional Parameters
  *        -3- Select the DMA instance to be used for the transfer
  *        -4- Select Callbacks functions called after Transfer complete and
               Transfer error interrupt detection
  *        -5- Initialize the DMA stream
  *        -6- Configure NVIC for DMA transfer complete/error interrupts
  * @param  None
  * @retval None
  */
static void DMA_Config(void)
{
  /*## -1- Enable DMA1 clock #################################################*/
  __HAL_RCC_DMA1_CLK_ENABLE();

  /*##-2- Select the DMA functional Parameters ###############################*/
  DmaHandle.Init.Request = DMA_REQUEST;                     /* Request is memory to memory      */
  DmaHandle.Init.Direction = DMA_MEMORY_TO_MEMORY;          /* M2M transfer mode                */
  DmaHandle.Init.PeriphInc = DMA_PINC_ENABLE;               /* Peripheral increment mode Enable */
  DmaHandle.Init.MemInc = DMA_MINC_ENABLE;                  /* Memory increment mode Enable     */
  DmaHandle.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD; /* Peripheral data alignment : Word */
  DmaHandle.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;    /* memory data alignment : Word     */
  DmaHandle.Init.Mode = DMA_NORMAL;                         /* Normal DMA mode                  */
  DmaHandle.Init.Priority = DMA_PRIORITY_VERY_HIGH;         /* priority level : very high       */
  DmaHandle.Init.FIFOMode = DMA_FIFOMODE_ENABLE;            /* FIFO mode enabled                */
  DmaHandle.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_1QUARTERFULL; /* FIFO threshold: 1/4 full   */
  DmaHandle.Init.MemBurst = DMA_MBURST_SINGLE;              /* Memory burst                     */
  DmaHandle.Init.PeriphBurst = DMA_PBURST_SINGLE;           /* Peripheral burst                 */

  /*##-3- Select the DMA instance to be used for the transfer : DMA1_Stream0 #*/
  DmaHandle.Instance = DMA_INSTANCE;

  /*##-4- Initialize the DMA stream ##########################################*/
  if (HAL_DMA_Init(&DmaHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /*##-5- Select Callbacks functions called after Transfer complete and Transfer error */
  HAL_DMA_RegisterCallback(&DmaHandle, HAL_DMA_XFER_CPLT_CB_ID, TransferComplete);
  HAL_DMA_RegisterCallback(&DmaHandle, HAL_DMA_XFER_ERROR_CB_ID, TransferError);

  /*##-6- Configure NVIC for DMA transfer complete/error interrupts ##########*/
  /* Set Interrupt Group Priority */
  HAL_NVIC_SetPriority(DMA_INSTANCE_IRQ, 0, 0);

  /* Enable the DMA STREAM global Interrupt */
  HAL_NVIC_EnableIRQ(DMA_INSTANCE_IRQ);
}

/**
  * @brief  Start the DMA Transfer and compares source and destination buffers
  * @note  This function is used to :
  *        -1- Reset transfer variables
  *        -2- Start the DMA transfer using the interrupt mode
  *        -3- wait until DMA transfer complete or transfer error
  *        -4- Turn LED_ORANGE on in case of DMA transfer error
  *        -5- Compare Source and Destination buffers
  * @param  SrcBuffer: The source memory Buffer address
  * @param  DstBuffer: The destination memory Buffer address
  * @param  BuuferLength: The length of data to be transferred from source to destination
  * @retval Status (0 or 1)
  */
static uint32_t DMA_StartAndCompare(uint32_t* SrcBuffer, uint32_t* DstBuffer, uint32_t BufferLength)
{
  /* Reset transfer variables */
  transferCompleteDetected = 0;
  transferErrorDetected    = 0;

  /* Start the DMA transfer */
  if (HAL_DMA_Start_IT(&DmaHandle, (uint32_t)SrcBuffer, (uint32_t)DstBuffer, BufferLength) != HAL_OK)
  {
    /* Transfer Error */
    Error_Handler();
  }

  while ((transferCompleteDetected == 0) && (transferErrorDetected == 0))
  {
    /* wait until DMA transfer complete or transfer error */
  }

  if (transferErrorDetected == 1)
  {
    /* DMA transfer error */
    return 1;
  }

  /* Compare Source and Destination buffers */
  if(memcmp(SrcBuffer, DstBuffer, BufferLength * 4) != 0)
  {
    /* Wrong comparison */
    return 1;
  }
  else
  {
    /* Right comparison */
    return 0;
  }
}

/**
  * @brief  DMA conversion complete callback
  * @note   This function is executed when the transfer complete interrupt
  *         is generated
  * @retval None
  */
static void TransferComplete(DMA_HandleTypeDef *DmaHandle)
{
  transferCompleteDetected = 1;
}

/**
  * @brief  DMA conversion error callback
  * @note   This function is executed when the transfer error interrupt
  *         is generated during DMA transfer
  * @retval None
  */
static void TransferError(DMA_HandleTypeDef *DmaHandle)
{
  transferErrorDetected = 1;
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
    while(1);
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
     while(1);
  }

  /*
    Note : The activation of the I/O Compensation Cell is recommended with communication  interfaces
          (GPIO, SPI, FMC, QSPI ...)  when  operating at  high frequencies(please refer to product datasheet)
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
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* User may add here some code to deal with this error */
  while(1)
  {
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

