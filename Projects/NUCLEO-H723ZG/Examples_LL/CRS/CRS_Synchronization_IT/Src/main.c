/**
  ******************************************************************************
  * @file    Examples_LL/CRS/CRS_Synchronization_IT/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to configure Clock Recovery Service in
  *          IT mode through the STM32H7xx CRS LL API.
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

/* Private includes ----------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#define CRS_STATUS_INIT       0
#define CRS_STATUS_SYNCOK     1
#define CRS_STATUS_SYNCWARN   2
#define CRS_STATUS_SYNCMISS   3
#define CRS_STATUS_SYNCERR    4

/* Oscillator time-out values */
#define LSE_TIMEOUT_VALUE     ((uint32_t)5000)  /* 5 s */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

__IO uint8_t ubButtonPress = 0;

/* Variable to save SYNC status*/
__IO uint32_t CRSSyncStatus = CRS_STATUS_INIT;

/* Variable used for Timeout management */
#if (USE_TIMEOUT == 1)
uint32_t Timeout = 0;
#endif /* USE_TIMEOUT */

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void LED_Init(void);
static void UserButton_Init(void);

void     Enable_Sync_Reference_Clock(void);
void     Enable_CRS_Clocks(void);
void     Configure_CRS(uint32_t SyncSource);
void     MCO_ConfigGPIO(void);
void     LED_On(void);
void     LED_Off(void);
void     LED_Blinking(uint32_t Period);
void     WaitForUserButtonPress(void);


/* Private user code ---------------------------------------------------------*/


