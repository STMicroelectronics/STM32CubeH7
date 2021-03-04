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

#ifndef CLICKBUTTONTRIGGER_HPP
#define CLICKBUTTONTRIGGER_HPP

#include <touchgfx/containers/buttons/AbstractButtonContainer.hpp>

namespace touchgfx
{
/**
 * @class ClickButtonTrigger ClickButtonTrigger.hpp touchgfx/containers/buttons/ClickButtonTrigger.hpp
 *
 * @brief A click button trigger.
 *
 *        A click button trigger. This trigger will create a button
 *        that reacts on clicks. This means it will call the action
 *        when it gets a touch released event.
 *
 *        The ClickButtonTrigger can be combined with one or more of
 *        the ButtonStyle classes to create a functional button.
 */
class ClickButtonTrigger : public AbstractButtonContainer
{
public:
    /**
     * @fn ClickButtonTrigger::ClickButtonTrigger()
     *
     * @brief Default constructor.
     */
    ClickButtonTrigger() :
        AbstractButtonContainer() { }

    /**
     * @fn virtual ClickButtonTrigger::~ClickButtonTrigger()
     *
     * @brief Destructor.
     */
    virtual ~ClickButtonTrigger() { }

    /**
     * @fn virtual void ClickButtonTrigger::handleClickEvent(const ClickEvent& event)
     *
     * @brief Handles the click event described by event.
     *
     *        Handles the click event described by event. The action
     *        callback is called when receiving a RELEASED event in
     *        PRESSED state.
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
        if (wasPressed && (event.getType() == ClickEvent::RELEASED) && action)
        {
            if (action->isValid())
            {
                action->execute(*this);
            }
        }
    }
};
} // namespace touchgfx

#endif // CLICKBUTTONTRIGGER_HPP
