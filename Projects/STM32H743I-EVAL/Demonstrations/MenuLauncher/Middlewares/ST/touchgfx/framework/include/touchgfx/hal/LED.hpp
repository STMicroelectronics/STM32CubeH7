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

#ifndef LED_HPP
#define LED_HPP

#include "lpc_types.h"
namespace touchgfx
{
/**
 * @class LED LED.hpp touchgfx/hal/LED.hpp
 *
 * @brief A led.
 */
class LED
{
public:

    /**
     * @fn static void LED::init();
     *
     * @brief Perform configuration of IO pins.
     *
     *        Perform configuration of IO pins.
     */
    static void init();

    /**
     * @fn static void LED::on(uint8_t nr);
     *
     * @brief Turn on a LED.
     *
     *        Turn on a LED.
     *
     * @param nr of the LED to switch on.
     */
    static void on(uint8_t nr);

    /**
     * @fn static void LED::off(uint8_t nr);
     *
     * @brief Turn off a LED.
     *
     *        Turn off a LED.
     *
     * @param nr of the LED to switch off.
     */
    static void off(uint8_t nr);

    /**
     * @fn static void LED::toggle(uint8_t nr);
     *
     * @brief Toggles a LED.
     *
     *        Toggles a LED.
     *
     * @param nr of the LED to toggle.
     */
    static void toggle(uint8_t nr);

    /**
     * @fn static bool LED::get(uint8_t nr);
     *
     * @brief Get state of a LED.
     *
     *        Get state of a LED.
     *
     * @param nr of the LED to toggle.
     *
     * @return the state of the LED.
     */
    static bool get(uint8_t nr);
};
}
#endif // LED_HPP
