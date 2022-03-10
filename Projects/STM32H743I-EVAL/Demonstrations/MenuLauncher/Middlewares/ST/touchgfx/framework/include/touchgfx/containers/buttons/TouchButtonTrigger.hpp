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
