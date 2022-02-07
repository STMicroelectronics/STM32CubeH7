/**
  ******************************************************************************
  * @file    ResourcesManager/ResourcesManager_SharedResources/CM4/Src/main.c
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
#include "res_mgr.h"

/** @addtogroup STM32H7xx_HAL_Applications
  * @{
  */

/** @addtogroup ResourcesManager_SharedResources
  * @{
  */
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define HSEM_ID_0 (0U) /* HW semaphore 0*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
COM_InitTypeDef  COM_Init;
/* Private function prototypes -----------------------------------------------*/

static void ResMgrSendMessage (uint32_t id, uint32_t msg);
static void ResMgrCallback (uint32_t id, uint32_t msg);
static void Error_Handler(void);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  ResMgr_Status_t status;

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

  /* Initialize LEDs */
  BSP_LED_Init(LED2);
  BSP_LED_Init(LED3);

  /* Start the resources manager */
  if ( ResMgr_Init(ResMgrSendMessage, ResMgrCallback) != RESMGR_OK)
  {
    Error_Handler();
  }

  /* COM Pre-Initialization*/
  COM_Init.BaudRate = 115200;
  COM_Init.WordLength=COM_WORDLENGTH_8B;
  COM_Init.StopBits=COM_STOPBITS_1;
  COM_Init.Parity=COM_PARITY_NONE;
  COM_Init.HwFlowCtl=COM_HWCONTROL_NONE;
  
  for (;;)
  {
    do
    {
      status =ResMgr_Request(RESMGR_ID_USART1, RESMGR_FLAGS_ACCESS_NORMAL  |\
                             RESMGR_FLAGS_CPU2 , 0, NULL) ;
    }while (status == RESMGR_BUSY);

    /* Here We can use the resource */

    BSP_LED_On(LED2);

    BSP_COM_Init(COM1,&COM_Init);
    printf("THIS MESSAGE IS FROM CM4 CORE \n\r");
    HAL_Delay(2000); /* To simutate the use of the resource*/
    BSP_COM_DeInit(COM1);
    ResMgr_Release(RESMGR_ID_USART1, RESMGR_FLAGS_CPU2);

    BSP_LED_Off(LED2);
    HAL_Delay(100);
  }

}





static void ResMgrSendMessage (uint32_t id, uint32_t msg)
{
}

static void ResMgrCallback (uint32_t id, uint32_t msg)
{
}


/**
* @brief  This function is executed in case of error occurrence.
* @param  None
* @retval None
*/
static void Error_Handler(void)
{
  /* Turn LED3 on */
  BSP_LED_On(LED3);
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

