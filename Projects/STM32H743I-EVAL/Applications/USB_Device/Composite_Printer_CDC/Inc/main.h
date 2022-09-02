/**
  ******************************************************************************
  * @file    main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                     www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_cdc.h"
#include "usbd_printer.h"
#include "usbd_printer_if.h"
#include "usbd_cdc_interface.h"
#include "stm32h743i_eval.h"
#include "stm32h743i_eval_io.h"
#include "usbd_cdc_interface.h"
#include <string.h>
/* FatFs includes component */
#include "ff_gen_drv.h"
#include "sd_diskio.h"
#include "usbd_composite_builder.h"
#ifdef USE_USB_FS
#ifndef PRNT_APP_RX_DATA_SIZE
#define PRNT_APP_RX_DATA_SIZE                               64U
#endif /* APP_RX_DATA_SIZE */

#ifndef PRNT_DATA_OUT_PACKET_SIZE
#define PRNT_DATA_OUT_PACKET_SIZE                      64U
#endif /* PRNT_DATA_OUT_PACKET_SIZE*/

#else /* USE_USB_HS */
#ifndef PRNT_APP_RX_DATA_SIZE
#define PRNT_APP_RX_DATA_SIZE                               512U
#endif /* APP_RX_DATA_SIZE */

#ifndef PRNT_DATA_OUT_PACKET_SIZE
#define PRNT_DATA_OUT_PACKET_SIZE                      512U
#endif /* PRNT_DATA_OUT_PACKET_SIZE */
#endif /* USE_USB_FS */

/* Exported types ------------------------------------------------------------*/
typedef struct
{
  FATFS SDFatFs;                       /* File system object for SD card logical drive */
  FIL MyFile;
  FRESULT res;                         /* FatFs function common result code */
  char SDPath[4];                      /* SD card logical drive path */
  uint32_t byteswritten;               /* File write/read counts */
}
SD_CardTypeDef;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Toggle_Leds(void);

static void Error_Handler(void)
{
  /* User may add here some code to deal with this error */
  for (;;)
  {
  }
}
#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
