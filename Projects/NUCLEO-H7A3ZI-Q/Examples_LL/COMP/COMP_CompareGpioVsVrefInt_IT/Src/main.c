/**
  ******************************************************************************
  * @file    Examples_LL/COMP/COMP_CompareGpioVsVrefInt_IT/Src/main.c
  * @author  MCD Application Team
  * @brief   This example shows how to use a comparator peripheral
  *          to compare a voltage level applied on a GPIO pin
  *          versus the internal voltage reference (VrefInt),
  *          in interrupt mode.
  *          This example is based on the STM32H7xx COMP LL API;
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

/** @addtogroup COMP_CompareGpioVsVrefInt_IT
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static void     SystemClock_Config(void);
void     Configure_COMP(void);
void     Activate_COMP(void);
void     LED_Init(void);
void     LED_On(void);
void     LED_Off(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* Configure the system clock to 280 MHz */
  SystemClock_Config();
  
  /* Initialize LED1 */
  LED_Init();
  
  /* Configure comparator */
  Configure_COMP();
  
  /* Activate comparator */
  Activate_COMP();
  
  /* Lock comparator instance */
  //LL_COMP_Lock(COMP1);
  
  /* Infinite loop */
  while (1)
  {
    /* Note: LED state depending on COMP status is set into COMP IRQ handler, */
    /*        refer to function "ComparatorTrigger_Callback()".               */
    
  }
}

/**
  * @brief  Configure comparator (COMP instance: COMP1), GPIO and EXTI line
  *         used by comparator.
  * @note   This function configures the comparator but does not enable it,
  *         in order to optimize power consumption (comparator enabled only
  *         when needed).
  * @note   Peripheral configuration is minimal configuration from reset values.
  *         Thus, some useless LL unitary functions calls below are provided as
  *         commented examples - setting is default configuration from reset.
  * @param  None
  * @retval None
  */
void Configure_COMP(void)
{
  __IO uint32_t wait_loop_index = 0;
  
  /*## Configuration of GPIO used by comparator ##############################*/
  
  /* Configure GPIO connected to the selected comparator input plus */
  /* Enable GPIO clock */
  LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOB);
  
  /* Configure GPIO in analog mode to be used as COMP input */
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_2, LL_GPIO_MODE_ANALOG);
  
  /*## Configuration of NVIC #################################################*/
  /* Configure NVIC to enable COMP1 interruptions */
  NVIC_SetPriority(COMP_IRQn, 0);
  NVIC_EnableIRQ(COMP_IRQn);
  
  /*## Configuration of comparator ###########################################*/
  
  /* Enable COMP clock */
  LL_APB4_GRP1_EnableClock(LL_APB4_GRP1_PERIPH_COMP12);
  
  /*## Configuration of comparator common instance ###########################*/
  
  /* Set comparator common instance window mode */
  /* Note: Window mode is common to 2 COMP instances, and therefore must      */
  /*       with a COMP common instance (COMP12_COMMON, ...)                   */
  //LL_COMP_SetCommonWindowMode(__LL_COMP_COMMON_INSTANCE(COMP1), LL_COMP_WINDOWMODE_DISABLE);
  
  /*## Configuration of comparator instance ##################################*/
  
  /* Set comparator instance operating mode to adjust power and speed */
  LL_COMP_SetPowerMode(COMP1, LL_COMP_POWERMODE_ULTRALOWPOWER);
  
  /* Set comparator inputs */
  LL_COMP_ConfigInputs(COMP1, LL_COMP_INPUT_MINUS_VREFINT, LL_COMP_INPUT_PLUS_IO2);
  
  /* Note: Call of the functions below are commented because they are         */
  /*       useless in this example:                                           */
  /*       settings corresponding to default configuration from reset state.  */
  
  /* Set comparator instance hysteresis mode of the input minus */
  //LL_COMP_SetInputHysteresis(COMP1, LL_COMP_HYSTERESIS_NONE);
  
  /* Set comparator instance output polarity */
  //LL_COMP_SetOutputPolarity(COMP1, LL_COMP_OUTPUTPOL_NONINVERTED);
  
  /* Set comparator instance blanking source */
  //LL_COMP_SetOutputBlankingSource(COMP1, LL_COMP_BLANKINGSRC_NONE);
  
  /* Delay for COMP voltage scaler stabilization time.                        */
  /* Compute number of CPU cycles to wait for, from delay in us.              */
  /* Note: Variable divided by 2 to compensate partially                      */
  /*       CPU processing cycles (depends on compilation optimization).       */
  /* Note: If system core clock frequency is below 200kHz, wait time          */
  /*       is only a few CPU processing cycles.                               */
  wait_loop_index = ((LL_COMP_DELAY_VOLTAGE_SCALER_STAB_US * (SystemCoreClock / (100000 * 2))) / 10);
  while(wait_loop_index != 0)
  {
    wait_loop_index--;
  }
  
  /*## Configuration of EXTI line used by comparator #########################*/
  /* Set EXTI line trigger edge */
  LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_20);;
  LL_EXTI_EnableFallingTrig_0_31(LL_EXTI_LINE_20);;
  
  /* Set EXTI line interruption */
  LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_20);
  
}

