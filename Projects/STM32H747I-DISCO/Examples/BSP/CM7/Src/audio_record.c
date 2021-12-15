/**
  ******************************************************************************
  * @file    BSP/CM7/Src/audio_record.c
  * @author  MCD Application Team
  * @brief   This example describes how to use DFSDM HAL API to realize
  *          audio recording.
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
#include "main.h"

/** @addtogroup BSP_Examples
  * @{
  */

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
/* Audio frequency */
extern AUDIO_ErrorTypeDef AUDIO_Start(uint32_t audio_start_address, uint32_t audio_file_size);
#define AUDIO_FREQUENCY            16000U
#define AUDIO_IN_PDM_BUFFER_SIZE  (uint32_t)(128*AUDIO_FREQUENCY/16000*2)
#define AUDIO_NB_BLOCKS    ((uint32_t)4)
#define AUDIO_BLOCK_SIZE   ((uint32_t)0xFFFE)
/* Size of the recorder buffer */
#define RECORD_BUFFER_SIZE        4096
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Define record Buf at D3SRAM @0x38000000 since the BDMA for SAI4 use only this memory */
#if defined ( __CC_ARM )  /* !< ARM Compiler */
  ALIGN_32BYTES (uint16_t recordPDMBuf[AUDIO_IN_PDM_BUFFER_SIZE]) __attribute__((section(".RAM_D3")));

#elif defined ( __ICCARM__ )  /* !< ICCARM Compiler */
  #pragma location=0x38000000
ALIGN_32BYTES (uint16_t recordPDMBuf[AUDIO_IN_PDM_BUFFER_SIZE]);
#elif defined ( __GNUC__ )  /* !< GNU Compiler */
  ALIGN_32BYTES (uint16_t recordPDMBuf[AUDIO_IN_PDM_BUFFER_SIZE]) __attribute__((section(".RAM_D3")));
#endif
static uint32_t AudioFreq[9] = {8000 ,11025, 16000, 22050, 32000, 44100, 48000, 96000, 192000};
ALIGN_32BYTES (uint16_t  RecPlayback[2*RECORD_BUFFER_SIZE]);
ALIGN_32BYTES (uint16_t  PlaybackBuffer[2*RECORD_BUFFER_SIZE]);
uint32_t VolumeLevel = 80;
uint32_t  InState = 0;
uint32_t  OutState = 0;
uint32_t *AudioFreq_ptr;
uint16_t playbackBuf[RECORD_BUFFER_SIZE*2];
BSP_AUDIO_Init_t  AudioInInit;
BSP_AUDIO_Init_t  AudioOutInit;
/* Pointer to record_data */
uint32_t playbackPtr;
uint32_t AudioBufferOffset;
/* Private function prototypes -----------------------------------------------*/
typedef enum {
  BUFFER_OFFSET_NONE = 0,
  BUFFER_OFFSET_HALF,
  BUFFER_OFFSET_FULL,
}BUFFER_StateTypeDef;
/* Private functions ---------------------------------------------------------*/

