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
/* Includes ------------------------------------------------------------------*/
#include <touchgfx/hal/HAL.hpp>
#include <touchgfx/hal/BoardConfiguration.hpp>
#include <touchgfx/canvas_widget_renderer/CanvasWidgetRenderer.hpp>
using namespace touchgfx;

#include "cmsis_os.h"
#include "storage.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#define configGUI_TASK_STK_SIZE                 ( 4096 )
#define CANVAS_BUFFER_SIZE (3600)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
#if defined ( __GNUC__ )
__attribute__((section(".CanvasSection"))) __attribute__ ((aligned (32)))
#elif defined ( __ICCARM__ )
#pragma data_alignment=32
#pragma location="CanvasSection"
#elif defined( __CC_ARM )
__attribute__((section(".CanvasSection"), zero_init)) __attribute__ ((aligned (32)))
#endif
static uint8_t canvasBuffer[CANVAS_BUFFER_SIZE];
osThreadId     GUITaskId = 0;

static void GUITask(void const* params)
{
  Storage_Init();
  touchgfx::HAL::getInstance()->taskEntry();
}
static char tgfx_thrd_name [] = "TGFX_GUI_Task";
static const osThreadDef_t tgfx_thrd ={ tgfx_thrd_name, GUITask, osPriorityNormal ,0 , configGUI_TASK_STK_SIZE};

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
int main(void)
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
  
  // Setup the CanvasWidgetRenderer. ONLY needed if you use CanvasWidgets
  // in your application. The CANVAS_BUFFER_SIZE can be adjusted to match
  // your needs in performance vs. RAM usage. Read more on this in the
  // TouchGFX Manual.
  CanvasWidgetRenderer::setupBuffer(canvasBuffer, CANVAS_BUFFER_SIZE);

  /* Create GUI task */
  GUITaskId = osThreadCreate(&tgfx_thrd, NULL);
  /* Start scheduler */
  osKernelStart();

  for (;;);
}
