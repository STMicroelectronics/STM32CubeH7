/**
  ******************************************************************************
  * @file    hal_octospi_utility.c
  * @author  MCD Application Team
  * @brief   This file provides helper to initialize OCTOSPI/externalFlash.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "hal_octospi_utility.h"
#include "macronix.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define OSPI_NB_INSTANCES 2

/* Exported macro ------------------------------------------------------------*/
#define OSPI1_CS_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOB_CLK_ENABLE()
#define OSPI1_CLK_P_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOF_CLK_ENABLE()
#define OSPI1_CLK_N_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOF_CLK_ENABLE()
#define OSPI1_D0_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOD_CLK_ENABLE()
#define OSPI1_D1_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOD_CLK_ENABLE()
#define OSPI1_D2_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOE_CLK_ENABLE()
#define OSPI1_D3_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOD_CLK_ENABLE()
#define OSPI1_D4_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOH_CLK_ENABLE()
#define OSPI1_D5_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOH_CLK_ENABLE()
#define OSPI1_D6_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOG_CLK_ENABLE()
#define OSPI1_D7_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOG_CLK_ENABLE()
#define OSPI1_DQS_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()

#define OSPI2_CS_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOK_CLK_ENABLE()
#define OSPI2_CLK_P_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOI_CLK_ENABLE()
#define OSPI2_CLK_N_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOI_CLK_ENABLE()
#define OSPI2_D0_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOI_CLK_ENABLE()
#define OSPI2_D1_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOI_CLK_ENABLE()
#define OSPI2_D2_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOI_CLK_ENABLE()
#define OSPI2_D3_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOI_CLK_ENABLE()
#define OSPI2_D4_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOJ_CLK_ENABLE()
#define OSPI2_D5_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOJ_CLK_ENABLE()
#define OSPI2_D6_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOK_CLK_ENABLE()
#define OSPI2_D7_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOK_CLK_ENABLE()
#define OSPI2_DQS_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOK_CLK_ENABLE()


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
OCTOSPI_TypeDef * OSPI_INSTANCE[OSPI_NB_INSTANCES] = {OCTOSPI1, OCTOSPI2};

OSPI_HandleTypeDef OSPIHandle[OSPI_NB_INSTANCES];

uint32_t OSPIAddressMode;
uint32_t OSPIDataMode;

uint32_t OSPI_INSTANCE_IDX;
uint32_t OSPI_PRESC_COEF = 0x03;

extern uint32_t NomberOfLine[4];


