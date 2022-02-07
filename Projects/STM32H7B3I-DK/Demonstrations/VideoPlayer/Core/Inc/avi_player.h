/**
******************************************************************************
* @file    avi_player.h
* @author  MCD Application Team
* @brief   header of avi_player.c file
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
#ifndef __AVI_PLAYER_H
#define __AVI_PLAYER_H
#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "avi_parser.h"
#include "avi_player_conf.h"
/* Exported types ------------------------------------------------------------*/
typedef enum {
  AUDIO_READY_EVT = 0,
  AUDIO_PLAYING_EVT,
  AUDIO_STOPPED_EVT,
  AUDIO_PAUSED_EVT,
  AUDIO_RESUMED_EVT,
  AUDIO_RESET_EVT,
  AUDIO_BUFFER_FULL_READ_EVT,
  AUDIO_BUFFER_HALF_READ_EVT,
  AUDIO_EXIT_EVT,

  VIDEO_READY_EVT,
  VIDEO_PLAYING_EVT,
  VIDEO_STOPPED_EVT,
  VIDEO_PAUSED_EVT,
  VIDEO_RESUMED_EVT,
  VIDEO_RESET_EVT,
  VIDEO_BUFFER_FULL_READ_EVT,
  VIDEO_EXIT_EVT,
}player_event_t;

typedef enum {
  PLAYER_INITIALIZED = 0,
  PLAYER_READY,
  PLAYER_STARTED,
  PLAYER_STOPPED,
  PLAYER_PAUSED,
  PLAYER_RESUMED,
  PLAYER_EOF_REACHED,
  PLAYER_NOT_READY,
  PLAYER_EXIT,
} player_state_t;

typedef enum {
  PLAYER_NO_ERROR = 0,
  PLAYER_INIT_ERROR,
  PLAYER_DATA_ERROR,
  PLAYER_PLAY_ERROR,
  PLAYER_AUDIO_ERROR,
  PLAYER_VIDEO_ERROR,

} player_res_t;

typedef struct audio_handle_s audio_handle_t;
typedef struct video_handle_s video_handle_t;
typedef struct media_player_handle_s  media_player_handle_t;

struct audio_handle_s {
  BSP_AUDIO_Init_t     AudioPlayInit;
  uint8_t             *audio_play_buffer;
  uint32_t             current_chunk;
  uint32_t             AudioInstance;
  uint32_t             volume_level;
};

struct media_player_handle_s {
  avi_parser_t      *parser;
  audio_handle_t *audio_handle;
  char media_list [MAX_FILE_NBR][MAX_NAME_SIZE];
  uint32_t current_idx;
  uint32_t  volume_level;
  player_state_t  player_state;
  player_res_t   (* init)             (media_player_handle_t *player);
  player_res_t   (* update_play_list) (media_player_handle_t *player, const char *media_file);
  player_res_t   (* set_avi_file)     (media_player_handle_t *player, uint32_t id);
  player_res_t   (* play)             (media_player_handle_t *player);
  player_res_t   (* pause)            (media_player_handle_t *player);
  player_state_t (* get_state)        (media_player_handle_t *player);
  uint32_t       (* get_elapsed_time) (media_player_handle_t *player);
  player_res_t (*set_volume)(media_player_handle_t *player, uint32_t volume);
};
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
player_res_t media_player_new(media_player_handle_t *player);

#ifdef __cplusplus
}
#endif
#endif /*__AVI_PLAYER_H*/
