/**
  ******************************************************************************
  * @file    Examples_LL/WWDG/WWDG_RefreshUntilUserEvent/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to configure WWDG down-counter (with Window)
  *          using the STM32H7xx WWDG LL API.
  *          Peripheral initialization done using LL unitary services functions.
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

/** @addtogroup STM32H7xx_LL_Examples
  * @{
  */

/** @addtogroup WWDG_RefreshUntilUserEvent
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static __IO uint8_t KeyPressed = 0;
/* Private function prototypes -----------------------------------------------*/
static void  SystemClock_Config(void);
void     Check_WWDG_Reset(void);
void     Configure_WWDG(void);
void     LED_Init(void);
void     LED_On(void);
void     UserButton_Init(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* Configure the system clock */
  SystemClock_Config();
  
  /* Initialize LED1 */
  LED_Init();

  /* Initialize button in EXTI mode */
  UserButton_Init();
  
  /* Check if the system has resumed from WWDG reset*/
  Check_WWDG_Reset();
  
  /* Configure the WWDG                    */
  /*   - Configure the Window to ~2s       */
  /*   - Configure the Prescaler           */
  /*   - Configure the Downcounter to ~2s  */
  /*   - Enable the WWDG                   */
  Configure_WWDG();

  /* Infinite loop */
  while (1)
  {
    if (1 != KeyPressed)
    {
      /* Refresh WWDG Downcounter to initial value ~2s*/
      LL_WWDG_SetCounter(WWDG1, 0x7E);
      
      LL_GPIO_TogglePin(LED1_GPIO_PORT, LED1_PIN);
      LL_mDelay(LED_BLINK_FAST);    
    }
  }
}

/**
  * @brief  This function configures WWDG
  * @param  None
  * @retval None
  */
void Configure_WWDG(void)
{
  /* Enable system wide reset for Window Watch Dog 1 */
  LL_RCC_WWDG1_EnableSystemReset();

  /* Enable the peripheral clock of DBG register (uncomment for debug purpose) */
  /* LL_DBGMCU_APB3_GRP1_FreezePeriph(LL_DBGMCU_APB3_GRP1_WWDG1_STOP); */
  
  /* Enable the peripheral clock WWDG */
  LL_APB3_GRP1_EnableClock(LL_APB3_GRP1_PERIPH_WWDG1);

  /* Configure WWDG */
  /* (1) set prescaler to have a rollover each about ~2s */
  /* (2) set window value to same value (~2s) as downcounter in order to ba able to refresh the WWDG almost immediately */
  /* (3) Refresh WWDG before activate it */
  /* (4) Activate WWDG */
  LL_WWDG_SetPrescaler(WWDG1, LL_WWDG_PRESCALER_8); /* (1) */
  LL_WWDG_SetWindow(WWDG1,0x7E);                    /* (2) */
  LL_WWDG_SetCounter(WWDG1, 0x7E);                  /* (3) */
  LL_WWDG_Enable(WWDG1);                            /* (4) */
}

/**
  * @brief  This function check if the system has resumed from WWDG reset
  * @param  None
  * @retval None
  */
void Check_WWDG_Reset(void)
{
  if (LL_RCC_IsActiveFlag_WWDG1RST())
  {
    /* clear WWDG reset flag */
    LL_RCC_ClearResetFlags();

    /* turn Led on and wait for user event to perform example again */
    LED_On();
    
    while(KeyPressed != 1)
    {
    }

    /* Reset KeyPressed value */
    KeyPressed = 0;
  }
}

/**
  * @brief  Initialize LED1.
  * @param  None
  * @retval None
  */
