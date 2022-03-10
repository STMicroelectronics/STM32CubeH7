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

#include <touchgfx/widgets/canvas/PainterRGB565.hpp>
#include <touchgfx/Color.hpp>

namespace touchgfx
{
PainterRGB565::PainterRGB565(colortype color, uint8_t alpha) :
    AbstractPainterRGB565()
{
    setColor(color, alpha);
}

void PainterRGB565::setColor(colortype color, uint8_t alpha)
{
    painterColor = (uint16_t)color;
    painterRed = painterColor & RMASK;
    painterGreen = painterColor & GMASK;
    painterBlue = painterColor & BMASK;
    setAlpha(alpha);
}

touchgfx::colortype PainterRGB565::getColor() const
{
    return painterColor;
}

void PainterRGB565::setAlpha(uint8_t alpha)
{
    painterAlpha = alpha;
}

uint8_t PainterRGB565::getAlpha() const
{
    return painterAlpha;
}

void PainterRGB565::render(uint8_t* ptr, int x, int xAdjust, int /*y*/, unsigned count, const uint8_t* covers)
{
    uint16_t* p = reinterpret_cast<uint16_t*>(ptr) + (x + xAdjust);
    uint8_t totalAlpha = LCD::div255(widgetAlpha * painterAlpha);
    if (totalAlpha == 0xFF)
    {
        do
        {
            uint8_t alpha = *covers;
            covers++;
            if (alpha == 0xFF)
            {
                *p = painterColor;
            }
            else
            {
                *p = mixColors(painterRed, painterGreen, painterBlue, *p, alpha);
            }
            p++;
        }
        while (--count != 0);
    }
    else
    {
        do
        {
            uint8_t alpha = LCD::div255((*covers) * totalAlpha);
            covers++;
            if (alpha == 0xFF)
            {
                *p = painterColor;
            }
            else
            {
                *p = mixColors(painterRed, painterGreen, painterBlue, *p, alpha);
            }
            p++;
        }
        while (--count != 0);
    }
}

bool PainterRGB565::renderNext(uint8_t& red, uint8_t& green, uint8_t& blue, uint8_t& alpha)
{
    red = painterRed >> 8;
    green = painterGreen >> 3;
    blue = painterBlue << 3;
    alpha = painterAlpha;
    return true;
}
} // namespace touchgfx
