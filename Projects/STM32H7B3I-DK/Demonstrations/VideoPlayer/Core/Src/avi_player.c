/**
******************************************************************************
* @file    avi_player.c
* @author  MCD Application Team
* @brief   avi player allows to decode/play  audio and video chunks
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

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include "avi_player.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static audio_handle_t audio_handle;
static avi_parser_t   media_parser;
static osThreadId audio_play_handle;
static osMessageQId audio_msg_queue;
uint32_t audio_chunks = 0;

#ifdef AUDIO_BUFFER_LOCATION
#if defined ( __ICCARM__ )
#pragma location=AUDIO_BUFFER_LOCATION
#pragma data_alignment=32
#else
__attribute__((section(".RAM_D3"))) __attribute__ ((aligned (32)))
#endif
uint8_t  AudioPlayBuffer[AUDIO_NBR_CHUNKS * AUDIO_CHUNK_SIZE];
#endif

uint32_t frame_rate;
uint32_t stream_rate;
uint32_t pause_time;
uint8_t is_resumed = 0;
uint32_t skipped_frames =  0;
osEvent  evt;
uint32_t init_done = 0;
uint32_t elapsed_time = 0;
/* Private function prototypes -----------------------------------------------*/
static void audio_play(void const *argument);

/* Private functions ---------------------------------------------------------*/

/**
* @brief  fill audio buffer with audio chunks
* @param  player: media player handler 
* @retval player error
*/
static player_res_t fill_audio_buffer(const media_player_handle_t *player)
{
  player_res_t ret = PLAYER_NO_ERROR;
  
  /* fill the audio_play_buffer with Audio chunks */
  if (player->parser->get_data(player->parser, (uint8_t *)&player->audio_handle->audio_play_buffer[0], AUDIO_CHUNK) == 0)
  {
    ret = PLAYER_DATA_ERROR;
  }
  else if (player->parser->get_data(player->parser, (uint8_t *)&player->audio_handle->audio_play_buffer[AUDIO_CHUNK_SIZE], AUDIO_CHUNK) == 0)
  {
    ret = PLAYER_DATA_ERROR;
  }
  
  if (ret == PLAYER_DATA_ERROR)
  {
    BSP_AUDIO_OUT_DeInit(AUDIO_INSTANCE);
  }
  
  return ret;
}

/**
* @brief  Deinitializes audio player
* @param  player: media player handler 
* @retval player error
*/
static player_res_t audio_deinit(const media_player_handle_t *player)
{
  
  BSP_AUDIO_OUT_Stop(player->audio_handle->AudioInstance);
  BSP_AUDIO_OUT_DeInit(player->audio_handle->AudioInstance);
  
  return PLAYER_NO_ERROR;
}

/**
* @brief  Initializes audio player
* @param  player: media player handler
* @param create_thread : to create or not audio player thread
* @retval player error
*/
static player_res_t audio_init(const media_player_handle_t *player, uint8_t create_thread)
{
  
  player_res_t ret = PLAYER_NO_ERROR;
  
  if (create_thread == 1)
  {
    osThreadDef (AudioPlayThrd, audio_play, osPriorityAboveNormal, 0, 10 * configMINIMAL_STACK_SIZE);
    audio_play_handle = osThreadCreate(osThread(AudioPlayThrd), (void *)player);
    if (audio_play_handle == NULL)
      return PLAYER_INIT_ERROR;
    
    osMessageQDef(audio_queue, 16, uint8_t);
    audio_msg_queue = osMessageCreate(osMessageQ(audio_queue), audio_play_handle);
  }
  
  player->audio_handle->audio_play_buffer = AudioPlayBuffer;
  
  player->audio_handle->AudioInstance = AUDIO_INSTANCE;
  player->audio_handle->AudioPlayInit.Device = AUDIO_OUT_DEVICE;
  player->audio_handle->AudioPlayInit.ChannelsNbr = AUDIO_NBR_CHANNELS;
  
  player->audio_handle->AudioPlayInit.SampleRate = AUDIO_FREQUENCY;
  player->audio_handle->AudioPlayInit.BitsPerSample = AUDIO_RESOLUTION_16B;
  player->audio_handle->AudioPlayInit.Volume =  player->volume_level != -1U ? player->volume_level : AUDIO_VOLUME_LEVEL;
  if(BSP_AUDIO_OUT_Init(player->audio_handle->AudioInstance, &player->audio_handle->AudioPlayInit) != BSP_ERROR_NONE)
  {
    ret = PLAYER_INIT_ERROR;
  }
  
  if (player->audio_handle->volume_level == -1U)
  {
    player->audio_handle->volume_level = AUDIO_VOLUME_LEVEL;
  }
  
  return ret;
}

