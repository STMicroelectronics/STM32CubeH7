/**
  ******************************************************************************
  * @file    net_custom.h
  * @author  MCD Application Team
  * @brief   Header for the network custom class.
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
#ifndef NET_CUSTOM_H
#define NET_CUSTOM_H

#ifdef __cplusplus
extern "C" {
#endif

/* network extension for Custom class interface */
int32_t net_custom_func(net_if_handle_t *pnetif, ...);

#ifdef __cplusplus
}
#endif
#endif /* NET_CUSTOM_H */
