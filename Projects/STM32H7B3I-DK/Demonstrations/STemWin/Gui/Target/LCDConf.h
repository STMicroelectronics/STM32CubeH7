/**
  ******************************************************************************
  * @file    lcdconf.h
  * @author  MCD Application Team
  * @version V1.4.6
  * @date    04-November-2016
  * @brief   Header for LCDConf file
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
#ifndef LCDCONF_H
#define LCDCONF_H

#include "GUI.h"
#include "GUIDRV_Lin.h"

#include <stdint.h>

#if !defined(WIN32)
typedef struct
{
  int32_t      address;
  int32_t      pending_buffer;
  int32_t      buffer_index;
  int32_t      xSize;
  int32_t      ySize;
  int32_t      BytesPerPixel;
  LCD_API_COLOR_CONV   *pColorConvAPI;
}
LCD_LayerPropTypedef;
#endif /* WIN32 */

#endif /* LCDCONF_H */