/* Private functions ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void OSPI_ResetMemory(uint32_t Idx);

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
  }
}

uint32_t OSPI_Init(uint32_t NomberOfLine, uint32_t InstanceNumber)
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

  OSPIHandle[InstanceNumber].Instance = OSPI_INSTANCE[InstanceNumber];
  HAL_OSPI_DeInit(&OSPIHandle[InstanceNumber]);

  OSPIHandle[InstanceNumber].Init.FifoThreshold         = 32;
  OSPIHandle[InstanceNumber].Init.DualQuad              = HAL_OSPI_DUALQUAD_DISABLE;
  OSPIHandle[InstanceNumber].Init.MemoryType            = HAL_OSPI_MEMTYPE_MICRON;
  OSPIHandle[InstanceNumber].Init.DeviceSize            = 17;
  OSPIHandle[InstanceNumber].Init.ChipSelectHighTime    = 2;
  OSPIHandle[InstanceNumber].Init.FreeRunningClock      = HAL_OSPI_FREERUNCLK_DISABLE;
  OSPIHandle[InstanceNumber].Init.ClockMode             = HAL_OSPI_CLOCK_MODE_0;
  OSPIHandle[InstanceNumber].Init.WrapSize              = HAL_OSPI_WRAP_NOT_SUPPORTED;
  OSPIHandle[InstanceNumber].Init.ClockPrescaler        = 4;
  OSPIHandle[InstanceNumber].Init.SampleShifting        = HAL_OSPI_SAMPLE_SHIFTING_NONE;
  OSPIHandle[InstanceNumber].Init.DelayHoldQuarterCycle = HAL_OSPI_DHQC_DISABLE;
  OSPIHandle[InstanceNumber].Init.ChipSelectBoundary    = 0;

  if (HAL_OSPI_Init(&OSPIHandle[InstanceNumber]) != HAL_OK)
  {
    Error_Handler();
  }

    OSPI_ResetMemory(InstanceNumber);

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

  sCommand.Instruction = READ_ID_CMD;
  sCommand.AddressMode = HAL_OSPI_ADDRESS_NONE;
  sCommand.NbData      = 20;
  sCommand.DataMode    = OSPIDataMode;

  if (HAL_OSPI_Command(&OSPIHandle[InstanceNumber], &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_OSPI_Receive(&OSPIHandle[InstanceNumber], id, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }

  return 0;
}

uint32_t OSPI_DeInit(uint32_t Idx)
{
  HAL_OSPI_DeInit(&OSPIHandle[Idx]);
  return 0;
}


uint32_t OSPI_Read(uint32_t * pBuffer, uint32_t FlashAddress, uint32_t Size, uint32_t Idx)
{
  __IO uint8_t *mem_addr;
  uint8_t *ptr_buffer;
  uint16_t index;

  mem_addr = (uint8_t *)FlashAddress;
  ptr_buffer = (uint8_t *)pBuffer;

  for (index = 0; index < Size * 4; index++)
  {
    *ptr_buffer =  *mem_addr;
    mem_addr++;
    ptr_buffer++;
  }

  return 0;
}
uint32_t OSPI_Erase( uint32_t Idx)
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

  if (HAL_OSPI_Command(&OSPIHandle[Idx], &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }

  /* Configure automatic polling mode to wait for write enabling ---- */
  sConfig.Match           = 0x02;
  sConfig.Mask            = 0x02;

  sCommand.Instruction    = READ_STATUS_REG_CMD;
  sCommand.DataMode       = HAL_OSPI_DATA_1_LINE;
  sCommand.NbData         = 1;

  if (HAL_OSPI_Command(&OSPIHandle[Idx], &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_OSPI_AutoPolling(&OSPIHandle[Idx], &sConfig,HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }

  /* Erasing Sequence ----------------------------------------------- */

  sCommand.Instruction = BLOCK_ERASE_CMD;
  sCommand.AddressMode = HAL_OSPI_ADDRESS_1_LINE;
  sCommand.AddressSize = HAL_OSPI_ADDRESS_24_BITS;
  sCommand.Address     = 0;
  sCommand.DataMode    = HAL_OSPI_DATA_NONE;
  if (HAL_OSPI_Command(&OSPIHandle[Idx], &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
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

  if (HAL_OSPI_Command(&OSPIHandle[Idx], &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_OSPI_AutoPolling(&OSPIHandle[Idx], &sConfig,HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
  return 0;
}

uint32_t OSPI_Write(uint32_t * pBuffer, uint32_t FlashAddress, uint32_t Size, uint32_t Idx)
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

  if (HAL_OSPI_SetFifoThreshold(&OSPIHandle[Idx], 4) != HAL_OK)
  {
    Error_Handler();
  }

  sCommand.Instruction = WRITE_ENABLE_CMD;
  sCommand.AddressMode = HAL_OSPI_ADDRESS_NONE;
  sCommand.DataMode    = HAL_OSPI_DATA_NONE;

  if (HAL_OSPI_Command(&OSPIHandle[Idx], &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }

  /* Configure automatic polling mode to wait for write enabling ---- */
  sConfig.Match           = 0x02;
  sConfig.Mask            = 0x02;

  sCommand.Instruction    = READ_STATUS_REG_CMD;
  sCommand.DataMode       = HAL_OSPI_DATA_1_LINE;
  sCommand.NbData         = 1;

  if (HAL_OSPI_Command(&OSPIHandle[Idx], &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_OSPI_AutoPolling(&OSPIHandle[Idx], &sConfig,HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }

  /* Writing Sequence ----------------------------------------------- */
  if (HAL_OSPI_SetFifoThreshold(&OSPIHandle[Idx], 4) != HAL_OK)
  {
    Error_Handler();
  }

  sCommand.Instruction = PAGE_PROG_CMD;
  sCommand.AddressMode = HAL_OSPI_ADDRESS_1_LINE;
  sCommand.AddressSize = HAL_OSPI_ADDRESS_24_BITS;
  sCommand.Address     = FlashAddress;
  sCommand.DataMode    = HAL_OSPI_DATA_1_LINE;
  sCommand.NbData      = Size;
  if (HAL_OSPI_Command(&OSPIHandle[Idx], &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_OSPI_Transmit(&OSPIHandle[Idx],(uint8_t *) pBuffer,HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
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

  if (HAL_OSPI_Command(&OSPIHandle[Idx], &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_OSPI_AutoPolling(&OSPIHandle[Idx], &sConfig,HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
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
static void OSPI_ResetMemory(uint32_t Idx)
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
  if (HAL_OSPI_Command(&OSPIHandle[Idx], &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }

  /* Send the reset memory command */
  sCommand.Instruction = RESET_MEMORY_CMD;
  if (HAL_OSPI_Command(&OSPIHandle[Idx], &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }

  HAL_Delay(40);
}

/**
  * @brief  This function set memory mapped mode.
  * @param  None
  * @retval None
  */
uint32_t OSPI_MemoryMap(uint32_t  Idx)
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

  if (HAL_OSPI_SetFifoThreshold(&OSPIHandle[Idx], 4) != HAL_OK)
  {
    Error_Handler();
  }

  sCommand.OperationType = HAL_OSPI_OPTYPE_WRITE_CFG;
  sCommand.Instruction   = PAGE_PROG_CMD;
  sCommand.AddressMode   = HAL_OSPI_ADDRESS_1_LINE;
  sCommand.AddressSize   = HAL_OSPI_ADDRESS_24_BITS;
  sCommand.DataMode      = HAL_OSPI_DATA_1_LINE;
  sCommand.DummyCycles   = 0;

  if (HAL_OSPI_Command(&OSPIHandle[Idx], &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }

  sCommand.OperationType      = HAL_OSPI_OPTYPE_READ_CFG;
  sCommand.Instruction        = 0x03;
  sCommand.AddressMode        = HAL_OSPI_ADDRESS_1_LINE;
  sCommand.AlternateBytesMode = HAL_OSPI_ALTERNATE_BYTES_NONE;
  sCommand.DataMode           = HAL_OSPI_DATA_1_LINE;
  sCommand.DummyCycles        = 0;

  if (HAL_OSPI_Command(&OSPIHandle[Idx], &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }

  sMemMappedCfg.TimeOutActivation = HAL_OSPI_TIMEOUT_COUNTER_DISABLE;
  sMemMappedCfg.TimeOutPeriod     = 0x20;

  if (HAL_OSPI_MemoryMapped(&OSPIHandle[Idx], &sMemMappedCfg))
  {
    Error_Handler();
  }

  return 0;
}



