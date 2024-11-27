/**
  ******************************************************************************
  * @file    FreeRTOS/FreeRTOS_AMP_Dual_RTOS/CM7/Src/main.c
  * @author  MCD Application Team
  *          This is the main program for Cortex-M7 
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
#define mainCHECK_TASK_PRIORITY			( configMAX_PRIORITIES - 2 )

/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void CPU_CACHE_Enable(void);
static void Error_Handler(void);
static void MPU_Config(void);

static void prvCore1Task( void *pvParameters );
static void prvCheckTask( void *pvParameters );
static BaseType_t xAreMessageBufferAMPTasksStillRunning( void );
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, uint32_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
   int32_t timeout;
   BaseType_t x;

  /* MPU Configuration */
  MPU_Config();
  /* Enable the CPU Cache */
  CPU_CACHE_Enable();

  /* Wait until CPU2 boots and enters in stop mode or timeout*/
  timeout = 0xFFFF;
  while((__HAL_RCC_GET_FLAG(RCC_FLAG_D2CKRDY) != RESET) && (timeout-- > 0));
  if ( timeout < 0 )
  {
    Error_Handler();
  }

  /* STM32H7xx HAL library initialization:
       - TIM6 timer is configured by default as source of HAL time base, but user
         can eventually implement his proper time base source (another general purpose
         timer for application or other time source), keeping in mind that Time base
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and
         handled in milliseconds basis.
         This application uses FreeRTOS, the RTOS initializes the systick to generate an interrupt each 1ms. 
         The systick is then used for FreeRTOS time base.
       - Set NVIC Group Priority to 4
       - Low Level Initialization
  */
  HAL_Init();
  
  /* Configure the system clock to 400 MHz */
  SystemClock_Config();
  
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);
  
  /* AIEC Common configuration: make CPU1 and CPU2 SWI line0
  sensitive to rising edge : Configured only once */
  HAL_EXTI_EdgeConfig(EXTI_LINE0 , EXTI_RISING_EDGE);
  
  /* Create control message buffer */
  xControlMessageBuffer = xMessageBufferCreateStatic( mbaCONTROL_MESSAGE_BUFFER_SIZE,ucStorageBuffer_ctr ,&xStreamBufferStruct_ctrl);  
  /* Create data message buffer */
  for( x = 0; x < mbaNUMBER_OF_CORE_2_TASKS; x++ )
  {
    xDataMessageBuffers[ x ] = xMessageBufferCreateStatic( mbaTASK_MESSAGE_BUFFER_SIZE, &ucStorageBuffer[x][0], &xStreamBufferStruct[x]);
    configASSERT( xDataMessageBuffers[ x ] );
  }

  /* Cortex-M7 will release Cortex-M4  by means of HSEM notification */
  /*HW semaphore Clock enable*/
  __HAL_RCC_HSEM_CLK_ENABLE();
  /*Take HSEM */
  HAL_HSEM_FastTake(HSEM_ID_0);
  /*Release HSEM in order to wakeup the CPU2(CM4) from stop mode*/
  HAL_HSEM_Release(HSEM_ID_0,0);
  
  /* Start the check task */
  xTaskCreate( prvCheckTask, "Check", configMINIMAL_STACK_SIZE, NULL, mainCHECK_TASK_PRIORITY, NULL );
  
  /* Start the Core 1 task */
  xTaskCreate( prvCore1Task, "AMPCore1", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL );		

  /* Start scheduler */
  vTaskStartScheduler();

  /* We should never get here as control is now taken by the scheduler */
  for (;;);
}

/* This task will periodically send data to tasks running on Core 2
   via message buffers. */
