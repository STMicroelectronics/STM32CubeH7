/**
  ******************************************************************************
  * @file    mailbox_hsem_if.h
  * @author  MCD Application Team
  * @brief   header for mailbox_hsem_if.c module
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
#ifndef MAILBOX_HSEM_IF_H_
#define MAILBOX_HSEM_IF_H_

/* USER CODE BEGIN firstSection */
/* can be used to modify / undefine following code or add new definitions */
/* USER CODE END firstSection */

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define HSEM_ID_0           0 /* CM7 to CM4 Notification */
#define HSEM_ID_1           1 /* CM4 to CM7 Notification */

/* Exported functions ------------------------------------------------------- */
int MAILBOX_Notify(void *priv, uint32_t id);
int MAILBOX_Init(void);
int MAILBOX_Poll(struct virtio_device *vdev);

#endif /* MAILBOX_HSEM_IF_H_ */
