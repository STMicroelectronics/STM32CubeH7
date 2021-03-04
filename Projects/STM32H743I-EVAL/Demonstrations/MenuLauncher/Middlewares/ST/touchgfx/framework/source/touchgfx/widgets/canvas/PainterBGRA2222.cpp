/**
  ******************************************************************************
  * This file is part of the TouchGFX 4.12.3 distribution.
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

#include <platform/driver/lcd/LCD8bpp_BGRA2222.hpp>
#include <touchgfx/Color.hpp>
#include <touchgfx/widgets/canvas/PainterBGRA2222.hpp>

namespace touchgfx
{
PainterBGRA2222::PainterBGRA2222(colortype color, uint8_t alpha) :
    AbstractPainterBGRA2222()
{
    setColor(color, alpha);
}

void PainterBGRA2222::setColor(colortype color, uint8_t alpha)
{
    painterColor = (uint8_t)color;
    painterRed = LCD8bpp_BGRA2222::getRedFromColor(color);
    painterGreen = LCD8bpp_BGRA2222::getGreenFromColor(color);
    painterBlue = LCD8bpp_BGRA2222::getBlueFromColor(color);
    setAlpha(alpha);
}

touchgfx::colortype PainterBGRA2222::getColor() const
{
    return painterColor;
}

void PainterBGRA2222::setAlpha(uint8_t alpha)
{
    painterAlpha = alpha;
}

uint8_t PainterBGRA2222::getAlpha() const
{
    return painterAlpha;
}

void PainterBGRA2222::render(uint8_t* ptr, int x, int xAdjust, int /*y*/, unsigned count, const uint8_t* covers)
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

bool PainterBGRA2222::renderNext(uint8_t& red, uint8_t& green, uint8_t& blue, uint8_t& alpha)
{
    red = painterRed;
    green = painterGreen;
    blue = painterBlue;
    alpha = painterAlpha;
    return true;
}
} // namespace touchgfx
