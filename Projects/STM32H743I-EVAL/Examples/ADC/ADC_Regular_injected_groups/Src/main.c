/**
  ******************************************************************************
  * @file    ADC/ADC_Regular_injected_groups/Src/main.c
  * @author  MCD Application Team
  * @brief   This example provides a short description of how to use the ADC
  *          peripheral to perform conversions using the 2 ADC groups: 
  *          group regular for ADC conversions on main stream and 
  *          group injected for ADC conversions limited on specific events
  *          (conversions injected within main conversions stream). Other 
  *          peripherals used: DMA, TIM (ADC group regular conversions 
  *          triggered  by TIM, ADC group regular conversion data
  *          transferred by DMA).
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

/** @addtogroup ADC_Regular_injected_groups
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define RANGE_12BITS                   ((uint32_t) 4095)    /* Max digital value with a full range of 12 bits */
#define RANGE_16BITS                   ((uint32_t)65535)    /* Max digital value with a full range of 16 bits */

/* ADC parameters */
#define ADCCONVERTEDVALUES_BUFFER_SIZE ((uint32_t)   32)    /* Size of array containing ADC converted values, aligned on cache line size */


#if defined(ADC_TRIGGER_FROM_TIMER)
/* Timer for ADC trigger parameters */
#define TIMER_FREQUENCY                ((uint32_t) 1000)    /* Timer frequency (unit: Hz). With a timer 16 bits and time base freq min 1Hz, range is min=1Hz, max=32kHz. */
#define TIMER_FREQUENCY_RANGE_MIN      ((uint32_t)    1)    /* Timer minimum frequency (unit: Hz), used to calculate frequency range. With a timer 16 bits, maximum frequency will be 32000 times this value. */
#define TIMER_PRESCALER_MAX_VALUE      (0xFFFF-1)           /* Timer prescaler maximum value (0xFFFF for a timer 16 bits) */
#endif /* ADC_TRIGGER_FROM_TIMER */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Peripherals handlers declaration */
/* ADC handler declaration */
ADC_HandleTypeDef    AdcHandle;

#if defined(ADC_TRIGGER_FROM_TIMER)
/* TIM handler declaration */
TIM_HandleTypeDef    TimHandle;
#endif /* ADC_TRIGGER_FROM_TIMER */

#if defined(WAVEFORM_VOLTAGE_GENERATION_FOR_TEST)
/* DAC handler declaration */
DAC_HandleTypeDef    DacHandle;  /* DAC used for waveform voltage generation for test */
#endif /* WAVEFORM_VOLTAGE_GENERATION_FOR_TEST */

/* Variable containing ADC conversions results */
ALIGN_32BYTES(__IO uint16_t   aADCxConvertedValues[ADCCONVERTEDVALUES_BUFFER_SIZE]); /* ADC conversion results table of regular group, channel on rank1 */
__IO uint16_t   uhADCxConvertedValue_Injected;                        /* ADC conversion result of injected group, channel on rank1 */

uint16_t        uhADCxConvertedValue_Regular_Avg_half1;  /* Average of the 1st half of ADC conversion results table of regular group, channel on rank1 */
uint16_t        uhADCxConvertedValue_Regular_Avg_half2;  /* Average of the 2nd half of ADC conversion results table of regular group, channel on rank1 */
uint16_t*       puhADCxConvertedValue_Regular_Avg;       /* Pointer to the average of the 1st or 2nd half of ADC conversion results table of regular group, channel on rank1 */

/* Variables to manage push button on board: interface between ExtLine interruption and main program */
__IO uint8_t    ubUserButtonClickEvent = RESET;  /* Event detection: Set after User Button interrupt */

/* Private function prototypes -----------------------------------------------*/
static void MPU_Config(void);
static void SystemClock_Config(void);
static void Error_Handler(void);
static void ADC_Config(void);
static void CPU_CACHE_Enable(void);

#if defined(ADC_TRIGGER_FROM_TIMER)
static void TIM_Config(void);
#endif /* ADC_TRIGGER_FROM_TIMER */

