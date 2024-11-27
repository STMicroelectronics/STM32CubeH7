/**
  ******************************************************************************
  * @file    Templates_LL/Src/main.c
  * @author  MCD Application Team
  * @brief   Main program body through the LL API
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

/** @addtogroup STM32H7xx_LL_Examples
  * @{
  */

/** @addtogroup Templates_LL
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void CPU_CACHE_Enable(void);
void     LED1_Init(void);
void     LED2_Init(void);
void     LED3_Init(void);
void     LED1_On(void);
void     LED2_On(void);
void     LED3_On(void);
void     LED1_Off(void);
void     LED2_Off(void);
void     LED3_Off(void);

#if (USE_VOS0_480MHZ_OVERCLOCK == 1)
static void SystemClock_Config_480MHz(void);
#else
static void SystemClock_Config_400MHz(void);
#endif /* USE_VOS0_480MHZ_OVERCLOCK */

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* Enable the CPU Cache */
  CPU_CACHE_Enable();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize LED1 */
  LED1_Init();

  /* Initialize LED2 */
  LED2_Init();

/* Initialize LED3 */
  LED3_Init();

  /* Infinite loop */
  while (1)
  {
    LED3_Off();
    LED1_On();
    LL_mDelay(100);
    LED1_Off();
    LED2_On();
    LL_mDelay(100);
    LED2_Off();
    LED3_On();
    LL_mDelay(100);
  }
}

/**
  * @brief  Initialize LED1.
  * @param  None
  * @retval None
  */
void LED1_Init(void)
{
  /* Enable the LED1 Clock */
  LED1_GPIO_CLK_ENABLE();

  /* Configure IO in output push-pull mode to drive external LED1 */
  LL_GPIO_SetPinMode(LED1_GPIO_PORT, LED1_PIN, LL_GPIO_MODE_OUTPUT);
}

/**
  * @brief  Initialize LED2.
  * @param  None
  * @retval None
  */
void LED2_Init(void)
{
  /* Enable the LED2 Clock */
  LED2_GPIO_CLK_ENABLE();

  /* Configure IO in output push-pull mode to drive external LED2 */
  LL_GPIO_SetPinMode(LED2_GPIO_PORT, LED2_PIN, LL_GPIO_MODE_OUTPUT);
}

/**
  * @brief  Initialize LED3.
  * @param  None
  * @retval None
  */
void LED3_Init(void)
{
  /* Enable the LED3 Clock */
  LED3_GPIO_CLK_ENABLE();

  /* Configure IO in output push-pull mode to drive external LED3 */
  LL_GPIO_SetPinMode(LED3_GPIO_PORT, LED3_PIN, LL_GPIO_MODE_OUTPUT);
}

/**
  * @brief  Turn-on LED1.
  * @param  None
  * @retval None
  */
void LED1_On(void)
{
  /* Turn LED1 on */
  LL_GPIO_SetOutputPin(LED1_GPIO_PORT, LED1_PIN);
}

/**
  * @brief  Turn-on LED2.
  * @param  None
  * @retval None
  */
void LED2_On(void)
{
  /* Turn LED2 on */
  LL_GPIO_SetOutputPin(LED2_GPIO_PORT, LED2_PIN);
}

/**
  * @brief  Turn-off LED3.
  * @param  None
  * @retval None
  */
void LED3_On(void)
{
  /* Turn LED3 off */
  LL_GPIO_SetOutputPin(LED3_GPIO_PORT, LED3_PIN);
}

/**
  * @brief  Turn-off LED1 .
  * @param  None
  * @retval None
  */
void LED1_Off(void)
{
  /* Turn LED1 off */
  LL_GPIO_ResetOutputPin(LED1_GPIO_PORT, LED1_PIN);
}

/**
  * @brief  Turn-off LED2 .
  * @param  None
  * @retval None
  */
void LED2_Off(void)
{
  /* Turn LED2 off */
  LL_GPIO_ResetOutputPin(LED2_GPIO_PORT, LED2_PIN);
}

/**
  * @brief  Turn-on LED3 .
  * @param  None
  * @retval None
  */
void LED3_Off(void)
{
  /* Turn LED3 on */
  LL_GPIO_ResetOutputPin(LED3_GPIO_PORT, LED3_PIN);
}

/**
  * @brief  System Clock Configuration
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  __IO uint32_t tmpreg =0;
  /* Power Configuration */
  LL_PWR_ConfigSupply(LL_PWR_LDO_SUPPLY);
  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
  while (LL_PWR_IsActiveFlag_VOS() == 0)
  {
  }
  /* Enable HSE oscillator */
  LL_RCC_HSE_EnableBypass();
  LL_RCC_HSE_Enable();
  while(LL_RCC_HSE_IsReady() != 1)
  {
  }
