/**
  ******************************************************************************
  * @file    BSP/CM7/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module for Cortex-M7.
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
#include "stm32h745i_discovery.h"
#include "stm32h745i_discovery_sdram.h"
#include "stm32h745i_discovery_lcd.h"
#include "stm32h745i_discovery_ts.h"
#include "stm32h745i_discovery_audio.h"
#include "stm32h745i_discovery_qspi.h"
#include "stm32h745i_discovery_mmc.h"
#include "stm32_lcd.h"



/* Exported variables --------------------------------------------------------*/
extern const unsigned char stlogo[];
extern __IO uint32_t SRAMTest;
extern __IO uint32_t ButtonState ;

typedef enum {
  AUDIO_ERROR_NONE = 0,
  AUDIO_ERROR_NOTREADY,
  AUDIO_ERROR_IO,
  AUDIO_ERROR_EOF,
}AUDIO_ErrorTypeDef;

typedef struct
{
  void   (*DemoFunc)(void);
  uint8_t DemoName[50];
  uint32_t DemoIndex;
}BSP_DemoTypedef;

/* Exported constants --------------------------------------------------------*/

#define SDRAM_WRITE_READ_ADDR        ((uint32_t)0xD0177000)

#define SDRAM_WRITE_READ_ADDR_OFFSET ((uint32_t)0x0800)

/* Defines for the Audio playing process */
#define PAUSE_STATUS     ((uint32_t)0x00) /* Audio Player in Pause Status */
#define RESUME_STATUS    ((uint32_t)0x01) /* Audio Player in Resume Status */
#define IDLE_STATUS      ((uint32_t)0x02) /* Audio Player in Idle Status */


#define LED_GREEN      LED1
#define LED_RED        LED2

#define AUDIO_REC_START_ADDR         SDRAM_WRITE_READ_ADDR
#define AUDIO_REC_TOTAL_SIZE         ((uint32_t) 0x0000E000)
#define AUDIO_RECPDM_START_ADDR      (AUDIO_REC_START_ADDR+AUDIO_REC_TOTAL_SIZE)

/* The Audio file is flashed with ST-Link Utility @ flash address =  AUDIO_SRC_FILE_ADDRESS */
#define AUDIO_SRC_FILE_ADDRESS       0x08080000   /* Audio file address in flash */
#define AUDIO_FILE_SIZE              0x80000

#define AUDIO_PLAY_SAMPLE        0
#define AUDIO_PLAY_RECORDED      1

/* Exported macro ------------------------------------------------------------*/
#define COUNT_OF_EXAMPLE(x)    (sizeof(x)/sizeof(BSP_DemoTypedef))

/* Exported functions ------------------------------------------------------- */
void Touchscreen_demo1 (void);
void Touchscreen_demo2 (void);
void LCD_demo (void);
void Log_demo(void);
void Touchscreen_demo(void);
void AudioPlay_demo(void);
void Error_Handler(void);
void SDRAM_demo(void);
void SDRAM_DMA_demo (void);
void AudioPlay_demo (void);
void AudioRecord_demo(void);
uint8_t AUDIO_Process(void);
void QSPI_demo (void);
void MMC_demo (void);
void DMA2_Stream0_IRQHandler(void);
void DMA2_Stream5_IRQHandler(void);
uint8_t CheckForUserInput(void);
void Toggle_Leds(void);
void Touchscreen_DrawBackground_Circles(uint8_t state);
void Touchscreen_demo3(void);
uint8_t TouchScreen_GetTouchPosition(void);
#endif /* __MAIN_H */

