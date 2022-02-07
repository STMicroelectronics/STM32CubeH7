/**
  ******************************************************************************
  * @file    CRYP/CRYP_AESmodes_DMA/Src/main.c
  * @author  MCD Application Team
  * @brief   This sample code shows how to use the STM32H7xx CRYP HAL API
  *          to encrypt and decrypt data using AES in ECB Algorithm.
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

/** @addtogroup CRYP_Example
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* CRYP handler declaration */
CRYP_HandleTypeDef hcryp;
CRYP_ConfigTypeDef Conf;
__IO uint32_t CrypCompleteDetected = 0;

/* AES Key  */
uint32_t AESKey128[4] = {0x2B7E1516 ,0x28AED2A6 ,0xABF71588 ,0x09CF4F3C};
 
/* Plaintext */
uint32_t Plaintext[16]  =    {0x6BC1BEE2 ,0x2E409F96 ,0xE93D7E11 ,0x7393172A ,
                              0xAE2D8A57 ,0x1E03AC9C ,0x9EB76FAC ,0x45AF8E51 ,
                              0x30C81C46 ,0xA35CE411 ,0xE5FBC119 ,0x1A0A52EF ,
                              0xF69F2445 ,0xDF4F9B17 ,0xAD2B417B ,0xE66C3710};

/* Expected ECB Ciphertext with DataType 32 ( no swapping)*/
uint32_t Ciphertext[16] =    {0x3AD77BB4 ,0x0D7A3660 ,0xA89ECAF3 ,0x2466EF97 ,
                              0xF5D3D585 ,0x03B9699D ,0xE785895A ,0x96FDBAAF ,
                              0x43B1CD7F ,0x598ECE23 ,0x881B00E3 ,0xED030688 ,
                              0x7B0C785E ,0x27E8AD3F ,0x82232071 ,0x04725DD4};

/* Plaintext byte swapping (DataType 8)*/
uint32_t Plaintext_8[16] =   {0xE2BEC16B ,0x969F402E ,0x117E3DE9 ,0x2A179373 ,
                              0x578A2DAE ,0x9CAC031E ,0xAC6FB79E ,0x518EAF45 ,
                              0x461CC830 ,0x11E45CA3 ,0x19C1FBE5 ,0xEF520A1A ,
                              0x45249FF6 ,0x179B4FDF ,0x7B412BAD ,0x10376CE6};

/* Expected ECB Ciphertext with DataType 8 */
uint32_t Ciphertext_8[16] =  {0xB47BD73A ,0x60367A0D ,0xF3CA9EA8 ,0x97EF6624 ,
                              0x85D5D3F5 ,0x9D69B903 ,0x5A8985E7 ,0xAFBAFD96 ,
                              0x7FCDB143 ,0x23CE8E59 ,0xE3001B88 ,0x880603ED ,
                              0x5E780C7B ,0x3FADE827 ,0x71202382 ,0xD45D7204};

/* Plaintext  half-word swapping (DataType 16) */
uint32_t Plaintext_16[16] =  {0xBEE26BC1 ,0x9F962E40 ,0x7E11E93D ,0x172A7393 ,
                              0x8A57AE2D ,0xAC9C1E03 ,0x6FAC9EB7 ,0x8E5145AF ,
                              0x1C4630C8 ,0xE411A35C ,0xC119E5FB ,0x52EF1A0A ,
                              0x2445F69F ,0x9B17DF4F ,0x417BAD2B ,0x3710E66C};

/* Expected ECB Ciphertext with DataType 16 */
uint32_t Ciphertext_16[16] = {0x7BB43AD7 ,0x36600D7A ,0xCAF3A89E ,0xEF972466 ,
                              0xD585F5D3 ,0x699D03B9 ,0x895AE785 ,0xBAAF96FD ,
                              0xCD7F43B1 ,0xCE23598E ,0x00E3881B ,0x0688ED03 ,
                              0x785E7B0C ,0xAD3F27E8 ,0x20718223 ,0x5DD40472};
/* Plaintext  bit swapping (DataType 1) */
uint32_t Plaintext_1[16] =   {0x477D83D6 ,0x69F90274 ,0x887EBC97 ,0x54E8C9CE ,
                              0xEA51B475 ,0x3935C078 ,0x35F6ED79 ,0x8A71F5A2 ,
                              0x6238130C ,0x88273AC5 ,0x9883DFA7 ,0xF74A5058,
                              0xA224F96F ,0xE8D9F2FB ,0xDE82D4B5 ,0x08EC3667};

/* Expected ECB Ciphertext with DataType 1 */
uint32_t Ciphertext_1[16] =  {0x2DDEEB5C, 0x066C5EB0, 0xCF537915 ,0xE9F76624  ,
                              0xA1ABCBAF,  0xB9969DC0 ,0x5A91A1E7 ,0xF55DBF69 ,
                              0xFEB38DC2 , 0xC473719A ,0xC700D811 ,0x1160C0B7 ,
                              0x7A1E30DE , 0xFCB517E4 ,0x8E04C441 ,0x2BBA4E20};


