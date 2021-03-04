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

#ifndef REPEATBUTTON_HPP
#define REPEATBUTTON_HPP

#include <touchgfx/widgets/Button.hpp>

namespace touchgfx
{
/**
* @class RepeatButton RepeatButton.hpp touchgfx/widgets/RepeatButton.hpp
 *
 * @brief A button with two states.
 *
 *        A button consists of two images, one for its normal state and one when it is pressed
 *        down. The button activates its pressed action immediately, the after a given delay
 *        and then repeatedly after an interval.
 *
 * @see Button
 */
class RepeatButton : public touchgfx::Button
{
public:

    /**
     * @fn RepeatButton::RepeatButton();
     *
     * @brief Default constructor.
     *
     *        Default constructor. Sets delay to 10 ticks and interval to 5 ticks.
     *
     * @see setDelay
     * @see setInterval
     */
    RepeatButton();

    /**
     * @fn virtual void RepeatButton::setDelay(int delay);
     *
     * @brief Sets the delay.
     *
     *        Sets the number of ticks from the first button activation until the next time it gets activated.
     *
     * @param delay The delay.
     *
     @see setInterval
     @see getDelay
     */
    virtual void setDelay(int delay);

    /**
     * @fn virtual int RepeatButton::getDelay();
     *
     * @brief Gets the delay.
     *
     Gets the delay in ticks.
     *
     * @return The delay.
     *
     * @see setDelay
     */
    virtual int getDelay();

    /**
     * @fn virtual void RepeatButton::setInterval(int interval);
     *
     * @brief Sets the interval.
     *
     *        Sets the interval in number of ticks between each each activation of the pressed
     *        button.
     *
     * @param interval The interval.
     *
     * @see setDelay
     * @see getInterval
     */
    virtual void setInterval(int interval);

    /**
     * @fn virtual int RepeatButton::getInterval();
     *
     * @brief Gets the interval.
     *
     *        Gets the interval in ticks.
     *
     * @return The interval.
     */
    virtual int getInterval();

    /**
     * @fn virtual void RepeatButton::handleClickEvent(const touchgfx::ClickEvent& event);
     *
     * @brief Handles the click event.
     *
     *        Handles the click event by immediately activating the button and then setting up
     *        a timer to repeatedly activate the button.
     *
     * @param event The event.
     */
    virtual void handleClickEvent(const touchgfx::ClickEvent& event);

    /**
     * @fn virtual void RepeatButton::handleTickEvent();
     *
     * @brief Handles the tick event.
     *
     *        Handles the tick event that takes care of counting down until the next time the
     *        buttons should be activated.
     */
    virtual void handleTickEvent();

private:
    int16_t ticksDelay;
    int16_t ticksInterval;

    int16_t ticks;
    int16_t ticksBeforeContinuous;
};
} // namespace touchgfx

#endif // REPEATBUTTON_HPP
