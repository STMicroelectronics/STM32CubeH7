/**
  ******************************************************************************
  * @file    PWR/PWR_Domain3SystemControl/CM4/Src/main.c
  * @author  MCD Application Team
  * @brief   This example shows how to maintain system control using
  *          D3 Domain only.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
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

/** @addtogroup PWR_Domain3SystemControl
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
  /* HW semaphore Clock enable */
  __HAL_RCC_HSEM_CLK_ENABLE();
  
  /* Configure the NVIC HSEM notification interrupt for CM4 */
  HAL_NVIC_SetPriority(HSEM2_IRQn, 0x0, 0);
  HAL_NVIC_EnableIRQ(HSEM2_IRQn);
  
  /* Activate HSEM notification for Cortex-M4 */
  HAL_HSEM_ActivateNotification(__HAL_HSEM_SEMID_TO_MASK(HSEM_ID_0));
  
  /* 
    Domain D2 goes to STOP mode (Cortex-M4 in deep-sleep) waiting for Cortex-M7 to
    perform system initialization (system clock config, external memory configuration.. )   
  */
  HAL_PWREx_ClearPendingEvent();
  HAL_PWREx_EnterSTOPMode(PWR_MAINREGULATOR_ON, PWR_STOPENTRY_WFE, PWR_D2_DOMAIN);

  /* Deactivate HSEM notification for Cortex-M4 */
  HAL_HSEM_DeactivateNotification(__HAL_HSEM_SEMID_TO_MASK(HSEM_ID_0));
  
#if defined (LOW_POWER_BOARD)
  
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
  
  /* -2- Initialize LED2 */
  BSP_LED_Init(LED2);
  
  while(1)
  {
    /* Turn On LED2 to indicate that CM4 is wokenup from STOP mode */
    BSP_LED_On(LED2);
    
    /* Insert 3 seconds delay */
    HAL_Delay(3000);
    
    /* Clear STOP flag */
    __HAL_PWR_CLEAR_FLAG(PWR_CPU_FLAGS);
    
    /* Enter D2 and D3 domains to DStop mode */
    if(__HAL_PWR_GET_FLAG(PWR_FLAG_STOP) == RESET)
    {    
      /* Turn Off LED2 */
      BSP_LED_Off(LED2);
      
      /* Enter D3 domain to DStop mode */
      HAL_PWREx_EnterSTOPMode(PWR_MAINREGULATOR_ON, PWR_STOPENTRY_WFI, PWR_D3_DOMAIN);
      
      /* Enter D2 domain to DStop mode */
      HAL_PWREx_EnterSTOPMode(PWR_MAINREGULATOR_ON, PWR_STOPENTRY_WFI, PWR_D2_DOMAIN);
    }
  }
  
#else
    while(1)
    {
    }
#endif /* LOW_POWER_BOARD */

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
