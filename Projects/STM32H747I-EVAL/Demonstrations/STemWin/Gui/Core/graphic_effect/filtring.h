/**
  ******************************************************************************
  * @file    filtring.h
  * @author  MCD Application Team
  * @brief   Header file for filtring.h module
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
