/**
  ******************************************************************************
  * @file    HASH/HASH_SHA224SHA256_DMA/Src/main.c
  * @author  MCD Application Team
  * @brief   This example provides a short description of HASH digest calculation
  *          using SHA224 and SHA256 example.
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

/** @addtogroup HASH_SHA224SHA256_DMA
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* HASH handler declaration */
HASH_HandleTypeDef     HashHandle;

/*Input Buffer */
__ALIGN_BEGIN const uint8_t Input[] __ALIGN_END = "The STM32 H7 series is the result of a perfect symbiosis of the real-time control capabilities of an MCU and the signal processing performance of a DSP, and thus complements the STM32 portfolio with a new class of devices, digital signal controllers (DSC).";

/* Expected Buffer for SHA224  */
__ALIGN_BEGIN const uint8_t ExpectSHA224Digest[28] __ALIGN_END = {0xdc, 0x10, 0x4a, 0x67, 0x5a, 0xed, 0x5a, 0x3e,
                                                                0xc1, 0x02, 0x85, 0x16, 0x6a, 0x10, 0xcc, 0x9a,
                                                                0x3d, 0xe7, 0x06, 0xcf, 0xe6, 0x5c, 0x40, 0xaf,
                                                                0xd4, 0xbd, 0x89, 0x6e,};

/* Expected Buffer in case of SHA256  */
__ALIGN_BEGIN const uint8_t ExpectSHA256Digest[32] __ALIGN_END = {0x18, 0xaa, 0x28, 0xaa, 0xf2, 0xad, 0xb2, 0x47, 
                                                                0x78, 0x47, 0xee, 0xc7, 0x45, 0x0a, 0x16, 0x38, 
                                                                0x45, 0xa2, 0xd9, 0x01, 0x48, 0xb1, 0x7c, 0x47,
                                                                0x2e, 0x22, 0xc8, 0x6b, 0x4f, 0xb4, 0x15, 0xa3};

/* Digest Buffer location should aligned to cache line size (32 bytes) */
ALIGN_32BYTES (static uint8_t Digest[32])={0};  

/* Private function prototypes -----------------------------------------------*/
static void MPU_Config(void);
static void SystemClock_Config(void);
static void CPU_CACHE_Enable(void);
static void Error_Handler(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
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

  /* Configure the system clock to 400 MHz */
  SystemClock_Config();

  /* Configure LED1, LED3 and LED4 */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED3);
  BSP_LED_Init(LED4);

  /****************************************************************************/
  /****************************** SHA224 **************************************/
  /****************************************************************************/
  if(HAL_HASH_DeInit(&HashHandle) != HAL_OK)
  {
    Error_Handler();
  }
  HashHandle.Init.DataType = HASH_DATATYPE_8B;

  if (HAL_HASH_Init(&HashHandle) != HAL_OK)
  {
    Error_Handler();
  }

  /* Start HASH computation using DMA transfer */
  if (HAL_HASHEx_SHA224_Start_DMA(&HashHandle, (uint8_t*)Input, strlen((char const*)Input)) != HAL_OK)
  {
    Error_Handler();
  }

  /* Get the computed digest value */
  if(HAL_HASHEx_SHA224_Finish(&HashHandle, Digest, 0xFF) != HAL_OK)
  {
    Error_Handler();
  }

  /* Compare computed digest with expected one */
  if(memcmp(Digest, ExpectSHA224Digest, 28) != 0)
  {
    BSP_LED_On(LED3);
  }
  else
  {
    BSP_LED_On(LED1);
  }

  /***************************** SHA256 ***************************************/
  /* CPU Data Cache maintenance :
    It is recommended to clean the CPU Data cache before starting the  DMA transfer. 
    As the source buffer may be prepared by the CPU, this guarantees that the source buffer 
    (if located in the D1 AXI-SRAM which is cacheable) will be up to date before starting the transfer.
  */
  SCB_CleanDCache_by_Addr((uint32_t*)Digest, 32); 
  
  if(HAL_HASH_DeInit(&HashHandle) != HAL_OK)
  {
    Error_Handler();
  }
  HashHandle.Init.DataType = HASH_DATATYPE_8B;
  
  if(HAL_HASH_Init(&HashHandle) != HAL_OK)
  {
    Error_Handler();
  }
  /* Start HASH computation using DMA transfer */
  if (HAL_HASHEx_SHA256_Start_DMA(&HashHandle, (uint8_t*)Input, strlen((char const*)Input)) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* CPU Data Cache maintenance :  
    It is recommended to invalidate the CPU Data cache after the DMA transfer. 
    As the destination buffer may be used by the CPU, this guarantees up-to-date data when CPU access 
    to the destination buffer located in the D1 AXI-SRAM (which is cacheable).
  */ 
  SCB_InvalidateDCache_by_Addr((uint32_t*)Digest, 32);
  
  /* Get the computed digest value */
  if (HAL_HASHEx_SHA256_Finish(&HashHandle, Digest, 0xFF) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* Compare computed digest with expected one */
  if(memcmp(Digest, ExpectSHA256Digest, 32) != 0)
  {
    Error_Handler();
  }
  else
  {
    BSP_LED_On(LED4);
  }
  while(1)
  {
  }
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

