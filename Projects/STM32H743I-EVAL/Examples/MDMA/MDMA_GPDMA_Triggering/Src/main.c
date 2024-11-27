/**
  ******************************************************************************
  * @file    MDMA/MDMA_GPDMA_Triggering/Src/main.c
  * @author  MCD Application Team
  * @brief   This sample code shows how to use MDMA HAL API to start a transfer
  *          using GP-DMA (DMA1 Stream5) complete flag as a trigger
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

/** @addtogroup STM32H7xx_HAL_Examples
  * @{
  */

/** @addtogroup MDMA_GPDMA_Triggering
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* UART handler declaration */
UART_HandleTypeDef UartHandle;

MDMA_HandleTypeDef     MDMA_Handle;

__IO uint32_t TransferErrorDetected = 0;
__IO uint32_t TransferCompleteDetected = 0;

/* Buffer used for transmission */
uint8_t aTxStartMessage[] = "\n\r ****MDMA transfer example using GP-DMA as HW trigger****\n\r Please Enter 12 characters using keyboard :\n\r";
uint8_t aTxEndMessage[] = "\n\r Example Finished\n\r";

/* Buffer used for UART reception */
uint8_t aUART_RxBuffer[RXBUFFERSIZE];

/* Buffer used for MDMA destination */
#if defined ( __ICCARM__ )
#pragma location = 0x20000000
uint8_t aMDMA_RxBuffer[RXBUFFERSIZE];
#elif defined ( __CC_ARM )
__attribute__((section (".DTCMRAM"))) uint8_t aMDMA_RxBuffer[RXBUFFERSIZE];
#elif defined ( __GNUC__ )
uint8_t __attribute__((section (".DTCMRAM"))) aMDMA_RxBuffer[RXBUFFERSIZE];
#endif


/* Private function prototypes -----------------------------------------------*/
static void MPU_Config(void);
static void SystemClock_Config(void);
static void UART_Config(void);
static void CPU_CACHE_Enable(void);
static void Error_Handler(void);
static void MDMA_TransferCompleteCallback(MDMA_HandleTypeDef *hmdma);
static void MDMA_TransferErrorCallback(MDMA_HandleTypeDef *hmdma);

/* Private functions ---------------------------------------------------------*/


/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  uint32_t hal_status = HAL_OK;

  /* Configure the MPU attributes */
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
  
  /* Configure leds */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED3);
  
  /* Configure the UART and send start message to the user */
  UART_Config();
  
  /*##-1- Enable the MDMA clock ###############################################*/  
  __HAL_RCC_MDMA_CLK_ENABLE();  
  
  /*##-2- Select the MDMA instance to be used : MDMA_Channel0 #################*/
  MDMA_Handle.Instance = MDMA_INSTANCE;  
  
  /*##-3- Configure the MDMA for block transfer in HW request mode ############*/  
  /* MDMA HW request is set to DMA1 stream5 Transfer Complete Flag 
     i.e The MDMA transfer will be triggered by the DMA1 Stream5 Transfer complete flag */
  MDMA_Handle.Init.Request              = MDMA_HW_REQUEST_TRIGGER;
  MDMA_Handle.Init.TransferTriggerMode  = MDMA_BLOCK_TRANSFER;  
  MDMA_Handle.Init.Priority             = MDMA_PRIORITY_HIGH;
  MDMA_Handle.Init.Endianness           = MDMA_LITTLE_ENDIANNESS_PRESERVE;

  MDMA_Handle.Init.SourceInc            = MDMA_SRC_INC_BYTE;
  MDMA_Handle.Init.DestinationInc       = MDMA_DEST_INC_BYTE;
  MDMA_Handle.Init.SourceDataSize       = MDMA_SRC_DATASIZE_BYTE;
  MDMA_Handle.Init.DestDataSize         = MDMA_DEST_DATASIZE_BYTE;
  MDMA_Handle.Init.DataAlignment        = MDMA_DATAALIGN_PACKENABLE;                            
  MDMA_Handle.Init.SourceBurst          = MDMA_SOURCE_BURST_SINGLE;
  MDMA_Handle.Init.DestBurst            = MDMA_DEST_BURST_SINGLE;
  MDMA_Handle.Init.BufferTransferLength = 128;

  MDMA_Handle.Init.SourceBlockAddressOffset  = 0;
  MDMA_Handle.Init.DestBlockAddressOffset    = 0;

  /*##-4- Initialize the MDMA channel ##########################################*/  
  hal_status = HAL_MDMA_Init(&MDMA_Handle);
  
  if(hal_status != HAL_OK)  
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  /*##-5- Select Callbacks functions called after MDMA Transfer complete and Transfer error */
  HAL_MDMA_RegisterCallback(&MDMA_Handle, HAL_MDMA_XFER_CPLT_CB_ID, MDMA_TransferCompleteCallback);
  HAL_MDMA_RegisterCallback(&MDMA_Handle, HAL_MDMA_XFER_ERROR_CB_ID, MDMA_TransferErrorCallback);
  
  /*##-6- Configure NVIC for MDMA transfer complete/error interrupts ##########*/
  /* Set Interrupt Group Priority */
  HAL_NVIC_SetPriority(MDMA_IRQn, 0, 0);
  
  /* Enable the MDMA channel global Interrupt */
  HAL_NVIC_EnableIRQ(MDMA_IRQn);  
  
  /*##-7- Start the MDMA transfer process #####################################*/ 
  /* MDMA will transfer content of  "aUART_RxBuffer" buffer to "aMDMA_RxBuffer" buffer 
     knowing that MDMA HW trigger is DMA1 stream 5 which is used for UART reception.
     as consequence MDMA transfer will be triggered once the UART received user message to 
     "aUART_RxBuffer" buffer and the MDMA will transfer it to the "aMDMA_RxBuffer" buffer
  */  
  hal_status = HAL_MDMA_Start_IT(&MDMA_Handle, (uint32_t)&aUART_RxBuffer, 
                                               (uint32_t)&aMDMA_RxBuffer, 
                                               RXBUFFERSIZE, 
                                               1);
  
  if(hal_status != HAL_OK)   
  {
    /* Transfer Error */
    Error_Handler();
  }
  
  /*##-8- Start UART peripheral reception process using DMA1 stream5 #########*/
  /* Any data received will be stored in "aUART_RxBuffer" buffer : the number max of
     data received is 12 */
  if (HAL_UART_Receive_DMA(&UartHandle, (uint8_t *)aUART_RxBuffer, RXBUFFERSIZE) != HAL_OK)
  {
    /* Transfer error in reception process */
    Error_Handler();
  }
  
  /*##-9- Wait for the end of the MDMA transfer or error #####################*/
  /* Note that when MDMA transfer ends it means that the user message has been transferred  
    using UART through the DMA1 stream 5 to "aUART_RxBuffer" buffer  and the MDMA has transferred 
    aUART_RxBuffer to the final destination "aMDMA_RxBuffer" buffer*/
  while((TransferCompleteDetected == 0) && (TransferErrorDetected == 0))
  {
  }

  /* If No error detected during MDMA transfer */
  if(TransferErrorDetected == 0)
  {
    /*##-10- Send the received MDMA Buffer "aMDMA_RxBuffer" back to the user ####*/
    /* Using UART in polling model as the DMA1 has no access to the DTCM RAM where the
    buffer "aMDMA_RxBuffer" is located */
    if (HAL_UART_Transmit(&UartHandle, (uint8_t*)aMDMA_RxBuffer, RXBUFFERSIZE, HAL_MAX_DELAY) != HAL_OK)
    {
      /* Transfer error in transmission process */
      Error_Handler();
    }
    
    /*##-11- Send the End Message ###############################################*/  
    if(HAL_UART_Transmit_DMA(&UartHandle, (uint8_t*)aTxEndMessage, TXENDMESSAGESIZE)!= HAL_OK)
    {
      /* Transfer error in transmission process */
      Error_Handler();
    }
    
    /*##-12- Wait for the end of the transfer ###################################*/  
    while (HAL_UART_GetState(&UartHandle) != HAL_UART_STATE_READY)
    {
    }
  }

  /* Infinite loop */  
  while (1)
  {
  }
}

