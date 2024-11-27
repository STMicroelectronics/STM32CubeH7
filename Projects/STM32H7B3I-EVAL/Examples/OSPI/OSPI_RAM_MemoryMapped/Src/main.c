/**
  ******************************************************************************
  * @file    OSPI/OSPI_RAM_MemoryMapped/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to configure and use OctoSPI through
  *          the STM32H7xx HAL API.
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

/** @addtogroup STM32H7xx_HAL_Examples
  * @{
  */

/** @addtogroup OSPI_RAM_MemoryMapped
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
OSPI_HandleTypeDef OSPIHandle;

/* Buffer used for transmission */
uint8_t aTxBuffer[] = " ****Memory-mapped OSPI communication****  ****Memory-mapped OSPI communication****  ****Memory-mapped OSPI communication****   ****Memory-mapped OSPI communication****   ****Memory-mapped OSPI communication****  ****Memory-mapped OSPI communication****  ";

/* Private function prototypes -----------------------------------------------*/
static void MPU_Config(void);
static void SystemClock_Config(void);
static void Error_Handler(void);
static void CPU_CACHE_Enable(void);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  OSPI_HyperbusCfgTypeDef sHyperbusCfg;
  OSPI_HyperbusCmdTypeDef sCommand;
  OSPI_MemoryMappedTypeDef sMemMappedCfg;

  uint32_t address = 0;
  uint16_t index;
  __IO uint8_t *mem_addr;

  /* Configure the MPU attributes */
  MPU_Config();

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

  /* Configure the system clock to 280 MHz */
  SystemClock_Config();

  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);
  BSP_LED_Init(LED3);

  /* Initialize OctoSPI ----------------------------------------------------- */
  OSPIHandle.Instance = OCTOSPI2;
  HAL_OSPI_DeInit(&OSPIHandle);
        
  OSPIHandle.Init.FifoThreshold         = 4;
  OSPIHandle.Init.DualQuad              = HAL_OSPI_DUALQUAD_DISABLE;
  OSPIHandle.Init.MemoryType            = HAL_OSPI_MEMTYPE_HYPERBUS;
  OSPIHandle.Init.DeviceSize            = OSPI_HYPERRAM_SIZE;
  OSPIHandle.Init.ChipSelectHighTime    = 1;
  OSPIHandle.Init.FreeRunningClock      = HAL_OSPI_FREERUNCLK_DISABLE;
  OSPIHandle.Init.ClockMode             = HAL_OSPI_CLOCK_MODE_0;
  OSPIHandle.Init.WrapSize              = HAL_OSPI_WRAP_NOT_SUPPORTED;
  OSPIHandle.Init.ClockPrescaler        = 3; 
  OSPIHandle.Init.SampleShifting        = HAL_OSPI_SAMPLE_SHIFTING_NONE;
  OSPIHandle.Init.DelayHoldQuarterCycle = HAL_OSPI_DHQC_ENABLE;
  OSPIHandle.Init.DelayBlockBypass      = HAL_OSPI_DELAY_BLOCK_BYPASSED;
  OSPIHandle.Init.ChipSelectBoundary    = 0;

    
  if (HAL_OSPI_Init(&OSPIHandle) != HAL_OK)
  {
    Error_Handler() ;
  }
  
  /* Configure the Hyperbus to access memory space -------------------------- */
  sHyperbusCfg.RWRecoveryTime   = OSPI_HYPERRAM_RW_REC_TIME;
  sHyperbusCfg.AccessTime       = OSPI_HYPERRAM_LATENCY;
  sHyperbusCfg.WriteZeroLatency = HAL_OSPI_LATENCY_ON_WRITE;
  sHyperbusCfg.LatencyMode      = HAL_OSPI_FIXED_LATENCY;
    
  if (HAL_OSPI_HyperbusCfg(&OSPIHandle, &sHyperbusCfg, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }

  /* Memory-mapped mode configuration --------------------------------------- */
  sCommand.AddressSpace = HAL_OSPI_MEMORY_ADDRESS_SPACE;
  sCommand.AddressSize  = HAL_OSPI_ADDRESS_32_BITS;
  sCommand.DQSMode      = HAL_OSPI_DQS_ENABLE;
  sCommand.Address      = 0;
  sCommand.NbData       = 1;
  
  if (HAL_OSPI_HyperbusCmd(&OSPIHandle, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
  
  sMemMappedCfg.TimeOutActivation = HAL_OSPI_TIMEOUT_COUNTER_DISABLE;
  
  if (HAL_OSPI_MemoryMapped(&OSPIHandle, &sMemMappedCfg) != HAL_OK)
  {
    Error_Handler();
  }
  
  while (1)
  {
    /* Writing Sequence ----------------------------------------------- */
    mem_addr = (__IO uint8_t *)(OCTOSPI2_BASE + address);
    for (index = 0; index < BUFFERSIZE; index++)
    {
      *mem_addr = aTxBuffer[index];
      mem_addr++;
    }
    
    /* Reading Sequence ----------------------------------------------- */
    mem_addr = (__IO uint8_t *)(OCTOSPI2_BASE + address);
    for (index = 0; index < BUFFERSIZE; index++)
    {
      if (*mem_addr != aTxBuffer[index])
      {
        BSP_LED_On(LED_RED);
      }
      mem_addr++;
    }
    BSP_LED_Toggle(LED_GREEN);
    HAL_Delay(100);
    
    address += OSPI_HYPERRAM_INCR_SIZE;
    if(address >= OSPI_HYPERRAM_END_ADDR)
    {
      address = 0;
    }
  }
}

/**
  * @brief  Transfer Error callback.
  * @param  hospi: OSPI handle
  * @retval None
  */
void HAL_OSPI_ErrorCallback(OSPI_HandleTypeDef *hospi)
{
  Error_Handler();
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  BSP_LED_On(LED_YELLOW);

  /* User may add here some code to deal with this error */
  while(1)
  {
  }
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
    Error_Handler();
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
    Error_Handler();
  }

    /*
  Note : The activation of the I/O Compensation Cell is recommended with communication  interfaces
          (GPIO, SPI, FMC, OSPI ...)  when  operating at  high frequencies(please refer to product datasheet)
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

  /* Configure the MPU attributes as WT for OCTOSPI2 */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress = OCTOSPI2_BASE;
  MPU_InitStruct.Size = MPU_REGION_SIZE_64MB;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER1;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL1;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Enable the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
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