void LED_Init(void)
{
  /* Enable the LED1 Clock */
  LED1_GPIO_CLK_ENABLE();

  /* Configure IO in output push-pull mode to drive external LED1 */
  LL_GPIO_SetPinMode(LED1_GPIO_PORT, LED1_PIN, LL_GPIO_MODE_OUTPUT);
  /* Reset value is LL_GPIO_OUTPUT_PUSHPULL */
  //LL_GPIO_SetPinOutputType(LED1_GPIO_PORT, LED1_PIN, LL_GPIO_OUTPUT_PUSHPULL);
  /* Reset value is LL_GPIO_SPEED_FREQ_LOW */
  //LL_GPIO_SetPinSpeed(LED1_GPIO_PORT, LED1_PIN, LL_GPIO_SPEED_FREQ_LOW);
  /* Reset value is LL_GPIO_PULL_NO */
  //LL_GPIO_SetPinPull(LED1_GPIO_PORT, LED1_PIN, LL_GPIO_PULL_NO);
}

/**
  * @brief  Turn-on LED1.
  * @param  None
  * @retval None
  */
void LED_On(void)
{
  /* Turn LED1 on */
  LL_GPIO_SetOutputPin(LED1_GPIO_PORT, LED1_PIN);
}

/**
  * @brief  Configures User push-button in EXTI Line Mode.
  * @param  None
  * @retval None
  */
void UserButton_Init(void)
{
  /* Enable the BUTTON Clock */
  USER_BUTTON_GPIO_CLK_ENABLE();
  
  /* Configure GPIO for BUTTON */
  LL_GPIO_SetPinMode(USER_BUTTON_GPIO_PORT, USER_BUTTON_PIN, LL_GPIO_MODE_INPUT);
  LL_GPIO_SetPinPull(USER_BUTTON_GPIO_PORT, USER_BUTTON_PIN, LL_GPIO_PULL_NO);

  /* Connect External Line to the GPIO*/
  USER_BUTTON_SYSCFG_SET_EXTI();
  
  /* Enable a rising trigger EXTI Interrupt */
  USER_BUTTON_EXTI_LINE_ENABLE();
  USER_BUTTON_EXTI_FALLING_TRIG_ENABLE();
  
  /* Configure NVIC for USER_BUTTON_EXTI_IRQn */ 
  NVIC_SetPriority(USER_BUTTON_EXTI_IRQn,0x03);
  NVIC_EnableIRQ(USER_BUTTON_EXTI_IRQn);
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = HSE BYPASS
  *            SYSCLK(Hz)                     = 2000000 (CPU Clock)
  *            HCLK(Hz)                       = 2000000 (AXI and AHBs Clock)
  *            AHB Prescaler                  = 1
  *            D1 APB3 Prescaler              = 1 (APB3 Clock  2MHz)
  *            D2 APB1 Prescaler              = 1 (APB1 Clock  2MHz)
  *            D2 APB2 Prescaler              = 1 (APB2 Clock  2MHz)
  *            D3 APB4 Prescaler              = 1 (APB4 Clock  2MHz)
  *            HSE Frequency(Hz)              = 8000000
  *            VDD(V)                         = 3.3
  *            Flash Latency(WS)              = 0
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
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);

  /* Set Sys & AHB & APB1 & APB2 & APB4  prescaler */
  LL_RCC_SetSysPrescaler(LL_RCC_SYSCLK_DIV_4);
  LL_RCC_SetAHBPrescaler(LL_RCC_AHB_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
  LL_RCC_SetAPB4Prescaler(LL_RCC_APB4_DIV_1);

  /* Set PLL1 as System Clock Source */
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSE);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSE)
  {
  }

  /* Set systick to 1ms */
  SysTick_Config(2000000 / 1000);

  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  SystemCoreClock = 2000000;
}


/******************************************************************************/
/*   USER IRQ HANDLER TREATMENT                                               */
/******************************************************************************/
/**
  * @brief  Function to manage IRQ Handler
  * @param  None
  * @retval None
  */
void UserButton_Callback(void)
{
  KeyPressed = 1;
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
#endif

/**
  * @}
  */

/**
  * @}
  */

