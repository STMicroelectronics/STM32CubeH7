/**
  ******************************************************************************
  * @file    fmc.c
  * @author  MCD Application Team
  * @brief   This file includes the fmc-memories supported STM32756G-EVAL,
  *          STM32H743I_EVAL evaluation boards.
  @verbatim
  PartNumber supported by the file:
  -----------------------
   - IS61WV102416BLL-10M :  SRAM external memory mounted on STM32H743I Evaluation board.
   - IS42S32800G-6BL     :  SDRAM external memory mounted on STM32H743I Evaluation board.

  @endverbatim
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
#include "stm32h7xx_hal.h"
#include "memory.h"
#include "memory_msp.h"

/** @addtogroup FMC
  * @{
  */

/** @defgroup FMC
  * @{
  */

/** @defgroup FMC_Private_Types_Definitions FMC Private Types Definitions
  * @{
  */
/**
  * @}
  */

/** @defgroup FMC_Private_Defines FMC Private Defines
  * @{
  */
/**
  * @}
  */

/** @defgroup FMC_Private_Macros FMC Private Macros
  * @{
  */
/**
  * @}
  */

/** @defgroup FMC_Private_Variables FMC Private Variables
  * @{
  */
/**
  * @}
  */

/** @defgroup FMC_Private_Functions FMC Private Functions
  * @{
  */

#if (DATA_AREA == USE_EXTERNAL_SRAM) || (CODE_AREA == USE_EXTERNAL_SRAM)
/** @defgroup SRAM_Constants SRAM Defines
  * @{
  */
/* #define SRAM_MEMORY_WIDTH    FMC_NORSRAM_MEM_BUS_WIDTH_8*/
#define SRAM_MEMORY_WIDTH    FMC_NORSRAM_MEM_BUS_WIDTH_16

#define SRAM_BURSTACCESS     FMC_BURST_ACCESS_MODE_DISABLE
/* #define SRAM_BURSTACCESS     FMC_BURST_ACCESS_MODE_ENABLE*/

#define SRAM_WRITEBURST      FMC_WRITE_BURST_DISABLE
/* #define SRAM_WRITEBURST     FMC_WRITE_BURST_ENABLE */

#define CONTINUOUSCLOCK_FEATURE    FMC_CONTINUOUS_CLOCK_SYNC_ONLY
/* #define CONTINUOUSCLOCK_FEATURE     FMC_CONTINUOUS_CLOCK_SYNC_ASYNC */
/**
  * @}
  */
/**
  * @brief  Initializes the SRAM device.
  * @retval SRAM status
  */