#if (USE_VOS0_480MHZ_OVERCLOCK == 1)
   LL_APB4_GRP1_EnableClock(LL_APB4_GRP1_PERIPH_SYSCFG);
   /* Delay after an RCC peripheral clock enabling */
   tmpreg = READ_BIT(RCC->APB4ENR, RCC_APB4ENR_SYSCFGEN);
  /* Enable the PWR overdrive */
   SET_BIT(SYSCFG->PWRCR, SYSCFG_PWRCR_ODEN);
  /* Delay after setting the syscfg boost setting */
  tmpreg = READ_BIT(SYSCFG->PWRCR, SYSCFG_PWRCR_ODEN);
  while (LL_PWR_IsActiveFlag_VOS() == 0)
  {
  }
  SystemClock_Config_480MHz();
#else
  SystemClock_Config_400MHz();
#endif /* (USE_VOS0_480MHZ_OVERCLOCK == 1) */

  /* Set FLASH latency */
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_4);
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
  (void)tmpreg;
}

#if (USE_VOS0_480MHZ_OVERCLOCK == 1)
/**
  * @brief  System Clock Configuration to 480MHz
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE BYPASS)
  *            SYSCLK(Hz)                     = 480000000 (CPU Clock)
  *            HCLK(Hz)                       = 240000000 (AXI and AHBs Clock)
  *            AHB Prescaler                  = 2
  *            D1 APB3 Prescaler              = 2 (APB3 Clock  120MHz)
  *            D2 APB1 Prescaler              = 2 (APB1 Clock  120MHz)
  *            D2 APB2 Prescaler              = 2 (APB2 Clock  120MHz)
  *            D3 APB4 Prescaler              = 2 (APB4 Clock  120MHz)
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 4
  *            PLL_N                          = 480
  *            PLL_P                          = 2
  *            PLL_Q                          = 4
  *            PLL_R                          = 2
  *            VDD(V)                         = 3.3
  *            Flash Latency(WS)              = 4
  * @param  None
  * @retval None
  */
static void SystemClock_Config_480MHz(void)
{
  /* Main PLL configuration and activation */
  LL_RCC_PLL_SetSource(LL_RCC_PLLSOURCE_HSE);
  LL_RCC_PLL1P_Enable();
  LL_RCC_PLL1Q_Enable();
  LL_RCC_PLL1R_Enable();
  LL_RCC_PLL1FRACN_Disable();
  LL_RCC_PLL1_SetVCOInputRange(LL_RCC_PLLINPUTRANGE_2_4);
  LL_RCC_PLL1_SetVCOOutputRange(LL_RCC_PLLVCORANGE_WIDE);
  LL_RCC_PLL1_SetM(4);
  LL_RCC_PLL1_SetN(480);
  LL_RCC_PLL1_SetP(2);
  LL_RCC_PLL1_SetQ(4);
  LL_RCC_PLL1_SetR(2);
  LL_RCC_PLL1_Enable();
  while(LL_RCC_PLL1_IsReady() != 1)
  {
  }

  /* Set systick to 1ms */
  SysTick_Config(480000000 / 1000);

  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  SystemCoreClock = 480000000;
}
#else
/**
  * @brief  System Clock Configuration to 400MHz
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
static void SystemClock_Config_400MHz(void)
{
  /* Main PLL configuration and activation */
  LL_RCC_PLL_SetSource(LL_RCC_PLLSOURCE_HSE);
  LL_RCC_PLL1P_Enable();
  LL_RCC_PLL1Q_Enable();
  LL_RCC_PLL1R_Enable();
  LL_RCC_PLL1FRACN_Disable();
  LL_RCC_PLL1_SetVCOInputRange(LL_RCC_PLLINPUTRANGE_2_4);
  LL_RCC_PLL1_SetVCOOutputRange(LL_RCC_PLLVCORANGE_WIDE);
  LL_RCC_PLL1_SetM(4);
  LL_RCC_PLL1_SetN(400);
  LL_RCC_PLL1_SetP(2);
  LL_RCC_PLL1_SetQ(4);
  LL_RCC_PLL1_SetR(2);
  LL_RCC_PLL1_Enable();
  while(LL_RCC_PLL1_IsReady() != 1)
  {
  }

  /* Set systick to 1ms */
  SysTick_Config(400000000 / 1000);

  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  SystemCoreClock = 400000000;

}
#endif /* USE_VOS0_480MHZ_OVERCLOCK == 1*/

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

/* ==============   BOARD SPECIFIC CONFIGURATION CODE END      ============== */

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
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