/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  uint32_t felim = 0;

  /* Configure the system clock to 520 MHz*/
  SystemClock_Config();

  /* Initialize LED1 */
  LED_Init();
  
  /* Initialize USER BUTTON in EXTI mode */
  UserButton_Init();

  /* Enable the reference clock used for synchronization (LSE) */
  Enable_Sync_Reference_Clock();

  /* Enable HSI48 and CRS */
  Enable_CRS_Clocks();

  /* For monitoring output HSI48 MCO1 pin(PA8) */
  MCO_ConfigGPIO();

  /* Wait for User push-button press to start transfer */
  WaitForUserButtonPress();

  /* Configure CRS with LSE parameters                                */
  /* but set the source to external via GPIO (which is not connected) */
  Configure_CRS(LL_CRS_SYNC_SOURCE_GPIO);

  /* Simulate a SYNC EVENT which will raise a SYNC_MISS EVENT */
  /* because no external sync clock is connected to GPIO      */
  LL_CRS_GenerateEvent_SWSYNC();

  /* Infinite loop */
  while (1)
  {
    switch (CRSSyncStatus)
    {
      case CRS_STATUS_SYNCOK:
        /* Power on LED1 */
        LED_On();
        /* For visual comfort on LED1 apply delay */
        LL_mDelay(500);
        /* Enable CRS SYNCWARN interrupt (in case it goes out of sync) */
        LL_CRS_EnableIT_SYNCWARN();
        break;

      case CRS_STATUS_SYNCMISS:
        /* Power off LED1 */
        LED_Off();
        /* Perform a new configuration to LSE when SYNC MISS is detected. */
        /* For visual comfort on LED1 apply delay before new configuration */
        LL_mDelay(1000);

        /* Change source to LSE and restart synchronization if necessary */
        if (LL_CRS_GetSyncSignalSource() != LL_CRS_SYNC_SOURCE_LSE)
        {
          /* CRS configuration can be changed only if frequency error is disabled */
          LL_CRS_DisableFreqErrorCounter();

          /* Change CRS SYNC signal source to LSE */
          LL_CRS_SetSyncSignalSource(LL_CRS_SYNC_SOURCE_LSE);

          /* Enable Frequency error counter */
          LL_CRS_EnableFreqErrorCounter();
        }
        else
        {
          /* Enable CRS SYNCOK interrupt */
          LL_CRS_EnableIT_SYNCOK();
        }
        break;

      case CRS_STATUS_SYNCWARN:
        /* Toggle LED1 when SYNC WARN is detected. */
        LED_On();
        LL_mDelay(100);
        LED_Off();
        LL_mDelay(100);
        LED_On();
        LL_mDelay(100);
        LED_Off();
        LL_mDelay(100);
        LED_On();
        LL_mDelay(100);
        LED_Off();
        LL_mDelay(100);
        /* Increase tolerance (FELIM value) until no more warning */
        LL_CRS_DisableFreqErrorCounter();
        felim = LL_CRS_GetFreqErrorLimit();
        LL_CRS_SetFreqErrorLimit(felim + 1);
        LL_CRS_EnableFreqErrorCounter();
        /* Enable CRS SYNCWARN interrupt (to allow resync) */
        LL_CRS_EnableIT_SYNCWARN();
        /* Enable CRS SYNCOK interrupt (if it was in SYNCOK but then in WARN again) */
        LL_CRS_EnableIT_SYNCOK();
        break;

      case CRS_STATUS_SYNCERR:
        /* Power off LED1 */
        LED_Off();
        break;

      default:
        break;
    }

  }

}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL1 (HSE BYPASS)
  *            SYSCLK(Hz)                     = 520000000 (CPU Clock)
  *            HCLK(Hz)                       = 260000000 (AXI and AHBs Clock)
  *            AHB Prescaler                  = 2
  *            D1 APB3 Prescaler              = 2 (APB3 Clock  130MHz)
  *            D2 APB1 Prescaler              = 2 (APB1 Clock  130MHz)
  *            D2 APB2 Prescaler              = 2 (APB2 Clock  130MHz)
  *            D3 APB4 Prescaler              = 2 (APB4 Clock  130MHz)
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 4
  *            PLL_N                          = 260
  *            PLL_P                          = 1
  *            PLL_Q                          = 4
  *            PLL_R                          = 2
  *            VDD(V)                         = 3.3
  *            Flash Latency(WS)              = 3
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE0);
  while (LL_PWR_IsActiveFlag_VOS() == 0)
  {
  }

  /* Enable HSE oscillator */
  LL_RCC_HSE_EnableBypass();
  LL_RCC_HSE_Enable();
  while(LL_RCC_HSE_IsReady() != 1)
  {
  }

  /* Set FLASH latency */
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_3);

  /* Main PLL configuration and activation */
  LL_RCC_PLL_SetSource(LL_RCC_PLLSOURCE_HSE);
  LL_RCC_PLL1P_Enable();
  LL_RCC_PLL1Q_Enable();
  LL_RCC_PLL1R_Enable();
  LL_RCC_PLL1FRACN_Disable();
  LL_RCC_PLL1_SetVCOInputRange(LL_RCC_PLLINPUTRANGE_2_4);
  LL_RCC_PLL1_SetVCOOutputRange(LL_RCC_PLLVCORANGE_WIDE);
  LL_RCC_PLL1_SetM(4);
  LL_RCC_PLL1_SetN(260);
  LL_RCC_PLL1_SetP(1);
  LL_RCC_PLL1_SetQ(4);
  LL_RCC_PLL1_SetR(2);
  LL_RCC_PLL1_Enable();
  while(LL_RCC_PLL1_IsReady() != 1)
  {
  }

  /* Set Sys & AHB & APB1 & APB2 & APB4  prescaler */
  LL_RCC_SetSysPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAHBPrescaler(LL_RCC_AHB_DIV_2);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_2);
  LL_RCC_SetAPB4Prescaler(LL_RCC_APB4_DIV_2);

  /* Set PLL1 as System Clock Source */
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL1);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL1)
  {
  }

  /* Set systick to 1ms */
  SysTick_Config(520000000 / 1000);

  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  SystemCoreClock = 520000000;
}


/**
  * @brief LED Initialization Function
  * @param None
  * @retval None
  */
static void LED_Init(void)
{
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* LED GPIO Port Clock Enable */
  LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOB);

  /* Reset GPIO for LED1*/
  LL_GPIO_ResetOutputPin(LED1_GPIO_Port, LED1_Pin);

  /* Configure GPIO for LED1 */
  GPIO_InitStruct.Pin = LED1_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(LED1_GPIO_Port, &GPIO_InitStruct);
  
}


/**
  * @brief Usr Button Initialization Function
  * @param None
  * @retval None
  */
static void UserButton_Init(void)
{
  LL_EXTI_InitTypeDef EXTI_InitStruct = {0};

  /* USER BUTTON GPIO Port Clock Enable */
  LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOC);
  
  /* SYSCFG Clock Enable */
  LL_APB4_GRP1_EnableClock(LL_APB4_GRP1_PERIPH_SYSCFG);

  /* Connect External Line to the USER BUTTON GPIO */
  LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTC, LL_SYSCFG_EXTI_LINE13);

  /* Configure EXTI for USER BUTTON */
  EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_13;
  EXTI_InitStruct.LineCommand = ENABLE;
  EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
  EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_FALLING;
  LL_EXTI_Init(&EXTI_InitStruct);

  /* Configure GPIO for USER BUTTON */
  LL_GPIO_SetPinPull(USER_BUTTON_GPIO_Port, USER_BUTTON_Pin, LL_GPIO_PULL_NO);
  LL_GPIO_SetPinMode(USER_BUTTON_GPIO_Port, USER_BUTTON_Pin, LL_GPIO_MODE_INPUT);

  /* EXTI interrupt init*/
  NVIC_SetPriority(EXTI15_10_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),15, 0));
  NVIC_EnableIRQ(EXTI15_10_IRQn);

}