uint32_t SRAM_Startup(void)
{
  SRAM_HandleTypeDef sramHandle;
  FMC_NORSRAM_TimingTypeDef Timing;
  static uint8_t sram_status;

  /* SRAM device configuration */
  sramHandle.Instance = FMC_NORSRAM_DEVICE;
  sramHandle.Extended = FMC_NORSRAM_EXTENDED_DEVICE;

  /* SRAM device configuration */
  /* Timing configuration derived from system clock (up to 400Mhz)
     for 100Mhz as SRAM clock frequency */
  Timing.AddressSetupTime      = 2;
  Timing.AddressHoldTime       = 1;
  Timing.DataSetupTime         = 2;
  Timing.BusTurnAroundDuration = 1;
  Timing.CLKDivision           = 2;
  Timing.DataLatency           = 2;
  Timing.AccessMode            = FMC_ACCESS_MODE_A;

  sramHandle.Init.NSBank             = FMC_NORSRAM_BANK3;
  sramHandle.Init.DataAddressMux     = FMC_DATA_ADDRESS_MUX_DISABLE;
  sramHandle.Init.MemoryType         = FMC_MEMORY_TYPE_SRAM;
  sramHandle.Init.MemoryDataWidth    = SRAM_MEMORY_WIDTH;
  sramHandle.Init.BurstAccessMode    = SRAM_BURSTACCESS;
  sramHandle.Init.WaitSignalPolarity = FMC_WAIT_SIGNAL_POLARITY_LOW;
  sramHandle.Init.WaitSignalActive   = FMC_WAIT_TIMING_BEFORE_WS;
  sramHandle.Init.WriteOperation     = FMC_WRITE_OPERATION_ENABLE;
  sramHandle.Init.WaitSignal         = FMC_WAIT_SIGNAL_DISABLE;
  sramHandle.Init.ExtendedMode       = FMC_EXTENDED_MODE_DISABLE;
  sramHandle.Init.AsynchronousWait   = FMC_ASYNCHRONOUS_WAIT_DISABLE;
  sramHandle.Init.WriteBurst         = SRAM_WRITEBURST;
  sramHandle.Init.ContinuousClock    = CONTINUOUSCLOCK_FEATURE;
  sramHandle.Init.WriteFifo          = FMC_WRITE_FIFO_DISABLE;
  sramHandle.Init.PageSize           = FMC_PAGE_SIZE_NONE;

  /* SRAM controller initialization */
  SRAM_MspInit();
  if(HAL_SRAM_Init(&sramHandle, &Timing, &Timing) != HAL_OK)
  {
    sram_status = MEMORY_ERROR;
  }
  else
  {
    sram_status = MEMORY_OK;
  }
  return sram_status;
}
#endif /*(DATA_AREA == USE_EXTERNAL_SRAM) || (CODE_AREA == USE_EXTERNAL_SRAM)*/


#if (DATA_AREA == USE_EXTERNAL_SDRAM) || (CODE_AREA == USE_EXTERNAL_SDRAM)
/** @defgroup SDRAM_Constants PSRAM Defines
  * @{
  */
/* #define SDRAM_MEMORY_WIDTH            FMC_SDRAM_MEM_BUS_WIDTH_8  */
/* #define SDRAM_MEMORY_WIDTH            FMC_SDRAM_MEM_BUS_WIDTH_16 */
#define SDRAM_MEMORY_WIDTH               FMC_SDRAM_MEM_BUS_WIDTH_32

#define SDCLOCK_PERIOD                   FMC_SDRAM_CLOCK_PERIOD_2
/* #define SDCLOCK_PERIOD                FMC_SDRAM_CLOCK_PERIOD_3 */

#define REFRESH_COUNT                    ((uint32_t)0x0603)   /* SDRAM refresh counter (100Mhz FMC clock) */

#define SDRAM_TIMEOUT                    ((uint32_t)0xFFFF)
/**
  * @brief  FMC SDRAM Mode definition register defines
  */
#define SDRAM_MODEREG_BURST_LENGTH_1             ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_LENGTH_2             ((uint16_t)0x0001)
#define SDRAM_MODEREG_BURST_LENGTH_4             ((uint16_t)0x0002)
#define SDRAM_MODEREG_BURST_LENGTH_8             ((uint16_t)0x0004)
#define SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL      ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_TYPE_INTERLEAVED     ((uint16_t)0x0008)
#define SDRAM_MODEREG_CAS_LATENCY_2              ((uint16_t)0x0020)
#define SDRAM_MODEREG_CAS_LATENCY_3              ((uint16_t)0x0030)
#define SDRAM_MODEREG_OPERATING_MODE_STANDARD    ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_PROGRAMMED ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_SINGLE     ((uint16_t)0x0200)
/**
  * @}
  */
/**
  * @brief  Initializes the SDRAM device.
  * @retval SDRAM status
  */
