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

#ifndef GPIO_HPP
#define GPIO_HPP

namespace touchgfx
{
/**
 * @class GPIO GPIO.hpp touchgfx/hal/GPIO.hpp
 *
 * @brief Interface class for manipulating GPIOs in order to do performance measurements on target.
 *
 *        Interface class for manipulating GPIOs in order to do performance measurements on
 *        target. Not used on the PC simulator.
 */
class GPIO
{
public:

    /**
     * @typedef enum GPIO_ID
     *
     * @brief Enum for the GPIOs used.
     *
     *        Enum for the GPIOs used.
     */
    typedef enum
    {
        VSYNC_FREQ,  /// Pin is toggled at each VSYNC
        RENDER_TIME, /// Pin is high when frame rendering begins, low when finished
        FRAME_RATE,  /// Pin is toggled when the frame buffers are swapped.
        MCU_ACTIVE   /// Pin is high when the MCU is doing work (i.e. not in idle task).
    } GPIO_ID;

    /**
     * @fn static void GPIO::init();
     *
     * @brief Perform configuration of IO pins.
     *
     *        Perform configuration of IO pins.
     */
    static void init();

    /**
     * @fn static void GPIO::set(GPIO_ID id);
     *
     * @brief Sets a pin high.
     *
     *        Sets a pin high.
     *
     * @param id the pin to set.
     */
    static void set(GPIO_ID id);

    /**
     * @fn static void GPIO::clear(GPIO_ID id);
     *
     * @brief Sets a pin low.
     *
     *        Sets a pin low.
     *
     * @param id the pin to set.
     */
    static void clear(GPIO_ID id);

    /**
     * @fn static void GPIO::toggle(GPIO_ID id);
     *
     * @brief Toggles a pin.
     *
     *        Toggles a pin.
     *
     * @param id the pin to toggle.
     */
    static void toggle(GPIO_ID id);

    /**
     * @fn static bool GPIO::get(GPIO_ID id);
     *
     * @brief Gets the state of a pin.
     *
     *        Gets the state of a pin.
     *
     * @param id the pin to get.
     *
     * @return true if the pin is high, false otherwise.
     */
    static bool get(GPIO_ID id);
}; // class GPIO
} // namespace touchgfx

#endif // GPIO_HPP
