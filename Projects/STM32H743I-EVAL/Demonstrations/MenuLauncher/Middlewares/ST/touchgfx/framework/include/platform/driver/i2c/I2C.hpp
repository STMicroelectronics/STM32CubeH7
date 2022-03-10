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

#ifndef I2C_HPP
#define I2C_HPP

#include <touchgfx/hal/Types.hpp>

namespace touchgfx
{
/**
 * @class I2C I2C.hpp platform/driver/i2c/I2C.hpp
 *
 * @brief Platform independent interface for I2C drivers.
 *
 *        Platform independent interface for I2C drivers.
 */
class I2C
{
public:
    /**
     * @fn I2C::I2C(uint8_t ch)
     *
     * @brief Stores the channel of the I2C bus to be configured.
     *
     *        Stores the channel of the I2C bus to be configured.
     *
     * @param ch I2C channel.
     */
    I2C(uint8_t ch):
        channel(ch)
    {
    }

    /**
     * @fn virtual I2C::~I2C()
     *
     * @brief Destructor.
     *
     *        Destructor.
     */
    virtual ~I2C()
    {
    }

    /**
     * @fn virtual void I2C::init() = 0;
     *
     * @brief Initializes the I2C driver.
     *
     *        Initializes the I2C driver.
     */
    virtual void init() = 0;

    /**
     * @fn virtual bool I2C::readRegister(uint8_t addr, uint8_t reg, uint8_t* data, uint32_t cnt) = 0;
     *
     * @brief Reads the specified register on the device with the specified address.
     *
     *        Reads the specified register on the device with the specified address.
     *
     * @param addr       The I2C device address.
     * @param reg        The register.
     * @param [out] data Pointer to buffer in which to place the result.
     * @param cnt        Size of buffer in bytes.
     *
     * @return true on success, false otherwise.
     */
    virtual bool readRegister(uint8_t addr, uint8_t reg, uint8_t* data, uint32_t cnt) = 0;

    /**
     * @fn virtual bool I2C::writeRegister(uint8_t addr, uint8_t reg, uint8_t val) = 0;
     *
     * @brief Writes the specified value in a register.
     *
     *        Writes the specified value in a register.
     *
     * @param addr The I2C device address.
     * @param reg  The register.
     * @param val  The new value.
     *
     * @return true on success, false otherwise.
     */
    virtual bool writeRegister(uint8_t addr, uint8_t reg, uint8_t val) = 0;

protected:
    uint8_t channel;    ///< I2c channel is stored in order to initialize and recover a specific I2C channel
};

} // namespace touchgfx

#endif // I2C_HPP
