/**
  ******************************************************************************
  * @file    USB_Device/AUDIO_Standalone/Src/usbd_audio_if.c
  * @author  MCD Application Team
  * @brief   USB Device Audio interface file.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------ */
#include "usbd_audio_if.h"
#include "stm32h743i_eval_audio.h"

/* Private typedef ----------------------------------------------------------- */
/* Private define ------------------------------------------------------------ */
/* Private macro ------------------------------------------------------------- */
/* Private function prototypes ----------------------------------------------- */
static int8_t Audio_Init(uint32_t AudioFreq, uint32_t Volume, uint32_t options);
static int8_t Audio_DeInit(uint32_t options);
static int8_t Audio_PlaybackCmd(uint8_t * pbuf, uint32_t size, uint8_t cmd);
static int8_t Audio_VolumeCtl(uint8_t vol);
static int8_t Audio_MuteCtl(uint8_t cmd);
static int8_t Audio_PeriodicTC(uint8_t *pbuf, uint32_t size, uint8_t cmd);
static int8_t Audio_GetState(void);

/* Private variables --------------------------------------------------------- */
extern USBD_HandleTypeDef USBD_Device;
USBD_AUDIO_ItfTypeDef USBD_AUDIO_fops = {
  Audio_Init,
  Audio_DeInit,
  Audio_PlaybackCmd,
  Audio_VolumeCtl,
  Audio_MuteCtl,
  Audio_PeriodicTC,
  Audio_GetState,
};

BSP_AUDIO_Init_t audio_init;
/* Private functions --------------------------------------------------------- */

/**
  * @brief  Initializes the AUDIO media low layer.
  * @param  AudioFreq: Audio frequency used to play the audio stream.
  * @param  Volume: Initial volume level (from 0 (Mute) to 100 (Max))
  * @param  options: Reserved for future use
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t Audio_Init(uint32_t AudioFreq, uint32_t Volume, uint32_t options)
{
  audio_init.Device = AUDIO_OUT_DEVICE_HEADPHONE;
  audio_init.ChannelsNbr = 2;
  audio_init.SampleRate = AudioFreq;
  audio_init.BitsPerSample = AUDIO_RESOLUTION_16B;
  audio_init.Volume = Volume;
  BSP_AUDIO_OUT_Init(0, &audio_init);

  return 0;
}

/**
  * @brief  De-Initializes the AUDIO media low layer.
  * @param  options: Reserved for future use
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t Audio_DeInit(uint32_t options)
{
  BSP_AUDIO_OUT_Stop(CODEC_PDWN_SW);
  return 0;
}

/**
  * @brief  Handles AUDIO command.
  * @param  pbuf: Pointer to buffer of data to be sent
  * @param  size: Number of data to be sent (in bytes)
  * @param  cmd: Command opcode
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t Audio_PlaybackCmd(uint8_t * pbuf, uint32_t size, uint8_t cmd)
{
  switch (cmd)
  {
  case AUDIO_CMD_PLAY:
  case AUDIO_CMD_START:
    BSP_AUDIO_OUT_Play(0, (uint8_t *) pbuf, 2 * size);
    break;

  default:
    break;
  }
  return 0;
}

/**
  * @brief  Controls AUDIO Volume.
  * @param  vol: Volume level (0..100)
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t Audio_VolumeCtl(uint8_t vol)
{
  BSP_AUDIO_OUT_SetVolume(0, vol);
  return 0;
}

/**
  * @brief  Controls AUDIO Mute.
  * @param  cmd: Command opcode
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t Audio_MuteCtl(uint8_t cmd)
{
  BSP_AUDIO_OUT_Mute(0);
  return 0;
}

/**
  * @brief  Audio_PeriodicTC
  * @param  cmd: Command opcode
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t Audio_PeriodicTC(uint8_t *pbuf, uint32_t size, uint8_t cmd)
{
  UNUSED(pbuf);
  UNUSED(size);
  UNUSED(cmd);

  return (0);
}

/**
  * @brief  Gets AUDIO State.
  * @param  None
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t Audio_GetState(void)
{
  return 0;
}

/**
  * @brief  Manages the DMA full Transfer complete event.
  * @param  None
  * @retval None
  */
void BSP_AUDIO_OUT_TransferComplete_CallBack(uint32_t Instance)
{
  USBD_AUDIO_Sync(&USBD_Device, AUDIO_OFFSET_FULL);
}

/**
  * @brief  Manages the DMA Half Transfer complete event.
  * @param  None
  * @retval None
  */
void BSP_AUDIO_OUT_HalfTransfer_CallBack(uint32_t Instance)
{
  USBD_AUDIO_Sync(&USBD_Device, AUDIO_OFFSET_HALF);
}

