/**
  ******************************************************************************
  * @file    ResourcesManager/ResourcesManager_UsageWithNotification/Common/Inc/cutom_ipc.h
  * @author  MCD Application Team
  * @brief   This file contains all the functions prototypes for the main.c
  *          file.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
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


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
