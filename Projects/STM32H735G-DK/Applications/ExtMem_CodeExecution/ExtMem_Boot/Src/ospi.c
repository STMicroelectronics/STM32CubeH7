/**
  ******************************************************************************
  * @file    ospi.c
  * @author  MCD Application Team
  * @brief   This file includes a driver for OSPI flashes support mounted on
  *          STM32H7B3I_DISCO evaluation boards.
  @verbatim
  PartNumber supported by the file:
  -----------------------
   - MX25LM51245G   :  OSPI Flash memory mounted on STM32H7B3I Discovery board.
  @endverbatim
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
#include "stm32h7xx_hal.h"
#include "memory.h"

#if (CODE_AREA == USE_OSPI) || (BINARY_AREA == USE_SPI_NOR) || (DATA_AREA == USE_EXTERNAL_RAM) || (CODE_AREA == USE_EXTERNAL_RAM)
/** @addtogroup OSPI
  * @{
  */

/** @defgroup OSPI
  * @{
  */


/* Private variables ---------------------------------------------------------*/

/** @defgroup OSPI_Private_Variables OSPI Private Variables
  * @{
  */
BSP_OSPI_NOR_Init_t     OSPI_Flash;
BSP_OSPI_RAM_Init_t     OSPI_RAM;

/**
  * @}
  */

/* Private Macros ------------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup OSPI_Private_Functions OSPI Private Functions
  * @{
  */
#if (BINARY_AREA == USE_SPI_NOR)
static uint32_t OSPI_Copy(uint32_t WriteAddr, uint32_t ReadAddr, uint32_t Size);
#endif
/** @defgroup OSPI_Exported_Functions OSPI Exported Functions
  * @{
  */

/**
  * @brief  Initializes and configure the OSPI interface.
  * @retval OSPI memory status
  */
uint32_t OSPI_Startup(uint32_t Mode)
{
#if (CODE_AREA == USE_OSPI)
  
  /* OSPI device configuration */
  OSPI_Flash.InterfaceMode = BSP_OSPI_NOR_OPI_MODE;
  OSPI_Flash.TransferRate  = BSP_OSPI_NOR_DTR_TRANSFER;
  
  if(BSP_OSPI_NOR_Init(0, &OSPI_Flash)!= BSP_ERROR_NONE)
  {
    return MEMORY_ERROR;
  }
  
  /* Enable MemoryMapped mode */
  if(BSP_OSPI_NOR_EnableMemoryMappedMode(0)!= BSP_ERROR_NONE)
  {
    return MEMORY_ERROR;
  } 

#elif (BINARY_AREA == USE_SPI_NOR)
  
  /* OSPI device configuration */
  OSPI_Flash.InterfaceMode = BSP_OSPI_NOR_SPI_MODE;
  OSPI_Flash.TransferRate  = BSP_OSPI_NOR_STR_TRANSFER;
  
  if(BSP_OSPI_NOR_Init(0, &OSPI_Flash)!= BSP_ERROR_NONE)
  {
    return MEMORY_ERROR;
  }
  
  if (OSPI_Copy(APPLICATION_ADDRESS, BINARY_BASE_OFFSET, BINARY_SIZE) != MEMORY_OK)
  {
    return MEMORY_ERROR;
  }
  
  /* De-Initialize the OSPI interface. */
  if(BSP_OSPI_NOR_DeInit(0)!= BSP_ERROR_NONE)
  {
    return MEMORY_ERROR;
  }
  
#endif /* (CODE_AREA == USE_OSPI) */
  return MEMORY_OK;
}

#if (BINARY_AREA == USE_SPI_NOR)
/**
  * @brief  Copy an amount of data from the OSPI memory to destination memory.
  * @param  WriteAddr: Pointer to data to be read
  * @param  ReadAddr: Read start address
  * @param  Size: Size of data to read
  * @retval OSPI memory status
  */
