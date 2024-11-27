/**
  ******************************************************************************
  * @file    OPAMP/OPAMP_PGA_ExternalBias/Src/main.c
  * @author  MCD Application Team
  * @brief   This example provides a short description of how to use the OPAMP
  *          peripheral in Follower mode.
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

/** @addtogroup OPAMP_PGA_ExternalBias
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
OPAMP_HandleTypeDef  OpampHandle;
DAC_HandleTypeDef    DacHandle;

__IO uint32_t UserButtonStatus = 0;  /* set to 1 after Wkup/Tamper push-button interrupt  */
static DAC_ChannelConfTypeDef sConfig;
/* Private function prototypes -----------------------------------------------*/
static void MPU_Config(void);
void DAC_Config(void);
void OPAMP_Config_Power(uint32_t powermode);
static void CPU_CACHE_Enable(void);
static void SystemClock_Config(void);
void Error_Handler(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
  uint32_t examplestate = 0; 
    
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
  
  /* Configure the system clock  */
  SystemClock_Config();

  /* Configure LEDs */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);
	BSP_LED_Init(LED3);
  /* Initialize the Wkup/User push-button.
     It is used for changing the gain */
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);

  /* Configure the DAC to set input voltage of OPAMP */
	DAC_Config();
	
  /* Configure the OPAMP1 in PGA mode */

  while (1)
  {
    /* Test if user an action occurred on the Wkup/User push-button */
    if (UserButtonStatus == 1)
    {
      UserButtonStatus = 0;
      /* Change the mode */
      if (examplestate == 0)
      {
				/* Turn LED1 on */
        BSP_LED_On(LED1);
				/* Turn LED2 off */
        BSP_LED_Off(LED2);
        /* Select gain 2 */
        OPAMP_Config_Power(OPAMP_PGA_GAIN_2_OR_MINUS_1);
        examplestate= 1;
		
      }
      else 
      {
				/* Turn LED2 on */
        BSP_LED_On(LED2);
				/* Turn LED1 off */
        BSP_LED_Off(LED1);
        /* Select gain 4 */
        OPAMP_Config_Power(OPAMP_PGA_GAIN_4_OR_MINUS_3);
        examplestate= 0;
		
      }
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
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* Turn LED3 on */
  BSP_LED_On(LED3);

  while (1)
  {
  }
}


void DAC_Config(void)
{
  /* Configure the DAC peripheral instance */
  DacHandle.Instance = DAC1;

  /*##-1- Initialize the DAC peripheral ######################################*/
  if (HAL_DAC_Init(&DacHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /*##-2- DAC channel Configuration ##########################################*/
  /* Select normal power mode */
  sConfig.DAC_SampleAndHold = DAC_SAMPLEANDHOLD_DISABLE;
  /* Select trigger none */
  sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
  /* Select DAC channel output buffer disabled */
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_DISABLE;
  /* Connect DAC output to on chip peripheral */
  sConfig.DAC_ConnectOnChipPeripheral = DAC_CHIPCONNECT_ENABLE;
  /* Select factory trimming */
  sConfig.DAC_UserTrimming = DAC_TRIMMING_FACTORY;
	
  if(HAL_DAC_ConfigChannel(&DacHandle, &sConfig, DAC_CHANNEL_1) != HAL_OK)
  {
    /* Channel configuration Error */
    Error_Handler();
  }
  /* Set DAC output voltage */
	HAL_DAC_SetValue(&DacHandle, DAC_CHANNEL_1, DAC_ALIGN_8B_R,0x26);
	
	/*##-3- Enable DAC Channel 1 ##########################################*/
  if(HAL_DAC_Start(&DacHandle, DAC_CHANNEL_1) != HAL_OK)
  {
   
    Error_Handler();
  }
}
/**             
  * @brief  OPAMP Configuration 
  * @param  None
  * @retval None
  */
void OPAMP_Config_Power(uint32_t pgagain)
{
  /* Set OPAMP instance */
  OpampHandle.Instance = OPAMP1; 

  /*##-1- Deinit  OPAMP    ##################################################*/
  HAL_OPAMP_DeInit(&OpampHandle);

  /*##-2- Configure OPAMP    ##################################################*/
  
  /* Select power mode */  
  OpampHandle.Init.PowerMode = OPAMP_POWERMODE_NORMAL; 
 
  /* Select PGA Mode */
  OpampHandle.Init.Mode = OPAMP_PGA_MODE; 
  
  /* Select VMx as bias voltage */
  OpampHandle.Init.PgaConnect = OPAMP_PGA_CONNECT_INVERTINGINPUT_IO0_BIAS;	
  
  /* Configure the gain */
  if (pgagain == OPAMP_PGA_GAIN_2_OR_MINUS_1)
  {
     OpampHandle.Init.PgaGain = OPAMP_PGA_GAIN_2_OR_MINUS_1;
	}
	else
	{
		OpampHandle.Init.PgaGain = OPAMP_PGA_GAIN_4_OR_MINUS_3;
	}		
  /* Select VPx as input for OPAMP */
  OpampHandle.Init.NonInvertingInput = OPAMP_NONINVERTINGINPUT_DAC_CH;
   
  /* Select the factory trimming */
  OpampHandle.Init.UserTrimming = OPAMP_TRIMMING_FACTORY;

  /* Init */
  if(HAL_OK != HAL_OPAMP_Init(&OpampHandle))
  {
    Error_Handler();
  }

  /*##-3- Start OPAMP    #####################################################*/
  /* Enable OPAMP */
  if(HAL_OK != HAL_OPAMP_Start(&OpampHandle))
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
  if (GPIO_Pin == BUTTON_USER_PIN)
  {
    UserButtonStatus = 1;
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

