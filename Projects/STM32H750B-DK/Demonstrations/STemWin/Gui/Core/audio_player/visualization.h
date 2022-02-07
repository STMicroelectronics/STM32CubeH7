/**
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
  * ----------------------------------------------------------------------
  * File        : visualisation.h
  * Purpose     : Needed definition for SW visualisation effects
  * ----------------------------------------------------------------------
  */

#ifndef VISUALISATION_H
#define VISUALISATION_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif
  
/* 320x200 fits into STM32H7's SRAM1 bank */
#define VIZ_WIDTH   200 /* must be a multiple of 4 */
#define VIZ_HEIGHT  112 /* must be a multiple of 4 */

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
