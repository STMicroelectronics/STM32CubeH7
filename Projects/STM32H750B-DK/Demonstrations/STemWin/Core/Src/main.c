/**
  ******************************************************************************
  * @file    main.c 
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup CORE
  * @{
  */

/** @defgroup MAIN
* @brief main file
* @{
*/ 

/** @defgroup MAIN_Private_TypesDefinitions
* @{
*/ 
typedef struct pwr_db
{
  __IO uint32_t t[0x30/4];
  __IO uint32_t PDR1;
}PWDDBG_TypeDef;

#define PWR_CFG_SMPS  0xCAFECAFE
#define PWR_CFG_LDO   0x5ACAFE5A

/**
* @}
*/ 

/** @defgroup MAIN_Private_Defines
* @{
*/ 
#define HSEM_ID_0                       (0U) /* HW semaphore 0*/

#define TS_TaskPRIORITY                 osPriorityHigh
#define TS_TaskSTACK_SIZE               (8 * configMINIMAL_STACK_SIZE)

#define GUI_TaskPRIORITY                osPriorityBelowNormal
#define GUI_TaskSTACK_SIZE              (32 * configMINIMAL_STACK_SIZE)
/**
* @}
*/ 


/** @defgroup MAIN_Private_Macros
* @{
*/ 
#define PWDDBG                          ((PWDDBG_TypeDef*)PWR)
#define DEVICE_IS_CUT_2_1()             (HAL_GetREVID() & 0x21ff) ? 1 : 0

/**
* @}
*/ 


/** @defgroup MAIN_Private_Variables
* @{
*/
static osMessageQId     TouchScreenEvent = 0;
static uint8_t          BSP_Initialized = 0;
uint32_t                AutoDemoId = 0;
uint32_t                AutoDemoTimeOutMs = AUTO_DEMO_TIMEOUT_0;
/**
* @}
*/ 


/** @defgroup MAIN_Private_FunctionPrototypes
* @{
*/ 

static void SystemClock_Config(void);
static void GUIThread(void const * argument);
static void TouchScreenTask(void const *argument);
static void CPU_CACHE_Enable(void);
       void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName );
/* List of all the module of the Demo */
extern K_ModuleItem_Typedef  video_player_board;
extern K_ModuleItem_Typedef  rocket_game_board;
extern K_ModuleItem_Typedef  audio_player_board;
extern K_ModuleItem_Typedef  analog_clock_board;
extern K_ModuleItem_Typedef  graphic_effects_board;


#if configAPPLICATION_ALLOCATED_HEAP == 1
#if defined ( __ICCARM__ )
#pragma location="heap_mem"
#else
__attribute__((section("heap_mem")))
#endif
uint8_t ucHeap[ configTOTAL_HEAP_SIZE ];
#endif /* configAPPLICATION_ALLOCATED_HEAP */

extern osSemaphoreId LcdUpdateSemaphoreId ;

/**
 * @}
 */ 

/** @defgroup MAIN_Private_Functions
 * @{
 */ 
/**
  * @brief  TouchScreen task
  * @param  argument: pointer that is passed to the thread function as start argument.
  * @retval None
  */
static void TouchScreenTask(void const *argument)
{
  osEvent event;

  /* Create Storage Message Queue */
  osMessageQDef(TSQueue, 16, uint8_t);
  TouchScreenEvent = osMessageCreate (osMessageQ(TSQueue), NULL);

  while(TouchScreenEvent)
  {
    event = osMessageGet( TouchScreenEvent, osWaitForever );

    /* Capture input event and updade cursor */
    if ( (BSP_Initialized == 1) && ( event.status == osEventMessage ) )
    {
      k_TouchUpdate();
    }
  }
}

void BSP_TS_Callback(uint32_t Instance)
{
  if(TouchScreenEvent)
  {
    osMessagePut ( TouchScreenEvent, 0, 0);
  }
}

/**
 * @brief  Main program
 * @param  None
 * @retval int
 */
int main(void)
{
  static __IO uint32_t BckRegValue = 0;


  /* Enable RCC PWR */
  __HAL_RCC_RTC_ENABLE();

  /* Enable RTC back-up registers access */
  __HAL_RCC_RTC_CLK_ENABLE();
  HAL_PWR_EnableBkUpAccess();

  /* Read SW Configuration */
  BckRegValue       = READ_REG(BKP_REG_SW_CFG);
  AutoDemoId        = (((BckRegValue & 0x0000FF00) >>  8) & 0xFF);
  AutoDemoTimeOutMs = (((BckRegValue & 0xFFFF0000) >> 16) & 0xFFFF);
  if(AutoDemoTimeOutMs == 0)
  {
    AutoDemoTimeOutMs = AUTO_DEMO_TIMEOUT_0;
  }

  HAL_PWR_DisableBkUpAccess();

  CPU_CACHE_Enable();
  
  /* STM32H7xx HAL library initialization:
  - Configure the Flash ART accelerator on ITCM interface
  - Configure the Systick to generate an interrupt each 1 msec
  - Set NVIC Group Priority to 4
  - Global MSP (MCU Support Package) initialization
  */
  HAL_Init();

  /* WA : Allow Debugger to connect under reset */
  HAL_Delay(1000);

  /* Configure the system clock */
  SystemClock_Config();

  /* cxialize RTC */
  k_CalendarBkupInit();

  HAL_PWR_EnableBkUpAccess();
  if(AutoDemoId)
  {
    /* Set SW Reset value to 2 : Pressing SW Reset button will do SW reset with this value */
    WRITE_REG(BKP_REG_SW_CFG, ((BckRegValue & 0xFFFFFF00) | 0x2));
  }
  else
  {
    WRITE_REG(BKP_REG_SW_CFG, ((BckRegValue & 0xFFFFFF00) | 0x1));
  }
  HAL_PWR_DisableBkUpAccess();

  /* Configure the board */
  BSP_Initialized = k_BspInit(); 

  if(BSP_Initialized)
  { 
    /* Create TS Thread */
    osThreadDef(TS_Task, TouchScreenTask, TS_TaskPRIORITY, 0, TS_TaskSTACK_SIZE);
    osThreadCreate(osThread(TS_Task), NULL);
    
    /* Create GUI task */
    osThreadDef(GUI_Thread, GUIThread, GUI_TaskPRIORITY, 0, GUI_TaskSTACK_SIZE);
    osThreadCreate (osThread(GUI_Thread), NULL);
    
    /* Initialise Modules function */
    k_ModuleInit();
    
    /* Link modules */ 
    k_ModuleAdd(&audio_player_board);       
    k_ModuleAdd(&video_player_board); 
    k_ModuleAdd(&rocket_game_board);
    k_ModuleAdd(&analog_clock_board);
    k_ModuleAdd(&graphic_effects_board);  
    
    /* Initialize Storage Units */
    Storage_Init();
    
    /* Start scheduler */
    osKernelStart ();
  }

  /* We should never get here as control is now taken by the scheduler */
  for( ;; )
  {
    /* Toggle LED GREEN */
    BSP_LED_Toggle(LED_GREEN);
    GUI_Delay(1000);
  }
}

