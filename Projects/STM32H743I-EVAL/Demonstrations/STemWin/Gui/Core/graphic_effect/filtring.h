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

#ifndef FILTRING_H
#define FILTRING_H

/* Includes ------------------------------------------------------------------*/
#include "WM.h"

/* External functions ---------------------------------------------------------*/

/**
  * @brief  This function is used to copy the content of a bitmap in to another bitmap.
  * @param  *srcBitmap Pointer to the source bitmap struct.
  * @param  dstAddress Address of the destination bitmap.
  * @retval None
  */
void CopyBitmap(const GUI_BITMAP *srcBitmap, unsigned long dstAddress);

/**
* @brief  This function is used to apply different types of filter on part of the given bitmap.
* @param  *srcBitmap Pointer to the bitmap that will be filtered.
* @param  *dstAddress Pointer to the bitmap where the filtred pixels will be stored.
* @param  aRect The rectangle area that will be filtred.
* @param  filterType The filter that will be applied to the source bitmap.
* @retval None
*/
void FilterBitmap(const GUI_BITMAP *srcBitmap, unsigned long dstAddress, GUI_RECT aRect, unsigned int filterType);

#endif

