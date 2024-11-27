/**
  ******************************************************************************
  * @file    CRC/CRC_Data_Reversing_16bit_CRC/Src/main.c
  * @author  MCD Application Team
  * @brief   This sample code shows how to use the STM32H7xx CRC HAL API
  *          to compute a 16-bit CRC code from a data word (32-bit data) buffer,
  *          based on a user-defined generating polynomial and initialization 
  *          value, with input and output data reversing features enabled.
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

/** @addtogroup CRC_Data_Reversing_16bit_CRC
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define BUFFER_SIZE  2  /* Input buffer CRC16_DATA32[] is 2-word (8-byte) long */ 

/* User-defined polynomial */
#define CRC_POLYNOMIAL_16B  0x1021  /* X^16 + X^12 + X^5 + 1, CRC-CCITT generating polynomial */

/* User-defined CRC init value */
/* As the CRC is 16-bit long, the init value is 16-bit long as well */
#define CRC_INIT_VALUE      0x5ABE

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* CRC handler declaration */
CRC_HandleTypeDef   CrcHandle;

/* Used for storing CRC Value */
__IO uint32_t uwCRCValue = 0;

/* Input data buffer */
static const uint32_t CRC16_DATA32[BUFFER_SIZE] = {0x1A2B3C4D, 0x8AC971BE};
                                                 
/* Check input data buffer, used to verify data reversal options */
static const uint32_t CRC16_DATA32_CHECK[BUFFER_SIZE] = {0xB23CD458, 0x7D8E9351};

/* Expected CRC value yielded by CRC16_DATA32[] input buffer 
   with output data reversal feature disabled (default)
   The 16 LSB bits are the 16-bit long CRC. */
uint32_t uwExpectedCRCValue = 0x00004C37;
/* Expected CRC value yielded by CRC16_DATA32[] input buffer 
   with output data reversal feature enabled.
   The 16 LSB bits are the 16-bit long CRC. */
