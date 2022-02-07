/**
  ******************************************************************************
  * @file    Demonstrations/extra_modules/ST/Core/CM4/Src/fw_motion.c
  * @author  MCD Application Team
  * @brief   fw motion implementation module
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
#ifdef USE_STM32H747I_DISCO

#include <stm32h7xx_hal.h>

#include <stm32h747i_discovery.h>
#include <stm32h747i_discovery_camera.h>

#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>

#include <cm_ipc.h>

#include <motion_detection.h>

extern uint8_t CAMERA_FRAME_BUFFER[];

static QueueHandle_t frames_q;

static bool motion_initialize(void)
{
    struct ipc_msg msg;

    /* initialize the camera */
    if (BSP_CAMERA_Init(RESOLUTION_R160x120) != CAMERA_OK)
        msg.m.state = IPC_CAMERA_ERROR;
    else {
        frames_q = xQueueCreate(16, sizeof(struct ipc_msg));
        configASSERT(frames_q);

        /* notify the CM7 core */
        msg.m.state = IPC_CAMERA_READY;
        ipc_sendmsg(&msg, sizeof(msg));
    }

    return (msg.m.state == IPC_CAMERA_READY);
}

static void motion_terminate()
{
    if (frames_q)
        vQueueDelete(frames_q);
}

static void motion_mainloop(void)
{
    struct ipc_msg msg;

    for (;;)
    {
        size_t size = ipc_recvmsg(&msg, sizeof(msg), -1);
        configASSERT(size == sizeof(msg));

        if (msg.m.command == IPC_CAMERA_ACQUIRE) {
            BSP_CAMERA_ContinuousStart((uint8_t*)CAMERA_FRAME_BUFFER);
            break;
        }
    }

    for (;;)
    {
        /* peek for IPC_CAMERA_SHUTDOWN */
        size_t size = ipc_recvmsg(&msg, sizeof(msg), 0);

        if (size == sizeof(msg)
            && msg.m.command == IPC_CAMERA_SHUTDOWN)
            break; /* exit main_loop */

        /* wait for a frame to be acquired */
        xQueueReceive(frames_q, &msg, portMAX_DELAY);

        /* msg.MotionX and msg.MotionY will return the motion vector */
        MotionDetect(&msg);

        /* notify CM7 core that a new frame is available */
        msg.m.state = IPC_CAMERA_PICT_READY;
        ipc_sendmsg(&msg, sizeof(msg));

        /* resume capture */
        BSP_CAMERA_Resume();
    }
}

static void BSP_CAMERA_FrameEventCallback(void)
{
    portBASE_TYPE px = pdFALSE;
    struct ipc_msg msg;

    BSP_CAMERA_Suspend();

    xQueueSendFromISR(frames_q, &msg, &px);
    portEND_SWITCHING_ISR(px);
}

struct firmware_desc motion_fw = {
    .uid = IPC_MOTION_FIRMWARE_ID,
    .initialize = motion_initialize,
    .terminate = motion_terminate,
    .main_loop = motion_mainloop,
    .dispatch = NULL
};

#endif