/**
  * @brief  This function enables the reference clock used for sync (LSE)
  * @param  None
  * @retval None
  */
void Enable_Sync_Reference_Clock(void)
{
  /* To enable LSE, before it is necessary to:
     - Reset the Back up Domain */
  LL_PWR_EnableBkUpAccess();

  /* Enable LSE only if disabled.*/
  if (LL_RCC_LSE_IsReady() == 0)
  {
    LL_RCC_ForceBackupDomainReset();
    LL_RCC_ReleaseBackupDomainReset();
    LL_RCC_LSE_Enable();
#if (USE_TIMEOUT == 1)
    Timeout = LSE_TIMEOUT_VALUE;
#endif /* USE_TIMEOUT */
    while (LL_RCC_LSE_IsReady() != 1)
    {
#if (USE_TIMEOUT == 1)
      if (LL_SYSTICK_IsActiveCounterFlag())
      {
        Timeout --;
      }
      if (Timeout == 0)
      {
        /* LSE activation error */
        LED_Blinking(LED_BLINK_ERROR);
      }
#endif /* USE_TIMEOUT */
    }
  }
}

/**
  * @brief  This function enables CRS and HSI48 clocks
  * @param  None
  * @retval None
  */
void Enable_CRS_Clocks(void)
{
  /* CRS initialization (enable HSI48 oscillator and enable CRS clock) */

  /* Enable CRS clock*/
  LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_CRS);


  /* Enable HSI48 only if disabled.*/
  if (LL_RCC_HSI48_IsReady() == 0)
  {
    LL_RCC_HSI48_Enable();
#if (USE_TIMEOUT == 1)
    Timeout = LSE_TIMEOUT_VALUE;
#endif /* USE_TIMEOUT */
    while (LL_RCC_HSI48_IsReady() != 1)
    {
#if (USE_TIMEOUT == 1)
      if (LL_SYSTICK_IsActiveCounterFlag())
      {
        Timeout --;
      }
      if (Timeout == 0)
      {
        /* HSI48 activation error */
        LED_Blinking(LED_BLINK_ERROR);
      }
#endif /* USE_TIMEOUT */
    }
  }
}

/**
  * @brief  This function configures CRS
  * @param  None
  * @retval None
  */
void Configure_CRS(uint32_t SyncSource)
{
  /* Enable and set CRS interrupt*/
  NVIC_SetPriority(CRS_IRQn, 0x03);
  NVIC_EnableIRQ(CRS_IRQn);

  /* HSI48 Synchronization:
     - HSI trimming value set to 0x00 to see the automatic calibration performed by CRS
     - Reload value calculated like this F(LSE)=32.768kHz then Reload=(f(Target)/f(LSE))-1= 0x5B7
     - Felim value calculated like this FELIM = (fTARGET / fSYNC) * STEP[%] / 100% / 2 with STEP=0.14% then FELIM = 2
  */
  LL_CRS_ConfigSynchronization(0x00,
                               2,
                               __LL_CRS_CALC_CALCULATE_RELOADVALUE(HSI48_VALUE, LSE_VALUE),
                               LL_CRS_SYNC_DIV_1 | SyncSource | LL_CRS_SYNC_POLARITY_FALLING
                              );

  /* Enable Automatic trimming */
  LL_CRS_EnableAutoTrimming();

  /* Enable Frequency error counter */
  LL_CRS_EnableFreqErrorCounter();

  /* Enable CRS interrupts */
  /* Each interruption can be enabled independently in using following functions:
  - LL_CRS_EnableIT_SYNCOK();
  - LL_CRS_EnableIT_SYNCWARN();
  - LL_CRS_EnableIT_ERR();
  - LL_CRS_EnableIT_ESYNC();
  */
  LL_CRS_WriteReg(CRS, CR, LL_CRS_ReadReg(CRS, CR) | LL_CRS_CR_SYNCOKIE | LL_CRS_CR_SYNCWARNIE | LL_CRS_CR_ERRIE | LL_CRS_CR_ESYNCIE);
}

/**
  * @brief  Configure MCO1 pin (PA8).
  * @param  None
  * @retval None
  */