#if defined(WAVEFORM_VOLTAGE_GENERATION_FOR_TEST)
static void WaveformVoltageGenerationForTest_Config(void);
static void WaveformVoltageGenerationForTest_Update(void);
#endif /* WAVEFORM_VOLTAGE_GENERATION_FOR_TEST */

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
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
  
  /* Configure the system clock to 400 MHz */
  SystemClock_Config();
  
  
  /*## Configure peripherals #################################################*/
  
  /* Initialize LEDs on board */
  BSP_LED_Init(LED4);
  BSP_LED_Init(LED1);
  
  /* Configure Tamper push-button in Interrupt mode */
  BSP_PB_Init(BUTTON_TAMPER, BUTTON_MODE_EXTI);
  
  /* Configure the ADCy peripheral */
  ADC_Config();
  
  /* Run the ADC calibration in single-ended mode */
  if (HAL_ADCEx_Calibration_Start(&AdcHandle, ADC_CALIB_OFFSET_LINEARITY, ADC_SINGLE_ENDED) != HAL_OK)
  {
    /* Calibration Error */
    Error_Handler();
  }

#if defined(ADC_TRIGGER_FROM_TIMER)
  /* Configure the TIM peripheral */
  TIM_Config();
#endif /* ADC_TRIGGER_FROM_TIMER */

#if defined(WAVEFORM_VOLTAGE_GENERATION_FOR_TEST)
  /* Configure the DAC peripheral and generate a constant voltage of Vdda/2.  */
  WaveformVoltageGenerationForTest_Config();
#endif /* WAVEFORM_VOLTAGE_GENERATION_FOR_TEST */
  
  
  /*## Enable peripherals ####################################################*/
  
#if defined(ADC_TRIGGER_FROM_TIMER)
  /* Timer enable */
  if (HAL_TIM_Base_Start(&TimHandle) != HAL_OK)
  {
    /* Counter Enable Error */
    Error_Handler();
  }
#endif /* ADC_TRIGGER_FROM_TIMER */
  
  
  /*## Start ADC conversions #################################################*/
  
  /* Start ADC conversion on regular group with transfer by DMA */
  if (HAL_ADC_Start_DMA(&AdcHandle,
                        (uint32_t *)aADCxConvertedValues,
                        ADCCONVERTEDVALUES_BUFFER_SIZE
                       ) != HAL_OK)
  {
    /* Start Error */
    Error_Handler();
  }
  
  
  /* Infinite loop */
  while (1)
  {

    /* Wait for event on push button to perform following actions */
    while ((ubUserButtonClickEvent) == RESET)
    {
    }
    /* Reset variable for next loop iteration */
    ubUserButtonClickEvent = RESET;
    
    /* Start ADC conversion on injected group */
    if (HAL_ADCEx_InjectedStart_IT(&AdcHandle) != HAL_OK)
    {
      /* Start Conversation Error */
      Error_Handler();
    }
    

#if defined(WAVEFORM_VOLTAGE_GENERATION_FOR_TEST)
    /* Modifies the voltage level incrementally from 0V to Vdda at each call. */
    /* Circular waveform of ramp: When the maximum level is reaches,          */
    /* restart from 0V.                                                       */
    WaveformVoltageGenerationForTest_Update();
#endif /* WAVEFORM_VOLTAGE_GENERATION_FOR_TEST */

    /* Wait for acquisition time of ADC samples on regular and injected       */
    /* groups:                                                                */
    /* wait time to let a full 1/2 buffer of regular group to be filled with  */
    /* new conversion values (in ms)      */
    HAL_Delay(31);
    
    /* Turn-on/off LED1 in function of ADC conversion result */
    /* - Turn-off if voltage measured by injected group is below voltage      */
    /*   measured by regular group (average of results table)                 */
    /* - Turn-off if voltage measured by injected group is above voltage      */
    /*   measured by regular group (average of results table)                 */
    
    /* Variables of conversions results are updated into ADC conversions      */
    /* interrupt callback.                                                    */
    if (uhADCxConvertedValue_Injected < *puhADCxConvertedValue_Regular_Avg)
    {
      BSP_LED_Off(LED1);
    }
    else
    {
      BSP_LED_On(LED1);
    }
  
    /* For information: ADC conversion results are stored into array          */
    /* "aADCxConvertedValues" (for debug: check into watch window)            */
    
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
}

/**
  * @brief  ADC configuration
  * @param  None
  * @retval None
  */
