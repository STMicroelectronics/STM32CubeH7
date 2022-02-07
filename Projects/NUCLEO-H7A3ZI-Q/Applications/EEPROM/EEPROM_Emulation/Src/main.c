/**
  ******************************************************************************
  * @file    EEPROM/EEPROM_Emulation/src/main.c
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32H7xx_HAL_Examples
  * @{
  */

/** @addtogroup BSP
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
FLASH_OBProgramInitTypeDef OBInit;

/* Virtual address defined by the user: 0xFFFF value is prohibited */
uint16_t VirtAddVarTab[NB_OF_VAR] = {0x5555, 0x6666, 0x7777};
uint16_t VarDataTab[NB_OF_VAR] = {0, 0, 0};
uint16_t VarValue,VarDataTmp = 0;

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Error_Handler(void);
static void CPU_CACHE_Enable(void);
static void MPU_Config(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
  /* Enable the CPU Cache */
  CPU_CACHE_Enable();

  /* Configure the MPU attributes as Write Through for Flash Bank 2 sectors 126 and 127  (used for EEPROM emulation)*/
  MPU_Config();

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock to have a system clock = 280 Mhz */
  SystemClock_Config();

  /* Unlock the Flash Program Erase controller */
  HAL_FLASH_Unlock();

  /* Configure LED1, LED2 and LED3 */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);
  BSP_LED_Init(LED3);

  /* Toggle LED2 */
  BSP_LED_Toggle(LED2);

  /* EEPROM Init */
  if( EE_Init() != EE_OK)
  {
    Error_Handler();
  }

  /* Toggle LED2 */
  BSP_LED_Toggle(LED2);

  /* --- Store successively many values of the three variables in the EEPROM ---*/
  /* Store 0x1000 values of Variable1 in EEPROM */
  for (VarValue = 1; VarValue <= 0x1000; VarValue++)
  {
    /* Sequence 1 */
    if((EE_WriteVariable(VirtAddVarTab[0],  VarValue)) != HAL_OK)
    {
      Error_Handler();
    }

    if((EE_ReadVariable(VirtAddVarTab[0],  &VarDataTab[0])) != HAL_OK)
    {
      Error_Handler();
    }
    if (VarValue != VarDataTab[0])
    {
      Error_Handler();
    }

    /* Sequence 2 */
    if(EE_WriteVariable(VirtAddVarTab[1], ~VarValue) != HAL_OK)
    {
      Error_Handler();
    }

    if(EE_ReadVariable(VirtAddVarTab[1],  &VarDataTab[1]) != HAL_OK)
    {
      Error_Handler();
    }
    if(((uint16_t)~VarValue) != VarDataTab[1])
    {
      Error_Handler();
    }

    /* Toggle LED2 */
    BSP_LED_Toggle(LED2);

    /* Sequence 3 */
    if(EE_WriteVariable(VirtAddVarTab[2],  VarValue << 1) != HAL_OK)
    {
      Error_Handler();
    }

    if(EE_ReadVariable(VirtAddVarTab[2],  &VarDataTab[2]) != HAL_OK)
    {
      Error_Handler();
    }
    if ((VarValue << 1) != VarDataTab[2])
    {
      Error_Handler();
    }
    /* Toggle LED2 */
    BSP_LED_Toggle(LED2);
  }

  /* Store 0x2000 values of Variable2 in EEPROM */
  for (VarValue = 1; VarValue <= 0x2000; VarValue++)
  {
    if(EE_WriteVariable(VirtAddVarTab[1], VarValue) != HAL_OK)
    {
      Error_Handler();
    }

    if(EE_ReadVariable(VirtAddVarTab[1], &VarDataTab[1]) != HAL_OK)
    {
      Error_Handler();
    }

    if(VarValue != VarDataTab[1])
    {
      Error_Handler();
    }
    /* Toggle LED2 */
    BSP_LED_Toggle(LED2);
  }

  /* read the last stored variables data*/
  if(EE_ReadVariable(VirtAddVarTab[0], &VarDataTmp) != HAL_OK)
  {
    Error_Handler();
  }

  if (VarDataTmp != VarDataTab[0])
  {
    Error_Handler();
  }

  if(EE_ReadVariable(VirtAddVarTab[1], &VarDataTmp) != HAL_OK)
  {
    Error_Handler();
  }

  if (VarDataTmp != VarDataTab[1])
  {
    Error_Handler();
  }

  if(EE_ReadVariable(VirtAddVarTab[2], &VarDataTmp) != HAL_OK)
  {
    Error_Handler();
  }

  if (VarDataTmp != VarDataTab[2])
  {
    Error_Handler();
  }
  /* Toggle LED2 */
  BSP_LED_Toggle(LED2);


  /* Store 0x3000 values of Variable3 in EEPROM */
  for (VarValue = 1; VarValue <= 0x3000; VarValue++)
  {
    if(EE_WriteVariable(VirtAddVarTab[2], VarValue) != HAL_OK)
    {
      Error_Handler();
    }

    if(EE_ReadVariable(VirtAddVarTab[2], &VarDataTab[2]) != HAL_OK)
    {
      Error_Handler();
    }
    if(VarValue != VarDataTab[2])
    {
      Error_Handler();
    }
    /* Toggle LED2 */
    BSP_LED_Toggle(LED2);
  }

  /* read the last stored variables data*/
  if(EE_ReadVariable(VirtAddVarTab[0], &VarDataTmp) != HAL_OK)
  {
    Error_Handler();
  }

  if (VarDataTmp != VarDataTab[0])
  {
    Error_Handler();
  }

  if(EE_ReadVariable(VirtAddVarTab[1], &VarDataTmp) != HAL_OK)
  {
    Error_Handler();
  }
  if (VarDataTmp != VarDataTab[1])
  {
    Error_Handler();
  }
  /* Toggle LED2 */
  BSP_LED_Toggle(LED2);

  if(EE_ReadVariable(VirtAddVarTab[2], &VarDataTmp) != HAL_OK)
  {
    Error_Handler();
  }
  if (VarDataTmp != VarDataTab[2])
  {
    Error_Handler();
  }

  while (1)
  {
    /* Turn ON LED1 */
    BSP_LED_On(LED1);
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
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 4
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
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};

  /*!< Supply configuration update enable */
  HAL_PWREx_ConfigSupply(PWR_DIRECT_SMPS_SUPPLY);

  /* The voltage scaling allows optimizing the power consumption when the device is
  clocked below the maximum system frequency, to update the voltage scaling value
  regarding system frequency refer to product datasheet.
  */
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
  RCC_OscInitStruct.PLL.PLLN = 280;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;

  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_1;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }

  /* Select PLL as system clock source and configure the bus clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_D1PCLK1 | RCC_CLOCKTYPE_PCLK1 | \
    RCC_CLOCKTYPE_PCLK2  | RCC_CLOCKTYPE_D3PCLK1);

  RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider  = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider  = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_6) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }
}

/**
  * @brief  Configure the MPU attributes as Normal Non Cacheable for Flash Bank 2 sectors 126 and 127 (used for EEPROM emulation).
  * @note   The Base Address is 0x081FC000 .
  *         The Configured Region Size is 256KB (2 x 128 KB for 2 flash sectors).
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

  /* Configure the MPU attributes as Normal Non Cacheable for Flash Bank 2 sectros 126 and 127 */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress = ADDR_FLASH_SECTOR_126_BANK2;
  MPU_InitStruct.Size = MPU_REGION_SIZE_256KB;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER1;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL1;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Enable the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* Turn ON LED3 */
  BSP_LED_On(LED3);

  /* Infinite loop */
  while (1)
  {
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

