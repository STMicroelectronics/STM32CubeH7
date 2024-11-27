/**
  ******************************************************************************
  * @file    DMA/DMAMUX_SYNC/Src/main.c
  * @author  MCD Application Team
  * @brief   This example shows how to use the DMA with the DMAMUX to
  *          synchronize a transfer with LPTIM1 output period using the
  *          STM32H7xx HAL API.
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

/** @addtogroup DMAMUX_SYNC
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t TxSyncMessage[] = "\n\r10\n\r09\n\r08\n\r07\n\r06\n\r05\n\r04\n\r03\n\r02\n\r01\n\r00";

/* Size of Transmission buffer */
#define TX_SYNC_MESSAGE_SIZE               (COUNTOF(TxSyncMessage) - 1)

uint8_t BriefMessage[] = "This example shows how to use the DMA with the DMAMUX to synchronize a transfer with LPTIM1 output signal.\n\rThe USART1 is used in DMA synchronized mode to send a countdown from 10 to 00 with a period of 2sec \n\r\n\rStart countdown :\n\r";
#define BRIEF_MESSAGE_SIZE                 (COUNTOF(BriefMessage) - 1)

uint8_t TxEndMessage[] = "\n\r\n\rExample Finished\n\r";
#define TX_END_MESSAGE_SIZE                (COUNTOF(TxEndMessage) - 1)

/* Uart Handler definition */
UART_HandleTypeDef   UartHandle;
/* LPTIM Handler definition */
LPTIM_HandleTypeDef  LptimHandle;
/* DMA Handler definition */
DMA_HandleTypeDef    DMA_Handle;

