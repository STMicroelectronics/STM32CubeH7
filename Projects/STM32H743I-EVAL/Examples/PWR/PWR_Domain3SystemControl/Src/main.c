/**
  ******************************************************************************
  * @file    PWR/PWR_Domain3SystemControl/Src/main.c
  * @author  MCD Application Team
  * @brief   This example shows how to maintain system control using
  *          D3 Domain only.
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

/** @addtogroup PWR_Domain3SystemControl
  * @{
  */

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define LPUART1_RX_WAKEUP_EXTI_LINE   EXTI_LINE34
#define LPUART1_GLOBAL_IT_EXTI_LINE   EXTI_LINE64
#define LPUART1_RX_WAKEUP_EXTI_MODE   (EXTI_MODE_IT | EXTI_MODE_EVT)
#define BUFFER_SIZE                   20U
#define TX_CHUNK_SIZE                 4U
#define DMAMUX_REQUEST_NUMBER         4U
#define DMA_TRANSFER_NUMBER           1U
#define DMA_TRANSFER_NO_ERROR         0U
#define LPUART_TIME_OUT               0xFFFU
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
__IO  uint32_t DMA_TransferErrorFlag    = 0;
__IO  uint32_t DMA_TransferCompleteFlag = 0;

#if defined ( __CC_ARM )  /* !< ARM Compiler */
  uint8_t aRxBuffer[BUFFER_SIZE] __attribute__((section(".RAM_D3")));
#elif defined ( __ICCARM__ )  /* !< ICCARM Compiler */
  #pragma location=0x38000000
  uint8_t aRxBuffer[BUFFER_SIZE];
#elif defined ( __GNUC__ )  /* !< GNU Compiler */
  uint8_t aRxBuffer[BUFFER_SIZE] __attribute__((section(".RAM_D3")));
#endif

uint8_t aTxBuffer[BUFFER_SIZE] = {
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J'
};

DMA_HandleTypeDef BDMA_Handle7;
UART_HandleTypeDef Lpuart1Handle;

/* Private function prototypes -----------------------------------------------*/
static void MPU_Config(void);
static void SystemClock_Config(void);
static void LPUART_Config(void);

#ifdef LOW_POWER_BOARD
static void DMA_TransferErrorCallback(DMA_HandleTypeDef *hdma);
static void DMA_TransferCompleteCallback(DMA_HandleTypeDef *hdma);
#else
static uint8_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);
#endif /* LOW_POWER_BOARD */

