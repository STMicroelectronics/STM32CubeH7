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

#ifndef ABSTRACTBUTTON_HPP
#define ABSTRACTBUTTON_HPP

#include <touchgfx/widgets/Widget.hpp>
#include <touchgfx/Callback.hpp>

namespace touchgfx
{
/**
 * @class AbstractButton AbstractButton.hpp touchgfx/widgets/AbstractButton.hpp
 *
 * @brief This class defines an abstract interface for button-like elements.
 *
 *        This class defines an abstract interface for button-like elements. A button is a
 *        clickable element that has two states - pressed or released - and executes an action
 *        when the pressed->released transition is made.
 *
 * @see Widget
 */
class AbstractButton : public Widget
{
public:

    /**
     * @fn AbstractButton::AbstractButton()
     *
     * @brief Constructor.
     *
     *        Constructs an AbstractButton instance in released state without an associated
     *        action.
     */
    AbstractButton() : Widget(), action(), pressed(false)
    {
        setTouchable(true);
    }

    /**
     * @fn virtual void AbstractButton::handleClickEvent(const ClickEvent& event);
     *
     * @brief Updates the current state of the button.
     *
     *        Updates the current state of the button - pressed or released - and invalidates
     *        it.
     *
     *        If a transition from the pressed to the released state was made, the associated
     *        action is executed and then the Widget is invalidated.
     *
     * @param event Information about the click.
     *
     * @see Drawable::handleClickEvent()
     */
    virtual void handleClickEvent(const ClickEvent& event);

    /**
     * @fn void AbstractButton::setAction(GenericCallback< const AbstractButton& >& callback)
     *
     * @brief Associates an action to be performed when the AbstractButton is clicked.
     *
     *        Associates an action to be performed when the AbstractButton is clicked.
     *
     * @param callback The callback to be executed. The callback will be given a reference to
     *                 the AbstractButton.
     *
     * @see GenericCallback
     */
    void setAction(GenericCallback< const AbstractButton& >& callback)
    {
        action = &callback;
    }

    /**
     * @fn virtual bool AbstractButton::getPressedState() const
     *
     * @brief Function to determine whether this AbstractButton is currently pressed.
     *
     *        Function to determine whether this AbstractButton is currently pressed.
     *
     * @return true if button is pressed, false otherwise.
     */
    virtual bool getPressedState() const
    {
        return pressed;
    }

    /**
     * @fn virtual uint16_t AbstractButton::getType() const
     *
     * @brief For GUI testing only.
     *
     *        For GUI testing only. Returns type of this drawable.
     *
     * @return TYPE_ABSTRACTBUTTON.
     */
    virtual uint16_t getType() const
    {
        return (uint16_t)TYPE_ABSTRACTBUTTON;
    }

protected:
    GenericCallback< const AbstractButton& >* action; ///< The callback to be executed when this AbstractButton is clicked

    bool pressed; ///< Is the button pressed or released? True if pressed.
};
} // namespace touchgfx

#endif // ABSTRACTBUTTON_HPP
