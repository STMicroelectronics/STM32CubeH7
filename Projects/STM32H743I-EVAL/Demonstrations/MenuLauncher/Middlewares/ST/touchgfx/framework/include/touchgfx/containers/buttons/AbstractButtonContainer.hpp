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

#ifndef ABSTRACTBUTTONCONTAINER_HPP
#define ABSTRACTBUTTONCONTAINER_HPP

#include <touchgfx/containers/Container.hpp>

namespace touchgfx
{
/**
 * @class AbstractButtonContainer AbstractButtonContainer.hpp touchgfx/containers/buttons/AbstractButtonContainer.hpp
 *
 * @brief An abstract button container.
 *
 *        An abstract button container. It defines pressed/not pressed
 *        state, the alpha value, and the action Callback of a
 *        button. AbstractButtonContainer is used as superclass for
 *        classes defining a specific button behaviour.
 *
 * @see ClickButtonTrigger, RepeatButtonTrigger, ToggleButtonTrigger, TouchButtonTrigger
 */
class AbstractButtonContainer : public Container
{
public:
    /**
     * @fn AbstractButtonContainer::AbstractButtonContainer()
     *
     * @brief Default constructor.
     */
    AbstractButtonContainer() : pressed(false), alpha(255), action(0)
    {
        setTouchable(true);
    }

    /**
     * @fn virtual AbstractButtonContainer::~AbstractButtonContainer()
     *
     * @brief Destructor.
     */
    virtual ~AbstractButtonContainer() {}

    /**
     * @fn void AbstractButtonContainer::setPressed(bool isPressed)
     *
     * @brief Sets the pressed state.
     *
     * @param isPressed True if is pressed, false if not.
     */
    void setPressed(bool isPressed)
    {
        pressed = isPressed;
        handlePressedUpdated();
    }

    /**
     * @fn bool AbstractButtonContainer::getPressed()
     *
     * @brief Gets the pressed state.
     *
     * @return True if it succeeds, false if it fails.
     */
    bool getPressed()
    {
        return pressed;
    }

    /**
     * @fn void AbstractButtonContainer::setAlpha(uint8_t newAlpha)
     *
     * @brief Sets an alpha value.
     *
     * @param newAlpha The new alpha.
     */
    void setAlpha(uint8_t newAlpha)
    {
        alpha = newAlpha;
        handleAlphaUpdated();
    }

    /**
     * @fn uint8_t AbstractButtonContainer::getAlpha() const
     *
     * @brief Gets the alpha.
     *
     * @return The alpha value.
     */
    uint8_t getAlpha() const
    {
        return alpha;
    }

    /**
     * @fn void AbstractButtonContainer::setAction(GenericCallback< const AbstractButtonContainer& >& callback)
     *
     * @brief Sets an action callback.
     *
     * @param callback The callback.
     */
    void setAction(GenericCallback< const AbstractButtonContainer& >& callback)
    {
        action = &callback;
    }
protected:
    bool pressed;        ///< True if pressed
    uint8_t alpha;       ///< The current alpha value. 255 denotes solid, 0 denotes completely transparent.

    GenericCallback< const AbstractButtonContainer& >* action;  ///< The action

    /**
     * @fn virtual void AbstractButtonContainer::handlePressedUpdated()
     *
     * @brief Handles the pressed updated.
     */
    virtual void handlePressedUpdated() {}

    /**
     * @fn virtual void AbstractButtonContainer::handleAlphaUpdated()
     *
     * @brief Handles the alpha updated.
     */
    virtual void handleAlphaUpdated() {}
};
} // namespace touchgfx

#endif // ABSTRACTBUTTONCONTAINER_HPP
