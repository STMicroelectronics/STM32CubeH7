/**
  ******************************************************************************
  * @file    BSP/CM7/Src/audio_record.c
  * @author  MCD Application Team
  * @brief   This example describes how to use DFSDM HAL API to realize
  *          audio recording.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#define AUDIO_FREQUENCY            BSP_AUDIO_FREQUENCY_16K
#define AUDIO_IN_PDM_BUFFER_SIZE  (uint32_t)(128*AUDIO_FREQUENCY/16000*DEFAULT_AUDIO_IN_CHANNEL_NBR)

/* Size of the recorder buffer */
#define RECORD_BUFFER_SIZE        4096

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Define record Buf at D3SRAM @0x38000000 since the BDMA for SAI4 use only this memory */
#if defined ( __CC_ARM )  /* !< ARM Compiler */
  ALIGN_32BYTES (uint16_t recordPDMBuf[AUDIO_IN_PDM_BUFFER_SIZE]) __attribute__((section(".RAM_D3")));

#elif defined ( __ICCARM__ )  /* !< ICCARM Compiler */
  #pragma location=0x38000000
  uint16_t recordPDMBuf[AUDIO_IN_PDM_BUFFER_SIZE]; 
#elif defined ( __GNUC__ )  /* !< GNU Compiler */
  ALIGN_32BYTES (uint16_t recordPDMBuf[AUDIO_IN_PDM_BUFFER_SIZE]) __attribute__((section(".RAM_D3")));
#endif

uint16_t playbackBuf[RECORD_BUFFER_SIZE*2];

/* Pointer to record_data */
uint32_t playbackPtr;

/* Private function prototypes -----------------------------------------------*/
static void AudioRecord_SetHint(void);

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
  AudioRecord_SetHint();
  
  /* Set audio input interface */
  BSP_AUDIO_IN_SelectInterface(AUDIO_IN_INTERFACE_PDM);
  
  /* Initialize audio IN at REC_FREQ*/
  if(BSP_AUDIO_IN_InitEx(INPUT_DEVICE_DIGITAL_MIC, AUDIO_FREQUENCY, DEFAULT_AUDIO_IN_BIT_RESOLUTION, DEFAULT_AUDIO_IN_CHANNEL_NBR) != AUDIO_OK)
  {
    /* Record Error */
    Error_Handler();
  }  
  
  /* Initialize audio OUT at REC_FREQ*/
  if(BSP_AUDIO_OUT_Init(OUTPUT_DEVICE_HEADPHONE, 70, AUDIO_FREQUENCY) != AUDIO_OK)
  {
    /* Record Error */
    Error_Handler();
  }
  
  /* Set audio slot */
  BSP_AUDIO_OUT_SetAudioFrameSlot(CODEC_AUDIOFRAME_SLOT_02);
  
  /* Start the record */
  BSP_AUDIO_IN_Record((uint16_t*)recordPDMBuf, AUDIO_IN_PDM_BUFFER_SIZE);

  /* Clear the LCD */
  BSP_LCD_Clear(LCD_COLOR_WHITE);

  /* Set LCD Demo description */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), 80);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
  BSP_LCD_SetFont(&Font24);
  BSP_LCD_DisplayStringAt(0, 0, (uint8_t *)"AUDIO RECORD", CENTER_MODE);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, 30, (uint8_t *)"This example is recording", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 45, (uint8_t *)"audio samples on Micros", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 60, (uint8_t *)"and play directly on CN11", CENTER_MODE);

  /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_DrawRect(10, 90, BSP_LCD_GetXSize() - 20, BSP_LCD_GetYSize()- 100);
  BSP_LCD_DrawRect(11, 91, BSP_LCD_GetXSize() - 22, BSP_LCD_GetYSize()- 102);
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);  
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, 130, (uint8_t *)"RECORD AND PLAYBACK ONGOING...", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 175, (uint8_t *)"Press Tamper push-button", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 200, (uint8_t *)"to stop recording", CENTER_MODE);
  
  /* Start audio output */
  BSP_AUDIO_OUT_Play((uint16_t*)playbackBuf, RECORD_BUFFER_SIZE*2); 

  while (1)
  {
    if (CheckForUserInput() > 0)
    {
      /* Stop the Player */
      BSP_AUDIO_OUT_Stop(CODEC_PDWN_SW);
      BSP_AUDIO_OUT_DeInit();

      /* Stop the recorder */
      BSP_AUDIO_IN_Stop();
      BSP_AUDIO_IN_DeInit();
      return;
    }
  }
} 
                                
