/**
  ******************************************************************************
  * @file    audio_player_lp.h
  * @author  MCD Application Team
  * @brief   header of audio player low power application file
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
#ifndef __AUDIO_PLAYER_LP_H
#define __AUDIO_PLAYER_LP_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "player.h"
  
/* Exported constants --------------------------------------------------------*/
#define AUDIOLP_ENTER_LPMODE 0x01
#define AUDIOLP_EXIT_LPMODE 0x02
  
/* Exported types ------------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void AUDIOLPModeThread(void const * argument);

#ifdef __cplusplus
}
#endif

#endif /*__AUDIO_PLAYER_APP_H */

