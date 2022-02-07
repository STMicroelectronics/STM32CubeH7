/**
  ******************************************************************************
  * @file    MenuLauncher/Core/CM7/Src/main.c
  * @author  MCD Application Team
  *          This is the main program for Cortex-M7
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
#include <stddef.h>
#include "main.h"
#include "splash.h"
#include "bsp.h"
#include "WM.h"
#include "MenuLauncher.h"
#if configAPPLICATION_ALLOCATED_HEAP == 1
#if defined ( __ICCARM__ )
#pragma location="heap_mem"
#else
__attribute__((section("heap_mem")))
#endif
uint8_t ucHeap[ configTOTAL_HEAP_SIZE ];
#endif /* configAPPLICATION_ALLOCATED_HEAP */

extern void LauncherStartUp(const uint8_t SoftwareReset);

/* Private typedef -----------------------------------------------------------*/
typedef  void (*pFunc)(void);

#define PWR_CFG_SMPS	0xCAFECAFE
#define PWR_CFG_LDO		0x5ACAFE5A

/* Private define ------------------------------------------------------------*/
#define HSEM_ID_0                       (0U) /* HW semaphore 0*/
#define AUTO_DEMO_TIMEOUT_0               20
#define AUTO_DEMO_TIMEOUT_1                5

#define TS_TaskPRIORITY                 osPriorityHigh /* osPriorityNormal osPriorityRealtime */
#define TS_TaskSTACK_SIZE               (configMINIMAL_STACK_SIZE * 2)

#define AUTODEMO_TaskPRIORITY           osPriorityAboveNormal /* osPriorityNormal osPriorityRealtime */
#define AUTODEMO_TaskSTACK_SIZE         (configMINIMAL_STACK_SIZE * 2)

#define GUI_TaskPRIORITY                osPriorityNormal /* osPriorityNormal osPriorityRealtime */
#define GUI_TaskSTACK_SIZE              (configMINIMAL_STACK_SIZE * 64)

