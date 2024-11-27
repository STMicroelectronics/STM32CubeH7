/**
  ******************************************************************************
  * @file    FMC/FMC_NOR/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to configure and use GPIOs through
  *          the STM32H7xx HAL API.
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

/** @addtogroup STM32H7xx_HAL_Examples
  * @{
  */

/** @addtogroup FMC_NOR
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define BUFFER_SIZE         ((uint32_t)0x1000)
#define WRITE_READ_ADDR     ((uint32_t)0x0800)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
NOR_HandleTypeDef hnor;
FMC_NORSRAM_TimingTypeDef NOR_Timing;

/* Read/Write Buffers */
uint16_t aTxBuffer[BUFFER_SIZE];
uint16_t aRxBuffer[BUFFER_SIZE];

/* Status variables */
__IO uint32_t uwWriteReadStatus = 0;

/* Counter index */
uint32_t uwIndex = 0;

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Error_Handler(void);
static void Fill_Buffer(uint16_t *pBuffer, uint32_t uwBufferLenght, uint16_t uwOffset);
static TestStatus Buffercmp(uint16_t *pBuffer1, uint16_t *pBuffer2, uint16_t BufferLength);
static void CPU_CACHE_Enable(void);
static void MPU_Config(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  
  /* Enable the CPU Cache */
  CPU_CACHE_Enable();
  /* Configure the FMC-NOR to device using the MPU */
  MPU_Config();
  
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

  /* Configure LED1 and LED3 */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED3);

  /*##-1- Configure the NOR device ##########################################*/
  /* NOR device configuration */

  hnor.Instance  = FMC_NORSRAM_DEVICE;
  hnor.Extended  = FMC_NORSRAM_EXTENDED_DEVICE;

  NOR_Timing.AddressSetupTime      = 9;
  NOR_Timing.AddressHoldTime       = 1;
  NOR_Timing.DataSetupTime         = 5;
  NOR_Timing.BusTurnAroundDuration = 4;
  NOR_Timing.CLKDivision           = 4;
  NOR_Timing.DataLatency           = 2;
  NOR_Timing.AccessMode            = FMC_ACCESS_MODE_B;
  
  hnor.Init.NSBank             = FMC_NORSRAM_BANK1;
  hnor.Init.DataAddressMux     = FMC_DATA_ADDRESS_MUX_DISABLE;
  hnor.Init.MemoryType         = FMC_MEMORY_TYPE_NOR;
  hnor.Init.MemoryDataWidth    = NOR_MEMORY_WIDTH;
  hnor.Init.BurstAccessMode    = FMC_BURST_ACCESS_MODE_DISABLE;
  hnor.Init.WaitSignalPolarity = FMC_WAIT_SIGNAL_POLARITY_LOW;
  hnor.Init.WaitSignalActive   = FMC_WAIT_TIMING_BEFORE_WS;
  hnor.Init.WriteOperation     = FMC_WRITE_OPERATION_ENABLE;
  hnor.Init.WaitSignal         = FMC_WAIT_SIGNAL_ENABLE;
  hnor.Init.ExtendedMode       = FMC_EXTENDED_MODE_DISABLE;
  hnor.Init.AsynchronousWait   = FMC_ASYNCHRONOUS_WAIT_ENABLE;
  hnor.Init.WriteBurst         = FMC_WRITE_BURST_DISABLE;
  hnor.Init.ContinuousClock    = FMC_CONTINUOUS_CLOCK_SYNC_ASYNC;

  /* Initialize the NOR controller */
  if(HAL_NOR_Init(&hnor, &NOR_Timing, &NOR_Timing) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /*##-2- NOR memory read/write access ######################################*/
  /* Fill the buffer to write */
  Fill_Buffer(aTxBuffer, BUFFER_SIZE, 0xD20F);

  /* Erase block */
  HAL_NOR_Erase_Block(&hnor, WRITE_READ_ADDR, NOR_BANK_ADDR);

  /* Wait until NOR is ready */
  if(HAL_NOR_GetStatus(&hnor, NOR_BANK_ADDR, ERASE_TIMEOUT) != HAL_NOR_STATUS_SUCCESS)
  {
    return HAL_NOR_STATUS_ERROR;
  }

  /* Write data to the NOR memory */
  for (uwIndex = 0; uwIndex < BUFFER_SIZE; uwIndex++)
  {
    /* Write data to NOR */
    HAL_NOR_Program(&hnor, (uint32_t *)(NOR_BANK_ADDR + WRITE_READ_ADDR + 2*uwIndex), &aTxBuffer[uwIndex]);

    /* Read NOR device status */
    if(HAL_NOR_GetStatus(&hnor, NOR_BANK_ADDR, PROGRAM_TIMEOUT) != HAL_NOR_STATUS_SUCCESS)
    {
      return HAL_NOR_STATUS_ERROR;
    }
  }

  /* Read back data from the NOR memory */
  if(HAL_NOR_ReadBuffer(&hnor, NOR_BANK_ADDR + WRITE_READ_ADDR, &aRxBuffer[0], BUFFER_SIZE) != HAL_OK)
  {
    return HAL_ERROR;
  }

  /*##-3- Checking data integrity ############################################*/
  uwWriteReadStatus = Buffercmp(aTxBuffer, aRxBuffer, BUFFER_SIZE);
  
  if (uwWriteReadStatus != PASSED)
  {
    /* KO */
    /* Toggle LED3 */
    while(1)
    {
      BSP_LED_Toggle(LED3);
      HAL_Delay(500);
    }
  }
  else
  {
    /* OK */
    /* Turn on LED1 */
    BSP_LED_On(LED1);
  }

  /* Infinite loop */
  while (1)
  {
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* Turn LED3 on */
  BSP_LED_On(LED3);
  while (1)
  {
  }
}

/**
  * @brief  Fills buffer with user predefined data.
  * @param  pBuffer: pointer on the buffer to fill
  * @param  uwBufferLenght: size of the buffer to fill
  * @param  uwOffset: first value to fill on the buffer
  * @retval None
  */
static void Fill_Buffer(uint16_t *pBuffer, uint32_t uwBufferLenght, uint16_t uwOffset)
{
  uint16_t tmpIndex = 0;

  /* Put in global buffer different values */
  for (tmpIndex = 0; tmpIndex < uwBufferLenght; tmpIndex++ )
  {
    pBuffer[tmpIndex] = tmpIndex + uwOffset;
  }
}

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval PASSED: pBuffer identical to pBuffer1
  *         FAILED: pBuffer differs from pBuffer1
  */
static TestStatus Buffercmp(uint16_t* pBuffer1, uint16_t* pBuffer2, uint16_t BufferLength)
{
  while (BufferLength--)
  {
    if (*pBuffer1 != *pBuffer2)
    {
      return FAILED;
    }

    pBuffer1++;
    pBuffer2++;
  }

  return PASSED;
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 400000000 (CPU Clock)
  *            HCLK(Hz)                       = 200000000 (AXI and AHBs Clock)
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
  
    /*Activate CSI clock mondatory for I/O Compensation Cell*/ 
  __HAL_RCC_CSI_ENABLE() ;
    
  /* Enable SYSCFG clock mondatory for I/O Compensation Cell */
  __HAL_RCC_SYSCFG_CLK_ENABLE() ;
  
  /* Enables the I/O Compensation Cell */ 
  HAL_EnableCompensationCell();
  
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
  * @brief  Configure the MPU attributes as Device for External NOR.
  * @note   The Base Address is NOR_BANK_ADDR
*         The Configured Region Size is 16MB : NOR size.
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

  /* Configure the MPU attributes as device for NOR */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress = NOR_BANK_ADDR;
  MPU_InitStruct.Size = MPU_REGION_SIZE_16MB;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;
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

