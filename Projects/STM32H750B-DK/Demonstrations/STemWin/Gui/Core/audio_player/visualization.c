/**
  ******************************************************************************
  * @file    visualisation.c
  * @author  MCD Application Team
  * @brief   visualisation functions
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
#include "visualization.h"

#include <stdint.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#if defined ( __ICCARM__ )
#include <intrinsics.h>
#endif

/* Private defines -----------------------------------------------------------*/
#if defined ( __GNUC__ )
__attribute__((section(".viz_front_buffer"))) __attribute__ ((aligned (32)))
#elif defined ( __ICCARM__ )
#pragma data_alignment=32
#pragma location="viz_front_buffer"
#elif defined( __CC_ARM )
__attribute__((section(".viz_front_buffer"), zero_init)) __attribute__ ((aligned (32)))
#endif
static uint8_t front_buffer[VIZ_WIDTH * VIZ_HEIGHT];

#if defined ( __GNUC__ )
__attribute__((section(".viz_back_buffer"))) __attribute__ ((aligned (32)))
#elif defined ( __ICCARM__ )
#pragma data_alignment=32
#pragma location="viz_back_buffer"
#elif defined( __CC_ARM )
__attribute__((section(".viz_back_buffer"), zero_init)) __attribute__ ((aligned (32)))
#endif
static uint8_t back_buffer[VIZ_WIDTH * VIZ_HEIGHT];

#if defined ( __GNUC__ )
__attribute__((section(".viz_deform_x_buffer"))) __attribute__ ((aligned (32)))
#elif defined ( __ICCARM__ )
#pragma data_alignment=32
#pragma location="viz_deform_x_buffer"
#elif defined( __CC_ARM )
__attribute__((section(".viz_deform_x_buffer"), zero_init)) __attribute__ ((aligned (32)))
#endif
static int16_t deform_offset_x[VIZ_WIDTH * VIZ_HEIGHT];

#if defined ( __GNUC__ )
__attribute__((section(".viz_deform_y_buffer"))) __attribute__ ((aligned (32)))
#elif defined ( __ICCARM__ )
#pragma data_alignment=32
#pragma location="viz_deform_y_buffer"
#elif defined( __CC_ARM )
__attribute__((section(".viz_deform_y_buffer"), zero_init)) __attribute__ ((aligned (32)))
#endif
static int16_t deform_offset_y[VIZ_WIDTH * VIZ_HEIGHT];

#define PI          3.141529f
#define PROJECTION  (4.0f / 3)

static int current_effect;
static struct viz_palette palette;

static uint32_t quarter_mask = 0xf8f8f8f8;
static uint32_t half_mask = 0xfefefefe;
static uint32_t attenuation_mask = 0x01010101;