static void ADC_Config(void)
{
  ADC_ChannelConfTypeDef   sConfig;
  ADC_InjectionConfTypeDef sConfigInjected;
  
  /* Configuration of AdcHandle init structure: ADC parameters and regular group */
  AdcHandle.Instance = ADCy;
  
  AdcHandle.Init.ClockPrescaler           = ADC_CLOCK_ASYNC_DIV2;             /* Asynchronous clock mode, input ADC clock divided by 2*/
  AdcHandle.Init.Resolution               = ADC_RESOLUTION_16B;              /* 16-bit resolution for converted data */
  AdcHandle.Init.ScanConvMode             = ADC_SCAN_DISABLE;                /* Sequencer disabled (ADC conversion on only 1 channel: channel set on rank 1) */
  AdcHandle.Init.EOCSelection             = ADC_EOC_SINGLE_CONV;             /* EOC flag picked-up to indicate conversion end */
  AdcHandle.Init.LowPowerAutoWait         = DISABLE;                         /* Auto-delayed conversion feature disabled */
#if defined(ADC_TRIGGER_FROM_TIMER)
  AdcHandle.Init.ContinuousConvMode       = DISABLE;                         /* Continuous mode disabled to have only 1 conversion at each conversion trig */
#else
  AdcHandle.Init.ContinuousConvMode       = ENABLE;                          /* Continuous mode to have maximum conversion speed (no delay between conversions) */
#endif
  AdcHandle.Init.NbrOfConversion          = 1;                               /* Parameter discarded because sequencer is disabled */
  AdcHandle.Init.DiscontinuousConvMode    = DISABLE;                         /* Parameter discarded because sequencer is disabled */
  AdcHandle.Init.NbrOfDiscConversion      = 1;                               /* Parameter discarded because sequencer is disabled */
#if defined(ADC_TRIGGER_FROM_TIMER)
  AdcHandle.Init.ExternalTrigConv         = ADC_EXTERNALTRIGCONV_Tx_TRGO;    /* Trig of conversion start done by external event */
  AdcHandle.Init.ExternalTrigConvEdge     = ADC_EXTERNALTRIGCONVEDGE_RISING;
#else
  AdcHandle.Init.ExternalTrigConv         = ADC_SOFTWARE_START;              /* Software start to trig the 1st conversion manually, without external event */
  AdcHandle.Init.ExternalTrigConvEdge     = ADC_EXTERNALTRIGCONVEDGE_NONE;   /* Parameter discarded because trig of conversion by software start (no external event) */
#endif
  AdcHandle.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DMA_CIRCULAR; /* Regular Conversion data stored in DR register only */
  AdcHandle.Init.Overrun                  = ADC_OVR_DATA_OVERWRITTEN;        /* DR register is overwritten with the last conversion result in case of overrun */
  AdcHandle.Init.OversamplingMode         = DISABLE;                         /* No oversampling */

  if (HAL_ADC_Init(&AdcHandle) != HAL_OK)
  {
    /* ADC initialization error */
    Error_Handler();
  }
  
  /* Configuration of channel on ADCx regular group on sequencer rank 1 */
  /* Note: Considering IT occurring after each number of                      */
  /*       "ADCCONVERTEDVALUES_BUFFER_SIZE" ADC conversions (IT by DMA end    */
  /*       of transfer), select sampling time and ADC clock with sufficient   */
  /*       duration to not create an overhead situation in IRQHandler.        */
  sConfig.Channel      = ADC_CHANNEL_3;               /* Sampled channel number */
  sConfig.Rank         = ADC_REGULAR_RANK_1;          /* Rank of sampled channel number ADCx_CHANNEL */
  sConfig.SamplingTime = ADC_SAMPLETIME_8CYCLES_5;    /* Sampling time (number of clock cycles unit) */
  sConfig.SingleDiff   = ADC_SINGLE_ENDED;            /* Single-ended input channel */
  sConfig.OffsetNumber = ADC_OFFSET_NONE;             /* No offset subtraction */ 
  sConfig.Offset = 0;                                 /* Parameter discarded because offset correction is disabled */

  if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)
  {
    /* Channel Configuration Error */
    Error_Handler();
  }
  
  
  /* Configure ADC injected channel */
  sConfigInjected.InjectedChannel               = ADC_CHANNEL_VREFINT;
  sConfigInjected.InjectedRank                  = ADC_INJECTED_RANK_1;
  sConfigInjected.InjectedSamplingTime          = ADC_SAMPLETIME_64CYCLES_5;
  sConfigInjected.InjectedSingleDiff            = ADC_SINGLE_ENDED;
  sConfigInjected.InjectedOffsetNumber          = ADC_OFFSET_NONE;
  sConfigInjected.InjectedOffset                = 0;
  sConfigInjected.InjectedNbrOfConversion       = 1;
  sConfigInjected.InjectedDiscontinuousConvMode = DISABLE;
  sConfigInjected.AutoInjectedConv              = DISABLE;
  sConfigInjected.QueueInjectedContext          = DISABLE;
  sConfigInjected.ExternalTrigInjecConv         = ADC_INJECTED_SOFTWARE_START;
  sConfigInjected.ExternalTrigInjecConvEdge     = ADC_EXTERNALTRIGINJECCONV_EDGE_NONE;
  sConfigInjected.InjecOversamplingMode         = DISABLE;  

  if (HAL_ADCEx_InjectedConfigChannel(&AdcHandle, &sConfigInjected) != HAL_OK)
  {
    /* Channel Configuration Error */
    Error_Handler();
  }
  
}