/**
  * @brief  Display Audio Play demo hint
  * @param  None
  * @retval None
  */
static void AudioRecord_SetHint(void)
{
  /* Clear the LCD */ 
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  
  /* Set LCD Demo description */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), 80);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE); 
  BSP_LCD_SetFont(&Font24);
  BSP_LCD_DisplayStringAt(0, 0, (uint8_t *)"AUDIO RECORD", CENTER_MODE);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, 30, (uint8_t *)"This example is recording", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 45, (uint8_t *)"audio samples on Micros", CENTER_MODE); 
  BSP_LCD_DisplayStringAt(0, 60, (uint8_t *)"and play directly on CN5", CENTER_MODE); 

   /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);  
  BSP_LCD_DrawRect(10, 90, BSP_LCD_GetXSize() - 20, BSP_LCD_GetYSize()- 100);
  BSP_LCD_DrawRect(11, 91, BSP_LCD_GetXSize() - 22, BSP_LCD_GetYSize()- 102);
}
/**
  * @brief Calculates the remaining file size and new position of the pointer.
  * @param  None
  * @retval None
  */
void BSP_AUDIO_IN_TransferComplete_CallBack(void)
{
  if(BSP_AUDIO_IN_GetInterface() == AUDIO_IN_INTERFACE_PDM)
  {
    /* Invalidate Data Cache to get the updated content of the SRAM*/
    SCB_InvalidateDCache_by_Addr((uint32_t *)&recordPDMBuf[AUDIO_IN_PDM_BUFFER_SIZE/2], AUDIO_IN_PDM_BUFFER_SIZE*2);

    BSP_AUDIO_IN_PDMToPCM((uint16_t*)&recordPDMBuf[AUDIO_IN_PDM_BUFFER_SIZE/2], &playbackBuf[playbackPtr]);

    /* Clean Data Cache to update the content of the SRAM */
    SCB_CleanDCache_by_Addr((uint32_t*)&playbackBuf[playbackPtr], AUDIO_IN_PDM_BUFFER_SIZE/4);

    playbackPtr += AUDIO_IN_PDM_BUFFER_SIZE/4/2;
    if(playbackPtr >= RECORD_BUFFER_SIZE)
    {
      playbackPtr = 0;
    }
  }
}

/**
  * @brief  Manages the DMA Half Transfer complete interrupt.
  * @param  None
  * @retval None
  */
void BSP_AUDIO_IN_HalfTransfer_CallBack(void)
{
  if(BSP_AUDIO_IN_GetInterface() == AUDIO_IN_INTERFACE_PDM)
  {
    /* Invalidate Data Cache to get the updated content of the SRAM*/
    SCB_InvalidateDCache_by_Addr((uint32_t *)&recordPDMBuf[0], AUDIO_IN_PDM_BUFFER_SIZE*2);

    BSP_AUDIO_IN_PDMToPCM((uint16_t*)&recordPDMBuf[0], &playbackBuf[playbackPtr]);

    /* Clean Data Cache to update the content of the SRAM */
    SCB_CleanDCache_by_Addr((uint32_t*)&playbackBuf[playbackPtr], AUDIO_IN_PDM_BUFFER_SIZE/4);

    playbackPtr += AUDIO_IN_PDM_BUFFER_SIZE/4/2;
    if(playbackPtr >= RECORD_BUFFER_SIZE)
    {
      playbackPtr = 0;
    }
  }
}

/**
  * @brief  Audio IN Error callback function
  * @param  None
  * @retval None
  */
void BSP_AUDIO_IN_Error_CallBack(void)
{
  /* Stop the program with an infinite loop */
  Error_Handler();
}

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
