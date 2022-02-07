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
uint8_t                 DualCoreEnabled = 0;
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
static void MPU_Config(void);
static void CPU_CACHE_Enable(void);
       void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName );
/* List of all the module of the Demo */
extern K_ModuleItem_Typedef  video_player_board;
extern K_ModuleItem_Typedef  rocket_game_board;
extern K_ModuleItem_Typedef  audio_player_board;
extern K_ModuleItem_Typedef  analog_clock_board;
extern K_ModuleItem_Typedef  graphic_effects_board;
extern K_ModuleItem_Typedef  dual_core_board;

#if configAPPLICATION_ALLOCATED_HEAP == 1
#if defined ( __ICCARM__ )
#pragma location="heap_mem"
#else
__attribute__((section("heap_mem")))
#endif
uint8_t ucHeap[ configTOTAL_HEAP_SIZE ];
#endif /* configAPPLICATION_ALLOCATED_HEAP */

extern osSemaphoreId LcdUpdateSemaphoreId ;
#if defined(USE_PWR_LDO_SUPPLY)
static uint32_t power_config = PWR_CFG_LDO;
#else
static uint32_t power_config = PWR_CFG_SMPS;
#endif /* USE_PWR_LDO_SUPPLY */

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
 
 /**
  * @brief  EXTI line detection callbacks.
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(!BSP_Initialized)
    return;

  switch(GPIO_Pin)
  {
    case BUTTON_WAKEUP_PIN :
    {
      /* Turn LEDs off */
      BSP_LED_Off(LED_RED);
#if defined (DEBUG_HW_JPEG)
      BSP_LED_Off(LED_ORANGE);
      BSP_LED_Off(LED_BLUE);
#endif /* DEBUG_HW_JPEG */
    }
    break;

#if defined (USE_SDCARD)
    case SD_DETECT_PIN :
    {
      k_StorageDetectSDCard();
    }
    break;
#endif /* USE_SDCARD */

    case TS_INT_PIN :
    {
      if(TouchScreenEvent)
      {
        osMessagePut ( TouchScreenEvent, 0, 0);
      }
    }
    break;
    default:
      break;
  }
}

/**
 * @brief  Main program
 * @param  None
 * @retval int
 */
int main(void)
{
  uint32_t timeout = 0xFFFF;
#if defined (USE_STM32H747I_EVAL_OOB)
  static __IO uint32_t BckRegValue = 0;
#endif /* USE_STM32H747I_EVAL_OOB */

  /* Enable RCC PWR */
  __HAL_RCC_RTC_ENABLE();

  /* Enable RTC back-up registers access */
  __HAL_RCC_RTC_CLK_ENABLE();
  HAL_PWR_EnableBkUpAccess();

  /* Read Power Configuration status */
#if defined (USE_STM32H747I_EVAL_OOB)
  power_config = (uint32_t )READ_REG(BKP_REG_PWR_CFG);
#endif /* USE_STM32H747I_EVAL_OOB */

  /* Read calibration values */
  TouchScreen_GetValue();

#if defined (USE_STM32H747I_EVAL_OOB)
  /* Read SW Configuration */
  BckRegValue       = READ_REG(BKP_REG_SW_CFG);
  AutoDemoId        = (((BckRegValue & 0x0000FF00) >>  8) & 0xFF);
  AutoDemoTimeOutMs = (((BckRegValue & 0xFFFF0000) >> 16) & 0xFFFF);
  if(AutoDemoTimeOutMs == 0)
  {
    AutoDemoTimeOutMs = AUTO_DEMO_TIMEOUT_0;
  }
#endif /* USE_STM32H747I_EVAL_OOB */

  HAL_PWR_DisableBkUpAccess();

  /* Wait until CPU2 boots and enters in stop mode or timeout*/
  while((__HAL_RCC_GET_FLAG(RCC_FLAG_D2CKRDY) != RESET) && (timeout-- > 0));
  if ( timeout == 0 )
  {
    DualCoreEnabled = 0;
  }
  else
  {
    DualCoreEnabled = 1;
  }

  /* Configure the MPU attributes as Write Through */
  MPU_Config();
  
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

#if defined (USE_STM32H747I_EVAL_OOB)
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
#endif /* USE_STM32H747I_EVAL_OOB */

  /* Configure the board */
  BSP_Initialized = k_BspInit(); 

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
  k_ModuleAdd(&dual_core_board);  

  if(DualCoreEnabled)
  {
    /* Initialize FreeRTOS/AMP IPC */
    ipc_init();

    printf("Resuming CPU2 : ");
    /*HW semaphore Clock enable*/
    __HAL_RCC_HSEM_CLK_ENABLE();
    /*Take HSEM */
    HAL_HSEM_FastTake(HSEM_ID_0);   
    /*Release HSEM in order to notify the CPU2(CM4)*/     
    HAL_HSEM_Release(HSEM_ID_0, 0);
    /* Wait until CPU2 boots and enters in stop mode or timeout*/
    timeout = 0xFFFF;
    while((__HAL_RCC_GET_FLAG(RCC_FLAG_D2CKRDY) == RESET) && (timeout-- > 0));
    if ( timeout == 0 )
      printf("KO\n");
    else
      printf("OK\n");
  }

  /* Initialize Storage Units */
  k_StorageInit();

  /* Start scheduler */
  osKernelStart ();

  /* We should never get here as control is now taken by the scheduler */
  for( ;; );
}

