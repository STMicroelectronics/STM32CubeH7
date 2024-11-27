/**
  ******************************************************************************
  * @file    PWR/PWR_Domain3SystemControl/CM7/Src/main.c
  * @author  MCD Application Team
  * @brief   Main program body for Cortex-M7.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2018 STMicroelectronics.
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
#define BDMA_CH7_IT_EXTI_LINE         EXTI_LINE73
#define LPUART1_RX_WAKEUP_EXTI_MODE   (EXTI_MODE_IT | EXTI_MODE_EVT)
#define BDMA_WAKEUP_EXTI_MODE         (EXTI_MODE_IT | EXTI_MODE_EVT)
#define BUFFER_SIZE                   20U
#define TX_CHUNK_SIZE                 4U
#define DMAMUX_REQUEST_NUMBER         4U
#define DMA_TRANSFER_NUMBER           1U
#define DMA_TRANSFER_NO_ERROR         0U

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO  uint32_t DMA_TransferErrorFlag    = 0;
__IO  uint32_t DMA_TransferCompleteFlag = 0;

#if defined ( __CC_ARM )  /* !< ARM Compiler */
  uint8_t aRxBuffer[BUFFER_SIZE] __attribute__((at(0x38000000)));
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
#if defined (LOW_POWER_BOARD)
static void DMA_TransferErrorCallback(DMA_HandleTypeDef *hdma);
static void DMA_TransferCompleteCallback(DMA_HandleTypeDef *hdma);
#else
static uint8_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);
#endif /* LOW_POWER_BOARD */

static void SystemClock_Config(void);
static void LPUART_Config(void);
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
#if defined (LOW_POWER_BOARD)
  int32_t timeout;

  /* System Init, System clock, voltage scaling and L1-Cache configuration are done by CPU1 (Cortex-M7)
     in the meantime Domain D2 is put in STOP mode(Cortex-M4 in deep-sleep)
  */

  /* This project template calls firstly two functions in order to configure MPU feature
  and to enable the CPU Cache, respectively MPU_Config() and CPU_CACHE_Enable().
  These functions are provided as template implementation that User may integrate
  in his application, to enhance the performance in case of use of AXI interface
  with several masters. */

  /* Enable the CPU Cache */
  CPU_CACHE_Enable();

  /* Wait until CPU2 boots and enters in stop mode or timeout*/
  timeout = 0xFFFF;
  while((__HAL_RCC_GET_FLAG(RCC_FLAG_D2CKRDY) != RESET) && (timeout-- > 0));
  if ( timeout < 0 )
  {
    Error_Handler();
  }

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

  /* When system initialization is finished, Cortex-M7 will release Cortex-M4  by means of
     HSEM notification */

  /*HW semaphore Clock enable*/
  __HAL_RCC_HSEM_CLK_ENABLE();

  /*Take HSEM */
  HAL_HSEM_FastTake(HSEM_ID_0);
  /*Release HSEM in order to notify the CPU2(CM4)*/
  HAL_HSEM_Release(HSEM_ID_0,0);

  /* wait until CPU2 wakes up from stop mode */
  timeout = 0xFFFF;
  while((__HAL_RCC_GET_FLAG(RCC_FLAG_D2CKRDY) == RESET) && (timeout-- > 0));
  if ( timeout < 0 )
  {
    Error_Handler();
  }

/* -2- Initialize LED1 and LED3 */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED3);

  /* -3- Configure the LPUART1 peripheral */
  LPUART_Config();

  HAL_DMA_MuxRequestGeneratorConfigTypeDef dmamux_ReqGenParams;
  HAL_DMA_MuxSyncConfigTypeDef dmamux_syncParams;

  /* Set LED1 On to inform the user that the system is start */
  BSP_LED_On(LED1);

  /* -4- Configure the BDMA as D3 domain penclear source */
  /* Enable BDMA clock */
  __HAL_RCC_BDMA_CLK_ENABLE();

  /* Configure the DMA channle7 handler for D3 penclear source */
  BDMA_Handle7.Instance                  = BDMA_Channel7;
  BDMA_Handle7.Init.Request              = BDMA_REQUEST_GENERATOR0;
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
  dmamux_ReqGenParams.SignalID  = HAL_DMAMUX2_REQ_GEN_LPUART1_RX_WKUP;  /* External request signal is Lpuart1_it_R_WUP_ASYNC signal */
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

  /* Allocate the Flash memory to the CM4 to be able to execute code when CM7 enter STOP mode */
  __HAL_RCC_FLASH_C2_ALLOCATE();

  /* -8- Configure the LPUART EXTI lines */
  /* Deactivate all LPUART1 interrupts and events for CM7 */
  HAL_EXTI_D1_EventInputConfig(LPUART1_RX_WAKEUP_EXTI_LINE, LPUART1_RX_WAKEUP_EXTI_MODE, DISABLE);
  HAL_EXTI_D1_EventInputConfig(LPUART1_GLOBAL_IT_EXTI_LINE, LPUART1_RX_WAKEUP_EXTI_MODE, DISABLE);

  /* Configure BDMA Channel7 event as D3 domain pendclear source  */
  HAL_EXTI_D3_EventInputConfig(LPUART1_RX_WAKEUP_EXTI_LINE, ENABLE, BDMA_CH7_CLEAR);

  /* Enable rising trigger for LPUART1 RX Event and Interrupt */
  HAL_EXTI_EdgeConfig(LPUART1_RX_WAKEUP_EXTI_LINE, EXTI_RISING_EDGE);

  /* Deactivate all LPUART1 interrupts and events for CM4 */
  HAL_EXTI_D2_EventInputConfig(LPUART1_RX_WAKEUP_EXTI_LINE, LPUART1_RX_WAKEUP_EXTI_MODE, DISABLE);
  HAL_EXTI_D2_EventInputConfig(LPUART1_GLOBAL_IT_EXTI_LINE, LPUART1_RX_WAKEUP_EXTI_MODE, DISABLE);

  /* Activate BDMA Channel 7 interrupt and event lines for CM7 */
  HAL_EXTI_D1_EventInputConfig(BDMA_CH7_IT_EXTI_LINE, BDMA_WAKEUP_EXTI_MODE, ENABLE);

  /* Deactivate BDMA Channel 7 interrupt and event lines for CM4 */
  HAL_EXTI_D2_EventInputConfig(BDMA_CH7_IT_EXTI_LINE, BDMA_WAKEUP_EXTI_MODE, DISABLE);

  /* -9- Insert 5 seconds delay before entering Stop mode */
  HAL_Delay(5000);

  while(1)
  {
    DMA_TransferCompleteFlag = 0;
    DMA_TransferErrorFlag    = 0;

    /* -10- Start the DMA Reception in interrupt mode */
    if(HAL_DMA_Start_IT(&BDMA_Handle7, (uint32_t)&(Lpuart1Handle.Instance->RDR),(uint32_t)&(aRxBuffer), BUFFER_SIZE)  != HAL_OK)
    {
      Error_Handler();
    }

    /* Clear STOP flag */
    __HAL_PWR_CLEAR_FLAG(PWR_CPU_FLAGS);

    /* -11- Set LED1 Off */
    BSP_LED_Off(LED1);

    /* Enter D1 domain to DStop mode */
    HAL_PWREx_EnterSTOPMode(PWR_MAINREGULATOR_ON, PWR_STOPENTRY_WFI, PWR_D1_DOMAIN);

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
    if(HAL_UART_Transmit(&Lpuart1Handle, (uint8_t*)&aRxBuffer, BUFFER_SIZE, HAL_MAX_DELAY) != HAL_OK)
    {
      Error_Handler();
    }
  }


