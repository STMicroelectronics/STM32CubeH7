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

#ifndef TOGGLEBUTTONTRIGGER_HPP
#define TOGGLEBUTTONTRIGGER_HPP

#include <touchgfx/containers/buttons/AbstractButtonContainer.hpp>

namespace touchgfx
{
/**
 * @class ToggleButtonTrigger ToggleButtonTrigger.hpp touchgfx/containers/buttons/ToggleButtonTrigger.hpp
 *
 * @brief A toggle button trigger.
 *
 *        A toggle button trigger. This trigger will create a button
 *        that reacts on clicks. This means it will call the action
 *        when it gets a touch released event.
 *
 *        The ToggleButtonTrigger will stay in pressed state until it
 *        is clicked again.
 *
 *        The ToggleButtonTrigger can be combined with one or more of
 *        the ButtonStyle classes to create a functional button.
 *
 */
class ToggleButtonTrigger : public AbstractButtonContainer
{
public:
    /**
     * @fn ToggleButtonTrigger::ToggleButtonTrigger()
     *
     * @brief Default constructor.
     */
    ToggleButtonTrigger() :
        AbstractButtonContainer(), toggleCanceled(false) { }

    /**
     * @fn virtual ToggleButtonTrigger::~ToggleButtonTrigger()
     *
     * @brief Destructor.
     */
    virtual ~ToggleButtonTrigger() { }

    /**
     * @fn void ToggleButtonTrigger::forceState(bool activeState);
     *
     * @brief Force the button into a specific state.
     *
     *        Use this function to force the button in one of the two possible states. If
     *        button is forced to the active state, then AbstractButtonContainer will be in a pressed state.
     *
     * @param activeState If true, the AbstractButtonContainer will appear pressed.
     */
    void forceState(bool activeState)
    {
        AbstractButtonContainer::setPressed(activeState);
    }

    /**
     * @fn void ToggleButtonTrigger::setToggleCanceled(bool isToggleCanceled)
     *
     * @brief Sets toggle canceled.
     *
     * @param isToggleCanceled True if is toggle canceled, false if not.
     */
    void setToggleCanceled(bool isToggleCanceled)
    {
        toggleCanceled = isToggleCanceled;
    }

    /**
     * @fn bool ToggleButtonTrigger::getToggleCanceled()
     *
     * @brief Gets toggle canceled.
     *
     * @return True if it succeeds, false if it fails.
     */
    bool getToggleCanceled()
    {
        return toggleCanceled;
    }

    /**
     * @fn virtual void ToggleButtonTrigger::handleClickEvent(const ClickEvent& event)
     *
     * @brief Handles the click event described by event.
     *
     * @param event The event.
     */
    virtual void handleClickEvent(const ClickEvent& event)
    {
        bool wasPressed = getPressed();
        bool newPressedValue = !getPressed();
        bool toggleCanceled = getToggleCanceled();
        setToggleCanceled(event.getType() == ClickEvent::CANCEL);

        if (((newPressedValue && !wasPressed) || (!newPressedValue && wasPressed)) && (event.getType() != ClickEvent::RELEASED) && !toggleCanceled)
        {
            setPressed(newPressedValue);
            invalidate();
        }

        if (!toggleCanceled && (event.getType() == ClickEvent::RELEASED) && action)
        {
            if (action->isValid())
            {
                action->execute(*this);
            }
        }
    }
protected:
    bool toggleCanceled; ///< True if toggle canceled
};
} // namespace touchgfx

#endif // TOGGLEBUTTONTRIGGER_HPP
