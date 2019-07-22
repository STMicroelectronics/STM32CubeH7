/**
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics International N.V. 
  * All rights reserved.</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
