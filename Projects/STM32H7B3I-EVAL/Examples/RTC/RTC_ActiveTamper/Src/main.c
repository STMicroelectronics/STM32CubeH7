/**
  ******************************************************************************
  * @file    RTC/RTC_ActiveTamper/Src/main.c
  * @author  MCD Application Team
  * @brief   This sample code shows how to use stm32h7xxxx RTC HAL API to 
  *          program the Active Tampers.
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

/** @addtogroup RTC_ActiveTamper
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint32_t TampIntStatus;
RTC_HandleTypeDef RTCHandle;

/* Private function prototypes -----------------------------------------------*/
static void MPU_Config(void);
static void SystemClock_Config(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  RTC_ActiveTampersTypeDef sAllTamper;
  uint32_t Seed[4] = {0xFEFEFEFEu, 0xAAAAAAAAu, 0x0u, 0xFFFF0000u};
  uint32_t i;

  /* Configure the MPU attributes */
  MPU_Config();

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

  /* Configure LED1 */
  BSP_LED_Init(LED1);

  /* Uncomment to freeze the RTC during debug */
  //__HAL_DBGMCU_FREEZE_RTC();

  /* Initialise the RTC */
  RTCHandle.Init.HourFormat     = RTC_HOURFORMAT_24;
  RTCHandle.Init.AsynchPrediv   = RTC_ASYNCH_PREDIV;
  RTCHandle.Init.SynchPrediv    = RTC_SYNCH_PREDIV;
  RTCHandle.Init.OutPut         = RTC_OUTPUT_DISABLE;
  RTCHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  RTCHandle.Init.OutPutType     = RTC_OUTPUT_TYPE_OPENDRAIN;
  RTCHandle.Instance            = RTC;
  if (HAL_RTC_Init(&RTCHandle) != HAL_OK)
  {
    Error_Handler();
  }

  /* Set all backup registers to 0xFFFFFFFF.
     Backup registers will be cleared by a tamper detection. */
  for (i = 0; i < RTC_BKP_NUMBER; i++)
  {
    HAL_RTCEx_BKUPWrite(&RTCHandle, i, 0xFFFFFFFF);
  }

  /* Configure active tamper common parameters  */
  sAllTamper.ActiveFilter = RTC_ATAMP_FILTER_ENABLE;
  sAllTamper.ActiveAsyncPrescaler = RTC_ATAMP_ASYNCPRES_RTCCLK_32;
  sAllTamper.TimeStampOnTamperDetection = RTC_TIMESTAMPONTAMPERDETECTION_ENABLE;
  sAllTamper.ActiveOutputChangePeriod = 4;
  sAllTamper.Seed[0] = Seed[0];
  sAllTamper.Seed[1] = Seed[1];
  sAllTamper.Seed[2] = Seed[2];
  sAllTamper.Seed[3] = Seed[3];

  /* Disable all Actives Tampers */
  for (i = 0; i < RTC_TAMP_NB; i++)
  {
    sAllTamper.TampInput[i].Enable = RTC_ATAMP_DISABLE;
    sAllTamper.TampInput[i].NoErase = RTC_TAMPER_ERASE_BACKUP_ENABLE;
    sAllTamper.TampInput[i].MaskFlag = RTC_TAMPERMASK_FLAG_DISABLE;
  }

  /* Configure and enable Active TAMP 2 In */
  sAllTamper.TampInput[RTC_ATAMP_2].Enable = RTC_ATAMP_ENABLE;
  sAllTamper.TampInput[RTC_ATAMP_2].Interrupt = RTC_ATAMP_INTERRUPT_ENABLE;
  sAllTamper.TampInput[RTC_ATAMP_2].Output = RTC_ATAMP_2;

  /* Reset the tamper interrupt status flag */
  TampIntStatus = 0x00u;

  /* Set active tampers */
  if (HAL_RTCEx_SetActiveTampers(&RTCHandle, &sAllTamper) != HAL_OK)
  {
    Error_Handler();
  }

  HAL_Delay(200);

  /* Check if Tamper is not detected */
  for (i = 0; i < RTC_BKP_NUMBER; i++)
  {
    if (HAL_RTCEx_BKUPRead(&RTCHandle, i) == 0x0u)
    {
      /* Please verify that All Tampers In and the Tamper Out are linked together */
      Error_Handler();
    }
  }

  /* Update the seed with "random" values */
  for (i = 0; i < 5; i++)
  {
    Seed[0] -= 0xFEFEFEu * i;
    Seed[1] += 0xAAAu;
    Seed[2] += Seed[0] - Seed[1];
    Seed[3] += 4 * Seed[0] + 0x123456u;

    if (HAL_RTCEx_SetActiveSeed(&RTCHandle, Seed) != HAL_OK)
    {
       Error_Handler();
    }

    HAL_Delay(100);
  }

  /* Check if Tamper is not detected */
  for (i = 0; i < RTC_BKP_NUMBER; i++)
  {
    if (HAL_RTCEx_BKUPRead(&RTCHandle, i) == 0)
    {
       Error_Handler();
    }
  }

  /* Wait for tamper detection. User must disconnect a wire */
  while (TampIntStatus == 0x00u);

  /* Disable Active Tampers */
  if (HAL_RTCEx_DeactivateActiveTampers(&RTCHandle) != HAL_OK)
  {
    Error_Handler();
  }

  /* Check that Backup registers have been cleared by the tamper detection */
  for (i = 0; i < RTC_BKP_NUMBER; i++)
  {
    if (HAL_RTCEx_BKUPRead(&RTCHandle, i) != 0)
    {
      Error_Handler();
    }
  }

  /* The test is OK */
  BSP_LED_On(LED1);
  while (1);
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
    while(1) {};
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
    while(1) {};
  }
}


/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  while (1)
  {
    /* Toggle the LED1 */
    BSP_LED_Toggle(LED1);
    HAL_Delay(100);
  }
}

/**
  * @brief  HAL Callbacks
  * @param  None
  * @retval None
  */
void HAL_RTCEx_Tamper2EventCallback(RTC_HandleTypeDef *RTCHandle)
{
  TampIntStatus |= TAMP_MISR_TAMP2MF;
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

