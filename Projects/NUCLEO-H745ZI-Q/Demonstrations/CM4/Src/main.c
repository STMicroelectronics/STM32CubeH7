/**
  ******************************************************************************
  * @file    Demonstrations/CM4/Src/main.c
  * @author  MCD Application Team
  * @brief   main program for Cortex-M4 
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

/** @addtogroup STM32H7xx_HAL_Demonstrations
  * @{
  */

/** @addtogroup Demo
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
typedef enum
{
  SHIELD_NOT_DETECTED = 0,
  SHIELD_DETECTED
}ShieldStatus;

/* Private define ------------------------------------------------------------*/
#define HSEM_ID_0 (0U) /* HW semaphore 0*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
volatile uint32_t Notif_Recieved = 0;
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
static ShieldStatus TFT_ShieldDetect(void);

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

  /* Initialize LED 2 */
  BSP_LED_Init(LED2);

  if(TFT_ShieldDetect() == SHIELD_DETECTED)
  {
    while (1)
    {
      /* Wait for Notification from CPU1(CM7)*/
      while( Notif_Recieved == 0)
      {
      }
      /* Notification received from CPU1(CM7)*/
      /* CM4 toggles LED2 */
      BSP_LED_On(LED2);
      HAL_Delay(200);
      BSP_LED_Off(LED2);
      
      Notif_Recieved = 0;
      /* HSEM Activate Notification*/
      HAL_HSEM_ActivateNotification(__HAL_HSEM_SEMID_TO_MASK(HSEM_ID_0));    
      
    }    
  }
  else
  {
    while (1)
    {
      /* Wait for Notification from CPU1(CM7)*/
      while( Notif_Recieved == 0)
      {
      }
      /* Notification received from CPU1(CM7)*/
      /* CM4 toggles LED2 */
      BSP_LED_Toggle(LED2);
      
      Notif_Recieved = 0;
      /* HSEM Activate Notification*/
      HAL_HSEM_ActivateNotification(__HAL_HSEM_SEMID_TO_MASK(HSEM_ID_0));    
      
    }    
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

/**
  * @brief  Check the availability of adafruit 1.8" TFT shield on top of STM32NUCLEO
  *         board. This is done by reading the state of IO PF.03 pin (mapped to
  *         JoyStick available on adafruit 1.8" TFT shield). If the state of PF.03
  *         is high then the adafruit 1.8" TFT shield is available.
  * @param  None
  * @retval SHIELD_DETECTED: 1.8" TFT shield is available
  *         SHIELD_NOT_DETECTED: 1.8" TFT shield is not available
  */
static ShieldStatus TFT_ShieldDetect(void)
{
  GPIO_InitTypeDef  GPIO_InitStruct;

  /* Enable GPIO clock */
  ADAFRUIT_802_ADCx_GPIO_CLK_ENABLE();

  GPIO_InitStruct.Pin = ADAFRUIT_802_ADCx_GPIO_PIN ;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(ADAFRUIT_802_ADCx_GPIO_PORT , &GPIO_InitStruct);

  if(HAL_GPIO_ReadPin(ADAFRUIT_802_ADCx_GPIO_PORT, ADAFRUIT_802_ADCx_GPIO_PIN) != 0)
  {
    return SHIELD_DETECTED;
  }
  else
  {
    return SHIELD_NOT_DETECTED;
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

