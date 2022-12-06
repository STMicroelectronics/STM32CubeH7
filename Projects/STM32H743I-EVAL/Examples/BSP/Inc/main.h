/**
  ******************************************************************************
  * @file    BSP/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
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
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stdio.h"
#include "string.h"
#include "stm32h7xx_hal.h"
#include "stm32h743i_eval.h"
#include "stm32h743i_eval_lcd.h"
#include "stm32h743i_eval_sd.h"
#include "stm32h743i_eval_eeprom.h"
#include "stm32h743i_eval_nor.h"
#include "stm32h743i_eval_sram.h"
#include "stm32h743i_eval_sdram.h"
#include "stm32h743i_eval_io.h"
#include "stm32h743i_eval_ts.h"
#include "stm32h743i_eval_audio.h"
#include "stm32h743i_eval_qspi.h"
#include "stm32_lcd.h"

/* Exported types ------------------------------------------------------------*/
typedef struct
{
  void   (*DemoFunc)(void);
  uint8_t DemoName[30];
  uint32_t DemoIndex;
}BSP_DemoTypedef;

typedef enum {
  AUDIO_ERROR_NONE = 0,
  AUDIO_ERROR_NOTREADY,
  AUDIO_ERROR_IO,
  AUDIO_ERROR_EOF,
}AUDIO_ErrorTypeDef;

typedef enum {
  AUDIO_STATE_IDLE = 0,
  AUDIO_STATE_INIT,
  AUDIO_STATE_PLAYING,
  AUDIO_STATE_PAUSE
}AUDIO_PLAYBACK_StateTypeDef;

typedef enum {
  TS_ACT_NONE = 0,
  TS_ACT_FREQ_DOWN,
  TS_ACT_FREQ_UP,
  TS_ACT_VOLUME_DOWN,
  TS_ACT_VOLUME_UP,
  TS_ACT_PAUSE = 0xFE
}TS_ActionTypeDef;

/* Exported variables --------------------------------------------------------*/
extern const unsigned char stlogo[];
extern uint32_t SdramTest;
extern uint32_t ButtonState;
extern uint32_t SdmmcTest;
extern uint32_t AudioUpdate;
extern uint32_t AudioInstance;
extern uint32_t TSInterruptTest;
extern uint32_t JoyStickDemo;
extern uint32_t AudioUpdate;
extern uint32_t PauseEnabledStatus;
extern uint16_t PlayBuffer[];
extern uint8_t FreqStr[];

extern TS_Init_t hTS;
extern TS_ActionTypeDef ts_action;
extern TS_State_t  TS_State;
extern AUDIO_PLAYBACK_StateTypeDef  audio_state;
/* Exported constants --------------------------------------------------------*/
/**
  * @brief  SDRAM Write read buffer start address after CAM Frame buffer
  * Assuming Camera frame buffer is of size 640x480 and format ARGB8888 (32 bits per pixel).
  */
#define SDRAM_WRITE_READ_ADDR        ((uint32_t)0xD0130000)
#define SDRAM_WRITE_READ_ADDR_OFFSET ((uint32_t)0x0800)

/* Defines for the Audio playing process */
#define PAUSE_STATUS     ((uint32_t)0x00) /* Audio Player in Pause Status */
#define RESUME_STATUS    ((uint32_t)0x01) /* Audio Player in Resume Status */
#define IDLE_STATUS      ((uint32_t)0x02) /* Audio Player in Idle Status */


#define LED_GREEN      LED1
#define LED_ORANGE     LED2
#define LED_RED        LED3
#define LED_BLUE       LED4

#define AUDIO_REC_START_ADDR         SDRAM_WRITE_READ_ADDR
#define AUDIO_REC_TOTAL_SIZE         ((uint32_t) 0x0000E000)
#define AUDIO_RECPDM_START_ADDR      (AUDIO_REC_START_ADDR+AUDIO_REC_TOTAL_SIZE)

/* The Audio file is flashed with ST-Link Utility @ flash address =  AUDIO_SRC_FILE_ADDRESS */
#define AUDIO_SRC_FILE_ADDRESS       0x08080000   /* Audio file address in flash */
#define AUDIO_FILE_SIZE              0x80000


/* AUDIO definitions */
#define AUDIO_BUFFER_SIZE            4096

#define AUDIO_PLAY_SAMPLE        0
#define AUDIO_PLAY_RECORDED      1

/* SD definitions */
#define SD_DMA_MODE      0U
#define SD_IT_MODE       1U
#define SD_POLLING_MODE  2U

/* Exported macro ------------------------------------------------------------*/
#define COUNT_OF_EXAMPLE(x)    (sizeof(x)/sizeof(BSP_DemoTypedef))

/* Exported functions ------------------------------------------------------- */
void LCD_demo (void);
void Joystick_demo(void);
void Touchscreen_demo1(void);
void Touchscreen_demo2(void);
void Touchscreen_DrawBackground_Circles(uint8_t state);
void POTENTIOMETER_demo (void);
void EEPROM_demo (void);
void QSPI_demo (void);
void SD_DMA_demo(void);
void SD_IT_demo(void);
void SD_POLLING_demo(void);
void NOR_demo (void);
void SRAM_demo (void);
void SRAM_DMA_demo (void);
void SDRAM_demo(void);
void SDRAM_DMA_demo(void);
void AudioPlay_demo (void);
void AudioRec_demo (void);
void REC_SingleBuff_demo(void);
void REC_MultiBuff_demo(void);
void REC_AnalogMicRecord_demo(void);
void REC_DigitalMicRecord_demo(void);
void REC_PDM_demo(void);
void Toggle_Leds(void);
void Error_Handler(void);
void Touchscreen_Calibration(void);
uint8_t CheckForUserInput(void);
uint8_t TouchScreen_GetTouchPosition(void);
uint8_t AUDIO_Process(void);

#endif /* __MAIN_H */