/**
  * @brief  Start task
  * @param  argument: pointer that is passed to the thread function as start argument.
//  * @retval None
  */
static void GUIThread(void const * argument)
{  
  /* Initialize GUI */
  GUI_Init();

  /* Enable Multibuffereing and set Layer0 as the default display layer */ 
  WM_MULTIBUF_Enable(1);
  GUI_SetLayerVisEx (1, 0);
  GUI_SelectLayer(0);

  GUI_SetBkColor(GUI_TRANSPARENT);
  GUI_Clear();
  
  /* Show the main menu */
  k_InitMenu();  
  
  /* Display immediately the Menu */
  GUI_Exec();
  
  /* Gui background Task */
  while(1) {
    GUI_Delay(10);
  }
}

/* Private functions ---------------------------------------------------------*/


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
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;
  HAL_StatusTypeDef ret = HAL_OK;

  /*!< Supply configuration update enable */
#if defined(USE_PWR_LDO_SUPPLY)
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);
#else
  HAL_PWREx_ConfigSupply(PWR_DIRECT_SMPS_SUPPLY);
#endif /* USE_PWR_LDO_SUPPLY */

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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;
  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4);
  if(ret != HAL_OK)
  {
    Error_Handler();
  }

  /* LCD clock configuration */
  /* PLL3_VCO Input = HSE_VALUE/PLL3M = 1 Mhz */
  /* PLL3_VCO Output = PLL3_VCO Input * PLL3N = 288 Mhz */
  /* PLLLCDCLK = PLL3_VCO Output/PLL3R = 288/32 = 9 Mhz */
  /* PLLUSBCLK = PLL3_VCO Output/PLL3Q = 288/6 = 48 Mhz */
  /* LTDC clock frequency = PLLLCDCLK = 9 Mhz */
  /* USB clock frequency = 48 Mhz */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
  PeriphClkInitStruct.PLL3.PLL3M = 25;
  PeriphClkInitStruct.PLL3.PLL3N = 288;
  PeriphClkInitStruct.PLL3.PLL3P = 2;
  PeriphClkInitStruct.PLL3.PLL3Q = 6;
  PeriphClkInitStruct.PLL3.PLL3R = 32;
  PeriphClkInitStruct.PLL3.PLL3VCOSEL = RCC_PLL3VCOMEDIUM;
  PeriphClkInitStruct.PLL3.PLL3RGE = RCC_PLL3VCIRANGE_0;

  if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /* PLL3 for USB Clock */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInitStruct.UsbClockSelection = RCC_USBCLKSOURCE_PLL3;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);

  /* Configures the External Low Speed oscillator (LSE) drive capability */
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_HIGH);

  /*##-1- Configure LSE as RTC clock source ##################################*/
  RCC_OscInitStruct.OscillatorType =  RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_OFF;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /*##-2- Enable RTC peripheral Clocks #######################################*/
  /* Enable RTC Clock */
  __HAL_RCC_RTC_ENABLE();

  /*
  Note : The activation of the I/O Compensation Cell is recommended with communication  interfaces
          (GPIO, SPI, FMC, QSPI ...)  when  operating at  high frequencies(please refer to product datasheet)
          The I/O Compensation Cell activation  procedure requires :
        - The activation of the CSI clock
        - The activation of the SYSCFG clock
        - Enabling the I/O Compensation Cell : setting bit[0] of register SYSCFG_CCCSR
  */

  __HAL_RCC_CSI_ENABLE() ;

  __HAL_RCC_SYSCFG_CLK_ENABLE() ;

  HAL_EnableCompensationCell();
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
	  BSP_LED_On(LED_RED);

  configASSERT (0);
}

void BSP_ErrorNotify(void)
{
  if(BSP_Initialized)
	BSP_LED_On(LED_RED);
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


#ifdef  USE_FULL_ASSERT
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
  while (1)
  {}
}
#endif


/**
* @}
*/ 

/**
* @}
*/ 

/**
* @}
*/ 

