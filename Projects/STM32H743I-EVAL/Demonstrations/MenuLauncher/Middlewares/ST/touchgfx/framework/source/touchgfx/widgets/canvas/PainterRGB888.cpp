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

#include <touchgfx/widgets/canvas/PainterRGB888.hpp>
#include <touchgfx/Color.hpp>

namespace touchgfx
{
PainterRGB888::PainterRGB888(colortype color, uint8_t alpha) :
    AbstractPainterRGB888()
{
    setColor(color, alpha);
}

void PainterRGB888::setColor(colortype color, uint8_t alpha)
{
    painterRed = Color::getRedColor(color);
    painterGreen = Color::getGreenColor(color);
    painterBlue = Color::getBlueColor(color);
    setAlpha(alpha);
}

touchgfx::colortype PainterRGB888::getColor() const
{
    return Color::getColorFrom24BitRGB(painterRed, painterGreen, painterBlue);
}

void PainterRGB888::setAlpha(uint8_t alpha)
{
    painterAlpha = alpha;
}

uint8_t PainterRGB888::getAlpha() const
{
    return painterAlpha;
}

void PainterRGB888::render(uint8_t* ptr, int x, int xAdjust, int /*y*/, unsigned count, const uint8_t* covers)
{
    uint8_t* p = reinterpret_cast<uint8_t*>(ptr) + ((x + xAdjust) * 3);
    uint8_t pByte;
    uint8_t totalAlpha = LCD::div255(widgetAlpha * painterAlpha);
    if (totalAlpha == 0xFF)
    {
        do
        {
            uint8_t alpha = *covers++;
            if (alpha == 0xFF)
            {
                *p++ = painterBlue;
                *p++ = painterGreen;
                *p++ = painterRed;
            }
            else
            {
                uint8_t ialpha = 0xFF - alpha;
                pByte = *p;
                *p++ = LCD::div255(painterBlue * alpha + pByte * ialpha);
                pByte = *p;
                *p++ = LCD::div255(painterGreen * alpha + pByte * ialpha);
                pByte = *p;
                *p++ = LCD::div255(painterRed * alpha + pByte * ialpha);
            }
        }
        while (--count != 0);
    }
    else if (totalAlpha != 0)
    {
        do
        {
            uint8_t alpha = LCD::div255(*covers++ * totalAlpha);
            uint8_t ialpha = 0xFF - alpha;
            pByte = *p;
            *p++ = LCD::div255(painterBlue * alpha + pByte * ialpha);
            pByte = *p;
            *p++ = LCD::div255(painterGreen * alpha + pByte * ialpha);
            pByte = *p;
            *p++ = LCD::div255(painterRed * alpha + pByte * ialpha);
        }
        while (--count != 0);
    }
}

bool PainterRGB888::renderNext(uint8_t& red, uint8_t& green, uint8_t& blue, uint8_t& alpha)
{
    red = painterRed;
    green = painterGreen;
    blue = painterBlue;
    alpha = painterAlpha;
    return true;
}
} // namespace touchgfx
