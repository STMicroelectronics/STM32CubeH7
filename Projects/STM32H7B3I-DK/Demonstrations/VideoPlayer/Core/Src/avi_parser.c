/**
******************************************************************************
* @file    avi_parser.c
* @author  MCD Application Team
* @brief   avi file parser based on the libriff api
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
#include "avi_parser.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t video_chunks = 0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
* @brief  Read riff headers
* @param  avi_parser: pointer to avi_parser_t structure that contains avi 
* parser information
* @retval AVI parser error
*/
static AVI_Status_t read_riff_headers(avi_parser_t *avi_parser)
{
  struct riff_levelStackE *ls;
  riff_handle *rh = avi_parser->parser.audio_riff;
  
  int ret = riff_seekLevelSub(rh);
  if (ret != RIFF_ERROR_NONE)
    return AVI_RIFF_ERR;
  
  ret = riff_seekInChunk(rh, 0);
  if (ret != RIFF_ERROR_NONE)
    return AVI_RIFF_ERR;
  
  ret = riff_readInChunk(rh, &(avi_parser->parser.avi_header), rh->c_size);
  if (ret == 0)
    return AVI_RIFF_ERR;
  
  ret = riff_seekNextChunk(rh);
  if (ret != RIFF_ERROR_NONE)
    return AVI_RIFF_ERR;
  
  ret = riff_seekLevelSub(rh);
  if (ret != RIFF_ERROR_NONE)
    return AVI_RIFF_ERR;
  
  ret = riff_seekInChunk(rh, 0);
  if (ret != RIFF_ERROR_NONE)
    return AVI_RIFF_ERR;
  
  ret = riff_readInChunk(rh, &(avi_parser->parser.stream_header), rh->c_size);
  if (ret == 0)
    return AVI_RIFF_ERR;
  
  ret = riff_rewind(rh);
  if (ret != RIFF_ERROR_NONE)
    return AVI_RIFF_ERR;
  
  
  ret = riff_seekNextChunk(rh);
  /* point to the "movi" chunk to be ready to read data chunks on both audio_riff and video_riff */
  
  while(ret == RIFF_ERROR_NONE)
  {
    ret = riff_seekNextChunk(rh);
    if (strcmp(rh->c_id, "LIST") == 0)
    {
      ret = riff_seekLevelSub(rh);
      ls = rh->ls + rh->ls_level - 1;
      
      if (strcmp((char *)(ls->c_type), "movi") == 0)
        break;
    }
  }
  
  if (ret != RIFF_ERROR_NONE)
    return AVI_RIFF_ERR;
  
  return AVI_OK;
}

/**
* @brief  Allocates the AVI parser structure
* @param  avi_parser pointer to avi_parser_t structure that contains avi 
*         parser information
* @param  filename avi file name
* @retval AVI parser error
*/
static AVI_Status_t avi_parser_alloc(avi_parser_t *avi_parser, char *filename)
{
  uint32_t file_size;
  
  if (avi_parser->parser.audio_riff == NULL)
  {
    avi_parser->parser.audio_riff = riff_handleAllocate();
    if (avi_parser->parser.audio_riff == NULL)
      return AVI_RIFF_ERR;
  }
  if (f_open(&(avi_parser->parser.audio_file_handle), filename, FA_READ | FA_OPEN_EXISTING) != FR_OK)
  {
    return AVI_RIFF_ERR;
  }
  
  avi_parser->parser.current_audio_chunk = 0;
  avi_parser->parser.current_total_video_chunk = 0;
  
  file_size = f_size(&(avi_parser->parser.audio_file_handle));
  
  if(riff_open_file(avi_parser->parser.audio_riff, &(avi_parser->parser.audio_file_handle), file_size) != RIFF_ERROR_NONE)
  {
    return AVI_RIFF_ERR;
  }
  
  if (read_riff_headers(avi_parser) != AVI_OK)
    return AVI_RIFF_ERR;
  
  return AVI_OK;
  
}

/**
* @brief  Allocates the AVI parser structure
* @param  avi_parser pointer to avi_parser_t structure that contains avi 
*         parser information
* @retval AVI parser error
*/
static AVI_Status_t avi_parser_dealloc(avi_parser_t *avi_parser)
{
  f_close(&(avi_parser->parser.audio_file_handle));
  riff_handleFree(avi_parser->parser.audio_riff);
  avi_parser->parser.audio_riff = NULL;
  
  
  return AVI_OK;
}

/**
* @brief  reads a data chunk from the file stream into buffer
* @param  avi_parser pointer to avi_parser_t structure that contains avi 
*         parser information
* @param  buffer pointer to output buffer
* @param  type: chunk type audio or video
* @retval AVI parser error
*/
uint32_t avi_parser_get_data(avi_parser_t *avi_parser, uint8_t *buffer, ChunkType type)
{
  uint32_t bytes_read = 0;
  int ret = RIFF_ERROR_NONE;
  
  riff_handle *rh = ((type == AUDIO_CHUNK) ? avi_parser->parser.audio_riff
                     : avi_parser->parser.video_riff);
  
  if( avi_parser->parser.current_audio_chunk == 0)
  {
    if (CHUNK_TYPE(rh, type))
    {
      riff_seekInChunk(rh, 0);
      bytes_read = riff_readInChunk(rh, buffer, rh->c_size);
      
      if(type == AUDIO_CHUNK)
      {
        avi_parser->parser.current_audio_chunk++;
        
      }
      
      else
      {
        avi_parser->parser.current_total_video_chunk++;
      }
      
      return bytes_read > 0 ? bytes_read : 0;
    }
  }
  ret = riff_seekNextChunk(rh);
  
  /* loop until finding next Chunk with required type or reach e/o file */
  while ((ret == RIFF_ERROR_NONE))
  {
    if (CHUNK_TYPE(rh, type))
    {
      break;
    }
    else
    {
      avi_parser->parser.current_total_video_chunk++;
    }
    ret = riff_seekNextChunk(rh);
  }
  
  if (ret == RIFF_ERROR_NONE)
  {
    riff_seekInChunk(rh, 0);
    bytes_read = riff_readInChunk(rh, buffer, rh->c_size);
  }
  
  if (bytes_read != rh->c_size)
  {
    bytes_read = 0; /* Error occurred */
  }
  
  if (type == AUDIO_CHUNK)
  {
    avi_parser->parser.current_audio_chunk++;
  }
  else
  {
    avi_parser->parser.current_video_chunk++;
    avi_parser->parser.current_total_video_chunk++;
  }
  
  return bytes_read;
}

/**
* @brief  Initializes the avi parser:
*         allocates the riff handles
*         assign the handle inner functions.
* @param  avi_parser pointer to avi_parser_t structure that contains avi 
*         parser information
* @retval AVI parser error
*/
AVI_Status_t init_avi_parser(avi_parser_t *avi_parser)
{
  avi_parser->parser.audio_riff = riff_handleAllocate();
  
  if (avi_parser->parser.audio_riff == NULL)
  {
    return AVI_RIFF_ERR;
  }
  
  avi_parser->parser.current_audio_chunk = 0;
  avi_parser->parser.current_total_video_chunk = 0;
  
  avi_parser->init  = avi_parser_alloc;
  avi_parser->deinit = avi_parser_dealloc;
  avi_parser->get_data = avi_parser_get_data;
  
  return AVI_OK;
}
