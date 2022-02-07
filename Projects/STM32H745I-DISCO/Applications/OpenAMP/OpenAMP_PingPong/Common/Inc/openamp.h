/**
  ******************************************************************************
  * @file    OpenAMP/OpenAMP_PingPong/Common/Inc/openamp.h
  * @author  MCD Application Team
  * @brief   Header file for openamp module
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
#ifndef __openamp_H
#define __openamp_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "openamp/open_amp.h"
#include "openamp_conf.h"


#define OPENAMP_send  rpmsg_send
#define OPENAMP_destroy_ept rpmsg_destroy_ept

/* Initialize the openamp framework*/
int MX_OPENAMP_Init(int RPMsgRole, rpmsg_ns_bind_cb ns_bind_cb);

/* Deinitialize the openamp framework*/
void OPENAMP_DeInit(void);

/* Initialize the endpoint struct*/
void OPENAMP_init_ept(struct rpmsg_endpoint *ept);

/* Create and register the endpoint */
int OPENAMP_create_endpoint(struct rpmsg_endpoint *ept, const char *name,
                            uint32_t dest, rpmsg_ept_cb cb,
                            rpmsg_ns_unbind_cb unbind_cb);

/* Check for new rpmsg reception */
void OPENAMP_check_for_message(void);

/* Wait loop on endpoint ready ( message dest address is know)*/
void OPENAMP_Wait_EndPointready(struct rpmsg_endpoint *rp_ept);

#ifdef __cplusplus
}
#endif
#endif /*__openamp_H */

