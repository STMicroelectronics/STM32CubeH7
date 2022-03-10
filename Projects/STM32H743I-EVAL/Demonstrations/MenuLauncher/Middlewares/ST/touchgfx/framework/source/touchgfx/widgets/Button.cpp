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

#include <touchgfx/widgets/Button.hpp>
#include <touchgfx/hal/HAL.hpp>

namespace touchgfx
{
void Button::draw(const Rect& invalidatedArea) const
{
    Bitmap bmp(AbstractButton::pressed ? down : up);
    Rect dirty(0, 0, bmp.getWidth(), bmp.getHeight());
    dirty &= invalidatedArea;
    if ((bmp.getId() != BITMAP_INVALID) && !dirty.isEmpty())
    {
        Rect r;
        translateRectToAbsolute(r);
        HAL::lcd().drawPartialBitmap(bmp, r.x, r.y, dirty, alpha);
    }
}

void Button::setBitmaps(const Bitmap& bmpReleased, const Bitmap& bmpPressed)
{
    up = bmpReleased;
    down = bmpPressed;
    // Adjust width and height of this widget to match bitmap. It is assumed
    // that the two bitmaps have same dimensions.
    Drawable::setWidth(down.getWidth());
    Drawable::setHeight(down.getHeight());
}

Rect Button::getSolidRect() const
{
    if (alpha < 255)
    {
        return Rect(0, 0, 0, 0);
    }

    return (AbstractButton::pressed ? down.getSolidRect() : up.getSolidRect());
}
} // namespace touchgfx