#if defined(ADC_TRIGGER_FROM_TIMER)
/**
  * @brief  TIM configuration
  * @param  None
  * @retval None
  */
static void TIM_Config(void)
{
  TIM_MasterConfigTypeDef master_timer_config;
  RCC_ClkInitTypeDef clk_init_struct = {0};       /* Temporary variable to retrieve RCC clock configuration */
  uint32_t latency;                               /* Temporary variable to retrieve Flash Latency */
  
  uint32_t timer_clock_frequency = 0;             /* Timer clock frequency */
  uint32_t timer_prescaler = 0;                   /* Time base prescaler to have timebase aligned on minimum frequency possible */
  
  /* Configuration of timer as time base:                                     */ 
  /* Caution: Computation of frequency is done for a timer instance on APB1   */
  /*          (clocked by PCLK1)                                              */
  /* Timer frequency is configured from the following constants:              */
  /* - TIMER_FREQUENCY: timer frequency (unit: Hz).                           */
  /* - TIMER_FREQUENCY_RANGE_MIN: timer minimum frequency possible            */
  /*   (unit: Hz).                                                            */
  /* Note: Refer to comments at these literals definition for more details.   */
  
  /* Retrieve timer clock source frequency */
  HAL_RCC_GetClockConfig(&clk_init_struct, &latency);
  /* If APB1 prescaler is different of 1, timers have a factor x2 on their    */
  /* clock source.                                                            */
  if (clk_init_struct.APB1CLKDivider == RCC_HCLK_DIV1)
  {
    timer_clock_frequency = HAL_RCC_GetPCLK1Freq();
  }
  else
  {
    timer_clock_frequency = HAL_RCC_GetPCLK1Freq() *2;
  }
  
  /* Timer prescaler calculation */
  /* (computation for timer 16 bits, additional + 1 to round the prescaler up) */
  timer_prescaler = (timer_clock_frequency / (TIMER_PRESCALER_MAX_VALUE * TIMER_FREQUENCY_RANGE_MIN)) +1;
  
  /* Set timer instance */
  TimHandle.Instance = TIMx;
  
  /* Configure timer parameters */
  TimHandle.Init.Period            = ((timer_clock_frequency / (timer_prescaler * TIMER_FREQUENCY)) - 1);
  TimHandle.Init.Prescaler         = (timer_prescaler - 1);
  TimHandle.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
  TimHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
  TimHandle.Init.RepetitionCounter = 0x0;
  
  if (HAL_TIM_Base_Init(&TimHandle) != HAL_OK)
  {
    /* Timer initialization Error */
    Error_Handler();
  }

  /* Timer TRGO selection */
  master_timer_config.MasterOutputTrigger = TIM_TRGO_UPDATE;
  master_timer_config.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  master_timer_config.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;

  if (HAL_TIMEx_MasterConfigSynchronization(&TimHandle, &master_timer_config) != HAL_OK)
  {
    /* Timer TRGO selection Error */
    Error_Handler();
  }
  
}
#endif /* ADC_TRIGGER_FROM_TIMER */

