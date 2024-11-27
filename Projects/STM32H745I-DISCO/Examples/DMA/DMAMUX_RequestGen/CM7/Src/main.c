/**
  ******************************************************************************
  * @file    DMA/DMAMUX_RequestGen/CM7/Src/main.c
  * @author  MCD Application Team
  * @brief   This example provides a description of how to configure
  *          DMA2D peripheral in Memory to Memory with blending transfer mode.
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

/** @addtogroup STM32H7xx_HAL_Examples
  * @{
  */

/** @addtogroup DMAMUX_RequestGen
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO   uint32_t DMA_TransferErrorFlag = 0;

/*Buffer location and size should aligned to cache line size (32 bytes) */
#if defined ( __ICCARM__ )
#pragma location = 0x38000000
uint32_t SRC_Buffer_LED1_Toggle[2]  =
                                      {  0xFFFF & (~LED1_PIN), /*Value for LED 1 ON  */
                                         0xFFFF                /*Value for LED 1 OFF */
                                      };
#elif defined ( __CC_ARM )
ALIGN_32BYTES(__attribute__((section (".RAM_D3"))) uint32_t SRC_Buffer_LED1_Toggle[2]) =
                                      {  0xFFFF & (~LED1_PIN), /*Value for LED 1 ON  */
                                         0xFFFF                /*Value for LED 1 OFF */
                                      };
#elif defined ( __GNUC__ )
ALIGN_32BYTES(uint32_t __attribute__((section (".RAM_D3"))) SRC_Buffer_LED1_Toggle[2])  =
                                      {  0xFFFF & (~LED1_PIN), /*Value for LED 1 ON  */
                                         0xFFFF                /*Value for LED 1 OFF */
                                      };
#endif

DMA_HandleTypeDef DMA_Handle;
LPTIM_HandleTypeDef  LptimHandle;

/* Private function prototypes -----------------------------------------------*/
static void MPU_Config(void);
static void SystemClock_Config(void);
static void CPU_CACHE_Enable(void);
static void LPTIM_Config(void);
static void HAL_TransferError(DMA_HandleTypeDef *hdma);
static void Error_Handler(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  HAL_DMA_MuxRequestGeneratorConfigTypeDef dmamux_ReqGenParams;

  /* System Init, System clock, voltage scaling and L1-Cache configuration are done by CPU1 (Cortex-M7)
     in the meantime Domain D2 is put in STOP mode(Cortex-M4 in deep-sleep)
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

  /* Configure the system clock to 400 MHz */
  SystemClock_Config();

  /* When system initialization is finished, Cortex-M7 could wakeup (when needed) the Cortex-M4  by means of
     HSEM notification or by any D2 wakeup source (SEV,EXTI..)   */

  /* -1- Initialize LEDs mounted on STM32H747I-DISCO board */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);


  /*##-2- Configure the DMA ##################################################*/

  /* Enable BDMA clock */
  __HAL_RCC_BDMA_CLK_ENABLE();

  /* Configure the DMA handler for Transmission process     */
  /* DMA mode is set to circular for an infinite DMA transfer */
  DMA_Handle.Instance                 = BDMA_Channel0;

  DMA_Handle.Init.Request             = BDMA_REQUEST_GENERATOR0;
  DMA_Handle.Init.Direction           = DMA_MEMORY_TO_PERIPH;
  DMA_Handle.Init.PeriphInc           = DMA_PINC_DISABLE;
  DMA_Handle.Init.MemInc              = DMA_MINC_ENABLE;
  DMA_Handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
  DMA_Handle.Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;
  DMA_Handle.Init.Mode                = DMA_CIRCULAR;
  DMA_Handle.Init.Priority            = DMA_PRIORITY_LOW;
  DMA_Handle.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
  DMA_Handle.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
  DMA_Handle.Init.MemBurst            = DMA_MBURST_SINGLE;
  DMA_Handle.Init.PeriphBurst         = DMA_PBURST_SINGLE;

  /* Initialize the DMA with for Transmission process */
  HAL_DMA_Init(&DMA_Handle);

  /* Select Callbacks functions called after Transfer complete and Transfer error */
  HAL_DMA_RegisterCallback(&DMA_Handle, HAL_DMA_XFER_CPLT_CB_ID, NULL);
  HAL_DMA_RegisterCallback(&DMA_Handle, HAL_DMA_XFER_ERROR_CB_ID, HAL_TransferError);

  /* NVIC configuration for DMA transfer complete interrupt*/
  HAL_NVIC_SetPriority(BDMA_Channel0_IRQn, 0, 1);
  HAL_NVIC_EnableIRQ(BDMA_Channel0_IRQn);

  /*##-3- Configure and enable the DMAMUX Request generator  ####################*/
  dmamux_ReqGenParams.SignalID  = HAL_DMAMUX2_REQ_GEN_LPTIM2_OUT; /* External request signal is LPTIM2 signal */
  dmamux_ReqGenParams.Polarity  = HAL_DMAMUX_REQ_GEN_RISING;      /* External request signal edge is Rising  */
  dmamux_ReqGenParams.RequestNumber = 1;                          /* 1 requests on each edge of the external request signal  */

  HAL_DMAEx_ConfigMuxRequestGenerator(&DMA_Handle, &dmamux_ReqGenParams);

  /* NVIC configuration for DMAMUX request generator overrun errors*/
  HAL_NVIC_SetPriority(DMAMUX2_OVR_IRQn, 0, 1);
  HAL_NVIC_EnableIRQ(DMAMUX2_OVR_IRQn);

  HAL_DMAEx_EnableMuxRequestGenerator (&DMA_Handle);

  /*##-4- Configure and enable the LPTIM2 used as DMA external request generator signal #####*/
  LPTIM_Config();

  /*##-5- Start the DMA transfer ################################################*/
  /* DMA source buffer is  SRC_BUFFER_LED1_TOGGLE containing values to be written
  to GPIOJ ODR register in order to turn LED1 On/Off each time comes a request from the DMAMUX request generator */
  HAL_DMA_Start_IT(&DMA_Handle, (uint32_t)SRC_Buffer_LED1_Toggle, (uint32_t)&GPIOJ->ODR, 2);

  /* Infinite loop */
  while (1)
  {
    if(DMA_TransferErrorFlag != 0)
    {
      Error_Handler();
    }
  }
}

/**
  * @brief  Configure and start the LPTIM2 with 2sec period and 50% duty cycle.
* @param  None
* @retval None
*/
static void LPTIM_Config(void)
{

  uint32_t periodValue;
  uint32_t pulseValue ;

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;

  /* Enable the LSE clock source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  /* LPTIM2 clock source set to LSE*/
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LPTIM2;
  PeriphClkInitStruct.Lptim2ClockSelection = RCC_LPTIM2CLKSOURCE_LSE;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);

  periodValue = (2 * LSE_VALUE)/4;    /* Calculate the Timer  Autoreload value for 2sec period */
  pulseValue  = periodValue/2;        /* Set the Timer  pulse value for 50% duty cycle         */

  /* TIM1 Peripheral clock enable */
  __HAL_RCC_LPTIM2_CLK_ENABLE();

  LptimHandle.Instance                           = LPTIM2;

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

static void HAL_TransferError(DMA_HandleTypeDef *hdma)
{
  DMA_TransferErrorFlag = 1;
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
  *            SYSCLK(Hz)                     = 400000000 (CM7 CPU Clock)
  *            HCLK(Hz)                       = 200000000 (CM4 CPU, AXI and AHBs Clock)
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

