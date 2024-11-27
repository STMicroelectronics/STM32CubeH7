/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    FatFs/FatFs_MultiAccess_RTOS/Src/main.c
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
#include "app_fatfs.h"
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
#define blckqSTACK_SIZE   configMINIMAL_STACK_SIZE
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
osThreadId_t StartThreadHandle;
osThreadId_t ConcurrentThreadHandle;
static osThreadAttr_t attr = {
                        .priority = osPriorityNormal,
                        .stack_size = 8 * configMINIMAL_STACK_SIZE,
                      };
					  
osMessageQId AppliEvent;
osMessageQId DiskEvent;	
uint32_t   DiskQueueMsg = 0;		
int32_t ProcessStatus = 0;
/* USER CODE BEGIN PV */
__IO uint32_t OsStatus = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
static void MPU_Config(void);
static void SystemClock_Config(void);
static void StartThread(void *argument);
static void ConcurrentThread(void *argument);
HAL_StatusTypeDef MX_SDMMC1_SD_Init(SD_HandleTypeDef *hsd1);
static void SD_ClockConfig(void);
void Success_Handler(void);
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
  BSP_LED_Init(LED2);
  BSP_LED_Init(LED3);
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  /* USER CODE BEGIN 2 */
  SD_ClockConfig();
  BSP_SD_Init(0);
  /* USER CODE END 2 */ 
  if(MX_FATFS_Init() == APP_OK)
  {     
    BSP_LED_Off(LED_ERROR);
    BSP_LED_Toggle(LED_OK);
    HAL_Delay(200);
  }
  else
  {
    Error_Handler();
  }
  osKernelInitialize();
  /* USER CODE BEGIN RTOS_MUTEX */
  
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  
  /* USER CODE END RTOS_TIMERS */

  /* Create the thread(s) */
  /* definition and creation of USER_Thread */
  attr.name = "USER_Thread";
  StartThreadHandle = osThreadNew(StartThread, NULL, (const osThreadAttr_t *)&attr);
  /* definition and creation of USER_ConcurrentThread */
  attr.name = "USER_ConcurrentThread";
  ConcurrentThreadHandle = osThreadNew(ConcurrentThread, NULL, (const osThreadAttr_t *)&attr);
  /* USER CODE BEGIN RTOS_THREADS */
  
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* Create Disk Queue */
  DiskEvent = osMessageQueueNew (1U, sizeof(uint16_t), NULL);  
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
/**
  * @brief  Configure SDMMC clock
  * @param  Mode SD_IT_MODE or SD_POLLING_MODE
  * @retval None
  */
static void SD_ClockConfig(void)
{

    RCC_PeriphCLKInitTypeDef RCC_PeriphCLKInitStruct;
    /*  SDMMC Clk: (((24/24) * 200) / 2) / SDMMC_NSpeed_CLK_DIV = 25Mhz */
    RCC_PeriphCLKInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SDMMC;
    RCC_PeriphCLKInitStruct.SdmmcClockSelection = RCC_SDMMCCLKSOURCE_PLL2;
    RCC_PeriphCLKInitStruct.PLL2.PLL2RGE = RCC_PLL1VCIRANGE_2;
    RCC_PeriphCLKInitStruct.PLL2.PLL2M = 24;
    RCC_PeriphCLKInitStruct.PLL2.PLL2N = 200;
    RCC_PeriphCLKInitStruct.PLL2.PLL2FRACN = 0;
    RCC_PeriphCLKInitStruct.PLL2.PLL2P = 0;
    RCC_PeriphCLKInitStruct.PLL2.PLL2R = 2;
    RCC_PeriphCLKInitStruct.PLL2.PLL2Q = 0;
    RCC_PeriphCLKInitStruct.PLL2.PLL2VCOSEL = RCC_PLL2VCOMEDIUM;
    HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphCLKInitStruct);

    __HAL_RCC_SDMMC1_FORCE_RESET();
    __HAL_RCC_SDMMC1_RELEASE_RESET();
}
/* USER CODE END 4 */

/* USER CODE BEGIN Header_SemaphoreThread1 */
/**
  * @brief  Function implementing the StartThread thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_SemaphoreThread1 */
static void StartThread(void *argument)
{

  /* USER CODE BEGIN 5 */
  int32_t ffStatus ;
  /* Infinite loop */
  while (1)
  {
    ffStatus = MX_FATFS_Process();
    if( ffStatus == APP_SD_UNPLUGGED)
    {
      BSP_LED_Off(LED_OK);
      BSP_LED_Toggle(LED_ERROR);
      HAL_Delay(200);
    }
    else if (ffStatus == APP_ERROR)
    {
      Error_Handler();
    }
    else if (ffStatus ==APP_OK)
    {
      osThreadSuspend (StartThreadHandle);
    }
  }
  /* USER CODE END 5 */ 
}

/* USER CODE BEGIN ConcurrentThread */
/**
* @brief Function implementing the ConcurrentThread thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END ConcurrentThread */
static void ConcurrentThread(void *argument)
{
  /* USER CODE BEGIN ConcurrentThread */
  osStatus_t status;  
  
  while (1)
  {
    status = osMessageQueueGet(DiskEvent, &DiskQueueMsg, NULL, osWaitForever);
    if(status == osOK)
    {
      switch(DiskQueueMsg)
      {
      case DISK_READY_EVENT:	  
	    FS_File2Operations(); 
        break;
        
      case DISK_REMOVE_EVENT:
        /* Unlink the USB disk I/O driver */
        MX_FATFS_DeInit();   
        Success_Handler();
        break;
        
      default:
        break;
      }
    }
  }
  /* USER CODE END ConcurrentThread */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  BSP_LED_Off(LED_OK);
  BSP_LED_On(LED_ERROR);
  
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

/**
  * @brief  This function is executed in case of success occurrence.
  * @retval None
  */
void Success_Handler(void)
{
  /* USER CODE BEGIN Success_Handler */
  BSP_LED_On(LED_OK);
  BSP_LED_Off(LED_ERROR);
  while (1)
  {
  }
  /* USER CODE END Success_Handler */
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

  /* Configure the MPU attributes as WT for SDRAM */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress = 0xD0000000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_32MB;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER1;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;

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

