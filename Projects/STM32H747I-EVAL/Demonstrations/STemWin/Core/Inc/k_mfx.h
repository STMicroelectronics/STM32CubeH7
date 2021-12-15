/**
  ******************************************************************************
  * @file    k_mfx.h
  * @author  MCD Application Team
  * @brief   Header for k_mfx.c file
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
#ifndef __K_MFX_H
#define __K_MFX_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/

/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void MfxInit(void);
void MfxDetectInt(void);

#ifdef __cplusplus
}
#endif

#endif /*__K_MFX_H */

