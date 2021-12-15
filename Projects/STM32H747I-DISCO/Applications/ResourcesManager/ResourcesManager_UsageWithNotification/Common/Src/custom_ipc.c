/**
  ******************************************************************************
  * @file    ResourcesManager/ResourcesManager_SharedResourcesWithNotication/Common/Src/custom_ipc.c
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
#include "custom_ipc.h"

/** @addtogroup STM32H7xx_HAL_Examples
  * @{
  */

/** @addtogroup ResourcesManager_SharedResources
  * @{
  */
  
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
#if defined ( __ICCARM__ )
#pragma location = 0x38001000
CPU_message_t  CPU_Messages[2]; /* CPU_Messages[0]: CPU1->CPU2 */
                                /* CPU_Messages[1]: CPU2->CPU1 */
#elif defined ( __CC_ARM )
CPU_message_t  CPU_Messages[2] __attribute__((at(0x38001000))) = {0}; /* CPU_Messages[0]: CPU1->CPU2 */
                                                                           /* CPU_Messages[1]: CPU2->CPU1 */
#elif defined ( __GNUC__ )
CPU_message_t  CPU_Messages[2] __attribute__((section(".CPU_Messages_section"))); /* CPU_Messages[0]: CPU1->CPU2 */
                                                                           /* CPU_Messages[1]: CPU2->CPU1 */

#endif

static CustomIpcCallback_t   CustomIpcCallbackFunct;
static uint32_t CurrentCore;
/**
* @brief  Initialize the module
* @param  core : core identificator (CPU1/CPU2)
* @param  Callback : Callback function to be called when message is received
* @retval Return Status
*/
CustIpc_Status_t Custom_IPC_Init(uint32_t core,CustomIpcCallback_t Callback)
{
	__HAL_RCC_HSEM_CLK_ENABLE();
  
  CurrentCore = core;
  if(Callback != NULL )
  {
    CustomIpcCallbackFunct = Callback;
  }
  else
  {
    CustomIpcCallbackFunct = NULL;
    CurrentCore=0xA5A5A5A5; /* Invalid */
    return CUSTOMIPC_ERROR;
  }  
  
  if (CurrentCore == CPU1)
  {
    HAL_NVIC_SetPriority(HSEM1_IRQn, 10, 0);
    HAL_NVIC_EnableIRQ(HSEM1_IRQn);
    HAL_HSEM_ActivateNotification(__HAL_HSEM_SEMID_TO_MASK(HSEM_ID_2));
  }
  else if (CurrentCore == CPU2)
  {
    HAL_NVIC_SetPriority(HSEM2_IRQn, 10, 0);
    HAL_NVIC_EnableIRQ(HSEM2_IRQn);
    HAL_HSEM_ActivateNotification(__HAL_HSEM_SEMID_TO_MASK(HSEM_ID_1));
  }
  else
  {
    return CUSTOMIPC_ERROR;
  }
  
  return CUSTOMIPC_OK;  
}

/**
  * @brief  DeInitializes the Resource Manager module
  * @param  core : core identificator (CPU1/CPU2)
  * @retval Return Status
  */
CustIpc_Status_t Custom_IPC_DeInit(uint32_t core)
{
  
  if (CurrentCore == CPU1)
  {
    HAL_NVIC_DisableIRQ(HSEM1_IRQn);
    HAL_HSEM_DeactivateNotification(__HAL_HSEM_SEMID_TO_MASK(HSEM_ID_2));
  }
  else if (CurrentCore == CPU2)
  {
    HAL_NVIC_DisableIRQ(HSEM2_IRQn);
    HAL_HSEM_DeactivateNotification(__HAL_HSEM_SEMID_TO_MASK(HSEM_ID_1));
  }
  else
  {
    return CUSTOMIPC_ERROR;
  }

  CustomIpcCallbackFunct = NULL;
  CurrentCore=0xA5A5A5A5; /* Invalid */
  
  return CUSTOMIPC_OK;  
}
/**
* @brief  Send Message 
* @param  id : Message identificator
* @param  msg : Message parameter
* @retval Return Status
*/
void Custom_IPC_Send(uint32_t id,uint32_t msg)
{

  if (CurrentCore == CPU1)
  {
    CPU_Messages[0].id = id;
    CPU_Messages[0].msg = msg;  
    HAL_HSEM_FastTake(HSEM_ID_1);   
    /*Release HSEM in order to notify the CPU2(CM4)*/     
    HAL_HSEM_Release(HSEM_ID_1,0);
  }
  else if (CurrentCore == CPU2)
  {
    CPU_Messages[1].id = id;
    CPU_Messages[1].msg = msg;  
    HAL_HSEM_FastTake(HSEM_ID_2);   
    /*Release HSEM in order to notify the CPU2(CM4)*/     
    HAL_HSEM_Release(HSEM_ID_2,0);
  }
  else
  {
  }
  
}

/**
* @brief  HSEM Callback,overwride weak HSEM  Callback
* @param  SemMask : Semaphore Mask 
* @retval None
*/
void HAL_HSEM_FreeCallback(uint32_t SemMask)
{
  if(CurrentCore == CPU1)
  {
    /* Message Received*/
    /*Activate Notification for next message */
    HAL_HSEM_ActivateNotification(__HAL_HSEM_SEMID_TO_MASK(HSEM_ID_2));
    
    CustomIpcCallbackFunct(CPU_Messages[1].id, CPU_Messages[1].msg);
  }
  else if(CurrentCore == CPU2)
  {
    /* Message Received*/
    /*Activate Notification for next message */
    HAL_HSEM_ActivateNotification(__HAL_HSEM_SEMID_TO_MASK(HSEM_ID_1));
    
    CustomIpcCallbackFunct(CPU_Messages[0].id, CPU_Messages[0].msg);
  }
}

/**
  * @}
  */

/**
  * @}
  */
  
