/**
  ******************************************************************************
  * @file    net_conf.h
  * @author  MCD Application Team
  * @brief   Configures the network socket APIs.
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
#ifndef __NET_CONF_H
#define __NET_CONF_H


#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

#define NET_DBG_INFO(...)  
#define NET_DBG_ERROR(...)  
#define NET_DBG_PRINT(...)  
#define NET_ASSERT(test,s)  
#define NET_PRINT(...)  
#define NET_PRINT_WO_CR(...)   
#define NET_WARNING(...)  


#include "net_conf_template.h"
#ifdef __cplusplus
}
#endif

#endif /* __NET_CONF_H */

