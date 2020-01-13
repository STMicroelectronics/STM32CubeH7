/**
  ******************** (C) COPYRIGHT 2019 STMicroelectronics *******************
  * @file    OTFDEC/OTFDEC_ExecutingCryptedInstruction/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to execute an encrypted instruction
  *          located on the OCTOSPI external flash using the OTFDEC peripheral.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
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
#include "hal_octospi_utility.h"
/** @addtogroup STM32H7xx_HAL_Examples
  * @{
  */


/* Private typedef -----------------------------------------------------------*/
typedef uint32_t (*Customized_f)(uint32_t a,uint32_t b);

/* Private define ------------------------------------------------------------*/
#define LINE_CONFIG_NUMBER               4
#define OCTOSPI_INSTANCE_1               0
#define OCTOSPI_INSTANCE_2               1
#define KEY_NUMBER                       32
#define NONCE_NUMBER                     2
#define OSPI_NB_INSTANCES                2
#define PAGE_SIZE                        0x100
#define BUFFER_SIZE                      0x100
#define START_ADRESS_OCTOSPI2            0x70000000
#define START_ADRESS_OTFDEC2_REGION2     0x70003200
#define END_ADRESS_OTFDEC2_REGION2       0x700032FF
#define Region1                          0
#define Region2                          1
#define Region3                          2
#define Region4                          3


/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef UartHandle;
OTFDEC_HandleTypeDef OtfdecHandle;
Customized_f Decypt_add15;
uint32_t Result_Decrypt_Add15 = 0;
OTFDEC_RegionConfigTypeDef OTFDEC_Config = {0};
uint32_t NomberOfLine[LINE_CONFIG_NUMBER] = {1, 2, 4, 8};
uint32_t Key[4]={0xA9876543, 0x210FEDCB, 0xA9876543, 0x210FEDCB};
uint32_t Crypt_add[2] ={0x5424adec,0x3e4b1c70};
uint32_t a=15;
uint32_t b=17;

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void CPU_CACHE_Enable(void);
void Error_Handler(void);
static uint8_t Ref_CRC_computation(uint32_t * keyin);

#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
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
  - Configure the Systick to generate an interrupt each 1 msec
  - Set NVIC Group Priority to 4
  - Low Level Initialization
  */
  HAL_Init();

  /* Configure the system clock to 280 MHz */
  SystemClock_Config();

  /* Enable OTFDEC2 clock */
  __HAL_RCC_OTFDEC2_CLK_ENABLE();

  /* Init OCTOSPI */
  OSPI_Init(NomberOfLine[0],OCTOSPI_INSTANCE_2) ;
  OSPI_Erase(OCTOSPI_INSTANCE_2);

  /* Writing Crypted DATA*/
  OSPI_Write( Crypt_add,START_ADRESS_OTFDEC2_REGION2- START_ADRESS_OCTOSPI2, 8,OCTOSPI_INSTANCE_2);

  /* Activate memory mapping */
  OSPI_MemoryMap(OCTOSPI_INSTANCE_2);

  /* Init Region2 Of OTFDEC2 */
   OtfdecHandle.Instance = OTFDEC2;
  if (HAL_OTFDEC_Init(&OtfdecHandle) != HAL_OK)
  {
    Error_Handler();
  }

  /* Set OTFDEC Mode */
  if (HAL_OTFDEC_RegionSetMode( &OtfdecHandle,Region2, OTFDEC_REG_MODE_INSTRUCTION_ACCESSES_ONLY )!= HAL_OK)
  {
    Error_Handler();
  }

  /* Set OTFDEC Key */
  if (HAL_OTFDEC_RegionSetKey( &OtfdecHandle,Region2, Key) != HAL_OK)
  {
    Error_Handler();
  }

  /* Retrieve Key CRC from OTFDEC Peripheral and compare it with the expected CRC
  computed by software */
  if ((uint8_t)(HAL_OTFDEC_RegionGetKeyCRC( &OtfdecHandle,Region2 )) !=  Ref_CRC_computation(Key))
  {
    Error_Handler();
  }

  /* Activate OTFDEC decryption */
  OTFDEC_Config.Nonce[0]     = 0x11111111;
  OTFDEC_Config.Nonce[1]     = 0x55555555;
  OTFDEC_Config.StartAddress = START_ADRESS_OTFDEC2_REGION2;
  OTFDEC_Config.EndAddress   = END_ADRESS_OTFDEC2_REGION2;
  OTFDEC_Config.Version      = 0xA5A5;
  if (HAL_OTFDEC_RegionConfig(&OtfdecHandle,Region2,  &OTFDEC_Config, OTFDEC_REG_CONFIGR_LOCK_ENABLE)!= HAL_OK)
  {
    Error_Handler();
  }
  /* Executing opcode of arithmetic addition  */
  Decypt_add15 = (Customized_f)(START_ADRESS_OTFDEC2_REGION2+1);
  Result_Decrypt_Add15 = Decypt_add15(a,b);

  UartHandle.Instance          = USART1;
  UartHandle.Init.BaudRate     = 9600;
  UartHandle.Init.WordLength   = UART_WORDLENGTH_8B;
  UartHandle.Init.StopBits     = UART_STOPBITS_1;
  UartHandle.Init.Parity       = UART_PARITY_NONE;
  UartHandle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
  UartHandle.Init.Mode         = UART_MODE_TX_RX;
  UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&UartHandle);

  /* Output a message on Hyperterminal using printf function */
  printf("\n\r UART Printf Example: retarget the C library printf function to the UART\n\r");
  printf(" a=15 and b=17\n\r");
  printf(" a+b+15 = ");
  printf("%d  ",Result_Decrypt_Add15);
  printf("\n\r** Test finished successfully. ** \n\r");


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
  * @brief  Manage error.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* User may add here some code to deal with this error */
  while(1)
  {
  }
}

/**
  * @brief  OTFDEC Key CRC computation
  * @note   Source code of the Key CRC computation. Function is used to compute
  *         the expected CRC and compare it with that provided by the Peripheral
  *         in KEYCRC bitfield after loading the key in OTFDEC_RxKEYR registers.
  * @param  keyin OTFDEC key
  * @retval 8-bit long CRC
  */
static uint8_t Ref_CRC_computation(uint32_t * keyin)
{
  uint8_t CRC7_POLY = 0x7;
  uint32_t key_strobe[4] = {0xAA55AA55, 0x3, 0x18, 0xC0};
  uint8_t  i, j, k, crc = 0x0;
  uint32_t  keyval;

  for (j = 0; j < 4; j++)
  {
    keyval = *(keyin+j);
    if (j == 0)
    {
      keyval ^= key_strobe[0];
    }
    else
    {
      keyval ^= (key_strobe[j] << 24) | (crc << 16) | (key_strobe[j] << 8) | crc;
    }

    for (i = 0, crc = 0; i < 32; i++)
    {
      k = ((crc >> 7) ^ ((keyval >> (31-i))&0xF)) & 1;
      crc <<= 1;
      if (k)
      {
        crc ^= CRC7_POLY;
       }
    }

    crc^=0x55;
  }

  return crc;

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
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART1 and Loop until the end of transmission */
  HAL_UART_Transmit(&UartHandle, (uint8_t *)&ch, 1, 0xFFFF);

  return ch;
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
