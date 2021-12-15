/**
  ******************************************************************************
  * @file    main.h
  * @author  MCD Application Team
  * @brief   Header for main.c file
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

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#ifndef WIN32
#include "stm32h7xx_hal.h"
#include "stm32h7b3i_discovery.h"
#include "stm32h7b3i_discovery_audio.h"
#include "stm32h7b3i_discovery_sdram.h"
#include "stm32h7b3i_discovery_ospi.h"
#include "stm32h7b3i_discovery_ts.h"
   
#include "cmsis_os.h"

/* FatFs includes component */
#include "ff_gen_drv.h"

#endif /* !WIN32 */

#ifndef WIN32
/* Kernel includes components */
#include "storage.h"
//#if defined(USE_TS_CALIBRATION)
//#include "calibration.h"
//#endif /* USE_TS_CALIBRATION */
//#include "MenuLauncher.h"
#endif /* !WIN32 */

/* standard includes components */
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* Exported types ------------------------------------------------------------*/
typedef struct Bitmap_Res_s
{
  uint16_t       x;
  uint16_t       y;
} Bitmap_Res_t;

typedef struct Demo_Header_s
{
  uint32_t       demo_id;
  uint32_t       icon_addr;
  Bitmap_Res_t   icon_res;
  uint32_t       logo_addr;
  Bitmap_Res_t   logo_res;
  uint32_t       sw_flags;
  uint32_t       reserved[2];
  unsigned char  demo_descr[32];
} Demo_Header_t;

typedef enum Demo_Id_e
{
  DEMO_ID_0,
  DEMO_ID_1,
  DEMO_ID_2,
  DEMO_ID_3,
  DEMO_ID_4,
  DEMO_ID_INVALID,
} Demo_Id_t;

typedef enum AutoDemoEvent_e
{
  AUTO_DEMO_DISABLE,
  AUTO_DEMO_ENABLE,
  AUTO_DEMO_RESET
} AutoDemoEvent_t;

/* Exported variables --------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define SW_FLAG_AUTO_DEMO         (1L << 0)
#define FB_BYTES_PER_PIXEL        4         /* 4 - 3 - 2 */

/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Error_Handler(void);
void BSP_ErrorHandler(void);

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

