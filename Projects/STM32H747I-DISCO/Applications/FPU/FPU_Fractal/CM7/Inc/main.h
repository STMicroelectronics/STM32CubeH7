/**
  ******************************************************************************
  * @file    FPU/FPU_Fractal/CM7/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module for Cortex-M7.
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
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32h747i_discovery.h"
#include "stm32h747i_discovery_lcd.h"
#include "stm32h747i_discovery_sdram.h"
#include "stm32h747i_discovery_bus.h"
#include "stm32h747i_discovery_ts.h"
#include "stm32_lcd.h"

#include <stdint.h>
#include <string.h>
#include <stdio.h>

/* Set this define to 1 to OverClocking the system clock to 480MHz*/
#define USE_VOS0_480MHZ_OVERCLOCK 0

/* 
   !!!Attention!!! 

   Over clocking the system clock to 480MHz is only available with LDO PWR configuration
    by default The STM32H747I-DISCO comes with DIRECT_SMPS configuration
    in order Over clock the system clock to 480MHz to the disco board must modified 
    to change the PWR path to LDO instead of DIRECT_SMPS

   to do so please change the following solder bridges :
   - LDO config : 
        Mount   : SB1, SB12, SB49
        Removed : SB2, SB11, SB19, SB46, SB48

   - Direct SMPS config : 
        Mount   : SB2, SB11, SB19, SB46, SB48 
        Removed : SB1, SB12, SB49

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
     - Remove R196 10K resistor and mount it on R192 position
     - This will change the boot pin to 1 instead of 0 and thus the device boot address will
        be changed to boot address 1 making the bootloader starting instead of the FW in the flash
       (FW that is setting a wrong LDO/SMPS config versus the HW board config)

     - Power on the board and connect using STM32CubeProgrammer
     - Erase the user FLash
     - Power off the board and revert the R196/R192 change (R192 not mounted, R196 mounted)
     - The board is now recovered and can proceed normally.     
          
*/
#define BOARD_HW_CONFIG_IS_LDO         0  /* Set to 1 : to confirm that board HW config is LDO, 0 if not */
#define BOARD_HW_CONFIG_IS_DIRECT_SMPS 1  /* Set to 1 : to confirm that board HW config is Direct SMPS, 0 if not */

#define ITERATION       ((uint32_t)256)
#define FRACTAL_ZOOM    ((uint16_t)200)
#define  REAL_CONSTANT  (0.285f)
#define  IMG_CONSTANT  (0.01f)

#define ARGB8888_BYTES_PER_PIXEL 4

#define LCD_FRAME_BUFFER  ((uint32_t)0xD0000000)

#define FRACTAL_FRAME_BUFFER  ((uint32_t)0xD0200000)

#define LCD_X_SIZE ((uint32_t) 800)
#define LCD_Y_SIZE ((uint32_t) 480)

#if (__FPU_USED == 1)
#define SCORE_FPU_MODE                "FPU On"
#else
#define SCORE_FPU_MODE                "FPU Off"
#endif  /* __FPU_USED */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