/**
* @brief  audio play thread
* @param  argument: pointer that is passed to the thread function as start argument 
* @retval none
*/
static void audio_play(void const *argument)
{
  media_player_handle_t *player =(media_player_handle_t *)argument;
  
  uint8_t msg = 0;
  
  while(1)
  {
    evt = osMessageGet(audio_msg_queue, osWaitForever);
    if (evt.status == osEventMessage)
    {
      audio_chunks = player->parser->parser.current_audio_chunk;
      
      msg = (uint8_t)evt.value.v;
      
      
      if (msg == AUDIO_BUFFER_FULL_READ_EVT)
      {
        if (player->parser->get_data(player->parser, (uint8_t *)&player->audio_handle->audio_play_buffer[AUDIO_CHUNK_SIZE], AUDIO_CHUNK) == 0)
        {
          osMessagePut(audio_msg_queue, AUDIO_STOPPED_EVT, 0);
        }
      }
      
      if (msg == AUDIO_BUFFER_HALF_READ_EVT)
      {
        if (player->parser->get_data(player->parser, (uint8_t *)&player->audio_handle->audio_play_buffer[0], AUDIO_CHUNK) == 0)
        {
          osMessagePut(audio_msg_queue, AUDIO_STOPPED_EVT, 0);
        }
      }
      
      if (msg == AUDIO_PLAYING_EVT)
      {
        BSP_AUDIO_OUT_Play(player->audio_handle->AudioInstance, (uint8_t *)player->audio_handle->audio_play_buffer, AUDIO_NBR_CHUNKS*AUDIO_CHUNK_SIZE);
      }
      
      if (msg == AUDIO_STOPPED_EVT)
      {      
        /* either the actual eof is reached or an access error occurred*/
        player->player_state = PLAYER_EOF_REACHED;
        BSP_AUDIO_OUT_Stop(player->audio_handle->AudioInstance);
      }
      
      if (msg == AUDIO_PAUSED_EVT)
      {
        BSP_AUDIO_OUT_Pause(player->audio_handle->AudioInstance);
      }
      
      if (msg == AUDIO_RESUMED_EVT)
      {
        BSP_AUDIO_OUT_Resume(player->audio_handle->AudioInstance);
      }
    }
  }
}

/**
* @brief  Manages the DMA Transfer complete event.
* @param  None
* @retval None
*/

void BSP_AUDIO_OUT_TransferComplete_CallBack(uint32_t Instance)
{
  osMessagePut(audio_msg_queue, AUDIO_BUFFER_FULL_READ_EVT, 0);
}

/**
* @brief  Manages the DMA Half Transfer complete event.
* @param  None
* @retval None
*/
void BSP_AUDIO_OUT_HalfTransfer_CallBack(uint32_t Instance)
{
  osMessagePut(audio_msg_queue, AUDIO_BUFFER_HALF_READ_EVT, 0);
}

/**
* @brief  Initializes media player
* @param  player: media player handler
* @retval player error
*/
static 	player_res_t media_player_init (media_player_handle_t *player)
{
  
  player_res_t ret = PLAYER_NO_ERROR;
  
  player->player_state = PLAYER_NOT_READY;
  init_avi_parser(player->parser);
  audio_init(player, 1);
  player->player_state = PLAYER_READY;
  return ret;
}

/**
* @brief  update play list
* @param  player: media player handler
* @param  media_file: avi file
* @retval player error
*/
player_res_t media_player_update_play_list (media_player_handle_t *player, const char *media_file )
{
  player_res_t ret = PLAYER_NO_ERROR;
  
  strcpy((player->media_list[player->current_idx]), media_file);
  player->current_idx++;
  
  return ret;
}


