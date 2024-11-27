/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    FreeRTOS/FreeRTOS_Mutexes/Src/main.c
  * @author  MCD Application Team
  * @brief   Main program body
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
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define mutexSHORT_DELAY     ((uint32_t) 20)
#define mutexNO_DELAY        ((uint32_t) 0)
#define mutexTWO_TICK_DELAY  ((uint32_t) 2)
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
osThreadId_t MutHighHandle;
osThreadId_t MutMediumHandle;
osThreadId_t MutLowHandle;
static osThreadAttr_t highattr = {
                        .priority = osPriorityBelowNormal,
                        .stack_size = configMINIMAL_STACK_SIZE,
                      };
static osThreadAttr_t mediumattr = {
                        .priority = osPriorityLow,
                        .stack_size = configMINIMAL_STACK_SIZE,
                      };
static osThreadAttr_t lowattr = {
                        .priority = osPriorityIdle,
                        .stack_size = configMINIMAL_STACK_SIZE,
                      };
osMutexId_t osMutexHandle;					 
/* USER CODE BEGIN PV */

/* Variables used to detect and latch errors */
__IO uint32_t HighPriorityThreadCycles = 0, MediumPriorityThreadCycles = 0, LowPriorityThreadCycles = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
static void MPU_Config(void);
static void SystemClock_Config(void);
static void MutexHighPriorityThread(void *argument);
static void MutexMediumPriorityThread(void *argument);
static void MutexLowPriorityThread(void *argument);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  /* STM32H7xx HAL library initialization:
       - Systick timer is configured by default as source of time base, but user 
         can eventually implement his proper time base source (a general purpose 
         timer for example or other time source), keeping in mind that Time base 
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
         handled in milliseconds basis.
       - Set NVIC Group Priority to 4
       - Low Level Initialization
     */
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Configure the MPU attributes */
  MPU_Config();

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  /* Initialize LEDs */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED3);
  BSP_LED_Init(LED4);
  BSP_LED_Init(LED2);
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */
  osKernelInitialize();
  /* Create the mutex(es) */
  osMutexHandle = osMutexNew(NULL);
  /* USER CODE BEGIN RTOS_MUTEX */
  
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  
  /* USER CODE END RTOS_TIMERS */

  /* Create the thread(s) */
  /* definition and creation of MutHigh */
  highattr.name = "MutHigh";
  MutHighHandle = osThreadNew(MutexHighPriorityThread, NULL, (const osThreadAttr_t *)&highattr);
  /* definition and creation of MutMedium */
  mediumattr.name = "MutMedium";
  MutMediumHandle = osThreadNew(MutexMediumPriorityThread, NULL, (const osThreadAttr_t *)&mediumattr);
  /* definition and creation of MutLow */
  lowattr.name = "MutLow";
  MutLowHandle = osThreadNew(MutexLowPriorityThread, NULL, (const osThreadAttr_t *)&lowattr);

  

  /* USER CODE BEGIN RTOS_THREADS */
  
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_QUEUES */
  
  /* USER CODE END RTOS_QUEUES */
 

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 280000000 (CPU Clock)
  *            HCLK(Hz)                       = 280000000 (Bus matrix and AHBs Clock)
  *            AHB Prescaler                  = 1
  *            CD APB3 Prescaler              = 2 (APB3 Clock  140MHz)
  *            CD APB1 Prescaler              = 2 (APB1 Clock  140MHz)
  *            CD APB2 Prescaler              = 2 (APB2 Clock  140MHz)
  *            SRD APB4 Prescaler             = 2 (APB4 Clock  140MHz)
  *            HSE Frequency(Hz)              = 24000000
  *            PLL_M                          = 12
  *            PLL_N                          = 280
  *            PLL_P                          = 2
  *            PLL_Q                          = 2
  *            PLL_R                          = 2
  *            VDD(V)                         = 3.3
  *            Flash Latency(WS)              = 6
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
  regarding system frequency refer to product datasheet.
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
  RCC_OscInitStruct.CSIState = RCC_CSI_OFF;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;

  RCC_OscInitStruct.PLL.PLLM = 12;
  RCC_OscInitStruct.PLL.PLLN = 280;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;

  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_1;
  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
  if(ret != HAL_OK)
  {
  while(1) {};
  }

  /* Select PLL as system clock source and configure  bus clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_D1PCLK1 | RCC_CLOCKTYPE_PCLK1 | \
    RCC_CLOCKTYPE_PCLK2  | RCC_CLOCKTYPE_D3PCLK1);

  RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider  = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider  = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;
  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_6);
  if(ret != HAL_OK)
  {
  while(1) {};
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_MutexHighPriorityThreadr */
/**
  * @brief  Function implementing the MutHigh thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_MutexHighPriorityThreadr */
