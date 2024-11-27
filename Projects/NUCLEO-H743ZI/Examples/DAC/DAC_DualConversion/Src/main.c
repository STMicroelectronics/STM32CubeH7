/**
  ******************************************************************************
  * @file    DAC/DAC_DualConversion/Src/main.c
  * @author  MCD Application Team
  * @brief   This example provides a short description of how to use the DAC
  *          peripheral to do a dual conversion.
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

/** @addtogroup DAC_DualConversion
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
DAC_HandleTypeDef    DacHandle;
static DAC_ChannelConfTypeDef sConfig;
__IO uint8_t ubSelectedWavesForm = 1;
__IO uint8_t ubKeyPressed = SET;

/* Private function prototypes -----------------------------------------------*/
static void HRTIM_Config(void);
static void CPU_CACHE_Enable(void);
static void SystemClock_Config(void);
static void Error_Handler(void);

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

  /* Configure the system clock */
  SystemClock_Config();
	
  /* Initialize LED3 */
  BSP_LED_Init(LED3);
	
  DacHandle.Instance = DACx;
	
  /* Initialize the Wkup/Tamper push-button.
     It is used for changing the noise generation between channels */
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);
	
 
  /*##-0- DeInit the DAC peripheral ##########################################*/

   if (HAL_DAC_DeInit(&DacHandle) != HAL_OK)
  {
    /* DeInitialization Error */
    Error_Handler();
  }
  
  /*##-1- Init the DAC peripheral #######################################*/
  
  if (HAL_DAC_Init(&DacHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
	
  /*##-2- Configure the HRTIM peripheral #######################################*/
   HRTIM_Config();
	
  /*##-3- Configure DAC channel1 and channel2 #############################################*/
  sConfig.DAC_Trigger = DAC_TRIGGER_HR1_TRGO1;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;


  if (HAL_DAC_ConfigChannel(&DacHandle, &sConfig, DAC_CHANNEL_1) != HAL_OK)
  {
    /* Channel configuration Error */
    Error_Handler();
  }
	
  if (HAL_DAC_ConfigChannel(&DacHandle, &sConfig, DAC_CHANNEL_2) != HAL_OK)
  {
    /* Channel configuration Error */
    Error_Handler();
  } 
	
  /*##-4- Enable DAC Channel1 and channel 2 ################################################*/
  if (HAL_DAC_Start(&DacHandle, DAC_CHANNEL_1) != HAL_OK)
  {
    /* Start Error */
    Error_Handler();
  }
  if (HAL_DAC_Start(&DacHandle, DAC_CHANNEL_2) != HAL_OK)
  {
    /* Start Error */
    Error_Handler();
  }
	
  /*##-5- Set DAC Both Channels DHR register ######################################*/
  if (HAL_DACEx_DualSetValue(&DacHandle, DAC_ALIGN_12B_R, 0xFF, 0xFF) != HAL_OK)
  {
    /* Setting value Error */
    Error_Handler();
  }
	
  /* Infinite loop */
  while (1)
  {	
		/* If the Key is pressed */
    if (ubKeyPressed != RESET)
    {
			/* select waves forms according to the Wkup/Tamper push-button status */
      if (ubSelectedWavesForm == 1)
      {
        /* Noise wave generation */
        if (HAL_DACEx_NoiseWaveGenerate(&DacHandle,DAC_CHANNEL_1,DAC_LFSRUNMASK_BITS10_0) != HAL_OK)
        {
          Error_Handler();
	}
        if (HAL_DACEx_NoiseWaveGenerate(&DacHandle,DAC_CHANNEL_2,DAC_LFSRUNMASK_BITS10_0) != HAL_OK)
        {
          Error_Handler();
        } 				
      }
      else
      {	
        /* Triangle wave generation */
        if (HAL_DACEx_TriangleWaveGenerate(&DacHandle,DAC_CHANNEL_1,DAC_TRIANGLEAMPLITUDE_1023) != HAL_OK)
        {
          Error_Handler();
        }	
        if (HAL_DACEx_TriangleWaveGenerate(&DacHandle,DAC_CHANNEL_2,DAC_TRIANGLEAMPLITUDE_1023) != HAL_OK)
        {
          Error_Handler();
        }	

      }
      ubKeyPressed = RESET;
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
  RCC_OscInitStruct.PLL.PLLN = 400;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLQ = 4;

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
  * @brief  EXTI line detection callbacks.
  * @param  GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  /* Change the wave */
  ubKeyPressed = 1;
  /* Change the selected waves forms */
  ubSelectedWavesForm = !ubSelectedWavesForm;
}
/**
  * @brief  HRTIM Configuration
  * @param  None
  * @retval None
  */
void HRTIM_Config(void)
{
  HRTIM_HandleTypeDef hhrtim;
  HRTIM_TimeBaseCfgTypeDef time_base_config_master;
  HRTIM_TimerCfgTypeDef timer_config_master;
	/*##-1- Configure the HRTIM peripheral #######################################*/
  
	/* HRTIM Initialization */
	hhrtim.Instance = (HRTIM_TypeDef *)NULL;
	
	hhrtim.hdmaMaster = (DMA_HandleTypeDef *)NULL;    
	hhrtim.hdmaTimerA = (DMA_HandleTypeDef *)NULL;     
	hhrtim.hdmaTimerB = (DMA_HandleTypeDef *)NULL;  
	hhrtim.hdmaTimerC = (DMA_HandleTypeDef *)NULL;  
	hhrtim.hdmaTimerD = (DMA_HandleTypeDef *)NULL;  
	hhrtim.hdmaTimerE = (DMA_HandleTypeDef *)NULL;
		
	hhrtim.Instance = HRTIM1;
	hhrtim.Init.HRTIMInterruptResquests = HRTIM_IT_NONE;
	HAL_HRTIM_Init(&hhrtim);

	/* Time base master configuration */
	time_base_config_master.Mode = HRTIM_MODE_CONTINUOUS;
	time_base_config_master.Period = 0xFFFF;
	time_base_config_master.PrescalerRatio = HRTIM_PRESCALERRATIO_DIV1;
	time_base_config_master.RepetitionCounter = 0;
	
	HAL_HRTIM_TimeBaseConfig(&hhrtim,HRTIM_TIMERINDEX_MASTER,&time_base_config_master);
													 

	/* Master Time configuration */
	timer_config_master.InterruptRequests = HRTIM_MASTER_IT_MUPD;
	timer_config_master.DMARequests = HRTIM_TIM_DMA_NONE;
	timer_config_master.HalfModeEnable = HRTIM_HALFMODE_DISABLED;
	timer_config_master.StartOnSync = HRTIM_SYNCSTART_DISABLED;
	timer_config_master.ResetOnSync = HRTIM_SYNCRESET_DISABLED;
        timer_config_master.DACSynchro = HRTIM_DACSYNC_DACTRIGOUT_1;
	timer_config_master.PreloadEnable = HRTIM_PRELOAD_ENABLED;
	timer_config_master.UpdateGating = HRTIM_UPDATEGATING_INDEPENDENT;
	timer_config_master.BurstMode = HRTIM_TIMERBURSTMODE_MAINTAINCLOCK;
	timer_config_master.RepetitionUpdate = HRTIM_UPDATEONREPETITION_ENABLED;
	timer_config_master.PushPull = HRTIM_TIMPUSHPULLMODE_DISABLED;
	timer_config_master.FaultEnable = HRTIM_TIMFAULTENABLE_NONE;
	timer_config_master.FaultLock = HRTIM_TIMFAULTLOCK_READWRITE;
	timer_config_master.DeadTimeInsertion = HRTIM_TIMDEADTIMEINSERTION_DISABLED;
	timer_config_master.DelayedProtectionMode = HRTIM_TIMER_A_B_C_DELAYEDPROTECTION_DISABLED;
	timer_config_master.UpdateTrigger= HRTIM_TIMUPDATETRIGGER_NONE;
	timer_config_master.ResetTrigger = HRTIM_TIMRESETTRIGGER_NONE;
	timer_config_master.ResetUpdate = HRTIM_TIMUPDATEONRESET_DISABLED;
	
	HAL_HRTIM_WaveformTimerConfig(&hhrtim,HRTIM_TIMERINDEX_MASTER,&timer_config_master);
	

  /*##-2- Enable HRTIM peripheral counter ######################################*/
  HAL_HRTIM_WaveformCounterStart(&hhrtim, HRTIM_TIMERID_MASTER);
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
  while(1)
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

