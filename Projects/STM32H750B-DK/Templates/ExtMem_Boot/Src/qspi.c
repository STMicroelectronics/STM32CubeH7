/**
  ******************************************************************************
  * @file    qspi.c
  * @author  MCD Application Team
  * @brief   This file includes a driver for QSPI flashes support mounted on
  *          STM32H750B_DISCO evaluation boards.
  @verbatim
  PartNumber supported by the file:
  -----------------------
   - MT25QL512    :  QSPI Flash memory mounted on STM32H750B Discovery board.
  @endverbatim
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
#include "stm32h7xx_hal.h"
#include "memory.h"
#include "memory_msp.h"
#include "mt25tl01g/mt25tl01g.h"

#if (CODE_AREA == USE_QSPI) || (BINARY_AREA == USE_SPI_NOR)
/** @addtogroup QSPI
  * @{
  */

/** @defgroup QSPI
  * @{
  */


/* Private variables ---------------------------------------------------------*/

/** @defgroup QSPI_Private_Variables QSPI Private Variables
  * @{
  */
QSPI_HandleTypeDef       QSPIHandle;
/**
  * @}
  */

/* Private Macros ------------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup QSPI_Private_Functions QSPI Private Functions
  * @{
  */
static uint32_t QSPI_ResetMemory(QSPI_HandleTypeDef *hqspi);
static uint32_t QSPI_EnterFourBytesAddress(QSPI_HandleTypeDef *hqspi);
static uint32_t QSPI_DummyCyclesCfg(QSPI_HandleTypeDef *hqspi);
static uint32_t QSPI_WriteEnable(QSPI_HandleTypeDef *hqspi);
#if (CODE_AREA == USE_QSPI)
static uint32_t QSPI_EnableMemoryMappedMode(QSPI_HandleTypeDef *hqspi);
#elif  (BINARY_AREA == USE_SPI_NOR)
static uint32_t QSPI_Shutdown(void);
static uint32_t QSPI_Copy(uint32_t WriteAddr, uint32_t ReadAddr, uint32_t Size);
#endif
static uint32_t QSPI_AutoPollingMemReady(QSPI_HandleTypeDef *hqspi, uint32_t Timeout);
static uint8_t QSPI_EnterQPI(QSPI_HandleTypeDef *hqspi);
/**
  * @}
  */

/** @defgroup QSPI_Exported_Functions QSPI Exported Functions
  * @{
  */

/**
  * @brief  Initializes and configure the QSPI interface.
  * @retval QSPI memory status
  */
uint32_t QSPI_Startup(uint32_t Mode)
{
  QSPIHandle.Instance = QUADSPI;

  /* Call the DeInit function to reset the driver */
  if (HAL_QSPI_DeInit(&QSPIHandle) != HAL_OK)
  {
    return MEMORY_ERROR;
  }

  /* System level initialization */
  QSPI_MspInit();

  /* QSPI initialization */
  /* ClockPrescaler set to 1, so QSPI clock = 200MHz / (1+3) = 50MHz */
  QSPIHandle.Init.ClockPrescaler     = 3;
  QSPIHandle.Init.FifoThreshold      = 1;
  QSPIHandle.Init.SampleShifting     = QSPI_SAMPLE_SHIFTING_HALFCYCLE;
  QSPIHandle.Init.FlashSize          = POSITION_VAL(MT25TL01G_FLASH_SIZE) - 1;
  QSPIHandle.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_4_CYCLE;
  QSPIHandle.Init.ClockMode          = QSPI_CLOCK_MODE_0;
  QSPIHandle.Init.FlashID            = QSPI_FLASH_ID_1;
  QSPIHandle.Init.DualFlash          = QSPI_DUALFLASH_ENABLE;

 if (HAL_QSPI_Init(&QSPIHandle) != HAL_OK)
  {
    return MEMORY_ERROR;
  }

  /* QSPI memory reset */
  if (QSPI_ResetMemory(&QSPIHandle) != MEMORY_OK)
  {
    return MEMORY_ERROR;
  }
 
  /* Set the QSPI memory in 4-bytes address mode */
  if (QSPI_EnterFourBytesAddress(&QSPIHandle) != MEMORY_OK)
  {
    return MEMORY_ERROR;
  }

  /* Configuration of the dummy cycles on QSPI memory side */
  if (QSPI_DummyCyclesCfg(&QSPIHandle) != MEMORY_OK)
  {
    return MEMORY_ERROR;
  }
 
#if (CODE_AREA == USE_QSPI)
  /* Enable MemoryMapped mode */
  if( QSPI_EnableMemoryMappedMode(&QSPIHandle) != MEMORY_OK )
  {
    return MEMORY_ERROR;
  }
#elif (BINARY_AREA == USE_SPI_NOR)
  if (QSPI_Copy(APPLICATION_ADDRESS, BINARY_BASE_OFFSET, BINARY_SIZE) != MEMORY_OK)
  {
    return MEMORY_ERROR;
  }
  
  if (QSPI_Shutdown() != MEMORY_OK)
  {
    return MEMORY_ERROR;
  }
#endif /* (CODE_AREA == USE_QSPI) */
  return MEMORY_OK;
}