/**
  * @brief Test Audio record.
  *   The main objective of this test is to check the hardware connection of the
  *   Audio peripheral.
  * @param  None
  * @retval None
*/
void AudioRecord_demo(void)
{
   uint32_t channel_nbr = 2;

  uint32_t x_size, y_size;

  BSP_LCD_GetXSize(0, &x_size);
  BSP_LCD_GetYSize(0, &y_size);

  /* Clear the LCD */
  UTIL_LCD_Clear(UTIL_LCD_COLOR_WHITE);
  /* Set Audio Demo description */
  UTIL_LCD_FillRect(0, 0, x_size, 90, UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_SetFont(&Font24);
  UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"AUDIO RECORD SAI PDM EXAMPLE", CENTER_MODE);
  UTIL_LCD_SetFont(&Font16);
  UTIL_LCD_DisplayStringAt(0, 24, (uint8_t *)"Make sure the SW2 is in position PDM ", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, 40,  (uint8_t *)"Press User button for next menu", CENTER_MODE);
  /* Set the LCD Text Color */
  UTIL_LCD_DrawRect(10, 100, x_size - 20, y_size - 110, UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_DrawRect(11, 101, x_size - 22, y_size - 112, UTIL_LCD_COLOR_BLUE);

  AudioFreq_ptr = AudioFreq+2; /* AUDIO_FREQUENCY_16K; */

  AudioOutInit.Device = AUDIO_OUT_DEVICE_AUTO;
  AudioOutInit.ChannelsNbr = channel_nbr;
  AudioOutInit.SampleRate = *AudioFreq_ptr;
  AudioOutInit.BitsPerSample = AUDIO_RESOLUTION_16B;
  AudioOutInit.Volume = VolumeLevel;

  AudioInInit.Device = AUDIO_IN_DEVICE_DIGITAL_MIC;
  AudioInInit.ChannelsNbr = channel_nbr;
  AudioInInit.SampleRate = *AudioFreq_ptr;
  AudioInInit.BitsPerSample = AUDIO_RESOLUTION_16B;
  AudioInInit.Volume = VolumeLevel;

  BSP_JOY_Init(JOY1, JOY_MODE_GPIO, JOY_ALL);

  /* Initialize Audio Recorder with 2 channels to be used */
  BSP_AUDIO_IN_Init(1, &AudioInInit);
  BSP_AUDIO_IN_GetState(1, &InState);

  BSP_AUDIO_OUT_Init(0, &AudioOutInit);

  BSP_AUDIO_OUT_SetDevice(0, AUDIO_OUT_DEVICE_HEADPHONE);

  /* Start Recording */
  UTIL_LCD_DisplayStringAt(0, 190, (uint8_t *)"Start Recording ", CENTER_MODE);
  BSP_AUDIO_IN_RecordPDM(1, (uint8_t*)&recordPDMBuf, 2*AUDIO_IN_PDM_BUFFER_SIZE);

  /* Play the recorded buffer*/
  UTIL_LCD_DisplayStringAt(0, 220, (uint8_t *)"Play the recorded buffer... ", CENTER_MODE);
  BSP_AUDIO_OUT_Play(0, (uint8_t*)&RecPlayback[0], 2*RECORD_BUFFER_SIZE);
  while (1)
  {
    if (CheckForUserInput() > 0)
    {
      ButtonState = 0;
      BSP_AUDIO_OUT_Stop(0);
      BSP_AUDIO_OUT_DeInit(0);
      BSP_AUDIO_IN_Stop(1);
      BSP_AUDIO_IN_DeInit(1);
      return;
    }
  }
}

/**
  * @brief Calculates the remaining file size and new position of the pointer.
  * @param  None
  * @retval None
  */
void  BSP_AUDIO_IN_TransferComplete_CallBack(uint32_t Instance)
{
    if(Instance == 1U)
  {
    /* Invalidate Data Cache to get the updated content of the SRAM*/
    SCB_InvalidateDCache_by_Addr((uint32_t *)&recordPDMBuf[AUDIO_IN_PDM_BUFFER_SIZE/2], AUDIO_IN_PDM_BUFFER_SIZE*2);

    BSP_AUDIO_IN_PDMToPCM(Instance, (uint16_t*)&recordPDMBuf[AUDIO_IN_PDM_BUFFER_SIZE/2], &RecPlayback[playbackPtr]);

    /* Clean Data Cache to update the content of the SRAM */
    SCB_CleanDCache_by_Addr((uint32_t*)&RecPlayback[playbackPtr], AUDIO_IN_PDM_BUFFER_SIZE/4);

    playbackPtr += AUDIO_IN_PDM_BUFFER_SIZE/4/2;
    if(playbackPtr >= RECORD_BUFFER_SIZE)
      playbackPtr = 0;
  }
  else
  {
    AudioBufferOffset = BUFFER_OFFSET_FULL;
  }

}

/**
  * @brief  Manages the DMA Half Transfer complete interrupt.
  * @param  None
  * @retval None
  */
void BSP_AUDIO_IN_HalfTransfer_CallBack(uint32_t Instance)
{
    if(Instance == 1U)
  {
    /* Invalidate Data Cache to get the updated content of the SRAM*/
    SCB_InvalidateDCache_by_Addr((uint32_t *)&recordPDMBuf[0], AUDIO_IN_PDM_BUFFER_SIZE*2);

    BSP_AUDIO_IN_PDMToPCM(Instance, (uint16_t*)&recordPDMBuf[0], &RecPlayback[playbackPtr]);

    /* Clean Data Cache to update the content of the SRAM */
    SCB_CleanDCache_by_Addr((uint32_t*)&RecPlayback[playbackPtr], AUDIO_IN_PDM_BUFFER_SIZE/4);

    playbackPtr += AUDIO_IN_PDM_BUFFER_SIZE/4/2;
    if(playbackPtr >= RECORD_BUFFER_SIZE)
    {
      playbackPtr = 0;
    }
  }
  else
  {
    AudioBufferOffset = BUFFER_OFFSET_HALF;
  }

}

/**
  * @brief  Audio IN Error callback function
  * @param  None
  * @retval None
  */
void BSP_AUDIO_IN_Error_CallBack(uint32_t Instance)
{
  /* Stop the program with an infinite loop */
  Error_Handler();
}
/**
  * @}
  */

