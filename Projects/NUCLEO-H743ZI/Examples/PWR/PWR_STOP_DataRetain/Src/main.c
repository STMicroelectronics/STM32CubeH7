/**
  ******************************************************************************
  * @file    PWR/PWR_STOP_DataRetain/Src/main.c
  * @author  MCD Application Team
  * @brief   This sample code shows how to retain data in the D3 SRAM
  *          memory when system enters STOP mode and D1 enters DSTANDBY mode.
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

/** @addtogroup PWR_STOP_DataRetain
  * @{
  */

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define LED_TOGGLE_DELAY      100
#define D3SRAM_ADDRESS        0x38000000
#define D3SRAM_DATA           0x30313233
#define D3SRAM_ERASE_DATA     0xFFFFFFFF
#define D3SRAM_SIZE           0x4000 /* 16 KB */
#define PWR_WAKEUP_PIN_FLAGS  (PWR_WAKEUP_FLAG1 | PWR_WAKEUP_FLAG2 | PWR_WAKEUP_FLAG3 | \
                               PWR_WAKEUP_FLAG4 | PWR_WAKEUP_FLAG5 | PWR_WAKEUP_FLAG6)

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
/* RTC handler declaration */
RTC_HandleTypeDef RTCHandle;
static __IO uint32_t TimingDelay;

/* Private function prototypes -----------------------------------------------*/
static void MPU_Config(void);
static void SystemClock_Config(void);
static void RTC_Config(void);
static void SRAM_WriteData(void);
static void SRAM_CheckData(void);
static void CPU_CACHE_Clean(void);
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

  /* Configure LED1, LED2 */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);

  /* Check and handle if D1 domain was resumed from StandBy mode */
  if(__HAL_PWR_GET_FLAG(PWR_FLAG_SB_D1) != RESET)
  {
    /* Check D3SRAM data */
    SRAM_CheckData();

    /* Clear Standby flag */
    __HAL_PWR_CLEAR_FLAG(PWR_CPU_FLAGS);
  }

  /* Configure RTC */
  RTC_Config();

  /* Insert 3 seconds delay */
  HAL_Delay(3000);

  /* The Following Wakeup sequence is highly recommended prior to each Standby mode entry
    mainly  when using more than one wakeup source this is to not miss any wakeup event.
    - Disable all used wakeup sources,
    - Clear all related wakeup flags,
    - Re-enable all used wakeup sources,
    - Enter the Standby mode.
  */
  /* Disable all used wakeup sources*/
  HAL_RTCEx_DeactivateWakeUpTimer(&RTCHandle);

  /* Clear all related wakeup flags */
  HAL_PWREx_ClearWakeupFlag(PWR_WAKEUP_PIN_FLAGS);

  /* Re-enable all used wakeup sources*/
  /* ## Setting the Wake up time ############################################*/
  /* RTC Wakeup Interrupt Generation:
    the wake-up counter is set to its maximum value to yield the longuest
    stand-by time to let the current reach its lowest operating point.
    The maximum value is 0xFFFF, corresponding to about 33 sec. when
    RTC_WAKEUPCLOCK_RTCCLK_DIV = RTCCLK_Div16 = 16

    Wakeup Time Base = (RTC_WAKEUPCLOCK_RTCCLK_DIV /(LSI))
    Wakeup Time = Wakeup Time Base * WakeUpCounter
      = (RTC_WAKEUPCLOCK_RTCCLK_DIV /(LSI)) * WakeUpCounter
      ==> WakeUpCounter = Wakeup Time / Wakeup Time Base

    To configure the wake up timer to 5s the WakeUpCounter is set to 0x2710:
    Wakeup Time Base = 16 /(~32.000KHz) = ~0.5 ms
    Wakeup Time = 0.5 ms  * WakeUpCounter
    Therefore, with wake-up counter =  0x2710  = 10,000
       Wakeup Time =  0,5 ms *  10,000 = 5 sec. */
  HAL_RTCEx_SetWakeUpTimer_IT(&RTCHandle, 0x2710, RTC_WAKEUPCLOCK_RTCCLK_DIV16);

  if(__HAL_PWR_GET_FLAG(PWR_FLAG_SB_D1) == RESET)
  {
    /* Write D3SRAM data */
    SRAM_WriteData();

    /* Before entering Stanby mode make CPU cache clean operation to ensure that data are written in D3SRAM */
    CPU_CACHE_Clean();

    /* Check written data */
    SRAM_CheckData();

    /* Set LED1 Off to inform the user that the system will enter STOP mode */
    BSP_LED_Off(LED1);

    /* Enter D2 Domain to Standby mode */
    HAL_PWREx_EnterSTANDBYMode(PWR_D2_DOMAIN);

    /* Enter D1 to Standby mode. In this case the system will enter STOP mode since D1
       is in DSTANDBY mode, D2 is in DSTANDBY and D3 is in DSTOP mode */
    HAL_PWREx_EnterSTANDBYMode(PWR_D1_DOMAIN);
  }

  while(1)
  {
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE BYPASS)
  *            SYSCLK(Hz)                     = 400000000 (CPU Clock)
  *            HCLK(Hz)                       = 200000000 (AXI and AHBs Clock)
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

  /*!< Supply configuration update enable */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

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

}
/**
  * @brief  RTC Configuration
  *         RTC Clocked by LSI (see HAL_RTC_MspInit)
  * @param  None
  * @retval None
  */