/**
  * @brief  Configure and initializes the UART and send start message to the user
  *         PC HyperTerminal.
  * @param  None
  * @retval None
  */
static void UART_Config(void)
{
  /*##-1- Configure the UART peripheral ######################################*/
  /* Put the USART peripheral in the Asynchronous mode (UART Mode) */
  /* UART configured as follows:
      - Word Length = 8 Bits (7 data bit + 1 parity bit) : 
                      BE CAREFUL : Program 7 data bits + 1 parity bit in PC HyperTerminal
      - Stop Bit    = One Stop bit
      - Parity      = ODD parity
      - BaudRate    = 9600 baud
      - Hardware flow control disabled (RTS and CTS signals) */
  UartHandle.Instance        = USARTx;

  UartHandle.Init.BaudRate   = 9600;
  UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
  UartHandle.Init.StopBits   = UART_STOPBITS_1;
  UartHandle.Init.Parity     = UART_PARITY_ODD;
  UartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
  UartHandle.Init.Mode       = UART_MODE_TX_RX;
  UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;

  if(HAL_UART_Init(&UartHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /*##-2- Start the UART transmission process #####################################*/
  /* Send "TxBuffer" buffer through the UART to the user */
  if(HAL_UART_Transmit_DMA(&UartHandle, (uint8_t*)aTxStartMessage, TXSTARTMESSAGESIZE)!= HAL_OK)
  {
    /* Transfer error in transmission process */
    Error_Handler();
  } 

  /*##-3- Wait for the end of the transfer ###################################*/  
  while (HAL_UART_GetState(&UartHandle) != HAL_UART_STATE_READY)
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
  * @brief  UART error callbacks
  * @param  huart: UART handle
  * @note   This example shows a simple way to report transfer error, and you can
  *         add your own implementation.
  * @retval None
  */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
  /* Turn LED3 on: Transfer error in reception/transmission process */
  BSP_LED_On(LED3);
}

/**
  * @brief  MDMA conversion complete callback
  * @note   This function is executed when the transfer complete interrupt
  *         is generated
  * @retval None
  */
static void MDMA_TransferCompleteCallback(MDMA_HandleTypeDef *hmdma)
{
  /* Turn LED1 on: MDMA Transfer is correct */
  BSP_LED_On(LED1);
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

  /* Turn LED3 on: MDMA Transfer error */
  BSP_LED_On(LED3);  
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