static void MutexHighPriorityThread(void *argument)
{

  /* USER CODE BEGIN 5 */
  /* Just to remove compiler warning */
  (void) argument;
  /* Infinite loop */
  for (;;)
  {
    /* The first time through, the mutex will be immediately available. On
    subsequent times through, the mutex will be held by the low priority thread
    at this point and this will cause the low priority thread to inherit
    the priority of this thread. In this case the block time must be
    long enough to ensure the low priority thread will execute again before the
    block time expires.  If the block time does expire then the error
    flag will be set here */
    if (osMutexAcquire(osMutexHandle, mutexTWO_TICK_DELAY) != osOK)
    {
      /* Toggle LED2 to indicate error */
      BSP_LED_Toggle(LED2);
    }

    /* Ensure the other thread attempting to access the mutex
    are able to execute to ensure they either block (where a block
    time is specified) or return an error (where no block time is
    specified) as the mutex is held by this task */
    osDelay(mutexSHORT_DELAY);

    /* We should now be able to release the mutex .
    When the mutex is available again the medium priority thread
    should be unblocked but not run because it has a lower priority
    than this thread.  The low priority thread should also not run
    at this point as it too has a lower priority than this thread */
    if (osMutexRelease(osMutexHandle) != osOK)
    {
      /* Toggle LED2 to indicate error */
      BSP_LED_Toggle(LED2);
    }

    /* Keep count of the number of cycles this thread has performed */
    HighPriorityThreadCycles++;
    BSP_LED_Toggle(LED1);

    /* Suspend ourselves to the medium priority thread can execute */
    osThreadSuspend(MutHighHandle);
  }
  /* USER CODE END 5 */ 
}

/* USER CODE BEGIN Header_MutexMediumPriorityThread */
/**
* @brief Function implementing the MutMedium thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_MutexMediumPriorityThread */
static void MutexMediumPriorityThread(void *argument)
{
  /* USER CODE BEGIN MutexMediumPriorityThread */
  /* Just to remove compiler warning */
  (void) argument;

  /* Infinite loop */
  for (;;)
  {
    /* This thread will run while the high-priority thread is blocked, and the
    high-priority thread will block only once it has the mutex - therefore
    this call should block until the high-priority thread has given up the
    mutex, and not actually execute past this call until the high-priority
    thread is suspended */
    if (osMutexAcquire(osMutexHandle, osWaitForever) == osOK)
    {
      if (osThreadGetState(MutHighHandle) != osThreadBlocked)
      {
        /* Did not expect to execute until the high priority thread was
        suspended.
        Toggle LED2 to indicate error */
        BSP_LED_Toggle(LED2);
      }
      else
      {
        /* Give the mutex back before suspending ourselves to allow
        the low priority thread to obtain the mutex */
        if (osMutexRelease(osMutexHandle) != osOK)
        {
          /* Toggle LED2 to indicate error */
          BSP_LED_Toggle(LED2);
        }
        osThreadSuspend(MutMediumHandle);
      }
    }
    else
    {
      /* We should not leave the osMutexWait() function
      until the mutex was obtained.
      Toggle LED2 to indicate error */
      BSP_LED_Toggle(LED2);
    }

    /* The High and Medium priority threads should be in lock step */
    if (HighPriorityThreadCycles != (MediumPriorityThreadCycles + 1))
    {
      /* Toggle LED2 to indicate error */
      BSP_LED_Toggle(LED2);
    }

    /* Keep count of the number of cycles this task has performed so a
    stall can be detected */
    MediumPriorityThreadCycles++;
    BSP_LED_Toggle(LED3);
  }
  /* USER CODE END MutexMediumPriorityThread */
}

/* USER CODE BEGIN Header_MutexLowPriorityThread */
/**
* @brief Function implementing the MutLow thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_MutexLowPriorityThread */
static void MutexLowPriorityThread(void *argument)
{
  /* USER CODE BEGIN MutexLowPriorityThread */
  /* Just to remove compiler warning */
  (void) argument;

  /* Infinite loop */
  for (;;)
  {
    /* Keep attempting to obtain the mutex.  We should only obtain it when
    the medium-priority thread has suspended itself, which in turn should only
    happen when the high-priority thread is also suspended */
    if (osMutexAcquire(osMutexHandle, mutexNO_DELAY) == osOK)
    {
      /* Is the haigh and medium-priority threads suspended? */
      if ((osThreadGetState(MutHighHandle) != osThreadBlocked) || (osThreadGetState(MutMediumHandle) != osThreadBlocked))
      {
        /* Toggle LED2 to indicate error */
        BSP_LED_Toggle(LED2);
      }
      else
      {
        /* Keep count of the number of cycles this task has performed
        so a stall can be detected */
        LowPriorityThreadCycles++;
        BSP_LED_Toggle(LED4);

        /* We can resume the other tasks here even though they have a
        higher priority than the this thread. When they execute they
        will attempt to obtain the mutex but fail because the low-priority 
        thread is still the mutex holder.  this thread will then inherit 
        the higher priority.  The medium-priority thread will block indefinitely
        when it attempts to obtain the mutex, the high-priority thread will only
        block for a fixed period and an error will be latched if the
        high-priority thread has not returned the mutex by the time this
        fixed period has expired */
        osThreadResume(MutMediumHandle);
        osThreadResume(MutHighHandle);

        /* Release the mutex, disinheriting the higher priority again */
        if (osMutexRelease(osMutexHandle) != osOK)
        {
          /* Toggle LED2 to indicate error */
          BSP_LED_Toggle(LED2);
        }
      }
    }

#if configUSE_PREEMPTION == 0
    {
      taskYIELD();
    }
#endif
  }
  /* USER CODE END MutexLowPriorityThread */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
 
  /* USER CODE END Error_Handler_Debug */
}


/**
  * @brief  Configure the MPU attributes
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

  /* Enable the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