static void CPU_CACHE_Enable(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{

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

  /* -1- Configure the system clock to 400 MHz */
  SystemClock_Config();
  HAL_RCC_MCOConfig( RCC_MCO2, RCC_MCO2SOURCE_SYSCLK, RCC_MCODIV_4);

  /* -2- Initialize LED1 and LED3 */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED3);

  /* -3- Configure the LPUART1 peripheral */
  LPUART_Config();

#ifdef LOW_POWER_BOARD

  HAL_DMA_MuxRequestGeneratorConfigTypeDef dmamux_ReqGenParams;
  HAL_DMA_MuxSyncConfigTypeDef dmamux_syncParams;

  /* Set LED1 On to inform the user that the system is start */
  BSP_LED_On(LED1);

  /* -4- Configure the BDMA as D3 domain penclear source */
  /* Enable BDMA clock */
  __HAL_RCC_BDMA_CLK_ENABLE();

  /* Configure the DMA channle7 handler for D3 penclear source */
  BDMA_Handle7.Instance                  = BDMA_Channel7;
  BDMA_Handle7.Init.Request              = DMA_REQUEST_GENERATOR0;
  BDMA_Handle7.Init.Direction            = DMA_PERIPH_TO_MEMORY;
  BDMA_Handle7.Init.PeriphInc            = DMA_PINC_DISABLE;
  BDMA_Handle7.Init.MemInc               = DMA_MINC_ENABLE;
  BDMA_Handle7.Init.PeriphDataAlignment  = DMA_PDATAALIGN_BYTE;
  BDMA_Handle7.Init.MemDataAlignment     = DMA_MDATAALIGN_BYTE;
  BDMA_Handle7.Init.Mode                 = DMA_NORMAL;
  BDMA_Handle7.Init.Priority             = DMA_PRIORITY_HIGH;
  BDMA_Handle7.Init.FIFOMode             = DMA_FIFOMODE_ENABLE;
  BDMA_Handle7.Init.FIFOThreshold        = DMA_FIFO_THRESHOLD_FULL;
  BDMA_Handle7.Init.MemBurst             = DMA_MBURST_SINGLE;
  BDMA_Handle7.Init.PeriphBurst          = DMA_PBURST_SINGLE;
  BDMA_Handle7.XferCpltCallback          = DMA_TransferCompleteCallback;
  BDMA_Handle7.XferErrorCallback         = DMA_TransferErrorCallback;

  if(HAL_DMA_Init(&BDMA_Handle7) != HAL_OK)
  {
    Error_Handler();
  }

  /* Select Callbacks functions called after Transfer complete and Transfer error */
  if(HAL_DMA_RegisterCallback(&BDMA_Handle7, HAL_DMA_XFER_CPLT_CB_ID, DMA_TransferCompleteCallback) != HAL_OK)
  {
    Error_Handler();
  }

  if(HAL_DMA_RegisterCallback(&BDMA_Handle7, HAL_DMA_XFER_ERROR_CB_ID, DMA_TransferErrorCallback)  != HAL_OK)
  {
    Error_Handler();
  }

  /* -5- Configure the DMAMUX synchronisation */
  /* Configure the DMAMUX with the Synchronization parameters for BDMA_Handle7 */
  dmamux_syncParams.EventEnable   = ENABLE;                    /* Enable DMAMUX event generation each time  RequestNumber are passed from DMAMUX to the DMA */
  dmamux_syncParams.SyncPolarity  = HAL_DMAMUX_SYNC_RISING;    /* Synchronization edge is Rising  */
  dmamux_syncParams.RequestNumber = DMAMUX_REQUEST_NUMBER;     /* 4 request are autorized after each edge of the sync signal */
  dmamux_syncParams.SyncSignalID  = HAL_DMAMUX2_SYNC_DMAMUX2_CH0_EVT; /* Sync signal is dmamux1_ch0_evt */
  dmamux_syncParams.SyncEnable    = DISABLE;                   /* Synchronization is disabled */

  if(HAL_DMAEx_ConfigMuxSync(&BDMA_Handle7, &dmamux_syncParams)  != HAL_OK)
  {
    Error_Handler();
  }

  /* NVIC configuration for BDMA_Handle7 transfer complete interrupt */
  HAL_NVIC_SetPriority(BDMA_Channel7_IRQn, 0, 1);
  HAL_NVIC_EnableIRQ(BDMA_Channel7_IRQn);

  /* -6- Configure the DMAMUX request generator */
  dmamux_ReqGenParams.SignalID  = HAL_DMAMUX2_REQ_GEN_LPUART1_RX_WKUP;  /* External request signal is HAL_DMAMUX2_REQUEST_GEN_SIGNAL_7 i.e Lpuart1_it_R_WUP_ASYNC signal */
  dmamux_ReqGenParams.Polarity  = HAL_DMAMUX_REQ_GEN_RISING;            /* External request signal edge is Rising  */
  dmamux_ReqGenParams.RequestNumber = DMAMUX_REQUEST_NUMBER;            /* 4 request on each edge of the external request signal  */

  if(HAL_DMAEx_ConfigMuxRequestGenerator(&BDMA_Handle7, &dmamux_ReqGenParams) != HAL_OK)
  {
    Error_Handler();
  }

  if(HAL_DMAEx_EnableMuxRequestGenerator (&BDMA_Handle7) != HAL_OK)
  {
    Error_Handler();
  }

  /* NVIC configuration for DMAMUX request generator overrun errors */
  HAL_NVIC_SetPriority(DMAMUX2_OVR_IRQn, 0, 1);
  HAL_NVIC_EnableIRQ(DMAMUX2_OVR_IRQn);

  /* -7- Activate autonomous bit for SRAM1, BDAM and LPUART1 peripherals */
  __HAL_RCC_LPUART1_CLKAM_ENABLE();
  __HAL_RCC_BDMA_CLKAM_ENABLE();
  __HAL_RCC_D3SRAM1_CLKAM_ENABLE();

  /* -8- Configure the LPUART EXTI lines */
  /* Deactivate all LPUART1 interrupts and events */
  HAL_EXTI_D1_EventInputConfig(LPUART1_RX_WAKEUP_EXTI_LINE, LPUART1_RX_WAKEUP_EXTI_MODE, DISABLE);
  HAL_EXTI_D1_EventInputConfig(LPUART1_GLOBAL_IT_EXTI_LINE, LPUART1_RX_WAKEUP_EXTI_MODE, DISABLE);

  /* Configure BDMA Channel7 event as D3 domain pendclear source  */
  HAL_EXTI_D3_EventInputConfig(LPUART1_RX_WAKEUP_EXTI_LINE, ENABLE, BDMA_CH7_CLEAR);

  /* Enable rising trigger for LPUART1 RX Event and Interrupt */
  HAL_EXTI_EdgeConfig(LPUART1_RX_WAKEUP_EXTI_LINE, EXTI_RISING_EDGE);

  /* -9- Insert 3 seconds delay before entering Stop mode */
  HAL_Delay(3000);

  while(1)
  {
    DMA_TransferCompleteFlag = 0;
    DMA_TransferErrorFlag    = 0;

    /* -10- Start the DMA Reception in interrupt mode */
    if(HAL_DMA_Start_IT(&BDMA_Handle7, (uint32_t)&(Lpuart1Handle.Instance->RDR),(uint32_t)&(aRxBuffer), BUFFER_SIZE)  != HAL_OK)
    {
      Error_Handler();
    }

    /* -11- Set LED1 Off and enter Stop mode */
    BSP_LED_Off(LED1);
    HAL_PWR_EnterSTOPMode(PWR_MAINREGULATOR_ON, PWR_STOPENTRY_WFI);

    /* -12- Check the results after wake-up */
    /* The transfer number must be equal to one because we used one DMA.
       If not the case then goto  Error_Handler (LED3 will turn on) */
    if(DMA_TransferCompleteFlag != DMA_TRANSFER_NUMBER)
    {
      Error_Handler();
    }

    /* If a DMA transfer ERROR occurred then goto Error_Handler (LED3 will turn on) */
    if(DMA_TransferErrorFlag != 0)
    {
      Error_Handler();
    }

    /* -13- Send back the received data when the system wake-up without errors */
    if(HAL_UART_Transmit(&Lpuart1Handle, (uint8_t*)&aRxBuffer, BUFFER_SIZE, LPUART_TIME_OUT) != HAL_OK)
    {
      Error_Handler();
    }
  }

#else

  uint32_t bufferIndex = 0;

  /* -4- Toggle led1 then waiting for user to press button */
  BSP_LED_On(LED1);

  /* -5- Configure Tamper Key */
  BSP_PB_Init(BUTTON_TAMPER, BUTTON_MODE_GPIO);

  /* -6- Wait for Tamper Key press before starting the Communication */
  while (BSP_PB_GetState(BUTTON_TAMPER) == RESET)
  {
  }

  /* -7- Wait for Tamper Key to be release before starting the Communication */
  while (BSP_PB_GetState(BUTTON_TAMPER) == SET)
  {
  }

  /* The board sends the message and expects to receive it back */
  while(1)
  {
    /* -8- Transmit the aTxBuffer data */
    if(HAL_UART_Transmit(&Lpuart1Handle, (uint8_t*)(aTxBuffer + bufferIndex), TX_CHUNK_SIZE, LPUART_TIME_OUT) != HAL_OK)
    {
      Error_Handler();
    }

    bufferIndex += TX_CHUNK_SIZE;

    if(bufferIndex >= BUFFER_SIZE)
    {
      /* -9- Receive the LPUART data */
      if(HAL_UART_Receive(&Lpuart1Handle, (uint8_t*)&aRxBuffer, BUFFER_SIZE, LPUART_TIME_OUT) != HAL_OK)
      {
        Error_Handler();
      }

      /* -10- Check the received data */
      if(Buffercmp(aRxBuffer, aTxBuffer, BUFFER_SIZE))
      {
        Error_Handler();
      }

      bufferIndex = 0;
      HAL_Delay(100);
      BSP_LED_Toggle(LED1);
    }
  }

#endif  /* LOW_POWER_BOARD */
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

  while ((PWR->D3CR & (PWR_D3CR_VOSRDY)) != PWR_D3CR_VOSRDY) {}

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
* @brief  Configure and enable LPUART.
* @param  None
* @retval None
*/
static void LPUART_Config(void)
{
  RCC_OscInitTypeDef  RCC_OscInitStruct;
  UART_WakeUpTypeDef  wakeUpConfig;
  HAL_StatusTypeDef   ret = HAL_OK;

  /* -1- Enable the HSI clock */
  __HAL_RCC_HSI_ENABLE();
  RCC_OscInitStruct.OscillatorType  =  RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.PLL.PLLState    = RCC_PLL_NONE;
  RCC_OscInitStruct.HSIState        = RCC_HSI_ON;
  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
  if(ret != HAL_OK)
  {
    Error_Handler();
  }

  /* -2- Configure the LPUART serial parameters */
  /* LPUART configured as follows:
      - Word Length = 8 Bits
      - Stop Bit    = One Stop bit
      - Parity      = None
      - BaudRate    = 115200 baud
      - Hardware flow control disabled (RTS and CTS signals) */

  /* Configure the LPUART serial parameters */
  Lpuart1Handle.Instance         = USARTx;
  Lpuart1Handle.Init.BaudRate    = 115200;
  Lpuart1Handle.Init.WordLength  = UART_WORDLENGTH_8B;
  Lpuart1Handle.Init.StopBits    = UART_STOPBITS_1;
  Lpuart1Handle.Init.Parity      = UART_PARITY_NONE;
  Lpuart1Handle.Init.HwFlowCtl   = UART_HWCONTROL_NONE;
  Lpuart1Handle.Init.Mode        = UART_MODE_TX_RX;
  Lpuart1Handle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

  ret = HAL_UART_Init(&Lpuart1Handle);
  if(ret != HAL_OK)
  {
    Error_Handler();
  }

  /* Enable the FIFO mode */
  HAL_UARTEx_EnableFifoMode(&Lpuart1Handle);

  /* Set the RXFIFO threshold */
  HAL_UARTEx_SetRxFifoThreshold(&Lpuart1Handle, UART_RXFIFO_THRESHOLD_1_4);

  /* Configure the LPUART to wakeup on start bit */
  wakeUpConfig.WakeUpEvent    = UART_WAKEUP_ON_STARTBIT;
  wakeUpConfig.AddressLength  = UART_ADDRESS_DETECT_7B;
  wakeUpConfig.Address        = 0x69;
  HAL_UARTEx_StopModeWakeUpSourceConfig(&Lpuart1Handle, wakeUpConfig);

  /* Enable the LPUART MCU wake-up in Stop mode */
  ret = HAL_UARTEx_EnableStopMode(&Lpuart1Handle);
  if(ret != HAL_OK)
  {
    Error_Handler();
  }

  /* Enable the LPUART wake-up from stop mode on the Fifo threshold Interrupt */
  __HAL_UART_ENABLE_IT(&Lpuart1Handle, UART_IT_RXFT);

  /* Disable the LPUART wake-up from low-power mode Interrupt */
  __HAL_UART_DISABLE_IT(&Lpuart1Handle, UART_IT_WUF);
}

#ifdef LOW_POWER_BOARD
/**
  * @brief  DMA Transfer Complete callback
  * @param  DMA_HandleTypeDef: DMA handle
  * @retval None
  */
static void DMA_TransferCompleteCallback(DMA_HandleTypeDef *hdma)
{
  DMA_TransferCompleteFlag++;
}

/**
  * @brief  DMA Transfer Error callback
  * @param  DMA_HandleTypeDef: DMA handle
  * @retval None
  */
static void DMA_TransferErrorCallback(DMA_HandleTypeDef *hdma)
{
  DMA_TransferErrorFlag++;
}

#else

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval 0  : pBuffer1 identical to pBuffer2
  *         >0 : pBuffer1 differs from pBuffer2
  */
static uint8_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
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

#endif /* LOW_POWER_BOARD */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* Turn LED3 on */
  BSP_LED_On(LED3);
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