uint32_t SDRAM_Startup(void)
{
  SDRAM_HandleTypeDef sdramHandle;
  static FMC_SDRAM_TimingTypeDef Timing;
  static FMC_SDRAM_CommandTypeDef Command;
  __IO uint32_t tmpmrd = 0;
  static uint8_t sdramstatus ;

  /* SDRAM device configuration */
  sdramHandle.Instance = FMC_SDRAM_DEVICE;

  /* Timing configuration for 100Mhz as SDRAM clock frequency (System clock is up to 200Mhz) */
  Timing.LoadToActiveDelay    = 2;
  Timing.ExitSelfRefreshDelay = 7;
  Timing.SelfRefreshTime      = 4;
  Timing.RowCycleDelay        = 7;
  Timing.WriteRecoveryTime    = 2;
  Timing.RPDelay              = 2;
  Timing.RCDDelay             = 2;

  sdramHandle.Init.SDBank             = FMC_SDRAM_BANK2;
  sdramHandle.Init.ColumnBitsNumber   = FMC_SDRAM_COLUMN_BITS_NUM_9;
  sdramHandle.Init.RowBitsNumber      = FMC_SDRAM_ROW_BITS_NUM_12;
  sdramHandle.Init.MemoryDataWidth    = SDRAM_MEMORY_WIDTH;
  sdramHandle.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;
  sdramHandle.Init.CASLatency         = FMC_SDRAM_CAS_LATENCY_3;
  sdramHandle.Init.WriteProtection    = FMC_SDRAM_WRITE_PROTECTION_DISABLE;
  sdramHandle.Init.SDClockPeriod      = SDCLOCK_PERIOD;
  sdramHandle.Init.ReadBurst          = FMC_SDRAM_RBURST_ENABLE;
  sdramHandle.Init.ReadPipeDelay      = FMC_SDRAM_RPIPE_DELAY_0;

  /* SDRAM controller initialization */

  SDRAM_MspInit();

  if(HAL_SDRAM_Init(&sdramHandle, &Timing) != HAL_OK)
  {
    sdramstatus = MEMORY_ERROR;
  }
  else
  {
  /* SDRAM initialization sequence */
      /* Step 1: Configure a clock configuration enable command */
  Command.CommandMode            = FMC_SDRAM_CMD_CLK_ENABLE;
  Command.CommandTarget          = FMC_SDRAM_CMD_TARGET_BANK2;
  Command.AutoRefreshNumber      = 1;
  Command.ModeRegisterDefinition = 0;

  /* Send the command */
  HAL_SDRAM_SendCommand(&sdramHandle, &Command, SDRAM_TIMEOUT);

  /* Step 2: Insert 100 us minimum delay */
  /* Inserted delay is equal to 1 ms due to systick time base unit (ms) */
  HAL_Delay(1);

  /* Step 3: Configure a PALL (precharge all) command */
  Command.CommandMode            = FMC_SDRAM_CMD_PALL;
  Command.CommandTarget          = FMC_SDRAM_CMD_TARGET_BANK2;
  Command.AutoRefreshNumber      = 1;
  Command.ModeRegisterDefinition = 0;

  /* Send the command */
  HAL_SDRAM_SendCommand(&sdramHandle, &Command, SDRAM_TIMEOUT);

  /* Step 4: Configure an Auto Refresh command */
  Command.CommandMode            = FMC_SDRAM_CMD_AUTOREFRESH_MODE;
  Command.CommandTarget          = FMC_SDRAM_CMD_TARGET_BANK2;
  Command.AutoRefreshNumber      = 8;
  Command.ModeRegisterDefinition = 0;

  /* Send the command */
  HAL_SDRAM_SendCommand(&sdramHandle, &Command, SDRAM_TIMEOUT);

  /* Step 5: Program the external memory mode register */
  tmpmrd = (uint32_t)SDRAM_MODEREG_BURST_LENGTH_1          |\
                     SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL   |\
                     SDRAM_MODEREG_CAS_LATENCY_3           |\
                     SDRAM_MODEREG_OPERATING_MODE_STANDARD |\
                     SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;

  Command.CommandMode            = FMC_SDRAM_CMD_LOAD_MODE;
  Command.CommandTarget          = FMC_SDRAM_CMD_TARGET_BANK2;
  Command.AutoRefreshNumber      = 1;
  Command.ModeRegisterDefinition = tmpmrd;

  /* Send the command */
  HAL_SDRAM_SendCommand(&sdramHandle, &Command, SDRAM_TIMEOUT);

  /* Step 6: Set the refresh rate counter */
  /* Set the device refresh rate */
  HAL_SDRAM_ProgramRefreshRate(&sdramHandle, REFRESH_COUNT);

    sdramstatus = MEMORY_OK;
  }

  return sdramstatus;
}
#endif /*(DATA_AREA == USE_EXTERNAL_SDRAM) || (CODE_AREA == USE_EXTERNAL_SDRAM)*/