/* Used for storing Encrypted text */
ALIGN_32BYTES (static uint32_t Encryptedtext[16])={0}; 

/* Used for storing Decrypted text */
ALIGN_32BYTES (static uint32_t Decryptedtext[16])={0}; 

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
  
  /* Configure LED1 and LED3 */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED3);
  
  /*## Initialize the CRYP IP  ###############################################*/ 
  /* Set the CRYP parameters */
  hcryp.Instance        = CRYP;
  hcryp.Init.DataType   = CRYP_DATATYPE_32B;
  hcryp.Init.KeySize    = CRYP_KEYSIZE_128B;
  hcryp.Init.pKey       = AESKey128; 
  hcryp.Init.Algorithm  = CRYP_AES_ECB; 
  
  /* Configure the CRYP  */
  HAL_CRYP_Init(&hcryp);
  
  /*##-1- AES ECB Encryption and Decryption in DMA mode with DataType 32 ######*/ 
  
  /* Encryption, result in  Encryptedtext buffer */ 
  HAL_CRYP_Encrypt_DMA(&hcryp, Plaintext, 16, Encryptedtext);
  
  /*Wait until output transfer complete*/
  while(CrypCompleteDetected == 0) 
  {  }
  
  /*Compare with expected result */
  if(memcmp(Encryptedtext, Ciphertext, 64) != 0)
  {
    /* Not expected result, wrong on Encryptedtext buffer: Turn LED3 on */
    Error_Handler();
  }
  
  /* Reset Output Transfer Complete Detect */
  CrypCompleteDetected = 0; 
  
  /* Decryption, result in  Decryptedtext buffer */ 
  HAL_CRYP_Decrypt_DMA(&hcryp, Ciphertext , 16, Decryptedtext);
  
  /*Wait until output transfer complete*/
  while(CrypCompleteDetected == 0) 
  {  }
  
  /*Compare with expected result */
  if(memcmp(Decryptedtext, Plaintext, 64) != 0)
  {
    /* Not expected result, wrong on Decryptedtext buffer: Turn LED3 on */
    Error_Handler();
  }
  /* Reset Output Transfer Complete Detect */
  CrypCompleteDetected = 0;  
  
  /*##-2- AES ECB Encryption and Decryption in DMA mode with DataType 8 ######*/ 
  
  /* Get the CRYP parameters */  
  HAL_CRYP_GetConfig(&hcryp, &Conf); 
  
  /* Set the CRYP parameters */
  Conf.DataType = CRYP_DATATYPE_8B;
  
  /* Configure the CRYP  */
  HAL_CRYP_SetConfig(&hcryp, &Conf);
  
  /* CPU Data Cache maintenance :
  It is recommended to clean the CPU Data cache before starting the  DMA transfer. 
  As the source buffer may be prepared by the CPU, this guarantees that the source buffer 
  (if located in the D1 AXI-SRAM which is cacheable) will be up to date before starting the transfer.
  */
  SCB_CleanDCache_by_Addr(Encryptedtext, 64); 
  
  /* Encryption, result in  Encryptedtext buffer */ 
  HAL_CRYP_Encrypt_DMA(&hcryp, Plaintext_8, 16, Encryptedtext);
  
  /*Wait until output transfer complete*/
  while(CrypCompleteDetected == 0) 
  {  }

  /* CPU Data Cache maintenance :  
    It is recommended to invalidate the CPU Data cache after the DMA transfer. 
    As the destination buffer may be used by the CPU, this guarantees Up-to-date data when CPU access 
    to the destination buffer located in the D1 AXI-SRAM (which is cacheable).
  */ 
  SCB_InvalidateDCache_by_Addr(Encryptedtext, 64);
  
  /*Compare with expected result */
  if(memcmp(Encryptedtext, Ciphertext_8, 64) != 0)
  {
    /* Not expected result, wrong on Encryptedtext buffer: Turn LED3 on */
    Error_Handler();
  }
  
  /* Reset Output Transfer Complete Detect */
  CrypCompleteDetected = 0; 
  
  /* CPU Data Cache maintenance :
    It is recommended to clean the CPU Data cache before starting the  DMA transfer. 
    As the source buffer may be prepared by the CPU, this guarantees that the source buffer 
    (if located in the D1 AXI-SRAM which is cacheable) will be up to date before starting the transfer.
  */
  SCB_CleanDCache_by_Addr(Decryptedtext, 64);
  
  /* Decryption, result in  Decryptedtext buffer */ 
  HAL_CRYP_Decrypt_DMA(&hcryp, Ciphertext_8 , 16, Decryptedtext);
  
  /*Wait until output transfer complete*/
  while(CrypCompleteDetected == 0) 
  {  }
  
  /* CPU Data Cache maintenance :  
    It is recommended to invalidate the CPU Data cache after the DMA transfer. 
    As the destination buffer may be used by the CPU, this guarantees Up-to-date data when CPU access 
    to the destination buffer located in the D1 AXI-SRAM (which is cacheable).
  */ 
  SCB_InvalidateDCache_by_Addr(Decryptedtext, 64);
  
  /*Compare with expected result */
  if(memcmp(Decryptedtext, Plaintext_8, 64) != 0)
  {
    /* Not expected result, wrong on Decryptedtext buffer: Turn LED3 on */
    Error_Handler();
  }
  /* Reset Output Transfer Complete Detect */
  CrypCompleteDetected = 0;  
  
  /*##-3- AES ECB Encryption and Decryption in DMA mode with DataType 16 ######*/ 
  
  /* Get the CRYP parameters */  
  HAL_CRYP_GetConfig(&hcryp, &Conf); 
  
  /* Set the CRYP parameters */
  Conf.DataType = CRYP_DATATYPE_16B;
  
  /* Configure the CRYP  */
  HAL_CRYP_SetConfig(&hcryp, &Conf);
  
  /* CPU Data Cache maintenance */
  SCB_CleanDCache_by_Addr(Encryptedtext, 64);
  
  /* Encryption, result in  Encryptedtext buffer */ 
  HAL_CRYP_Encrypt_DMA(&hcryp, Plaintext_16, 16, Encryptedtext);
  
  /*Wait until output transfer complete*/
  while(CrypCompleteDetected == 0) 
  {  }
  
  /* CPU Data Cache maintenance */ 
  SCB_InvalidateDCache_by_Addr(Encryptedtext, 64);
  
  /*Compare with expected result */
  if(memcmp(Encryptedtext, Ciphertext_16, 64) != 0)
  {
    /* Not expected result, wrong on Encryptedtext buffer: Turn LED3 on */
    Error_Handler();
  }
  
  /* Reset Output Transfer Complete Detect */
  CrypCompleteDetected = 0; 
  
  /* CPU Data Cache maintenance */
  SCB_CleanDCache_by_Addr(Decryptedtext, 64);
  
  /* Decryption, result in  Decryptedtext buffer */ 
  HAL_CRYP_Decrypt_DMA(&hcryp, Ciphertext_16 , 16, Decryptedtext);
  
  /*Wait until output transfer complete*/
  while(CrypCompleteDetected == 0) 
  {  }
  
  /* CPU Data Cache maintenance */ 
  SCB_InvalidateDCache_by_Addr(Decryptedtext, 64);
  
  /*Compare with expected result */
  if(memcmp(Decryptedtext, Plaintext_16, 64) != 0)
  {
    /* Not expected result, wrong on Decryptedtext buffer: Turn LED3 on */
    Error_Handler();
  }
  /* Reset Output Transfer Complete Detect */
  CrypCompleteDetected = 0; 
  
  /*##-4- AES ECB Encryption and Decryption in DMA mode with DataType 1 ######*/ 
  
  /* Get the CRYP parameters */  
  HAL_CRYP_GetConfig(&hcryp, &Conf); 
  
  /* Set the CRYP parameters */
  Conf.DataType = CRYP_DATATYPE_1B;
  
  /* Configure the CRYP  */
  HAL_CRYP_SetConfig(&hcryp, &Conf);
  
  /* CPU Data Cache maintenance */
  SCB_CleanDCache_by_Addr(Decryptedtext, 64);
  
  /* Encryption, result in  Encryptedtext buffer */ 
  HAL_CRYP_Encrypt_DMA(&hcryp, Plaintext_1, 16, Encryptedtext);
  
  /*Wait until output transfer complete*/
  while(CrypCompleteDetected == 0) 
  {  }
  
  /* CPU Data Cache maintenance */ 
  SCB_InvalidateDCache_by_Addr(Encryptedtext, 64);
  
  /*Compare with expected result */
  if(memcmp(Encryptedtext, Ciphertext_1, 64) != 0)
  {
    /* Not expected result, wrong on Encryptedtext buffer: Turn LED3 on */
    Error_Handler();
  }
  
  /* Reset Output Transfer Complete Detect */
  CrypCompleteDetected = 0; 
  
  /* CPU Data Cache maintenance  */
  SCB_CleanDCache_by_Addr(Decryptedtext, 64);
  
  /* Decryption, result in  Decryptedtext buffer */ 
  HAL_CRYP_Decrypt_DMA(&hcryp, Ciphertext_1 , 16, Decryptedtext);
  
  /*Wait until output transfer complete*/
  while(CrypCompleteDetected == 0) 
  {  }
  
  /* CPU Data Cache maintenance */ 
  SCB_InvalidateDCache_by_Addr(Decryptedtext, 64); 
  
  /*Compare with expected result */
  if(memcmp(Decryptedtext, Plaintext_1, 64) != 0)
  {
    /* Not expected result, wrong on Decryptedtext buffer: Turn LED3 on */
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
  
  /*!< Supply configuration update enable */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

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