#if defined(WAVEFORM_VOLTAGE_GENERATION_FOR_TEST)
/**
  * @brief  For this example, generate a waveform voltage on a spare DAC
  *         channel, so user has just to connect a wire between DAC channel 
  *         (pin PF.07) and ADC channel (pin PF.07) to run this example.
  *         (this prevents the user from resorting to an external signal generator)
  *         This function configures the DAC and generates a constant voltage of Vdda/2.
  *         To modify the voltage level, use function "WaveformVoltageGenerationForTest_Update"
  * @param  None
  * @retval None
  */
static void WaveformVoltageGenerationForTest_Config(void)
{
  static DAC_ChannelConfTypeDef sConfig;

  /*## Configure peripherals #################################################*/
  /* Configuration of DACx peripheral */
  DacHandle.Instance = DACx;

  if (HAL_DAC_Init(&DacHandle) != HAL_OK)
  {
    /* DAC initialization error */
    Error_Handler();
  }

  /* Configuration of DAC channel */
  sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;

  if (HAL_DAC_ConfigChannel(&DacHandle, &sConfig, DACx_CHANNEL_TO_ADCx_CHANNELa) != HAL_OK)
  {
    /* Channel configuration error */
    Error_Handler();
  }
  
  /*## Enable peripherals ####################################################*/
  
  /* Set DAC Channel data register: channel corresponding to ADC channel CHANNELa */
  /* Set DAC output to 1/2 of full range (4095 <=> Vdda=3.3V): 2048 <=> 1.65V */
  if (HAL_DAC_SetValue(&DacHandle, DACx_CHANNEL_TO_ADCx_CHANNELa, DAC_ALIGN_12B_R, RANGE_12BITS/2) != HAL_OK)
  {
    /* Setting value Error */
    Error_Handler();
  }
  
  /* Enable DAC Channel: channel corresponding to ADC channel CHANNELa */
  if (HAL_DAC_Start(&DacHandle, DACx_CHANNEL_TO_ADCx_CHANNELa) != HAL_OK)
  {
    /* Start Error */
    Error_Handler();
  }

}

/**
  * @brief  For this example, generate a waveform voltage on a spare DAC
  *         channel, so user has just to connect a wire between DAC channel 
  *         (pin PF.07) and ADC channel (pin PF.07) to run this example.
  *         (this prevents the user from resorting to an external signal generator)
  *         This function modifies the voltage level  from 0V to Vdda in 4 steps,
  *         incrementally at each function call.
  *         Circular waveform of ramp: When the maximum level is reaches,
  *         restart from 0V.
  * @param  None
  * @retval None
  */
static void WaveformVoltageGenerationForTest_Update(void)
{
  static uint8_t ub_dac_steps_count = 0;      /* Count number of clicks: Incremented after User Button interrupt */
  
  /* Set DAC voltage on channel corresponding to ADCy_CHANNELa              */
  /* in function of user button clicks count.                               */
  /* Set DAC output on 5 voltage levels, successively to:                   */
  /*  - minimum of full range (0 <=> ground 0V)                             */
  /*  - 1/4 of full range (4095 <=> Vdda=3.3V): 1023 <=> 0.825V             */
  /*  - 1/2 of full range (4095 <=> Vdda=3.3V): 2048 <=> 1.65V              */
  /*  - 3/4 of full range (4095 <=> Vdda=3.3V): 3071 <=> 2.475V             */
  /*  - maximum of full range (4095 <=> Vdda=3.3V)                          */
  if (HAL_DAC_SetValue(&DacHandle,
                       DACx_CHANNEL_TO_ADCx_CHANNELa,
                       DAC_ALIGN_12B_R,
                       ((RANGE_12BITS * ub_dac_steps_count) / 4)
                      ) != HAL_OK)
  {
    /* Start Error */
    Error_Handler();
  }
  
  /* Wait for voltage settling time */
  HAL_Delay(1);
  
  /* Manage ub_dac_steps_count to increment it in 4 steps and circularly.   */
  if (ub_dac_steps_count < 4)
  {
    ub_dac_steps_count++;
  }
  else
  {
    ub_dac_steps_count = 0;
  }

}
#endif /* WAVEFORM_VOLTAGE_GENERATION_FOR_TEST */