/**
  * @brief  Perform comparator activation procedure
  *         (COMP instance: COMP1).
  * @note   Operations:
  *         - Enable comparator instance
  *         - Wait for comparator startup time
  *           (required to reach propagation delay specification)
  * @param  None
  * @retval None
  */
void Activate_COMP(void)
{
  __IO uint32_t wait_loop_index = 0;
  
  /* Enable comparator */
  LL_COMP_Enable(COMP1);
  
  /* Delay for comparator startup time.                                       */
  /* Compute number of CPU cycles to wait for, from delay in us.              */
  /* Note: Variable divided by 2 to compensate partially                      */
  /*       CPU processing cycles (depends on compilation optimization).       */
  /* Note: If system core clock frequency is below 200kHz, wait time          */
  /*       is only a few CPU processing cycles.                               */
  wait_loop_index = ((LL_COMP_DELAY_STARTUP_US * (SystemCoreClock / (100000 * 2))) / 10);
  while(wait_loop_index != 0)
  {
    wait_loop_index--;
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
  * @brief  Turn-off LED1.
  * @param  None
  * @retval None
  */
void LED_Off(void)
{
  /* Turn LED1 off */
  LL_GPIO_ResetOutputPin(LED1_GPIO_PORT, LED1_PIN);
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL1 (HSE)
  *            SYSCLK(Hz)                     = 280000000 (CPU Clock)
  *            HCLK(Hz)                       = 280000000 (AXI and AHBs Clock)
  *            AHB Prescaler                  = 1
  *            CD APB3 Prescaler              = 2 (APB3 Clock  140MHz)
  *            CD APB1 Prescaler              = 2 (APB1 Clock  140MHz)
  *            CD APB2 Prescaler              = 2 (APB2 Clock  140MHz)
  *            SRD APB4 Prescaler             = 2 (APB4 Clock  140MHz)
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 4
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
  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE0);
  while (LL_PWR_IsActiveFlag_VOS() == 0)
  {
  }

  /* Enable HSE oscillator */
  LL_RCC_HSE_EnableBypass();
  LL_RCC_HSE_SelectDigitalClock();
  LL_RCC_HSE_Enable();
  while(LL_RCC_HSE_IsReady() != 1)
  {
  }

  /* Set FLASH latency */
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_6);

  /* Main PLL configuration and activation */
  LL_RCC_PLL_SetSource(LL_RCC_PLLSOURCE_HSE);
  LL_RCC_PLL1P_Enable();
  LL_RCC_PLL1Q_Enable();
  LL_RCC_PLL1R_Enable();
  LL_RCC_PLL1FRACN_Disable();
  LL_RCC_PLL1_SetVCOInputRange(LL_RCC_PLLINPUTRANGE_2_4);
  LL_RCC_PLL1_SetVCOOutputRange(LL_RCC_PLLVCORANGE_WIDE);
  LL_RCC_PLL1_SetM(4);
  LL_RCC_PLL1_SetN(280);
  LL_RCC_PLL1_SetP(2);
  LL_RCC_PLL1_SetQ(2);
  LL_RCC_PLL1_SetR(2);
  LL_RCC_PLL1_Enable();

  while(LL_RCC_PLL1_IsReady() != 1)
  {
  }

  /* Set Sys & AHB & APB1 & APB2 & APB4  prescaler */
  LL_RCC_SetSysPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAHBPrescaler(LL_RCC_AHB_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_2);
  LL_RCC_SetAPB4Prescaler(LL_RCC_APB4_DIV_2);

  /* Set PLL1 as System Clock Source */
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL1);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL1)
  {
  }

  /* Set systick to 1ms */
  SysTick_Config(280000000 / 1000);

  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  SystemCoreClock = 280000000;
}

/******************************************************************************/
/*   USER IRQ HANDLER TREATMENT                                               */
/******************************************************************************/

/**
  * @brief  Comparator interruption callback
  *         This function is executed when the comparator threshold
  *         is triggered.
  * @retval None
  */
void ComparatorTrigger_Callback()
{
  /* Set LED state in function of comparator output level */
  if (LL_COMP_ReadOutputLevel(COMP1) == LL_COMP_OUTPUT_LEVEL_HIGH)
  {
    LED_On();
  }
  else
  {
    LED_Off();
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

