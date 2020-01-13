/**
  ******************************************************************************
  * @file    audio_conf.h
  * @author  MCD Application Team
  * @brief   Audio processing configuration file.
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
#ifndef __AUDIO_CONF_H_
#define __AUDIO_CONF_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

/* Exported types ------------------------------------------------------------*/

/* Callback functions type*/
typedef uint32_t (fnReadCallback_TypeDef)(
                                          void *       pCompressedData,             /* [OUT] Pointer to buffer to fill with coded MP3 data */
                                          uint32_t     nDataSizeInBytes,            /* Buffer size in Bytes */
                                          void *       pUser                        /* User pointer: can be used to pass additional parameters */
                                            );

typedef void (fnXFerCpltCallback_TypeDef)( uint8_t Direction,
                                          uint8_t** pbuf,
                                          uint32_t* pSize);

typedef uint32_t (fnSetPositionCallback_TypeDef)( uint32_t Pos);

/* Exported variables --------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Audio OUT Instance 0:SAI, 1:SPI6/I2S instance */
#define AUDIO_OUT_INSTANCE             1

/*-- Specify "malloc" function to be used by the Player to allocate memory region --*/
#ifndef PLAYER_MALLOC
#define PLAYER_MALLOC(x)                pvPortMalloc(x)
#endif /* PLAYER_MALLOC */

/*-- Specify "free" function to be used by Player to release, previously, allocated memory region --*/
#ifndef PLAYER_FREE
#define PLAYER_FREE(x)                  vPortFree(x)
#endif /* PLAYER_FREE */

/* Enable Dcache clean */
#define   CLEAN_DCACHE    1 

/* Enable audio low power mode feature */
#define AUDIO_LOWPOWER_SUPPORT    1  

#define AUDIO_THREAD_STACK_SIZE   (8 * configMINIMAL_STACK_SIZE)

/*-- Audio Codecs --*/
#define __WAV_DECODER__
#define __MP3_DECODER__


/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
#endif  /* __AUDIO_CONF_H_ */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
