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

#include <touchgfx/widgets/AbstractButton.hpp>

namespace touchgfx
{
void AbstractButton::handleClickEvent(const ClickEvent& event)
{
    bool wasPressed = pressed;
    pressed = (event.getType() == ClickEvent::PRESSED);
    if ((pressed && !wasPressed) || (!pressed && wasPressed))
    {
        // Pressed state changed, so invalidate
        invalidate();
    }
    if (wasPressed && (event.getType() == ClickEvent::RELEASED) && action)
    {
        // This is a click. Fire callback.
        if (action->isValid())
        {
            action->execute(*this);
        }
    }
}
} // namespace touchgfx