/**
* @brief  Set new Avi file to play
* @param  player: media player handler
* @param  id: new avi file
* @retval player error
*/
player_res_t media_player_set_avi_file (media_player_handle_t *player, uint32_t id)
{
  
  if ((player->player_state != PLAYER_READY))
  {
    player->parser->deinit(player->parser);
    audio_deinit(player);
    audio_init(player, 0);
  }
  
  player->parser->init(player->parser, (char *)player->media_list[id]);
  fill_audio_buffer(player);
  
  player->player_state = PLAYER_STOPPED;
  
  return PLAYER_NO_ERROR;
}

/**
* @brief  Play avi file
* @param  player: media player handler
* @retval player error
*/
static 	player_res_t media_player_play (media_player_handle_t *player)
{
  player_res_t ret = PLAYER_NO_ERROR;
  
  if (player->player_state == PLAYER_STOPPED)
  {
    osMessagePut(audio_msg_queue, AUDIO_PLAYING_EVT , 0);
  }
  else
  {
    osMessagePut(audio_msg_queue, AUDIO_RESUMED_EVT, 0);
  }
  
  player->player_state = PLAYER_RESUMED;
  
  return ret;
}

/**
* @brief  Pause avi file
* @param  player: media player handler
* @retval player error
*/
static 	player_res_t media_player_pause (media_player_handle_t *player)
{
  player_res_t ret = PLAYER_NO_ERROR;
  
  if (player->player_state == PLAYER_RESUMED)
  {
    osMessagePut(audio_msg_queue, AUDIO_PAUSED_EVT, 0);
    player->player_state = PLAYER_PAUSED;
  }
  
  return ret;
}

/**
* @brief  Get avi player state
* @param  player: media player handler
* @retval player state
*/
static 	player_state_t media_player_get_state (media_player_handle_t *player)
{
  return player->player_state;
}

/**
* @brief  Get avi player elapsed time
* @param  player: media player handler
* @retval elapsed time
*/
static uint32_t  media_player_get_elapsed_time(media_player_handle_t *player)
{
  /*
  * since the media file duration is using in the video chunks
  * but the video decoding may reuse or skip chunks, we use the audio parser
  * to accurately count the total video chunks already found.
  */
  
  elapsed_time =(player->parser->parser.avi_header.SecPerFrame/1000) * player->parser->parser.current_total_video_chunk;
  return elapsed_time;
  
}

/**
* @brief  Set audio volume
* @param  player: media player handler
* @param  Volume: Volume level to be set in percentage from 0% to 100% (0 for 
*         Mute and 100 for Max volume level).
* @retval player error
*/
static 	player_res_t media_player_set_volume(media_player_handle_t *player, uint32_t volume)
{
  player_res_t ret = PLAYER_NO_ERROR;
  if (volume == 0)
  {
    if(BSP_AUDIO_OUT_Mute(player->audio_handle->AudioInstance) != BSP_ERROR_NONE)
      ret = PLAYER_AUDIO_ERROR;
  }
  else
  {
    if (player->volume_level == 0)
    {
      if(BSP_AUDIO_OUT_UnMute(player->audio_handle->AudioInstance) != BSP_ERROR_NONE)
      {
        return PLAYER_AUDIO_ERROR;
      }
    }
    if (BSP_AUDIO_OUT_SetVolume(player->audio_handle->AudioInstance, volume) != BSP_ERROR_NONE)
      ret = PLAYER_AUDIO_ERROR;
  }
  
  player->volume_level = volume;
  return ret;
}

/**
* @brief  Play new avi file
* @param  player: media player handler
* @retval player error
*/
player_res_t media_player_new(media_player_handle_t *player)
{
  player_res_t ret = PLAYER_NO_ERROR;
  if (init_done == 0)
  {
    player->parser            = &media_parser;
    player->audio_handle      = &audio_handle;
    
    player->init              = media_player_init;
    player->update_play_list  = media_player_update_play_list;
    
    player->set_avi_file      = media_player_set_avi_file;
    player->play              = media_player_play;
    player->pause             = media_player_pause;
    player->get_state         = media_player_get_state;
    player->get_elapsed_time  = media_player_get_elapsed_time;
    
    player->set_volume        = media_player_set_volume;
    
    player->player_state = PLAYER_INITIALIZED;
    
    memset(player->media_list, '\0', sizeof(player->media_list));
    player->volume_level = -1U;
    player->current_idx = 0;
    init_done = 1;
  }
  return ret;
}
