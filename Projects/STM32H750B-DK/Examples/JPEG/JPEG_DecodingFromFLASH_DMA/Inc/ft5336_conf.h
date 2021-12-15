/**
  ******************************************************************************
  * @file    ft5336_conf.h
  * @author  MCD Application Team
  * @brief   This file contains specific configuration for the
  *          ft5336.c that can be modified by user.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2015 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef FT5336_CONF_H
#define FT5336_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* Macros --------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define FT5336_AUTO_CALIBRATION_ENABLED      0U
#define FT5336_MAX_X_LENGTH                  480U
#define FT5336_MAX_Y_LENGTH                  272U

#ifdef __cplusplus
}
#endif
#endif /* FT5336_CONF_H */

