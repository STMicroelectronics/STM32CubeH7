/**
  ******************************************************************************
  * @file    CRYP/CRYP_AESCCM_IT/Src/main.c
  * @author  MCD Application Team
  * @brief   This sample code shows how to use the STM32H7xx CRYP HAL API
  *          to encrypt/decrypt data and get TAG using AES CCM Algorithm.
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

/** @addtogroup CRYP_Example
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
CRYP_HandleTypeDef hcryp; /* CRYP handler declaration */

__IO uint32_t CrypCompleteDetected = 0; /* CRYP complete detection */

/* AES Key  */
uint32_t AES192Key[6] =   {0xF9677EA3,0x01BAA45D,0x41A33918,0x81D8CDB1, 0x296E6C88, 0x09114C80,};

/* B0 block */
uint32_t BlockB0[4] = {0x7B2F50A1,0xBA5F9AA4,0xC477A82B,0x00000010};

/* B1 block*/
uint32_t BlockB1[4] = {0x000E7578,0x561AD597,0xBD0EAF3F,0xE77DF111};

/* Plaintext */
uint32_t Plaintext[4] = {0xEC5D99F5,0x6EE67628,0x5DF80935,0x0726067E};

/* Expected CCM Ciphertext with AESKey192 */
uint32_t Ciphertext[4] = {0xAD8B6E0D,0x08040B0B,0xCDAC71EE,0xDCD486AF};

/* Expected CCM TAG */
uint32_t ExpectedTAG[4] = {0xEF8DFABB,0x29F6FE68,0xD70AA868,0x26BB30DB};


/* Used for storing the encrypted text */
uint32_t Encryptedtext[4]={0}; 

/* Used for storing the decrypted text */
uint32_t Decryptedtext[4]={0};

/* Used for storing the computed MAC (TAG) */
uint32_t TAG[4]={0};

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

  /* Configure the system clock to 280 MHz */
  SystemClock_Config();

  /* Configure LED1 and LED3 */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED3);
   
  /*## Initialize the CRYP IP  ###############################################*/ 
  
  /* Set the CRYP parameters */
  hcryp.Instance        = CRYP;
  hcryp.Init.DataType   = CRYP_DATATYPE_32B;
  hcryp.Init.KeySize    = CRYP_KEYSIZE_192B;
  hcryp.Init.pKey       = AES192Key; 
  hcryp.Init.Algorithm  = CRYP_AES_CCM;
  hcryp.Init.Header     = BlockB1;
  hcryp.Init.HeaderSize = 4;
  hcryp.Init.B0         = BlockB0; 
  
  /* Initialize CRYP */ 
  HAL_CRYP_Init(&hcryp);
  
  /*##-1- AES CCM Encryption & TAG generation in interrupt mode ##############*/ 
  
  /* Encryption, result in  Encryptedtext buffer */ 
  HAL_CRYP_Encrypt_IT(&hcryp, Plaintext, 4, Encryptedtext);
  
  /*Wait until output transfer complete*/
  while(CrypCompleteDetected == 0) 
  {  }  
  
  /* Reset Output Transfer Complete Detect */
  CrypCompleteDetected = 0;  
  
  /*Compare results with expected buffer*/
  if(memcmp(Encryptedtext, Ciphertext, 16) != 0)
  {
    /* Not expected result, wrong on Encryptedtext buffer: Turn LED3 on */
    Error_Handler();
  }
  
  /* Generate the authentication TAG */
  HAL_CRYPEx_AESCCM_GenerateAuthTAG(&hcryp,TAG, TIMEOUT_VALUE);
  
  /*Compare results with expected TAG buffer*/ 
  if(memcmp(TAG, ExpectedTAG, 16) != 0)
  {
    /* Not expected result, wrong on TAG buffer: Turn LED3 on */
    Error_Handler();
  }
  
  /*##-2- AES CCM Decryption & TAG generation in interrupt mode ################*/   
  
  /* Decryption, result in  Decryptedtext buffer */ 
  HAL_CRYP_Decrypt_IT(&hcryp,Ciphertext , 4, Decryptedtext);
  
  /*Wait until output transfer complete*/
  while(CrypCompleteDetected == 0) 
  {  }  
  
  /* Reset Output Transfer Complete Detect */
  CrypCompleteDetected = 0;  
  
  /*Compare results with expected buffer*/
  if(memcmp(Decryptedtext, Plaintext, 16) != 0)
  {
    /* Not expected result, wrong on DecryptedText buffer: Turn LED3 on */
    Error_Handler();
  }
  
  /* Generate the authentication TAG */
  HAL_CRYPEx_AESCCM_GenerateAuthTAG(&hcryp,TAG, TIMEOUT_VALUE);
  
  /*Compare results with expected buffer*/ 
  if(memcmp(TAG, ExpectedTAG, 16) != 0)
  {
    /* Not expected result, wrong on TAG buffer: Turn LED3 on */
    Error_Handler();
  }
  else
  {
    /* Right Encryptedtext & Decryptedtext buffer : Turn LED1 on */
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
  * @brief  Output FIFO transfer completed callbacks.
  * @param  hcryp: pointer to a CRYP_HandleTypeDef structure that contains
  *         the configuration information for CRYP module
  * @retval 1 if output FIFO transfer completed
  */
 void HAL_CRYP_OutCpltCallback(CRYP_HandleTypeDef *hcryp)
 {
     CrypCompleteDetected = 1;
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