#if defined ( __ICCARM__ )
#pragma location="ExtQSPIFlashSection" 
#else
__attribute__((section(".ExtQSPIFlashSection")))  
#endif
static const uint32_t fire_palette[256] = {
0x000000,
0x000018,
0x000018,
0x00001c,
0x000020,
0x000020,
0x000024,
0x000028,
0x080028,
0x100024,
0x180024,
0x200020,
0x28001c,
0x30001c,
0x380018,
0x400014,
0x480014,
0x500010,
0x580010,
0x60000c,
0x680008,
0x700008,
0x780004,
0x800000,
0x800000,
0x840000,
0x880000,
0x8c0000,
0x900000,
0x900000,
0x940000,
0x980000,
0x9c0000,
0xa00000,
0xa00000,
0xa40000,
0xa80000,
0xac0000,
0xb00000,
0xb40000,
0xb80400,
0xbc0400,
0xc00800,
0xc40800,
0xc80c00,
0xcc0c00,
0xd01000,
0xd41000,
0xd81400,
0xdc1400,
0xe01800,
0xe41800,
0xe81c00,
0xec1c00,
0xf02000,
0xf42000,
0xfc2400,
0xfc2400,
0xfc2800,
0xfc2800,
0xfc2c00,
0xfc2c00,
0xfc3000,
0xfc3000,
0xfc3400,
0xfc3400,
0xfc3800,
0xfc3800,
0xfc3c00,
0xfc3c00,
0xfc4000,
0xfc4000,
0xfc4400,
0xfc4400,
0xfc4800,
0xfc4800,
0xfc4c00,
0xfc4c00,
0xfc5000,
0xfc5000,
0xfc5400,
0xfc5400,
0xfc5800,
0xfc5800,
0xfc5c00,
0xfc6000,
0xfc6000,
0xfc6400,
0xfc6400,
0xfc6800,
0xfc6800,
0xfc6c00,
0xfc6c00,
0xfc7000,
0xfc7000,
0xfc7400,
0xfc7400,
0xfc7800,
0xfc7800,
0xfc7c00,
0xfc7c00,
0xfc8000,
0xfc8000,
0xfc8400,
0xfc8400,
0xfc8800,
0xfc8800,
0xfc8c00,
0xfc8c00,
0xfc9000,
0xfc9000,
0xfc9400,
0xfc9800,
0xfc9800,
0xfc9c00,
0xfc9c00,
0xfca000,
0xfca000,
0xfca400,
0xfca400,
0xfca800,
0xfca800,
0xfcac00,
0xfcac00,
0xfcb000,
0xfcb000,
0xfcb400,
0xfcb400,
0xfcb800,
0xfcb800,
0xfcbc00,
0xfcbc00,
0xfcc000,
0xfcc000,
0xfcc400,
0xfcc400,
0xfcc800,
0xfcc800,
0xfccc00,
0xfcd000,
0xfcd000,
0xfcd000,
0xfcd000,
0xfcd000,
0xfcd400,
0xfcd400,
0xfcd400,
0xfcd400,
0xfcd800,
0xfcd800,
0xfcd800,
0xfcd800,
0xfcd800,
0xfcdc00,
0xfcdc00,
0xfcdc00,
0xfcdc00,
0xfce000,
0xfce000,
0xfce000,
0xfce000,
0xfce400,
0xfce400,
0xfce400,
0xfce400,
0xfce400,
0xfce800,
0xfce800,
0xfce800,
0xfce800,
0xfcec00,
0xfcec00,
0xfcec00,
0xfcec00,
0xfcf000,
0xfcf000,
0xfcf000,
0xfcf000,
0xfcf000,
0xfcf400,
0xfcf400,
0xfcf400,
0xfcf400,
0xfcf800,
0xfcf800,
0xfcf800,
0xfcf800,
0xfcfc00,
0xfcfc04,
0xfcfc08,
0xfcfc0c,
0xfcfc10,
0xfcfc14,
0xfcfc18,
0xfcfc1c,
0xfcfc20,
0xfcfc24,
0xfcfc28,
0xfcfc28,
0xfcfc2c,
0xfcfc30,
0xfcfc34,
0xfcfc38,
0xfcfc3c,
0xfcfc40,
0xfcfc44,
0xfcfc48,
0xfcfc4c,
0xfcfc50,
0xfcfc54,
0xfcfc54,
0xfcfc58,
0xfcfc5c,
0xfcfc60,
0xfcfc64,
0xfcfc68,
0xfcfc6c,
0xfcfc70,
0xfcfc74,
0xfcfc78,
0xfcfc7c,
0xfcfc7c,
0xfcfc80,
0xfcfc84,
0xfcfc88,
0xfcfc8c,
0xfcfc90,
0xfcfc94,
0xfcfc98,
0xfcfc9c,
0xfcfca0,
0xfcfca4,
0xfcfca8,
0xfcfca8,
0xfcfcac,
0xfcfcb0,
0xfcfcb4,
0xfcfcb8,
0xfcfcbc,
0xfcfcc0,
0xfcfcc4,
0xfcfcc8,
0xfcfccc,
0xfcfcd0,
0xfcfcd0,
0xfcfcd4,
0xfcfcd8,
0xfcfcdc,
0xfcfce0,
0xfcfce4,
0xfcfce8,
0xfcfcec,
0xfcfcf0,
0xfcfcf4,
0xfcfcf8,
0xfcfcfc
};

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  From cartesian to polar
  * @param  x: Horizontal position
  * @param  y: Vertical position
  * @param  d: Distance
  * @param  r: angle
  * @retval None
  */
static void to_polar(int x, int y, float* d, float* r)
{
  float distance, angle;
  distance = sqrtf((float)(x * x + y * y));
  angle = acosf((float)(x / distance));
  
  if (y < 0)
  {
    angle = -angle;
  }
  
  *d = distance;
  *r = angle;
}

/**
  * @brief  From polar to cartesian
  * @param  d: Distance
  * @param  r: angle
  * @param  x: Horizontal position
  * @param  y: Vertical position
  * @retval None
  */
static void to_cartesian(float d, float r, int* x, int* y)
{
  *x = (int)(d * cosf(r));
  *y = (int)(d * sinf(r));
}