/**
  * @brief  Start task
  * @param  argument: pointer that is passed to the thread function as start argument.
  * @retval None
  */
static void GUIThread(void const * argument)
{  
  /* Initialize GUI */
  GUI_Init();

  /* Enable Multibuffereing and set Layer0 as the default display layer */ 
  WM_MULTIBUF_Enable(1);
  GUI_SetLayerVisEx (1, 0);
  GUI_SelectLayer(0);

  /* Show the main menu */
  k_InitMenu();  
  
  /* Display immediately the Menu */
  GUI_Exec();

  /* Gui background Task */
  while(1) {
    GUI_Delay(10);
  }
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

  /*!< Supply configuration update enable */
  if(power_config == PWR_CFG_LDO)
  {
    HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);
  }
  else
  {
    if(DEVICE_IS_CUT_2_1() == 0)
    {
      /* WA to avoid losing SMPS regulation in run mode */
      PWDDBG->PDR1 = 0xCAFECAFE;
      __DSB();
      PWDDBG->PDR1 |= (1<<5 | 1<<3);
      __DSB();
    }
    HAL_PWREx_ConfigSupply(PWR_DIRECT_SMPS_SUPPLY);
  }

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
  
  /*
  Note : The activation of the I/O Compensation Cell is recommended with communication  interfaces
          (GPIO, SPI, FMC, QSPI ...)  when  operating at  high frequencies(please refer to product datasheet)       
          The I/O Compensation Cell activation  procedure requires :
        - The activation of the CSI clock
        - The activation of the SYSCFG clock
        - Enabling the I/O Compensation Cell : setting bit[0] of register SYSCFG_CCCSR
  
          To do this please uncomment the following code 
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

void BSP_ErrorHandler(void)
{
  if(BSP_Initialized)
	BSP_LED_On(LED_RED);
}


/**
  * @brief  Configure the MPU attributes as Write Through for SRAM1/2.
  * @note   The Base Address is 0x24000000 since this memory interface is the AXI.
  *         The Region Size is 512KB, it is related to D1-AXI SRAM memory size.
  * @param  None
  * @retval None
  */
