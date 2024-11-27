/**
  ******************************************************************************
  * @file  FLASH/FLASH_WriteProtection/Src/main.c
  * @author  MCD Application Team
  * @brief   This example provides a description of how to erase and program the
  *      STM32H7xx FLASH.
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

/** @addtogroup FLASH_WriteProtection
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define FLASH_WRP_SECTORS   (OB_WRP_SECTOR_6 | OB_WRP_SECTOR_7) /* sectors 6 and 7 */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
FLASH_OBProgramInitTypeDef OBInit;
__IO uint32_t SectorsWRPStatus = 0xFFF;

/* Private function prototypes -----------------------------------------------*/
static void MPU_Config(void);
static void SystemClock_Config(void);
static void CPU_CACHE_Enable(void);
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

  /* Configure Tamper push-button */
  BSP_PB_Init(BUTTON_TAMPER, BUTTON_MODE_GPIO);

  /*##-1- Initialize the LCD #################################################*/
  /* Initialize the LCD */
  BSP_LCD_Init(0, LCD_ORIENTATION_LANDSCAPE);
  UTIL_LCD_SetFuncDriver(&LCD_Driver);
  /* Set LCD Foreground Layer  */
  UTIL_LCD_SetLayer(0);
  /* Set LCD font */
  UTIL_LCD_SetFont(&Font20);

  /*##-2- Display messages on LCD ############################################*/
  /* Clear the LCD */
  UTIL_LCD_Clear(UTIL_LCD_COLOR_WHITE);

  /* Set the LCD Text Color */
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLUE);

  /* Display test name on LCD */
  UTIL_LCD_DisplayStringAtLine(0, (uint8_t *)"   Flash Write  ");
  UTIL_LCD_DisplayStringAtLine(1, (uint8_t *)"   protection test  ");
  UTIL_LCD_DisplayStringAtLine(2, (uint8_t *)"    Press User  ");
  UTIL_LCD_DisplayStringAtLine(3, (uint8_t *)"   Tamper-button  ");

  while (1)
  {
    /* Wait for Tamper push-button to be pushed */
    while (BSP_PB_GetState(BUTTON_TAMPER) != RESET)
    {
    }

    /* Wait for Tamper push-button to be released */
    while (BSP_PB_GetState(BUTTON_TAMPER) != SET)
    {
    }

    /* Get FLASH_WRP_SECTORS write protection status */
    OBInit.Banks     = FLASH_BANK_1;
    HAL_FLASHEx_OBGetConfig(&OBInit);
    SectorsWRPStatus = OBInit.WRPSector & FLASH_WRP_SECTORS;

    if (SectorsWRPStatus == FLASH_WRP_SECTORS)
    {
      /* If FLASH_WRP_SECTORS are write protected, disable the write protection */

      /* Allow Access to option bytes sector */
      HAL_FLASH_OB_Unlock();

      /* Allow Access to Flash control registers and user Flash */
      HAL_FLASH_Unlock();

      /* Disable FLASH_WRP_SECTORS write protection */
      OBInit.OptionType = OPTIONBYTE_WRP;
      OBInit.Banks      = FLASH_BANK_1;
      OBInit.WRPState   = OB_WRPSTATE_DISABLE;
      OBInit.WRPSector  = FLASH_WRP_SECTORS;
      HAL_FLASHEx_OBProgram(&OBInit);

      /* Start the Option Bytes programming process */
      if (HAL_FLASH_OB_Launch() != HAL_OK)
      {
        /* User can add here some code to deal with this error */
        while (1)
        {
        }
      }

      /* Prevent Access to option bytes sector */
      HAL_FLASH_OB_Lock();

      /* Disable the Flash option control register access (recommended to protect
      the option Bytes against possible unwanted operations) */
      HAL_FLASH_Lock();

      /* Get FLASH_WRP_SECTORS write protection status */
      OBInit.Banks     = FLASH_BANK_1;
      HAL_FLASHEx_OBGetConfig(&OBInit);
      SectorsWRPStatus = OBInit.WRPSector & FLASH_WRP_SECTORS;

      /* Check if FLASH_WRP_SECTORS write protection is disabled */
      if (SectorsWRPStatus == 0)
      {
        /* Set the LCD Text Color */
        UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_GREEN);

        UTIL_LCD_DisplayStringAtLine(5, (uint8_t *)"    Write     ");
        UTIL_LCD_DisplayStringAtLine(6, (uint8_t *)"  protection is   ");
        UTIL_LCD_DisplayStringAtLine(7, (uint8_t *)"     disabled     ");
      }
      else
      {
        /* Set the LCD Text Color */
        UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_RED);

        UTIL_LCD_DisplayStringAtLine(5, (uint8_t *)"    Write     ");
        UTIL_LCD_DisplayStringAtLine(6, (uint8_t *)"  protection is   ");
        UTIL_LCD_DisplayStringAtLine(7, (uint8_t *)"   not disabled   ");
      }
    }
    else
    {
      /* If FLASH_WRP_SECTORS are not write protected, enable the write protection */

      /* Allow Access to option bytes sector */
      HAL_FLASH_OB_Unlock();

      /* Allow Access to Flash control registers and user Flash */
      HAL_FLASH_Unlock();

      /* Enable FLASH_WRP_SECTORS write protection */
      OBInit.OptionType = OPTIONBYTE_WRP;
      OBInit.Banks      = FLASH_BANK_1;
      OBInit.WRPState   = OB_WRPSTATE_ENABLE;
      OBInit.WRPSector  = FLASH_WRP_SECTORS;
      HAL_FLASHEx_OBProgram(&OBInit);

      /* Start the Option Bytes programming process */
      if (HAL_FLASH_OB_Launch() != HAL_OK)
      {
        /* User can add here some code to deal with this error */
        while (1)
        {
        }
      }

      /* Prevent Access to option bytes sector */
      HAL_FLASH_OB_Lock();

      /* Disable the Flash option control register access (recommended to protect
      the option Bytes against possible unwanted operations) */
      HAL_FLASH_Lock();

      /* Get FLASH_WRP_SECTORS write protection status */
      OBInit.Banks     = FLASH_BANK_1;
      HAL_FLASHEx_OBGetConfig(&OBInit);
      SectorsWRPStatus = OBInit.WRPSector & FLASH_WRP_SECTORS;

      /* Check if FLASH_WRP_SECTORS are write protected */
      if (SectorsWRPStatus == FLASH_WRP_SECTORS)
      {
        /* Set the LCD Text Color */
        UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_GREEN);
      
        UTIL_LCD_DisplayStringAtLine(5, (uint8_t *)"    Write     ");
        UTIL_LCD_DisplayStringAtLine(6, (uint8_t *)"  protection is   ");
        UTIL_LCD_DisplayStringAtLine(7, (uint8_t *)"     enabled    ");
      }
      else
      {
        /* Set the LCD Text Color */
        UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_RED);

        UTIL_LCD_DisplayStringAtLine(5, (uint8_t *)"    Write     ");
        UTIL_LCD_DisplayStringAtLine(6, (uint8_t *)"  protection is   ");
        UTIL_LCD_DisplayStringAtLine(7, (uint8_t *)"   not enabled    ");
      }
    }
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

  while ((PWR->D3CR & (PWR_D3CR_VOSRDY)) != PWR_D3CR_VOSRDY) {}
 
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
  while(1) { ; }
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
  while(1) { ; }
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
  *     where the assert_param error has occurred.
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