void MCO_ConfigGPIO(void)
{
  /* MCO1 Clock Enable */
  LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOA);

  /* Configure the MCO1 pin in alternate function mode */
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_8, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_8, LL_GPIO_OUTPUT_PUSHPULL);
  LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_8, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_8, LL_GPIO_PULL_NO);
  LL_GPIO_SetAFPin_8_15(GPIOA, LL_GPIO_PIN_8, LL_GPIO_AF_0);

  /* Select MCO1 clock source and prescaler */
  LL_RCC_ConfigMCO(LL_RCC_MCO1SOURCE_HSI48, LL_RCC_MCO1_DIV_1);
}

/**
  * @brief  Turn-on LED1.
  * @param  None
  * @retval None
  */
void LED_On(void)
{
  /* Turn LED1 on */
  LL_GPIO_SetOutputPin(LED1_GPIO_Port, LED1_Pin);

}

/**
  * @brief  Turn-off LED1.
  * @param  None
  * @retval None
  */
void LED_Off(void)
{
  /* Turn LED1 off */
  LL_GPIO_ResetOutputPin(LED1_GPIO_Port, LED1_Pin);
}

/**
  * @brief  Set LED1 to Blinking mode for an infinite loop (toggle period based on value provided as input parameter).
  * @param  Period : Period of time (in ms) between each toggling of LED
  *   This parameter can be user defined values. Pre-defined values used in that example are :
  *     @arg LED_BLINK_FAST : Fast Blinking
  *     @arg LED_BLINK_SLOW : Slow Blinking
  *     @arg LED_BLINK_ERROR : Error specific Blinking
  * @retval None
  */
void LED_Blinking(uint32_t Period)
{
  /* Toggle IO in an infinite loop */
  while (1)
  {
    LL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
    LL_mDelay(Period);
  }
}

/**
  * @brief  Wait for User push-button press to start transfer.
  * @param  None
  * @retval None
  */
/*  */
void WaitForUserButtonPress(void)
{
  while (ubButtonPress == 0)
  {
    LL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
    LL_mDelay(LED_BLINK_FAST);
  }
  /* Ensure that LED1 is turned Off */
  LED_Off();
}


/******************************************************************************/
/*   USER IRQ HANDLER TREATMENT                                               */
/******************************************************************************/
/**
  * @brief  Function to manage User button callback interruption
  * @param  None
  * @retval None
  */
void UserButton_Callback(void)
{
  /* Update User push-button variable : to be checked in waiting loop in main program */
  ubButtonPress = 1;
}

/**
  * @brief  Handle CRS interruption
  * @param  none
  * @retval none
  */
void CRS_Handler_Callback(void)
{
  /* Check CRS SYNCOK flag  */
  if (LL_CRS_IsActiveFlag_SYNCOK() && LL_CRS_IsEnabledIT_SYNCOK())
  {
    CRSSyncStatus = CRS_STATUS_SYNCOK;

    /* Disable CRS SYNCOK interrupt since synchronization succeeded */
    LL_CRS_DisableIT_SYNCOK();

    /* Clear CRS SYNC event OK bit */
    LL_CRS_ClearFlag_SYNCOK();
  }

  /* Check CRS SYNCWARN flag  */
  if (LL_CRS_IsActiveFlag_SYNCWARN() && LL_CRS_IsEnabledIT_SYNCWARN())
  {
    CRSSyncStatus = CRS_STATUS_SYNCWARN;

    /* Disable temporary CRS SYNCWARN interrupt to let the main loop to be execute */
    LL_CRS_DisableIT_SYNCWARN();

    /* Clear CRS SYNCWARN bit */
    LL_CRS_ClearFlag_SYNCWARN();
  }

  /* Check CRS SYNCERR flag  */
  if (LL_CRS_IsActiveFlag_SYNCERR() && LL_CRS_IsEnabledIT_ERR())
  {
    CRSSyncStatus = CRS_STATUS_SYNCERR;

    /* Clear CRS Error bit */
    LL_CRS_ClearFlag_ERR();
  }

  /* Check CRS SYNC Missed flag  */
  if (LL_CRS_IsActiveFlag_SYNCMISS() && LL_CRS_IsEnabledIT_ERR())
  {
    CRSSyncStatus = CRS_STATUS_SYNCMISS;

    /* Clear CRS SYNC Missed bit */
    LL_CRS_ClearFlag_ERR();
  }

  /* Check CRS Expected SYNC flag  */
  if (LL_CRS_IsActiveFlag_ESYNC() && LL_CRS_IsEnabledIT_ESYNC())
  {
    /* frequency error counter reached a zero value */
    LL_CRS_ClearFlag_ESYNC();
  }
}



/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{

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
     ex: printf("Wrong parameters value: file %s on line %d", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif /* USE_FULL_ASSERT */

