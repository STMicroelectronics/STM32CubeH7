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
