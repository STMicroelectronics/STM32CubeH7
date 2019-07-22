/**
  ******************************************************************************
  * @file    visualisation.h
  * @author  MCD Application Team
  * @brief   header file for visualisation.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

#ifndef VISUALISATION_H
#define VISUALISATION_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif
  
/* 320x200 fits into STM32H7's SRAM1 bank */
#define VIZ_WIDTH   320 /* must be a multiple of 4 */
#define VIZ_HEIGHT  200 /* must be a multiple of 4 */

struct viz_effect 
{
  void (*init) (void);
  void (*function)(uint8_t* src, uint8_t* dest);
  char description[32];
  bool attenuation;
};

extern struct viz_effect viz_effects[5];

struct viz_palette 
{
  struct 
  {
    uint8_t r;
    uint8_t g;
    uint8_t b;
  } color[256];
};

void visualization_initialize(void);
struct viz_effect* visualization_current(void);
void visualization_set_palette(struct viz_palette* palette);
void visualization_next(void);
void visualization_render(uint8_t *samples, int num_samples);
void visualization_copy_framebuffer(void* framebuffer, int pitch, int x, int y, int width, int height);

#ifdef __cplusplus
}
#endif

#endif
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