/* Private macro -------------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static __IO uint32_t SoftwareReset = 0;
static uint32_t AutoDemoTimeOutMs = AUTO_DEMO_TIMEOUT_0;
static uint32_t AutoDemoId = 0;
static uint8_t BSP_Initialized = 0;
osSemaphoreId TSSemaphoreID;
osMessageQId AutoDemoEvent = {0};

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void MPU_Config(void);
static void CPU_CACHE_Enable(void);
static void TouchScreenTask(void const *argument);
static void GUIThread(void const * argument);

extern void SUBDEMO_StartAutoDemo(const uint8_t demo_id);

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
  * @brief  Configure the MPU attributes as Write Through for Internal D1SRAM.
  * @note   The Base Address is 0x24000000 since this memory interface is the AXI.
  *         The Configured Region Size is 512KB because same as D1SRAM size.
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

  /* Configure the MPU attributes as SO for whole 2MB of Flash memory */
  MPU_InitStruct.Enable           = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress      = FLASH_BASE;
  MPU_InitStruct.Size             = MPU_REGION_SIZE_2MB;
  MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
  MPU_InitStruct.IsBufferable     = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable      = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsShareable      = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.Number           = MPU_REGION_NUMBER1;
  MPU_InitStruct.TypeExtField     = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec      = MPU_INSTRUCTION_ACCESS_DISABLE;
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

  /* Configure the MPU attributes for the QSPI 128MB to Cacheable WT */
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

  /* Configure the MPU attributes as WB for implemented Flash memory */
  MPU_InitStruct.Enable           = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress      = FLASH_BASE;
  MPU_InitStruct.Size             = MPU_REGION_SIZE_128KB;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable     = MPU_ACCESS_BUFFERABLE;
  MPU_InitStruct.IsCacheable      = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsShareable      = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.Number           = MPU_REGION_NUMBER4;
  MPU_InitStruct.TypeExtField     = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec      = MPU_INSTRUCTION_ACCESS_ENABLE;
  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Configure the MPU attributes for the QSPI 2MB to Cacheable WT with instruction access */
  MPU_InitStruct.Enable           = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress      = QSPI_BASE_ADDRESS;
  MPU_InitStruct.Size             = MPU_REGION_SIZE_2MB;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable     = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable      = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsShareable      = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.Number           = MPU_REGION_NUMBER5;
  MPU_InitStruct.TypeExtField     = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec      = MPU_INSTRUCTION_ACCESS_ENABLE;
  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Setup SDRAM in SO */
  MPU_InitStruct.Enable           = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress      = SDRAM_DEVICE_ADDR;
  MPU_InitStruct.Size             = MPU_REGION_SIZE_256MB;
  MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
  MPU_InitStruct.IsBufferable     = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable      = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsShareable      = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.Number           = MPU_REGION_NUMBER6;
  MPU_InitStruct.TypeExtField     = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec      = MPU_INSTRUCTION_ACCESS_DISABLE;
  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Setup SDRAM in Write-through (Buffers) */
  MPU_InitStruct.Enable           = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress      = SDRAM_DEVICE_ADDR;
  MPU_InitStruct.Size             = MPU_REGION_SIZE_32MB;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable     = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable      = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsShareable      = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.Number           = MPU_REGION_NUMBER7;
  MPU_InitStruct.TypeExtField     = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec      = MPU_INSTRUCTION_ACCESS_DISABLE;
  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Setup AXI SRAM, SRAM1 and SRAM2 in Write-through */
  MPU_InitStruct.Enable           = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress      = D1_AXISRAM_BASE;
  MPU_InitStruct.Size             = MPU_REGION_SIZE_512KB;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable     = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable      = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsShareable      = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.Number           = MPU_REGION_NUMBER8;
  MPU_InitStruct.TypeExtField     = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec      = MPU_INSTRUCTION_ACCESS_DISABLE;
  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Setup D2 SRAM1 & SRAM2 in Write-through */
  MPU_InitStruct.Enable           = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress      = D2_AXISRAM_BASE;
  MPU_InitStruct.Size             = MPU_REGION_SIZE_256KB;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable     = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable      = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsShareable      = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.Number           = MPU_REGION_NUMBER9;
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

/**
  * @brief  TouchScreen task
  * @param  argument: pointer that is passed to the thread function as start argument.
  * @retval None
  */
static void TouchScreenTask(void const *argument)
{
  /* Create the TS semaphore */
  osSemaphoreDef(TSSemaphore);
  TSSemaphoreID = osSemaphoreCreate(osSemaphore(TSSemaphore), 1);
  /* initially take the TS Lock */
  osSemaphoreWait( TSSemaphoreID, osWaitForever );

  while(1)
  {
    osSemaphoreWait( TSSemaphoreID, osWaitForever );

    /* Capture input event and updade cursor */
    if(BSP_Initialized == 1)
    {
      if(BSP_TouchUpdate() && AutoDemoEvent)
      {
        osMessagePut ( AutoDemoEvent, AUTO_DEMO_RESET, 0);
      }
    }
  }
}

/**
  * @brief  Start task
  * @param  argument: pointer that is passed to the thread function as start argument.
  * @retval None
  */
static void GUIThread(void const * argument)
{
  GUI_Init();

  /* Activate the use of memory device feature */
  WM_MULTIBUF_Enable(1);

  /* Demo Startup */
  LauncherStartUp(SoftwareReset);

  /* Gui background Task */
  while(1) {
    GUI_Delay(10);
  }
}

