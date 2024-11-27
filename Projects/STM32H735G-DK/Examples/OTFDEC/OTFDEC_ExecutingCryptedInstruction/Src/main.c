/**
******************************************************************************
* @file    OTFDEC/OTFDEC_Executing_Crypted_Instruction/Src/main.c
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

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define START_ADRESS_OCOTOSPI1                   0x90000000
#define START_ADRESS_OTFDEC1_REGION2             0x90000400
#define END_ADRESS_OTFDEC1_REGION2               0x900007FF

#define REGION2                                  1
#define SIZE_CRYPTED_ADD                         2

typedef uint32_t (*Customized_f)(uint32_t a,uint32_t b);

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
OTFDEC_RegionConfigTypeDef Config;
OSPI_HandleTypeDef OSPIHandle ;
OTFDEC_HandleTypeDef hotfdec;
OSPI_MemoryMappedTypeDef sMemMappedCfg;
uint32_t ExternalCRC = 0;
uint32_t InternalCRC = 0;
Customized_f External_Customized;
uint32_t OSPIAddressMode;
uint32_t OSPIDataMode;
uint32_t a=15;
uint32_t b=17;

uint32_t NomberOfLine[4] = {1, 2, 4, 8};
uint32_t Crypt_add[SIZE_CRYPTED_ADD] ={0x82bc10d8, 0xf7140591};
uint32_t Key[4]={0xEDCBA987, 0x6543210F, 0xEDCBA987, 0x6543210F };

/* Private function prototypes -----------------------------------------------*/
static void MPU_Config(void);
static void SystemClock_Config(void);
static void Error_Handler(void);
static void CPU_CACHE_Enable(void);
static uint32_t OSPI_DeInit(void);
static uint32_t OSPI_Init(uint32_t NomberOfLine);

