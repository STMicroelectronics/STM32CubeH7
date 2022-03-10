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

#ifndef BUTTONS_HPP
#define BUTTONS_HPP

namespace touchgfx
{
/**
 * @class Buttons Buttons.hpp touchgfx/hal/Buttons.hpp
 *
 * @brief A buttons.
 */
class Buttons
{
public:

    /**
     * @fn static void Buttons::init();
     *
     * @brief Perform configuration of IO pins.
     *
     *        Perform configuration of IO pins.
     */
    static void init();

    /**
     * @fn static unsigned int Buttons::sample();
     *
     * @brief Sample button states.
     *
     *        Sample button states.
     *
     * @return the sampled state of the buttons.
     */
    static unsigned int sample();
};
}
#endif // BUTTONS_HPP
