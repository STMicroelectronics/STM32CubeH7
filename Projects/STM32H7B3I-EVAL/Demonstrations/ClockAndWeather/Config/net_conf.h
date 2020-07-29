/**
  ******************************************************************************
  * @file    net_conf.h
  * @author  MCD Application Team
  * @brief   Configures the network socket APIs.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