static void RTC_Config(void)
{
  /* Configure RTC */
  RTCHandle.Instance = RTC;

  /* Set the RTC time base to 1s */
  /* Configure RTC prescaler and RTC data registers as follow:
    - Hour Format     = Format 24
    - Asynch Prediv   = Value according to source clock
    - Synch Prediv    = Value according to source clock
    - OutPut          = Output Disable
    - OutPutPolarity  = High Polarity
    - OutPutType      = Open Drain */
  RTCHandle.Init.HourFormat = RTC_HOURFORMAT_24;
  RTCHandle.Init.AsynchPrediv = RTC_ASYNCH_PREDIV;
  RTCHandle.Init.SynchPrediv = RTC_SYNCH_PREDIV;
  RTCHandle.Init.OutPut = RTC_OUTPUT_DISABLE;
  RTCHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  RTCHandle.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if(HAL_RTC_Init(&RTCHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
}

/**
  * @brief  Write data into D3 SRAM memory
  * @param  None
  * @retval None
  */
static void SRAM_WriteData()
{
  uint32_t SRAM_Index;

  /* Erase  D3 SRAM */
  for( SRAM_Index = 0; SRAM_Index < D3SRAM_SIZE ; SRAM_Index++)
  {
   *(__IO uint32_t*)(D3SRAM_ADDRESS + (SRAM_Index * 4)) = D3SRAM_ERASE_DATA ;
  }

  /* Fill  D3 SRAM */
  for( SRAM_Index = 0; SRAM_Index < D3SRAM_SIZE ; SRAM_Index++)
  {
   *(__IO uint32_t*)(D3SRAM_ADDRESS + (SRAM_Index * 4)) = D3SRAM_DATA;
  }
}

/**
  * @brief  Check data from D3 SRAM memory
  * @param  None
  * @retval None
  */
static void SRAM_CheckData()
{
  uint32_t SRAM_Index;

  /* Chedk  D3 SRAM data */
  for( SRAM_Index = 0; SRAM_Index < D3SRAM_SIZE ; SRAM_Index++)
  {
    if ( *(__IO uint32_t*)(D3SRAM_ADDRESS + (SRAM_Index * 4)) != D3SRAM_DATA)
    {
      Error_Handler();
    }
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
  /* Turn LED2 on */
  BSP_LED_On(LED2);
  while(1)
  {
  }
}

/**
* @brief  CPU L1-Cache clean.
* @param  None
* @retval None
*/
static void CPU_CACHE_Clean(void)
{
  /* Clean D-Cache */
  SCB_CleanDCache();
}

/**
  * @brief SYSTICK callback
  * @param None
  * @retval None
  */
void HAL_SYSTICK_Callback(void)
{
  HAL_IncTick();
  if (TimingDelay != 0)
  {
    TimingDelay--;
  }
  else
  {
    /* Toggle LED1 */
    BSP_LED_Toggle(LED1);
    TimingDelay = LED_TOGGLE_DELAY;
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
  * @}
  */

/**
  * @}
  */

