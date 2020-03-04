
#include <touchgfx/hal/HAL.hpp>
#include <touchgfx/hal/BoardConfiguration.hpp>
#include <touchgfx/canvas_widget_renderer/CanvasWidgetRenderer.hpp>

using namespace touchgfx;

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "storage_app.h"

#ifdef STM32H743xx
#ifdef __GNUC__
#define LOCATION_DTCM_ATTRIBUTE __attribute__ ((section ("SDIOSection"))) __attribute__ ((aligned(4)))
#define LOCATION_DTCM_PRAGMA
#elif defined(__arm__)
#define LOCATION_DTCM_ATTRIBUTE __attribute__ ((section ("SDIOSection"))) __attribute__ ((aligned(4)))
#define LOCATION_DTCM_PRAGMA
#else
#include <intrinsics.h>
#define LOCATION_DTCM_PRAGMA _Pragma("location=\"SDIOSection\"")
#define LOCATION_DTCM_ATTRIBUTE
#endif
#endif

LOCATION_DTCM_PRAGMA uint8_t ucHeap[ configTOTAL_HEAP_SIZE ] LOCATION_DTCM_ATTRIBUTE;

/**
 * Define the FreeRTOS task priorities and stack sizes
 */
#define configGUI_TASK_PRIORITY                 ( tskIDLE_PRIORITY + 1 )
#define configGUI_TASK_STK_SIZE                 ( 8 * 1024 )

static void GUITask(void* params)
{
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

  vTaskStartScheduler();

  for(;;);

}

