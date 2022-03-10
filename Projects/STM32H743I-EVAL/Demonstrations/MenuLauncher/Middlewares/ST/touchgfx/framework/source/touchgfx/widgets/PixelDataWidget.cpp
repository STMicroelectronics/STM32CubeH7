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

#include <touchgfx/widgets/PixelDataWidget.hpp>
#include <touchgfx/hal/HAL.hpp>

namespace touchgfx
{
PixelDataWidget::PixelDataWidget() :
    Widget(),
    buffer(0),
    format(Bitmap::RGB888),
    alpha(255)
{
}

void PixelDataWidget::setPixelData(uint8_t* const data)
{
    buffer = data;
}

void PixelDataWidget::setBitmapFormat(Bitmap::BitmapFormat f)
{
    format = f;
}

void PixelDataWidget::setAlpha(uint8_t a)
{
    this->alpha = a;
}

uint8_t PixelDataWidget::getAlpha() const
{
    return alpha;
}

void PixelDataWidget::draw(const touchgfx::Rect& invalidatedArea) const
{
    if (buffer != 0)
    {
        //convert to lcd coordinates
        const touchgfx::Rect absolute = getAbsoluteRect();
        //copy to LCD
        HAL::lcd().blitCopy(buffer, format, absolute, invalidatedArea, alpha, false);
    }
}

touchgfx::Rect PixelDataWidget::getSolidRect() const
{
    Rect solidRect(0, 0, 0, 0);
    if (alpha == 255)
    {
        // There are at least some solid pixels
        switch (format)
        {
        case Bitmap::BW:       ///< 1-bit, black / white, no alpha channel
        case Bitmap::BW_RLE:   ///< 1-bit, black / white, no alpha channel compressed with horizontal RLE
        case Bitmap::GRAY2:    ///< 2-bit, gray scale, no alpha channel
        case Bitmap::GRAY4:    ///< 4-bit, gray scale, no alpha channel
        case Bitmap::RGB565:   ///< 16-bit, 5 bits for red, 6 bits for green, 5 bits for blue, no alpha channel
        case Bitmap::RGB888:   ///< 16-bit, 5 bits for red, 6 bits for green, 5 bits for blue, no alpha channel
            //All solid pixels
            solidRect.width = getWidth();
            solidRect.height = getHeight();
            break;
        case Bitmap::ARGB8888: ///< 32-bit, 8 bits for each of red, green, blue and alpha channel
        case Bitmap::ARGB2222: ///< 8-bit color
        case Bitmap::ABGR2222: ///< 8-bit color
        case Bitmap::RGBA2222: ///< 8-bit color
        case Bitmap::BGRA2222: ///< 8-bit color
        case Bitmap::L8:       ///< 8-bit indexed color
            //No knowledge about solid pixels
            break;
        }
    }
    return solidRect;
}
} // namespace touchgfx
