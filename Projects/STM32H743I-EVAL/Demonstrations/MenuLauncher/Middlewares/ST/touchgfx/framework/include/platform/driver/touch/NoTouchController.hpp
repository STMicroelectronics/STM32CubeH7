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

#ifndef NOTOUCHCONTROLLER_HPP
#define NOTOUCHCONTROLLER_HPP

#include <platform/driver/touch/TouchController.hpp>

namespace touchgfx
{
/**
 * Empty TouchController implementation which does nothing. Use this if your display does
 * not have touch input capabilities.
 */
class NoTouchController : public TouchController
{
public:
    /**
     * @fn virtual void NoTouchController::init()
     *
     * @brief Empty initialization.
     *
     *        Empty initialization.
     */
    virtual void init()
    {
    }

    /**
     * @fn virtual NoTouchController::~NoTouchController()
     *
     * @brief Destructor.
     *
     *        Destructor.
     */
    virtual ~NoTouchController()
    {
    }

    /**
     * @fn virtual bool NoTouchController::sampleTouch(int32_t& x, int32_t& y)
     *
     * @brief Does nothing.
     *
     * @param [out] x unused.
     * @param [out] y unused.
     *
     * @return false.
     */
    virtual bool sampleTouch(int32_t& x, int32_t& y)
    {
        return false;
    }
};

} // namespace touchgfx

#endif // NOTOUCHCONTROLLER_HPP