#else

  uint32_t bufferIndex = 0;

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

  /* -2- Initialize LED1 and LED3 */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED3);

  /* -3- Configure the LPUART1 peripheral */
  LPUART_Config();

  /* -4- Toggle led1 then waiting for user to press button */
  BSP_LED_On(LED1);

  /* -5- Configure Tamper Key */
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);

  /* -6- Wait for Tamper Key press before starting the Communication */
  while (BSP_PB_GetState(BUTTON_USER) == RESET)
  {
  }

  /* -7- Wait for Tamper Key to be release before starting the Communication */
  while (BSP_PB_GetState(BUTTON_USER) == SET)
  {
  }

  /* The board sends the message and expects to receive it back */
  while(1)
  {
    /* -8- Transmit the aTxBuffer data */
    if(HAL_UART_Transmit(&Lpuart1Handle, (uint8_t*)(aTxBuffer + bufferIndex), TX_CHUNK_SIZE, HAL_MAX_DELAY) != HAL_OK)
    {
      Error_Handler();
    }
    bufferIndex += TX_CHUNK_SIZE;

    if(bufferIndex >= BUFFER_SIZE)
    {
      /* -9- Receive the LPUART data */
      if(HAL_UART_Receive(&Lpuart1Handle, (uint8_t*)&aRxBuffer, BUFFER_SIZE, HAL_MAX_DELAY) != HAL_OK)
      {
        Error_Handler();
      }

      /* -10- Check the received data */
      if(Buffercmp(aRxBuffer, aTxBuffer, BUFFER_SIZE))
      {
        Error_Handler();
      }

      bufferIndex = 0;
      HAL_Delay(200);
      BSP_LED_Toggle(LED1);

    }
  }

#endif /* LOW_POWER_BOARD */
}


/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE BYPASS)
  *            SYSCLK(Hz)                     = 400000000 (CPU Clock)
  *            HCLK(Hz)                       = 200000000 (Cortex-M4 CPU, Bus matrix Clocks)
  *            AHB Prescaler                  = 2
  *            D1 APB3 Prescaler              = 2 (APB3 Clock  100MHz)
  *            D2 APB1 Prescaler              = 2 (APB1 Clock  100MHz)
  *            D2 APB2 Prescaler              = 2 (APB2 Clock  100MHz)
  *            D3 APB4 Prescaler              = 2 (APB4 Clock  100MHz)
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 4
  *            PLL_N                          = 400
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
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
  RCC_OscInitStruct.CSIState = RCC_CSI_OFF;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;

  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 400;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLQ = 4;

  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_1;
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

          To do this please uncomment the following code
  */

  /*
  __HAL_RCC_CSI_ENABLE() ;

  __HAL_RCC_SYSCFG_CLK_ENABLE() ;

  HAL_EnableCompensationCell();
  */
}

/**
* @brief  Configure and enable LPUART.
* @param  None
* @retval None
*/
static void LPUART_Config(void)
{
  RCC_OscInitTypeDef  RCC_OscInitStruct;
  __IO UART_WakeUpTypeDef  wakeUpConfig;
  HAL_StatusTypeDef   ret = HAL_OK;

  /* -1- Enable the HSI clock */
  __HAL_RCC_HSI_ENABLE();
  RCC_OscInitStruct.OscillatorType  = RCC_OSCILLATORTYPE_HSI;
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
  Lpuart1Handle.FifoMode         = UART_FIFOMODE_ENABLE;
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

#if defined (LOW_POWER_BOARD)

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