static void prvCore1Task( void *pvParameters )
{
  BaseType_t x;
  uint32_t ulNextValue = 0;
  const TickType_t xDelay = pdMS_TO_TICKS( 250 );
  char cString[ 15 ];
  
  /* Remove warning about unused parameters. */
  ( void ) pvParameters;
  
  for( ;; )
  {
    /* Create the next string to send.  The value is incremented on each
    loop iteration, and the length of the string changes as the number of
    digits in the value increases. */
    sprintf( cString, "%lu", ( unsigned long ) ulNextValue );
    
    /* Send the value from this Core to the tasks on the Core 2 via the message 
    buffers.  This will result in sbSEND_COMPLETED()
    being executed, which in turn will write the handle of the message
    buffer written to into xControlMessageBuffer then generate an interrupt
    in core 2. */
    for( x = 0; x < mbaNUMBER_OF_CORE_2_TASKS; x++ )
    {
      xMessageBufferSend( xDataMessageBuffers[ x ], 
                         ( void * ) cString,
                         strlen( cString ),
                         mbaDONT_BLOCK );
      
      /* Delay before repeating */
      vTaskDelay( xDelay );
    }

    ulNextValue++;
  }
}

/*-----------------------------------------------------------*/

/* Reimplementation of sbSEND_COMPLETED(), defined as follows in FreeRTOSConfig.h:
   #define sbSEND_COMPLETED( pxStreamBuffer ) vGenerateCore2Interrupt( pxStreamBuffer )

  Called from within xMessageBufferSend().  As this function also calls
  xMessageBufferSend() itself it is necessary to guard against a recursive
  call.  If the message buffer just updated is the message buffer written to
  by this function, then this is a recursive call, and the function can just
  exit without taking further action.
*/
void vGenerateCore2Interrupt( void * xUpdatedMessageBuffer )
{
  MessageBufferHandle_t xUpdatedBuffer = ( MessageBufferHandle_t ) xUpdatedMessageBuffer;
  
  if( xUpdatedBuffer != xControlMessageBuffer )
  {
    /* Use xControlMessageBuffer to pass the handle of the message buffer
    written to by core 1 to the interrupt handler about to be generated in
    core 2. */
    xMessageBufferSend( xControlMessageBuffer, &xUpdatedBuffer, sizeof( xUpdatedBuffer ), mbaDONT_BLOCK );
    
    /* This is where the interrupt would be generated. */
    HAL_EXTI_D1_EventInputConfig(EXTI_LINE0 , EXTI_MODE_IT,  DISABLE);
    HAL_EXTI_D2_EventInputConfig(EXTI_LINE0 , EXTI_MODE_IT,  ENABLE);
    HAL_EXTI_GenerateSWInterrupt(EXTI_LINE0);
  }
}

/* 
  Check if the application still running
*/
static BaseType_t xAreMessageBufferAMPTasksStillRunning( void )
{
  static uint32_t ulLastCycleCounters[ mbaNUMBER_OF_CORE_2_TASKS ] = { 0 };
  BaseType_t xDemoStatus = pdPASS;
  BaseType_t x;
  
  /* Called by the check task to determine the health status of the tasks
  implemented in this demo. */
  for( x = 0; x < mbaNUMBER_OF_CORE_2_TASKS; x++ )
  {
    if( ulLastCycleCounters[ x ] == ulCycleCounters[ x ] )
    {
      xDemoStatus = pdFAIL;
    }
    else
    {
      ulLastCycleCounters[ x ] = ulCycleCounters[ x ];
    }
  }
  
  return xDemoStatus;
}

/* Check task function 
   */
