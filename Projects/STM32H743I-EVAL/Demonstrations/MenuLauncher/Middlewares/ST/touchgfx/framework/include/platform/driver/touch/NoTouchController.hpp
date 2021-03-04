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
