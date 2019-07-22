/**
  ******************************************************************************
  * @file    OpenAMP/OpenAMP_RTOS_PingPong/Common/Inc/platform_info.h
  * @author  MCD Application Team
  * @brief   This file contains the headers for the platform.
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
#ifndef PLATFORM_INFO_H_
#define PLATFORM_INFO_H_

#include "openamp/hil.h"
#include "openamp.h"
/*************************/
/* IPC Device parameters */
/*************************/


#define RPMSG_CHAN_NAME              "OpenAMP_RTOS_PingPong_demo"

#define HSEM_ID_0           0 /* CM7 to CM4 Notification */
#define HSEM_ID_1           1 /* CM4 to CM7 Notification */

#define RX_NO_MSG           0
#define RX_NEW_MSG          1

struct hil_proc *platform_create_proc(int proc_index);
struct hil_proc *platform_init(int role);
int mailbox_init(void);
#endif /* PLATFORM_INFO_H_ */
