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

#ifndef TOUCHCONTROLLER_HPP
#define TOUCHCONTROLLER_HPP

#include <touchgfx/hal/Types.hpp>

namespace touchgfx
{
/**
 * @class TouchController TouchController.hpp platform/driver/touch/TouchController.hpp
 *
 * @brief Basic Touch Controller interface.
 *
 *        Basic Touch Controller interface.
 */
class TouchController
{
public:
    /**
     * @fn virtual TouchController::~TouchController()
     *
     * @brief Destructor.
     *
     *        Destructor.
     */
    virtual ~TouchController()
    {
    }

    /**
     * @fn virtual void TouchController::init() = 0;
     *
     * @brief Initializes touch controller.
     *
     *        Initializes touch controller.
     */
    virtual void init() = 0;

    /**
     * @fn virtual bool TouchController::sampleTouch(int32_t& x, int32_t& y) = 0;
     *
     * @brief Checks whether the touch screen is being touched, and if so, what coordinates.
     *
     *        Checks whether the touch screen is being touched, and if so, what coordinates.
     *
     * @param [out] x The x position of the touch
     * @param [out] y The y position of the touch
     *
     * @return True if a touch has been detected, otherwise false.
     */
    virtual bool sampleTouch(int32_t& x, int32_t& y) = 0;
};

} // namespace touchgfx

#endif // TOUCHCONTROLLER_HPP