/**
  * @brief EXTI line detection callbacks
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == BUTTON_TAMPER_PIN)
  {
    /* Set variable to report push button event to main program */
    ubUserButtonClickEvent = SET;
  }
  
}

/**
  * @brief  Conversion complete callback in non blocking mode
  * @param  AdcHandle : ADC handle
  * @note   This example shows a simple way to report end of conversion
  *         and get conversion result. You can add your own implementation.
  * @retval None
  */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *AdcHandle)
{
  uint32_t tmp_index = 0;
  uint32_t tmp_average = 0; /* Variable 32 bits for intermediate processing */
  
  /* When the 2nd half of the buffer is reached, compute these results while  */
  /* the 1st half of the buffer is updated by the ADC and DMA transfers.      */
  SCB_InvalidateDCache_by_Addr ((uint32_t *) &aADCxConvertedValues[ADCCONVERTEDVALUES_BUFFER_SIZE/2], ADCCONVERTEDVALUES_BUFFER_SIZE);

  /* Process average of the 2nd half of the buffer */
  for (tmp_index = 0; tmp_index < (ADCCONVERTEDVALUES_BUFFER_SIZE/2); tmp_index++)
  {
    tmp_average += aADCxConvertedValues[tmp_index + (ADCCONVERTEDVALUES_BUFFER_SIZE/2)];
  }
  tmp_average /= (ADCCONVERTEDVALUES_BUFFER_SIZE/2);
  uhADCxConvertedValue_Regular_Avg_half2 = (uint16_t)tmp_average;
  
  /* Affect pointer to the average of the 2nd half of ADC conversion results  */
  /* table of regular group, channel on rank1.                                */
  puhADCxConvertedValue_Regular_Avg = &uhADCxConvertedValue_Regular_Avg_half2;
  
}

/**
  * @brief  Conversion DMA half-transfer callback in non blocking mode 
  * @param  hadc: ADC handle
  * @retval None
  */
void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc)
{
  uint32_t tmp_index = 0;
  uint32_t tmp_average = 0; /* Variable 32 bits for intermediate processing */
  
  /* When the 1st half of the buffer is reached, compute these results while  */
  /* the 2nd half of the buffer is updated by the ADC and DMA transfers.      */
  SCB_InvalidateDCache_by_Addr ((uint32_t *) &aADCxConvertedValues[0], ADCCONVERTEDVALUES_BUFFER_SIZE);
  
  /* Process average of the 1st half of the buffer */
  for (tmp_index = 0; tmp_index < (ADCCONVERTEDVALUES_BUFFER_SIZE/2); tmp_index++)
  {
    tmp_average += aADCxConvertedValues[tmp_index];
  }
  tmp_average /= (ADCCONVERTEDVALUES_BUFFER_SIZE/2);
  uhADCxConvertedValue_Regular_Avg_half1 = (uint16_t)tmp_average;
  
  /* Affect pointer to the average of the 1st half of ADC conversion results  */
  /* table of regular group, channel on rank1.                                */
  puhADCxConvertedValue_Regular_Avg = &uhADCxConvertedValue_Regular_Avg_half1;
  
}

/**
  * @brief  Injected conversion complete callback in non blocking mode 
  * @param  hadc: ADC handle
  * @retval None
  */
void HAL_ADCEx_InjectedConvCpltCallback(ADC_HandleTypeDef* hadc)
{
  uhADCxConvertedValue_Injected = HAL_ADCEx_InjectedGetValue(hadc, ADC_INJECTED_RANK_1);
}

/**
  * @brief  Analog watchdog callback in non blocking mode. 
  * @param  hadc: ADC handle
  * @retval None
  */

/**
  * @brief  ADC error callback in non blocking mode
  *        (ADC conversion with interruption or transfer by DMA)
  * @param  hadc: ADC handle
  * @retval None
  */
void HAL_ADC_ErrorCallback(ADC_HandleTypeDef *hadc)
{
  /* In case of ADC error, call main error handler */
  Error_Handler();
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* User may add here some code to deal with a potential error */
  
  /* In case of error, LED4 is toggling at a frequency of 1Hz */
  while(1)
  {
    /* Toggle LED4 */
    BSP_LED_Toggle(LED4);
    HAL_Delay(500);
  }
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
  * @}
  */

/**
  * @}
  */

