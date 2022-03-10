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

#include <touchgfx/widgets/RepeatButton.hpp>

namespace touchgfx
{
RepeatButton::RepeatButton() : Button(), ticksDelay(30), ticksInterval(15), ticks(0), ticksBeforeContinuous(0)
{
}

void RepeatButton::setDelay(int delay)
{
    ticksDelay = delay;
}

int RepeatButton::getDelay()
{
    return ticksDelay;
}

void RepeatButton::setInterval(int interval)
{
    ticksInterval = interval;
}

int RepeatButton::getInterval()
{
    return ticksInterval;
}

void RepeatButton::handleClickEvent(const touchgfx::ClickEvent& event)
{
    pressed = false; // To prevent AbstractButton from calling action->execute().
    invalidate(); // Force redraw after forced state change
    Button::handleClickEvent(event);
    if (event.getType() == touchgfx::ClickEvent::PRESSED)
    {
        if (action && action->isValid())
        {
            action->execute(*this);
        }
        ticks = 0;
        ticksBeforeContinuous = ticksDelay;
        touchgfx::Application::getInstance()->registerTimerWidget(this);
    }
    else
    {
        touchgfx::Application::getInstance()->unregisterTimerWidget(this);
    }
}

void RepeatButton::handleTickEvent()
{
    Button::handleTickEvent();

    if (pressed)
    {
        if (ticks == ticksBeforeContinuous)
        {
            if (action && action->isValid())
            {
                action->execute(*this);
            }

            ticks = 0;
            ticksBeforeContinuous = ticksInterval;
        }
        else
        {
            ticks++;
        }
    }
}
} // namespace touchgfx
