/**
  ******************************************************************************
  * @file    PSSI_Transmit_Receive_DMA/Src/main.c 
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
#include "string.h"
#include "stdio.h"

/** @addtogroup STM32H7xx_HAL_Examples
  * @{
  */

/** @addtogroup PSSI_Transmit_Receive_DMA
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
PSSI_HandleTypeDef   PSSI_Handle;
TIM_HandleTypeDef    htim;
TIM_OC_InitTypeDef   sConfig; /* Timer Output Compare Configuration Structure declaration */

/* Private define ------------------------------------------------------------*/
#define MASTER_BOARD
#define  TIMx                TIM1
#define  PERIOD_VALUE       (uint32_t)(1000 - 1)              /* Period Value             */
#define  PULSE1_VALUE       (uint32_t)(PERIOD_VALUE/2)        /* Capture Compare 1 Value  */


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Counter Prescaler value */
uint32_t uhPrescalerValue = 0;
uint32_t Request_received = 0 ;
uint32_t data_cmp = 0 ;
uint32_t i;
__IO uint32_t PSSI_HAL_PSSI_TransmitComplete_count = 0;
__IO uint32_t PSSI_HAL_PSSI_ReceiveComplete_count = 0;
__IO uint32_t PSSI_HAL_PSSI_ErrorCallback_count = 0;

#ifndef MASTER_BOARD
ALIGN_32BYTES (char     pData8_S_TRSMT[64] ="Hello from Slave");  /* Data to transmit from Slave */
ALIGN_32BYTES (char     pData8_S_RCV[64]); 
#else
ALIGN_32BYTES (char     pData8_M_RCV[64]);  
ALIGN_32BYTES (char     pData8_M_TRSMT[64]); /* Data to transmit from Master */
#endif

/* Private function prototypes -----------------------------------------------*/
static void MPU_Config(void);
static void SystemClock_Config(void);
static void Error_Handler(void);
static void CPU_CACHE_Enable(void);

#ifdef MASTER_BOARD
static void Timer_Config(TIM_HandleTypeDef *htim);
static void CLK_On(void);
static void CLK_Off(void);
static uint32_t Fetch_Slave_Request(char Received_Buffer[64]);
#endif /*MASTER_BOARD*/

/* Private functions ---------------------------------------------------------*/
void HAL_PSSI_TxCpltCallback(PSSI_HandleTypeDef *hpssi);
void HAL_PSSI_RxCpltCallback(PSSI_HandleTypeDef *hpssi);
void HAL_PSSI_ErrorCallback(PSSI_HandleTypeDef *hpssi);


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
  
  HAL_Init();
  
  /* Configure the system clock to 520 MHz */
  SystemClock_Config();
  
  /* Initialize LEDs */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);
  BSP_LED_Init(LED3);
  
  /* Common Configuration for PSSI*/
  PSSI_Handle.Instance = PSSI;
  PSSI_Handle.Init.BusWidth = HAL_PSSI_8LINES;
  PSSI_Handle.Init.DataWidth = HAL_PSSI_32BITS;
  PSSI_Handle.Init.ControlSignal = HAL_PSSI_DE_ENABLE;
  
  if(HAL_PSSI_Init(&PSSI_Handle) != HAL_OK)
  {
    Error_Handler();
  }
  
