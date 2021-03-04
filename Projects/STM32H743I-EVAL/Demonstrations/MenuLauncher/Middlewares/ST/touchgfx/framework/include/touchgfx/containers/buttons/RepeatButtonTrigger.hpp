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

#ifndef REPEATBUTTONTRIGGER_HPP
#define REPEATBUTTONTRIGGER_HPP

#include <touchgfx/containers/buttons/AbstractButtonContainer.hpp>

namespace touchgfx
{
/**
 * @class RepeatButtonTrigger RepeatButtonTrigger.hpp touchgfx/containers/buttons/RepeatButtonTrigger.hpp
 *
 * @brief A repeat button trigger.
 *
 *        A repeat button trigger. This trigger will create a button
 *        that reacts to a consistent touch. This means it will call the action repeatedly
 *        as long as it is touched.
 *
 *        The RepeatButtonTrigger can be combined with one or more of
 *        the ButtonStyle classes to create a functional button.
 */
class RepeatButtonTrigger : public AbstractButtonContainer
{
public:
    /**
     * @fn RepeatButtonTrigger::RepeatButtonTrigger()
     *
     * @brief Default constructor.
     */
    RepeatButtonTrigger() :
        AbstractButtonContainer(), ticksDelay(30), ticksInterval(15), ticks(0), ticksBeforeContinuous(0) { }

    /**
     * @fn virtual RepeatButtonTrigger::~RepeatButtonTrigger()
     *
     * @brief Destructor.
     */
    virtual ~RepeatButtonTrigger() { }

    /**
     * @fn void RepeatButtonTrigger::setDelay(int delay)
     *
     * @brief Sets a delay.
     *
     * @param delay The delay.
     */
    void setDelay(int delay)
    {
        ticksDelay = delay;
    }

    /**
     * @fn int RepeatButtonTrigger::getDelay()
     *
     * @brief Gets the delay.
     *
     * @return The delay.
     */
    int getDelay()
    {
        return ticksDelay;
    }

    /**
     * @fn void RepeatButtonTrigger::setInterval(int interval)
     *
     * @brief Sets an interval.
     *
     * @param interval The interval.
     */
    void setInterval(int interval)
    {
        ticksInterval = interval;
    }

    /**
     * @fn int RepeatButtonTrigger::getInterval()
     *
     * @brief Gets the interval.
     *
     * @return The interval.
     */
    int getInterval()
    {
        return ticksInterval;
    }

    /**
     * @fn void RepeatButtonTrigger::handleClickEvent(const touchgfx::ClickEvent& event)
     *
     * @brief Handles the click event described by event.
     *
     * @param event The event.
     */
    void handleClickEvent(const touchgfx::ClickEvent& event)
    {
        bool wasPressed = getPressed();
        bool newPressedValue = (event.getType() == ClickEvent::PRESSED);
        if ((newPressedValue && !wasPressed) || (!newPressedValue && wasPressed))
        {
            setPressed(newPressedValue);
            invalidate();
        }

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

    /**
     * @fn void RepeatButtonTrigger::handleTickEvent()
     *
     * @brief Handles the tick event.
     */
    void handleTickEvent()
    {
        AbstractButtonContainer::handleTickEvent();

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

private:
    int16_t ticksDelay;
    int16_t ticksInterval;

    int16_t ticks;
    int16_t ticksBeforeContinuous;
};
} // namespace touchgfx

#endif // REPEATBUTTONTRIGGER_HPP
