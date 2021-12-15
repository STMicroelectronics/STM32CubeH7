/**
  ******************************************************************************
  * @file    ResourcesManager/ResourcesManager_UsageWithNotification/Common/Inc/cutom_ipc.h
  * @author  MCD Application Team
  * @brief   This file contains all the functions prototypes for the main.c
  *          file.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef CUSTOM_IPC_H
#define CUSTOM_IPC_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Exported define -----------------------------------------------------------*/

#define HSEM_ID_1 (1U) /* HW semaphore 1: CPU1->CPU2*/
#define HSEM_ID_2 (2U) /* HW semaphore 2: CPU2->CPU1*/

#define CPU1 0x1
#define CPU2 0x2

/* Exported types  -----------------------------------------------------------*/  
typedef struct  __CPU_message_t
{
  uint32_t id;
  uint32_t msg;
} CPU_message_t;

typedef void (* CustomIpcCallback_t ) (uint32_t id, uint32_t msg); 

typedef enum
{  CUSTOMIPC_OK       = 0x00U,  
   CUSTOMIPC_ERROR    = 0x01U,  
}CustIpc_Status_t;

extern CPU_message_t  CPU_Messages[2]; /* CPU_Messages[0]: CPU1->CPU2 */
                                       /* CPU_Messages[1]: CPU2->CPU1 */

/* Exported function prototypes ----------------------------------------------*/


CustIpc_Status_t Custom_IPC_Init(uint32_t core,CustomIpcCallback_t Callback);
CustIpc_Status_t Custom_IPC_DeInit(uint32_t core);
void Custom_IPC_Send(uint32_t id,uint32_t msg);

#ifdef __cplusplus
}
#endif

#endif /* CUSTOM_IPC_H */


