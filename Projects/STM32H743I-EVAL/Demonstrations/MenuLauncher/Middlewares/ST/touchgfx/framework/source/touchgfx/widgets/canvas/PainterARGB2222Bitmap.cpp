/**
  ******************************************************************************
  * This file is part of the TouchGFX 4.12.3 distribution.
  *
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

#include <touchgfx/widgets/canvas/PainterARGB2222Bitmap.hpp>

namespace touchgfx
{
PainterARGB2222Bitmap::PainterARGB2222Bitmap(const Bitmap& bmp, uint8_t alpha) :
    AbstractPainterARGB2222(), bitmapARGB2222Pointer(0)
{
    setBitmap(bmp);
    setAlpha(alpha);
}

void PainterARGB2222Bitmap::setBitmap(const Bitmap& bmp)
{
    bitmap = bmp;
    assert((bitmap.getId() == BITMAP_INVALID || bitmap.getFormat() == Bitmap::ARGB2222) && "The chosen painter only works with ARGB2222 bitmaps");
    bitmapRectToFrameBuffer = bitmap.getRect();
    DisplayTransformation::transformDisplayToFrameBuffer(bitmapRectToFrameBuffer);
}

void PainterARGB2222Bitmap::setAlpha(uint8_t alpha)
{
    painterAlpha = alpha;
}

uint8_t PainterARGB2222Bitmap::getAlpha() const
{
    return painterAlpha;
}

void PainterARGB2222Bitmap::render(uint8_t* ptr,
                                   int x,
                                   int xAdjust,
                                   int y,
                                   unsigned count,
                                   const uint8_t* covers)
{
    uint8_t* p = ptr + (x + xAdjust);

    currentX = x + areaOffsetX;
    currentY = y + areaOffsetY;

    if (!renderInit())
    {
        return;
    }

    if (currentX + (int)count > bitmapRectToFrameBuffer.width)
    {
        count = bitmapRectToFrameBuffer.width - currentX;
    }

    uint8_t totalAlpha = LCD::div255(widgetAlpha * painterAlpha);
    const uint8_t* src = bitmapARGB2222Pointer;
    if (totalAlpha == 0xFF)
    {
        do
        {
            uint8_t srcAlpha = ((*src) >> 6) * 0x55;
            uint8_t alpha = LCD::div255((*covers) * srcAlpha);
            covers++;
            if (alpha == 0xFF)
            {
                // Solid pixel
                *p = *src;
            }
            else if (alpha)
            {
                // Non-Transparent pixel
                *p = mixColors(*src, *p, alpha);
            }
            p++;
            src++;
        }
        while (--count != 0);
    }
    else
    {
        do
        {
            uint8_t srcAlpha = ((*src) >> 6) * 0x55;
            uint8_t alpha = LCD::div255((*covers) * srcAlpha);
            covers++;
            if (alpha) // This can never get to max=0xFF*0xFF as totalAlpha<255
            {
                // Non-Transparent pixel
                *p = mixColors(*src, *p, alpha);
            }
            p++;
            src++;
        }
        while (--count != 0);
    }
}

bool PainterARGB2222Bitmap::renderInit()
{
    bitmapARGB2222Pointer = 0;

    if (bitmap.getId() == BITMAP_INVALID)
    {
        return false;
    }

    if ((currentX >= bitmapRectToFrameBuffer.width) ||
            (currentY >= bitmapRectToFrameBuffer.height))
    {
        // Outside bitmap area, do not draw anything
        // Consider the following instead of "return" to get a tiled image:
        //   currentX %= bitmapRectToFrameBuffer.width
        //   currentY %= bitmapRectToFrameBuffer.height
        return false;
    }

    if (bitmap.getFormat() == Bitmap::ARGB2222)
    {
        bitmapARGB2222Pointer = bitmap.getData();
        if (!bitmapARGB2222Pointer)
        {
            return false;
        }
        bitmapARGB2222Pointer += currentX + currentY * bitmapRectToFrameBuffer.width;
        return true;
    }

    return false;
}

bool PainterARGB2222Bitmap::renderNext(uint8_t& red, uint8_t& green, uint8_t& blue, uint8_t& alpha)
{
    if (currentX >= bitmapRectToFrameBuffer.width)
    {
        return false;
    }
    else if (bitmapARGB2222Pointer != 0)
    {
        uint16_t ARGB2222 = *bitmapARGB2222Pointer++;
        red = LCD8bpp_ARGB2222::getRedFromColor(ARGB2222);
        green = LCD8bpp_ARGB2222::getGreenFromColor(ARGB2222);
        blue = LCD8bpp_ARGB2222::getBlueFromColor(ARGB2222);
        alpha = (ARGB2222 >> 6) * 0x55; // To get full range 0-0xFF
    }
    // Apply given alpha from setAlpha()
    alpha = LCD::div255(alpha * painterAlpha);
    return true;
}
} // namespace touchgfx