/**
  * @brief  Deform
  * @param  src : source buffer
  * @param  dest: destination buffer
  * @retval None
  */
static void deform(uint8_t* src, uint8_t* dest)
{
  int x, y;
  int offset_x, offset_y;
  int index = 0;

  for (y = 0; y < VIZ_HEIGHT; y++) 
  {
    for (x = 0; x < VIZ_WIDTH; x++) 
    {
      offset_x = (int)deform_offset_x[index];
      offset_y = (int)deform_offset_y[index];
      if (offset_x >= 0 && offset_x < VIZ_WIDTH
          && offset_y >= 0 && offset_y < VIZ_HEIGHT)
      {
        dest[index] = src[offset_x + offset_y * VIZ_WIDTH];
      }
      else
      {
        dest[index] = 0;
      }
      index++;
    }
  }
}

/**
  * @brief  Deform horizontally
  * @param  None
  * @retval None
  */
static void horizontal_deform(void)
{
  int x, y;
  int offset = 0;

  for (y = 0; y < VIZ_HEIGHT; y++) 
  {
    for (x = 0; x < VIZ_WIDTH; x++) 
    {
      deform_offset_x[offset] = (int)((VIZ_WIDTH / 2 - x) / 5 + x);
      deform_offset_y[offset] = (int)y;
      offset++;
    }
  }
}

/**
  * @brief  Radial deform
  * @param  None
  * @retval None
  */
static void radial_deform(void)
{
  int x, y;
  int offset = 0;

  for (y = 0; y < VIZ_HEIGHT; y++) 
  {
    for (x = 0; x < VIZ_WIDTH; x++) 
    {
      float u = (float)(x - VIZ_WIDTH / 2);
      float v = (float)(y - VIZ_HEIGHT / 2);
      float d = sqrtf(u * u + v * v);
      float z = 40.0f * fabsf(sinf(d / 30)) + 40;
      deform_offset_x[offset] = (int)((PROJECTION * u / z - u) / 32.0f + x);
      deform_offset_y[offset] = (int)((PROJECTION * v / z - v) / 32.0f + y);
      offset++;
    }
  }
}

/**
  * @brief  Rotation deform
  * @param  None
  * @retval None
  */
static void rotation_deform(void)
{
  int x, y;
  int offset = 0;
  for (y = 0; y < VIZ_HEIGHT; y++) 
  {
    for (x = 0; x < VIZ_WIDTH; x++) 
    {
      float u = (float)(x - VIZ_WIDTH / 2);
      float v = (float)(y - VIZ_HEIGHT / 2);
      deform_offset_x[offset] = (int)(cosf(0.0349f) * u + sinf(0.0349f) * v - u + x);
      deform_offset_y[offset] = (int)(-sinf(0.0349f) * u + cosf(0.0349f) * v - v + y);
      offset++;
    }
  }
}

/**
  * @brief  Swirl deform
  * @param  None
  * @retval None
  */
static void swirl_deform(void)
{
  int x, y;
  int offset = 0;

  for (y = 0; y < VIZ_HEIGHT; y++) 
  {
    for (x = 0; x < VIZ_WIDTH; x++) 
    {
      float r, d;
      int u = x - VIZ_WIDTH / 2;
      int v = y - VIZ_HEIGHT / 2;
      to_polar(u, v, &d, &r);
      r = r + 0.1f * cosf(d / 20);
      to_cartesian(d, r, &u, &v);
      deform_offset_x[offset] = (int)(u + VIZ_WIDTH / 2);
      deform_offset_y[offset] = (int)(v + VIZ_HEIGHT / 2);
      offset++;
    }
  }
}

/**
  * @brief  Mosaic deform
  * @param  None
  * @retval None
  */
static void mosaic_deform(void)
{
  int x, y;
  int offset = 0;
  
  for (y = 0; y < VIZ_HEIGHT; y++) 
  {
    for (x = 0; x < VIZ_WIDTH; x++) 
    {
      float u = (float)(x - VIZ_WIDTH / 2) / (float)(VIZ_WIDTH / 2);
      float v = (float)(y - VIZ_HEIGHT / 2) / (float)(VIZ_HEIGHT / 2);
      float tmpx = u * 0.94f + sinf(v * PI * 8) * 0.03f;
      float tmpy = v * 0.94f + sinf(u * PI * 8) * 0.03f;
      deform_offset_x[offset] = (int)((VIZ_WIDTH / 2) * tmpx + VIZ_WIDTH / 2);
      deform_offset_y[offset] = (int)((VIZ_HEIGHT / 2) * tmpy + VIZ_HEIGHT / 2);
      offset++;
    }
  }
}

