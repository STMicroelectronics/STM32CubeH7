/**
  ******************************************************************************
  * @file    cm_ipc.h
  * @author  MCD Application Team
  * @brief   Header for cm_ipc.h file
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
#ifndef __CMx_IPC_H
#define __CMx_IPC_H

#include <stdbool.h>

/* IPC messages to be defined by App */
#define IPC_COMMAND_RENDER_FRAME        1
#define IPC_COMMAND_FRAME_ACK           2

#define IPC_CAMERA_INVAL                3
#define IPC_CAMERA_READY                4
#define IPC_CAMERA_ERROR                5
#define IPC_CAMERA_PICT_READY           6
#define IPC_CAMERA_ACQUIRE              7
#define IPC_CAMERA_SHUTDOWN             8

#define IPC_CTRL_SELECT_FIRMWARE         10
#define IPC_CTRL_FIRMWARE_STATUS         11

#define IPC_FRACTAL_FIRMWARE_ID          (1 << 16)
#define IPC_MOTION_FIRMWARE_ID           (2 << 16)

enum ipc_firmware_status {
    IPC_OK = 0,
    IPC_FAILED,
    IPC_NOT_FOUND
};

/* struct ipc_msg to be defined once for all firmwares. */
struct ipc_msg {
    union {
        uint32_t command;
        uint32_t state;
    } m;
    uint32_t arg1;
    uint32_t arg2;
    uint32_t arg3;
    uint32_t arg4;
    uint32_t arg5;
    uint32_t arg6;
    int32_t MotionX;
    int32_t MotionY;
};

int ipc_init(void);
void ipc_isr(void);

size_t ipc_sendmsg(struct ipc_msg* msg, uint32_t size);
size_t ipc_recvmsg(struct ipc_msg* msg, uint32_t size, int32_t timeout);

#ifdef CORE_CM4
struct fw_list {
    struct fw_list *next;
};

struct firmware_desc {
    struct fw_list list;
    uint32_t uid; /* a unique identifier per firmware */
    bool (*initialize)(void);
    void (*terminate)(void);
    void (*main_loop)(void); /* firmware's main_loop function. If NULL messages will be forwarded to the firmware using dispatch */
    void (*dispatch)(struct ipc_msg* msg, uint32_t size);
};

void register_firmware(struct firmware_desc* desc);
int select_firmware(uint32_t uid);

extern struct firmware_desc *current_fw;

extern struct firmware_desc fractal_fw;
#ifdef USE_STM32H747I_DISCO
extern struct firmware_desc motion_fw;
#endif
#endif

#endif