#ifdef MASTER_BOARD  
  /* Configure Tim to generate the clock for PSSI slave */
  Timer_Config(&htim) ;
  
  while(1)
  {
    CLK_On();
    
    /* Turn LED 2 ON: Receive is ongoing */
    BSP_LED_On(LED2);
    
    while(Request_received != 1)
    {
     
      if(HAL_PSSI_Receive_DMA(&PSSI_Handle, (uint32_t*)pData8_M_RCV , sizeof(pData8_M_RCV)/4)!= HAL_OK)
      {
        Error_Handler();
      }
      
      while(PSSI_HAL_PSSI_ReceiveComplete_count !=1)
      {
        /* wait until receiving data is complete */
      }
      
      if(Fetch_Slave_Request(pData8_M_RCV) == 1)
      {
        Request_received = 1;
      }
      
      /*
      CPU Data Cache maintenance :
      It is recommended to clean the CPU Data cache after the DMA transfer.
      As the destination buffer may be used by the CPU, this guarantees Up-to-date data when CPU access
      to the destination buffer located in the D1 AXI-SRAM (which is cacheable).
      */
      SCB_CleanDCache_by_Addr((uint32_t*)pData8_M_TRSMT,sizeof(pData8_M_TRSMT));
      
      HAL_Delay(10);    
      
      PSSI_HAL_PSSI_ReceiveComplete_count = 0 ;
    }
    
    /* Start transmitting the received & modified data */
    if(HAL_PSSI_Transmit_DMA(&PSSI_Handle, (uint32_t*)pData8_M_TRSMT, sizeof(pData8_M_TRSMT)/4)!= HAL_OK)
    {
      Error_Handler();
    }
    
    HAL_Delay(2);
    
    while(PSSI_HAL_PSSI_TransmitComplete_count != 1)
    {
      /* wait until transmit data is complete */
    }

    BSP_LED_Off(LED2) ;

     /* Turn LED 1 ON: Slave request was received and transmit is done */
    BSP_LED_On(LED1) ;
    
    HAL_Delay(20);
    
    /* Transmit done */
    Request_received = 0 ;
    
    PSSI_HAL_PSSI_TransmitComplete_count = 0 ;
    
    CLK_Off();
    
    /*Wait 1s before the next loop*/
    HAL_Delay(1000);
    
    BSP_LED_Off(LED1) ;
  }
#else 
  /*PSSI slave Part*/

  /* Start transmitting the data */
  if(HAL_PSSI_Transmit_DMA(&PSSI_Handle, (uint32_t*)(pData8_S_TRSMT), sizeof(pData8_S_TRSMT)/4)!= HAL_OK)
  {
    Error_Handler();
  }
  
  while(PSSI_HAL_PSSI_TransmitComplete_count != 1)
  {
    /* wait until transmit data is complete */
  }
  
  /* Turn LED 2 ON: transmit is done and receive is ongoing*/
  BSP_LED_On(LED2) ;
  
  HAL_Delay(2);
  /* Start receiving the data */
  if(HAL_PSSI_Receive_DMA(&PSSI_Handle, (uint32_t*)(pData8_S_RCV), sizeof(pData8_S_RCV)/4)!= HAL_OK)
  {
    Error_Handler();
  }
  
  while(PSSI_HAL_PSSI_ReceiveComplete_count != 1)
  {
    /* wait until receiving data is complete */
  }
    
  BSP_LED_Off(LED2) ;
  
  /*********************************************************************************************/  
  
  /* Check first that the transmitted data is received */
  for(i = 0; i < strlen(pData8_S_TRSMT) ;i++)
  {
    if(pData8_S_TRSMT[i] != pData8_S_RCV[i])
    {
      data_cmp++;
	  break;
    }
  }
  
  /*if OK , check then, that the Master has modified the transmitted data*/
  if((data_cmp == 0) && (strstr(pData8_S_RCV," Master") != 0))
  { /* Infinite loop */
    while (1)
    {
      /* Turn LED 1 ON: Master response is received correctly */
      BSP_LED_On(LED1) ;
    }
  }
  else /*KO*/
  {
    Error_Handler();
  }

#endif /*MASTER_BOARD*/
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE BYPASS)
  *            SYSCLK(Hz)                     = 520000000 (CPU Clock)
  *            HCLK(Hz)                       = 260000000 (AXI and AHBs Clock)
  *            AHB Prescaler                  = 2
  *            D1 APB3 Prescaler              = 2 (APB3 Clock  130MHz)
  *            D2 APB1 Prescaler              = 2 (APB1 Clock  130MHz)
  *            D2 APB2 Prescaler              = 2 (APB2 Clock  130MHz)
  *            D3 APB4 Prescaler              = 2 (APB4 Clock  130MHz)
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 4
  *            PLL_N                          = 260
  *            PLL_P                          = 1
  *            PLL_Q                          = 4
  *            PLL_R                          = 2
  *            VDD(V)                         = 3.3
  *            Flash Latency(WS)              = 3
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  HAL_StatusTypeDef ret = HAL_OK;

  /* The voltage scaling allows optimizing the power consumption when the device is
     clocked below the maximum system frequency, to update the voltage scaling value
     regarding system frequency refer to product datasheet.  */
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
  RCC_OscInitStruct.PLL.PLLN = 260;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  RCC_OscInitStruct.PLL.PLLP = 1;
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
  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3);
  if(ret != HAL_OK)
  {
    Error_Handler();
  }
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
    /* Turn LED 3 ON : PSSI Config Error, Transfer Error
       or data received  by PSSI slave is not compliant to the sent one */
    BSP_LED_On(LED3) ;
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


