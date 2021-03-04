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

#ifndef TOUCHBUTTONTRIGGER_HPP
#define TOUCHBUTTONTRIGGER_HPP

#include <touchgfx/containers/buttons/AbstractButtonContainer.hpp>

namespace touchgfx
{
/**
 * @class TouchButtonTrigger TouchButtonTrigger.hpp touchgfx/containers/buttons/TouchButtonTrigger.hpp
 *
 * @brief A touch button trigger.
 *
 *        A touch button trigger. This trigger will create a button
 *        that reacts on touches. This means it will call the action
 *        when it gets a touch pressed event.
 *
 *        The TouchButtonTrigger can be combined with one or more of
 *        the ButtonStyle classes to create a functional button.
 */
class TouchButtonTrigger : public AbstractButtonContainer
{
public:
    /**
     * @fn TouchButtonTrigger::TouchButtonTrigger()
     *
     * @brief Default constructor.
     */
    TouchButtonTrigger() :
        AbstractButtonContainer() { }

    /**
     * @fn virtual TouchButtonTrigger::~TouchButtonTrigger()
     *
     * @brief Destructor.
     */
    virtual ~TouchButtonTrigger() { }

    /**
     * @fn virtual void TouchButtonTrigger::handleClickEvent(const ClickEvent& event)
     *
     * @brief Handles the click event described by event.
     *
     * @param event The event.
     */
    virtual void handleClickEvent(const ClickEvent& event)
    {
        bool wasPressed = getPressed();
        bool newPressedValue = (event.getType() == ClickEvent::PRESSED);
        if ((newPressedValue && !wasPressed) || (!newPressedValue && wasPressed))
        {
            setPressed(newPressedValue);
            invalidate();
        }
        if (newPressedValue && action)
        {
            if (action->isValid())
            {
                action->execute(*this);
            }
        }
    }
};
} // namespace touchgfx

#endif // TOUCHBUTTONTRIGGER_HPP
