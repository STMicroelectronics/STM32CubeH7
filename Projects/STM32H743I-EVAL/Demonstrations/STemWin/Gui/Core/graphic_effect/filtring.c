/**
  ******************************************************************************
  * @file    graphic_effects_win.c
  * @author  MCD Application Team
  * @brief   graphic effects filtring functions
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
#include "WM.h"

/* Private define ------------------------------------------------------------*/
#ifndef MIN
  #define MIN(a,b) ((a) < (b) ? (a) : (b))
#endif

#ifndef MAX
  #define MAX(a,b) ((a) > (b) ? (a) : (b))
#endif

/* Define the filter length*/
#define KernelLength 9

/* Private functions ---------------------------------------------------------*/
void CopyBitmap(const GUI_BITMAP *srcBitmap, unsigned long dstAddress);
void FilterBitmap(const GUI_BITMAP *srcBitmap, unsigned long dstAddress, GUI_RECT aRect, unsigned int filterType);

/**
  * @brief  This function is used to copy the content of a bitmap in to another bitmap.
  * @param  *srcBitmap Pointer to the source bitmap struct.
  * @param  dstAddress Address of the destination bitmap.
  * @retval None
  */
void CopyBitmap(const GUI_BITMAP *srcBitmap, unsigned long dstAddress)
{
  unsigned long* srcPtr;
  unsigned long* dstPtr;
  int            x, y;

  /* Get the pointer to the bitmap pixel memory */
  srcPtr = (unsigned long *)srcBitmap->pData;
  dstPtr = (unsigned long *)dstAddress;

  /* Copy the bitmap row by row */
  for (y = 0; y < srcBitmap->YSize; y++)
  {
    for (x = 0; x < srcBitmap->XSize; x++)
    {
      dstPtr[(y * (srcBitmap->XSize)) + x] = srcPtr[(y * (srcBitmap->XSize)) + x];
    }
  }
}

/**
* @brief  This function is used to apply different types of filter on part of the given bitmap.
* @param  *srcBitmap Pointer to the bitmap that will be filtered.
* @param  *dstAddress Pointer to the bitmap where the filtred pixels will be stored.
* @param  aRect The rectangle area that will be filtred.
* @param  filterType The filter that will be applied to the source bitmap.
* @retval None
*/
void FilterBitmap(const GUI_BITMAP *srcBitmap, unsigned long dstAddress, GUI_RECT aRect, unsigned int filterType)
{
  unsigned long* srcPtr;
  unsigned long* dstPtr;

  /* Get the pointers to the bitmaps pixels memory */
  srcPtr = (unsigned long *)srcBitmap->pData;
  dstPtr = (unsigned long *)dstAddress;

  float EdgeDetectionFilter[KernelLength] =
  {
    -1, -1, -1,
    -1,  8, -1,
    -1, -1, -1
  };

  float SmoothingFilter[KernelLength] =
  {
    1, 1, 1,
    1, 2, 1,
    1, 1, 1
  };

  float SharpeningFilter[KernelLength] =
  {
    -1, -1, -1,
    -1,  9, -1,
    -1, -1, -1
  };

  float RaisingFilter[KernelLength] =
  {
    0, 0, -2,
    0, 2,  0,
    1, 0,  0
  };

  float MotionBlurFilter[KernelLength] =
  {
    0, 0, 1,
    0, 0, 0,
    1, 0, 0
  };

  float kernel[KernelLength];

  /* Initialize the kernel depending on the filter type */
  for (int i = 0; i < KernelLength; i++)
  {
    if (filterType == 0)
    {
      kernel[i] = EdgeDetectionFilter[i];
    }
    if (filterType == 1)
    {
      kernel[i] = SmoothingFilter[i];
    }
    if (filterType == 2)
    {
      kernel[i] = SharpeningFilter[i];
    }
    if (filterType == 3)
    {
      kernel[i] = RaisingFilter[i];
    }
    if (filterType == 4)
    {
      kernel[i] = MotionBlurFilter[i];
    }
  }

  /* The array indices is a simple optimization table used to find neighboring pixels. */
  int indices[9] =
  {
    -(srcBitmap->XSize + 1), -srcBitmap->XSize, -(srcBitmap->XSize - 1),
                         -1,                 0,                      +1,
       srcBitmap->XSize - 1,  srcBitmap->XSize,    srcBitmap->XSize + 1
  };

  float denominator = 0.0f;
  float fRed        = 0.0;
  float fGreen      = 0.0;
  float fBlue       = 0.0;
  int iRed          = 0;
  int iGreen        = 0;
  int iBlue         = 0;
  int indexOffset   = 0;
  int pixel         = 0;

  /* The denominator is the sum of parameter kernel, it will be the denominator when calculating average. */       
  for (int i = 0; i < KernelLength; i++)
  {
    denominator += kernel[i];
  }

  if (denominator == 0.0f)
  {
    denominator = 1.0f;
  }

  /* Start filtring the bitmap */
  for (int i = aRect.y0; i < aRect.y1; i++)
  {
    for (int j = aRect.x0; j < aRect.x1; j++)
    {
      indexOffset = (i * (srcBitmap->XSize)) + j;

      fRed = fGreen = fBlue = 0.0f;
      for (int k = 0; k < KernelLength; k++)
      {
        if ((indexOffset + indices[k] <= 0))
        {
          pixel = srcPtr[0];
        }
        else
        {
          pixel = srcPtr[indexOffset + indices[k]];
        }

        fRed   += ((pixel & 0x00FF0000) >> 16) * kernel[k];
        fGreen += ((pixel & 0x0000FF00) >> 8)  * kernel[k];
        fBlue  += (pixel & 0x000000FF)         * kernel[k];
      }

      iRed   = (int)(fRed / denominator);
      iGreen = (int)(fGreen / denominator);
      iBlue  = (int)(fBlue / denominator);

      /* Clip the RGB values */
      iRed   = MIN( MAX( iRed, 0 ), 255 );
      iGreen = MIN( MAX( iGreen, 0 ), 255 );
      iBlue  = MIN( MAX( iBlue, 0 ), 255 );

      dstPtr[indexOffset] = 0xFF000000 | ((iRed << 16) & 0xFF0000) | ((iGreen << 8) & 0xFF00) | (iBlue & 0xFF);
    }
  }
}
