/**
  ******************************************************************************
  * @file    PWR/PWR_D2ON_D1OFF/CM4/Src/main.c
  * @author  MCD Application Team
  * @brief   Main program body
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

/** @addtogroup PWR_D2ON_D1OFF
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static void Error_Handler(void);

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
  
  
  /* Initialize LED2 */
  BSP_LED_Init(LED2);

  /* D1 domain must be OFF. If D1 domain clock is ready then return Error */ 
  if(__HAL_RCC_GET_FLAG(RCC_FLAG_D1CKRDY))
  {
    Error_Handler();
  }
    
  while(1)
  {
    /* Test passed toggle LED2 each 100 ms */
    HAL_Delay(100);
    BSP_LED_Toggle(LED2);
  }
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

