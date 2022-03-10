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

#include <touchgfx/containers/scrollers/ScrollWheel.hpp>

namespace touchgfx
{
ScrollWheel::ScrollWheel()
    : ScrollWheelBase()
{
}

ScrollWheel::~ScrollWheel()
{
}

void ScrollWheel::setDrawables(DrawableListItemsInterface& drawableListItems, GenericCallback<DrawableListItemsInterface*, int16_t, int16_t>& updateDrawableCallback)
{
    stopAnimation();
    numberOfDrawables = drawableListItems.getNumberOfDrawables();

    list.setDrawables(drawableListItems, 0, updateDrawableCallback);

    setOffset(0);
}
} // namespace touchgfx
