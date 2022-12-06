/**
  ******************************************************************************
  * @file    cm_ipc.c
  * @author  MCD Application Team
  * @brief   Cortex-M IPC implementation module
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
#include "stm32h7xx_hal.h"

#include <cmsis_os.h>
#include <message_buffer.h>

#include <stdio.h>

#include "cm_ipc.h"

#define HSEM_PROCESS            12U
#define HSEM_SYNC_ID            (7U) /* use HW semaphore 7*/
#define STIPC_SYNC_EVENT        ((HSEM_SYNC_ID << 8) | HSEM_PROCESS)

#ifdef CORE_CM7
#define STIPC_LED               LED_GREEN
#define HSEM_IRQn               HSEM1_IRQn
#define HSEM_TX_ID              (8U) /* use HW semaphore 8*/
#define HSEM_RX_ID              (9U) /* use HW semaphore 9*/
#elif CORE_CM4
#define STIPC_LED               LED_BLUE
#define HSEM_IRQn               HSEM2_IRQn
#define HSEM_TX_ID              (9U) /* use HW semaphore 8*/
#define HSEM_RX_ID              (8U) /* use HW semaphore 9*/
#else
#error Invalid Cortex-M configuration
#endif

#define TASK_MSG_BUFFER_SIZE        (16 * (sizeof(struct ipc_msg) + 4))
#define CONTROL_MSG_BUFFER_SIZE     24
#if defined(__ICCARM__) 
#ifdef CORE_CM7
#pragma location = 0x10040000
static MessageBufferHandle_t xtx_ctrl_buf = 0;
#pragma location = 0x10040004
static MessageBufferHandle_t xtx_data_buf = 0;
#pragma location = 0x10040008
static MessageBufferHandle_t xrx_ctrl_buf = 0;
#pragma location = 0x1004000c
static MessageBufferHandle_t xrx_data_buf = 0;
#elif CORE_CM4
#pragma location = 0x10040000
__no_init MessageBufferHandle_t xrx_ctrl_buf;
#pragma location = 0x10040004
__no_init MessageBufferHandle_t xrx_data_buf;
#pragma location = 0x10040008
__no_init MessageBufferHandle_t xtx_ctrl_buf;
#pragma location = 0x1004000c
__no_init MessageBufferHandle_t xtx_data_buf;
#else
#error Invalid Cortex-M configuration
#endif
#elif defined(__CC_ARM)
#ifdef CORE_CM7
static MessageBufferHandle_t xtx_ctrl_buf __attribute__((at(0x10040000))) = 0;
static MessageBufferHandle_t xtx_data_buf __attribute__((at(0x10040004)))= 0;
static MessageBufferHandle_t xrx_ctrl_buf __attribute__((at(0x10040008))) = 0;
static MessageBufferHandle_t xrx_data_buf __attribute__((at(0x1004000c))) = 0;
#elif CORE_CM4
MessageBufferHandle_t xrx_ctrl_buf __attribute__((at(0x10040000), zero_init));
MessageBufferHandle_t xrx_data_buf __attribute__((at(0x10040004), zero_init));
MessageBufferHandle_t xtx_ctrl_buf __attribute__((at(0x10040008), zero_init));
MessageBufferHandle_t xtx_data_buf __attribute__((at(0x1004000c), zero_init));
#else
#error Invalid Cortex-M configuration
#endif


#elif defined(__GNUC__)
#ifdef CORE_CM7
static MessageBufferHandle_t xtx_ctrl_buf __attribute__((section("xtx_ctrl_Section"))) = 0;
static MessageBufferHandle_t xtx_data_buf __attribute__((section("xtx_data_Section")))= 0;
static MessageBufferHandle_t xrx_ctrl_buf __attribute__((section("xrx_ctrl_Section"))) = 0;
static MessageBufferHandle_t xrx_data_buf __attribute__((section("xrx_data_Section"))) = 0;
#elif CORE_CM4
MessageBufferHandle_t xrx_ctrl_buf __attribute__((section("xtx_ctrl_Section")));
MessageBufferHandle_t xrx_data_buf __attribute__((section("xtx_data_Section")));
MessageBufferHandle_t xtx_ctrl_buf __attribute__((section("xrx_ctrl_Section")));
MessageBufferHandle_t xtx_data_buf __attribute__((section("xrx_data_Section")));
#else
#error Invalid Cortex-M configuration
#endif
#endif
static osMessageQId  STIPCEvent = 0;