#if (BINARY_AREA == USE_SPI_NOR)
/**
  * @brief  De-Initializes and the QSPI interface.
  * @retval None
  */
static uint32_t QSPI_Shutdown(void)
{
  /* Call the DeInit function to reset the driver */
  if (HAL_QSPI_DeInit(&QSPIHandle) != HAL_OK)
  {
    return MEMORY_ERROR;
  }

  /* System level De-initialization */
  QSPI_MspDeInit();
  
  return MEMORY_OK;
}

/**
  * @brief  Copy an amount of data from the QSPI memory to destination memory.
  * @param  WriteAddr: Pointer to data to be read
  * @param  ReadAddr: Read start address
  * @param  Size: Size of data to read
  * @retval QSPI memory status
  */
static uint32_t QSPI_Copy(uint32_t WriteAddr, uint32_t ReadAddr, uint32_t Size)
{
  QSPI_CommandTypeDef s_command;

  /* Initialize the read command */
  s_command.InstructionMode   = QSPI_INSTRUCTION_4_LINES;
  s_command.Instruction       = MT25TL01G_QUAD_INOUT_FAST_READ_DTR_CMD; /* DTR QUAD INPUT/OUTPUT FAST READ and 4-BYTE DTR FAST READ commands */
  s_command.AddressMode       = QSPI_ADDRESS_4_LINES;
  s_command.AddressSize       = QSPI_ADDRESS_32_BITS;
  s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  s_command.DataMode          = QSPI_DATA_4_LINES;
  s_command.DummyCycles       = MT25TL01G_DUMMY_CYCLES_READ_QUAD_DTR - 1;
  s_command.DdrMode           = QSPI_DDR_MODE_ENABLE;
  s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_HALF_CLK_DELAY;
  s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  while (Size)
  {
    s_command.NbData            = (Size<256)?Size:256;
    s_command.Address           = ReadAddr;

    /* Configure the command */
    if (HAL_QSPI_Command(&QSPIHandle, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
      return MEMORY_ERROR;
    }

    /* Reception of the data */
    if (HAL_QSPI_Receive(&QSPIHandle, (uint8_t *)WriteAddr, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
      return MEMORY_ERROR;
    }

    if (Size > 256)
    {
      Size      -= 256;
      WriteAddr += 256;
      ReadAddr  += 256;
    }
    else
    {
      Size = 0;
    }
  }

  return MEMORY_OK;
}
#endif /* (BINARY_AREA == USE_SPI_NOR) */

/**
  * @}
  */

/** @addtogroup QSPI_Private_Functions
  * @{
  */

#if (CODE_AREA == USE_QSPI)
/**
  * @brief  Configure the QSPI in memory-mapped mode
  * @retval QSPI memory status
  */
static uint32_t QSPI_EnableMemoryMappedMode(QSPI_HandleTypeDef *hqspi)
{
  QSPI_CommandTypeDef      s_command;
  QSPI_MemoryMappedTypeDef s_mem_mapped_cfg;

  /* Configure the command for the read instruction */
  s_command.InstructionMode   = QSPI_INSTRUCTION_4_LINES;
  s_command.Instruction       = MT25TL01G_QUAD_INOUT_FAST_READ_DTR_CMD; /* DTR QUAD INPUT/OUTPUT FAST READ and 4-BYTE DTR FAST READ commands */
  s_command.AddressMode       = QSPI_ADDRESS_4_LINES;
  s_command.AddressSize       = QSPI_ADDRESS_32_BITS;
  s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  s_command.DataMode          = QSPI_DATA_4_LINES;
  s_command.DummyCycles       = MT25TL01G_DUMMY_CYCLES_READ_QUAD_DTR - 1;
  s_command.DdrMode           = QSPI_DDR_MODE_ENABLE;
  s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_HALF_CLK_DELAY;
  s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  /* Configure the memory mapped mode */
  s_mem_mapped_cfg.TimeOutActivation = QSPI_TIMEOUT_COUNTER_DISABLE;
  s_mem_mapped_cfg.TimeOutPeriod     = 0;

  if (HAL_QSPI_MemoryMapped(&QSPIHandle, &s_command, &s_mem_mapped_cfg) != HAL_OK)
  {
    return MEMORY_ERROR;
  }

  return MEMORY_OK;
}
#endif /* (BINARY_AREA == USE_SPI_NOR) */

/**
  * @brief  This function reset the QSPI memory.
  * @param  hqspi: QSPI handle
  * @retval None
  */
static uint32_t QSPI_ResetMemory(QSPI_HandleTypeDef *hqspi)
{
 QSPI_CommandTypeDef s_command;

  /* Initialize the reset enable command */
  s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
  s_command.Instruction       = MT25TL01G_RESET_ENABLE_CMD;
  s_command.AddressMode       = QSPI_ADDRESS_NONE;
  s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  s_command.DataMode          = QSPI_DATA_NONE;
  s_command.DummyCycles       = 0;
  s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
  s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  /* Send the command */
  if (HAL_QSPI_Command(hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MEMORY_ERROR;
  }

  /* Send the reset memory command */
  s_command.Instruction = MT25TL01G_RESET_MEMORY_CMD;
  if (HAL_QSPI_Command(hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MEMORY_ERROR;
  }

  s_command.InstructionMode   = QSPI_INSTRUCTION_4_LINES;
  s_command.Instruction       = MT25TL01G_RESET_ENABLE_CMD;
  /* Send the command */
  if (HAL_QSPI_Command(hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MEMORY_ERROR;
  }

  /* Send the reset memory command */
  s_command.Instruction = MT25TL01G_RESET_MEMORY_CMD;
  if (HAL_QSPI_Command(hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MEMORY_ERROR;
  }

  /* Enter QSPI memory in QPI mode */
  if(QSPI_EnterQPI(hqspi) != MEMORY_OK)
  {
    return MEMORY_ERROR;
  }  

  /* Configure automatic polling mode to wait the memory is ready */
  if (QSPI_AutoPollingMemReady(hqspi, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != MEMORY_OK)
  {
    return MEMORY_ERROR;
  }

  return MEMORY_OK;
}

/**
  * @brief  This function set the QSPI memory in 4-byte address mode
  * @param  hqspi: QSPI handle
  * @retval None
  */
static uint32_t QSPI_EnterFourBytesAddress(QSPI_HandleTypeDef *hqspi)
{
  QSPI_CommandTypeDef s_command;

  /* Initialize the command */
  s_command.InstructionMode   = QSPI_INSTRUCTION_4_LINES;
  s_command.Instruction       = MT25TL01G_ENTER_4_BYTE_ADDR_MODE_CMD;
  s_command.AddressMode       = QSPI_ADDRESS_NONE;
  s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  s_command.DataMode          = QSPI_DATA_NONE;
  s_command.DummyCycles       = 0;
  s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
  s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  /* Enable write operations */
  if (QSPI_WriteEnable(hqspi) != MEMORY_OK)
  {
    return MEMORY_ERROR;
  }

  /* Send the command */
  if (HAL_QSPI_Command(hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != MEMORY_OK)
  {
    return MEMORY_ERROR;
  }

  /* Configure automatic polling mode to wait the memory is ready */
  if (QSPI_AutoPollingMemReady(hqspi, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != MEMORY_OK)
  {
    return MEMORY_ERROR;
  }

  return MEMORY_OK;
}

/**
  * @brief  This function configure the dummy cycles on memory side.
  * @param  hqspi: QSPI handle
  * @retval None
  */
static uint32_t QSPI_DummyCyclesCfg(QSPI_HandleTypeDef *hqspi)
{
  QSPI_CommandTypeDef s_command;
  uint16_t reg=0;

  /* Initialize the read volatile configuration register command */
  s_command.InstructionMode   = QSPI_INSTRUCTION_4_LINES;
  s_command.Instruction       = MT25TL01G_READ_VOL_CFG_REG_CMD;
  s_command.AddressMode       = QSPI_ADDRESS_NONE;
  s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  s_command.DataMode          = QSPI_DATA_4_LINES;
  s_command.DummyCycles       = 0;
  s_command.NbData            = 2;
  s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
  s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  /* Configure the command */
  if (HAL_QSPI_Command(hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MEMORY_ERROR;
  }

  /* Reception of the data */
  if (HAL_QSPI_Receive(hqspi, (uint8_t *)(&reg), HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MEMORY_ERROR;
  }

  /* Enable write operations */
  if (QSPI_WriteEnable(hqspi) != MEMORY_OK)
  {
    return MEMORY_ERROR;
  }

  /* Update volatile configuration register (with new dummy cycles) */
  s_command.Instruction = MT25TL01G_WRITE_VOL_CFG_REG_CMD;
  MODIFY_REG(reg, 0xF0F0, ((MT25TL01G_DUMMY_CYCLES_READ_QUAD << 4) |
                               (MT25TL01G_DUMMY_CYCLES_READ_QUAD << 12)));

  /* Configure the write volatile configuration register command */
  if (HAL_QSPI_Command(hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MEMORY_ERROR;
  }

  /* Transmission of the data */
  if (HAL_QSPI_Transmit(hqspi, (uint8_t *)(&reg), HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MEMORY_ERROR;
  }

  return MEMORY_OK;
}

/**
  * @brief  This function send a Write Enable and wait it is effective.
  * @param  hqspi: QSPI handle
  * @retval None
  */
static uint32_t QSPI_WriteEnable(QSPI_HandleTypeDef *hqspi)
{
  QSPI_CommandTypeDef     s_command;
  QSPI_AutoPollingTypeDef s_config;

  /* Enable write operations */
  s_command.InstructionMode   = QSPI_INSTRUCTION_4_LINES;
  s_command.Instruction       = MT25TL01G_WRITE_ENABLE_CMD;
  s_command.AddressMode       = QSPI_ADDRESS_NONE;
  s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  s_command.DataMode          = QSPI_DATA_NONE;
  s_command.DummyCycles       = 0;
  s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
  s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  if (HAL_QSPI_Command(hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MEMORY_ERROR;
  }

  /* Configure automatic polling mode to wait for write enabling */
  s_config.Match           = MT25TL01G_SR_WREN | (MT25TL01G_SR_WREN << 8);
  s_config.Mask            = MT25TL01G_SR_WREN | (MT25TL01G_SR_WREN << 8);
  s_config.MatchMode       = QSPI_MATCH_MODE_AND;
  s_config.StatusBytesSize = 2;
  s_config.Interval        = 0x10;
  s_config.AutomaticStop   = QSPI_AUTOMATIC_STOP_ENABLE;

  s_command.Instruction    = MT25TL01G_READ_STATUS_REG_CMD;
  s_command.DataMode       = QSPI_DATA_4_LINES;

  if (HAL_QSPI_AutoPolling(hqspi, &s_command, &s_config, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MEMORY_ERROR;
  }

  return MEMORY_OK;
}

/**
  * @brief  This function read the SR of the memory and wait the EOP.
  * @param  hqspi: QSPI handle
  * @param  Timeout
  * @retval None
  */
static uint32_t QSPI_AutoPollingMemReady(QSPI_HandleTypeDef *hqspi, uint32_t Timeout)
{
  QSPI_CommandTypeDef     s_command;
  QSPI_AutoPollingTypeDef s_config;

  /* Configure automatic polling mode to wait for memory ready */
  s_command.InstructionMode   = QSPI_INSTRUCTION_4_LINES;
  s_command.Instruction       = MT25TL01G_READ_STATUS_REG_CMD;
  s_command.AddressMode       = QSPI_ADDRESS_NONE;
  s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  s_command.DataMode          = QSPI_DATA_4_LINES;
  s_command.DummyCycles       = 2;
  s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
  s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  s_config.Match           = 0;
  s_config.MatchMode       = QSPI_MATCH_MODE_AND;
  s_config.Interval        = 0x10;
  s_config.AutomaticStop   = QSPI_AUTOMATIC_STOP_ENABLE;
  s_config.Mask            = MT25TL01G_SR_WIP | (MT25TL01G_SR_WIP <<8);
  s_config.StatusBytesSize = 2;

  if (HAL_QSPI_AutoPolling(hqspi, &s_command, &s_config, Timeout) != HAL_OK)
  {
    return MEMORY_ERROR;
  }

  return MEMORY_OK;
}

/**
  * @brief  This function enter the QPSI memory in QPI mode
  * @param  hqspi QSPI handle 
  * @retval QSPI status
  */
static uint8_t QSPI_EnterQPI(QSPI_HandleTypeDef *hqspi)
{
  QSPI_CommandTypeDef s_command;

  s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
  s_command.Instruction       = MT25TL01G_ENTER_QUAD_CMD;
  s_command.AddressMode       = QSPI_ADDRESS_NONE;
  s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  s_command.DataMode          = QSPI_DATA_NONE;
  s_command.DummyCycles       = 0;
  s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
  s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  if (HAL_QSPI_Command(hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MEMORY_ERROR;
  }
  
  return MEMORY_OK;
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
#endif /* (CODE_AREA == USE_QSPI) || (BINARY_AREA == USE_SPI_NOR) */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
