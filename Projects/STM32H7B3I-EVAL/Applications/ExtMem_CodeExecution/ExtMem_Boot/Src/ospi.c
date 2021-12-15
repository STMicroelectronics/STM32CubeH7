/**
  ******************************************************************************
  * @file    ospi.c
  * @author  MCD Application Team
  * @brief   This file includes a driver for OSPI flashes support mounted on
  *          STM32H7B3I_EVAL boards.
  @verbatim
  PartNumber supported by the file:
  -----------------------
   - MX25LM51245G   :  OSPI Flash memory mounted on STM32H7B3I EVAL board.
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

#if (CODE_AREA == USE_OSPI) || (BINARY_AREA == USE_SPI_NOR)
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
  uint8_t ospinor_status = MEMORY_OK;
#if (CODE_AREA == USE_OSPI)
  
  /* OSPI device configuration */
  OSPI_Flash.InterfaceMode = BSP_OSPI_NOR_OPI_MODE;
  OSPI_Flash.TransferRate  = BSP_OSPI_NOR_DTR_TRANSFER;
  
  if(BSP_OSPI_NOR_DeInit(0)!= BSP_ERROR_NONE)
  {
    ospinor_status =  MEMORY_ERROR;
  }
  
  if(BSP_OSPI_NOR_Init(0, &OSPI_Flash)!= BSP_ERROR_NONE)
  {
    ospinor_status =  MEMORY_ERROR;
  }
  
  /* Enable MemoryMapped mode */
  if(BSP_OSPI_NOR_EnableMemoryMappedMode(0)!= BSP_ERROR_NONE)
  {
    ospinor_status = MEMORY_ERROR;
  } 

#elif (BINARY_AREA == USE_SPI_NOR)
  
  /* OSPI device configuration */
  OSPI_Flash.InterfaceMode = BSP_OSPI_NOR_SPI_MODE;
  OSPI_Flash.TransferRate  = BSP_OSPI_NOR_STR_TRANSFER;
  
  if(BSP_OSPI_NOR_Init(0, &OSPI_Flash)!= BSP_ERROR_NONE)
  {
    ospinor_status = MEMORY_ERROR;
  }
  
  if (OSPI_Copy(APPLICATION_ADDRESS, BINARY_BASE_OFFSET, BINARY_SIZE) != MEMORY_OK)
  {
    ospinor_status = MEMORY_ERROR;
  }
  
  /* De-Initialize the OSPI interface. */
  if(BSP_OSPI_NOR_DeInit(0)!= BSP_ERROR_NONE)
  {
    ospinor_status = MEMORY_ERROR;
  }
  
#endif /* (CODE_AREA == USE_OSPI) */
  return ospinor_status;
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
HAL_StatusTypeDef MX_OSPI_NOR_Init(OSPI_HandleTypeDef *hospi, MX_OSPI_Config *Config)
{
    /* OctoSPI initialization */
  hospi->Instance = OCTOSPI1;

  hospi->Init.FifoThreshold         = 4;
  hospi->Init.DualQuad              = HAL_OSPI_DUALQUAD_DISABLE;
  hospi->Init.DeviceSize            = Config->MemorySize; /* 512 MBits */
  hospi->Init.ChipSelectHighTime    = 2;
  hospi->Init.FreeRunningClock      = HAL_OSPI_FREERUNCLK_DISABLE;
  hospi->Init.ClockMode             = HAL_OSPI_CLOCK_MODE_0;
  hospi->Init.WrapSize              = HAL_OSPI_WRAP_NOT_SUPPORTED;
  hospi->Init.ClockPrescaler        = 4;
  hospi->Init.SampleShifting        = Config->SampleShifting;
  hospi->Init.ChipSelectBoundary    = 0;
  hospi->Init.DelayBlockBypass      = HAL_OSPI_DELAY_BLOCK_BYPASSED;

  if (Config->TransferRate == (uint32_t)BSP_OSPI_NOR_DTR_TRANSFER)
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
  * @}
  */

/**
  * @}
  */

#endif /* (CODE_AREA == USE_OSPI) || (BINARY_AREA == USE_SPI_NOR) */