void vGenerateRemoteInterrupt(void * xUpdatedMessageBuffer);
static void prvCoreInterruptHandler(void);

/**
  * @brief Semaphore Released Callback.
  * @param SemMask: Mask of Released semaphores
  * @retval None
  */
void HAL_HSEM_FreeCallback(uint32_t SemMask)
{
  if((SemMask &  __HAL_HSEM_SEMID_TO_MASK(HSEM_RX_ID))!= 0)
  {
    // printf("CPU%d : Received new IPC message.\n", (HAL_GetCurrentCPUID() == HSEM_CPU1_COREID ? 1:2));

    /* Re-Activate HSEM notification for Cortex-M4*/
    HAL_HSEM_ActivateNotification(__HAL_HSEM_SEMID_TO_MASK(HSEM_RX_ID));

    /* Toggle STIPC LED */
    // BSP_LED_Toggle(STIPC_LED);

    prvCoreInterruptHandler();
  }
  else if((SemMask &  __HAL_HSEM_SEMID_TO_MASK(HSEM_SYNC_ID))!= 0)
  {
    // printf("CPU%d : Received new Sync message.\n", (HAL_GetCurrentCPUID() == HSEM_CPU1_COREID ? 1:2));

    /* Re-Activate HSEM notification for Cortex-M4*/
    HAL_HSEM_ActivateNotification(__HAL_HSEM_SEMID_TO_MASK(HSEM_SYNC_ID));

    if (STIPCEvent && osKernelRunning())
    {
      osMessagePut ( STIPCEvent, STIPC_SYNC_EVENT, 0);
    }
  }
}

