/**
  ******************************************************************************
  * @file    Templates_LL/CM4/Src/main.c
  * @author  MCD Application Team
  * @brief   Main program body through the LL API for Cortex-M4 
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
#include "ll_common.h"

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
static void EnterD2Domain_STOPMode(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /*HW semaphore Clock enable*/
  LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_HSEM);

 /* Activate HSEM notification for Cortex-M4*/
  LL_HSEM_EnableIT_C2IER(HSEM, LL_HSEM_MASK_0);

  /* 
    Domain D2 goes to STOP mode (Cortex-M4 in deep-sleep) waiting for Cortex-M7 to
    perform system initialization (system clock config, external memory configuration.. )   
  */
   EnterD2Domain_STOPMode();
     
  /* Clear HSEM flag */  
  LL_HSEM_ClearFlag_C2ICR(HSEM, LL_HSEM_MASK_0);

  /* Set systick to 1ms */
  SysTick_Config(200000000 / 1000);

  
  /* Add Cortex-M4 user application code here */ 

  /* Infinite loop */
  while (1)
  {
  }
}

static void EnterD2Domain_STOPMode(void)
{
  /* Clear pending events if any */
  __SEV();
  __WFE();
    
  /* Select the domain Power Down DeepSleep */  
  LL_PWR_SetRegulModeDS(LL_PWR_REGU_DSMODE_MAIN);
  /* Keep DSTOP mode when D2 domain enters Deepsleep */
  LL_PWR_CPU_SetD2PowerMode(LL_PWR_CPU_MODE_D2STOP);
  LL_PWR_CPU2_SetD2PowerMode(LL_PWR_CPU2_MODE_D2STOP);
  /* Set SLEEPDEEP bit of Cortex System Control Register */
  LL_LPM_EnableDeepSleep();
  /* Ensure that all instructions done before entering STOP mode */
  __DSB();
  __ISB();

  /* Request Wait For Event */
  __WFE();

    /* Reset SLEEPDEEP bit of Cortex System Control Register, 
       the following LL API Clear SLEEPDEEP bit of Cortex System Control Register   */
    LL_LPM_EnableSleep();
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

