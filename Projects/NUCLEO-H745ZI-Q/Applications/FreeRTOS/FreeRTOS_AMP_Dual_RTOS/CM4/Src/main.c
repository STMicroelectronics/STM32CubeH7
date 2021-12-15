/**
  ******************************************************************************
  * @file    FreeRTOS/FreeRTOS_AMP_Dual_RTOS/CM4/Src/main.c
  * @author  MCD Application Team
  *          This is the main program for Cortex-M4 
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* Standard includes. */
#include "stdio.h"
#include "string.h"

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "message_buffer.h"
#include "MessageBufferAMP.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define HSEM_ID_0 (0U) /* HW semaphore 0*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Set to pdFALSE if any errors are detected.  Used to inform the check task
that something might be wrong. */
BaseType_t xDemoStatus = pdPASS;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static void Error_Handler(void);
static void prvCore2Tasks( void *pvParameters );
static void prvCore2InterruptHandler( void );
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, uint32_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  BaseType_t x;

 /*HW semaphore Clock enable*/
  __HAL_RCC_HSEM_CLK_ENABLE();
 
 /* Activate HSEM notification for Cortex-M4*/
  HAL_HSEM_ActivateNotification(__HAL_HSEM_SEMID_TO_MASK(HSEM_ID_0));
  
  /* 
    Domain D2 goes to STOP mode (Cortex-M4 in deep-sleep) waiting for Cortex-M7 to
    perform system initialization (system clock config, external memory configuration.. )   
  */
  HAL_PWREx_ClearPendingEvent();
  HAL_PWREx_EnterSTOPMode(PWR_MAINREGULATOR_ON, PWR_STOPENTRY_WFE, PWR_D2_DOMAIN);

  /* Clear HSEM flag */
  __HAL_HSEM_CLEAR_FLAG(__HAL_HSEM_SEMID_TO_MASK(HSEM_ID_0));

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
 
  HAL_NVIC_SetPriority(EXTI0_IRQn, 0xFU, 0U);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);
  
  if (( xControlMessageBuffer == NULL )|( xDataMessageBuffers[0] == NULL ) | ( xDataMessageBuffers[1] == NULL ))
  {
    Error_Handler();
  }

  for( x = 0; x < mbaNUMBER_OF_CORE_2_TASKS; x++ )
  {    
    /* Pass the loop counter into the created task using the task's
    parameter.  The task then uses the value as an index into the
    ulCycleCounters and xDataMessageBuffers arrays. */
    xTaskCreate( prvCore2Tasks,
                "AMPCore2",
                configMINIMAL_STACK_SIZE,
                ( void * ) x,
                tskIDLE_PRIORITY + 1,
                NULL );
  }
   
  /* Start scheduler */
  vTaskStartScheduler();

  /* We should never get here as control is now taken by the scheduler */
  for (;;);

}

static void prvCore2Tasks( void *pvParameters )
{
  BaseType_t x;
  size_t xReceivedBytes;
  uint32_t ulNextValue = 0;
  char cExpectedString[ 15 ];
  char cReceivedString[ 15 ];
  
  /* The index into the xDataMessageBuffers and ulLoopCounter arrays is
  passed into this task using the task's parameter. */
  x = ( BaseType_t ) pvParameters;
  configASSERT( x < mbaNUMBER_OF_CORE_2_TASKS );
  
  for( ;; )
  {
    /* Create the string that is expected to be received this time round. */
    sprintf( cExpectedString, "%lu", ( unsigned long ) ulNextValue );
    
    /* Wait to receive the next message from core 1. */
    memset( cReceivedString, 0x00, sizeof( cReceivedString ) );
    xReceivedBytes = xMessageBufferReceive( xDataMessageBuffers[ x ],
                                            cReceivedString,
                                            sizeof( cReceivedString ),
                                            portMAX_DELAY );
    
    /* Check the number of bytes received was as expected. */
    configASSERT( xReceivedBytes == strlen( cExpectedString ) );
    
    /* If the received string matches that expected then increment the loop
    counter so the check task knows this task is still running. */
    if( strcmp( cReceivedString, cExpectedString ) == 0 )
    {
      ( ulCycleCounters[ x ] )++;
    }
    else
    {
      xDemoStatus = pdFAIL;
    }
    
    /* Expect the next string in sequence the next time around. */
    ulNextValue++;
  }
}

/* Handler for the interrupts that are triggered on core 1 but execute on core 2. */
static void prvCore2InterruptHandler( void )
{
  MessageBufferHandle_t xUpdatedMessageBuffer;
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  
  /* xControlMessageBuffer contains the handle of the message buffer that
  contains data. */
  if( xMessageBufferReceiveFromISR( xControlMessageBuffer,
                                   &xUpdatedMessageBuffer,
                                   sizeof( xUpdatedMessageBuffer ),
                                   &xHigherPriorityTaskWoken ) == sizeof( xUpdatedMessageBuffer ) )
  {
    /* Call the API function that sends a notification to any task that is
    blocked on the xUpdatedMessageBuffer message buffer waiting for data to
    arrive. */
    xMessageBufferSendCompletedFromISR( xUpdatedMessageBuffer, &xHigherPriorityTaskWoken );
  }
  
  /* Normal FreeRTOS yield from interrupt semantics, where
  xHigherPriorityTaskWoken is initialized to pdFALSE and will then get set to
  pdTRUE if the interrupt safe API unblocks a task that has a priority above
  that of the currently executing task. */
  portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}

/* */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  prvCore2InterruptHandler();
  HAL_EXTI_D2_ClearFlag(EXTI_LINE0);
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* User may add here some code to deal with this error */
  while(1)
  {
  }
}
/* configUSE_STATIC_ALLOCATION is set to 1, so the application must provide an
implementation of vApplicationGetIdleTaskMemory() to provide the memory that is
used by the Idle task. */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, uint32_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  /* If the buffers to be provided to the Idle task are declared inside this
  function then they must be declared static - otherwise they will be allocated on
  the stack and so not exists after this function exits. */
  static StaticTask_t xIdleTaskTCB;
  static uint32_t uxIdleTaskStack[ configMINIMAL_STACK_SIZE ];

      /* Pass out a pointer to the StaticTask_t structure in which the Idle task's
      state will be stored. */
      *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

      /* Pass out the array that will be used as the Idle task's stack. */
      *ppxIdleTaskStackBuffer = uxIdleTaskStack;

      /* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
      Note that, as the array is necessarily of type StackType_t,
      configMINIMAL_STACK_SIZE is specified in words, not bytes. */
      *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}




#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/**
  * @}
  */

