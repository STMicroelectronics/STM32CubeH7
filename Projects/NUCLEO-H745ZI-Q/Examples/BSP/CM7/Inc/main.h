/**
  ******************************************************************************
  * @file    BSP/CM7/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module for Cortex-M7
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2018 STMicroelectronics.
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
#include "adafruit_802.h"
#include "adafruit_802_conf.h"
#include "stm32h7xx_nucleo.h"
#include "adafruit_802_sd.h"
#include "adafruit_802_lcd.h"
#include "stm32_lcd.h"

/* Exported types ------------------------------------------------------------*/
typedef struct
{
  void   (*DemoFunc)(void);
  uint8_t DemoName[30];
  uint32_t DemoIndex;
}BSP_DemoTypedef;
extern uint32_t ButtonState;
extern const unsigned char stlogo[];

/* Exported constants --------------------------------------------------------*/
#define KEY_NOT_PRESSED 0x00
#define KEY_PRESSED     0x01

/* Exported macro ------------------------------------------------------------*/
#define COUNT_OF_EXAMPLE(x)    (sizeof(x)/sizeof(BSP_DemoTypedef))

#ifdef USE_FULL_ASSERT
/* Assert activated */
#define ASSERT(__condition__)                do { if(__condition__) \
                                                   {  assert_failed(__FILE__, __LINE__); \
                                                      while(1);  \
                                                    } \
                                              }while(0)
#else
/* Assert not activated : macro has no effect */
#define ASSERT(__condition__)                  do { if(__condition__) \
                                                   {  ErrorCounter++; \
                                                    } \
                                              }while(0)
#endif /* USE_FULL_ASSERT */

/* Exported functions ------------------------------------------------------- */
void Error_Handler(void);
uint8_t CheckForUserInput(void);
void Joystick_demo (void);
void LCD_demo (void);
void SD_demo (void);

#endif /* __MAIN_H */


