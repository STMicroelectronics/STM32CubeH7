/**
******************************************************************************
  * @file    : USB_Host/MSC_Standalone/USB_Host/App/explorer.h
  * @author  : MCD Application Team
  * @brief   : Header for explorer.c module
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
#ifndef __EXPLORER_H
#define __EXPLORER_H

/* Includes ------------------------------------------------------------------*/
#include "usb_host.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
FRESULT msc_explore_disk(char *path, uint8_t recu_level);

#endif /* __EXPLORER_H */