/* Private function prototypes -----------------------------------------------*/
static void MPU_Config(void);
static void SystemClock_Config(void);
static void CPU_CACHE_Enable(void);
void LPTIM_Config(void);
static void Error_Handler(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  HAL_DMA_MuxSyncConfigTypeDef dmamux_syncParams;

  /* This project calls firstly function CPU_CACHE_Enable().
     This function is provided as implementation that the user may integrate
     into his application to enable the Coretx-M7 I/D cache and enhance the performance.
   */

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

  /* Configure the system clock to 280 MHz */
  SystemClock_Config();

  /* -1- Initialize LEDs mounted on STM32H7B3I-EVAL board */
  BSP_LED_Init(LED_GREEN);
  BSP_LED_Init(LED_RED);

  /*##-2- Configure the DMA ##################################################*/
  /* Enable DMA clock */
  DMAx_CLK_ENABLE();

  /* Configure the DMA handler for Transmission process */
  DMA_Handle.Instance                 = USARTx_TX_DMA_STREAM;
  DMA_Handle.Init.Request             = DMA_REQUEST_USART1_TX;
  DMA_Handle.Init.Direction           = DMA_MEMORY_TO_PERIPH;
  DMA_Handle.Init.PeriphInc           = DMA_PINC_DISABLE;
  DMA_Handle.Init.MemInc              = DMA_MINC_ENABLE;
  DMA_Handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  DMA_Handle.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
  DMA_Handle.Init.Mode                = DMA_NORMAL;
  DMA_Handle.Init.Priority            = DMA_PRIORITY_LOW;
  DMA_Handle.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
  DMA_Handle.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
  DMA_Handle.Init.MemBurst            = DMA_MBURST_SINGLE;
  DMA_Handle.Init.PeriphBurst         = DMA_PBURST_SINGLE;

  /* Initialize the DMA with for Transmission process */
  HAL_DMA_Init(&DMA_Handle);

  /* Configure the DMAMUX with the Synchronization parameters */
  dmamux_syncParams.EventEnable   = ENABLE;                       /* Enable DMAMUX event generation each time  RequestNumber are passed from DMAMUX to the DMA */
  dmamux_syncParams.SyncPolarity  = HAL_DMAMUX_SYNC_RISING;       /* Synchronization edge is Rising  */
  dmamux_syncParams.RequestNumber = 4;                            /* 4 requests are autorized after each edge of the sync signal */
  dmamux_syncParams.SyncSignalID  = HAL_DMAMUX1_SYNC_LPTIM1_OUT;  /* Sync signal is LPTIM1_OUT */
  dmamux_syncParams.SyncEnable    = ENABLE;                       /* Synchronization is enabled */

  HAL_DMAEx_ConfigMuxSync(&DMA_Handle, &dmamux_syncParams);

  /*##-3- Configure and start the LPTIM1 used for DMA transfer Synchronization#*/
  /* LPTIM1 is configured with 2sec period */
  LPTIM_Config();

  /*##-4- Configure the UART ##################################################*/
  /* USART configuration */
  UartHandle.Instance = USARTx;

  /* Configure the serial params */
  UartHandle.Init.BaudRate   = 115200;
  UartHandle.Init.Mode       = UART_MODE_TX_RX;
  UartHandle.Init.Parity     = UART_PARITY_NONE;
  UartHandle.Init.StopBits   = UART_STOPBITS_1;
  UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
  UartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;

  if (HAL_UART_Init(&UartHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /*##-5-Send Brief Message with the UART in Polling mode ######################*/
  /* Start transmission data through "BriefMessage" buffer */
  if(HAL_UART_Transmit(&UartHandle, (uint8_t*)BriefMessage, BRIEF_MESSAGE_SIZE, HAL_MAX_DELAY )!= HAL_OK)
  {
    /* Transfer error in transmission process */
    Error_Handler();
  }

  /*##-6- Start the synchronized transmission process #####################################*/
  /* Start transmission of the countdown data through "TxSyncMessage" buffer */
  if(HAL_UART_Transmit_DMA(&UartHandle, (uint8_t*)TxSyncMessage, TX_SYNC_MESSAGE_SIZE)!= HAL_OK)
  {
    /* Transfer error in transmission process */
    Error_Handler();
  }

  /*##-7- Wait for the end of the synchronized transfer ###################################*/
  while (HAL_UART_GetState(&UartHandle) != HAL_UART_STATE_READY)
  {
  }

  /*##-8-Send example ending  Message with the UART in Polling mode #####################################*/
  /* Start transmission data through "TxEndMessage" buffer */
  if(HAL_UART_Transmit(&UartHandle, (uint8_t*)TxEndMessage, TX_END_MESSAGE_SIZE, HAL_MAX_DELAY )!= HAL_OK)
  {
    /* Transfer error in transmission process */
    Error_Handler();
  }

  BSP_LED_On(LED_GREEN);

  /* Infinite loop */
  while (1)
  {
  }
}

/**
  * @brief  Configure and start the LPTIM1 with 2sec period and 50% duty cycle.
  * @param  None
  * @retval None
  */
void LPTIM_Config(void)
{

  uint32_t periodValue;
  uint32_t pulseValue ;

  GPIO_InitTypeDef  GPIO_InitStruct;

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;

  __HAL_RCC_GPIOD_CLK_ENABLE();

  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF1_LPTIM1;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /* Enable the LSE clock source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  /* LPTIM1 clock source set to LSE*/
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LPTIM1;
  PeriphClkInitStruct.Lptim1ClockSelection = RCC_LPTIM1CLKSOURCE_LSE;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);

  periodValue = (2 * LSE_VALUE) / 4;    /* Calculate the Timer  Autoreload value for 2sec period */
  pulseValue  = periodValue / 2;        /* Set the Timer  pulse value for 50% duty cycle         */

  /* TIM1 Peripheral clock enable */
  __HAL_RCC_LPTIM1_CLK_ENABLE();

  LptimHandle.Instance                           = LPTIM1;

  LptimHandle.Init.CounterSource                 = LPTIM_COUNTERSOURCE_INTERNAL;
  LptimHandle.Init.UpdateMode                    = LPTIM_UPDATE_ENDOFPERIOD;
  LptimHandle.Init.OutputPolarity                = LPTIM_OUTPUTPOLARITY_LOW;
  LptimHandle.Init.Clock.Source                  = LPTIM_CLOCKSOURCE_APBCLOCK_LPOSC;
  LptimHandle.Init.Clock.Prescaler               = LPTIM_PRESCALER_DIV4;
  LptimHandle.Init.UltraLowPowerClock.Polarity   = LPTIM_CLOCKPOLARITY_RISING;
  LptimHandle.Init.UltraLowPowerClock.SampleTime = LPTIM_CLOCKSAMPLETIME_DIRECTTRANSITION;
  LptimHandle.Init.Trigger.Source                = LPTIM_TRIGSOURCE_SOFTWARE;
  LptimHandle.Init.Trigger.ActiveEdge            = LPTIM_ACTIVEEDGE_RISING;
  LptimHandle.Init.Trigger.SampleTime            = LPTIM_TRIGSAMPLETIME_DIRECTTRANSITION;

  if(HAL_LPTIM_Init(&LptimHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* Start the timer */
  if (HAL_LPTIM_PWM_Start(&LptimHandle, periodValue, pulseValue) != HAL_OK)
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
  /* Turn RED LED on */
  BSP_LED_On(LED_RED);

  while(1)
  {
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
    Error_Handler();
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
    Error_Handler();
  }

    /*
  Note : The activation of the I/O Compensation Cell is recommended with communication  interfaces
          (GPIO, SPI, FMC, OSPI ...)  when  operating at  high frequencies(please refer to product datasheet)
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

