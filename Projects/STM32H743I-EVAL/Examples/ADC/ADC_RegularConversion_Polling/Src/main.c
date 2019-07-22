/**
  ******************************************************************************
  * @file    ADC/ADC_RegularConversion_Polling/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to use Polling mode to convert data.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32H7xx_HAL_Examples
  * @{
  */

/** @addtogroup ADC_RegularConversion_Polling
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* ADC handler declaration */
ADC_HandleTypeDef    AdcHandle;

/* Variable used to get converted value */
__IO uint16_t uhADCxConvertedValue = 0;

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void CPU_CACHE_Enable(void);
static void Error_Handler(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
  ADC_ChannelConfTypeDef sConfig;
  
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

  /* Configure LED3 */
  BSP_LED_Init(LED3);

  /*##-1- Configure the ADC peripheral #######################################*/
  AdcHandle.Instance          = ADCx;
  
  if (HAL_ADC_DeInit(&AdcHandle) != HAL_OK)
  {
    /* ADC de-initialization Error */
    Error_Handler();
  }
  


  AdcHandle.Init.ClockPrescaler           = ADC_CLOCK_ASYNC_DIV2;          /* Asynchronous clock mode, input ADC clock divided by 2*/
  AdcHandle.Init.Resolution               = ADC_RESOLUTION_16B;            /* 16-bit resolution for converted data */
  AdcHandle.Init.ScanConvMode             = DISABLE;                       /* Sequencer disabled (ADC conversion on only 1 channel: channel set on rank 1) */
  AdcHandle.Init.EOCSelection             = ADC_EOC_SINGLE_CONV;           /* EOC flag picked-up to indicate conversion end */
  AdcHandle.Init.LowPowerAutoWait         = DISABLE;                       /* Auto-delayed conversion feature disabled */
  AdcHandle.Init.ContinuousConvMode       = DISABLE;                       /* Continuous mode disabled to have only 1 conversion at each conversion trig */
  AdcHandle.Init.NbrOfConversion          = 1;                             /* Parameter discarded because sequencer is disabled */
  AdcHandle.Init.DiscontinuousConvMode    = DISABLE;                       /* Parameter discarded because sequencer is disabled */
  AdcHandle.Init.NbrOfDiscConversion      = 1;                             /* Parameter discarded because sequencer is disabled */
  AdcHandle.Init.ExternalTrigConv         = ADC_SOFTWARE_START;            /* Software start to trig the 1st conversion manually, without external event */
  AdcHandle.Init.ExternalTrigConvEdge     = ADC_EXTERNALTRIGCONVEDGE_NONE; /* Parameter discarded because software trigger chosen */
  AdcHandle.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DR;         /* Regular Conversion data stored in DR register only */
  AdcHandle.Init.Overrun                  = ADC_OVR_DATA_OVERWRITTEN;      /* DR register is overwritten with the last conversion result in case of overrun */
  AdcHandle.Init.OversamplingMode         = DISABLE;                       /* No oversampling */

  if (HAL_ADC_Init(&AdcHandle) != HAL_OK)
  {
    /* ADC initialization Error */
    Error_Handler();
  }

  /*##-2- Configure ADC regular channel ######################################*/
  sConfig.Channel      = ADCx_CHANNEL;                /* Sampled channel number */
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

  /* Run the ADC calibration in single-ended mode */
  if (HAL_ADCEx_Calibration_Start(&AdcHandle, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED) != HAL_OK)
  {
    /* Calibration Error */
    Error_Handler();
  }
  
  /*##-3- Start the conversion process #######################################*/
  if (HAL_ADC_Start(&AdcHandle) != HAL_OK)
  {
    /* Start Conversation Error */
    Error_Handler();
  }

  /*##-4- Wait for the end of conversion #####################################*/
  /*  For simplicity reasons, this example is just waiting till the end of the
      conversion, but application may perform other tasks while conversion
      operation is ongoing. */
  if (HAL_ADC_PollForConversion(&AdcHandle, 10) != HAL_OK)
  {
    /* End Of Conversion flag not set on time */
    Error_Handler();
  }
  else
  {
    /* ADC conversion completed */
    /*##-5- Get the converted value of regular channel  ########################*/
    uhADCxConvertedValue = HAL_ADC_GetValue(&AdcHandle);
  }
  
  /* Infinite loop */
  while (1)
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
  
  /*!< Supply configuration update enable */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

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
  while (1)
  {
  }
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
