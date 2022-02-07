/**
  ******************************************************************************
  * @file    main.c
  * @author  MCD Application Team
  * @brief   Cortex-M4 main module
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

#include "main.h"
#include "bsp.h"
#include <cm_ipc.h>

#if configAPPLICATION_ALLOCATED_HEAP == 1
#if defined ( __ICCARM__ )
#pragma location="heap_mem"
#else
__attribute__((section("heap_mem")))
#endif
uint8_t ucHeap[ configTOTAL_HEAP_SIZE ];
#endif /* configAPPLICATION_ALLOCATED_HEAP */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define HSEM_ID_0 (0U) /* HW semaphore 0*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t BSP_Initialized = 0;

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

static void prvCore2Task(void *pvParameters);

int main(void)
{
    /* Enable the ART cache for the 0x08100000 (1M) range */
    __HAL_RCC_ART_CLK_ENABLE();
    while ((RCC->AHB1ENR & (0x00004000)) != 0x00004000) {}
    *(uint32_t*)(0x40024400) = 0x00008101;

    /* HW semaphore Clock enable */
    __HAL_RCC_HSEM_CLK_ENABLE();

    /* Activate HSEM notification for Cortex-M4*/
    HAL_HSEM_ActivateNotification(__HAL_HSEM_SEMID_TO_MASK(HSEM_ID_0));

    /* 
      Domain D2 goes to STOP mode (Cortex-M4 in deep-sleep) waiting for Cortex-M7 to
      perform system initialization (system clock config, external memory configuration.. )   
    */
    HAL_PWREx_EnterSTOPMode(PWR_MAINREGULATOR_ON, PWR_STOPENTRY_WFE, PWR_D2_DOMAIN);

    /* Clear STOP flag */
    __HAL_PWR_CLEAR_FLAG(PWR_CPU_FLAGS);

    /* STM32H7xx HAL library initialization:
       - Systick timer is configured by default as source of time base, but user 
         can eventually implement his proper time base source (a general purpose 
         timer for example or other time source), keeping in mind that Time base 
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
         handled in milliseconds basis.
       - Set NVIC Group Priority to 4
       - Low Level Initialization
     */
    HAL_Init();

    /* WA : Allow CM7 to boot */
    HAL_Delay(1000);

    BSP_Initialized = BSP_Config();

    if(BSP_Initialized)
    {
        xTaskCreate(prvCore2Task, "AMPCore2", configMINIMAL_STACK_SIZE, (void*)NULL, tskIDLE_PRIORITY + 1, NULL);

        /* Start scheduler */
        vTaskStartScheduler();
    }

    /* Infinite loop */
    for (;;);
}

static void prvCore2Task(void *pvParameters)
{
    struct ipc_msg msg;
    int ret;

    /* Busy loop, until FreeRTOS/AMP handles get initialized */
    ipc_init();

    register_firmware(&fractal_fw);
#ifdef USE_STM32H747I_DISCO
    register_firmware(&motion_fw);
#endif /* USE_STM32H747I_DISCO */

    for (;;)
    {
        /* wait for a message from the CM7 core */
        size_t len = ipc_recvmsg(&msg, sizeof(msg), -1);
        
        if(len != sizeof(msg))
          Error_Handler();

        switch (msg.m.command) {
        case IPC_CTRL_SELECT_FIRMWARE:
            /* select a new firmware */
            ret = select_firmware(msg.arg1);

            /* forward status to the CM7 host */
            msg.m.state = IPC_CTRL_FIRMWARE_STATUS;
            msg.arg1 = ret;
            len = ipc_sendmsg(&msg, sizeof(msg));
            if(len != sizeof(msg))
              Error_Handler();

            if (!ret) {
                if (current_fw && current_fw->main_loop)
                    current_fw->main_loop(); /* doesn't return until firmware yields control */
            }
            break;
        default:
            /* forward message to current firmware */
            if (current_fw)
                current_fw->dispatch(&msg, len);
            break;
        }
    }
}

/* Handler for IPI interrupts that are triggered by CM7 core */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if(!BSP_Initialized)
      return;

    switch(GPIO_Pin)
    {
        case BUTTON_WAKEUP_PIN :
        {
            /* Toggle LED GREEN */
            BSP_LED_Toggle(LED_ORANGE);
        }
        break;

        default:
            break;
    }
}

HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
{
    /*Configure the SysTick to have interrupt in 1ms time basis*/
    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);

    /*Configure the SysTick IRQ priority */
    HAL_NVIC_SetPriority(SysTick_IRQn, TickPriority ,0);

    /* Return function status */
    return HAL_OK;
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* Turn LED RED on */
  if(BSP_Initialized)
      BSP_LED_On(LED_ORANGE);

  configASSERT(0);
}

void BSP_ErrorHandler(void)
{
  // FIXME : We just ignore the BSP errors because we got many I2C timeout errors
  // Error_Handler();

  if(BSP_Initialized)
	BSP_LED_On(LED_ORANGE);
}

#ifdef configUSE_MALLOC_FAILED_HOOK
/**
  * @brief  Application Malloc failure Hook
  * @param  None
  * @retval None
  */
void vApplicationMallocFailedHook(TaskHandle_t xTask, char *pcTaskName)
{
  printf( "%s(): MALLOC FAILED !!!\n", pcTaskName );

  Error_Handler();
}
#endif /* configUSE_MALLOC_FAILED_HOOK */

#ifdef configCHECK_FOR_STACK_OVERFLOW
void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName )
{
  printf( "%s(): STACK OVERFLOW !!!\n", pcTaskName );

  Error_Handler();
}
#endif /* configCHECK_FOR_STACK_OVERFLOW */

#ifdef USE_FULL_ASSERT
/**
* @brief  assert_failed
*         Reports the name of the source file and the source line number
*         where the assert_param error has occurred.
* @param  File: pointer to the source file name
* @param  Line: assert_param error line source number
* @retval None
*/
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line
  number,ex: printf("Wrong parameters value: file %s on line %d\r\n", 
  file, line) */
  
  /* Infinite loop */
  Error_Handler();
}
#endif

