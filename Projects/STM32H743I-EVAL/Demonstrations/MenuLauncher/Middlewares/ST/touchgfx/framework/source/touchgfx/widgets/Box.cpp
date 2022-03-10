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

#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/hal/HAL.hpp>

namespace touchgfx
{
Rect Box::getSolidRect() const
{
    Rect solidRect(0, 0, 0, 0);
    if (reportAsSolid || alpha == 255)
    {
        solidRect.width = rect.width;
        solidRect.height = rect.height;
    }
    return solidRect;
}

void Box::draw(const Rect& area) const
{
    Rect dirty = area;
    translateRectToAbsolute(dirty);
    HAL::lcd().fillRect(dirty, color, alpha);
}
} // namespace touchgfx