void MPU_Config( void )
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

  /* Configure the MPU attributes as WB for Flash */
  MPU_InitStruct.Enable           = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress      = FLASH_BASE;
  MPU_InitStruct.Size             = MPU_REGION_SIZE_2MB;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable     = MPU_ACCESS_BUFFERABLE;
  MPU_InitStruct.IsCacheable      = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsShareable      = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.Number           = MPU_REGION_NUMBER1;
  MPU_InitStruct.TypeExtField     = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec      = MPU_INSTRUCTION_ACCESS_ENABLE;
  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Configure the MPU attributes for Quad-SPI area to strongly ordered
     This setting is essentially needed to avoid MCU blockings!
     See also STM Application Note AN4861 */
  MPU_InitStruct.Enable           = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress      = QSPI_BASE_ADDRESS;
  MPU_InitStruct.Size             = MPU_REGION_SIZE_256MB;
  MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
  MPU_InitStruct.IsBufferable     = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable      = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsShareable      = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.Number           = MPU_REGION_NUMBER2;
  MPU_InitStruct.TypeExtField     = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec      = MPU_INSTRUCTION_ACCESS_DISABLE;
  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Configure the MPU attributes for the QSPI 64MB to normal memory Cacheable, must reflect the real memory size */
  MPU_InitStruct.Enable           = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress      = QSPI_BASE_ADDRESS;
  MPU_InitStruct.Size             = MPU_REGION_SIZE_128MB;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable     = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable      = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsShareable      = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.Number           = MPU_REGION_NUMBER3;
  MPU_InitStruct.TypeExtField     = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec      = MPU_INSTRUCTION_ACCESS_DISABLE;
  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Setup SDRAM in Write-through (framebuffer) */
  MPU_InitStruct.Enable           = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress      = SDRAM_DEVICE_ADDR;
  MPU_InitStruct.Size             = MPU_REGION_SIZE_32MB;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable     = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable      = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsShareable      = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.Number           = MPU_REGION_NUMBER4;
  MPU_InitStruct.TypeExtField     = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec      = MPU_INSTRUCTION_ACCESS_DISABLE;
  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Setup AXI SRAM in Write-through */
  MPU_InitStruct.Enable           = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress      = D1_AXISRAM_BASE;
  MPU_InitStruct.Size             = MPU_REGION_SIZE_512KB;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable     = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable      = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsShareable      = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.Number           = MPU_REGION_NUMBER5;
  MPU_InitStruct.TypeExtField     = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec      = MPU_INSTRUCTION_ACCESS_DISABLE;
  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Disable access to D2 SRAM1 */
  MPU_InitStruct.Enable           = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress      = D2_AXISRAM_BASE;
  MPU_InitStruct.Size             = MPU_REGION_SIZE_256KB;
  MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
  MPU_InitStruct.IsBufferable     = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable      = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsShareable      = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.Number           = MPU_REGION_NUMBER6;
  MPU_InitStruct.TypeExtField     = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec      = MPU_INSTRUCTION_ACCESS_DISABLE;
  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Setup D2 SRAM2 as device memory */
  MPU_InitStruct.Enable           = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress      = (D2_AXISRAM_BASE + 0x40000);
  MPU_InitStruct.Size             = MPU_REGION_SIZE_32KB;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable     = MPU_ACCESS_BUFFERABLE;
  MPU_InitStruct.IsCacheable      = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsShareable      = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.Number           = MPU_REGION_NUMBER7;
  MPU_InitStruct.TypeExtField     = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec      = MPU_INSTRUCTION_ACCESS_DISABLE;
  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Disable access to D2 SRAM3 */
  MPU_InitStruct.Enable           = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress      = D3_SRAM_BASE;
  MPU_InitStruct.Size             = MPU_REGION_SIZE_64KB;
  MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
  MPU_InitStruct.IsBufferable     = MPU_ACCESS_BUFFERABLE;
  MPU_InitStruct.IsCacheable      = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsShareable      = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.Number           = MPU_REGION_NUMBER8;
  MPU_InitStruct.TypeExtField     = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec      = MPU_INSTRUCTION_ACCESS_DISABLE;
  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
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

