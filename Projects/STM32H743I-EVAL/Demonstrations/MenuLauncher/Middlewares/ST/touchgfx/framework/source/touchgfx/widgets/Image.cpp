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

#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/hal/HAL.hpp>

namespace touchgfx
{
void Image::setBitmap(const Bitmap& bmp)
{
    bitmap = bmp;
    // When setting bitmap, adjust size of this widget to match.
    setWidth(bitmap.getWidth());
    setHeight(bitmap.getHeight());

    // This bool is no longer used, but maintained for backwards compat.
    hasTransparentPixels = bitmap.hasTransparentPixels();
}

void Image::draw(const Rect& invalidatedArea) const
{
    Rect meAbs;
    translateRectToAbsolute(meAbs); //To find our x and y coords in absolute.

    // Calculate intersection between bitmap rect and invalidated area.
    Rect dirtyBitmapArea = bitmap.getRect() & invalidatedArea;

    if (!dirtyBitmapArea.isEmpty())
    {
        HAL::lcd().drawPartialBitmap(bitmap, meAbs.x, meAbs.y, dirtyBitmapArea, alpha);
    }
}

Rect Image::getSolidRect() const
{
    // If alpha is less than solid, we have an empty solid rect.
    if (alpha < 255)
    {
        return Rect(0, 0, 0, 0);
    }
    // Return solid rect from bitmap (precalculated).
    return bitmap.getSolidRect();
}
} // namespace touchgfx