static uint32_t OSPI_Copy(uint32_t WriteAddr, uint32_t ReadAddr, uint32_t Size)
{
  while (Size)
  {
    if( BSP_OSPI_NOR_Read (0,  (uint8_t *)WriteAddr,  ReadAddr, Size)!= BSP_ERROR_NONE)
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

#if defined (USE_EXTERNAL_RAM)
uint32_t OSPI_RAM_Startup(void)
{
  
  /* OSPI device configuration */
  OSPI_RAM.BurstLength = BSP_OSPI_RAM_BURST_32_BYTES;
  OSPI_RAM.BurstType  = BSP_OSPI_RAM_LINEAR_BURST;
  OSPI_RAM.LatencyType = BSP_OSPI_RAM_FIXED_LATENCY;
  
  if(BSP_OSPI_RAM_Init(0,&OSPI_RAM) != BSP_ERROR_NONE)
  {
    return MEMORY_ERROR;
  }
  
  if(BSP_OSPI_RAM_EnableMemoryMappedMode(0) != BSP_ERROR_NONE)
  {
    return MEMORY_ERROR;
  }
  
  return MEMORY_OK;
}
#endif

/**
  * @brief  OSPI Clock Config for OSPI Memories .
  * @param  hospi  OSPI Handle
  *         Being __weak it can be overwritten by the application
  * @retval HAL_status
  */
HAL_StatusTypeDef MX_OSPI_ClockConfig(OSPI_HandleTypeDef *hospi)
{
  UNUSED(hospi);

  /* Keep using Bus clock for OSPI for safe execution from external OSPI memory.
     Do not use RCC PLL2 as OSPI source clock, RCC may be reset by the external memory running application at SystemInit */

  return HAL_OK;
}

/**
  * @brief  Initializes the OSPI interface.
  * @param  hospi          OSPI handle
  * @param  Config         OSPI config structure
  * @retval BSP status
  */
HAL_StatusTypeDef MX_OSPI_NOR_Init(OSPI_HandleTypeDef *hospi, MX_OSPI_InitTypeDef *Config)
{
  /*
   * With Init->ClockPrescaler = 2, the OSPI-NOR will run at 125 MHz which is above the spec
   * We need to change the default configuration to avoid corruption.
   * New Configuration for 62.5 MHz is :
   *   ClockPrescaler = 4
   * This will be reconfigured later by the selected Application after succeeding the system reset
   */
  
  /* OctoSPI initialization */
  hospi->Instance = OCTOSPI1;

  hospi->Init.FifoThreshold      = 4;
  hospi->Init.DualQuad           = HAL_OSPI_DUALQUAD_DISABLE;
  hospi->Init.DeviceSize         = Config->MemorySize; /* 512 MBits */
  hospi->Init.ChipSelectHighTime = 2;
  hospi->Init.FreeRunningClock   = HAL_OSPI_FREERUNCLK_DISABLE;
  hospi->Init.ClockMode          = HAL_OSPI_CLOCK_MODE_0;
  hospi->Init.WrapSize           = HAL_OSPI_WRAP_NOT_SUPPORTED;
  hospi->Init.ClockPrescaler     = 4;
  hospi->Init.SampleShifting     = Config->SampleShifting;
  hospi->Init.ChipSelectBoundary = 0;

  if (Config->TransferRate == BSP_OSPI_NOR_DTR_TRANSFER)
  {
    hospi->Init.MemoryType            = HAL_OSPI_MEMTYPE_MACRONIX;
    hospi->Init.DelayHoldQuarterCycle = HAL_OSPI_DHQC_ENABLE;
  }
  else
  {
    hospi->Init.MemoryType            = HAL_OSPI_MEMTYPE_MICRON;
    hospi->Init.DelayHoldQuarterCycle = HAL_OSPI_DHQC_DISABLE;
  }

  return HAL_OSPI_Init(hospi);
}

/**
  * @brief  Initializes the OSPI interface.
  * @param  hospi          OSPI handle
  * @param  Init           OSPI config structure
  * @retval BSP status
  */
HAL_StatusTypeDef MX_OSPI_RAM_Init(OSPI_HandleTypeDef *hospi, MX_OSPI_InitTypeDef *Init)
{
  OSPI_HyperbusCfgTypeDef sHyperbusCfg;
  HAL_StatusTypeDef status;

  hospi->Instance = OCTOSPI2;
  HAL_OSPI_DeInit(hospi);

  hospi->Init.FifoThreshold         = 4;
  hospi->Init.DualQuad              = HAL_OSPI_DUALQUAD_DISABLE;
  hospi->Init.MemoryType            = HAL_OSPI_MEMTYPE_HYPERBUS;
  hospi->Init.DeviceSize            = Init->MemorySize;
  hospi->Init.ChipSelectHighTime    = 4;
  hospi->Init.FreeRunningClock      = HAL_OSPI_FREERUNCLK_DISABLE;
  hospi->Init.ClockMode             = HAL_OSPI_CLOCK_MODE_0;
  hospi->Init.WrapSize              = HAL_OSPI_WRAP_NOT_SUPPORTED;
  hospi->Init.ClockPrescaler        = 4;
  hospi->Init.SampleShifting        = Init->SampleShifting;
  hospi->Init.DelayHoldQuarterCycle = HAL_OSPI_DHQC_ENABLE;
  hospi->Init.Refresh               = 250; /*4us @62.5MHz*/
  hospi->Init.ChipSelectBoundary    = 23;  /* memory die boundary 2^23=8MBs*/

  status = HAL_OSPI_Init(hospi);
  if (status == HAL_OK)
  {
    sHyperbusCfg.RWRecoveryTime   = RW_RECOVERY_TIME;
    sHyperbusCfg.AccessTime       = DEFAULT_INITIAL_LATENCY;
    sHyperbusCfg.WriteZeroLatency = HAL_OSPI_LATENCY_ON_WRITE;
    sHyperbusCfg.LatencyMode      = HAL_OSPI_FIXED_LATENCY;

    status = HAL_OSPI_HyperbusCfg(hospi, &sHyperbusCfg, HAL_OSPI_TIMEOUT_DEFAULT_VALUE);
  }

  return status;
}
/**
  * @}
  */

/**
  * @}
  */

#endif /* (CODE_AREA == USE_OSPI) || (BINARY_AREA == USE_SPI_NOR) */