int ipc_init(void)
{
    /* Create STIPC Message Queue */
    osMessageQDef(STIPC_QUEUE, 10, uint16_t);
    STIPCEvent = osMessageCreate (osMessageQ(STIPC_QUEUE), NULL);

    /* Enable HSEM Interrupt */
    HAL_NVIC_SetPriority(HSEM_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(HSEM_IRQn);

      /*HW semaphore Clock enable*/
    __HAL_RCC_HSEM_CLK_ENABLE();

    HAL_HSEM_ActivateNotification(__HAL_HSEM_SEMID_TO_MASK(HSEM_SYNC_ID));
    HAL_HSEM_ActivateNotification(__HAL_HSEM_SEMID_TO_MASK(HSEM_RX_ID));

#ifdef CORE_CM4
    xtx_ctrl_buf = xMessageBufferCreate(CONTROL_MSG_BUFFER_SIZE);
    configASSERT(xtx_ctrl_buf);
    xrx_ctrl_buf = xMessageBufferCreate(CONTROL_MSG_BUFFER_SIZE);
    configASSERT(xrx_ctrl_buf);
    xtx_data_buf = xMessageBufferCreate(TASK_MSG_BUFFER_SIZE);
    configASSERT(xtx_data_buf);
    xrx_data_buf = xMessageBufferCreate(TASK_MSG_BUFFER_SIZE);
    configASSERT(xrx_data_buf);
#endif
#ifdef CORE_CM7
    /* Take the HW Semaphore with Process1 ID  */
    if(HAL_HSEM_Take(HSEM_SYNC_ID, HSEM_PROCESS) == HAL_OK)
    {
        /* Release the HW Semaphore */  
        HAL_HSEM_Release(HSEM_SYNC_ID, HSEM_PROCESS);
    }
#endif

    if(osKernelRunning())
    {
      osEvent event = osMessageGet( STIPCEvent, osWaitForever );
      if( event.status == osEventMessage )
      {
        printf("CPU%d : Received Sync Event.\n", (HAL_GetCurrentCPUID() == HSEM_CPU1_COREID ? 1:2));
      }
    }

    return 0;
}

static void prvCoreInterruptHandler(void)
{
    MessageBufferHandle_t xUpdatedMessageBuffer;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    if (!xrx_ctrl_buf)
        return;

    /* xControlMessageBuffer contains the handle of the message buffer that
       contains data. */
    if (xMessageBufferReceiveFromISR(xrx_ctrl_buf,
                                     &xUpdatedMessageBuffer,
                                     sizeof( xUpdatedMessageBuffer),
                                     &xHigherPriorityTaskWoken)
        == sizeof(xUpdatedMessageBuffer))
    {
        /* Call the API function that sends a notification to any task that is
           blocked on the xUpdatedMessageBuffer message buffer waiting for data
           to arrive. */
        xMessageBufferSendCompletedFromISR(xUpdatedMessageBuffer,
                                           &xHigherPriorityTaskWoken);
    }

    /* Normal FreeRTOS yield from interrupt semantics, where
       xHigherPriorityTaskWoken is initialized to pdFALSE and will then get set
       to pdTRUE if the interrupt safe API unblocks a task that has a priority
       above that of the currently executing task. */
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void vGenerateRemoteInterrupt(void * xUpdatedMessageBuffer)
{
    MessageBufferHandle_t xUpdatedBuffer =
        (MessageBufferHandle_t)xUpdatedMessageBuffer;

    if (xUpdatedBuffer != xtx_ctrl_buf)
    {
        /* Use xControlMessageBuffer to pass the handle of the message buffer
           written to by core 1 to the interrupt handler about to be generated
           in core 2. */
        xMessageBufferSend(xtx_ctrl_buf, &xUpdatedBuffer,
                           sizeof(xUpdatedBuffer), 0);

        /* Take the HW Semaphore with Process1 ID  */
        if(HAL_HSEM_Take(HSEM_TX_ID, HSEM_PROCESS) == HAL_OK)
        {
            /* Release the HW Semaphore */  
            HAL_HSEM_Release(HSEM_TX_ID, HSEM_PROCESS);
        }
    }
}

void ipc_isr(void)
{
    prvCoreInterruptHandler();
}

size_t ipc_sendmsg(struct ipc_msg* msg, uint32_t size)
{
    if(xtx_data_buf == 0)
      return 0;

    size_t len = xMessageBufferSend(xtx_data_buf, msg, size, portMAX_DELAY);

    return len;
}

size_t ipc_recvmsg(struct ipc_msg* msg, uint32_t size, int32_t timeout)
{
    size_t len;

    if (timeout < 0)
        len = xMessageBufferReceive(xrx_data_buf, msg, size, portMAX_DELAY);
    else
        len = xMessageBufferReceive(xrx_data_buf, msg, size, (TickType_t)timeout);

    return len;
}

#ifdef CORE_CM4
static struct fw_list fw_head;

struct firmware_desc *current_fw = NULL;

#define container_of(ptr, type, member) \
        (struct firmware_desc *)((char *)ptr - offsetof(type, member))

void register_firmware(struct firmware_desc* desc)
{
    desc->list.next = fw_head.next;
    fw_head.next = &desc->list;
}

int select_firmware(uint32_t uid)
{
    /* if uid == 0 then unlink firmware and return */
    if (current_fw && uid == 0) {
        current_fw->terminate();
        current_fw = NULL;
        return IPC_OK;
    }

    struct firmware_desc *new_fw = NULL;
    struct fw_list *fw = fw_head.next;

    /* lookup the requested uid */
    while (fw) {
        struct firmware_desc *desc =
            container_of(fw, struct firmware_desc, list);
        if (desc && desc->uid == uid) {
            new_fw = desc;
            break;
        }
        fw = fw->next;
    }

    if (!new_fw)
        return -IPC_NOT_FOUND; /* requested firmware not found */
    else {
        /* switch to new firmware */
        if (!new_fw->initialize())
            return -IPC_FAILED; /* failed initializing new firmware */

        /* terminate the previously active firmware */
        if (current_fw)
            current_fw->terminate();

        current_fw = new_fw;

        return IPC_OK;
    }
}
#endif