static uint32_t OSPI_Write(uint32_t * pBuffer, uint32_t FlashAddress, uint32_t Size);
static uint32_t OSPI_MemoryMap(void);
static uint32_t Execute_Only(uint32_t a,uint32_t b);
static void OSPI_ResetMemory(void);
static uint32_t OSPI_Erase(void);

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
  - Configure the Systick to generate an interrupt each 1 msec
  - Set NVIC Group Priority to 4
  - Low Level Initialization
  */
  HAL_Init();

  /* Configure the system clock to 520 MHz */
  SystemClock_Config();

  BSP_LED_Init(LED_GREEN);
  BSP_LED_Init(LED_RED);

  /* Init OCTOSPI1 which interface MX25LM512ABA1G12 Macronix Flash Memory in single mode  */
  OSPI_Init(NomberOfLine[0]) ;

  /* Erase the Flash Memory   */
  OSPI_Erase();

  /* Preload external Flash Memory with ciphred instrution*/
  OSPI_Write( Crypt_add,START_ADRESS_OTFDEC1_REGION2- START_ADRESS_OCOTOSPI1, SIZE_CRYPTED_ADD*4);

  /* Activate memory mapping */
  OSPI_MemoryMap();

  /*Init OTFDEC*/
  hotfdec.Instance = OTFDEC1;
  HAL_OTFDEC_DeInit(&hotfdec);

  if (HAL_OTFDEC_Init(&hotfdec) != HAL_OK)
  {
    Error_Handler();
  }
  __HAL_OTFDEC_ENABLE_IT(&hotfdec, OTFDEC_ALL_INT);

  /* Set OTFDEC Mode */
  if (HAL_OTFDEC_RegionSetMode(&hotfdec, REGION2, OTFDEC_REG_MODE_INSTRUCTION_OR_DATA_ACCESSES) != HAL_OK)
  {
    Error_Handler();
  }

  /* Set OTFDEC Key */
  if (HAL_OTFDEC_RegionSetKey(&hotfdec,REGION2, Key) != HAL_OK)
  {
    Error_Handler();
  }

  /* Configure and lock region,enable OTFDEC decryption */
  Config.Nonce[0]     = 0xA5A5A5A5;
  Config.Nonce[1]     = 0xC3C3C3C3;
  Config.StartAddress = START_ADRESS_OTFDEC1_REGION2;
  Config.EndAddress   = END_ADRESS_OTFDEC1_REGION2;
  Config.Version      = 0xABCD;
  if (HAL_OTFDEC_RegionConfig(&hotfdec, REGION2, &Config, OTFDEC_REG_CONFIGR_LOCK_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /* Execute external customized CRC */
  External_Customized = (Customized_f)(START_ADRESS_OTFDEC1_REGION2+1);
  ExternalCRC = External_Customized(15,17);

  /* Execute external customized CRC */
  InternalCRC = Execute_Only(15,17);

  /* Check Result */
  if ( (InternalCRC&0xff) != (ExternalCRC&0xff) )
  {
    BSP_LED_On(LED_RED);
  }
  else 
  {
    BSP_LED_On(LED_GREEN);
  }
  /* DeInit OTFDEC */
  HAL_OTFDEC_DeInit(&hotfdec);

  /* DeInit OCTOSPI */
  OSPI_DeInit();
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
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
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

  /*
  Note : The activation of the I/O Compensation Cell is recommended with communication  interfaces
  (GPIO, SPI, FMC, OSPI ...)  when  operating at  high frequencies (please refer to product datasheet)
  The I/O Compensation Cell activation  procedure requires :
  - The activation of the CSI clock
  - The activation of the SYSCFG clock
  - Enabling the I/O Compensation Cell : setting bit[0] of register SYSCFG_CCCSR

  To do this please uncomment the following code
  */

  /*
  __HAL_RCC_CSI_ENABLE() ;

  __HAL_RCC_SYSCFG_CLK_ENABLE() ;

  HAL_EnableCompensationCell();
  */

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
    BSP_LED_Toggle(LED_RED);
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

  /* Configure the MPU OSPI flash */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress = 0x90000000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_64MB;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER1;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x0;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
  
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

static uint32_t Execute_Only(uint32_t a,uint32_t b)
{

  return a+b+15;
}
/******************************************************************************/
/*                           OCTOSPI  Helper                                  */
/******************************************************************************/

static uint32_t OSPI_Init(uint32_t NomberOfLine)
{
  uint8_t id[20];

  OSPI_RegularCmdTypeDef  sCommand;

  if ( (NomberOfLine != 1) && (NomberOfLine != 2) && (NomberOfLine != 4) && (NomberOfLine != 8) )
  {
    Error_Handler();
  }

  switch (NomberOfLine)
  {
  case 1: OSPIAddressMode = HAL_OSPI_ADDRESS_1_LINE;  OSPIDataMode = HAL_OSPI_DATA_1_LINE;  break;
  case 2: OSPIAddressMode = HAL_OSPI_ADDRESS_2_LINES; OSPIDataMode = HAL_OSPI_DATA_2_LINES; break;
  case 4: OSPIAddressMode = HAL_OSPI_ADDRESS_4_LINES; OSPIDataMode = HAL_OSPI_DATA_4_LINES; break;
  case 8: OSPIAddressMode = HAL_OSPI_ADDRESS_8_LINES; OSPIDataMode = HAL_OSPI_DATA_8_LINES; break;
  default: Error_Handler(); break;
  }

  OSPIHandle.Instance = OCTOSPI1;
  HAL_OSPI_DeInit(&OSPIHandle);

  OSPIHandle.Init.FifoThreshold         = 32;
  OSPIHandle.Init.DualQuad              = HAL_OSPI_DUALQUAD_DISABLE;
  OSPIHandle.Init.MemoryType            = HAL_OSPI_MEMTYPE_MACRONIX;
  OSPIHandle.Init.DeviceSize            = 17;
  OSPIHandle.Init.ChipSelectHighTime    = 2;
  OSPIHandle.Init.FreeRunningClock      = HAL_OSPI_FREERUNCLK_DISABLE;
  OSPIHandle.Init.ClockMode             = HAL_OSPI_CLOCK_MODE_0;
  OSPIHandle.Init.WrapSize              = HAL_OSPI_WRAP_NOT_SUPPORTED;
  OSPIHandle.Init.ClockPrescaler        = 0x20;
  OSPIHandle.Init.SampleShifting        = HAL_OSPI_SAMPLE_SHIFTING_NONE;
  OSPIHandle.Init.DelayHoldQuarterCycle = HAL_OSPI_DHQC_DISABLE;
  OSPIHandle.Init.ChipSelectBoundary    = 0;

  if (HAL_OSPI_Init(&OSPIHandle) != HAL_OK)
  {
    Error_Handler();
  }

  OSPI_ResetMemory();

  /* Initialize the reset enable command */
  sCommand.OperationType         = HAL_OSPI_OPTYPE_COMMON_CFG;
  sCommand.FlashId               = HAL_OSPI_FLASH_ID_1;
  sCommand.InstructionMode       = HAL_OSPI_INSTRUCTION_1_LINE;
  sCommand.InstructionSize       = HAL_OSPI_INSTRUCTION_8_BITS;
  sCommand.InstructionDtrMode    = HAL_OSPI_INSTRUCTION_DTR_DISABLE;
  sCommand.AddressDtrMode        = HAL_OSPI_ADDRESS_DTR_DISABLE;
  sCommand.AlternateBytes        = 0x00;
  sCommand.AlternateBytesMode    = HAL_OSPI_ALTERNATE_BYTES_NONE;
  sCommand.AlternateBytesSize    = HAL_OSPI_ALTERNATE_BYTES_8_BITS;
  sCommand.AlternateBytesDtrMode = HAL_OSPI_ALTERNATE_BYTES_DTR_DISABLE;
  sCommand.DataDtrMode           = HAL_OSPI_DATA_DTR_DISABLE;
  sCommand.DummyCycles           = 0;
  sCommand.DQSMode               = HAL_OSPI_DQS_DISABLE;
  sCommand.SIOOMode              = HAL_OSPI_SIOO_INST_EVERY_CMD;

  sCommand.Instruction           = READ_ID_CMD;
  sCommand.AddressMode           = HAL_OSPI_ADDRESS_NONE;
  sCommand.NbData                = 20;
  sCommand.DataMode              = OSPIDataMode;

  if (HAL_OSPI_Command(&OSPIHandle, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_OSPI_Receive(&OSPIHandle, id, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }

  return 0;
}

static uint32_t OSPI_DeInit(void)
{
  OSPIHandle.Instance = OCTOSPI1;
  HAL_OSPI_DeInit(&OSPIHandle);
  return 0;
}


static uint32_t OSPI_Write(uint32_t * pBuffer, uint32_t FlashAddress, uint32_t Size)
{
  OSPI_RegularCmdTypeDef  sCommand;
  OSPI_AutoPollingTypeDef sConfig;

  if (Size == 0)
  {
    Error_Handler();
  }

  sConfig.MatchMode              = HAL_OSPI_MATCH_MODE_AND;
  sConfig.AutomaticStop          = HAL_OSPI_AUTOMATIC_STOP_ENABLE;
  sConfig.Interval               = 0x10;

  /* Initialize the command */
  sCommand.OperationType         = HAL_OSPI_OPTYPE_COMMON_CFG;
  sCommand.Instruction           = PAGE_PROG_CMD;
  sCommand.FlashId               = HAL_OSPI_FLASH_ID_1;
  sCommand.InstructionMode       = HAL_OSPI_INSTRUCTION_1_LINE;
  sCommand.InstructionSize       = HAL_OSPI_INSTRUCTION_8_BITS ;
  sCommand.InstructionDtrMode    = HAL_OSPI_INSTRUCTION_DTR_DISABLE;
  sCommand.AddressDtrMode        = HAL_OSPI_ADDRESS_DTR_DISABLE;
  sCommand.AlternateBytes        = 0x00;
  sCommand.AlternateBytesMode    = HAL_OSPI_ALTERNATE_BYTES_NONE;
  sCommand.AlternateBytesSize    = HAL_OSPI_ALTERNATE_BYTES_8_BITS;
  sCommand.AlternateBytesDtrMode = HAL_OSPI_ALTERNATE_BYTES_DTR_DISABLE;
  sCommand.DataDtrMode           = HAL_OSPI_DATA_DTR_DISABLE;
  sCommand.DummyCycles           = 0;
  sCommand.DQSMode               = HAL_OSPI_DQS_DISABLE;
  sCommand.SIOOMode              = HAL_OSPI_SIOO_INST_EVERY_CMD;

  /* 1- Enable write operations ----------------------------------------- */
  sCommand.Instruction = WRITE_ENABLE_CMD;
  sCommand.AddressMode = HAL_OSPI_ADDRESS_NONE;
  sCommand.DataMode    = HAL_OSPI_DATA_NONE;

  if (HAL_OSPI_SetFifoThreshold(&OSPIHandle, 4) != HAL_OK)
  {
    Error_Handler();
  }

  sCommand.Instruction = WRITE_ENABLE_CMD;
  sCommand.AddressMode = HAL_OSPI_ADDRESS_NONE;
  sCommand.DataMode    = HAL_OSPI_DATA_NONE;

  if (HAL_OSPI_Command(&OSPIHandle, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }

  /* Configure automatic polling mode to wait for write enabling ---- */
  sConfig.Match           = 0x02;
  sConfig.Mask            = 0x02;

  sCommand.Instruction    = READ_STATUS_REG_CMD;
  sCommand.DataMode       = HAL_OSPI_DATA_1_LINE;
  sCommand.NbData         = 1;

  if (HAL_OSPI_Command(&OSPIHandle, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_OSPI_AutoPolling(&OSPIHandle, &sConfig,HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }

  /* Writing Sequence ----------------------------------------------- */
  if (HAL_OSPI_SetFifoThreshold(&OSPIHandle, 4) != HAL_OK)
  {
    Error_Handler();
  }

  sCommand.Instruction = PAGE_PROG_CMD;
  sCommand.AddressMode = HAL_OSPI_ADDRESS_1_LINE;
  sCommand.AddressSize = HAL_OSPI_ADDRESS_24_BITS;
  sCommand.Address     = FlashAddress;
  sCommand.DataMode    = HAL_OSPI_DATA_1_LINE;
  sCommand.NbData      = Size;
  if (HAL_OSPI_Command(&OSPIHandle, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_OSPI_Transmit(&OSPIHandle,(uint8_t *) pBuffer,HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }

  /* Configure automatic polling mode to wait for end of program ---- */
  sConfig.Match           = MEMORY_READY_MATCH_VALUE;
  sConfig.Mask            = MEMORY_READY_MASK_VALUE;

  sCommand.Instruction    = READ_STATUS_REG_CMD;
  sCommand.AddressMode    = HAL_OSPI_ADDRESS_NONE;
  sCommand.DataMode       = HAL_OSPI_DATA_1_LINE;
  sCommand.NbData         = 1;

  if (HAL_OSPI_Command(&OSPIHandle, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_OSPI_AutoPolling(&OSPIHandle, &sConfig,HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
  return 0;
}

/**
* @brief  This function set memory mapped mode.
* @param  None
* @retval None
*/
static uint32_t OSPI_MemoryMap(void)
{

  OSPI_RegularCmdTypeDef   sCommand;
  OSPI_MemoryMappedTypeDef sMemMappedCfg;

  /* Initialize the command */
  sCommand.OperationType         = HAL_OSPI_OPTYPE_COMMON_CFG;
  sCommand.Instruction           = PAGE_PROG_CMD;
  sCommand.FlashId               = HAL_OSPI_FLASH_ID_1;
  sCommand.InstructionMode       = HAL_OSPI_INSTRUCTION_1_LINE;
  sCommand.InstructionSize       = HAL_OSPI_INSTRUCTION_8_BITS ;
  sCommand.InstructionDtrMode    = HAL_OSPI_INSTRUCTION_DTR_DISABLE;
  sCommand.AddressDtrMode        = HAL_OSPI_ADDRESS_DTR_DISABLE;
  sCommand.AlternateBytes        = 0x00;
  sCommand.AlternateBytesMode    = HAL_OSPI_ALTERNATE_BYTES_NONE;
  sCommand.AlternateBytesSize    = HAL_OSPI_ALTERNATE_BYTES_8_BITS;
  sCommand.AlternateBytesDtrMode = HAL_OSPI_ALTERNATE_BYTES_DTR_DISABLE;
  sCommand.DataDtrMode           = HAL_OSPI_DATA_DTR_DISABLE;
  sCommand.DummyCycles           = 0;
  sCommand.DQSMode               = HAL_OSPI_DQS_DISABLE;
  sCommand.SIOOMode              = HAL_OSPI_SIOO_INST_EVERY_CMD;

  if (HAL_OSPI_SetFifoThreshold(&OSPIHandle, 4) != HAL_OK)
  {
    Error_Handler();
  }

  sCommand.OperationType = HAL_OSPI_OPTYPE_WRITE_CFG;
  sCommand.Instruction   = PAGE_PROG_CMD;
  sCommand.AddressMode   = HAL_OSPI_ADDRESS_1_LINE;
  sCommand.AddressSize   = HAL_OSPI_ADDRESS_24_BITS;
  sCommand.DataMode      = HAL_OSPI_DATA_1_LINE;
  sCommand.DummyCycles   = 0;

  if (HAL_OSPI_Command(&OSPIHandle, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }

  sCommand.OperationType      = HAL_OSPI_OPTYPE_READ_CFG;
  sCommand.Instruction        = 0x03;
  sCommand.AddressMode        = HAL_OSPI_ADDRESS_1_LINE;
  sCommand.AlternateBytesMode = HAL_OSPI_ALTERNATE_BYTES_NONE;
  sCommand.DataMode           = HAL_OSPI_DATA_1_LINE;
  sCommand.DummyCycles        = 0;

  if (HAL_OSPI_Command(&OSPIHandle, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }

  sMemMappedCfg.TimeOutActivation = HAL_OSPI_TIMEOUT_COUNTER_DISABLE;
  sMemMappedCfg.TimeOutPeriod     = 0x20;

  if (HAL_OSPI_MemoryMapped(&OSPIHandle, &sMemMappedCfg))
  {
    Error_Handler();
  }

  return 0;
}

/**
* @brief  This function reset the OSPI memory.
* @param  None
* @retval None
*/
static void OSPI_ResetMemory(void)
{
  OSPI_RegularCmdTypeDef  sCommand;

  /* Initialize the reset enable command */
  sCommand.OperationType         = HAL_OSPI_OPTYPE_COMMON_CFG;
  sCommand.FlashId               = HAL_OSPI_FLASH_ID_1;
  sCommand.InstructionMode       = HAL_OSPI_INSTRUCTION_1_LINE;
  sCommand.InstructionSize       = HAL_OSPI_INSTRUCTION_8_BITS;
  sCommand.InstructionDtrMode    = HAL_OSPI_INSTRUCTION_DTR_DISABLE;
  sCommand.AddressMode           = HAL_OSPI_ADDRESS_NONE;
  sCommand.AlternateBytesMode    = HAL_OSPI_ALTERNATE_BYTES_NONE;
  sCommand.DataDtrMode           = HAL_OSPI_DATA_DTR_DISABLE;
  sCommand.DummyCycles           = 0;
  sCommand.DQSMode               = HAL_OSPI_DQS_DISABLE;
  sCommand.SIOOMode              = HAL_OSPI_SIOO_INST_EVERY_CMD;
  sCommand.DataMode              = HAL_OSPI_DATA_NONE;

  sCommand.Instruction = RESET_ENABLE_CMD;
  /* Send the command */
  if (HAL_OSPI_Command(&OSPIHandle, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }


  /* Send the reset memory command */
  sCommand.Instruction = RESET_MEMORY_CMD;
  if (HAL_OSPI_Command(&OSPIHandle, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }

  HAL_Delay(40);
}

static uint32_t OSPI_Erase(void)
{
  OSPI_RegularCmdTypeDef  sCommand;
  OSPI_AutoPollingTypeDef sConfig;

  sConfig.MatchMode              = HAL_OSPI_MATCH_MODE_AND;
  sConfig.AutomaticStop          = HAL_OSPI_AUTOMATIC_STOP_ENABLE;
  sConfig.Interval               = 0x10;

  /* Initialize the command */
  sCommand.OperationType         = HAL_OSPI_OPTYPE_COMMON_CFG;
  sCommand.Instruction           = PAGE_PROG_CMD;
  sCommand.FlashId               = HAL_OSPI_FLASH_ID_1;
  sCommand.InstructionMode       = HAL_OSPI_INSTRUCTION_1_LINE;
  sCommand.InstructionSize       = HAL_OSPI_INSTRUCTION_8_BITS ;
  sCommand.InstructionDtrMode    = HAL_OSPI_INSTRUCTION_DTR_DISABLE;
  sCommand.AddressDtrMode        = HAL_OSPI_ADDRESS_DTR_DISABLE;
  sCommand.AlternateBytes        = 0x00;
  sCommand.AlternateBytesMode    = HAL_OSPI_ALTERNATE_BYTES_NONE;
  sCommand.AlternateBytesSize    = HAL_OSPI_ALTERNATE_BYTES_8_BITS;
  sCommand.AlternateBytesDtrMode = HAL_OSPI_ALTERNATE_BYTES_DTR_DISABLE;
  sCommand.DataDtrMode           = HAL_OSPI_DATA_DTR_DISABLE;
  sCommand.DummyCycles           = 0;
  sCommand.DQSMode               = HAL_OSPI_DQS_DISABLE;
  sCommand.SIOOMode              = HAL_OSPI_SIOO_INST_EVERY_CMD;

  /* 1- Enable write operations ----------------------------------------- */
  sCommand.Instruction = WRITE_ENABLE_CMD;
  sCommand.AddressMode = HAL_OSPI_ADDRESS_NONE;
  sCommand.DataMode    = HAL_OSPI_DATA_NONE;

  if (HAL_OSPI_Command(&OSPIHandle, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }

  /* Configure automatic polling mode to wait for write enabling ---- */
  sConfig.Match           = 0x02;
  sConfig.Mask            = 0x02;

  sCommand.Instruction    = READ_STATUS_REG_CMD;
  sCommand.DataMode       = HAL_OSPI_DATA_1_LINE;
  sCommand.NbData         = 1;

  if (HAL_OSPI_Command(&OSPIHandle, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_OSPI_AutoPolling(&OSPIHandle, &sConfig,HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }

  /* Erasing Sequence ----------------------------------------------- */

  sCommand.Instruction = BLOCK_ERASE_CMD;
  sCommand.AddressMode = HAL_OSPI_ADDRESS_1_LINE;
  sCommand.AddressSize = HAL_OSPI_ADDRESS_24_BITS;
  sCommand.Address     = 0;
  sCommand.DataMode    = HAL_OSPI_DATA_NONE;
  if (HAL_OSPI_Command(&OSPIHandle, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }

  /* Configure automatic polling mode to wait for end of erase ------ */
  sConfig.Match           = MEMORY_READY_MATCH_VALUE;
  sConfig.Mask            = MEMORY_READY_MASK_VALUE;

  sCommand.Instruction    = READ_STATUS_REG_CMD;
  sCommand.AddressMode    = HAL_OSPI_ADDRESS_NONE;
  sCommand.DataMode       = HAL_OSPI_DATA_1_LINE;
  sCommand.NbData         = 1;

  if (HAL_OSPI_Command(&OSPIHandle, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_OSPI_AutoPolling(&OSPIHandle, &sConfig,HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
  return 0;
}

/**
* @}
*/

