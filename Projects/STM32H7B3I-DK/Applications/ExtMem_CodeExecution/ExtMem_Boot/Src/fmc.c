/**
  ******************************************************************************
  * @file    fmc.c
  * @author  MCD Application Team
  * @brief   This file includes the fmc-memories supported STM32H7B3I_DISCO boards.
  @verbatim
  PartNumber supported by the file:
  -----------------------
   - IS42S16800F-6BLI   :  SDRAM external memory mounted on STM32H7B3I DISCOVERY board.

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

#if (DATA_AREA == USE_EXTERNAL_SDRAM) || (CODE_AREA == USE_EXTERNAL_SDRAM)
/**
  * @brief  Initializes the SDRAM device.
  * @retval SDRAM status
  */
uint32_t SDRAM_Startup(void)
{
  /* SDRAM device configuration */
  if(BSP_SDRAM_Init(0) != BSP_ERROR_NONE)
  {
    return MEMORY_ERROR;
  }
  
  return MEMORY_OK;
}

#endif /*(DATA_AREA == USE_EXTERNAL_SDRAM) || (CODE_AREA == USE_EXTERNAL_SDRAM)*/

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