static void prvCheckTask( void *pvParameters )
{
  TickType_t xNextWakeTime;
  const TickType_t xCycleFrequency = pdMS_TO_TICKS( 2000UL );
  
  /* Just to remove compiler warning. */
  ( void ) pvParameters;
  
  /* Initialise xNextWakeTime - this only needs to be done once. */
  xNextWakeTime = xTaskGetTickCount();
  
  for( ;; )
  {
    /* Place this task in the blocked state until it is time to run again. */
    vTaskDelayUntil( &xNextWakeTime, xCycleFrequency );
    
    if( xAreMessageBufferAMPTasksStillRunning() != pdPASS )
    {
      /* Application fail */
      BSP_LED_Off(LED1);
      BSP_LED_On(LED2);
    }
    else
    {
      /* Application still running */
      BSP_LED_Off(LED2);
      BSP_LED_Toggle(LED1);
    }
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

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 400000000 (Cortex-M7 CPU Clock)
  *            HCLK(Hz)                       = 200000000 (Cortex-M4 CPU, Bus matrix Clocks)
  *            AHB Prescaler                  = 2
  *            D1 APB3 Prescaler              = 2 (APB3 Clock  100MHz)
  *            D2 APB1 Prescaler              = 2 (APB1 Clock  100MHz)
  *            D2 APB2 Prescaler              = 2 (APB2 Clock  100MHz)
  *            D3 APB4 Prescaler              = 2 (APB4 Clock  100MHz)
  *            HSE Frequency(Hz)              = 25000000
  *            PLL_M                          = 5
  *            PLL_N                          = 160
  *            PLL_P                          = 2
  *            PLL_Q                          = 4
  *            PLL_R                          = 2
  *            VDD(V)                         = 3.3
  *            Flash Latency(WS)              = 4
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  HAL_StatusTypeDef ret = HAL_OK;

  /* The voltage scaling allows optimizing the power consumption when the device is 
     clocked below the maximum system frequency, to update the voltage scaling value 
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}
  
  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
  RCC_OscInitStruct.CSIState = RCC_CSI_OFF;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;

  RCC_OscInitStruct.PLL.PLLM = 5;
  RCC_OscInitStruct.PLL.PLLN = 160;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLQ = 4;

  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
  if(ret != HAL_OK)
  {
    Error_Handler();
  }
  
/* Select PLL as system clock source and configure  bus clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_D1PCLK1 | RCC_CLOCKTYPE_PCLK1 | \
                                 RCC_CLOCKTYPE_PCLK2  | RCC_CLOCKTYPE_D3PCLK1);

  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;  
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2; 
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2; 
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2; 
  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4);
  if(ret != HAL_OK)
  {
    Error_Handler();
  }
  
  /*
  Note : The activation of the I/O Compensation Cell is recommended with communication  interfaces
          (GPIO, SPI, FMC, QSPI ...)  when  operating at  high frequencies(please refer to product datasheet)       
          The I/O Compensation Cell activation  procedure requires :
        - The activation of the CSI clock
        - The activation of the SYSCFG clock
        - Enabling the I/O Compensation Cell : setting bit[0] of register SYSCFG_CCCSR
  
          To do this please uncomment the following code 
  */
 
  /*  
  __HAL_RCC_CSI_ENABLE() ;
  
  __HAL_RCC_SYSCFG_CLK_ENABLE() ;
  
  HAL_EnableCompensationCell();
  */  
}

/**
* @brief  CPU L1-Cache enable.
* @param  None
* @retval None
*/
static void CPU_CACHE_Enable(void)
{
  /* Enable I-Cache */
  SCB_EnableICache();

  /* Enable D-Cache */
  SCB_EnableDCache();
}

/**
  * @brief  Configure the MPU attributes as Not Cacheable for Internal D3SRAM.
  * @note   The Base Address is 0x38000000 (D3_SRAM_BASE).
  *         The Configured Region Size is 64KB because same as D3SRAM size.
  * @param  None
  * @retval None
  */
static void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct;
  
  /* Disable the MPU */
  HAL_MPU_Disable();

  /* Configure the MPU as Strongly ordered for not defined regions */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress = 0x00;
  MPU_InitStruct.Size = MPU_REGION_SIZE_4GB;
  MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x87;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Configure the MPU attributes as WT for SRAM */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress = D3_SRAM_BASE;
  MPU_InitStruct.Size = MPU_REGION_SIZE_64KB;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER1;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Enable the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
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

