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

#include <touchgfx/widgets/BoxWithBorder.hpp>
#include <touchgfx/hal/HAL.hpp>

namespace touchgfx
{
Rect BoxWithBorder::getSolidRect() const
{
    Rect solidRect(0, 0, 0, 0);
    if (alpha == 255)
    {
        solidRect.width = rect.width;
        solidRect.height = rect.height;
    }
    return solidRect;
}

void BoxWithBorder::draw(const Rect& area) const
{
    Rect dirty = area & Rect(borderSize, borderSize, getWidth() - (2 * borderSize), getHeight() - (2 * borderSize));
    translateRectToAbsolute(dirty);
    HAL::lcd().fillRect(dirty, color, alpha);

    if (borderSize > 0)
    {
        Rect borders[4] =
        {
            Rect(0, 0, getWidth(), borderSize),                                                     // Upper
            Rect(0, getHeight() - borderSize, getWidth(), borderSize),                              // lower
            Rect(0, borderSize, borderSize, getHeight() - (2 * borderSize)),                        // left
            Rect(getWidth() - borderSize, borderSize, borderSize, getHeight() - (2 * borderSize))   // right
        };

        for (int i = 0; i < 4; i++)
        {
            Rect borderDirty = borders[i] & area;
            if (!borderDirty.isEmpty())
            {
                translateRectToAbsolute(borderDirty);
                HAL::lcd().fillRect(borderDirty, borderColor, alpha);
            }
        }
    }
}
}