#ifdef __GNUC__
__attribute__( ( always_inline ) ) inline uint32_t __uqsub8(uint32_t op1, uint32_t op2)
{
  uint32_t result;

  __asm volatile ("uqsub8 %0, %1, %2" : "=r" (result) : "r" (op1), "r" (op2) );
  return(result);
}
#endif

/**
  * @brief  Blur filter function
  * @param  effect: visual effect needed
  * @param  src   : source buffer
  * @param  dest  : destination buffer
  * @retval None
  */
static void blur_filter(struct viz_effect *effect, uint8_t *src, uint8_t *dest)
{
  register uint32_t pixel0, pixel1, pixel2, pixel3, pixel4;
  
  /* skip two lines */
  int count = (VIZ_WIDTH * (VIZ_HEIGHT - 2)) / 4; 
  src += VIZ_WIDTH;
  dest += VIZ_WIDTH;
  
  while (count-- > 0) 
  {
    pixel0 = *(uint32_t*)src;
    pixel0 &= half_mask;
    pixel0 >>= 1;

    pixel1 = *(uint32_t*)(src - 1);
    pixel1 &= quarter_mask;
    pixel1 >>= 3;
    pixel0 += pixel1;
    
    pixel2 = *(uint32_t*)(src + 1);
    pixel2 &= quarter_mask;
    pixel2 >>= 3;
    pixel0 += pixel2;
    
    pixel3 = *(uint32_t*)(src - VIZ_WIDTH);
    pixel3 &= quarter_mask;
    pixel3 >>= 3;
    pixel0 += pixel3;
    
    pixel4 = *(uint32_t*)(src + VIZ_WIDTH);
    pixel4 &= quarter_mask;
    pixel4 >>= 3;
    pixel0 += pixel4;
    
    *(uint32_t*)dest = pixel0;
    src += 4;
    dest += 4;
  }
  if (effect->attenuation) 
  {
    /* attenuate pixels to avoid stationary state */
#if defined(WIN32)
    count = (VIZ_WIDTH * (VIZ_HEIGHT - 2));
    dest -= VIZ_WIDTH * (VIZ_HEIGHT - 1);
    while (count-- > 0) 
    {
      uint8_t pixel = *dest;
      if (pixel > 0)
      {
        pixel--;
      }
      *dest++ = pixel;
    }
#else
    count = (VIZ_WIDTH * (VIZ_HEIGHT - 2)) / 4;
    dest -= VIZ_WIDTH * (VIZ_HEIGHT - 1);
    while (count-- > 0) 
    {
      uint32_t pixel = *(uint32_t*)dest;
#ifdef __arm__
      *(uint32_t*)dest = __uqsub8(pixel, attenuation_mask);
#else
      *(uint32_t*)dest = __UQSUB8(pixel, attenuation_mask);
#endif
      dest += 4;
    }
#endif
  }
}

/* Table of visual effects functions */
struct viz_effect viz_effects[5] = {
    { radial_deform, deform, "radial effect", true},
    { horizontal_deform, deform, "zoom effect", false },
    { rotation_deform, deform, "rotation effect", true },
    { swirl_deform, deform, "swirl effect", true },
    { mosaic_deform, deform, "mosaic effect", true }
};

/**
  * @brief  Visualisation initialisation
  * @param  None
  * @retval None
  */
void visualization_initialize(void)
{
  current_effect = 3;
  memset((void*)front_buffer, 0, VIZ_WIDTH * VIZ_HEIGHT);
  memset((void*)back_buffer, 0, VIZ_WIDTH * VIZ_HEIGHT);
  viz_effects[current_effect].init();
  
 for (int i = 0; i < 256; i++) 
 {
   palette.color[i].r = (fire_palette[i] & 0xff0000) >> 16;
   palette.color[i].g = (fire_palette[i] & 0xff00) >> 8;
   palette.color[i].b = fire_palette[i] & 0xff;
 }
}

/**
  * @brief  Visualisation initialisation
  * @param  p : Pointer to palette to use
  * @retval None
  */
void visualization_set_palette(struct viz_palette* p)
{
  memcpy(&palette, p, sizeof(struct viz_palette));
}

/**
  * @brief  Function to get the current effect used
  * @param  None
  * @retval Current visualisation effect
  */
struct viz_effect* visualization_current(void)
{
  return &viz_effects[current_effect];
}

