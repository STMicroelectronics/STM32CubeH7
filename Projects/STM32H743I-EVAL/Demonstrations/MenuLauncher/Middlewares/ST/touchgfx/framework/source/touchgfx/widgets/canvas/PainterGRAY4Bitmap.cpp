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

#include <touchgfx/widgets/canvas/PainterGRAY4Bitmap.hpp>
#include <platform/driver/lcd/LCD4bpp.hpp>

namespace touchgfx
{
PainterGRAY4Bitmap::PainterGRAY4Bitmap(const Bitmap& bmp, uint8_t alpha) :
    AbstractPainterGRAY4(), bitmapGRAY4Pointer(0), bitmapAlphaPointer(0)
{
    setBitmap(bmp);
    setAlpha(alpha);
}

void PainterGRAY4Bitmap::setBitmap(const Bitmap& bmp)
{
    bitmap = bmp;
    assert((bitmap.getId() == BITMAP_INVALID || bitmap.getFormat() == Bitmap::GRAY4) && "The chosen painter only works with GRAY4 bitmaps");
    bitmapRectToFrameBuffer = bitmap.getRect();
    DisplayTransformation::transformDisplayToFrameBuffer(bitmapRectToFrameBuffer);
}

void PainterGRAY4Bitmap::setAlpha(uint8_t alpha)
{
    painterAlpha = alpha;
}

uint8_t PainterGRAY4Bitmap::getAlpha() const
{
    return painterAlpha;
}

void PainterGRAY4Bitmap::render(uint8_t* ptr, int x, int xAdjust, int y, unsigned count, const uint8_t* covers)
{
    currentX = x + areaOffsetX;
    currentY = y + areaOffsetY;
    x += xAdjust;

    if (!renderInit())
    {
        return;
    }

    if (currentX + (int)count > bitmapRectToFrameBuffer.width)
    {
        count = bitmapRectToFrameBuffer.width - currentX;
    }

    uint8_t totalAlpha = LCD::div255(widgetAlpha * painterAlpha);
    // Get alpha data (GRAY4 format)
    if (bitmapAlphaPointer)
    {
        if (totalAlpha == 0xFF)
        {
            do
            {
                uint8_t gray = LCD4getPixel(bitmapGRAY4Pointer, currentX);
                uint8_t alpha = LCD::div255((*covers) * (LCD4getPixel(bitmapAlphaPointer, currentX) * 0x11));
                covers++;

                if (alpha == 0xFF)
                {
                    // Render a solid pixel
                    LCD4setPixel(ptr, x, gray);
                }
                else
                {
                    uint8_t ialpha = 0xFF - alpha;
                    uint8_t p_gray = LCD4getPixel(ptr, x);
                    LCD4setPixel(ptr, x, LCD::div255((gray * alpha + p_gray * ialpha) * 0x11) >> 4);
                }
                currentX++;
                x++;
            }
            while (--count != 0);
        }
        else
        {
            do
            {
                uint8_t gray = LCD4getPixel(bitmapGRAY4Pointer, currentX);
                uint8_t alpha = LCD::div255((*covers) * LCD::div255(totalAlpha * (LCD4getPixel(bitmapAlphaPointer, currentX) * 0x11)));
                uint8_t ialpha = 0xFF - alpha;
                covers++;

                uint8_t p_gray = LCD4getPixel(ptr, x);
                LCD4setPixel(ptr, x, LCD::div255((gray * alpha + p_gray * ialpha) * 0x11) >> 4);
                currentX++;
                x++;
            }
            while (--count != 0);
        }
    }
    else
    {
        if (totalAlpha == 255)
        {
            do
            {
                uint8_t gray = LCD4getPixel(bitmapGRAY4Pointer, currentX);
                uint8_t alpha = (*covers);
                covers++;

                if (alpha == 0xFF)
                {
                    // Render a solid pixel
                    LCD4setPixel(ptr, x, gray);
                }
                else
                {
                    uint8_t ialpha = 0xFF - alpha;
                    uint8_t p_gray = LCD4getPixel(ptr, x);
                    LCD4setPixel(ptr, x, LCD::div255((gray * alpha + p_gray * ialpha) * 0x11) >> 4);
                }
                currentX++;
                x++;
            }
            while (--count != 0);
        }
        else
        {
            do
            {
                uint8_t gray = LCD4getPixel(bitmapGRAY4Pointer, currentX);
                uint8_t alpha = LCD::div255((*covers) * totalAlpha);
                uint8_t ialpha = 0xFF - alpha;
                covers++;

                uint8_t p_gray = LCD4getPixel(ptr, x);
                LCD4setPixel(ptr, x, LCD::div255((gray * alpha + p_gray * ialpha) * 0x11) >> 4);
                currentX++;
                x++;
            }
            while (--count != 0);
        }
    }
}

bool PainterGRAY4Bitmap::renderInit()
{
    bitmapGRAY4Pointer = 0;
    bitmapAlphaPointer = 0;

    if (bitmap.getId() == BITMAP_INVALID)
    {
        return false;
    }

    if ((currentX >= bitmapRectToFrameBuffer.width) ||
            (currentY >= bitmapRectToFrameBuffer.height))
    {
        // Outside bitmap area, do not draw anything
        return false;
    }

    if (bitmap.getFormat() == Bitmap::GRAY4)
    {
        bitmapGRAY4Pointer = (const uint8_t*)bitmap.getData();
        if (!bitmapGRAY4Pointer)
        {
            return false;
        }
        bitmapGRAY4Pointer += currentY * ((bitmapRectToFrameBuffer.width + 1) / 2);
        bitmapAlphaPointer = (const uint8_t*)bitmap.getExtraData();
        if (bitmapAlphaPointer)
        {
            bitmapAlphaPointer += currentY * ((bitmapRectToFrameBuffer.width + 1) / 2);
        }
        return true;
    }

    return false;
}

bool PainterGRAY4Bitmap::renderNext(uint8_t& gray, uint8_t& alpha)
{
    if (currentX >= bitmapRectToFrameBuffer.width)
    {
        return false;
    }

    if (bitmapGRAY4Pointer != 0)
    {
        gray = LCD4getPixel(bitmapGRAY4Pointer, currentX);
        if (bitmapAlphaPointer)
        {
            alpha = LCD4getPixel(bitmapAlphaPointer, currentX);
            alpha |= alpha << 4; // Upscale from 0-15 to 0-255
        }
        else
        {
            alpha = 0xFF; // No alpha per pixel in the image, it is solid
        }
    }
    // Apply given alpha from setAlpha()
    alpha = LCD::div255(alpha * painterAlpha);
    return true;
}
} // namespace touchgfx
