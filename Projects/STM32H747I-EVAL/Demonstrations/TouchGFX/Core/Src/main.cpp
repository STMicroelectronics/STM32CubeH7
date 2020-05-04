/**
  ******************************************************************************
  * @file    main.cpp 
  * @author  MCD Application Team
  * @brief   This file provides main program functions
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


#include <touchgfx/hal/HAL.hpp>
#include <touchgfx/hal/BoardConfiguration.hpp>
#include <touchgfx/canvas_widget_renderer/CanvasWidgetRenderer.hpp>
using namespace touchgfx;

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include <bsp.h>
#include <storage_app.h>

#include <rtctask.hpp>

/**
 * Define the FreeRTOS task priorities and stack sizes
 */
#define configGUI_TASK_PRIORITY                 ( tskIDLE_PRIORITY + 3 )

#define configGUI_TASK_STK_SIZE                 ( 4048 )

extern xQueueHandle GUITaskQueue;

static void GUITask(void* params)
{
  GUITaskQueue = xQueueGenericCreate( 5, 1, 0 );
  BspInit();
  STORAGE_Init();
  touchgfx::HAL::getInstance()->taskEntry();
}

int main (void)
{
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
  xTaskCreate( GUITask, "GUITask",
               configGUI_TASK_STK_SIZE,
               NULL,
               configGUI_TASK_PRIORITY,
               NULL);
  
  xTaskCreate( RTCTask, "RTCTask",
               512,
               NULL,
               configGUI_TASK_PRIORITY - 1,
               NULL);

  vTaskStartScheduler();

  for(;;);

}

#ifdef USE_STM32H747I_EVAL
#define MFX_IRQ_PENDING_GPIO    0x01

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin == GPIO_PIN_8)
  {
    uint32_t irqPending;

    /* Disable MFX interrupt to manage current one */
    HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);

    irqPending =MFXSTM32L152_GlobalITStatus(&Io_CompObj, MFXSTM32L152_REG_ADR_IRQ_PENDING);

    /* GPIO IT from MFX */
    if(irqPending & MFX_IRQ_PENDING_GPIO)
    {
      uint32_t statusGpio = MFXSTM32L152_IO_ITStatus(&Io_CompObj,TS_INT_PIN | SD_DETECT_PIN);

      if(statusGpio & TS_INT_PIN)
      {
        /* Update GFX Library state machine according to touch acquisition ? */
      }

      else if(statusGpio & SD_DETECT_PIN)
      {
        if(BSP_SD_IsDetected(0))
        {
          /* Enable SD Interrupt mode */
          BSP_SD_Init(0);
          BSP_SD_DetectITConfig(0);
          osMessagePut(StorageEvent, MSDDISK_CONNECTION_EVENT, 0);
        }
        else
        {
          osMessagePut(StorageEvent, MSDDISK_DISCONNECTION_EVENT, 0);
        }
      }
      else
      {
        while (1) { ; }
      }

       BSP_IO_ClearIT(0,statusGpio);
    }

    /* Re-enable MFX interrupt */
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
  }
}
#endif // USE_STM32H747I_EVAL
