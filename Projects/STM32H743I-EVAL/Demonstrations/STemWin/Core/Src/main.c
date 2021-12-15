/**
  ******************************************************************************
  * @file    main.c 
  * @author  MCD Application Team
  * @brief   This file provides main program functions
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
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

/**
* @}
*/ 

/** @defgroup MAIN_Private_Defines
* @{
*/ 
/**
* @}
*/ 


/** @defgroup MAIN_Private_Macros
* @{
*/ 
osTimerId lcd_timer;
int OSReady = 0;

/**
* @}
*/ 


/** @defgroup MAIN_Private_Variables
* @{
*/
/**
* @}
*/ 


/** @defgroup MAIN_Private_FunctionPrototypes
* @{
*/ 

static void SystemClock_Config(void);
static void GUIThread(void const * argument);
static void TimerCallback(void const *n);
static void MPU_Config(void);
static void Error_Handler(void);
static void CPU_CACHE_Enable(void);
       void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName );
/* List of all the module of the Demo */
extern K_ModuleItem_Typedef  video_player_board;
extern K_ModuleItem_Typedef  rocket_game_board;
extern K_ModuleItem_Typedef  audio_player_board;
extern K_ModuleItem_Typedef  analog_clock_board;
extern K_ModuleItem_Typedef  graphic_effects_board;

#if (configAPPLICATION_ALLOCATED_HEAP == 1)
uint8_t  ucHeap[configTOTAL_HEAP_SIZE];
#endif

extern osSemaphoreId LcdUpdateSemaphoreId ;
/**
 * @}
 */ 

/** @defgroup MAIN_Private_Functions
 * @{
 */ 

/**
 * @brief  Main program
 * @param  None
 * @retval int
 */
int main(void)
{  
  /* Configure the MPU attributes as Write Through */
  MPU_Config();
  
  CPU_CACHE_Enable();
  
  /* STM32H7xx HAL library initialization:
  - Configure the Systick to generate an interrupt each 1 msec
  - Set NVIC Group Priority to 4
  - Global MSP (MCU Support Package) initialization
  */
  HAL_Init();
  
  /* Configure the system clock */
  SystemClock_Config();
  
  /* Configure the board */
  k_BspInit(); 
 
  
  /* Enable SRAM1/SRAM2/SRAM3 clocks on D2 domain */
  __HAL_RCC_D2SRAM1_CLK_ENABLE();
  __HAL_RCC_D2SRAM2_CLK_ENABLE();
  __HAL_RCC_D2SRAM3_CLK_ENABLE(); 
  
  /* cxialize RTC */
  k_CalendarBkupInit();    
  
  /* Create GUI task */
  osThreadDef(GUI_Thread, GUIThread, osPriorityLow, 0, 8 * 1024);
  osThreadCreate (osThread(GUI_Thread), NULL); 
  
  /* Initialise Modules function */
  k_ModuleInit();
  
  /* Link modules */ 
  k_ModuleAdd(&audio_player_board);       
  k_ModuleAdd(&video_player_board); 
  k_ModuleAdd(&rocket_game_board);
  k_ModuleAdd(&analog_clock_board);
  k_ModuleAdd(&graphic_effects_board);
  
  /* Create Touch screen Timer */
  osTimerDef(TS_Timer, TimerCallback);
  lcd_timer =  osTimerCreate(osTimer(TS_Timer), osTimerPeriodic, (void *)0);

  /* Start the TS Timer */
  osTimerStart(lcd_timer, 30); 
  
  OSReady = 1;
  
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
  /* Initialize Storage Units */
  k_StorageInit();
  
  /* Initialize GUI */
  GUI_Init();
  
  GUI_X_InitOS();
  
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
    osSemaphoreWait( LcdUpdateSemaphoreId , 1000 );
    GUI_Exec();
  }
}

/**
  * @brief  vApplicationStackOverflowHook
  * @param  xTask : task handle
  * @param  pcTaskName : pointer task name
  * @retval None
  */
void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName )
{
  while(1);
}

/**
  * @brief  Timer callbacsk (40 ms)
  * @param  n: Timer index 
  * @retval None
  */
static void TimerCallback(void const *n)
{  
  k_TouchUpdate();
}



/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 400000000 (Domain1 frequency)
  *            HCLK(Hz)                       = 20000000  (Domain2 frequency)
  *            D1AHB Prescaler                = 1
  *            D1APB1 Prescaler               = 1
  *            D2APB1 Prescaler               = 1
  *            D2APB2 Prescaler               = 1
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
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

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
 
  /*activate CSI clock mondatory for I/O Compensation Cell*/  
  __HAL_RCC_CSI_ENABLE() ;
    
  /* Enable SYSCFG clock mondatory for I/O Compensation Cell */
  __HAL_RCC_SYSCFG_CLK_ENABLE() ;
  
  /* Enables the I/O Compensation Cell */    
  HAL_EnableCompensationCell();  
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

  /* Setup D2 SRAM1 in Write-through */
  MPU_InitStruct.Enable           = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress      = D2_AXISRAM_BASE;
  MPU_InitStruct.Size             = MPU_REGION_SIZE_256KB;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable     = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable      = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsShareable      = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.Number           = MPU_REGION_NUMBER6;
  MPU_InitStruct.TypeExtField     = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec      = MPU_INSTRUCTION_ACCESS_DISABLE;
  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Setup D2 SRAM2 in Write-through */
  MPU_InitStruct.Enable           = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress      = (D2_AXISRAM_BASE + 0x40000);
  MPU_InitStruct.Size             = MPU_REGION_SIZE_32KB;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable     = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable      = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsShareable      = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.Number           = MPU_REGION_NUMBER7;
  MPU_InitStruct.TypeExtField     = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec      = MPU_INSTRUCTION_ACCESS_DISABLE;
  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Setup D2 SRAM3 in Write-through */
  MPU_InitStruct.Enable           = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress      = D3_SRAM_BASE;
  MPU_InitStruct.Size             = MPU_REGION_SIZE_64KB;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable     = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable      = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsShareable      = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.Number           = MPU_REGION_NUMBER8;
  MPU_InitStruct.TypeExtField     = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec      = MPU_INSTRUCTION_ACCESS_DISABLE;
  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Configure the MPU attributes for NOR */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress = 0x60000000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_16MB;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER9;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;

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

