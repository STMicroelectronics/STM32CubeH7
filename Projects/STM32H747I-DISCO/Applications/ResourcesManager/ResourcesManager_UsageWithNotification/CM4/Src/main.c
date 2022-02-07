/**
  ******************************************************************************
  * @file    ResourcesManager/ResourcesManager_SharedResourcesWithNotication/CM4/Src/main.c
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
#include <stdio.h>
#include <string.h>

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
uint8_t CM4_Message[] = "THIS MESSAGE IS FROM CM4 CORE \n\r";
UART_HandleTypeDef UartHandle;
void  *ResoucreHandle;
static __IO uint32_t ResourceStatus = 0;
static __IO uint32_t ResoucreID = 0xFFFFFFFF;
/* Private function prototypes -----------------------------------------------*/
static void ResMgrSendMessage (uint32_t id, uint32_t msg);
static void ResMgrCallback (uint32_t id, uint32_t msg);
static void CustomIPC_RecievedMessage (uint32_t id, uint32_t msg);
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
  /* clear old status due to HSEM Notification used for wakeup*/
  __HAL_HSEM_CLEAR_FLAG(__HAL_HSEM_SEMID_TO_MASK(HSEM_ID_0));
	
  HAL_NVIC_ClearPendingIRQ(HSEM2_IRQn);

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
  
  /* Initialize a custom IPC for messaging between the 2 cores */ 
  if ( Custom_IPC_Init(CPU2,CustomIPC_RecievedMessage)!= CUSTOMIPC_OK)
  {
    Error_Handler();
  }
  /* Start the resources manager */
  if ( ResMgr_Init(ResMgrSendMessage, ResMgrCallback) != RESMGR_OK)
  {
    Error_Handler();
  }


  for (;;)
  {
    /* RESMGR_FLAGS_INHERIT_HANDLE Flag is used to inherit the resource context frm the other core.
       So we can use the resource without doing the initialization */
    status =ResMgr_Request(RESMGR_ID_USART2, RESMGR_FLAGS_INHERIT_HANDLE |\
                           RESMGR_FLAGS_ACCESS_PEND  | RESMGR_FLAGS_CPU2,\
                           0, &ResoucreHandle) ;
    if(status == RESMGR_BUSY)
    {
      while((ResourceStatus != RESMGR_MSG_PEND_ASSIGN) || (ResoucreID != RESMGR_ID_USART2))
      {
        /*waiting for the resource until be released by the second Core and assigned to current Core*/
      }
      /*Resource is allocated we can use it*/
      ResourceStatus = 0;
      ResoucreID = 0xFFFFFFFF;
    }
    else if(status == RESMGR_ERROR)
    {
      /*Error */
      Error_Handler();
    }

    /* Here The resource is Free (Released), We can use it */
    BSP_LED_On(LED2);
    /* copy the inherited handle to local handle,So We can use the resource without doing the initialization */
    memcpy(&UartHandle, ResoucreHandle,sizeof (UART_HandleTypeDef));

    HAL_UART_Transmit(&UartHandle,CM4_Message,sizeof(CM4_Message),0xff);
    HAL_Delay(2000); /* To simutate the use of the resource*/
    ResMgr_Release(RESMGR_ID_USART2, RESMGR_FLAGS_CPU2);

    BSP_LED_Off(LED2);
    HAL_Delay(100);
  }

}





static void ResMgrSendMessage (uint32_t id, uint32_t msg)
{
  Custom_IPC_Send(id,msg);
}

static void ResMgrCallback (uint32_t id, uint32_t msg)
{
}

static void CustomIPC_RecievedMessage (uint32_t id, uint32_t msg)
{
  ResourceStatus = msg;
  ResoucreID = id;
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

