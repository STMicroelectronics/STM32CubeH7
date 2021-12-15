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

/* EVAL includes component */
#include "stm32h743i_eval.h"
#include "stm32h743i_eval_sdram.h"
//#include "stm32h743i_eval_lcd.h"
#include "stm32h743i_eval_ts.h"
#include "stm32h743i_eval_io.h"
#include "stm32h743i_eval_sd.h"
#include "stm32h743i_eval_audio.h"
#include "stm32h743i_eval_qspi.h"
                 
/* FatFs includes components */
#include "ff_gen_drv.h"
#include "sd_diskio.h"
#include "cmsis_os.h"

   
/* GUI includes components */
#include "GUI.h"
#include "DIALOG.h"
#include "ST_GUI_Addons.h"

 /* Kernel includes components */
#include "k_module.h"
#include "k_storage.h"
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
/* Exported variables --------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */ 
   

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