#if (CODE_AREA == USE_NOR)
/*#define NOR_MEMORY_WIDTH    FMC_NORSRAM_MEM_BUS_WIDTH_8*/
#define NOR_MEMORY_WIDTH    FMC_NORSRAM_MEM_BUS_WIDTH_16


#define NOR_BURSTACCESS    FMC_BURST_ACCESS_MODE_DISABLE  
/* #define NOR_BURSTACCESS    FMC_BURST_ACCESS_MODE_ENABLE*/
  
#define NOR_WRITEBURST    FMC_WRITE_BURST_DISABLE  
/* #define NOR_WRITEBURST   FMC_WRITE_BURST_ENABLE */
 
#define CONTINUOUSCLOCK_FEATURE    FMC_CONTINUOUS_CLOCK_SYNC_ONLY 
/* #define CONTINUOUSCLOCK_FEATURE     FMC_CONTINUOUS_CLOCK_SYNC_ASYNC */ 
/**
  * @brief  Initializes the NOR device.
  * @retval NOR status
  */
uint32_t NOR_Startup(void)
{
  NOR_HandleTypeDef norHandle;
  FMC_NORSRAM_TimingTypeDef Timing;
  norHandle.Instance  = FMC_NORSRAM_DEVICE;
  norHandle.Extended  = FMC_NORSRAM_EXTENDED_DEVICE;
  
  /* NOR device configuration */
  /* Timing configuration derived from system clock */  

  Timing.AddressSetupTime      = 9;
  Timing.AddressHoldTime       = 1;
  Timing.DataSetupTime         = 5;
  Timing.BusTurnAroundDuration = 4;
  Timing.CLKDivision           = 4;
  Timing.DataLatency           = 2;
  Timing.AccessMode            = FMC_ACCESS_MODE_A;
  
  norHandle.Init.NSBank             = FMC_NORSRAM_BANK1;
  norHandle.Init.DataAddressMux     = FMC_DATA_ADDRESS_MUX_DISABLE;
  norHandle.Init.MemoryType         = FMC_MEMORY_TYPE_NOR;
  norHandle.Init.MemoryDataWidth    = NOR_MEMORY_WIDTH;
  norHandle.Init.BurstAccessMode    = NOR_BURSTACCESS;
  norHandle.Init.WaitSignalPolarity = FMC_WAIT_SIGNAL_POLARITY_LOW;
  norHandle.Init.WaitSignalActive   = FMC_WAIT_TIMING_BEFORE_WS;
  norHandle.Init.WriteOperation     = FMC_WRITE_OPERATION_ENABLE;
  norHandle.Init.WaitSignal         = FMC_WAIT_SIGNAL_ENABLE;
  norHandle.Init.ExtendedMode       = FMC_EXTENDED_MODE_DISABLE;
  norHandle.Init.AsynchronousWait   = FMC_ASYNCHRONOUS_WAIT_ENABLE;
  norHandle.Init.WriteBurst         = NOR_WRITEBURST;
  norHandle.Init.ContinuousClock    = CONTINUOUSCLOCK_FEATURE;
  norHandle.Init.WriteFifo          = FMC_WRITE_FIFO_DISABLE;  
  norHandle.Init.PageSize           = FMC_PAGE_SIZE_NONE; 
  /* NOR controller initialization */
  NOR_MspInit();
  
  if(HAL_NOR_Init(&norHandle, &Timing, &Timing) != HAL_OK)
  {
    return MEMORY_ERROR;
  }
  else
  {
    return MEMORY_OK;
  }

}
#endif /* (CODE_AREA == USE_NOR) */
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
