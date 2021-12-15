/**
  ******************************************************************************
  * @file    audio_if.c
  * @author  MCD Application Team
  * @brief   Audio common interface
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
  
/* Includes ------------------------------------------------------------------*/

#include "audio_if.h"

/** @addtogroup AUDIO_PLAYER_MODULE
  * @{
  */

/** @defgroup AUDIO_APPLICATION
  * @brief audio application routines
  * @{
  */


/* External variables --------------------------------------------------------*/
static AUDIO_IFTypeDef  AudioIf;

#if defined ( __ICCARM__ )
#pragma data_alignment=32
#pragma location="audio_buffers"
#elif defined(__CC_ARM)
__attribute__((section(".audio_buffers"), zero_init)) __attribute__ ((aligned (32)))
#elif defined(__GNUC__)
__attribute__((section(".audio_buffers"))) __attribute__ ((aligned (32)))
#endif
AUDIO_ProcessTypdef haudio;


/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

 
 /**
  * @brief  Register Audio callbacks
  * @param  callbacks
  * @retval None
  */
void AUDIO_IF_RegisterCallbacks(pFunc  tc_cb, 
                                pFunc  ht_cb, 
                                pFunc  err_cb)
{
    AudioIf.TransferComplete_CallBack = tc_cb;
    AudioIf.HalfTransfer_CallBack = ht_cb; 
    AudioIf.Error_CallBack = err_cb;   
}
/**
  * @brief  Manages the DMA Transfer complete interrupt.
  * @param  None
  * @retval None
  */
void BSP_AUDIO_OUT_TransferComplete_CallBack(uint32_t Instance)
{
  if(AudioIf.TransferComplete_CallBack)
  {
    AudioIf.TransferComplete_CallBack();
  }
}

/**
  * @brief  Manages the DMA Half Transfer complete interrupt.
  * @param  None
  * @retval None
  */
void BSP_AUDIO_OUT_HalfTransfer_CallBack(uint32_t Instance)
{ 
  if (AudioIf.HalfTransfer_CallBack )
  {
    AudioIf.HalfTransfer_CallBack();
  }
}

/**
  * @brief  Manages the DMA FIFO error interrupt.
  * @param  None
  * @retval None
  */
void BSP_AUDIO_OUT_Error_CallBack(uint32_t Instance)
{
  if(AudioIf.Error_CallBack)
  {
    AudioIf.Error_CallBack();
  }
}
