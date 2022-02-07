/**
  ******************************************************************************
  * @file    PWR/PWR_STANDBY_RTC/CM4/Src/main.c
  * @author  MCD Application Team
  * @brief   Main program body for Cortex-M4.
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

/** @addtogroup STM32H7xx_HAL_Examples
  * @{
  */

/** @addtogroup PWR_STANDBY_RTC
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  
 /*HW semaphore Clock enable*/
  __HAL_RCC_HSEM_CLK_ENABLE();
 
  /* Configure the NVIC HSEM notification interrupt for CM4 */
  HAL_NVIC_SetPriority(HSEM2_IRQn, 0x0, 0);
  HAL_NVIC_EnableIRQ(HSEM2_IRQn);
  
 /* Activate HSEM notification for Cortex-M4*/
  HAL_HSEM_ActivateNotification(__HAL_HSEM_SEMID_TO_MASK(HSEM_ID_0));
  
  /* 
    Domain D2 goes to STOP mode (Cortex-M4 in deep-sleep) waiting for Cortex-M7 to
    perform system initialization (system clock config, external memory configuration.. )   
  */
  HAL_PWREx_ClearPendingEvent();
  HAL_PWREx_EnterSTOPMode(PWR_MAINREGULATOR_ON, PWR_STOPENTRY_WFE, PWR_D2_DOMAIN);

  /* Deactivate HSEM notification for Cortex-M4 */
  HAL_HSEM_DeactivateNotification(__HAL_HSEM_SEMID_TO_MASK(HSEM_ID_0));
   
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
  
  /* Initialize LED2 */
  BSP_LED_Init(LED2);
  
  /* Turn ON LED2 */
  BSP_LED_On(LED2);
    
  /* Insert 3 seconds delay */
  HAL_Delay(3000);  
    
  /* Turn Off LED2 */
  BSP_LED_Off(LED2);
  
  /* Enter D3 domain to DStandby mode */
  HAL_PWREx_EnterSTANDBYMode(PWR_D3_DOMAIN);
  
  /* Enter D2 domain to DStandby mode */
  HAL_PWREx_EnterSTANDBYMode(PWR_D2_DOMAIN);

  /* Infinite loop */
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