static void AutoDemoTask(void const * argument)
{
  uint32_t EventTimeOut = (AutoDemoTimeOutMs * 1000);
  osEvent event;
  AutoDemoEvent_t OldState = AUTO_DEMO_RESET;
  AutoDemoEvent_t State = AUTO_DEMO_ENABLE; 

  /* Create Storage Message Queue */
  osMessageQDef(AutoDemoQueue, 10, uint16_t);
  AutoDemoEvent = osMessageCreate (osMessageQ(AutoDemoQueue), NULL);

  printf("Starting Auto Demo task - Timeout = %lu\n", AutoDemoTimeOutMs);

  for ( ; ; )
  {
    event = osMessageGet( AutoDemoEvent, EventTimeOut );

    if( event.status == osEventMessage )
    {
      OldState = State;
      switch (event.value.v)
      {
        case AUTO_DEMO_DISABLE :
          if(State != AUTO_DEMO_DISABLE)
          {
            EventTimeOut  = osWaitForever;
            State         = AUTO_DEMO_DISABLE;
            printf("%s : DISABLED\n", __func__);
          }
        break;

        case AUTO_DEMO_ENABLE :
          if(State != AUTO_DEMO_ENABLE)
          {
            EventTimeOut  = (AutoDemoTimeOutMs * 1000);
            State         = AUTO_DEMO_ENABLE;
            printf("%s : ENABLED\n", __func__);
          }
        break;

        case AUTO_DEMO_RESET :
          if(State == AUTO_DEMO_ENABLE)
          {
            /* Reset AutoDemo ID and TimeOut */
            AutoDemoTimeOutMs = AUTO_DEMO_TIMEOUT_0;
            AutoDemoId        = DEMO_ID_0;
            State             = AUTO_DEMO_RESET;
            printf("%s : RESET\n", __func__);
          }
        break;

        default :
        break;
      }
    }
    else if ( event.status == osEventTimeout )
    {
      /* Start the AutoDemo Mode */
      AutoDemoTimeOutMs = AUTO_DEMO_TIMEOUT_1;
      if ( AutoDemoId > DEMO_ID_1 )
      {
        AutoDemoId = DEMO_ID_0;
      }
      AutoDemoId++;
      EventTimeOut  = 1000;
      OldState      = AUTO_DEMO_DISABLE;
    }

    if(OldState != State)
    {
      printf("Updating Backup registers - 0x%08lu\n", ((AutoDemoTimeOutMs << 16) | (AutoDemoId << 8) | 0x1));
      HAL_PWR_EnableBkUpAccess();
      WRITE_REG(BKP_REG_SW_CFG, ((AutoDemoTimeOutMs << 16) | (AutoDemoId << 8) | 0x1));
      HAL_PWR_DisableBkUpAccess();
    }

    if(AutoDemoTimeOutMs == AUTO_DEMO_TIMEOUT_1)
    {
      /* Now try to jump to the selected demo */
      printf("%s : Jumping to Sub-Demo %lu\n", __func__, (AutoDemoId-1));
      SUBDEMO_StartAutoDemo((AutoDemoId-1));
    }
  }
}

void BSP_TS_Callback(uint32_t Instance)
{
  if(TSSemaphoreID)
  {
    osSemaphoreRelease(TSSemaphoreID);
  }
}

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  static __IO uint32_t thirdPartyAdress = 0;
  static __IO uint32_t BckRegValue = 0;

  /* Disable FMC Bank1 to avoid speculative/cache accesses */
  FMC_Bank1_R->BTCR[0] &= ~FMC_BCRx_MBKEN;

  /* Enable RCC PWR */
  __HAL_RCC_RTC_ENABLE();

  /* When system initialization is finished, Cortex-M7 will release (wakeup) Cortex-M4  by means of
     HSEM notification. Cortex-M4 release could be also ensured by any Domain D2 wakeup source (SEV,EXTI..).
  */

  /* Enable RTC back-up registers access */
  __HAL_RCC_RTC_CLK_ENABLE();
  HAL_PWR_EnableBkUpAccess();

  BckRegValue       = READ_REG(BKP_REG_SW_CFG);
  thirdPartyAdress  = READ_REG(BKP_REG_SUBDEMO_ADDRESS);

  SoftwareReset     = (((BckRegValue & 0x000000FF)      ) & 0xFF);
  AutoDemoId        = (((BckRegValue & 0x0000FF00) >>  8) & 0xFF);
  AutoDemoTimeOutMs = (((BckRegValue & 0xFFFF0000) >> 16) & 0xFFFF);

  if ( AutoDemoTimeOutMs == 0 )
  {
    AutoDemoTimeOutMs = (2 * AUTO_DEMO_TIMEOUT_0);
  }
  
  if(SoftwareReset == 2)
  {
    AutoDemoTimeOutMs = AUTO_DEMO_TIMEOUT_0;
  }

