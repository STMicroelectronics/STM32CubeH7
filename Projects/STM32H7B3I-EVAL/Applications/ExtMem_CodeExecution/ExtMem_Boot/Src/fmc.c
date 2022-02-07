/**
  ******************************************************************************
  * @file    fmc.c
  * @author  MCD Application Team
  * @brief   This file includes the fmc-memories supported STM32756G-EVAL,
  *          STM32H7B3I_EVAL evaluation boards.
  @verbatim
  PartNumber supported by the file:
  -----------------------
   - IS61WV102416BLL-10M :  SRAM external memory mounted on STM32H7B3I Evaluation board.
   - IS42S32800G-6BL     :  SDRAM external memory mounted on STM32H7B3I Evaluation board.

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
/**
  * @}
  */
/**
  * @brief  Initializes the SRAM device.
  * @retval SRAM status
  */
uint32_t SRAM_Startup(void)
{
  static uint8_t sram_status  = MEMORY_OK;

  /* SRAM device configuration */
  if(BSP_SRAM_Init(0)!= BSP_ERROR_NONE)
  {
    sram_status = MEMORY_ERROR;
  }

  return sram_status;
}
#endif /*(DATA_AREA == USE_EXTERNAL_SRAM) || (CODE_AREA == USE_EXTERNAL_SRAM)*/


#if (DATA_AREA == USE_EXTERNAL_SDRAM) || (CODE_AREA == USE_EXTERNAL_SDRAM)
/** @defgroup SDRAM_Constants PSRAM Defines
  * @{
  */
/**
  * @brief  FMC SDRAM Mode definition register defines
  */
/**
  * @}
  */
/**
  * @brief  Initializes the SDRAM device.
  * @retval SDRAM status
  */
uint32_t SDRAM_Startup(void)
{
  static uint8_t sdramstatus  = MEMORY_OK;

 if(BSP_SDRAM_Init(0)!= BSP_ERROR_NONE)
 {
   sdramstatus = MEMORY_ERROR;
 }
  return sdramstatus;
}
#endif /*(DATA_AREA == USE_EXTERNAL_SDRAM) || (CODE_AREA == USE_EXTERNAL_SDRAM)*/

#if (CODE_AREA == USE_NOR)
/**
  * @brief  Initializes the NOR device.
  * @retval NOR status
  */
uint32_t NOR_Startup(void)
{
  static uint8_t norstatus  = MEMORY_OK;

  if(BSP_NOR_Init(0) != BSP_ERROR_NONE)
  {
    norstatus = MEMORY_ERROR ;
  }
  return norstatus;
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
