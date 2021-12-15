/**
  ******************************************************************************
  * @file    main.h 
  * @author  MCD Application Team
  * @brief   Header for main.c file
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
 extern "C" {
#endif
   
/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"
#include "stm32h7xx_it.h"

/* DISCO includes component */
#include "stm32h750b_discovery.h"
#include "stm32h750b_discovery_sdram.h"
#include "stm32h750b_discovery_ts.h"
#include "stm32h750b_discovery_audio.h"
#include "stm32h750b_discovery_qspi.h"
                 
/* FatFs includes components */
#include "ff_gen_drv.h"
#include <cmsis_os.h>

   
/* GUI includes components */
#include "GUI.h"
#include "DIALOG.h"
#include "ST_GUI_Addons.h"

 /* Kernel includes components */
#include "k_module.h"
#include "storage.h"
#include "k_rtc.h"
#include "k_bsp.h"

/* utilities and standard includes components */   
#include "cpu_utils.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

/* Exported types ------------------------------------------------------------*/
typedef enum module_id_e
{
  MODULE_MAIN_MENU        = 0
, MODULE_AUDIO_PLAYER     /* 1 */
, MODULE_VIDEO_PLAYER     /* 2 */
, MODULE_GAME             /* 3 */
, MODULE_ANALOG_CLOCK     /* 4 */
, MODULE_GRAPHIC_EFFECTS  /* 5 */
, MODULE_COUNT
} module_id_t;

typedef struct Bitmap_Res_s
{
  uint16_t       x;
  uint16_t       y;
} Bitmap_Res_t;

typedef struct Demo_Header_s
{
  uint32_t       demo_id;
  const uint32_t * const  icon_addr;
  Bitmap_Res_t   icon_res;
  const uint32_t * const  logo_addr;
  Bitmap_Res_t   logo_res;
  uint32_t       sw_flags;
  uint32_t       reserved[2];
  unsigned char  demo_descr[32];
} Demo_Header_t;

/* Exported variables --------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define FB_BYTES_PER_PIXEL        2
#define SW_FLAG_AUTO_DEMO         (1L << 0)

#define AUTO_DEMO_TIMEOUT_0       20
#define AUTO_DEMO_TIMEOUT_1        5
#define AUTO_DEMO_STEP_TIMEOUT     5
#define BKP_REG_PWR_CFG           (RTC->BKP28R)
#define BKP_REG_SW_CFG            (RTC->BKP27R)
#define BKP_REG_SUBDEMO_ADDRESS   (RTC->BKP26R)
#define BKP_REG_CALIB_DR0         (RTC->BKP25R)
#define BKP_REG_CALIB_DR1         (RTC->BKP24R)

/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */ 
void Error_Handler( void );
void BSP_ErrorNotify(void);

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