#if defined (USE_PWR_LDO_SUPPLY)
  WRITE_REG(BKP_REG_PWR_CFG, PWR_CFG_LDO);
#else
  WRITE_REG(BKP_REG_PWR_CFG, PWR_CFG_SMPS);
#endif /* USE_PWR_LDO_SUPPLY */
  WRITE_REG(BKP_REG_SW_CFG, ((AutoDemoTimeOutMs << 16) | (AutoDemoId << 8) | 0x1));
  WRITE_REG(BKP_REG_SUBDEMO_ADDRESS, 0);

  HAL_PWR_DisableBkUpAccess();

  /* System Init, System clock, voltage scaling and L1-Cache configuration are done by CPU1 (Cortex-M7)
     in the meantime Domain D2 is put in STOP mode(Cortex-M4 in deep-sleep)
  */

  /* Configure the MPU attributes as Write Through */
  MPU_Config();

  if (thirdPartyAdress != 0)
  {
    BSP_Initialized = BSP_Config(1);

    if(BSP_Initialized)
    {
      /* Reinitialize the Stack pointer*/
      __set_MSP(*(__IO uint32_t*) thirdPartyAdress);
      /* Jump to application address */
      ((pFunc) (*(__IO uint32_t*) (thirdPartyAdress + 4)))();
    }
  }
  else
  {
    /* Enable the CPU Cache */
    CPU_CACHE_Enable();

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

    /* Configure the system clock to 400 MHz */
    SystemClock_Config();

    /* Enable CRC to Unlock GUI */
    __HAL_RCC_CRC_CLK_ENABLE();

    BSP_Initialized = BSP_Config(0);
  }

  if(BSP_Initialized)
  {
    /***********************************************************/
    /* Create TS Thread */
    osThreadDef(TS_Task, TouchScreenTask, TS_TaskPRIORITY, 1, TS_TaskSTACK_SIZE);
    osThreadCreate(osThread(TS_Task), NULL);

    /* Create GUI task */
    osThreadDef(GUI_Thread, GUIThread, GUI_TaskPRIORITY, 0, GUI_TaskSTACK_SIZE);
    osThreadCreate (osThread(GUI_Thread), NULL);

    /* Create Auto Demo Task */
    osThreadDef(AutoDemoTask, AutoDemoTask, AUTODEMO_TaskPRIORITY, 0, AUTODEMO_TaskSTACK_SIZE);
    osThreadCreate (osThread(AutoDemoTask), NULL);

    /* Start scheduler */
    osKernelStart();
  }

  /* We should never get here as control is now taken by the scheduler */
  while (1)
  {
    BSP_LED_Toggle(LED_RED);
    HAL_Delay(500);
  }
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
  {
	  BSP_LED_On(LED_RED);
  }

  configASSERT (0);
}

void BSP_ErrorHandler(void)
{
  if(BSP_Initialized)
  {
    printf( "%s(): BSP Error !!!\n", __func__ );
    BSP_LED_On(LED_RED);
  }
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

  printf( "ASSERT FAILED at %s, line %d!!!\n", file, line );

  Error_Handler();
}
#endif

/**
  * @}
  */

/**
  * @}
  */

