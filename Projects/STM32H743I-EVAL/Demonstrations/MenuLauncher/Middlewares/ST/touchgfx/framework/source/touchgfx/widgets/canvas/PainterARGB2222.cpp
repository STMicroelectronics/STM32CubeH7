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

#include <platform/driver/lcd/LCD8bpp_ARGB2222.hpp>
#include <touchgfx/Color.hpp>
#include <touchgfx/widgets/canvas/PainterARGB2222.hpp>

namespace touchgfx
{
PainterARGB2222::PainterARGB2222(colortype color, uint8_t alpha) :
    AbstractPainterARGB2222()
{
    setColor(color, alpha);
}

void PainterARGB2222::setColor(colortype color, uint8_t alpha)
{
    painterColor = (uint8_t)color;
    painterRed = LCD8bpp_ARGB2222::getRedFromColor(color);
    painterGreen = LCD8bpp_ARGB2222::getGreenFromColor(color);
    painterBlue = LCD8bpp_ARGB2222::getBlueFromColor(color);
    setAlpha(alpha);
}

touchgfx::colortype PainterARGB2222::getColor() const
{
    return painterColor;
}

void PainterARGB2222::setAlpha(uint8_t alpha)
{
    painterAlpha = alpha;
}

uint8_t PainterARGB2222::getAlpha() const
{
    return painterAlpha;
}

void PainterARGB2222::render(uint8_t* ptr, int x, int xAdjust, int /*y*/, unsigned count, const uint8_t* covers)
{
    uint8_t* p = ptr + (x + xAdjust);
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

bool PainterARGB2222::renderNext(uint8_t& red, uint8_t& green, uint8_t& blue, uint8_t& alpha)
{
    red = painterRed;
    green = painterGreen;
    blue = painterBlue;
    alpha = painterAlpha;
    return true;
}
} // namespace touchgfx