/**
  * @brief  Function to set the next effect used
  * @param  None
  * @retval None
  */
void visualization_next(void)
{
  current_effect++;
  current_effect %= sizeof(viz_effects) / sizeof(struct viz_effect);

  viz_effects[current_effect].init();
}

/**
  * @brief  Function to draw a needed line
  * @param  framebuffer: Frame buffer used for the draw 
  * @param  x1: Horizontal begin of line
  * @param  y1: Vertical begin of line
  * @param  x2: Horizontal end of line
  * @param  y2: Vertical end of line
  * @retval None
  */
static void draw_line(uint8_t *framebuffer, int x1, int y1, int x2, int y2)
{
  int x, y, dx, dy, dx1, dy1;
  int px, py, xe, ye, i;

  dx = x2 - x1;
  dy = y2 - y1;
  dx1 = abs(dx);
  dy1 = abs(dy);
  px = 2 * dy1 - dx1;
  py = 2 * dx1 - dy1;
    
  if (dy1 <= dx1) 
  {
    if (dx >= 0)
    {
      x = x1;
      y = y1;
      xe = x2;
    }
    else
    {
      x = x2;
      y = y2;
      xe = x1;
    }
    
    framebuffer[x + y * VIZ_WIDTH] = 255;
    for(i = 0; x < xe; i++) 
    {
      x = x + 1;
      if (px < 0)
      {
        px = px + 2 * dy1;
      }
      else
      {
        if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) 
        {
          y = y + 1;
        }
        else 
        {
          y = y - 1;
        }
        px = px + 2 * (dy1 - dx1);
      }
      framebuffer[x + y * VIZ_WIDTH] = 255;
    }
  }
  else 
  {
    if(dy >= 0) 
    {
      x = x1;
      y = y1;
      ye = y2;
    }
    else
    {
      x = x2;
      y = y2;
      ye = y1;
    }
    framebuffer[x + y * VIZ_WIDTH] = 255;
    for(i = 0; y < ye; i++) 
    {
      y = y + 1;
      if (py <= 0) 
      {
        py = py + 2 * dx1;
      }
      else
      {
        if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) 
        {
          x = x + 1;
        }
        else
        {
          x = x - 1;
        }
        py = py + 2 * (dx1 - dy1);
      }
      framebuffer[x + y * VIZ_WIDTH] = 255;
    }
  }
}

/**
  * @brief  Function to render a given visualisation
  * @param  samples    : Samples used for the rendering 
  * @param  num_samples: Nbre of samples
  * @retval None
  */
void visualization_render(uint8_t *samples, int num_samples)
{
  /* draw new audio samples on top of the dest buffer */
  float x = 0, step = (float)(VIZ_WIDTH / num_samples);
  int prev_sample = ((int)samples[0] - 64) + (VIZ_HEIGHT / 2);
  for (int i = 1; i < num_samples; i++) 
  {
    int new_sample = ((int)samples[i] - 64) + (VIZ_HEIGHT / 2);
    draw_line((uint8_t*)front_buffer, (int)x, prev_sample, (int)(x + step), new_sample);
    x += step;
    prev_sample = new_sample;
  }
  /* apply deformation */
  viz_effects[current_effect].function((uint8_t*)front_buffer, (uint8_t*)back_buffer);
  /* blur the dest buffer back into the src buffer */
  blur_filter(&viz_effects[current_effect], (uint8_t*)back_buffer, (uint8_t*)front_buffer);
}

/**
  * @brief  Function to copy the visualisation to a frame buffer
  * @param  framebuffer: Frame buffer 
  * @param  pitch      : pitch of the buffer
  * @param  x          : x position
  * @param  y          : y position
  * @param  width      : width of the buffer used 
  * @param  height     : height of the buffer used
  * @retval None
  */
void visualization_copy_framebuffer(void* framebuffer, int pitch, int x, int y, int width, int height)
{
  uint8_t *src = (uint8_t*)front_buffer + x + y * VIZ_WIDTH;
  uint16_t *dest = (uint16_t*)framebuffer;
  
  for (int h = 0; h < height; h++) 
  {
    for (int w = 0; w < width; w++) 
    {
      uint8_t index = *src++;
      *dest++ = ((palette.color[index].r & 0xf8) << 8)
          | ((palette.color[index].g & 0xfc) << 3)
          | ((palette.color[index].b & 0xf8) >> 3);
    }
    dest += pitch - VIZ_WIDTH;
  }
}