uint32_t uwExpectedCRCValue_reversed;

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
  
  /* Configure the system clock to 520 MHz */
  SystemClock_Config();

  /* Configure LED2 and LED1 */
  BSP_LED_Init(LED2);
  BSP_LED_Init(LED1);  


  /****************************************************************************/
  /*                                                                          */
  /*                     CRC peripheral initialization                        */
  /*                                                                          */    
  /****************************************************************************/
    
  CrcHandle.Instance = CRC;

  /* The default polynomial is not used. The one to be used must be defined 
     in CrcHandle.Init.GeneratingPolynomial */  
  CrcHandle.Init.DefaultPolynomialUse    = DEFAULT_POLYNOMIAL_DISABLE;
  
  /* Set the value of the generating polynomial. 
    The one used in that example is the 16-bit long CRC generating
    polynomial X^16 + X^12 + X^5 + 1 */
  CrcHandle.Init.GeneratingPolynomial    = CRC_POLYNOMIAL_16B;
  
  /* The user-defined generating polynomial yields a 16-bit long CRC */
  CrcHandle.Init.CRCLength               = CRC_POLYLENGTH_16B;

  /* The default init value is not used */
  CrcHandle.Init.DefaultInitValueUse     = DEFAULT_INIT_VALUE_DISABLE;
  
  /* User init value is used instead */
  CrcHandle.Init.InitValue               = CRC_INIT_VALUE;

  /* The input data are inverted by word */
  CrcHandle.Init.InputDataInversionMode  = CRC_INPUTDATA_INVERSION_WORD;
  /* The bit reversal is done on a full word basis.
     
     The first word written in the CRC DR register is
      0x4D3C2B1A.
     
     Bit reversal done by the hardware on the full word leads to the actual 
     CRC processing of
      0xB23CD458.
     
     Similarly, the second word written in the peripheral is 0x8AC971BE, leading to
     the processing of 0x7D8E9351 after hardware input data reversal.
     
     
     This means that the field InputDataInversionMode set to CRC_INPUTDATA_INVERSION_WORD
     applied to {0x4D3C2B1A, 0x8AC971BE}
     yields the same result as InputDataInversionMode set to CRC_INPUTDATA_INVERSION_NONE
     applied to {0xB23CD458, 0x7D8E9351}.
     
  */       
  
  /* The output data are inverted */
  CrcHandle.Init.OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_ENABLE;
  /* Output data reversal can only be done at bit level. 
     The expected CRC is 0xEC32 after output data reversal, meaning 
     that the CRC before the reversal operation is 0x4C37. */

  /* The input data are words (32-bit long data) */
  CrcHandle.InputDataFormat = CRC_INPUTDATA_FORMAT_WORDS;

  /* De-initialize the CRC peripheral */
  if (HAL_CRC_DeInit(&CrcHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }  

  /* Then, initialize the CRC handle */
  if (HAL_CRC_Init(&CrcHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }


  /****************************************************************************/
  /*                                                                          */
  /*                      CRC computation                                     */
  /*                                                                          */    
  /****************************************************************************/

  /* The 16-bit long CRC of the 2-word buffer is computed. After peripheral initialization, 
     the CRC calculator is initialized with the user-defined value that is 0x5ABE.
    
    The computed CRC is stored in uint32_t uwCRCValue. The 16-bit long CRC is made of
    uwCRCValue 16 LSB bits. */

  uwCRCValue = HAL_CRC_Accumulate(&CrcHandle, (uint32_t *)&CRC16_DATA32, BUFFER_SIZE);

  /* Compare the CRC value to the expected one (reversed) */
  uwExpectedCRCValue_reversed = (__RBIT(uwExpectedCRCValue) >> 16);
  if (uwCRCValue != uwExpectedCRCValue_reversed)
  {
    /* Wrong CRC value: Turn LED2 on */  
    Error_Handler();
  }
             
  
  
  /****************************************************************************/
  /*                                                                          */
  /*         CRC computation with reversal options disabled                   */
  /*                                                                          */    
  /****************************************************************************/

  /* Next, the input and output data inversion features are disabled and
     it is verified that the input data CRC16_DATA32_CHECK[] which is defined with 
     a different endianness scheme yields the same CRC.
     
     As explained above, CRC16_DATA32_CHECK is obtained from CRC16_DATA32
     by a bit-reversal operation carried out on full word and vice versa. */
     
    
  /* Reversal options are disabled */  
  CrcHandle.Init.InputDataInversionMode  = CRC_INPUTDATA_INVERSION_NONE; 
  CrcHandle.Init.OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_DISABLE; 
  
   /* Peripheral re-initialization. The CRC calculator is initialized again with 
     the user-defined value that is 0x5ABE. */
  if (HAL_CRC_Init(&CrcHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }


  /*  The newly computed CRC is stored in uint32_t uwCRCValue. */
  uwCRCValue = HAL_CRC_Accumulate(&CrcHandle, (uint32_t *)&CRC16_DATA32_CHECK, BUFFER_SIZE);


  /* Compare the new CRC value to the expected one, taking into account the
     output data reversal feature is disabled. */
  if (uwCRCValue != uwExpectedCRCValue)
  {
    /* Wrong CRC value: Turn LED2 on */  
    Error_Handler();
  }
  else
  {
    /* Right CRC value: Turn LED1 on */
    BSP_LED_On(LED1);    
  }                                           


  /* Infinite loop */
  while (1)
  {
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 520000000 (CPU Clock)
  *            HCLK(Hz)                       = 260000000 (AXI and AHBs Clock)
  *            AHB Prescaler                  = 2
  *            D1 APB3 Prescaler              = 2 (APB3 Clock  130MHz)
  *            D2 APB1 Prescaler              = 2 (APB1 Clock  130MHz)
  *            D2 APB2 Prescaler              = 2 (APB2 Clock  130MHz)
  *            D3 APB4 Prescaler              = 2 (APB4 Clock  130MHz)
  *            HSE Frequency(Hz)              = 25000000
  *            PLL_M                          = 5
  *            PLL_N                          = 104
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
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
  RCC_OscInitStruct.CSIState = RCC_CSI_OFF;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;

  RCC_OscInitStruct.PLL.PLLM = 5;
  RCC_OscInitStruct.PLL.PLLN = 104;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  RCC_OscInitStruct.PLL.PLLP = 1;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLQ = 4;

  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
  if(ret != HAL_OK)
  {
    while(1) {};
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
    while(1) {};
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
  while (1)
  {
    /* Error if LED2 is slowly blinking (1 sec. period) */
    BSP_LED_Toggle(LED2); 
    HAL_Delay(1000);   
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
  * @}
  */

/**
  * @}
  */