#ifdef MASTER_BOARD
void Timer_Config(TIM_HandleTypeDef *htim)
{
    /* Compute the prescaler value to have TIM1 counter clock equal to 20000 Hz */
  uhPrescalerValue = (uint32_t)(250000000 / (20000000)) - 1;
  
  htim->Instance = TIMx;

  htim->Init.Prescaler         = uhPrescalerValue;
  htim->Init.Period            = PERIOD_VALUE;
  htim->Init.ClockDivision     = 0;
  htim->Init.CounterMode       = TIM_COUNTERMODE_UP;
  htim->Init.RepetitionCounter = 0;
  
  if (HAL_TIM_PWM_Init(htim) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /*##-2- Configure the PWM channel #########################################*/
  /* configuration for the channel */
  sConfig.OCMode       = TIM_OCMODE_PWM1;
  sConfig.OCPolarity   = TIM_OCPOLARITY_HIGH;
  sConfig.OCFastMode   = TIM_OCFAST_DISABLE;
  sConfig.OCNPolarity  = TIM_OCNPOLARITY_HIGH;
  sConfig.OCNIdleState = TIM_OCNIDLESTATE_RESET;

  sConfig.OCIdleState  = TIM_OCIDLESTATE_RESET;

  /* Set the pulse value for channel 1 */
  sConfig.Pulse = PULSE1_VALUE;
  
  if (HAL_TIM_PWM_ConfigChannel(htim, &sConfig, TIM_CHANNEL_1) != HAL_OK)
  {
    /* Configuration Error */
    Error_Handler();
  }

}

static void CLK_On(void)
{
  /*Start PWM signals generation #######################################*/
  /* Start channel 1 */
  if (HAL_TIM_PWM_Start(&htim, TIM_CHANNEL_1) != HAL_OK)
  {
    /* PWM Generation Error */
    Error_Handler();
  }
}

static void CLK_Off(void)
{
  if (HAL_TIM_PWM_Stop(&htim, TIM_CHANNEL_1) != HAL_OK)
  {
    /* PWM Generation Error */
    Error_Handler();
  }
}

uint32_t Fetch_Slave_Request(char Received_Buffer[64])
{
  uint32_t j;
  char REPLY[23] = ": received, from Master" ;
  
  if(strstr(Received_Buffer,"Slave") != 0)
  {
    for(i=0 ; i < strlen(Received_Buffer) ; i++)
    {
      pData8_M_TRSMT[i] = Received_Buffer[i];
    }
    
    for(j=0;j<strlen(REPLY);j++)
    {
      pData8_M_TRSMT[i] = REPLY[j];
      i++;
    }
    return 1;
  }
  else 
  {
    return 0;
  }
}

#endif /* MASTER_BOARD */


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
void assert_failed(uint8_t* file, uint32_t line)
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
  * @brief PSSI transfer complete callback
  * @par hppsi: PSSI HAL handle
  * @retval None
  */
void HAL_PSSI_TxCpltCallback(PSSI_HandleTypeDef *hpssi)
{
  PSSI_HAL_PSSI_TransmitComplete_count++;
}
/**
  * @brief PSSI receive complete callback
  * @par hppsi: PSSI HAL handle
  * @retval None
  */
 void HAL_PSSI_RxCpltCallback(PSSI_HandleTypeDef *hpssi)
{
  PSSI_HAL_PSSI_ReceiveComplete_count++;
}

/**
  * @brief PSSI transfer error callback
  * @par hppsi: PSSI HAL handle
  * @retval None
  */
void HAL_PSSI_ErrorCallback(PSSI_HandleTypeDef *hpssi)
{
  PSSI_HAL_PSSI_ErrorCallback_count++;
}
/**
  * @}
  */

/**
  * @}
  */

