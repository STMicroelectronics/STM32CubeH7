/**
  ******************************************************************************
  * @file    HSEM/HSEM_CoreNotification/CM4/Src/main.c
  * @author  MCD Application Team
  * @brief   This is the main program for Cortex-M4 
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
#define HSEM_ID_0 (0U) /* HW semaphore 0*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
volatile uint32_t Notif_Recieved;
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

  /* Activate HSEM notification for Cortex-M4*/
  HAL_HSEM_ActivateNotification(__HAL_HSEM_SEMID_TO_MASK(HSEM_ID_0));

  /* 
  Domain D2 goes to STOP mode (Cortex-M4 in deep-sleep) waiting for Cortex-M7 to
  perform system initialization (system clock config, external memory configuration.. )   
  */
  HAL_PWREx_ClearPendingEvent();
  HAL_PWREx_EnterSTOPMode(PWR_MAINREGULATOR_ON, PWR_STOPENTRY_WFE, PWR_D2_DOMAIN);
  /* Clear HSEM flag */
  __HAL_HSEM_CLEAR_FLAG(__HAL_HSEM_SEMID_TO_MASK(HSEM_ID_0));
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

  /* Add Cortex-M4 user application code here */ 
  /* Enable HSEM Interrupt */
  HAL_NVIC_SetPriority(HSEM2_IRQn, 10, 0);
  HAL_NVIC_EnableIRQ(HSEM2_IRQn);  
  /* Initialize LED 3 */
  BSP_LED_Init(LED3);  
   
  while (1)
  {
    Notif_Recieved = 0;
    /* HSEM Activate Notification*/
    HAL_HSEM_ActivateNotification(__HAL_HSEM_SEMID_TO_MASK(HSEM_ID_0));
    /* Wait for Notification from CPU1(CM7)*/
    while( Notif_Recieved == 0)
    {
    }
    /* Notification received from CPU1(CM7)*/
    /* CM4 toggles LED3 */
    BSP_LED_Toggle(LED3);         

  }

}




/**
  * @brief Semaphore Released Callback.
  * @param SemMask: Mask of Released semaphores
  * @retval None
  */
void HAL_HSEM_FreeCallback(uint32_t SemMask)
{
  Notif_Recieved = 1;
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

