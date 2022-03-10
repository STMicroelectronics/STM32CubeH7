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

#include <touchgfx/widgets/canvas/AbstractPainterBW.hpp>

namespace touchgfx
{
void AbstractPainterBW::render(uint8_t* ptr,
                               int x,
                               int xAdjust,
                               int y,
                               unsigned count,
                               const uint8_t* /*covers*/)
{
    currentX = x + areaOffsetX;
    currentY = y + areaOffsetY;
    x += xAdjust;
    unsigned char* p = ptr + (x / 8);

    if (!renderInit())
    {
        return;
    }

    do
    {
        uint8_t color;
        if (renderNext(color))
        {
            unsigned pixel = 1 << (7 - (x % 8));
            if (!color)
            {
                *p &= ~pixel;
            }
            else
            {
                *p |= pixel;
            }
        }
        if (((++x) % 8) == 0)
        {
            p++;
        }
        currentX++;
    }
    while (--count);
}
} // namespace touchgfx
