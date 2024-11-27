/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    FreeRTOS/FreeRTOS_Semaphore/Src/main.c
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
#define semtstSTACK_SIZE      configMINIMAL_STACK_SIZE
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
osThreadId_t SEM_Thread1Handle;
osThreadId_t SEM_Thread2Handle;
static osThreadAttr_t idleattr = {
                        .priority = osPriorityIdle,
                        .stack_size = configMINIMAL_STACK_SIZE,
                      };
static osThreadAttr_t lowattr = {
                        .priority = osPriorityLow,
                        .stack_size = configMINIMAL_STACK_SIZE,
                      };
osSemaphoreId_t osSemaphoreHandle;				 
/* USER CODE BEGIN PV */
__IO uint32_t OsStatus = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
static void MPU_Config(void);
static void SystemClock_Config(void);
static void SemaphoreThread1(void *argument);
static void SemaphoreThread2(void *argument);
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
  BSP_LED_Init(LED2);
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */
  osKernelInitialize();
  /* USER CODE BEGIN RTOS_MUTEX */
  
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* definition and creation of osSemaphore */
  osSemaphoreHandle = osSemaphoreNew(1U, 1U, NULL);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  
  /* USER CODE END RTOS_TIMERS */

  /* Create the thread(s) */
  
  /* definition and creation of SEM_Thread1 */
  lowattr.name = "SEM_Thread1";
  SEM_Thread1Handle = osThreadNew(SemaphoreThread1, osSemaphoreHandle, (const osThreadAttr_t *)&lowattr);
  /* definition and creation of SEM_Thread2 */
  idleattr.name = "SEM_Thread2";
  SEM_Thread2Handle = osThreadNew(SemaphoreThread2, osSemaphoreHandle, (const osThreadAttr_t *)&idleattr);

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
  *            System Clock source            = PLL (HSE BYPASS)
  *            SYSCLK(Hz)                     = 520000000 (CPU Clock)
  *            HCLK(Hz)                       = 260000000 (AXI and AHBs Clock)
  *            AHB Prescaler                  = 2
  *            D1 APB3 Prescaler              = 2 (APB3 Clock  130MHz)
  *            D2 APB1 Prescaler              = 2 (APB1 Clock  130MHz)
  *            D2 APB2 Prescaler              = 2 (APB2 Clock  130MHz)
  *            D3 APB4 Prescaler              = 2 (APB4 Clock  130MHz)
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 4
  *            PLL_N                          = 260
  *            PLL_P                          = 1
  *            PLL_Q                          = 4
  *            PLL_R                          = 2
  *            VDD(V)                         = 3.3
  *            Flash Latency(WS)              = 3
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  HAL_StatusTypeDef ret = HAL_OK;

  /* The voltage scaling allows optimizing the power consumption when the device is
     clocked below the maximum system frequency, to update the voltage scaling value
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
  RCC_OscInitStruct.CSIState = RCC_CSI_OFF;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;

  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 260;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  RCC_OscInitStruct.PLL.PLLP = 1;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLQ = 4;

  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_1;
  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
  if(ret != HAL_OK)
  {
    /* Initialization Error */
    while(1);
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
  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3);
  if(ret != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_SemaphoreThread1 */
/**
  * @brief  Function implementing the SEM_Thread1 thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_SemaphoreThread1 */
static void SemaphoreThread1(void *argument)
{

  /* USER CODE BEGIN 5 */
  uint32_t count = 0;
  osSemaphoreId_t semaphore = (osSemaphoreId_t) argument;
  /* Infinite loop */
  for (;;)
  {

    if (semaphore != NULL)
    {
    OsStatus = osSemaphoreAcquire(semaphore, osWaitForever);
      /* Try to obtain the semaphore */
      if (OsStatus == osOK)
      {
        count = osKernelGetTickCount() + 5000;

        /* Toggle LED1 every 200 ms for 5 seconds */
        while (count > osKernelGetTickCount())
        {
          /* Toggle LED1 */
          BSP_LED_Toggle(LED1);

          /* Delay 200 ms */
          osDelay(200);
        }

        /* Turn off LED1*/
        BSP_LED_Off(LED1);
        /* Release the semaphore */
        OsStatus = osSemaphoreRelease(semaphore);

        /* Suspend ourseleves to execute thread 2 (lower priority)  */
        OsStatus = osThreadSuspend(SEM_Thread1Handle);
      }
    }
  }
  /* USER CODE END 5 */ 
}

/* USER CODE BEGIN Header_SemaphoreThread2 */
/**
* @brief Function implementing the SEM_Thread2 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_SemaphoreThread2 */
static void SemaphoreThread2(void *argument)
{
  /* USER CODE BEGIN SemaphoreThread2 */
  uint32_t count = 0;
  osSemaphoreId_t semaphore = (osSemaphoreId_t) argument;
  /* Infinite loop */
  for (;;)
  {
    if (semaphore != NULL)
    {
    OsStatus = osSemaphoreAcquire(semaphore, osWaitForever);
      {
        /* Resume Thread 1 (higher priority)*/
        OsStatus = osThreadResume(SEM_Thread1Handle);

        count = osKernelGetTickCount() + 5000;

        /* Toggle LED2 every 200 ms for 5 seconds*/
        while (count > osKernelGetTickCount())
        {
          BSP_LED_Toggle(LED2);

          osDelay(200);
        }

        /* Turn off LED2 */
        BSP_LED_Off(LED2);

        /* Release the semaphore to unblock Thread 1 (higher priority)  */
        OsStatus = osSemaphoreRelease(semaphore);
      }
    }
  }
  /* USER CODE END SemaphoreThread2 */
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
void assert_failed(uint8_t* file, uint32_t line)
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

