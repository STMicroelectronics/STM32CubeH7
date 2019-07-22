/**
  ******************************************************************************
  * @file    PWR/PWR_VOS0_480MHZ/CM7/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module for Cortex-M7
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"
#include "stm32h7xx_nucleo_144.h"
#include "common.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Set this define to 1 to OverClocking the system clock to 480MHz*/
#define USE_VOS0_480MHZ_OVERCLOCK 0

/*
   !!!Attention!!!
   Over clocking the system clock to 480MHz is only available with LDO PWR configuration
    by default The NUCLEO-H745ZI-Q board comes with DIRECT_SMPS configuration
    in order Over clock the system clock to 480MHz to the nucleo board must modified
    to change the PWR path to LDO instead of DIRECT_SMPS

   to do so please change the following solder bridges :
   - LDO config :
        Mount   : SB25, R33(0R), SB74
                  C58 & C54 = 2.2uF instead of 100nF
        Removed : SB92, SB79, R35(0R), SB75, SB76

   - Direct SMPS config (default config):
        Mount   : SB92, SB79, R35(0R), SB75, SB76
                  C58 & C54 = 100nF
        Removed : SB25, R33(0R), SB74

   Note that the Board HW configuration must match with the FW config
   if not will face a deadlock (can't connect the board any more)
   the FW PWR configuration correspond in the main.c to the following :
   Function SystemClock_Config :
     - case of LDO config : HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);
     - case of Direct SMPS config : HAL_PWREx_ConfigSupply(PWR_DIRECT_SMPS_SUPPLY);

   BOARD_HW_CONFIG_IS_LDO and BOARD_HW_CONFIG_IS_DIRECT_SMPS defines are used to confirm the
   board HW setting :
     - Set BOARD_HW_CONFIG_IS_LDO to 1 and BOARD_HW_CONFIG_IS_DIRECT_SMPS to 0 to confirm that
       the board is configured in LDO

     - Set BOARD_HW_CONFIG_IS_LDO to 0 and BOARD_HW_CONFIG_IS_DIRECT_SMPS to 1 to confirm that
       the board is configured in direct SMPS

   if a dead lock is faced due to a mismatch between the HW board setting and the FW setting (LDO,SMPS)
   user can recover the board by the following :
     - Power off the board
     - Put 3.3V on CN11 connector pin BT0 (Boot 0 pin)
     - This will change the boot pin to 1 instead of 0 and thus the device boot address will
        be changed to boot address 1 making the bootloader starting instead of the FW in the flash
       (FW that is setting a wrong LDO/SMPS config versus the HW board config)

     - Power on the board and connect using STM32CubeProgrammer
     - Erase the user FLash
     - Power off the board and remove the 3.3V input from CN11 connector BT0 pin
       to get back to boot address 0 config
     - The board is now recovered and can proceed normally.

*/
#define BOARD_HW_CONFIG_IS_LDO         0  /* Set to 1 : to confirm that board HW config is LDO, 0 if not */
#define BOARD_HW_CONFIG_IS_DIRECT_SMPS 1  /* Set to 1 : to confirm that board HW config is Direct SMPS, 0 if not */

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
