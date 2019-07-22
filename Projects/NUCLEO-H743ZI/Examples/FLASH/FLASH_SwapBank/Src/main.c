/**
  ******************************************************************************
  * @file    FLASH/FLASH_SwapBank/Src/main.c
  * @author  MCD Application Team
  * @brief   This example provides a description of how to erase and program the
  *          STM32H7xx FLASH.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32H7xx_HAL_Examples
  * @{
  */

/** @addtogroup FLASH_EraseProgram
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
FLASH_OBProgramInitTypeDef OBInit;
/* Private function prototypes -----------------------------------------------*/
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

  /* Configure User push-button */
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);

  /* Initialize LED1 and LED2 */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);

  
  /* Unlock tha User Flash area */
  HAL_FLASH_Unlock();
  
  HAL_FLASH_OB_Unlock();
  
  /*--- If Wake-up button is pushed, Swap Bank1/Bank2 contents ---*/

  while (1)
  {
    /* Wait for BUTTON_USER is released */
    if (BSP_PB_GetState(BUTTON_USER) == SET)
    {
      while (BSP_PB_GetState(BUTTON_USER) == SET);

      /* Get the Dual boot configuration status */
      HAL_FLASHEx_OBGetConfig(&OBInit);

      /* Get FLASH_WRP_SECTORS write protection status */
      OBInit.Banks     = FLASH_BANK_1;
      HAL_FLASHEx_OBGetConfig(&OBInit);
      
      /* Check Swap Flash banks  status */
      if ((OBInit.USERConfig & OB_SWAP_BANK_ENABLE) == OB_SWAP_BANK_DISABLE) 
      {
        /*Swap to bank2 */
        /*Set OB SWAP_BANK_OPT to swap Bank2*/
        OBInit.OptionType = OPTIONBYTE_USER;
        OBInit.USERType   = OB_USER_SWAP_BANK;
        OBInit.USERConfig = OB_SWAP_BANK_ENABLE;
        HAL_FLASHEx_OBProgram(&OBInit);
        
        /* Launch Option bytes loading */
        HAL_FLASH_OB_Launch();
        
        /*
          as the  CPU is executing from the Flash Bank1, and the I-Cache is enabled :
          Instruction cache must be invalidated after bank switching to ensure that
          CPU will fetch correct instructions from the flash.
        */
        SCB_InvalidateICache();
      }
      else
      {
        /* Swap to bank1 */
        /*Set OB SWAP_BANK_OPT to swap Bank1*/
        OBInit.OptionType = OPTIONBYTE_USER;
        OBInit.USERType = OB_USER_SWAP_BANK;
        OBInit.USERConfig = OB_SWAP_BANK_DISABLE;
        HAL_FLASHEx_OBProgram(&OBInit);
        
        /* Launch Option bytes loading */
        HAL_FLASH_OB_Launch();
        
        /*
          as the  CPU is executing from the Flash Bank1, and the I-Cache is enabled :
          Instruction cache must be invalidated after bank switching to ensure that
          CPU will fetch correct instructions from the flash.
        */
        SCB_InvalidateICache();
      }
    }
    else
    {
#ifdef FLASH_BANK1
      /* Toggle LED1 */
      BSP_LED_Toggle(LED1);
      /* Turn off LED2 */
      BSP_LED_Off(LED2);
#else
      /* Toggle LED2 */
      BSP_LED_Toggle(LED2);
      /* Turn off LED1 */
      BSP_LED_Off(LED1);
#endif

      /* Insert 100 ms delay */
      HAL_Delay(100);
    }
  }
}


/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE BYPASS)
  *            SYSCLK(Hz)                     = 400000000 (CPU Clock)
  *            HCLK(Hz)                       = 200000000 (AXI and AHBs Clock)
  *            AHB Prescaler                  = 2
  *            D1 APB3 Prescaler              = 2 (APB3 Clock  100MHz)
  *            D2 APB1 Prescaler              = 2 (APB1 Clock  100MHz)
  *            D2 APB2 Prescaler              = 2 (APB2 Clock  100MHz)
  *            D3 APB4 Prescaler              = 2 (APB4 Clock  100MHz)
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 4
  *            PLL_N                          = 400
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
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
  RCC_OscInitStruct.CSIState = RCC_CSI_OFF;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;

  RCC_OscInitStruct.PLL.PLLM = 4;
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
