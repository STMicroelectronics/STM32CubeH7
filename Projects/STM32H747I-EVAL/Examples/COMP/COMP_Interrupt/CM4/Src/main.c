/**
  ******************************************************************************
  * @file    COMP/COMP_Interrupt/CM4/Src/main.c
  * @author  MCD Application Team
  * @brief   Main program body for Cortex-M4.
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

/** @addtogroup Templates
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void COMP_TriggerCallback(void);
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  
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
    
  /* -1- Initialize LEDs mounted on STM32H747I-EVAL board */
  BSP_LED_Init(LED4);
  
  /* Enable the COMPx IRQ Channel */
  HAL_NVIC_SetPriority(COMP_IRQn, 3, 0);
  HAL_NVIC_EnableIRQ(COMP_IRQn);
  
  /* 
  Domain D2 goes to STOP mode (Cortex-M4 in deep-sleep) waiting for Cortex-M7 to
  perform system initialization (system clock config, external memory configuration.. )   
  */
  HAL_PWREx_EnterSTOPMode(PWR_MAINREGULATOR_ON, PWR_STOPENTRY_WFI, PWR_D2_DOMAIN);
  
  /* Infinite loop */
  while (1)
  {
  }
}

/**
  * @brief  Comparator callback.
  * @param  hcomp COMP handle
  * @retval None
  */
void COMP_TriggerCallback(void)
{
  HAL_EXTI_D2_EventInputConfig(EXTI_LINE20 , EXTI_MODE_IT,  DISABLE);
  /* Clear the COMP1 D2 EXTI flag */
  __HAL_COMP_COMP1_EXTID2_CLEAR_FLAG();

  /* Clear the COMP channel 1 interrupt flag */
  __HAL_COMP_CLEAR_C1IFLAG();

  /* LED4 On */
  BSP_LED_On(LED4);

}




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

