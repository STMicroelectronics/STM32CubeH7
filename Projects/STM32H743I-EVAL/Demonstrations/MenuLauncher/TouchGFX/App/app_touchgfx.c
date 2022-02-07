/**
  ******************************************************************************
  * File Name          : app_touchgfx.c
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

#include "app_touchgfx.h"

void touchgfx_init(void);
void touchgfx_taskEntry(void);

/**
 * Initialize TouchGFX application
 */
void MX_TouchGFX_Init(void)
{
  // Calling farward to touchgfx_init in C++ domain
  touchgfx_init();
}

/**
 * TouchGFX application entry function
 */
void MX_TouchGFX_Process(void)
{
  // Calling farward to touchgfx_init in C++ domain
  touchgfx_taskEntry();
}

/**
 * TouchGFX application thread
 */
void TouchGFX_Task(void *argument)
{
  // Calling farward to touchgfx_init in C++ domain
  touchgfx_taskEntry();
}

