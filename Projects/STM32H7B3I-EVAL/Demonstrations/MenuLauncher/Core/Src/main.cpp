/**
  ******************************************************************************
  * @file    main.cpp
  * @author  MCD Application Team
  * @brief   This file provides main program functions
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

#include <touchgfx/hal/HAL.hpp>
#include <touchgfx/hal/BoardConfiguration.hpp>
#include <touchgfx/canvas_widget_renderer/CanvasWidgetRenderer.hpp>
using namespace touchgfx;

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "bsp.h"

typedef  void (*pFunc)(void);

/**
 * Define the FreeRTOS task priorities and stack sizes
 */
#define configGUI_TASK_PRIORITY                 ( tskIDLE_PRIORITY + 3 )

#define configGUI_TASK_STK_SIZE                 ( 4096 )

static void GUITask(void* params)
{
    touchgfx::HAL::getInstance()->taskEntry();
}

int main(void)
{
  
    uint32_t JumpAdress = 0;
  
    HAL_PWR_EnableBkUpAccess();
    JumpAdress  = READ_REG(BKP_REG_SUBDEMO_ADDRESS);
    WRITE_REG(BKP_REG_SUBDEMO_ADDRESS, 0);  
    HAL_PWR_DisableBkUpAccess();
  
    if(JumpAdress != 0) 
    {
    
      /* Reinitialize the Stack pointer*/
      __set_MSP(*(__IO uint32_t*) JumpAdress);
      /* Jump to application address */
      ((pFunc) (*(__IO uint32_t*) (JumpAdress + 4)))();
    }

    hw_init();
    touchgfx_init();

    /**
     * IMPORTANT NOTICE!
     *
     * The GUI task stack size might need to be increased if creating a new application.
     * The current value of 950 in this template is enough for the examples distributed with
     * TouchGFX, but is often insufficient for larger applications.
     * For reference, the touchgfx_2014 demo uses a stacksize of 2000.
     * If you experience inexplicable hard faults with your custom application, try increasing the
     * stack size.
     *
     * Also note that in FreeRTOS, stack size is in words. So a stack size of 950 actually consumes
     * 3800 bytes of memory. The configTOTAL_HEAP_SIZE define in platform/os/FreeRTOSConfig.h configures
     * the total amount of memory available for FreeRTOS, including task stacks. This value is expressed
     * in BYTES, not words. So, with a stack size of 950, your total heap should be at least 4K.
     */
    xTaskCreate(GUITask, "GUITask",
                configGUI_TASK_STK_SIZE,
                NULL,
                configGUI_TASK_PRIORITY,
                NULL);

    vTaskStartScheduler();

    for (;;);
}
