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
