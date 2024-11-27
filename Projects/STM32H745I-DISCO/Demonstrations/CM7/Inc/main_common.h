/**
  ******************************************************************************
  * @file    Demonstration/STM32H745-Discovery_Demo/CM7/Inc/main_common.h
  * @author  MCD Application Team
  * @brief   Common header file
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_COMMON_H
#define __MAIN_COMMON_H

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* Set this define to 1 to OverClocking the system clock to 480MHz*/
#define USE_VOS0_480MHZ_OVERCLOCK 0
/*
   !!!Attention!!!

   Over clocking the system clock to 480MHz is only available with LDO PWR configuration
    by default The STM32H745I-DISCO comes with DIRECT_SMPS configuration
    in order to Over clock the system clock to 480MHz to the disco board must modified
    to change the PWR path to LDO instead of DIRECT_SMPS

   to do so please change the following solder bridges :
   For STM32H745I-DISCO rev.B
       - LDO config :
            Mount   : SB6, SB17, SB45
            Removed : SB5, SB18, SB44, SB46, L4

       - Direct SMPS config :
            Mount   : SB5, SB18, SB44, SB46, L4
            Removed : SB6, SB17, SB45

    For STM32H745I-DISCO rev.A
       - LDO config :
            Mount   : SB6, SB17, and add 2.2uF capacitor on C18 and C75
            Removed : L4, C52, SB5, SB18

       - Direct SMPS config :
            Mount   : C52, SB5, SB18
            Removed : SB6, SB17, C18 and C75
            L4 should be removed, and short its two pads together

   Note that the Board HW configuration must match with the FW config
   if not will face a deadlock (can't connect the board any more)
   The FW PWR configuration corresponds to the following:
   Compilation switch: :
     - In the case of LDO config: Add "USE_PWR_LDO_SUPPLY" define in the toolchain preprocessor settings.
     - In the case of Direct SMPS config: Add "USE_PWR_DIRECT_SMPS_SUPPLY" define in the toolchain preprocessor settings.

   BOARD_HW_CONFIG_IS_LDO and BOARD_HW_CONFIG_IS_DIRECT_SMPS defines are used to confirm the
   board HW settings :
     - Set BOARD_HW_CONFIG_IS_LDO to 1 and BOARD_HW_CONFIG_IS_DIRECT_SMPS to 0 to confirm that
       the board is configured in LDO

     - Set BOARD_HW_CONFIG_IS_LDO to 0 and BOARD_HW_CONFIG_IS_DIRECT_SMPS to 1 to confirm that
       the board is configured in direct SMPS

   if a dead lock is faced due to a mismatch between the HW board setting and the FW setting (LDO,SMPS)
   user can recover the board by the following :
     - Power off the board
     - Remove R143 10K resistor and mount it on R144 position
     - This will change the boot pin to 1 instead of 0 and thus the device boot address will
        be changed to boot address 1 making the bootloader starting instead of the FW in the flash
       (FW that is setting a wrong LDO/SMPS config versus the HW board config)

     - Power on the board and connect using STM32CubeProgrammer
     - Erase the user fLash
     - Power off the board and revert the R143/R144 change (R144 not mounted, R143 mounted)
     - The board is now recovered and can proceed normally.

*/
#define BOARD_HW_CONFIG_IS_LDO         0  /* Set to 1 : to confirm that board HW config is LDO, 0 if not */
#define BOARD_HW_CONFIG_IS_DIRECT_SMPS 1  /* Set to 1 : to confirm that board HW config is Direct SMPS, 0 if not */

#endif /* __MAIN_COMMON_H */
