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

#ifndef BUTTONCONTROLLER_HPP
#define BUTTONCONTROLLER_HPP

#include <touchgfx/hal/Types.hpp>

namespace touchgfx
{
/**
 * @class ButtonController ButtonController.hpp platform/driver/button/ButtonController.hpp
 *
 * @brief Interface for sampling external key events.
 *
 *        Interface for sampling external key events.
 */
class ButtonController
{
public:
    /**
     * @fn virtual ButtonController::~ButtonController()
     *
     * @brief Destructor.
     *
     *        Destructor.
     */
    virtual ~ButtonController()
    {
    }

    /**
     * @fn virtual void ButtonController::init() = 0;
     *
     * @brief Initializes button controller.
     *
     *        Initializes button controller.
     */
    virtual void init() = 0;

    /**
     * @fn virtual bool ButtonController::sample(uint8_t& key) = 0;
     *
     * @brief Sample external key events.
     *
     *        Sample external key events.
     *
     * @param [in,out] key Output parameter that will be set to the key value if a keypress was
     *                     detected.
     *
     * @return True if a keypress was detected and the "key" parameter is set to a value.
     */
    virtual bool sample(uint8_t& key) = 0;

    /**
     * @fn virtual void reset()
     *
     * @brief Resets button controller.
     *
     *        Resets button controller. Does nothing in the default implementation.
     *
     */
    virtual void reset() {}
};

} // namespace touchgfx

#endif // BUTTONCONTROLLER_HPP
