/**
  ******************************************************************************
  * @file    main.h 
  * @author  MCD Application Team
  * @brief   Header for main.c file
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics International N.V. 
  * All rights reserved.</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
