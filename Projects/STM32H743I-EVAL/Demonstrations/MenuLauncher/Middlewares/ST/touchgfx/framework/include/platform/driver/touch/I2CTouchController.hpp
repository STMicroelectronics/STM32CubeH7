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

#ifndef I2CTOUCHCONTROLLER_HPP
#define I2CTOUCHCONTROLLER_HPP

#include <touchgfx/hal/Types.hpp>
#include <platform/driver/touch/TouchController.hpp>
#include <platform/driver/i2c/I2C.hpp>

namespace touchgfx
{
/**
 * @class I2CTouchController I2CTouchController.hpp platform/driver/touch/I2CTouchController.hpp
 *
 * @brief Specific I2C-enabled type of Touch Controller.
 *
 *        Specific I2C-enabled type of Touch Controller.
 *
 * @sa TouchController
 */
class I2CTouchController : public TouchController
{
public:
    /**
     * @fn I2CTouchController::I2CTouchController(I2C& i2c)
     *
     * @brief Constructor.
     *
     *        Constructor. Initializes I2C driver.
     *
     * @param [in,out] i2c I2C driver.
     */
    I2CTouchController(I2C& i2c):
        i2c(i2c)
    {
        i2c.init();
    }

    /**
     * @fn virtual I2CTouchController::~I2CTouchController()
     *
     * @brief Destructor.
     *
     *        Destructor.
     */
    virtual ~I2CTouchController()
    {
    }

    /**
     * @fn virtual void I2CTouchController::init() = 0;
     *
     * @brief Initializes touch controller.
     *
     *        Initializes touch controller.
     */
    virtual void init() = 0;

    /**
     * @fn virtual bool I2CTouchController::sampleTouch(int32_t& x, int32_t& y) = 0;
     *
     * @brief Checks whether the touch screen is being touched.
     *
     *        Checks whether the touch screen is being touched, and if so, what coordinates.
     *
     * @param [out] x The x position of the touch
     * @param [out] y The y position of the touch
     *
     * @return True if a touch has been detected, otherwise false.
     */
    virtual bool sampleTouch(int32_t& x, int32_t& y) = 0;

protected:
    I2C& i2c; ///< I2C driver
};

} // namespace touchgfx

#endif // I2CTOUCHCONTROLLER_HPP
