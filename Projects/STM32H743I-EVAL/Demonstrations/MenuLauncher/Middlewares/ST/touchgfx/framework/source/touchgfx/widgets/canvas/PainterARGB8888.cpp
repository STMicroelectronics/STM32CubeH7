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

#include <touchgfx/widgets/canvas/PainterARGB8888.hpp>
#include <touchgfx/Color.hpp>

namespace touchgfx
{
PainterARGB8888::PainterARGB8888(colortype color, uint8_t alpha) :
    AbstractPainterARGB8888()
{
    setColor(color, alpha);
}

void PainterARGB8888::setColor(colortype color, uint8_t alpha)
{
    painterRed = Color::getRedColor(color);
    painterGreen = Color::getGreenColor(color);
    painterBlue = Color::getBlueColor(color);
    setAlpha(alpha);
}

touchgfx::colortype PainterARGB8888::getColor() const
{
    return Color::getColorFrom24BitRGB(painterRed, painterGreen, painterBlue);
}

void PainterARGB8888::setAlpha(uint8_t alpha)
{
    painterAlpha = alpha;
}

uint8_t PainterARGB8888::getAlpha() const
{
    return painterAlpha;
}

void PainterARGB8888::render(uint8_t* ptr, int x, int xAdjust, int /*y*/, unsigned count, const uint8_t* covers)
{
    uint8_t* p = reinterpret_cast<uint8_t*>(ptr) + ((x + xAdjust) * 4);
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
                *p++ = 0xff;
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
                pByte = *p;
                *p++ = pByte + alpha - LCD::div255(pByte * alpha);
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
            pByte = *p;
            *p++ = pByte + alpha - LCD::div255(pByte * alpha);
        }
        while (--count != 0);
    }
}

bool PainterARGB8888::renderNext(uint8_t& red, uint8_t& green, uint8_t& blue, uint8_t& alpha)
{
    red = painterRed;
    green = painterGreen;
    blue = painterBlue;
    alpha = painterAlpha;
    return true;
}
} // namespace touchgfx
