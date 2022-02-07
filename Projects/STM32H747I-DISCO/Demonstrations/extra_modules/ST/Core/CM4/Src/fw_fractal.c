/**
  ******************************************************************************
  * @file    fw_fractal.c
  * @author  MCD Application Team
  * @brief   fractal implementation module
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

#include <stdlib.h>
#include <stdint.h>

#include <FreeRTOS.h>
#include <task.h>

#include <cm_ipc.h>

static uint8_t Fractalbuffer[160 * 120];

#define ITERATION       ((uint32_t)256)
#define REAL_CONSTANT   (0.285f)
#define IMG_CONSTANT    (0.01f)

static void render_julia(uint16_t offset_x, uint16_t offset_y, uint16_t zoom, uint8_t **buffer)
{
    float tmp1, tmp2;
    float num_real, num_img;
    float radius;
    uint8_t i;
    uint16_t x, y;

    for (y = 0; y < 120; y++) {
        for (x = 0; x < 160; x++) {
            num_real = y - offset_y;
            num_real = num_real / zoom;
            num_img = x - offset_x;
            num_img = num_img / zoom;
            i = 0;
            radius = 0;

            while ((i < ITERATION - 1) && (radius < 4)) {
                tmp1 = num_real * num_real;
                tmp2 = num_img * num_img;
                num_img = 2 * num_real * num_img + IMG_CONSTANT;
                num_real = tmp1 - tmp2 + REAL_CONSTANT;
                radius = tmp1 + tmp2;
                i++;
            }

            Fractalbuffer[x + y * 160] = i;
        }
    }

    /* return the address of the local buffer in D2 SRAM1 */
    *buffer = Fractalbuffer;
}

static bool fractal_initialize(void)
{
    return true;
}

static void fractal_terminate(void)
{
}

static void fractal_dispatch(struct ipc_msg* msg, uint32_t size)
{
    if (msg->m.command == IPC_COMMAND_RENDER_FRAME)
    {
        uint8_t *ret_buf;

        /* render a new frame into the shared buffer */
        render_julia((uint16_t)msg->arg1, (uint16_t)msg->arg2,
                     (uint16_t)msg->arg3, &ret_buf);

        /* notify CM7 that a new frame is available */
        msg->m.state = IPC_COMMAND_FRAME_ACK;
        msg->arg1 = (uint32_t)ret_buf;

        size_t len = ipc_sendmsg(msg, sizeof(*msg));
        configASSERT(len == sizeof(*msg));
    }
}

struct firmware_desc fractal_fw = {
    .uid = IPC_FRACTAL_FIRMWARE_ID,
    .initialize = fractal_initialize,
    .terminate = fractal_terminate,
    .main_loop = NULL,
    .dispatch = fractal_dispatch
};
